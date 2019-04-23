/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/StreamingSurfaceExtractor.hpp $
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

#include "FunctionDataInfo.hpp"
#include "Tree.hpp"
#include "StreamingOctreeTraversal.hpp"

#include "Collections/HashMap.hpp"
using Bolitho::HashMap;

#include "Math/Matrix4.hpp"
using Bolitho::Math::Matrix4;
using Bolitho::Math::Vector3;

#include "Math/Geometry/Types.hpp"
using Bolitho::Math::Geometry::INDEXEDTRIANGLE;

#include "System/Threading/CriticalSection.hpp"
using Bolitho::System::CriticalSection;
#include "System/Threading/ScopedLock.hpp"
using Bolitho::System::Lock;

class MarchingOctreeExtractor
{
  

  class Vertex
  {
  public:
    Vertex() {}
    Vertex(CONST Vector3<FLOAT>& Position, UINT Index)
    {
      m_Position = Position;
      m_Index = Index;
    }
    
    Vector3<FLOAT> m_Position;
    UINT m_Index;
  };
  
  class IVertex : public Vertex
  {
  public:
    IVertex() {}
    IVertex(CONST Vertex& v, CORNERKEY key)
    {
      m_Position = v.m_Position;
      m_Index = v.m_Index;
      m_Key = key;
    }
    
    CORNERKEY m_Key;
  };

  typedef Pair<IVertex,IVertex> IVertexPair;
  typedef Pair<CORNERKEY,CORNERKEY> Edge;
  typedef Array<Edge> EdgeArray;
  
  class IsoVertexData
  {
  public:
    CriticalSection m_Lock;
    hash_map<CORNERKEY,FLOAT> *m_pValues[2*CONFIG_REGIONCOUNT];
    hash_map<CORNERKEY,Vector3F>* m_pNormals[2*CONFIG_REGIONCOUNT];
    hash_map<CORNERKEY,Vertex>* m_pVertices[3];
    hash_map<CORNERKEY,Vertex>* m_pChildVertices[3];
    hash_map<FACEKEY,EdgeArray> *m_pEdges[3];
    
    IsoVertexData();
    ~IsoVertexData();
    VOID Advance();

    FLOAT GetValue(UINT RegionId, UINT i, CORNERKEY Key)
    { return (*m_pValues[i*CONFIG_REGIONCOUNT + RegionId])[Key]; }
    Vector3F GetNormal(UINT RegionId, UINT i, CORNERKEY Key)
    { return (*m_pNormals[i*CONFIG_REGIONCOUNT + RegionId])[Key]; }
    Vertex GetVertex(UINT i, CORNERKEY Key)
    { return (*m_pVertices[i])[Key]; }
    Vertex GetChildVertex(UINT i, CORNERKEY Key)
    { return (*m_pChildVertices[i])[Key]; }
    Array<Edge>& GetEdges(UINT i, EDGEKEY Key)
    { return (*m_pEdges[i])[Key]; }

    BOOL ContainsValue(UINT RegionId, UINT i, CORNERKEY Key)
    { return (m_pValues[i*CONFIG_REGIONCOUNT + RegionId]->find(Key) != m_pValues[i*CONFIG_REGIONCOUNT + RegionId]->end()); }
    BOOL ContainsNormal(UINT RegionId, UINT i, CORNERKEY Key)
    { return (m_pNormals[i*CONFIG_REGIONCOUNT + RegionId]->find(Key) != m_pNormals[i*CONFIG_REGIONCOUNT + RegionId]->end()); }
    BOOL ContainsVertex(UINT i, CORNERKEY Key)
    { return (m_pVertices[i]->find(Key) != m_pVertices[i]->end()); }
    BOOL ContainsChildVertex(UINT i, CORNERKEY Key)
    { return (m_pChildVertices[i]->find(Key) != m_pChildVertices[i]->end()); }
    BOOL ContainsEdges(UINT i, FACEKEY Key)
    { return (m_pEdges[i]->find(Key) != m_pEdges[i]->end()); }

    BOOL GetValue(UINT RegionId, UINT i, CORNERKEY Key, FLOAT& Value)
    { if (m_pValues[i*CONFIG_REGIONCOUNT + RegionId]->find(Key) != m_pValues[i*CONFIG_REGIONCOUNT + RegionId]->end()) { Value = (*m_pValues[i*CONFIG_REGIONCOUNT + RegionId])[Key]; return TRUE; } else { return FALSE; } }
    BOOL GetNormal(UINT RegionId, UINT i, CORNERKEY Key, Vector3F& Normal)
    { if (m_pNormals[i*CONFIG_REGIONCOUNT + RegionId]->find(Key) != m_pNormals[i*CONFIG_REGIONCOUNT + RegionId]->end()) { Normal = (*m_pNormals[i*CONFIG_REGIONCOUNT + RegionId])[Key]; return TRUE; } else { return FALSE; } }
    BOOL GetVertex(UINT i, CORNERKEY Key, Vertex& Vertex)
    { if (m_pVertices[i]->find(Key) != m_pVertices[i]->end()) { Vertex = (*m_pVertices[i])[Key]; return TRUE; } else { return FALSE; } }
    BOOL GetChildVertex(UINT i, CORNERKEY Key, Vertex& Vertex)
    { if (m_pChildVertices[i]->find(Key) != m_pChildVertices[i]->end()) { Vertex = (*m_pChildVertices[i])[Key]; return TRUE; } else { return FALSE; } }

    VOID SetValue(UINT RegionId, UINT i, CORNERKEY Key, FLOAT Value)
    { (*m_pValues[i*CONFIG_REGIONCOUNT + RegionId])[Key] = Value; }
    VOID SetNormal(UINT RegionId, UINT i, CORNERKEY Key, Vector3F Normal)
    { (*m_pNormals[i*CONFIG_REGIONCOUNT + RegionId])[Key] = Normal; }
    VOID SetVertex(UINT i, CORNERKEY Key, Vertex V)
    { (*m_pVertices[i])[Key] = V; }
    VOID SetChildVertex(UINT i, CORNERKEY Key, Vertex V)
    { (*m_pChildVertices[i])[Key] = V; }
    VOID AddEdge(UINT i, EDGEKEY Key, Edge& E)
    { (*m_pEdges[i])[Key].Add(E); }


    /* Locks the depth level lock */
    VOID Lock()
    { m_Lock.Lock(); }
    /* Unlocks the depth level lock */
    VOID Unlock()
    { m_Lock.Unlock(); }

  };

  class CornerValueFunction
  {
  public:
    INT m_Offset;
    DOUBLE m_IsoValue;
    MarchingOctreeExtractor* m_pMOE;
    IsoVertexData* m_pIVD;
    NodeFunctionDataInfo& m_FDI;

    CornerValueFunction(NodeFunctionDataInfo& FDI) : m_FDI(FDI)
    {}

    struct THREADLOCAL
    {
       UINT ThreadId;
       UINT RegionId;
			 UINT PartitionId;
       TreeNode::NeighborCache Neighbours;
    };

    VOID InitThreadLocal(Tree* pTree, THREADLOCAL& L)
    {
      L.Neighbours.Initialize(pTree);
    }

    /* Calls GetCornerValue */
    VOID ProcessNode(Tree* pTree, TreeNode& N, THREADLOCAL& L);
    
    CONST STATIC BOOL ThreadSafe = TRUE;

    /* Returns the cached value, or computes the corner value */
    /*  RW: m_pIVD[N.Depth()].m_pValues */
    DOUBLE GetCornerValue(Tree* pTree, TreeNode& N, INT CornerIndex, THREADLOCAL& L);

  };
  
  class PropogateDataFunction
  {
  public:
    INT m_Offset;
    IsoVertexData * m_pIVD;
    
    struct THREADLOCAL { UINT ThreadId; UINT RegionId; UINT PartitionId; };
    VOID InitThreadLocal(Tree* pTree, THREADLOCAL& L)
    {}
 
    /* R: m_pIVD[ALL except N.Depth()].m_pValues */
    /* W: m_pIVD[N.Depth()].m_pValues */
    /* R: m_pIVD[ALL except N.Depth()].m_pNormals */
    /* W: m_pIVD[N.Depth()].m_pNormals */
    VOID ProcessNode(Tree* pTree,TreeNode& N, THREADLOCAL& L);
    
    CONST STATIC BOOL ThreadSafe = TRUE;
  };
  
  class IsoVertexFunction
  {
  public:
    INT m_Offset;
    MarchingOctreeExtractor* m_pMOE;
    
    struct THREADLOCAL
    {
      UINT ThreadId;
      UINT RegionId;
			UINT PartitionId;
      TreeNode::NeighborCache Neighbours;
    };

    VOID InitThreadLocal(Tree* pTree, THREADLOCAL& L)
    {
      L.Neighbours.Initialize(pTree);
    }

    /* Calls AddEdge and AddVertex */
    VOID ProcessNode(Tree* pTree,TreeNode& N, THREADLOCAL& L);

    CONST STATIC BOOL ThreadSafe = TRUE;
  };
  
  class IsoSurfaceFunction
  {
  public:
    MarchingOctreeExtractor* m_pMOE;
    
    struct THREADLOCAL
    {
      UINT ThreadId;
      UINT RegionId;
			UINT PartitionId;
      TreeNode::NeighborCache Neighbours;
    };

    VOID InitThreadLocal(Tree* pTree, THREADLOCAL& L)
    {
      L.Neighbours.Initialize(pTree);
    }

    /* Calls AddVertex, AddEdge, EdgeListToEdgeLoops, EdgeLoopToTriangles and AddTriangle */
    /* R: m_pIVD[N.Depth()].m_pEdges */
    /* R: m_pIVD[N.Depth()].m_pChildVertices */
    VOID ProcessNode(Tree* pTree,TreeNode& N, THREADLOCAL& L);

    CONST STATIC BOOL ThreadSafe = TRUE;
  private:
    VOID EdgeListToEdgeLoops(Array<IVertexPair>& EdgeList, Array<Array<IVertex> >& Loops);
    VOID EdgeLoopToTriangles(Array<IVertex>& Loop, Array<INDEXEDTRIANGLE>& Triangles);
  };
  
  IsoVertexFunction m_IVF;
  PropogateDataFunction m_PDF;
  IsoSurfaceFunction m_ISF;
  CornerValueFunction m_CVF;

  IsoVertexData* m_pIVD;

  Ptr<Tree> m_pTree;
  
  NodeFunctionDataInfo m_FDI;
  
  DOUBLE m_IsoValue;

  
  /* Pulls up Vertices from children */
  /*  Called from UpdateExtractor */
  /*  R: m_pIVD[Depth].m_pVertices */
  /*  W: m_pIVD[Depth-1].m_pVertices */
  VOID PushDownVertices(INT Depth);
  
  /* Emits a finalized triangle to the output stream */
  /*  Called from IsoSurfaceFunction::ProcessNode */
  UINT EmitTriangle(CONST INDEXEDTRIANGLE& Triangle);
  UINT m_TriangleCount;
  Ptr<File> m_pTriangles;
  CriticalSection m_OutputLock;

  /* Emits a finalized, transformed vertex position to the output stream */
  /* Called from AddVertex */
  UINT EmitVertex(CONST Vector3F& Position, CONST Vector3F& Normal, CONST InterpolatedAttribute& CustomAttribute);
  Ptr<File> m_pVertices;
  UINT m_VertexCount;
  Matrix4<FLOAT> m_Transform;
  
  /* Adds an edge */
  /*  Called from IsoSurfaceFunction::ProcessNode and IsoVertexFunction::ProcessNode */
  /*  RW: m_pIVD[ANY].m_pEdges */
  /*  RW: m_pIVD[ANY].m_pChildVertices */
  /*  Calls AddVertex */
  VOID AddEdge(TreeNode& Node, INT EdgeIndex1, INT EdgeIndex2, BOOL ForceFront, UINT RegionId, TreeNode::NeighborCache& Neighbours);
  CriticalSection m_AddEdgeLock;

  /* Gets the depth and offset of the face neighbour of a node */
  /*  Called from AddEdge */
  VOID GetFaceNeighbor(TreeNode& Node, INT FaceIndex, TreeNode::NeighborCache& Neighbours, USHORT& Depth, ULONGLONG& StreamIndex);
  
  /* Calls and returns an IndexedVertex object to represent it */
  /*  Called from IsoSurfaceFunction::ProcessNode, IsoVertexFunction::ProcessNode, AddEdge */
  /*  RW: m_pIVD[N.Depth()].m_pVertices */
  /*  R: m_pIVD[N.Depth()].m_pValues */
  /*  R: m_pIVD[N.Depth()].m_pNormals */
  IVertex AddVertex(TreeNode& Node, INT EdgeIndex, BOOL ForceFront, UINT RegionId, TreeNode::NeighborCache& Neighbours);
  CriticalSection m_AddVertexLock;

  #if defined(CONFIG_CUSTOMATTRIBUTE)
  /* Called from AddVertex */
  InterpolatedAttribute GetEdgeCustomAttribute(TreeNode& Node, TreeNode::NeighborCache& Neighbours, Vector3F Position);
  #endif

  /* Gets the normal at the corner of a node */
  /*  Called from AddVertex */
  Vector3F GetCornerNormal(TreeNode& Node, INT CornerIndex, TreeNode::NeighborCache& Neighbours);

public:
  MarchingOctreeExtractor();
  ~MarchingOctreeExtractor();
  
  VOID Init(Ptr<Tree> pTree, Ptr<File> pVertices, Ptr<File> pTriangles, CONST Matrix4<FLOAT>& Transform);
  VOID InitExtractor();
  VOID UpdateExtractor(INT depth);
  VOID FinalizeExtractor();
};

