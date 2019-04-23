/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/Tree.hpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 917 $
 * Last Updated: $Date: 2008-12-29 14:41:52 -0800 (Mon, 29 Dec 2008) $
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

#include "DataStream.hpp"

#include "Math/Vector3.hpp"
using Bolitho::Math::Vector3;
using Bolitho::Math::Vector3F;

#include "Utility/Pair.hpp"
using Bolitho::Pair;

#include "Time/Timer.hpp"
using Bolitho::Timer;

#include "System/Threading/ThreadPool.hpp"
using Bolitho::System::ThreadPool;
using Bolitho::System::CriticalSection;

#include "System/Threading/AsynchronousWorker.hpp"
using Bolitho::System::AsynchronousWorker;

#include "Collections/HashMap.hpp"
using Bolitho::HashMap;

#include "Memory/Md5.hpp"
using Bolitho::Md5Hash;
using Bolitho::Md5;

#include "Utility/Traits.hpp"

#include "StreamManager.hpp"

struct STREAMINGSTATE
{
  USHORT TreeHeight;
  DOUBLE WeightSum;
  DOUBLE IsoValue;
  UINT InterpolationAttributeOffset;
};


/* Provides access to topology and data for a tree node.  
 * Presents a unified view of a node, even when information is split across records in different streams
 * The class can either be used as an `iterator' pattern or a `pointer' pattern
 */

class Tree;

class TreeNode
{
  friend class Tree;
public:
  TreeNode();
  TreeNode(CONST TreeNode& N);

  /* Provides raw access to each data partition */
  PARTITION0* Partition0()
  { return m_pPartition0; }
  CONST PARTITION0* Partition0() CONST
  { return m_pPartition0; }

  PARTITION1* Partition1()
  { return m_pPartition1; }
  CONST PARTITION1* Partition1() CONST
  { return m_pPartition1; }

  PARTITION2* Partition2()
  { return m_pPartition2; }
  CONST PARTITION2* Partition2() CONST
  { return m_pPartition2; }

  PARTITION3* Partition3()
  { return m_pPartition3; }
  CONST PARTITION3* Partition3() CONST
  { return m_pPartition3; }

  PARTITION4* Partition4()
  { return m_pPartition4; }
  CONST PARTITION4* Partition4() CONST
  { return m_pPartition4; }

  PARTITION5* Partition5()
  { return m_pPartition5; }
  CONST PARTITION5* Partition5() CONST
  { return m_pPartition5; }

  /* Returns the offset of this node */
  STREAMOFFSET StreamOffset() CONST;

  /* The node depth from NODETOPOLOGY */
  USHORT Depth() CONST
  { return m_pPartition0->Depth; }

  /* The x-offset from NODETOPOLOGY */
  USHORT OffsetX() CONST
  { return m_pPartition0->Offset[0]; }
  /* The y-offset from NODETOPOLOGY */
  USHORT OffsetY() CONST
  { return m_pPartition0->Offset[1]; }
  /* The z-offset from NODETOPOLOGY */
  USHORT OffsetZ() CONST
  { return m_pPartition0->Offset[2]; }
  /* The offset from NODETOPOLOGY */
  USHORT Offset(USHORT i) CONST
  { return m_pPartition0->Offset[i]; }

  /* The x-offset from NODETOPOLOGY */
  SHORT IndexX() CONST
  { return m_pPartition0->Offset[0]-((1<<m_pPartition0->Depth)-1); }
  /* The y-offset from NODETOPOLOGY */
  SHORT IndexY() CONST
  { return m_pPartition0->Offset[1]-((1<<m_pPartition0->Depth)-1); }
  /* The z-offset from NODETOPOLOGY */
  SHORT IndexZ() CONST
  { return m_pPartition0->Offset[2]-((1<<m_pPartition0->Depth)-1); }

  /* Compute the in-depth offset of the node */
  VOID GetDepthAndOffset(USHORT& Depth, UINT Offset[3]) CONST;
  VOID GetDepthAndOffset(INT& Depth, INT Offset[3]) CONST;

  /* Compute the the center and width of the node */
  VOID GetCenterAndWidth(FLOAT Center[3], FLOAT& Width) CONST;

  /* The node Value from NODEDATA*/
  FLOAT& Value()
  { return m_pPartition4->Value; }
  /* The node Solution from NODEDATA */
  FLOAT& Solution()
  { return m_pPartition5->Solution; }
  /* The node Weight from NODEDATA*/
  DOUBLE& WeightContribution()
  { return m_pPartition2->WeightContribution; }
  /* The normal from TREETOPLOGY */
  Vector3<FLOAT>& Normal()
  { return m_pPartition1->Normal; }

  #if defined(CONFIG_CUSTOMATTRIBUTE)
  /* The interpolated attribute from TREETOPLOGY */
  InterpolatedAttribute& CustomAttribute()
  { return m_pPartition1->CustomAttribute; }
  /* The interpolated attribute weight from TREETOPLOGY */
  FLOAT& CustomAttributeWeight()
  { return m_pPartition2->CustomAttributeWeight; }
  #endif

  /* The marching cubes indices from NODEDATA*/
  CHAR* MCIndex()
  { return m_pPartition3->MCIndex; }

  /* The node index from NODEDATA */
  INT& NodeIndex()
  { return m_pPartition3->NodeIndex; }

  /* The marching cube index from NODEDATA */
  INT& MarchingCubeIndex()
  { return m_pPartition3->MarchingCubeIndex; }

  /* Sets a TreeNode to be a handle to a child of this node
  * Returns TRUE iff the child exists
  */
  BOOL Child(TreeNode& Node, USHORT i) CONST;

  /* The nodes first child from TREETOPLOGY
  * Zero is a valid index.  Invalid indices are represented by -1
  */
  STREAMOFFSET Children() CONST
  { return m_pPartition0->Children; }

  /* Returns TRUE iff the node has children */
  BOOL HasChildren() CONST
  { return m_pPartition0->Children != INVALIDINDEX; }

  /* Moves this object to point to a child */
  BOOL MoveToChild(USHORT i);

  /* Moves to the next node in the stream */
  VOID MoveToNext();

  /* The sentinal value used to represent an invalid stream offset */
  STATIC CONST STREAMOFFSET INVALIDINDEX = MAXULONGLONG;

protected:
  /* Constructs a TreeNode object for a given tree, depth, and record offset */
  TreeNode(Tree* pTree, USHORT Depth, STREAMOFFSET Offset);

  /* The pointers into each data partition for this node */
  PARTITION0* m_pPartition0;
  PARTITION1* m_pPartition1;
  PARTITION2* m_pPartition2;
  PARTITION3* m_pPartition3;
  PARTITION4* m_pPartition4;
  PARTITION5* m_pPartition5;
  
  #if defined(CONFIG_STOREDNEIGHBOURS)
  PARTITION6* m_pPartition6;
  #endif

  /* The tree this node belongs to */
  Tree* m_pTree;

  public:
    class NeighborCache;
    /* Represents all possible 1-ring neighbors to a TreeNode at a given depth */
    class Neighbors
    {
      friend class NeighborCache;
    public:
      Neighbors();

      VOID Clear();

      STREAMOFFSET operator() (UINT x, UINT y, UINT z)
      { return m_pNeighbors[x][y][z]; }

    protected:
      STREAMOFFSET m_pNeighbors[3][3][3];
    };

    /* A class to compute the 1-ring neighborhood of a TreeNode and all its ancestors */
    class NeighborCache
    {
      USHORT m_Depth;
      Ptr<Tree> m_pTree;

      Neighbors& InternalComputeNeighbors(TreeNode& Node);
      Neighbors* m_pNeighbors;

    public:
      Neighbors& operator() (USHORT Depth)
      { Assert(Depth <= m_Depth); return m_pNeighbors[Depth]; }

      STREAMOFFSET operator() (USHORT Depth, UINT x, UINT y, UINT z)
      { Assert(Depth <= m_Depth); return m_pNeighbors[Depth].m_pNeighbors[x][y][z]; }

      NeighborCache();
      ~NeighborCache();

      VOID Initialize(Ptr<Tree> pTree);

      Neighbors& ComputeNeighbors(TreeNode& Node);
    };

    class NeighborCache2;
    /* Represents all possible 1-ring neighbors to a TreeNode at a given depth */
    class Neighbors2
    {
      friend class NeighborCache2;
    public:
      Neighbors2();
      VOID Clear();

      STREAMOFFSET operator() (UINT x, UINT y, UINT z)
      { return m_pNeighbors[x][y][z]; }
    protected:
      STREAMOFFSET m_pNeighbors[5][5][5];
    };

    /* A class to compute the 2-ring neighborhood of a TreeNode and all its ancestors */
    class NeighborCache2
    {
    public:
      NeighborCache2();
      ~NeighborCache2();

      VOID Initialize(Ptr<Tree> pTree);
      Neighbors2& ComputeNeighbors(TreeNode& Node);

      Neighbors2& operator() (USHORT Depth)
      { Assert(Depth <= m_Depth); return m_pNeighbors[Depth]; }
      STREAMOFFSET operator() (USHORT Depth, UINT x, UINT y, UINT z)
      { Assert(Depth <= m_Depth); return m_pNeighbors[Depth].m_pNeighbors[x][y][z]; }

    protected:
      Neighbors2& InternalComputeNeighbors(TreeNode& Node);

      USHORT m_Depth;
      Ptr<Tree> m_pTree;
      Neighbors2* m_pNeighbors;
    };


};

/* Represents the octree structure. */
class Tree : public Object
{
  friend class TreeNode;
  friend class StreamingOctreeConstructor;
  friend class LaplacianMatrixSolver;
  friend class MarchingOctreeExtractor;
public:
  STREAMOFFSET InsertBrood(USHORT Depth, CONST PARTITION0 Partition0[8], CONST PARTITION1 Partition1[8], CONST PARTITION2 Partition2[8], CONST PARTITION3 Partition3[8], CONST PARTITION4 Partition4[8], CONST PARTITION5 Partition5[8]);

  VOID InsertRoot(CONST PARTITION0& Partition0, CONST PARTITION1& Partition1, CONST PARTITION2& Partition2, CONST PARTITION3& Partition3, CONST PARTITION4& Partition4, CONST PARTITION5& Partition5);

  /* Compute PointSet hash */
  Md5Hash ComputePointSetHash();
  /* Compute PointIndex hash */
  Md5Hash ComputePointIndexHash() CONST;

  /* Returns the tree root */
  TreeNode Root();

  /* Returns a given node */
  TreeNode GetNode(USHORT Depth, STREAMOFFSET Offset);

  /* Returns the SPATIALINDEX for a given depth, slice and region */
  SPATIALINDEX* GetNodeIndex(USHORT Depth, UINT Slice, UINT Region= 0);

  /* Returns the SPATIALINDEX for a given depth, slice and region */
  SPATIALINDEX* GetPointIndex(USHORT Depth, UINT Slice, UINT Region= 0);

  /* Returns the maximum number of nodes in any slice at a given depth */
  STREAMOFFSET GetMaximumNodeSliceCount(USHORT Depth);

  /* Returns the average number of nodes in the slices at a given depth */
  STREAMOFFSET GetAverageNodeSliceCount(USHORT Depth);

  /* Returns the number of points in the specified slice at a given depth */
  STREAMOFFSET GetPointSliceCount(USHORT Depth, UINT SliceIndex);

  /* Returns the maximum number of points in a slice at a given depth */
  STREAMOFFSET GetMaximumPointSliceCount(USHORT Depth);

  /* Returns the average number of points in a slice at a given depth */
  STREAMOFFSET GetAveragePointSliceCount(USHORT Depth);

  /* Returns the number of nodes in the specified slice at a given depth */
  STREAMOFFSET GetNodeSliceCount(USHORT Depth, UINT SliceIndex);

  /* Returns the maximum number of nodes in any slice at any depth */
  STREAMOFFSET GetMaximumNodeSliceCount();

  /* Returns the number of nodes at a given depth */
  STREAMOFFSET GetNodeCount(USHORT Depth);

  /* Returns the coarsest depth that was advanced when advancing to slice i */
  USHORT CoarsestDepthChanged(UINT SliceIndex);

  /* Returns the Current Node Index at a specific depth */
  UINT GetCurrentNodeIndex(USHORT Depth)
  { return m_pCurrentNodeIndex[Depth]; }

  /* Create a Tree object and a new set of files on disk */
  static Ptr<Tree> Create(LPCTSTR InputFilename, LPCTSTR StreamingFilename, LPCTSTR InterpolationAttributeName, USHORT MaximumDepth, BOOL DeleteOnComplete);

  /* Iterate nodes
  * Calls NODEFUNCTION for each node in a given plane, at a given depth
  * RelativeIndex must fall within +/- m_Neighbourhood
  * If the NODEFUNCTION is threadsafe, calling with ThreadSafe=TRUE will enable multithreading
  */
  template<class NODEFUNCTION>
  VOID IterateNodes(INT RelativeIndex, USHORT Depth, NODEFUNCTION& F);
  template<class NODEFUNCTION>
  VOID IterateNodesBrood(INT RelativeIndex, USHORT Depth, NODEFUNCTION& F);

  /* Iterate points
  * Calls POINTFUNCTION for each point in a given 2-plane
  * RelativeIndex must fall within +/- m_Neighbourhood
  */
  template<class POINTFUNCTION>
  VOID IteratePoints(INT RelativeIndex, POINTFUNCTION& F);


  /* Advances the current plane */
  VOID AdvancePoints(UINT AdvanceAmount = 1);
  VOID AdvanceNodes(USHORT Depth, UINT AdvanceAmount = 1);

  /* Initialises the tree for a pass across the data with a given neighbourhood */
  VOID Initialise(INT Neighbourhood, DWORD TopologyFlags, DWORD DataFlags, DWORD PointFlags);
  VOID Initialise(INT BackNeighbourhood, INT ForwardNeighbourhood, DWORD TopologyFlags, DWORD DataFlags, DWORD PointFlags);
  VOID Initialise(Pair<INT,INT>* pNeighbourhoods, DWORD TopologyFlags, DWORD DataFlags, DWORD PointFlags);

  /* Finalises the tree after a pass across the data */
  VOID Finalise();

  /* Returns the height of the tree */
  USHORT Height() CONST
  { return m_State.TreeHeight; }

  /* Returns the maximum depth */
  USHORT MaximumDepth() CONST
  { return m_State.TreeHeight-1; }

  DOUBLE GetWeightSum()
  { return m_State.WeightSum; }
  DOUBLE GetIsoValue()
  { return m_State.IsoValue; }

  /* Returns the resolution of the tree */
  UINT Resolution() CONST
  { return 1u << (m_State.TreeHeight-1); }

  /* Data Stream Access */
  DataStream& GetPointStream()
  { return m_StreamManager.GetPointStream(); }

  DataStream& GetNodeStream(USHORT Depth, UINT Partition)
  { return m_StreamManager.GetNodeStream(Depth, Partition); }

  /* Fixes Offsets in the NodeIndex */
  VOID FixupNodeIndexOffsets(USHORT Depth);

  #if defined(CONFIG_MULTITHREADED)
  /* Returns a reference to the threadpool */
  ThreadPool& GetThreadPool()
  { return m_IteratePool; }

	UINT GetThreadCount()
	{ return m_ThreadCount; }
  #endif

  ~Tree();
protected:
  Tree(LPCTSTR StreamingFilename);

  template<class POINTFUNCTION>
  VOID IteratePointsST(USHORT Depth, UINT Slice, POINTFUNCTION& F);
  template<class NODEFUNCTION>
  VOID IterateNodesST(USHORT Depth, UINT Slice, NODEFUNCTION& F);
  template<class NODEFUNCTION>
  VOID IterateNodesBroodST(USHORT Depth, UINT Slice, NODEFUNCTION& F);

  #if defined(CONFIG_MULTITHREADED)
  /* The thread pool used to execute IterateNodes calls */
  ThreadPool m_IteratePool;
  UINT m_ThreadCount;

  template<class NODEFUNCTION>
  struct IterateNodesInfo
  {
    IterateNodesInfo(Tree* pTree, NODEFUNCTION& Func) : F(Func), pTree(pTree)
    { }

    NODEFUNCTION& F;
    SPATIALINDEX* pIndex;
    STREAMOFFSET Offset;
    STREAMOFFSET Count;
    UINT StartRegion;
    UINT EndRegion;
		UINT PartitionId;
    INT Depth;
    UINT Slice;
    Tree* pTree;
  };

  template<class NODEFUNCTION>
  static DWORD WINAPI IterateNodesProc(IterateNodesInfo<NODEFUNCTION>* pInfo);
  template<class NODEFUNCTION>
  static DWORD WINAPI IterateNodesBroodProc(IterateNodesInfo<NODEFUNCTION>* pInfo);
  template<class NODEFUNCTION>
  VOID IterateNodesMT(USHORT Depth, UINT Slice, NODEFUNCTION& F);
  template<class NODEFUNCTION>
  VOID IterateNodesBroodMT(USHORT Depth, UINT Slice, NODEFUNCTION& F);
  
  template<class POINTFUNCTION>
  struct IteratePointsInfo
  {
    IteratePointsInfo(Tree* pTree, POINTFUNCTION& Func) : F(Func), pTree(pTree)
    { }

    POINTFUNCTION& F;
    SPATIALINDEX* pIndex;
    UINT StartRegion;
    UINT EndRegion;
    ORIENTEDPOINT* pPointData;
    Tree* pTree;
  };

  template<class POINTFUNCTION>
  static DWORD WINAPI IteratePointsProc(IteratePointsInfo<POINTFUNCTION>* pInfo);
  template<class POINTFUNCTION>
  VOID IteratePointsMT(USHORT Depth, UINT Slice, POINTFUNCTION& F);

  #endif

  /* The stream manager */
  StreamManager m_StreamManager;

  /* The prefix used to name all temporary files */
  String m_StreamingFilename;
  
  /* The state file */
  Ptr<File> m_pStateFile;

  /* Private Initialisation */
  VOID Initialise(DWORD TopologyFlags, DWORD DataFlags, DWORD PointFlags);

  /* The current computation neighbourhood.  Set via Initialise */
  Pair<INT,INT>* m_pNeighbourhoods;
  /* The current computation plane.  Must be in range [0,2^d) */
  UINT m_CurrentPointIndex;
  /* The current computation plane for each depth */
  UINT* m_pCurrentNodeIndex;

  /* Persistant state */
  STREAMINGSTATE m_State;

};

#include "TreeImpl.hpp"
