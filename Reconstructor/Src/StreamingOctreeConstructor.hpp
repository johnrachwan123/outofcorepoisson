/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/StreamingOctreeConstructor.hpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 884 $
 * Last Updated: $Date: 2008-11-19 11:36:58 -0800 (Wed, 19 Nov 2008) $
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

#include "Collections/CircularArray.hpp"
using Bolitho::CircularArray;
using Bolitho::Array;

#include "Tree.hpp"
#include "FunctionDataInfo.hpp"
#include "Octree.hpp"

#include "System/Threading/MultiThreadedModel.hpp"
using Bolitho::MultiThreadedModel;

typedef OctNode OctreeNode;
typedef PoolAllocator<OctreeNode,MultiThreadedModel> NodeAllocator;

class StreamingOctreeConstructor
{
  class UnprocessedPoint
  {
  public:
    UnprocessedPoint();
    UnprocessedPoint(Vector3F& Position, Vector3F& Normal, InterpolatedAttribute& CustomAttribute, FLOAT Weight, OctreeNode* pNode);
    
    Vector3F m_Position;
    Vector3F m_Normal;
    FLOAT m_Weight;
    InterpolatedAttribute m_CustomAttribute;

    OctreeNode* m_pNode;
  };
  
  class PerDepthData
  {
  public:
    PerDepthData();
    
    CircularArray< Array<UnprocessedPoint> > m_UnprocessedPoints[CONFIG_MAXTHREADS];
    CircularArray< NodeAllocator > m_NodeAllocators[CONFIG_MAXTHREADS];
  };
  
  /* Creates tree topology and updates the weight function for each point */
  class WeightFunction
  {
    friend class StreamingOctreeConstructor;
  public:
    WeightFunction(OctreeNode* pRoot) : m_pRoot(pRoot)
    {}

    CONST STATIC BOOL ThreadSafe = TRUE;

    struct THREADLOCAL
    { 
      OctreeNode::NeighborCache Neighbors; 
      UINT ThreadId;
      UINT RegionId;
    };

    VOID InitThreadLocal(Tree* pTree, THREADLOCAL& L)
    {
      L.Neighbors.Initialize(pTree->MaximumDepth()); 
      #if defined(CONFIG_MULTITHREADED)
      L.ThreadId = ThreadPool::GetThreadId();
      #else
      L.ThreadId = 0;
      #endif
    }

    VOID ProcessPoint(Tree* pTree, Vector3F& Position, Vector3F& Normal, THREADLOCAL& L);

  private:
    VOID UpdateWeightContribution(OctreeNode* pNode, CONST Vector3F& Position, FLOAT Weight, THREADLOCAL& L);

    OctreeNode* m_pRoot;
    CriticalSection m_Lock;
    StreamingOctreeConstructor* m_pSOC;
  };

  /* Places points into the unprocessed point list */
  class PointFunction
  {
    friend class StreamingOctreeConstructor;
  public:
    PointFunction(OctreeNode* pRoot) : m_pRoot(pRoot)
    {}

    CONST STATIC BOOL ThreadSafe = TRUE;

    struct THREADLOCAL { UINT ThreadId; UINT RegionId; };
    VOID InitThreadLocal(Tree* pTree, THREADLOCAL& L) {}

    VOID ProcessPoint(Tree* pTree, Vector3F Position, Vector3F Normal, THREADLOCAL& L);
    
  private:
    OctreeNode* m_pRoot;
    PerDepthData* m_pPerDepthData;
    CriticalSection m_Lock;

    INT m_Slice;
  };

  /* Places points at their final position in the tree once the weighting function is known */
  class UnprocessedPointFunction
  {
    friend class StreamingOctreeConstructor;
  public:
    UnprocessedPointFunction(NodeFunctionDataInfo& FDI, FLOAT SamplesPerNode) : m_FDI(FDI), m_SamplesPerNode(SamplesPerNode)
    {}

    CONST STATIC BOOL ThreadSafe = TRUE;

    struct THREADLOCAL 
    {
      OctreeNode::NeighborCache Neighbors; 
      UINT ThreadId;
      UINT RegionId;
    };

    VOID InitThreadLocal(Tree* pTree, OctreeNode* pRoot, THREADLOCAL& L) 
    {
      L.Neighbors.Initialize(pTree->MaximumDepth()); 
      #if defined(CONFIG_MULTITHREADED)
      L.ThreadId = ThreadPool::GetThreadId();
      #else
      L.ThreadId = 0;
      #endif
    }

    VOID ProcessPoint(UINT Slice, USHORT Depth, UnprocessedPoint& Point, THREADLOCAL& L);

  private:
    PerDepthData* m_pPerDepthData;
    FLOAT m_SamplesPerNode;
    NodeFunctionDataInfo& m_FDI;
    CriticalSection m_Lock;

    DOUBLE GetSampleWeight(OctreeNode* pNode, CONST Vector3F& Position, THREADLOCAL& L);
    VOID SplatOrientedPoint(OctreeNode* pNode, CONST Vector3F& Position, CONST Vector3F& Normal, CONST InterpolatedAttribute& CustomAttribute, FLOAT CustomAttributeWeight, THREADLOCAL& L);
  };

  class NormalFunction
  {
    friend class StreamingOctreeConstructor;
  public:
    NormalFunction(NodeFunctionDataInfo& FDI, USHORT MaxDepth) : m_FDI(FDI), m_MaxDepth(MaxDepth)
    {
      for (UINT i=0; i<CONFIG_MAXTHREADS; i++)
        m_WeightSum[i] = 0.0;
    }

    CONST STATIC BOOL ThreadSafe = TRUE;

    struct THREADLOCAL 
    { 
      OctreeNode::NeighborCache2 Neighbors2; 
      UINT ThreadId;
      UINT RegionId;
    };

    VOID InitThreadLocal(Tree* pTree, OctreeNode* pRoot, THREADLOCAL& L) 
    { 
      L.Neighbors2.Initialize(pTree->MaximumDepth()); 
      #if defined(CONFIG_MULTITHREADED)
      L.ThreadId = ThreadPool::GetThreadId();
      #else
      L.ThreadId = 0;
      #endif
    }

    VOID ProcessNode(UINT Slice, USHORT Depth, OctreeNode* pNode, THREADLOCAL& L);
  private:
    CriticalSection m_Lock;
    NodeFunctionDataInfo& m_FDI;
    DOUBLE m_WeightSum[CONFIG_MAXTHREADS];
    USHORT m_MaxDepth;
  };

  /* Writes a brood of nodes and writes them to the streams */
  class WriteNodesFunction
  {
  public:
    WriteNodesFunction(Ptr<Tree> pTree, BOOL ClipTree) : m_pTree(pTree), m_ClipTree(ClipTree)
    { m_ClippedNodes = 0; }

    CONST STATIC BOOL ThreadSafe = FALSE;

    struct THREADLOCAL 
    {
      PARTITION0 Partition0[8];
      PARTITION1 Partition1[8];
      PARTITION2 Partition2[8];
      PARTITION3 Partition3[8];
      PARTITION4 Partition4[8];
      PARTITION5 Partition5[8];

      UINT ThreadId;
      UINT RegionId;
    };

    VOID InitThreadLocal(Tree* pTree, OctreeNode* pRoot, THREADLOCAL& L) {}

    VOID ProcessNode(UINT Slice, USHORT Depth, OctreeNode* pNode, THREADLOCAL& L);

    UINT m_ClippedNodes;
  private:
    BOOL m_ClipTree;
    Ptr<Tree> m_pTree;
    CriticalSection m_Lock;
  };

  PerDepthData* m_pPerDepthData;
  
  NodeFunctionDataInfo m_FDI;
  String m_InterpolationAttributeName;

  Ptr<Tree> m_pTree;

  WeightFunction m_WeightFunction;
  PointFunction m_PointFunction;

  UnprocessedPointFunction m_UnprocessedPointFunction;
  NormalFunction m_NormalFunction;
  WriteNodesFunction m_WriteNodesFunction;
  
  OctreeNode m_Root;

  VOID ProcessPoints(USHORT Depth, INT Slice);
  VOID ProcessNormals(USHORT Depth, INT Slice);
  VOID WriteOutNodes(USHORT Depth, INT Slice);
  
  STATIC INT OctantIndex(CONST Vector3F& Center, CONST Vector3F& Point);
  
  template<class NODEFUNCTION>
  VOID IterateNodes(UINT Slice, USHORT Depth, BOOL IterateBroods, NODEFUNCTION& F);
  
  #if defined(CONFIG_MULTITHREADED)
  template<class NODEFUNCTION>
  struct IterateNodesInfo
  {
    IterateNodesInfo(NODEFUNCTION& F) : Function(F)
    {}

    Tree* pTree;
    OctreeNode* pRoot;
    NODEFUNCTION& Function;
    SIZE_T Count;
    OctreeNode** ppNodes;
    UINT Slice;
    USHORT Depth;
  };
  
  template<class NODEFUNCTION>
  STATIC DWORD IterateNodesProc(IterateNodesInfo<NODEFUNCTION>* pInfo);
  #endif

  VOID GetSliceNodeList(OctNode* pNode, UINT CurrentSlice, UINT Width, UINT Slice, USHORT Depth, BOOL IterateBroods, Array<OctreeNode*>& Nodes);

  template<class POINTFUNCTION>
  VOID IterateUnprocessedPoints(UINT Slice, USHORT Depth, POINTFUNCTION& F);

#if defined(CONFIG_MULTITHREADED)
  template<class POINTFUNCTION>
  struct IterateUnprocessedPointsInfo
  {
    IterateUnprocessedPointsInfo(POINTFUNCTION& F) : Function(F)
    {}

    Tree* pTree;
    OctreeNode* pRoot;
    POINTFUNCTION& Function;
    SIZE_T Count;
    UnprocessedPoint* pPoints;
    UINT Slice;
    USHORT Depth;
  };
  
  template<class POINTFUNCTION>
  STATIC DWORD IterateUnprocessedPointsProc(IterateUnprocessedPointsInfo<POINTFUNCTION>* pInfo);
  #endif


public:
  
  StreamingOctreeConstructor(Ptr<Tree> pTree, FLOAT SamplesPerNode, BOOL ClipTree, FLOAT SampleRadius);
  ~StreamingOctreeConstructor();
  
  NodeAllocator& GetAllocator(OctreeNode* N, UINT ThreadId);
  
  VOID InitConstructor();
  VOID UpdateConstructor(UINT Slice);
  VOID FinalizeConstructor();
  
};

#include "StreamingOctreeConstructorImpl.hpp"

