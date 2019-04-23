/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/Octree.hpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 881 $
 * Last Updated: $Date: 2008-11-17 10:09:37 -0800 (Mon, 17 Nov 2008) $
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

#include "Allocator.hpp"
#include "BinaryNode.hpp"
#include "MarchingCubes.hpp"

using Bolitho::Sum;

class OctNode
{
public:
  /* The depth of the node */
  USHORT Depth() CONST
  { return m_Depth; }
  /* The offset of the node in the x, y or z dimension */
  USHORT Offset(SIZE_T i) CONST
  { return m_Offset[i]; }

  /* Read the center weight contribution value */
  FLOAT CenterWeightContribution()
  { return Sum<FLOAT,CONFIG_MAXTHREADS>(&m_CenterWeightContribution[0]); }
  /* Read the node value */
  DOUBLE Value()
  { return Sum<DOUBLE,CONFIG_MAXTHREADS>(&m_Value[0]); }
  /* Read the weight contribution */
  DOUBLE WeightContribution()
  { return Sum<DOUBLE,CONFIG_MAXTHREADS>(&m_WeightContribution[0]); }

  /* Read the center weight contribution value */
  FLOAT& CenterWeightContribution(UINT i)
  { return m_CenterWeightContribution[i]; }
  /* Read the node value */
  DOUBLE& Value(UINT i)
  { return m_Value[i]; }
  /* Read the weight contribution */
  DOUBLE& WeightContribution(UINT i)
  { return m_WeightContribution[i]; }

  /* The normal */
  Vector3F Normal()
  { return Sum<Vector3F,CONFIG_MAXTHREADS>(&m_Normal[0]); }
  /* The normal */
  Vector3F& Normal(UINT i)
  { return m_Normal[i]; }

  /* The interpolated attribute */
  InterpolatedAttribute CustomAttribute()
  { return Sum<InterpolatedAttribute,CONFIG_MAXTHREADS>(&m_CustomAttribute[0]); }
  /* The interpolated attribute */
  InterpolatedAttribute& CustomAttribute(UINT i)
  { return m_CustomAttribute[i]; }

  /* The interpolated attribute weight */
  FLOAT CustomAttributeWeight()
  { return Sum<FLOAT,CONFIG_MAXTHREADS>(&m_CustomAttributeWeightContribution[0]); }
  /* The interpolated attribute weight */
  FLOAT& CustomAttributeWeight(UINT i)
  { return m_CustomAttributeWeightContribution[i]; }

  /* Returns the i-th child node */
  OctNode* GetChild(USHORT i)
  { return &m_pChildren[i]; }
  /* Returns the nodes parent */
  OctNode* GetParent()
  { return m_pParent; }
  /* Sets the nodes parent */
  VOID SetParent(OctNode* pParent)
  { m_pParent = pParent; }

  /* Returns this nodes child index */
  USHORT GetChildIndex()
  { return (USHORT)(this - m_pParent->m_pChildren); }

  /* Returns TRUE if the node has children */
  BOOL HasChildren()
  { return m_pChildren != 0; }
  /* Gets the stream offset that the children reside at */
  ULONGLONG GetChildrenIndex()
  { return m_ChildrenIndex; }
  /* Sets the stream offset that the children reside at */
  VOID SetChildrenIndex(ULONGLONG ChildrenIndex)
  { m_ChildrenIndex = ChildrenIndex; }

  OctNode();
  ~OctNode();

  /* Gets the i-th child node.  Creates all children if they dont exist */
  template<class AllocatingClass>
  OctNode* GetOrCreateChild(USHORT i, AllocatingClass* ac);

  /* Allocates a brood of children */
  template<class AllocatingClass>
  OctNode* CreateChildren(AllocatingClass* c);

  /* Deletes the children of this node */
  VOID DeleteChildren()
  { for (SIZE_T i=0; i<8; i++) m_pChildren[i].m_pParent = 0; m_pChildren = 0; }


  VOID GetDepthAndOffset(INT& Depth, INT Offset[3]) CONST; 
  VOID GetDepthAndOffset(USHORT& Depth, UINT Offset[3]) CONST; 

  /* Computes the center and width of the node if the entire tree is a partitioning of the unit-cube */
  VOID GetCenterAndWidth(Vector3F& Center, FLOAT& Width) CONST;

  STATIC VOID Index(SIZE_T Depth, CONST SIZE_T Offset[3], USHORT& D, USHORT Off[3]);

  /* Validates that the tree and all descendents form a valid tree */
  VOID Validate();

  /* Returns TRUE iff a given node is a descendant node of this node */
  BOOL IsDescendant(OctNode* pNode);
private:
  STATIC CONST INT DepthShift, OffsetShift, OffsetShift1, OffsetShift2, OffsetShift3;
  STATIC CONST INT DepthMask, OffsetMask;

  FLOAT m_CenterWeightContribution[CONFIG_MAXTHREADS];
  DOUBLE m_WeightContribution[CONFIG_MAXTHREADS];
  DOUBLE m_Value[CONFIG_MAXTHREADS];
  Vector3F m_Normal[CONFIG_MAXTHREADS];
  InterpolatedAttribute m_CustomAttribute[CONFIG_MAXTHREADS];
  FLOAT m_CustomAttributeWeightContribution[CONFIG_MAXTHREADS];
  
  USHORT m_Depth;
  USHORT m_Offset[3];

  OctNode* m_pParent;
  OctNode* m_pChildren;
  ULONGLONG m_ChildrenIndex;

  STATIC CriticalSection m_Lock[512];
public:

#if 0
  class Neighbours
  {
  public:
    Neighbours(SIZE_T Height);
    ~Neighbours();

    /* Gets a pointer to a neighbouring node, or NULL if it doesn't exist */
    OctNode* operator() (SIZE_T Depth, SIZE_T i, SIZE_T j, SIZE_T k) CONST;
    OctNode* GetNeighbour(SIZE_T Depth, SIZE_T i, SIZE_T j, SIZE_T k) CONST;

    /* Clears all neighbour information */
    VOID Clear();

    /* Clears the neighbour information at a specific depth */
    VOID ClearDepth(SIZE_T i);

    /* Computes the neighbours for a given node.
    * If Create is TRUE, then all neighbours that don't exist will be created 
    */
    VOID ComputeNeighbours(OctNode* pNode, BOOL Create = FALSE);

  private:
    SIZE_T m_Height;
    OctNode* m_pNeighbors;
  };

  class NeighborKey
  {
  private:
    USHORT m_Depth;
    Neighbors* m_Neighbors;

    template<class AllocatingClass>
    Neighbors& InternalSetNeighbors(OctNode* node,AllocatingClass* ac);
    Neighbors& InternalGetNeighbors(OctNode* node);

  public:

    NeighborKey();
    ~NeighborKey();

    VOID Initialize(USHORT depth);
    template<class AllocatingClass>

    Neighbors& SetNeighbors(OctNode* node,AllocatingClass* ac);
    Neighbors& GetNeighbors(OctNode* node);
  };
#else

  class NeighborCache;
  class Neighbors
  {
    friend class NeighborCache;

  protected:
    OctNode* m_pNeighbors[3][3][3];

  public:
    OctNode* operator() (UINT x, UINT y, UINT z)
    { return m_pNeighbors[x][y][z]; }

    Neighbors();

    /* Sets all m_pNeighbors to NULL */
    VOID Clear();
  };

  class NeighborCache
  {
  public:
    NeighborCache();
    ~NeighborCache();

    OctNode* operator() (USHORT Depth, UINT x, UINT y, UINT z)
    { return m_pNeighbors[Depth].m_pNeighbors[x][y][z]; }

    Neighbors& operator() (USHORT Depth)
    { return m_pNeighbors[Depth]; }

    /* Initializes a cache up to a specified depth */
    VOID Initialize(USHORT Depth);

    template<class AllocatingClass>
    Neighbors& CreateNeighbors(OctNode* node,AllocatingClass* ac);
    Neighbors& ComputeNeighbors(OctNode* node);

  protected:
    USHORT m_Depth;

    template<class AllocatingClass>
    Neighbors& InternalCreateNeighbors(OctNode* node,AllocatingClass* ac);
    Neighbors& InternalComputeNeighbors(OctNode* node);

    Neighbors* m_pNeighbors;

  };
#endif

  class NeighborCache2;
  class Neighbors2
  {
    friend class NeighborCache2;

  public:
    /* Reads the neighbour at a node relative position (x,y,z) */
    OctNode* operator() (UINT x, UINT y, UINT z)
    { return m_pNeighbors[x][y][z]; }

    Neighbors2();
    
    /* Sets all neighbour pointers to NULL */
    VOID Clear();

  protected:
    OctNode* m_pNeighbors[5][5][5];
  };

  class NeighborCache2
  {
  public:
    /* Reads the neighbour at a node relative position (x,y,z) */
    OctNode* operator() (USHORT Depth, UINT x, UINT y, UINT z)
    { return m_pNeighbors[Depth].m_pNeighbors[x][y][z]; }

    /* Gets all the neighbours at a given depth */
    Neighbors2& operator() (USHORT Depth)
    { return m_pNeighbors[Depth]; }

    NeighborCache2();
    ~NeighborCache2();

    VOID Initialize(USHORT Depth);

    template<class AllocatingClass>
    Neighbors2& CreateNeighbors(OctNode* node,AllocatingClass* ac);
    Neighbors2& ComputeNeighbors(OctNode* node);

  protected:
    USHORT m_Depth;
    template<class AllocatingClass>
    Neighbors2& InternalCreateNeighbors(OctNode* node,AllocatingClass* ac);
    Neighbors2& InternalComputeNeighbors(OctNode* node);
    Neighbors2* m_pNeighbors;
  };
};

#include "OctreeImpl.hpp"

