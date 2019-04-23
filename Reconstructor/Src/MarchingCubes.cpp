/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/MarchingCubes.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 910 $
 * Last Updated: $Date: 2008-12-29 12:23:49 -0800 (Mon, 29 Dec 2008) $
 * 
 * 
 * Copyright (c) 2006-2007, Matthew G Bolitho;  Michael Kazhdan
 * All rights reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer. Redistributions in binary form must reproduce
 * the above copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the distribution. 
 * 
 * Neither the name of the Johns Hopkins University nor the names of its contributors
 * may be used to endorse or promote products derived from this software without specific
 * prior written permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 ***************************************************************************************************/

#include "Config.hpp"

#include "MarchingCubes.hpp"

////////////
// Square //
////////////

INT Square::CornerIndex(INT x, INT y)
{
  return (y << 1) | x;
}

void Square::FactorCornerIndex(INT idx,INT& x,INT& y){
	x=(idx>>0)%2;
	y=(idx>>1)%2;
}
INT Square::EdgeIndex(INT orientation,INT i){
	switch(orientation){
		case 0: // x
			if(!i)	{return  0;} // (0,0) -> (1,0)
			else	{return  2;} // (0,1) -> (1,1)
		case 1: // y
			if(!i)	{return  3;} // (0,0) -> (0,1)
			else	{return  1;} // (1,0) -> (1,1)
	};
	return -1;
}
void Square::FactorEdgeIndex(INT idx,INT& orientation,INT& i){
	switch(idx){
		case 0: case 2:
			orientation=0;
			i=idx/2;
			return;
		case 1: case 3:
			orientation=1;
			i=((idx/2)+1)%2;
			return;
	};
}
void Square::EdgeCorners(INT idx,INT& c1,INT& c2){
	INT orientation,i;
	FactorEdgeIndex(idx,orientation,i);
	switch(orientation){
		case 0:
			c1=CornerIndex(0,i);
			c2=CornerIndex(1,i);
			break;
		case 1:
			c1=CornerIndex(i,0);
			c2=CornerIndex(i,1);
			break;
	};
}
void Square::OrientedEdgeCorners(INT idx,INT& c1,INT& c2){
	INT orientation,i;
	FactorEdgeIndex(idx,orientation,i);
	switch(orientation){
		case 0:
			c1=CornerIndex((i )&1,i);
			c2=CornerIndex((i+1)&1,i);
			break;
		case 1:
			c1=CornerIndex(i,(i+1)&1);
			c2=CornerIndex(i,(i )&1);
			break;
	};
}
INT Square::ReflectEdgeIndex(INT idx,INT edgeIndex){
	INT orientation=edgeIndex%2;
	INT o,i;
	FactorEdgeIndex(idx,o,i);
	if(o!=orientation){return idx;}
	else{return EdgeIndex(o,(i+1)%2);}
}
INT Square::ReflectCornerIndex(INT idx,INT edgeIndex){
	INT orientation=edgeIndex%2;
	INT x,y;
	FactorCornerIndex(idx,x,y);
	switch(orientation){
		case 0:	return CornerIndex((x+1)%2,y);
		case 1:	return CornerIndex(x,(y+1)%2);
	};
	return -1;
}



//////////
// Cube //
//////////
INT Cube::CornerIndex(INT x, INT y, INT z)
{
  return (z << 2) | (y << 1) | x;
}

VOID Cube::FactorCornerIndex(UINT Index, UINT& x, UINT& y, UINT& z)
{
	x = (Index >> 0) % 2;
	y = (Index >> 1) % 2;
	z = (Index >> 2) % 2;
}

INT Cube::EdgeIndex(INT Orientation, INT i, INT j)
{
  return (i | (j << 1)) | (Orientation << 2);
}

VOID Cube::FactorEdgeIndex(UINT Index, INT& Orientation, UINT& i, UINT& j)
{
	Orientation = Index >> 2;
	i = Index & 1;
	j = (Index & 2) >> 1;
}

INT Cube::FaceIndex(INT x, INT y, INT z)
{
	if (x < 0)
    return 0;
	else if	(x > 0)
    return 1;
	else if	(y < 0)
    return 2;
	else if	(y > 0)
    return 3;
	else if	(z < 0)
    return 4;
	else if	(z > 0)
    return 5;
	else
    return -1;
}

INT Cube::FaceIndex(INT dir,INT offSet){return (dir<<1)|offSet;}

void Cube::FactorFaceIndex(INT idx,INT& x,INT& y,INT& z){
	x=y=z=0;
	switch(idx){
		case 0:		x=-1;	break;
		case 1:		x= 1;	break;
		case 2:		y=-1;	break;
		case 3:		y= 1;	break;
		case 4:		z=-1;	break;
		case 5:		z= 1;	break;
	};
}

VOID Cube::FactorFaceIndex(INT Index, INT& Direction, INT& Offset)
{
	Direction = Index >> 1;
	Offset = Index & 1;
}

INT Cube::FaceAdjacentToEdges(INT eIndex1,INT eIndex2)
{
	INT f1,f2,g1,g2;
	FacesAdjacentToEdge(eIndex1,f1,f2);
	FacesAdjacentToEdge(eIndex2,g1,g2);
	if(f1==g1 || f1==g2){return f1;}
	if(f2==g1 || f2==g2){return f2;}
	return -1;
}

void Cube::FacesAdjacentToEdge(INT eIndex,INT& f1Index,INT& f2Index)
{
	INT Orientation;
  UINT i1;
  UINT i2;
	FactorEdgeIndex(eIndex, Orientation, i1, i2);
	i1 <<= 1;
	i2 <<= 1;
	i1--;
	i2--;

	switch(Orientation)
  {
		case 0:
			f1Index = FaceIndex(0, i1, 0);
			f2Index = FaceIndex(0, 0, i2);
			break;
		case 1:
			f1Index = FaceIndex(i1, 0, 0);
			f2Index = FaceIndex(0, 0, i2);
			break;
		case 2:
			f1Index = FaceIndex(i1, 0, 0);
			f2Index = FaceIndex(0, i2, 0);
			break;
	};
}
void Cube::EdgeCorners(INT idx,INT& c1,INT& c2){
	INT orientation;
  UINT i1;
  UINT i2;
	FactorEdgeIndex(idx,orientation,i1,i2);
	switch(orientation){
		case 0:
			c1=CornerIndex(0,i1,i2);
			c2=CornerIndex(1,i1,i2);
			break;
		case 1:
			c1=CornerIndex(i1,0,i2);
			c2=CornerIndex(i1,1,i2);
			break;
		case 2:
			c1=CornerIndex(i1,i2,0);
			c2=CornerIndex(i1,i2,1);
			break;
	};
}
void Cube::FaceCorners(INT idx,INT& c1,INT& c2,INT& c3,INT& c4){
	INT i=idx%2;
	switch(idx/2){
	case 0:
		c1=CornerIndex(i,0,0);
		c2=CornerIndex(i,1,0);
		c3=CornerIndex(i,0,1);
		c4=CornerIndex(i,1,1);
		return;
	case 1:
		c1=CornerIndex(0,i,0);
		c2=CornerIndex(1,i,0);
		c3=CornerIndex(0,i,1);
		c4=CornerIndex(1,i,1);
		return;
	case 2:
		c1=CornerIndex(0,0,i);
		c2=CornerIndex(1,0,i);
		c3=CornerIndex(0,1,i);
		c4=CornerIndex(1,1,i);
		return;
	}
}

INT Cube::AntipodalCornerIndex(INT idx)
{
	UINT x,y,z;
	FactorCornerIndex(idx,x,y,z);
	return CornerIndex((x+1)%2,(y+1)%2,(z+1)%2);
}
INT Cube::FaceReflectFaceIndex(INT idx,INT faceIndex){
	if(idx/2!=faceIndex/2){return idx;}
	else{
		if(idx%2)	{return idx-1;}
		else		{return idx+1;}
	}
}
INT Cube::FaceReflectEdgeIndex(INT idx,INT faceIndex){
	INT orientation=faceIndex/2;
	INT o;
  UINT i;
  UINT j;

	FactorEdgeIndex(idx,o,i,j);
	if(o==orientation){return idx;}
	switch(orientation){
		case 0:	return EdgeIndex(o,(i+1)%2,j);
		case 1:
			switch(o){
				case 0:	return EdgeIndex(o,(i+1)%2,j);
				case 2:	return EdgeIndex(o,i,(j+1)%2);
			};
		case 2:	return EdgeIndex(o,i,(j+1)%2);
	};
	return -1;
}
INT Cube::FaceReflectCornerIndex(INT idx,INT faceIndex)
{
  INT orientation = faceIndex / 2;
	UINT x,y,z;
	FactorCornerIndex(idx,x,y,z);
	switch(orientation)
  {
		case 0:	return CornerIndex((x+1)%2,y,z);
		case 1:	return CornerIndex(x,(y+1)%2,z);
		case 2: return CornerIndex(x,y,(z+1)%2);
	};
	return -1;
}

INT Cube::EdgeReflectCornerIndex(INT idx,INT edgeIndex)
{
	INT orientation;
  UINT x;
  UINT y;
  UINT z;
	FactorEdgeIndex(edgeIndex,orientation,x,y);
	FactorCornerIndex(idx,x,y,z);
	switch(orientation){
		case 0:	return CornerIndex(x     ,(y+1)%2,(z+1)%2);
		case 1:	return CornerIndex((x+1)%2, y     ,(z+1)%2);
		case 2:	return CornerIndex((x+1)%2,(y+1)%2, z    );
	};
	return -1;
}
INT	Cube::EdgeReflectEdgeIndex(INT edgeIndex)
{
	INT o;
  UINT i1,i2;
	FactorEdgeIndex(edgeIndex,o,i1,i2);
	return Cube::EdgeIndex(o,(i1+1)%2,(i2+1)%2);
}

INT Cube::SquareToCubeCorner(INT fIndex,INT cIndex)
{
	// Assuming that the offset is 0, this returns corners in a consistent orientation
	INT dir,off;
  INT i1,i2;
	FactorFaceIndex(fIndex,dir,off);
	Square::FactorCornerIndex(cIndex,i1,i2);
	switch(dir)
	{
	case 0:
		return CornerIndex(off,i1,i2);
	case 1:
		return CornerIndex(i1,off,(i2+1)&1);
	case 2:
		return CornerIndex(i1,i2,off);
	}
	return -1;
}
INT Cube::SquareToCubeEdge(INT fIndex,INT eIndex)
{
	// Assuming that the offset is 0, this returns corners in a consistent orientation
	INT dir,off;
  INT o,i;
	FactorFaceIndex(fIndex,dir,off);
	Square::FactorEdgeIndex(eIndex,o,i);
	switch(dir)
	{
	case 0:
		if(o==0)
			return EdgeIndex(1,off,i);
		else if(o==1)
			return EdgeIndex(2,off,i);
		else
			return -1;
	case 1:
		if(o==0)
			return EdgeIndex(0,off,(i+1)&1);
		else if(o==1)
			return EdgeIndex(2,i,off);
		else
			return -1;
	case 2:
		if(o==0)
			return EdgeIndex(0,i,off);
		else if(o==1)
			return EdgeIndex(1,i,off);
		else
			return -1;
	}
	return -1;
}

/////////////////////
// MarchingSquares //
/////////////////////
INT MarchingSquares::GetIndex(const double v[Square::CORNERS],DOUBLE iso){
	INT idx=0;
	for(INT i=0;i<Square::CORNERS;i++){if(v[i]<iso){idx|=(1<<i);}}
	return idx;
}
INT MarchingSquares::GetIndex(const float v[Square::CORNERS],FLOAT iso){
	INT idx=0;
	for(INT i=0;i<Square::CORNERS;i++){if(v[i]<iso){idx|=(1<<i);}}
	return idx;
}
INT MarchingSquares::GetFullIndex(const double v[Square::CORNERS],DOUBLE iso)
{
	INT idx=0;
	double sum=0;
	// Corner Index
	for(INT i=0;i<Square::CORNERS;i++)
	{
		if(v[i]<iso){idx|=(1<<i);}
		sum+=v[i];
	}
	// Face Index
	if(sum<(iso*4))	idx|=1<<Square::CORNERS;
	return idx;
}
INT MarchingSquares::GetFullIndex(const float v[Square::CORNERS],FLOAT iso)
{
	INT idx=0;
	float sum=0;
	// Corner Index
	for(INT i=0;i<Square::CORNERS;i++)
	{
		if(v[i]<iso){idx|=(1<<i);}
		sum+=v[i];
	}
	// Face Index
	if(sum<(iso*4))	idx|=1<<Square::CORNERS;
	return idx;
}

MarchingSquares::FaceEdges MarchingSquares::__caseTable[1<<(Square::CORNERS)];
MarchingSquares::FaceEdges MarchingSquares::__fullCaseTable[1<<(Square::CORNERS+1)];
void MarchingSquares::SetCaseTable()
{
	for(INT idx=0;idx<(1<<Square::CORNERS);idx++)
	{
		INT c1,c2;
		__caseTable[idx].count=0;
		for(INT i=0;i<Square::EDGES;i++)
		{
			Square::OrientedEdgeCorners(i,c1,c2);
			if(!(idx&(1<<c1)) && (idx&(1<<c2)))
				__caseTable[idx].edge[__caseTable[idx].count++].first=i;
		}
		__caseTable[idx].count=0;
		for(INT i=0;i<Square::EDGES;i++)
		{
			Square::OrientedEdgeCorners(i,c1,c2);
			if((idx&(1<<c1)) && !(idx&(1<<c2)))
				__caseTable[idx].edge[__caseTable[idx].count++].second=i;
		}
	}
}
void MarchingSquares::SetFullCaseTable()
{
	INT off=1<<Square::CORNERS;
	SetCaseTable();
	for(INT idx=0;idx<(1<<Square::CORNERS);idx++)
	{
		__fullCaseTable[idx]=__fullCaseTable[idx|off]=__caseTable[idx];
		if(__caseTable[idx].count==2)
		{
			INT c;	// A corner that's clipped off
			INT centerValue;
			INT c1,c2,d1,d2;
			Square::EdgeCorners(__caseTable[idx].edge[0].first,c1,c2);
			Square::EdgeCorners(__caseTable[idx].edge[0].second,d1,d2);
			if(c1==d1 || c1==d2)
				c=c1;
			else
				c=c2;
			if(idx & (1<<c))
				centerValue=1;
			else
				centerValue=0;
			centerValue<<=Square::CORNERS;
			__fullCaseTable[idx|centerValue].edge[0].first=__caseTable[idx].edge[0].first;
			__fullCaseTable[idx|centerValue].edge[1].first=__caseTable[idx].edge[1].first;
			__fullCaseTable[idx|centerValue].edge[1].second=__caseTable[idx].edge[0].second;
			__fullCaseTable[idx|centerValue].edge[0].second=__caseTable[idx].edge[1].second;
		}
	}
}
const MarchingSquares::FaceEdges& MarchingSquares::caseTable(INT idx)
{
	return __caseTable[idx];
}
const MarchingSquares::FaceEdges& MarchingSquares::fullCaseTable(INT idx)
{
	return __fullCaseTable[idx];
}

///////////////////
// MarchingCubes //
///////////////////
INT MarchingCubes::GetIndex(const double v[Cube::CORNERS],DOUBLE iso){
	INT idx=0;
	for(INT i=0;i<Cube::CORNERS;i++)
		if(v[i]<iso)
			idx|=1<<i;
	return idx;
}
INT MarchingCubes::HasRoots(INT mcIndex){
	if(mcIndex==0 || mcIndex==255){return 0;}
	else{return 1;}
}
INT MarchingCubes::GetIndex(const float v[Cube::CORNERS],FLOAT iso){
	INT idx=0;
	for(INT i=0;i<Cube::CORNERS;i++)
		if(v[i]<iso)
			idx|=1<<i;
	return idx;
}
INT MarchingCubes::HasEdgeRoots(INT mcIndex,INT edgeIndex){
	INT c1,c2;
	Cube::EdgeCorners(edgeIndex,c1,c2);
	if(((mcIndex&(1<<c1)) && !(mcIndex&(1<<c2))) || (!(mcIndex&(1<<c1)) && (mcIndex&(1<<c2))))
		return 1;
	else
		return 0;
}
void MarchingCubes::GetEdgeLoops(std::vector< std::pair<INT,INT> >& edges,std::vector< std::vector<INT> >& loops)
{
	INT loopSize=0;
	INT idx;
	std::pair<INT,INT> e,temp;
	loops.clear();
	while(edges.size())
	{
		e=edges[0];
		loops.resize(loopSize+1);
		edges[0]=edges[edges.size()-1];
		edges.pop_back();
		loops[loopSize].push_back(e.first);
		idx=e.second;
		for(INT j=INT(edges.size())-1;j>=0;j--){
			if(edges[j].first==idx || edges[j].second==idx){
				if(edges[j].first==idx)	{temp=edges[j];}
				else					{temp.first=edges[j].second;temp.second=edges[j].first;}
				idx=temp.second;
				loops[loopSize].push_back(temp.first);
				edges[j]=edges[edges.size()-1];
				edges.pop_back();
				j=INT(edges.size());
			}
		}
		loopSize++;
	}
}
std::vector< std::vector<INT> > MarchingCubes::__caseTable[1<<Cube::CORNERS];

INT MarchingCubes::GetFullIndex(const double values[Cube::CORNERS],DOUBLE iso)
{
	INT idx=0;
	INT c1,c2,c3,c4;
	for(INT i=0;i<Cube::CORNERS;i++)
		if(values[i]<iso)
			idx|=1<<i;
	if(!idx)
		return idx;
	if(idx==255)
		return idx|(63<<Cube::CORNERS);

	for(INT i=0;i<Cube::FACES;i++)
	{
		Cube::FaceCorners(i,c1,c2,c3,c4);
		double temp=values[c1]+values[c2]+values[c3]+values[c4];
		if(temp<iso*4)
			idx|=1<<(Cube::CORNERS+i);
	}
	return idx;
}
INT MarchingCubes::GetFullIndex(const float values[Cube::CORNERS],FLOAT iso)
{
	INT idx=0;
	INT c1,c2,c3,c4;
	for(INT i=0;i<Cube::CORNERS;i++)
		if(values[i]<iso)
			idx|=1<<i;
	if(!idx)
		return idx;
	if(idx==255)
		return idx|(63<<Cube::CORNERS);

	for(INT i=0;i<Cube::FACES;i++)
	{
		Cube::FaceCorners(i,c1,c2,c3,c4);
		float temp=values[c1]+values[c2]+values[c3]+values[c4];
		if(temp<iso*4)
			idx|=1<<(Cube::CORNERS+i);
	}
	return idx;
}

const std::vector< std::vector<INT> >& MarchingCubes::caseTable(INT idx)
{
	return __caseTable[idx];
}

void MarchingCubes::SetCaseTable()
{
	MarchingSquares::SetCaseTable();
	INT dir,off;
	for(INT idx=0; idx<(1<<Cube::CORNERS); idx++)
	{
		std::vector<std::pair<INT,INT> > edges;
		for(INT f=0; f<Cube::FACES; f++)
		{
			INT fIdx=0;
			Cube::FactorFaceIndex(f,dir,off);
			for(INT fc=0;fc<Square::CORNERS;fc++)
				if(idx&(1<<Cube::SquareToCubeCorner(f,fc)))
					fIdx|=1<<fc;

			for(INT i=0;i<MarchingSquares::caseTable(fIdx).count;i++)
				if(off)
					edges.push_back(std::pair<INT,INT>(Cube::SquareToCubeEdge(f,MarchingSquares::caseTable(fIdx).edge[i].first),Cube::SquareToCubeEdge(f,MarchingSquares::caseTable(fIdx).edge[i].second)));
				else
					edges.push_back(std::pair<INT,INT>(Cube::SquareToCubeEdge(f,MarchingSquares::caseTable(fIdx).edge[i].second),Cube::SquareToCubeEdge(f,MarchingSquares::caseTable(fIdx).edge[i].first)));
		}
		__caseTable[idx].clear();
		GetEdgeLoops(edges,__caseTable[idx]);
	}
}

INT MarchingCubes::__fullCaseMap[1<<(Cube::CORNERS+Cube::FACES)];
std::vector< std::vector< std::vector<INT> > > MarchingCubes::__fullCaseTable;

const std::vector< std::vector <INT> >& MarchingCubes::caseTable(INT idx,INT useFull)
{
	if(useFull)
		return __fullCaseTable[__fullCaseMap[idx] ];
	else
		return __caseTable[idx];
}
const std::vector< std::vector<INT> >& MarchingCubes::fullCaseTable(INT idx)
{
	return __fullCaseTable[__fullCaseMap[idx] ];
}
void MarchingCubes::SetFullCaseTable()
{
	MarchingSquares::SetFullCaseTable();
	INT dir,off;
  INT tSize=0;

	memset(__fullCaseMap,-1,sizeof(INT)*(1<<(Cube::CORNERS+Cube::FACES)));
	for(INT idx=0;idx<(1<<Cube::CORNERS);idx++)
	{
		INT tCount=1;
		for(INT f=0;f<Cube::FACES;f++)
		{
			INT fIdx=0;
			Cube::FactorFaceIndex(f,dir,off);
			for(INT fc=0;fc<Square::CORNERS;fc++)
				if(idx&(1<<Cube::SquareToCubeCorner(f,fc)))
					fIdx|=1<<fc;

			if(MarchingSquares::fullCaseTable(fIdx).count==2)
				tCount<<=1;
		}
		tSize+=tCount;
	}
	__fullCaseTable.resize(tSize);

	tSize=0;
	for(INT idx=0;idx<(1<<Cube::CORNERS);idx++)
	{
		INT aCount=0,uaCount=0;
		INT aFaces[Cube::FACES],uaFaces[Cube::FACES];
		
		for(INT f=0;f<Cube::FACES;f++)
		{
			INT fIdx=0;
			Cube::FactorFaceIndex(f,dir,off);
			for(INT fc=0;fc<Square::CORNERS;fc++)
				if(idx&(1<<Cube::SquareToCubeCorner(f,fc)))
					fIdx|=1<<fc;

			if(MarchingSquares::fullCaseTable(fIdx).count==2)
			{
				aFaces[aCount]=f;
				aCount++;
			}
			else
			{
				uaFaces[uaCount]=f;
				uaCount++;
			}
		}

		for(INT aIndex=0;aIndex<(1<<aCount);aIndex++)
		{
			std::vector<std::pair<INT,INT> > edges;
			INT aFlag=0;
			for(INT i=0;i<aCount;i++)
				if(aIndex&(1<<i))
					aFlag|=1<<aFaces[i];

			for(INT f=0;f<Cube::FACES;f++)
			{
				INT fIdx=0;
				if(aFlag & (1<<f))
					fIdx|=1<<Square::CORNERS;

				Cube::FactorFaceIndex(f,dir,off);
				for(INT fc=0;fc<Square::CORNERS;fc++)
					if(idx&(1<<Cube::SquareToCubeCorner(f,fc)))
						fIdx|=1<<fc;
				for(INT i=0;i<MarchingSquares::fullCaseTable(fIdx).count;i++)
					if(off)
						edges.push_back(std::pair<INT,INT>(Cube::SquareToCubeEdge(f,MarchingSquares::fullCaseTable(fIdx).edge[i].first),Cube::SquareToCubeEdge(f,MarchingSquares::fullCaseTable(fIdx).edge[i].second)));
					else
						edges.push_back(std::pair<INT,INT>(Cube::SquareToCubeEdge(f,MarchingSquares::fullCaseTable(fIdx).edge[i].second),Cube::SquareToCubeEdge(f,MarchingSquares::fullCaseTable(fIdx).edge[i].first)));
			}
			for(INT uaIndex=0;uaIndex<(1<<uaCount);uaIndex++)
			{
				INT uaFlag=0;
				for(INT i=0;i<uaCount;i++)
					if(uaIndex&(1<<i))
						uaFlag|=1<<uaFaces[i];

				__fullCaseMap[idx|((aFlag|uaFlag)<<Cube::CORNERS)]=tSize;
			}
			__fullCaseTable[tSize].clear();
			GetEdgeLoops(edges,__fullCaseTable[tSize]);
			tSize++;
		}
	}
}
INT MarchingCubes::IsAmbiguous(INT idx)
{
	for(INT f=0;f<Cube::FACES;f++)
	{
		INT fIdx=0;
		for(INT fc=0;fc<Square::CORNERS;fc++)
			if(idx&(1<<Cube::SquareToCubeCorner(f,fc)))
				fIdx|=1<<fc;

		if(MarchingSquares::caseTable(fIdx).count==2)
			return 1;
	}
	return 0;
}
INT MarchingCubes::IsAmbiguous(INT idx,INT f)
{
	INT fIdx=0;
	for(INT fc=0;fc<Square::CORNERS;fc++)
		if(idx&(1<<Cube::SquareToCubeCorner(f,fc)))
			fIdx|=1<<fc;

	if(MarchingSquares::caseTable(fIdx).count==2)
		return 1;
	return 0;
}
