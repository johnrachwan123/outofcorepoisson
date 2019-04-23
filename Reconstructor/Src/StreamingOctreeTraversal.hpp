/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/StreamingOctreeTraversal.hpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 815 $
 * Last Updated: $Date: 2008-10-15 08:26:37 -0700 (Wed, 15 Oct 2008) $
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

#include "Utility/Bits.hpp"
using Bolitho::Interleave;
using Bolitho::DeInterleave;

#include "MarchingCubes.hpp"
#include "BinaryNode.hpp"
#include "Tree.hpp"


typedef ULONGLONG KEY;
typedef KEY EDGEKEY;
typedef KEY CORNERKEY;
typedef KEY FACEKEY;
typedef KEY CENTERKEY;

////////////////
// VertexData //
////////////////
class VertexData
{
public:
  static KEY IndexToKey(CONST UINT Index[3])
  {
    //return (LONGLONG)Interleave((UINT16)Index[2], (UINT16)Index[1], (UINT16)Index[0]);
    //return (LONGLONG)(Index[0]) | (LONGLONG)(Index[1]) << 15 | (LONGLONG)(Index[2]) << 30;
    //return (LONGLONG)(Index[2]) | (LONGLONG)(Index[1]) << 15 | (LONGLONG)(Index[0]) << 30;

    return (ULONGLONG)Index[0] << 32 | Interleave((UINT16)Index[1], (UINT16)Index[2]);
  }

  static VOID KeyToIndex(KEY Key, UINT Index[3])
  {
    //Index[2] = (SIZE_T)(Key & MASK);
    //Index[1] = (SIZE_T)((Key >> 15) & MASK);
    //Index[0] = (SIZE_T)((Key >> 30) & MASK);

    Index[0] = Key >> 32;
    UINT16 A, B;
    DeInterleave((UINT32)(Key & 0xffffffff), A, B);

    Index[1] = A;
    Index[2] = B;
  }

public:

  static INT CommonEdgeDepth(EDGEKEY Key1, EDGEKEY Key2, USHORT MaxDepth)
  {
    UINT Offset1[3];
    UINT Offset2[3];
    UINT EdgeIndex1;
    UINT EdgeIndex2;
    USHORT Depth1;
    USHORT Depth2;

    INT Orientation1;
    INT Orientation2;
    UINT i1,i2, j1,j2;

    FactorEdgeKey(MaxDepth, Key1, Depth1, Offset1, EdgeIndex1);
    FactorEdgeKey(MaxDepth, Key2, Depth2, Offset2, EdgeIndex2);

    Cube::FactorEdgeIndex(EdgeIndex1, Orientation1, i1, j1);
    Cube::FactorEdgeIndex(EdgeIndex2, Orientation2, i2, j2);

    // If the orientations differ, then there is no common parent
    if(Orientation1 != Orientation2)
      return -1;

    while(Depth1 < Depth2)
    {
      // If the edge does not exist in the parent, there is no common parent edge
      if(Orientation1 == 0)
      {
        if(i2 != (Offset2[1] & 1) || j2 != (Offset2[2] & 1))
          return -1;
      }
      else if(Orientation1 == 1)
      {
        if(i2 != (Offset2[0] & 1) || j2 != (Offset2[2] & 1))
          return -1;
      }
      else if(Orientation1 == 2)
      {
        if(i2 != (Offset2[0] & 1) || j2 != (Offset2[1] & 1))
          return -1;
      }
      Depth2--;
      Offset2[0] >>= 1;
      Offset2[1] >>= 1;
      Offset2[2] >>= 1;
    }

    while(Depth2 < Depth1)
    {
      // If the edge does not exist in the parent, there is no common parent edge
      if(Orientation1 == 0)
      {
        if(i1 != (Offset1[1] & 1) || j1 != (Offset1[2] & 1))
          return -1;
      }
      else if(Orientation1 == 1)
      {
        if(i1 != (Offset1[0] & 1) || j1 != (Offset1[2] & 1))
          return -1;
      }
      else if(Orientation1 == 2)
      {
        if(i1 != (Offset1[0] & 1) || j1 != (Offset1[1] & 1))
          return -1;
      }
      Depth1--;
      Offset1[0] >>= 1;
      Offset1[1] >>= 1;
      Offset1[2] >>= 1;
    }

    if(Orientation1 == 0)
    {
      if(Offset1[1] != Offset2[1] || Offset1[2] != Offset2[2])
        return -1;
    }
    else if(Orientation1 == 1)
    {
      if(Offset1[0] != Offset2[0] || Offset1[2] != Offset2[2])
        return -1;
    }
    else if(Orientation1 == 2)
    {
      if(Offset1[0] != Offset2[0] || Offset1[1] != Offset2[1])
        return -1;
    }

    while(Depth1 >= 0)
    {
      if(Offset1[Orientation1] == Offset2[Orientation1])
        return Depth1;

      if(Orientation1 == 0)
      {
        if(i1 != (Offset1[1] & 1) || j1 != (Offset1[2] & 1) || i2 != (Offset2[1] & 1) || j2 != (Offset2[1] & 1))
          return -1;
      }
      else if(Orientation1 == 1)
      {
        if(i1 != (Offset1[0] & 1) || j1 != (Offset1[2] & 1) || i2 != (Offset2[0] & 1) || j2 != (Offset2[2] & 1))
          return -1;
      }
      else if(Orientation1 == 2)
      {
        if(i1 != (Offset1[0] & 1) || j1 != (Offset1[1] & 1) || i2 != (Offset2[0] & 1) || j2 != (Offset2[1] & 1))
          return -1;
      }

      Depth1--;
      Offset1[0] >>= 1;
      Offset2[0] >>= 1;
      Offset1[1] >>= 1;
      Offset2[1] >>= 1;
      Offset1[2] >>= 1;
      Offset2[2] >>= 1;
    }

    return -1;
  }

  static VOID FactorEdgeKey(USHORT MaxDepth, EDGEKEY Key, USHORT& Depth, UINT Offset[3], UINT& EdgeIndex)
  {
    UINT Index[3];
    INT o;
    KeyToIndex(Key, Index);

    Depth = MaxDepth;
    while(!(Index[0] & 1) && !(Index[1] & 1) && !(Index[2] & 1))
    {
      Index[0]>>=1;
      Index[1]>>=1;
      Index[2]>>=1;
      Depth--;
    }

    if(Index[0] & 1)
      o = 0;
    else if(Index[1] & 1)
      o = 1;
    else if(Index[2] & 1)
      o = 2;
    else
      TraceError(0, "Could not factor edge key\n");

    Offset[0] = Index[0] >> 1;
    Offset[1] = Index[1] >> 1;
    Offset[2] = Index[2] >> 1;

    EdgeIndex = Cube::EdgeIndex(o,0,0);
  }

  static void FactorEdgeKey(USHORT Depth, USHORT MaxDepth, EDGEKEY Key, UINT Offset[3], UINT& EdgeIndex)
  {
    UINT Index[3];
    INT o;
    KeyToIndex(Key, Index);

    Index[0] >>= (MaxDepth - Depth);
    Index[1] >>= (MaxDepth - Depth);
    Index[2] >>= (MaxDepth - Depth);

    if(Index[0] & 1)
      o = 0;
    else if(Index[1] & 1)
      o = 1;
    else if(Index[2] & 1)
      o = 2;
    else
      TraceError(0, "Could not factor edge key\n");

    Offset[0] = Index[0] >> 1;
    Offset[1] = Index[1] >> 1;
    Offset[2] = Index[2] >> 1;

    EdgeIndex = Cube::EdgeIndex(o,0,0);
  }

  static EDGEKEY EdgeKey(USHORT Depth, CONST UINT Offset[3], UINT EdgeIndex, USHORT MaxDepth, UINT Index[3])
  {
    INT o;
    UINT i1;
    UINT i2;
    
    Cube::FactorEdgeIndex(EdgeIndex, o, i1, i2);
    switch(o)
    {
    case 0:
      Index[0] = BinaryNode<FLOAT>::CornerIndex(MaxDepth+1,Depth+1,Offset[0]<<1,1);
      Index[1] = BinaryNode<FLOAT>::CornerIndex(MaxDepth+1,Depth  ,Offset[1]   ,i1);
      Index[2] = BinaryNode<FLOAT>::CornerIndex(MaxDepth+1,Depth  ,Offset[2]   ,i2);
      break;
    case 1:
      Index[0] = BinaryNode<FLOAT>::CornerIndex(MaxDepth+1,Depth  ,Offset[0]   ,i1);
      Index[1] = BinaryNode<FLOAT>::CornerIndex(MaxDepth+1,Depth+1,Offset[1]<<1,1);
      Index[2] = BinaryNode<FLOAT>::CornerIndex(MaxDepth+1,Depth  ,Offset[2]   ,i2);
      break;
    case 2:
      Index[0] = BinaryNode<FLOAT>::CornerIndex(MaxDepth+1,Depth  ,Offset[0]   ,i1);
      Index[1] = BinaryNode<FLOAT>::CornerIndex(MaxDepth+1,Depth  ,Offset[1]   ,i2);
      Index[2] = BinaryNode<FLOAT>::CornerIndex(MaxDepth+1,Depth+1,Offset[2]<<1,1);
      break;
    };

    return IndexToKey(Index);
  }

  static EDGEKEY EdgeKey(USHORT Depth, CONST UINT Offset[3], UINT EdgeIndex, USHORT MaxDepth)
  {
    UINT Index[3];
    return EdgeKey(Depth, Offset, EdgeIndex, MaxDepth, Index);
  }

  static EDGEKEY EdgeKey(CONST TreeNode& Node, UINT EdgeIndex, USHORT MaxDepth, UINT Index[3])
  {
    USHORT Depth;
    UINT Offset[3];

    Node.GetDepthAndOffset(Depth, Offset);

    return EdgeKey(Depth, Offset, EdgeIndex, MaxDepth, Index);
  }

  static EDGEKEY EdgeKey(CONST TreeNode& Node, UINT EdgeIndex, USHORT MaxDepth)
  {
    UINT Index[3];
    return EdgeKey(Node, EdgeIndex, MaxDepth, Index);
  }

  static FACEKEY FaceKey(CONST TreeNode& Node, UINT FaceIndex, USHORT MaxDepth, UINT Index[3])
  {
    INT Direction, FaceOffset;
    Cube::FactorFaceIndex(FaceIndex, Direction, FaceOffset);

    USHORT Depth;
    UINT Offset[3];

    Node.GetDepthAndOffset(Depth, Offset);

    for(SIZE_T i=0; i<3; i++)
      Index[i] = BinaryNode<FLOAT>::CornerIndex(MaxDepth+1, Depth+1, Offset[i] << 1, 1);

    Index[Direction] = BinaryNode<FLOAT>::CornerIndex(MaxDepth+1, Depth, Offset[Direction], FaceOffset);

    return IndexToKey(Index);
  }

  static FACEKEY FaceKey(CONST TreeNode& Node, UINT FaceIndex, USHORT MaxDepth)
  {
    UINT Index[3];
    return FaceKey(Node, FaceIndex, MaxDepth, Index);
  }

  static CORNERKEY CornerKey(USHORT Depth, CONST UINT Offset[3], UINT CornerIndex, USHORT MaxDepth, UINT Index[3])
  {
    UINT x[3];
    Cube::FactorCornerIndex(CornerIndex, x[0], x[1], x[2]);

    for(SIZE_T i=0; i<3; i++)
      Index[i] = BinaryNode<FLOAT>::CornerIndex(MaxDepth+1, Depth, Offset[i], x[i]);

    return IndexToKey(Index);
  }

  static CORNERKEY CornerKey(CONST TreeNode& Node, UINT CornerIndex, USHORT MaxDepth, UINT Index[3])
  {
    USHORT Depth;
    UINT Offset[3];

    Node.GetDepthAndOffset(Depth, Offset);
    return CornerKey(Depth, Offset, CornerIndex, MaxDepth, Index);
  }

  static CORNERKEY CornerKey(CONST TreeNode& Node, UINT CornerIndex, USHORT MaxDepth)
  {
    UINT Index[3];
    return CornerKey(Node, CornerIndex, MaxDepth, Index);
  }

  static CENTERKEY CenterKey(USHORT Depth, CONST UINT Offset[3], USHORT MaxDepth, UINT Index[3])
  {
    for(SIZE_T i=0; i<3; i++)
      Index[i] = BinaryNode<FLOAT>::CornerIndex(MaxDepth+1, Depth+1, Offset[i] << 1, 1);

    return IndexToKey(Index);
  }

  static CENTERKEY CenterKey(CONST TreeNode& Node, USHORT MaxDepth, UINT Index[3])
  {
    USHORT Depth;
    UINT Offset[3];

    Node.GetDepthAndOffset(Depth, Offset);
    return CenterKey(Depth, Offset, MaxDepth, Index);
  }

  static CENTERKEY CenterKey(CONST TreeNode& Node, USHORT MaxDepth)
  {
    UINT Index[3];
    return CenterKey(Node, MaxDepth, Index);
  }

};

