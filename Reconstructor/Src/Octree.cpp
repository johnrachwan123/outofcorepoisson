/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/Octree.cpp $
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
#include "Tree.hpp"
#include "Octree.hpp"

#include <stdlib.h>
#include <math.h>
#include <algorithm>

CONST INT OctNode::DepthShift = 5;
CONST INT OctNode::OffsetShift = 19;
CONST INT OctNode::DepthMask = (1<<DepthShift)-1;
CONST INT OctNode::OffsetMask = (1<<OffsetShift)-1;
CONST INT OctNode::OffsetShift1 = DepthShift;
CONST INT OctNode::OffsetShift2 = OffsetShift1 + OffsetShift;
CONST INT OctNode::OffsetShift3 = OffsetShift2 + OffsetShift;

CriticalSection OctNode::m_Lock[512];

OctNode::OctNode()
{
  m_pParent = 0;
  m_pChildren = 0;
  m_Depth = 0;
  m_Offset[0] = m_Offset[1] = m_Offset[2] = 0;
  m_ChildrenIndex = (ULONGLONG)-1;

  for (UINT i=0; i<CONFIG_MAXTHREADS; i++)
  {
    m_WeightContribution[i] = 0.0;
    m_Value[i] = 0.0;
    m_CenterWeightContribution[i] = 0.0f;
    m_CustomAttribute[i] = InterpolatedAttributeEmpty;
    m_CustomAttributeWeightContribution[i] = 0.0f;
  }
}

OctNode::~OctNode()
{
  m_pParent = 0;
  m_pChildren = 0;
}

VOID OctNode::Validate()
{
  Assert(this != 0);

  /* If node has children, verify them */
  if (m_pChildren)
  {
    for (SIZE_T i=0; i<8; i++)
    {
      Assert(m_pChildren[i].Depth() == Depth()+1);
      Assert(m_pChildren[i].GetParent() == this);
      m_pChildren[i].Validate();
    }
  }

  if (m_pParent)
  {
    Assert(m_pParent->Depth() == Depth()-1);
    BOOL Found = FALSE;
    for (USHORT i=0; i<8; i++)
      Found |= (m_pParent->GetChild(i) == this);
    Assert(Found);
  }

}

BOOL OctNode::IsDescendant(OctNode* pNode)
{
  if (pNode == this)
    return TRUE;

  if (m_pChildren != 0)
  {
    for (SIZE_T i=0; i<8; i++)
      if (m_pChildren[i].IsDescendant(pNode))
        return TRUE;
  }

  return FALSE;
}

VOID OctNode::Index(SIZE_T Depth, CONST SIZE_T Offset[3], USHORT& d, USHORT off[3])
{
  d = (USHORT)Depth;

  off[0] = (USHORT)((SIZE_T)(1 << Depth) + Offset[0] - 1);
  off[1] = (USHORT)((SIZE_T)(1 << Depth) + Offset[1] - 1);
  off[2] = (USHORT)((SIZE_T)(1 << Depth) + Offset[2] - 1);
}


VOID OctNode::GetDepthAndOffset(INT& Depth, INT Offset[3]) CONST
{
  Depth = m_Depth;
  Offset[0] = (m_Offset[0] + 1) & (~(1 << Depth));
  Offset[1] = (m_Offset[1] + 1) & (~(1 << Depth));
  Offset[2] = (m_Offset[2] + 1) & (~(1 << Depth));
}

VOID OctNode::GetDepthAndOffset(USHORT& Depth, UINT Offset[3]) CONST
{
  Depth = m_Depth;
  Offset[0] = (m_Offset[0] + 1) & (~(1 << Depth));
  Offset[1] = (m_Offset[1] + 1) & (~(1 << Depth));
  Offset[2] = (m_Offset[2] + 1) & (~(1 << Depth));
}

/*

inline VOID OctNode::DepthAndOffset(const LONGLONG& index,INT& depth,INT offset[3])
{
	depth=INT(index&DepthMask);
	offset[0]=(INT((index>>OffsetShift1)&OffsetMask)+1)&(~(1<<depth));
	offset[1]=(INT((index>>OffsetShift2)&OffsetMask)+1)&(~(1<<depth));
	offset[2]=(INT((index>>OffsetShift3)&OffsetMask)+1)&(~(1<<depth));
}
*/

//
//inline INT OctNode::Depth(const LONGLONG& index){return INT(index&DepthMask);}

/*
template <class NodeData,class Real>
VOID OctNode<NodeData,Real>::centerAndWidth(Point3D<Real>& center,Real& width) const{
	INT depth,offset[3];
	depth=INT(d);
	offset[0]=(INT(off[0])+1)&(~(1<<depth));
	offset[1]=(INT(off[1])+1)&(~(1<<depth));
	offset[2]=(INT(off[2])+1)&(~(1<<depth));
	width=Real(1.0/(1<<depth));
	for(INT dim=0;dim<3;dim++){center.coords[dim]=Real(0.5+offset[dim])*width;}
}
*/

VOID OctNode::GetCenterAndWidth(Vector3<FLOAT>& Center, FLOAT& Width) CONST
{
  INT depth;
  INT offset[3];
  
  depth = INT(m_Depth);

  offset[0]=(INT(m_Offset[0])+1)&(~(1<<depth));
  offset[1]=(INT(m_Offset[1])+1)&(~(1<<depth));
  offset[2]=(INT(m_Offset[2])+1)&(~(1<<depth));
  
  Width = 1.0f / (1<<depth);
  
  for(INT i=0; i<3; i++)
    Center[i] = (0.5f + offset[i])*Width;
}

/*
template <class NodeData,class Real>
inline VOID OctNode<NodeData,Real>::CenterAndWidth(const LONGLONG& index,Point3D<Real>& center,Real& width){
	INT depth,offset[3];
	depth=index&DepthMask;
	offset[0]=(INT((index>>OffsetShift1)&OffsetMask)+1)&(~(1<<depth));
	offset[1]=(INT((index>>OffsetShift2)&OffsetMask)+1)&(~(1<<depth));
	offset[2]=(INT((index>>OffsetShift3)&OffsetMask)+1)&(~(1<<depth));
	width=Real(1.0/(1<<depth));
	for(INT dim=0;dim<3;dim++){center.coords[dim]=Real(0.5+offset[dim])*width;}
}
*/
/*
template <class NodeData,class Real>
INT OctNode<NodeData,Real>::maxDepth() const{
	if(!children){return 0;}
	else{
		INT c,d;
		for(INT i=0;i<Cube::CORNERS;i++){
			d=children[i].maxDepth();
			if(!i || d>c){c=d;}
		}
		return c+1;
	}
}
template <class NodeData,class Real>
INT OctNode<NodeData,Real>::nodes() const{
	if(!children){return 1;}
	else{
		INT c=0;
		for(INT i=0;i<Cube::CORNERS;i++){c+=children[i].nodes();}
		return c+1;
	}
}
template <class NodeData,class Real>
INT OctNode<NodeData,Real>::leaves() const{
	if(!children){return 1;}
	else{
		INT c=0;
		for(INT i=0;i<Cube::CORNERS;i++){c+=children[i].leaves();}
		return c;
	}
}
template<class NodeData,class Real>
INT OctNode<NodeData,Real>::maxDepthLeaves(INT maxDepth) const{
	if(depth()>maxDepth){return 0;}
	if(!children){return 1;}
	else{
		INT c=0;
		for(INT i=0;i<Cube::CORNERS;i++){c+=children[i].maxDepthLeaves(maxDepth);}
		return c;
	}
}
*/
/*
template <class NodeData,class Real>
const OctNode<NodeData,Real>* OctNode<NodeData,Real>::root() const{
	const OctNode* temp=this;
	while(temp->m_pParent){temp=temp->m_pParent;}
	return temp;
}


template <class NodeData,class Real>
const OctNode<NodeData,Real>* OctNode<NodeData,Real>::nextBranch(const OctNode* current) const{
	if(!current->m_pParent || current==this){return NULL;}
	if(current-current->m_pParent->m_pChildren==Cube::CORNERS-1){return nextBranch(current->m_pParent);}
	else{return current+1;}
}
template <class NodeData,class Real>
OctNode<NodeData,Real>* OctNode<NodeData,Real>::nextBranch(OctNode* current){
	if(!current->m_pParent || current==this){return NULL;}
	if(current-current->m_pParent->m_pChildren==Cube::CORNERS-1){return nextBranch(current->m_pParent);}
	else{return current+1;}
}
template <class NodeData,class Real>
const OctNode<NodeData,Real>* OctNode<NodeData,Real>::nextLeaf(const OctNode* current) const{
	if(!current){
		const OctNode<NodeData,Real>* temp=this;
		while(temp->m_pChildren){temp=&temp->m_pChildren[0];}
		return temp;
	}
	if(current->m_pChildren){return current->nextLeaf();}
	const OctNode* temp=nextBranch(current);
	if(!temp){return NULL;}
	else{return temp->nextLeaf();}
}
template <class NodeData,class Real>
OctNode<NodeData,Real>* OctNode<NodeData,Real>::nextLeaf(OctNode* current){
	if(!current){
		OctNode<NodeData,Real>* temp=this;
		while(temp->m_pChildren){temp=&temp->m_pChildren[0];}
		return temp;
	}
	if(current->m_pChildren){return current->nextLeaf();}
	OctNode* temp=nextBranch(current);
	if(!temp){return NULL;}
	else{return temp->nextLeaf();}
}

template <class NodeData,class Real>
const OctNode<NodeData,Real>* OctNode<NodeData,Real>::nextNode(const OctNode* current) const{
	if(!current){return this;}
	else if(current->m_pChildren){return &current->m_pChildren[0];}
	else{return nextBranch(current);}
}
template <class NodeData,class Real>
OctNode<NodeData,Real>* OctNode<NodeData,Real>::nextNode(OctNode* current){
	if(!current){return this;}
	else if(current->m_pChildren){return &current->m_pChildren[0];}
	else{return nextBranch(current);}
}
*/
/*
template<class NodeData,class Real>
INT OctNode<NodeData,Real>::CornerIndex(const Point3D<Real>& center,const Point3D<Real>& p){
	INT cIndex=0;
	if(p.coords[0]>center.coords[0]){cIndex|=1;}
	if(p.coords[1]>center.coords[1]){cIndex|=2;}
	if(p.coords[2]>center.coords[2]){cIndex|=4;}
	return cIndex;
}
*/


#if 0
//---------------------------------------------------------------------------------------------------

OctNode::Neighbours::Neighbours(SIZE_T Height)
{
  m_Height = Height;
	m_pNeighbours = new OctNode[27*m_Height];

	Clear();
}
//---------------------------------------------------------------------------------------------------

VOID OctNode::Neighbours::Clear()
{
	ZeroMemory(m_pNeighbours, sizeof(OctNode*)*27*m_Depth);
}
//---------------------------------------------------------------------------------------------------

VOID OctNode::Neighbours::ClearDepth(SIZE_T i)
{
	ZeroMemory(m_pNeighbours + i*27, sizeof(OctNode*)*27);
}
//---------------------------------------------------------------------------------------------------

OctNode::Neighbours::~Neighbours()
{
	if(m_pNeighbours)
    delete[] m_pNeighbours;
	m_pNeighbors = 0;
}
//---------------------------------------------------------------------------------------------------

OctNode* OctNode::Neighbours::operator() (SIZE_T Depth, SIZE_T i, SIZE_T j, SIZE_T k) CONST
{
	return GetNeighbour(Depth, i, j, k);
}
//---------------------------------------------------------------------------------------------------

OctNode* OctNode::Neighbours::GetNeighbour(SIZE_T Depth, SIZE_T i, SIZE_T j, SIZE_T k) CONST
{
	Assert(Depth < m_Height);
	Assert(i < 3);
	Assert(j < 3);
	Assert(k < 3);

	return m_pNeighbours[Depth*27 + 9*i + 3*j + k];
}
//---------------------------------------------------------------------------------------------------

#else

OctNode::Neighbors::Neighbors()
{
  Clear();
}

VOID OctNode::Neighbors::Clear()
{
  ZeroMemory(m_pNeighbors, sizeof(OctNode*)*27);
}

OctNode::NeighborCache::NeighborCache()
{
  m_pNeighbors = 0;
}

OctNode::NeighborCache::~NeighborCache()
{
  if(m_pNeighbors)
    delete[] m_pNeighbors;
  m_pNeighbors = 0;
}

VOID OctNode::NeighborCache::Initialize(USHORT Depth)
{
  if(m_pNeighbors)
    delete[] m_pNeighbors;
  m_pNeighbors = 0;

  m_Depth = Depth;
  m_pNeighbors = new Neighbors[Depth+1];
}

OctNode::Neighbors& OctNode::NeighborCache::ComputeNeighbors(OctNode* node)
{
  INT d=node->Depth();
  if(node!=m_pNeighbors[d].m_pNeighbors[1][1][1])
    for(INT i=d;i<=m_Depth;i++)
      m_pNeighbors[i].Clear();
  return InternalComputeNeighbors(node);
}

OctNode::Neighbors& OctNode::NeighborCache::InternalComputeNeighbors(OctNode* node)
{
  INT d=node->Depth();
  if(node!=m_pNeighbors[d].m_pNeighbors[1][1][1])
  {
    m_pNeighbors[d].Clear();

    if(!node->m_pParent)
    {
      m_pNeighbors[d].m_pNeighbors[1][1][1] = node;
    }
    else
    {
      Neighbors& temp = InternalComputeNeighbors(node->m_pParent);
      INT i,j,k;
      UINT x1,y1,z1;
      UINT x2,y2,z2;
      INT idx=INT(node-node->m_pParent->m_pChildren);
      Cube::FactorCornerIndex( idx   ,x1,y1,z1);
      Cube::FactorCornerIndex((~idx)&7,x2,y2,z2);
      for(i=0;i<2;i++)
      {
        for(j=0;j<2;j++)
        {
          for(k=0;k<2;k++)
          {
            m_pNeighbors[d].m_pNeighbors[x2+i][y2+j][z2+k]=&node->m_pParent->m_pChildren[Cube::CornerIndex(i,j,k)];
          }
        }
      }

      // Set the m_pNeighbors from across the faces
      i=x1<<1;
      if(temp.m_pNeighbors[i][1][1] && temp.m_pNeighbors[i][1][1]->m_pChildren){
        for(j=0;j<2;j++){for(k=0;k<2;k++){m_pNeighbors[d].m_pNeighbors[i][y2+j][z2+k]=&temp.m_pNeighbors[i][1][1]->m_pChildren[Cube::CornerIndex(x2,j,k)];}}
      }
      j=y1<<1;
      if(temp.m_pNeighbors[1][j][1] && temp.m_pNeighbors[1][j][1]->m_pChildren){
        for(i=0;i<2;i++){for(k=0;k<2;k++){m_pNeighbors[d].m_pNeighbors[x2+i][j][z2+k]=&temp.m_pNeighbors[1][j][1]->m_pChildren[Cube::CornerIndex(i,y2,k)];}}
      }
      k=z1<<1;
      if(temp.m_pNeighbors[1][1][k] && temp.m_pNeighbors[1][1][k]->m_pChildren){
        for(i=0;i<2;i++){for(j=0;j<2;j++){m_pNeighbors[d].m_pNeighbors[x2+i][y2+j][k]=&temp.m_pNeighbors[1][1][k]->m_pChildren[Cube::CornerIndex(i,j,z2)];}}
      }

      // Set the m_pNeighbors from across the edges
      i=x1<<1;	j=y1<<1;
      if(temp.m_pNeighbors[i][j][1] && temp.m_pNeighbors[i][j][1]->m_pChildren){
        for(k=0;k<2;k++){m_pNeighbors[d].m_pNeighbors[i][j][z2+k]=&temp.m_pNeighbors[i][j][1]->m_pChildren[Cube::CornerIndex(x2,y2,k)];}
      }
      i=x1<<1;	k=z1<<1;
      if(temp.m_pNeighbors[i][1][k] && temp.m_pNeighbors[i][1][k]->m_pChildren){
        for(j=0;j<2;j++){m_pNeighbors[d].m_pNeighbors[i][y2+j][k]=&temp.m_pNeighbors[i][1][k]->m_pChildren[Cube::CornerIndex(x2,j,z2)];}
      }
      j=y1<<1;	k=z1<<1;
      if(temp.m_pNeighbors[1][j][k] && temp.m_pNeighbors[1][j][k]->m_pChildren){
        for(i=0;i<2;i++){m_pNeighbors[d].m_pNeighbors[x2+i][j][k]=&temp.m_pNeighbors[1][j][k]->m_pChildren[Cube::CornerIndex(i,y2,z2)];}
      }

      // Set the neighbor from across the corner
      i=x1<<1;	j=y1<<1;	k=z1<<1;
      if(temp.m_pNeighbors[i][j][k] && temp.m_pNeighbors[i][j][k]->m_pChildren){ 
        m_pNeighbors[d].m_pNeighbors[i][j][k]=&temp.m_pNeighbors[i][j][k]->m_pChildren[Cube::CornerIndex(x2,y2,z2)];
      }
    }
  }
  return m_pNeighbors[node->Depth()];
}
#endif


/////////////////////////
// OctNodeNeighborCache2 //
/////////////////////////

OctNode::Neighbors2::Neighbors2()
{
  Clear();
}

VOID OctNode::Neighbors2::Clear()
{
  ZeroMemory(m_pNeighbors, sizeof(OctNode*)*125);
}


OctNode::NeighborCache2::NeighborCache2()
{
  m_pNeighbors=NULL;
}


OctNode::NeighborCache2::~NeighborCache2()
{
  if(m_pNeighbors){delete[] m_pNeighbors;}
  m_pNeighbors=NULL;
}


VOID OctNode::NeighborCache2::Initialize(USHORT d)
{
  if(m_pNeighbors){delete[] m_pNeighbors;}
  m_pNeighbors=NULL;
  m_Depth = d;
  m_pNeighbors=new Neighbors2[d+1];
}


OctNode::Neighbors2& OctNode::NeighborCache2::ComputeNeighbors(OctNode* node)
{
  INT d=node->Depth();
  if(node!=m_pNeighbors[d].m_pNeighbors[2][2][2]){
    for(INT i=d;i<=m_Depth;i++){m_pNeighbors[i].Clear();}
  }
  return InternalComputeNeighbors(node);
}


OctNode::Neighbors2& OctNode::NeighborCache2::InternalComputeNeighbors(OctNode* node)
{
  INT d=node->Depth();
  if(node!=m_pNeighbors[d].m_pNeighbors[2][2][2])
  {
    m_pNeighbors[d].Clear();

    if(!node->m_pParent){m_pNeighbors[d].m_pNeighbors[2][2][2]=node;}
    else{
      Neighbors2& temp = InternalComputeNeighbors(node->m_pParent);
      UINT x1,y1,z1;
      UINT x2,y2,z2;
      INT ii,jj,kk;
      INT idx=INT(node-node->m_pParent->m_pChildren);
      Cube::FactorCornerIndex(idx,x1,y1,z1);

      for(INT i=0;i<5;i++)
      {
        x2=i+x1;
        ii=x2&1;
        x2=1+(x2>>1);
        for(INT j=0;j<5;j++)
        {
          y2=j+y1;
          jj=y2&1;
          y2=1+(y2>>1);
          for(INT k=0;k<5;k++)
          {
            z2=k+z1;
            kk=z2&1;
            z2=1+(z2>>1);
            if(temp.m_pNeighbors[x2][y2][z2] && temp.m_pNeighbors[x2][y2][z2]->m_pChildren)
            {
              m_pNeighbors[d].m_pNeighbors[i][j][k]=&temp.m_pNeighbors[x2][y2][z2]->m_pChildren[Cube::CornerIndex(ii,jj,kk)];
            }
          }
        }
      }
    }
  }
  return m_pNeighbors[node->Depth()];
}



/*
template <class NodeData,class Real>
INT OctNode<NodeData,Real>::write(const char* fileName) const{
FILE* fp=fopen(fileName,"wb");
if(!fp){return 0;}
INT ret=write(fp);
fclose(fp);
return ret;
}
template <class NodeData,class Real>
INT OctNode<NodeData,Real>::write(FILE* fp) const{
fwrite(this,sizeof(OctNode<NodeData,Real>),1,fp);
if(children){for(INT i=0;i<Cube::CORNERS;i++){children[i].write(fp);}}
return 1;
}
template <class NodeData,class Real>
INT OctNode<NodeData,Real>::read(const char* fileName){
FILE* fp=fopen(fileName,"rb");
if(!fp){return 0;}
INT ret=read(fp);
fclose(fp);
return ret;
}
template <class NodeData,class Real>
INT OctNode<NodeData,Real>::read(FILE* fp){
fread(this,sizeof(OctNode<NodeData,Real>),1,fp);
parent=NULL;
if(children){
children=NULL;
initChildren();
for(INT i=0;i<Cube::CORNERS;i++){
children[i].read(fp);
children[i].parent=this;
}
}
return 1;
}
*/
/*
template<class NodeData,class Real>
INT OctNode<NodeData,Real>::width(INT maxDepth) const {
INT d=depth();
return 1<<(maxDepth-d); 
}
template<class NodeData,class Real>
VOID OctNode<NodeData,Real>::centerIndex(INT maxDepth,INT index[3]) const {
INT d,o[3];
depthAndOffset(d,o);
for(INT i=0;i<3;i++){index[i]=BinaryNode<Real>::CornerIndex(maxDepth,d+1,o[i]<<1,1);}
}
*/
