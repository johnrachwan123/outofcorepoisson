/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/MarchingCubes.hpp $
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

#pragma once

#include <vector>

class Square
{
public:
  const static INT CORNERS=4,EDGES=4;
  static INT CornerIndex(INT x, INT y);
  static VOID FactorCornerIndex(INT idx, INT& x, INT& y);
  static INT EdgeIndex(INT orientation, INT i);
  static VOID FactorEdgeIndex(INT idx, INT& orientation, INT& i);
  
  static INT ReflectCornerIndex(INT idx, INT edgeIndex);
  static INT ReflectEdgeIndex(INT idx, INT edgeIndex);
  
  static VOID EdgeCorners(INT idx, INT& c1, INT &c2);
  static VOID OrientedEdgeCorners(INT idx, INT& c1, INT &c2);
};

class Cube
{
public:
  const static INT CORNERS=8,EDGES=12,FACES=6;
  
  static INT CornerIndex(INT x, INT y, INT z);
  static VOID FactorCornerIndex(UINT idx, UINT& x, UINT& y, UINT& z);
  static INT EdgeIndex(INT Orientation, INT i, INT j);
  static VOID FactorEdgeIndex(UINT Index, INT& Orientation, UINT& i, UINT& j);
  static INT  FaceIndex(INT dir, INT offSet);
  static INT  FaceIndex(INT x, INT y, INT z);
  static VOID FactorFaceIndex(INT idx, INT& x, INT &y, INT& z);
  static VOID FactorFaceIndex(INT Index, INT& Direction, INT& Offset);
  
  static INT  AntipodalCornerIndex(INT idx);
  static INT  FaceReflectCornerIndex(INT idx, INT faceIndex);
  static INT  FaceReflectEdgeIndex(INT idx, INT faceIndex);
  static INT	FaceReflectFaceIndex(INT idx, INT faceIndex);
  static INT	EdgeReflectCornerIndex(INT idx, INT edgeIndex);
  static INT	EdgeReflectEdgeIndex(INT edgeIndex);
  
  static INT  FaceAdjacentToEdges(INT eIndex1, INT eIndex2);
  static VOID FacesAdjacentToEdge(INT eIndex, INT& f1Index, INT& f2Index);
  
  static VOID EdgeCorners(INT idx, INT& c1, INT &c2);
  static VOID FaceCorners(INT idx, INT& c1, INT &c2, INT& c3, INT& c4);
  
  static INT SquareToCubeCorner(INT fIndex, INT cIndex);
  static INT SquareToCubeEdge(INT fIndex, INT eIndex);
};

class MarchingSquares
{
public:
  class FaceEdges
  {
  public:
    INT count;
    std::pair<INT,INT> edge[2];
  };
private:
  static FaceEdges __caseTable	[1<<(Square::CORNERS )];
  static FaceEdges __fullCaseTable[1<<(Square::CORNERS+1)];
public:
  static VOID SetCaseTable();
  static VOID SetFullCaseTable();
  
  static const FaceEdges& caseTable(INT idx);
  static const FaceEdges& fullCaseTable(INT idx);
  static INT GetFullIndex(const double values[Square::CORNERS],DOUBLE iso);
  static INT GetFullIndex(const float values[Square::CORNERS],FLOAT iso);
  static INT GetIndex(const double values[Square::CORNERS],DOUBLE iso);
  static INT GetIndex(const float values[Square::CORNERS],FLOAT iso);
};

class MarchingCubes
{
  static VOID GetEdgeLoops(std::vector< std::pair<INT,INT> >& edges,std::vector< std::vector<INT> >& loops);
  static std::vector< std::vector<INT> > __caseTable[1<<Cube::CORNERS];
  static INT __fullCaseMap[1<<(Cube::CORNERS+Cube::FACES)];
  static std::vector< std::vector< std::vector<INT> > > __fullCaseTable;
public:
  static VOID SetCaseTable();
  static VOID SetFullCaseTable();
  
  static INT GetFullIndex(const double values[Cube::CORNERS],DOUBLE iso);
  static INT GetFullIndex(const float values[Cube::CORNERS],FLOAT iso);
  static const std::vector< std::vector<INT> >& caseTable(INT idx);
  static const std::vector< std::vector<INT> >& fullCaseTable(INT idx);
  static const std::vector< std::vector<INT> >& caseTable(INT idx,INT useFull);
  
  static INT IsAmbiguous(INT idx);
  static INT IsAmbiguous(INT idx,INT f);
  static INT GetIndex(const float values[Cube::CORNERS],FLOAT iso);
  static INT GetIndex(const double values[Cube::CORNERS],DOUBLE iso);
  static INT HasRoots(INT mcIndex);
  static INT HasEdgeRoots(INT mcIndex,INT edgeIndex);
  
};

