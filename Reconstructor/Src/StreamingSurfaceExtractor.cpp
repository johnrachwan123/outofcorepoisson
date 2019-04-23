/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/StreamingSurfaceExtractor.cpp $
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

#include "Math/Geometry/MinimalAreaTriangulation.hpp"
using Bolitho::Math::Geometry::GetMinimalTriangulation;

#include "Collections/Array.hpp"
using Bolitho::Array;

#include "Utility/Bits.hpp"
using Bolitho::Interleave;

#include "Math/Polynomial.hpp"
using Bolitho::Math::PolynomialN;

#include "StreamingSurfaceExtractor.hpp"
#include "StreamingOctreeTraversal.hpp"

//---------------------------------------------------------------------------------------------------
VOID MarchingOctreeExtractor::IsoSurfaceFunction::EdgeLoopToTriangles(Array<IVertex>& Loop, Array<INDEXEDTRIANGLE>& Triangles)
{
  Triangles.Resize(0);
  Array<Vector3F> LoopVertices;
  LoopVertices.Resize(Loop.Length());
  for(SIZE_T i=0; i<Loop.Length(); i++)
    LoopVertices[i] = Loop[i].m_Position;
  Triangles = GetMinimalTriangulation(LoopVertices);

  for(SIZE_T i=0; i<Triangles.Length(); i++)
    for(SIZE_T j=0; j<3; j++)
      Triangles[i].Vertex[j] = Loop[Triangles[i].Vertex[j]].m_Index;
}
//---------------------------------------------------------------------------------------------------
VOID MarchingOctreeExtractor::IsoSurfaceFunction::EdgeListToEdgeLoops(Array<IVertexPair>& list, Array<Array<IVertex> >& loops)
{
  Array< Pair<IVertex,INT> > StartPoints, EndPoints;

  while(!list.IsEmpty())
  {
    IVertexPair edge = list.EraseLast();
    Array<IVertex> front, back;
    IVertexPair FrontEdge = edge;
    IVertexPair BackEdge = edge;
    
    front.Add(FrontEdge.First);

    BOOL PairFound = TRUE;
    while(PairFound)
    {
      PairFound = FALSE;
      for(SIZE_T i = 0; i < list.Length(); i++)
      {
        if(list[i].First.m_Index == FrontEdge.Second.m_Index)
        {
          FrontEdge = list[i];
          list[i] = list.GetLast();
          list.EraseLast();
          front.Add(FrontEdge.First);
          PairFound = TRUE;
          break;
        }
      }
    }

    PairFound = TRUE;
    while(PairFound)
    {
      PairFound = FALSE;
      for(SIZE_T i=0; i<list.Length(); i++)
      {
        if(list[i].Second.m_Index == BackEdge.First.m_Index)
        {
          BackEdge = list[i];
          list[i] = list.GetLast();
          list.EraseLast();
          back.Add(BackEdge.First);
          PairFound = TRUE;
          break;
        }
      }
    }

    SIZE_T idx = loops.Length();
    loops.Resize(idx+1);
    for(SIZE_T i=back.Length()-1; i<back.Length(); i--)
      loops[idx].Add(back[i]);
    for(SIZE_T i=0; i<front.Length(); i++)
      loops[idx].Add(front[i]);

    if(BackEdge.First.m_Index != FrontEdge.Second.m_Index)
    {
      loops[idx].Add(FrontEdge.Second);
      StartPoints.Add(Pair<IVertex,INT>(BackEdge.First,(INT)idx));
      EndPoints.Add(Pair<IVertex,INT>(FrontEdge.Second,(INT)idx));
      
      TraceDWarning(this, "Unmatched Edge");
    }
  }

#if defined(DEBUG)
  if(EndPoints.Length())
  {
    for(SIZE_T i=0; i<EndPoints.Length(); i++)
    {
      USHORT d;
      UINT off[3];
      UINT eIndex;
      INT o;
      UINT i1,i2;
      VertexData::FactorEdgeKey(m_pMOE->m_pTree->MaximumDepth(), StartPoints[i].First.m_Key, d, off, eIndex);
      Cube::FactorEdgeIndex(eIndex, o, i1, i2);
      TraceWarning(this, "\t%d] %d\t(%d %d %d)\t%d\t", i, d, off[0], off[1], off[2], o);
      VertexData::FactorEdgeKey(m_pMOE->m_pTree->MaximumDepth(), EndPoints[i].First.m_Key, d, off, eIndex);
      Cube::FactorEdgeIndex(eIndex, o, i1, i2);
      TraceWarning(this, "%d\t(%d %d %d)\t%d\n", d, off[0], off[1], off[2], o);
    }
  }
#endif

  while(!EndPoints.IsEmpty())
  {
    Pair<IVertex,INT> ePoint = EndPoints.EraseLast();
    INT ii;
    INT d = -1;
    for(INT i=0; i<StartPoints.Length(); i++)
    {
      INT tempD = VertexData::CommonEdgeDepth(ePoint.First.m_Key, StartPoints[i].First.m_Key, m_pMOE->m_pTree->MaximumDepth());
      if (tempD == -1)
        break;

      if(tempD > d)
      {
        d = tempD;
        ii = i;
      }
    }

    if(d == -1)
    {
      TraceDWarning(this, "Paired vertex not found");
    }
    else
    {
      Pair<IVertex,INT> sPoint = StartPoints[ii];

      #if defined(DEBUG)
      USHORT d;
      UINT off[3];
      UINT eIndex;
      INT o;
      UINT i1,i2;
      VertexData::FactorEdgeKey(m_pMOE->m_pTree->MaximumDepth(),ePoint.First.m_Key,d,off,eIndex);
      Cube::FactorEdgeIndex(eIndex,o,i1,i2);
      TraceWarning(this,"Matching: %d\t(%d %d %d)\t%d",d,off[0],off[1],off[2],o);
      VertexData::FactorEdgeKey(m_pMOE->m_pTree->MaximumDepth(),sPoint.First.m_Key,d,off,eIndex);
      Cube::FactorEdgeIndex(eIndex,o,i1,i2);
      TraceWarning(this,"->\t%d\t(%d %d %d)\t%d",d,off[0],off[1],off[2],o);
      #endif

      StartPoints[ii] = StartPoints.GetLast();
      StartPoints.EraseLast();

      if(ePoint.Second != sPoint.Second)
      {
        for(SIZE_T i=0; i<loops[sPoint.Second].Length(); i++)
          loops[ePoint.Second].Add(loops[sPoint.Second][i]);
        loops[ePoint.Second].Resize(0);
        for(SIZE_T i=0; i<EndPoints.Length(); i++)
          if(EndPoints[i].Second == sPoint.Second)
            EndPoints[i].Second = ePoint.Second;
      }
    }
  }
}
//---------------------------------------------------------------------------------------------------
VOID MarchingOctreeExtractor::IsoSurfaceFunction::ProcessNode(Tree* pTree, TreeNode& N, THREADLOCAL& L)
{
  USHORT Depth = N.Depth();
  if(N.HasChildren())
    return;

  Array<Pair<IVertex,IVertex> > EdgeList;
  Array<Array<IVertex> > EdgeLoops;
  Array<INDEXEDTRIANGLE> Triangles;

  INT mcIndex = 0;
  INT face0,face1;

  face0=Cube::FaceIndex(0,0);
  face1=Cube::FaceIndex(0,1);

  for(INT fc=0;fc<Square::CORNERS;fc++)
  {
    INT c0 = Cube::SquareToCubeCorner(face0, fc);
    INT c1 = Cube::SquareToCubeCorner(face1, fc);
    if(N.MCIndex()[0] & (1<<fc))
      mcIndex |= 1<<c0;
    if(N.MCIndex()[1] & (1<<fc))
      mcIndex |= 1<<c1;
  }

  CONST std::vector<std::vector<INT> >& table = MarchingCubes::caseTable(mcIndex);
  if (Depth == pTree->MaximumDepth())
  {
    EdgeLoops.Resize(table.size());

    for(SIZE_T i=0; i<table.size(); i++)
    {
      EdgeLoops[i].Resize(table[i].size());
      for(SIZE_T j=0; j<table[i].size(); j++)
      {
        EdgeLoops[i][j] = m_pMOE->AddVertex(N,table[i][j], FALSE, L.RegionId, L.Neighbours);
        INT FaceIndex = Cube::FaceAdjacentToEdges(table[i][j], table[i][(j+1) % table[i].size()]);
        if(FaceIndex != Cube::FaceIndex(0,0) && FaceIndex != Cube::FaceIndex(0,1))
          m_pMOE->AddEdge(N, table[i][j], table[i][(j+1) % table[i].size()], FALSE, L.RegionId, L.Neighbours);
      }
    }
  }
  else
  {
    for(UINT i=0; i<6; i++)
    {
      FACEKEY FaceKey = VertexData::FaceKey(N, i, pTree->Height()-1);
      INT dir,off,idx;
      Cube::FactorFaceIndex(i,dir,off);
      if(dir)
        idx = 1;
      else
        idx = off << 1;

      if (m_pMOE->m_pIVD[Depth].ContainsEdges(idx, FaceKey))
      {
        Array<Edge>& Edges = m_pMOE->m_pIVD[Depth].GetEdges(idx, FaceKey);
        Pair<Vertex,Vertex> Edge;

        for(SIZE_T j=0;j<Edges.Length();j++)
        {
          if (!m_pMOE->m_pIVD[Depth].GetChildVertex(0, Edges[j].First, Edge.Second) &&
              !m_pMOE->m_pIVD[Depth].GetChildVertex(1, Edges[j].First, Edge.Second) &&
              !m_pMOE->m_pIVD[Depth].GetChildVertex(2, Edges[j].First, Edge.Second))
                TraceWarning(this, "Could not find vertex: %lld", Edges[j].First);

          if (!m_pMOE->m_pIVD[Depth].GetChildVertex(0, Edges[j].Second, Edge.First) &&
              !m_pMOE->m_pIVD[Depth].GetChildVertex(1, Edges[j].Second, Edge.First) &&
              !m_pMOE->m_pIVD[Depth].GetChildVertex(2, Edges[j].Second, Edge.First))
                TraceWarning(this, "Could not find vertex: %lld", Edges[j].Second);

          EdgeList.Add(Pair<IVertex,IVertex>(IVertex(Edge.First, Edges[j].Second), IVertex(Edge.Second, Edges[j].First)));
        }
      }
      else
      {
        for(SIZE_T j=0; j<table.size(); j++)
        {
          for(SIZE_T k=0; k<table[j].size(); k++)
          {
            if(i == Cube::FaceAdjacentToEdges(table[j][k], table[j][(k+1) % table[j].size()]))
            {
              if(i != Cube::FaceIndex(0,0) && i != Cube::FaceIndex(0,1))
                m_pMOE->AddEdge(N, table[j][k], table[j][(k+1) % table[j].size()], FALSE, L.RegionId, L.Neighbours);
              EdgeList.Add(Pair<IVertex,IVertex>(m_pMOE->AddVertex(N, table[j][k], FALSE, L.RegionId, L.Neighbours), m_pMOE->AddVertex(N, table[j][(k+1) % table[j].size()], FALSE, L.RegionId, L.Neighbours)));
            }
          }
        }
      }
    }
    EdgeListToEdgeLoops(EdgeList, EdgeLoops);
  }


  for(SIZE_T i=0; i<EdgeLoops.Length(); i++)
  {
    EdgeLoopToTriangles(EdgeLoops[i], Triangles);
    for(SIZE_T j=0; j<Triangles.Length(); j++)
      m_pMOE->EmitTriangle(Triangles[j]);
  }
}
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
VOID MarchingOctreeExtractor::PropogateDataFunction::ProcessNode(Tree* pTree, TreeNode& N, THREADLOCAL& L)
{
  if(N.HasChildren())
    return;

  USHORT Depth = N.Depth();
  UINT idx[3];

  for(UINT y=0; y<2; y++)
  {
    for(UINT z=0; z<2; z++)
    {
      INT CornerIndex = Cube::CornerIndex(m_Offset, y, z);
      CORNERKEY Key = VertexData::CornerKey(N, CornerIndex, pTree->Height()-1, idx);

      // Search for the value in the hash-tables of the children
      for(USHORT h=Depth+1; h<pTree->Height(); h++)
      {
        FLOAT Value;
        if(m_pIVD[h].GetValue(L.RegionId, 1, Key, Value))
        {
          m_pIVD[Depth].SetValue(L.RegionId, 1, Key, Value);
          break;
        }
      }

      for (USHORT h=Depth+1; h<pTree->Height(); h++)
      {
        Vector3F Normal;
        if (m_pIVD[h].GetNormal(L.RegionId, 1, Key, Normal))
        {
          m_pIVD[Depth].SetNormal(L.RegionId, 1, Key, Normal);
          break;
        }
      }
    }
  }
}
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
MarchingOctreeExtractor::IsoVertexData::IsoVertexData()
{
  for (UINT i=0; i<2*CONFIG_REGIONCOUNT; i++)
  {
    m_pValues[i] = new hash_map<CORNERKEY,FLOAT>();
    m_pNormals[i] = new hash_map<CORNERKEY,Vector3F>();
  }

  m_pVertices[0] = new hash_map<CORNERKEY,Vertex>();
  m_pVertices[1] = new hash_map<CORNERKEY,Vertex>();
  m_pVertices[2] = new hash_map<CORNERKEY,Vertex>();
  m_pChildVertices[0] = new hash_map<CORNERKEY,Vertex>();
  m_pChildVertices[1] = new hash_map<CORNERKEY,Vertex>();
  m_pChildVertices[2] = new hash_map<CORNERKEY,Vertex>();
  m_pEdges[0] = new hash_map<EDGEKEY,EdgeArray>();
  m_pEdges[1] = new hash_map<EDGEKEY,EdgeArray>();
  m_pEdges[2] = new hash_map<EDGEKEY,EdgeArray>();
}
//---------------------------------------------------------------------------------------------------
MarchingOctreeExtractor::IsoVertexData::~IsoVertexData()
{
  for (UINT i=0; i<2*CONFIG_REGIONCOUNT; i++)
  {
    if(m_pValues[i])
      delete m_pValues[i];
    m_pValues[i] = 0;

    if(m_pNormals[i])
      delete m_pNormals[i];
    m_pNormals[i] = 0;
  }

  if(m_pVertices[0])
    delete m_pVertices[0];
  if(m_pVertices[1])
    delete m_pVertices[1];
  if(m_pVertices[2])
    delete m_pVertices[2];

  m_pVertices[0] = 0;
  m_pVertices[1] = 0;
  m_pVertices[2] = 0;

  if(m_pChildVertices[0])
    delete m_pChildVertices[0];
  if(m_pChildVertices[1])
    delete m_pChildVertices[1];
  if(m_pChildVertices[2]) 
    delete m_pChildVertices[2];

  m_pChildVertices[0] = 0;
  m_pChildVertices[1] = 0;
  m_pChildVertices[2] = 0;

  if(m_pEdges[0])
    delete m_pEdges[0];
  if(m_pEdges[1])
    delete m_pEdges[1];
  if(m_pEdges[2])
    delete m_pEdges[2];

  m_pEdges[0] = 0;
  m_pEdges[1] = 0;
  m_pEdges[2] = 0;

}
//---------------------------------------------------------------------------------------------------
VOID MarchingOctreeExtractor::IsoVertexData::Advance()
{
  for (UINT i=0; i<CONFIG_REGIONCOUNT; i++)
  {
    delete m_pValues[i];
    m_pValues[i] = m_pValues[i + CONFIG_REGIONCOUNT];
    m_pValues[i + CONFIG_REGIONCOUNT] = new hash_map<CORNERKEY,FLOAT>();

    delete m_pNormals[i];
    m_pNormals[i] = m_pNormals[i + CONFIG_REGIONCOUNT];
    m_pNormals[i + CONFIG_REGIONCOUNT] = new hash_map<CORNERKEY,Vector3F>();
  }

  delete m_pVertices[0];
  delete m_pVertices[1];
  m_pVertices[0] = m_pVertices[2];
  m_pVertices[1] = new hash_map<CORNERKEY,Vertex>();
  m_pVertices[2] = new hash_map<CORNERKEY,Vertex>();

  delete m_pChildVertices[0];
  delete m_pChildVertices[1];
  m_pChildVertices[0] = m_pChildVertices[2];
  m_pChildVertices[1] = new hash_map<CORNERKEY,Vertex>();
  m_pChildVertices[2] = new hash_map<CORNERKEY,Vertex>();

  delete m_pEdges[0];
  delete m_pEdges[1];
  m_pEdges[0] = m_pEdges[2];
  m_pEdges[1] = new hash_map<EDGEKEY,EdgeArray>();
  m_pEdges[2] = new hash_map<EDGEKEY,EdgeArray>();

}
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
VOID MarchingOctreeExtractor::CornerValueFunction::ProcessNode(Tree* pTree, TreeNode& N, THREADLOCAL& L)
{
  if(N.HasChildren())
    return;

  DOUBLE Value[4];

  int yy, zz, fIndex = 0;
  int face = Cube::FaceIndex(0, m_Offset);
  for(UINT fc=0; fc<4; fc++)
  {
    Square::FactorCornerIndex(fc, yy, zz);
    INT cornerIndex = Cube::CornerIndex(m_Offset, yy, zz);
    Value[fc] = GetCornerValue(pTree, N, cornerIndex, L);
  }

  N.MCIndex()[m_Offset] = MarchingSquares::GetIndex(Value, m_IsoValue);
}
//---------------------------------------------------------------------------------------------------
DOUBLE MarchingOctreeExtractor::CornerValueFunction::GetCornerValue(Tree* pTree, TreeNode& N, INT CornerIndex, THREADLOCAL& L)
{
  UINT idx[3];
  CORNERKEY Key = VertexData::CornerKey(N, CornerIndex, pTree->Height()-1, idx);

  //m_pIVD[N.Depth()].Lock();
  FLOAT CachedValue = 0.0f;
  if (m_pIVD[N.Depth()].GetValue(L.RegionId, 1, Key, CachedValue))
  {
    //m_pIVD[N.Depth()].m_Lock.Unlock();
    return CachedValue;
  }
  //m_pIVD[N.Depth()].Unlock();

  DOUBLE Value = 0;

  L.Neighbours.ComputeNeighbors(N);
  for(USHORT i=0;i<=N.Depth();i++)
  {
    for(UINT j=0;j<3;j++)
    {
      for(UINT k=0;k<3;k++)
      {
        for(UINT l=0;l<3;l++)
        {
          if(L.Neighbours(i,j,k,l) != TreeNode::INVALIDINDEX)
          {
            TreeNode Temp = pTree->GetNode(i, L.Neighbours(i,j,k,l));
            Value += (DOUBLE)Temp.Solution() * m_FDI.FunctionData.ValueTables(Temp.OffsetX(), idx[0]) * m_FDI.FunctionData.ValueTables(Temp.OffsetY(), idx[1]) * m_FDI.FunctionData.ValueTables(Temp.OffsetZ(), idx[2]);
          }
        }
      }
    }
  }

  UINT x,y,z;
  USHORT d = N.Depth();

  Cube::FactorCornerIndex(CornerIndex, x, y, z);
  for(INT i=0;i<2;i++)
  {
    for(INT j=0;j<2;j++)
    {
      for(INT k=0;k<2;k++)
      {
        if(L.Neighbours(d,x+i,y+j,z+k) != TreeNode::INVALIDINDEX)
        {
          TreeNode Temp = pTree->GetNode(d, L.Neighbours(d,x+i,y+j,z+k));
          INT ii=Cube::AntipodalCornerIndex(Cube::CornerIndex(i,j,k));
          while(Temp.Children() != TreeNode::INVALIDINDEX)
          {
            Temp.MoveToChild(ii);
            Value += (DOUBLE)Temp.Solution() * m_FDI.FunctionData.ValueTables(Temp.OffsetX(),idx[0]) * m_FDI.FunctionData.ValueTables(Temp.OffsetY(),idx[1]) * m_FDI.FunctionData.ValueTables(Temp.OffsetZ(),idx[2]);
          }
        }
      }
    }
  }
  
  //m_pIVD[N.Depth()].Lock();
  m_pIVD[N.Depth()].SetValue(L.RegionId, 1, Key, (FLOAT)Value);
  //m_pIVD[N.Depth()].Unlock();

  return Value;
}
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
VOID MarchingOctreeExtractor::IsoVertexFunction::ProcessNode(Tree* pTree, TreeNode& N, THREADLOCAL& L)
{
  if(N.HasChildren())
    return;

  INT fIndex = N.MCIndex()[m_Offset];
  INT face = Cube::FaceIndex(0, m_Offset);
  INT EdgeIndex1, EdgeIndex2;

  for(SIZE_T i=0; i<MarchingSquares::caseTable(fIndex).count; i++)
  {
    if(m_Offset)
    {
      EdgeIndex1=Cube::SquareToCubeEdge(face,MarchingSquares::caseTable(fIndex).edge[i].first);
      EdgeIndex2=Cube::SquareToCubeEdge(face,MarchingSquares::caseTable(fIndex).edge[i].second);
    }
    else
    {
      EdgeIndex2=Cube::SquareToCubeEdge(face,MarchingSquares::caseTable(fIndex).edge[i].first);
      EdgeIndex1=Cube::SquareToCubeEdge(face,MarchingSquares::caseTable(fIndex).edge[i].second);
    }
    m_pMOE->AddVertex(N, EdgeIndex1, TRUE, L.RegionId, L.Neighbours);
    m_pMOE->AddVertex(N, EdgeIndex2, TRUE, L.RegionId, L.Neighbours);
    m_pMOE->AddEdge(N, EdgeIndex1, EdgeIndex2, TRUE, L.RegionId, L.Neighbours);
  }
}
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
MarchingOctreeExtractor::MarchingOctreeExtractor() : m_CVF(m_FDI)
{
  m_pTree = 0;
  m_VertexCount = 0;
  m_pIVD = 0;
  MarchingCubes::SetCaseTable();
}
//---------------------------------------------------------------------------------------------------
MarchingOctreeExtractor::~MarchingOctreeExtractor()
{
  m_pTree = 0;
}
//---------------------------------------------------------------------------------------------------
VOID MarchingOctreeExtractor::Init(Ptr<Tree> pTree, Ptr<File> pVertices, Ptr<File> pTriangles, CONST Matrix4<FLOAT>& Transform)
{
  m_pTriangles = pTriangles;
  m_pVertices = pVertices;
  m_VertexCount = 0;
  m_Transform = Transform;

  m_pTree = pTree;
  m_IsoValue = m_pTree->m_State.IsoValue;

  Trace("Iso Value: %f %x", m_IsoValue, m_IsoValue);

  m_FDI.Initialize(m_pTree->Height()-1);
  m_FDI.FunctionData.InitializeValueTables(FunctionDataUsage::VALUE | FunctionDataUsage::DVALUE, 0, 1.0f/ (1<<(m_pTree->Height()-1)));

  m_pIVD = new IsoVertexData[m_pTree->Height()];
  m_CVF.m_IsoValue = m_IsoValue;
  m_CVF.m_pMOE = this;
  m_CVF.m_pIVD = m_pIVD;

  m_ISF.m_pMOE = this;
  m_PDF.m_pIVD = m_pIVD;
  m_IVF.m_pMOE = this;

  m_pTree->Initialise(2, 1 ,DataAccess::READ, DataAccess::READWRITE, DataAccess::NONE);

}
//---------------------------------------------------------------------------------------------------
VOID MarchingOctreeExtractor::InitExtractor()
{
  #if defined(CONFIG_PROFILE)
  TimeAllFunctionCalls();
  #endif
}
//---------------------------------------------------------------------------------------------------
VOID MarchingOctreeExtractor::UpdateExtractor(INT FromDepth)
{
  #if defined(CONFIG_PROFILE)
  TimeAllFunctionCalls();
  #endif

  for(INT Depth = m_pTree->MaximumDepth(); Depth>=FromDepth; Depth--)
  {
    /* Gather the values and normals from the children */
    m_PDF.m_Offset = 1;
    m_pTree->IterateNodes(-1, Depth, m_PDF);
    m_PDF.m_Offset = 0;
    m_pTree->IterateNodes(0, Depth, m_PDF);

    /* Set the corner values from either side of the back plane of the slice */
    m_CVF.m_Offset = 1;
    m_pTree->IterateNodes(-1, Depth, m_CVF);
    m_CVF.m_Offset = 0;
    m_pTree->IterateNodes(0, Depth, m_CVF);

    /* Compute the iso-vertices from either side of the back plane of the slice */
    m_IVF.m_Offset = 1;
    m_pTree->IterateNodes(-1,Depth, m_IVF);
    m_IVF.m_Offset = 0;
    m_pTree->IterateNodes(0,Depth, m_IVF);

    /* Extract the iso surface from the previous slice */
    m_pTree->IterateNodes(-1,Depth, m_ISF);

    /* Add the iso-vertices to the parent level */
    PushDownVertices(Depth);
  }

  /* Advance the data hash tables */
  for(INT Depth = m_pTree->MaximumDepth(); Depth>=(INT)FromDepth; Depth--)
  {
    m_pIVD[Depth].Advance();
    m_pTree->AdvanceNodes(Depth);
  }
}
//---------------------------------------------------------------------------------------------------
VOID MarchingOctreeExtractor::FinalizeExtractor()
{
  #if defined(CONFIG_PROFILE)
  TimeAllFunctionCalls();
  #endif

  for(INT d=m_pTree->Height()-1;d>=0;d--)
  {
    /* Gather the values and normals from the children */
    m_PDF.m_Offset = 1;
    m_pTree->IterateNodes(-1,d,m_PDF);
    m_PDF.m_Offset = 0;
    m_pTree->IterateNodes(0,d,m_PDF);

    /* Set the corner values from either side of the back plane of the slice */
    m_CVF.m_Offset = 1;
    m_pTree->IterateNodes(-1,d,m_CVF);

    /* Compute the iso-vertices from either side of the back plane of the slice */
    m_IVF.m_Offset = 1;
    m_pTree->IterateNodes(-1,d,m_IVF);
    m_IVF.m_Offset = 0;
    m_pTree->IterateNodes(0,d,m_IVF);

    /* Extract the iso surface from the previous slice */
    m_pTree->IterateNodes(-1,d,m_ISF);
  }

  m_pTree->Finalise();

  delete[] m_pIVD;
  m_pIVD = 0;
}
//---------------------------------------------------------------------------------------------------
VOID MarchingOctreeExtractor::PushDownVertices(INT depth)
{
  if(!depth)
    return;

  hash_map<CORNERKEY,Vertex>::iterator iter;

  for (iter = m_pIVD[depth].m_pVertices[1]->begin(); iter != m_pIVD[depth].m_pVertices[1]->end(); iter++)
  {
    UINT offset[3];
    UINT eIndex;
    UINT i1;
    UINT i2;
    INT o;
    EDGEKEY eKey;
    VertexData::FactorEdgeKey(depth, m_pTree->Height()-1, iter->first, offset, eIndex);
    Cube::FactorEdgeIndex(eIndex,o,i1,i2);

    // Check if the edge exists on the parent
    if ((i1 != (offset[1]&1)) || (i2 != (offset[2]&1)))
      continue;
    offset[0]>>=1;
    offset[1]>>=1;
    offset[2]>>=1;

    eKey=VertexData::EdgeKey(depth-1,offset,eIndex,m_pTree->Height()-1);

    if(m_pIVD[depth-1].m_pVertices[1]->find(eKey)==m_pIVD[depth-1].m_pVertices[1]->end())
      (*m_pIVD[depth-1].m_pVertices[1])[eKey]=iter->second;
    else
      m_pIVD[depth-1].m_pVertices[1]->erase(eKey);
  }

  for (iter = m_pIVD[depth].m_pVertices[2]->begin(); iter != m_pIVD[depth].m_pVertices[2]->end(); iter++)
  {
    INT o;
    UINT i1;
    UINT i2;
    UINT offset[3];
    UINT eIndex;
    EDGEKEY eKey;
    VertexData::FactorEdgeKey(depth,m_pTree->Height()-1,iter->first,offset,eIndex);
    Cube::FactorEdgeIndex(eIndex,o,i1,i2);

    // Check if the edge exists on the parent
    if(o==1)
    {
      if ((i1 != (offset[0]&1)) || (i2 != (offset[2]&1)))
        continue;
    }
    else
    {
      if ((i1 != (offset[0]&1)) || (i2 != (offset[1]&1)))
        continue;
    }
    offset[0]>>=1;
    offset[1]>>=1;
    offset[2]>>=1;

    eKey=VertexData::EdgeKey(depth-1,offset,eIndex,m_pTree->Height()-1);

    if(m_pIVD[depth-1].m_pVertices[2]->find(eKey)==m_pIVD[depth-1].m_pVertices[2]->end())
      (*m_pIVD[depth-1].m_pVertices[2])[eKey]=iter->second;
    else
      m_pIVD[depth-1].m_pVertices[2]->erase(eKey);
  }
}
//---------------------------------------------------------------------------------------------------
Vector3<FLOAT> MarchingOctreeExtractor::GetCornerNormal(TreeNode& N, INT CornerIndex, TreeNode::NeighborCache& Neighbours)
{
  UINT idx[3];
  VertexData::CornerKey(N, CornerIndex, m_pTree->Height() - 1, idx);
  
  Vector3F Normal;

  Neighbours.ComputeNeighbors(N);

  for(INT i=0;i<=N.Depth();i++)
  {
    for(INT j=0;j<3;j++)
    {
      for(INT k=0;k<3;k++)
      {
        for(INT l=0;l<3;l++)
        {
          if(Neighbours(i,j,k,l) != TreeNode::INVALIDINDEX)
          {
            TreeNode Temp = m_pTree->GetNode(i, Neighbours(i,j,k,l));
            
            Normal[0] += Temp.Solution() * (FLOAT)(m_FDI.FunctionData.DValueTables(Temp.OffsetX(), idx[0]) * m_FDI.FunctionData.ValueTables(Temp.OffsetY(), idx[1]) * m_FDI.FunctionData.ValueTables(Temp.OffsetZ(), idx[2]));
            Normal[1] += Temp.Solution() * (FLOAT)(m_FDI.FunctionData.ValueTables(Temp.OffsetX(), idx[0]) * m_FDI.FunctionData.DValueTables(Temp.OffsetY(), idx[1]) * m_FDI.FunctionData.ValueTables(Temp.OffsetZ(), idx[2]));
            Normal[2] += Temp.Solution() * (FLOAT)(m_FDI.FunctionData.ValueTables(Temp.OffsetX(), idx[0]) * m_FDI.FunctionData.ValueTables(Temp.OffsetY(), idx[1]) * m_FDI.FunctionData.DValueTables(Temp.OffsetZ(), idx[2]));
          }
        }
      }
    }
  }

  return Normal;
}
//---------------------------------------------------------------------------------------------------
VOID MarchingOctreeExtractor::GetFaceNeighbor(TreeNode& N, INT fIndex, TreeNode::NeighborCache& Neighbours, USHORT& Depth, ULONGLONG& StreamIndex)
{
  INT dir,offset,idx[3];
  Cube::FactorFaceIndex(fIndex,dir,offset);

  Neighbours.ComputeNeighbors(N);
  StreamIndex = TreeNode::INVALIDINDEX;

  idx[0] = idx[1] = idx[2] = 1;
  idx[dir] = offset<<1;
  for(Depth = N.Depth(); Depth>=0; Depth--)
  {
    if(Neighbours(Depth, idx[0], idx[1], idx[2]) != TreeNode::INVALIDINDEX)
    {
      StreamIndex = Neighbours(Depth, idx[0], idx[1], idx[2]);
      return;
    }
  }
}
//---------------------------------------------------------------------------------------------------
VOID MarchingOctreeExtractor::AddEdge(TreeNode& N, INT eIndex1, INT eIndex2, INT ForceFront, UINT RegionId, TreeNode::NeighborCache& Neighbours)
{
  //Lock<CriticalSection> L(m_AddEdgeLock);

  INT f=Cube::FaceAdjacentToEdges(eIndex1,eIndex2);
  ULONGLONG streamIndex;
  USHORT depth;
  
  if(f == -1)
    return;

  GetFaceNeighbor(N, f, Neighbours, depth, streamIndex);
  if(depth==N.Depth() || streamIndex == TreeNode::INVALIDINDEX)
    return;

  TreeNode nbr=m_pTree->GetNode(depth,streamIndex);
  INT dir,offset,newOffset;
  Cube::FactorFaceIndex(f,dir,offset);

  newOffset=(offset+1)&1;
  FACEKEY fKey = VertexData::FaceKey(nbr, Cube::FaceIndex(dir,newOffset), m_pTree->Height()-1);
  Edge e;
  Vertex v1,v2;
  e.First = VertexData::EdgeKey(N, eIndex1, m_pTree->Height()-1);
  e.Second = VertexData::EdgeKey(N, eIndex2, m_pTree->Height()-1);

  m_AddEdgeLock.Lock();
  if(dir!=0)	// If the face is not in the x-plane
    m_pIVD[depth].AddEdge(1, fKey, e);
  else		// Otherwise
    m_pIVD[depth].AddEdge(2, fKey, e);
  m_AddEdgeLock.Unlock();

  // Must add vertices to vertex list as well...
  v1 = AddVertex(N,eIndex1,ForceFront, RegionId, Neighbours);
  v2 = AddVertex(N,eIndex2,ForceFront, RegionId, Neighbours);

  INT o;
  UINT i1;
  UINT i2;
  Cube::FactorEdgeIndex(eIndex1,o,i1,i2);

  m_AddEdgeLock.Lock();
  if(!o)
    m_pIVD[depth].SetChildVertex(1, e.First, v1);
  else
    if(ForceFront)
      m_pIVD[depth].SetChildVertex(2, e.First, v1);
    else
      m_pIVD[depth].SetChildVertex(i1<<1, e.First, v1);

  Cube::FactorEdgeIndex(eIndex2,o,i1,i2);
  if(!o)
    m_pIVD[depth].SetChildVertex(1, e.Second, v2);
  else
    if(ForceFront)
      m_pIVD[depth].SetChildVertex(2, e.Second, v2);
    else
      m_pIVD[depth].SetChildVertex(i1<<1, e.Second, v2);

  m_AddEdgeLock.Unlock();
}
//---------------------------------------------------------------------------------------------------
UINT MarchingOctreeExtractor::EmitTriangle(CONST INDEXEDTRIANGLE& Triangle)
{
  Lock<CriticalSection> L(m_OutputLock);
  m_pTriangles->Write(&Triangle, sizeof(Triangle));
  return m_TriangleCount++;
}
//---------------------------------------------------------------------------------------------------
UINT MarchingOctreeExtractor::EmitVertex(CONST Vector3F& Position, CONST Vector3F& Normal, CONST InterpolatedAttribute& CustomAttribute)
{
  Lock<CriticalSection> L(m_OutputLock);
  m_pVertices->Write(&Position, sizeof(Position));
  m_pVertices->Write(&Normal, sizeof(Normal));
  //m_pVertices->Write(&CustomAttribute, sizeof(CustomAttribute));

  return m_VertexCount++;
}
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
MarchingOctreeExtractor::IVertex MarchingOctreeExtractor::AddVertex(TreeNode& N, INT EdgeIndex, BOOL ForceFront, UINT RegionId, TreeNode::NeighborCache& Neighbours)
{
  UINT idx[3];
  INT Orientation;
  UINT i1;
  UINT i2;
  USHORT d = N.Depth();
  
  Cube::FactorEdgeIndex(EdgeIndex, Orientation, i1, i2);

  EDGEKEY EdgeKey = VertexData::EdgeKey(N, EdgeIndex, m_pTree->Height()-1, idx);

  #if defined(DEBUG)
  UINT off[3];
  UINT mishaIndex;
  LONGLONG mishaKey;
  VertexData::FactorEdgeKey(N.Depth(),m_pTree->Height()-1,EdgeKey,off,mishaIndex);
  mishaKey=VertexData::EdgeKey(N.Depth(),off,mishaIndex,m_pTree->Height()-1);
  if(EdgeKey != mishaKey)
  {
    printf("Badness: %lld != %lld\n",EdgeKey,mishaKey);
    USHORT d;
    printf("\tOut depth and offset: %d\t%d %d %d\n",N.Depth(),off[0],off[1],off[2]);
    Cube::FactorEdgeIndex(mishaIndex,Orientation,i1,i2);
    printf("\t\t%d\t%d %d\n",Orientation,i1,i2);
    N.GetDepthAndOffset(d,off);
    printf("\t In depth and offset: %d\t%d %d %d\n",N.Depth(),off[0],off[1],off[2]);
    Cube::FactorEdgeIndex(EdgeIndex,Orientation,i1,i2);
    printf("\t\t%d\t%d %d\n",Orientation,i1,i2);
    exit(0);
  }
  #endif

  {
    Lock<CriticalSection> Lock(m_AddVertexLock);
    Vertex V;
    if(Orientation == 0 && m_pIVD[d].GetVertex(1, EdgeKey, V))
    {
      return IVertex(V ,EdgeKey);
    }
    else if(ForceFront)
    {
      if(m_pIVD[d].GetVertex(2, EdgeKey, V))
        return IVertex(V ,EdgeKey);
    }
    else
    {
      if(m_pIVD[d].GetVertex(i1<<1, EdgeKey, V))
        return IVertex(V, EdgeKey);
    }

    #if defined(DEBUG)
    if(!ForceFront && Orientation != 0 && i1 == 0)
    {
      Trace("Adding unexpected iso-vertex %d %d %d\n", Orientation,i1,i2);
    }
    #endif
  }

  Vector3<FLOAT> Position;
  FLOAT Center[3], Width;
  INT Corner1;
  INT Corner2;
  Cube::EdgeCorners(EdgeIndex, Corner1, Corner2);
  
  FLOAT Value1;
  FLOAT Value2;
  Vector3F Normal1;
  Vector3F Normal2;
  FLOAT dx1,dx2;

  N.GetCenterAndWidth(Center, Width);

  CORNERKEY CornerKey = VertexData::CornerKey(N, Corner1, m_pTree->Height()-1, idx);
  if (ForceFront)
  {
    Assert(m_pIVD[d].ContainsValue(RegionId, 1, CornerKey));
    Value1 = m_pIVD[d].GetValue(RegionId, 1, CornerKey);

    if (!m_pIVD[d].GetNormal(RegionId, 1, CornerKey, Normal1))
    {
      Normal1 = GetCornerNormal(N, Corner1, Neighbours);
      m_pIVD[d].SetNormal(RegionId, 1, CornerKey, Normal1);
    }
  }
  else
  {
    Assert(m_pIVD[d].ContainsValue(RegionId, Corner1 & 1, CornerKey));
    Value1 = m_pIVD[d].GetValue(RegionId, Corner1 & 1, CornerKey);

    if (!m_pIVD[d].GetNormal(RegionId, Corner1 & 1, CornerKey, Normal1))
    {
      Normal1 = GetCornerNormal(N, Corner1, Neighbours);
      m_pIVD[d].SetNormal(RegionId, Corner1 & 1, CornerKey, Normal1);
    }
  }

  CornerKey = VertexData::CornerKey(N, Corner2, m_pTree->Height()-1, idx);

  if (ForceFront)
  {
    Assert(m_pIVD[d].ContainsValue(RegionId, 1, CornerKey));
    Value2 = m_pIVD[d].GetValue(RegionId, 1, CornerKey);

    if(!m_pIVD[d].GetNormal(RegionId, 1, CornerKey, Normal2))
    {
      Normal2 = GetCornerNormal(N,Corner2,Neighbours);
      m_pIVD[d].SetNormal(RegionId, 1, CornerKey, Normal2);
    }
  }
  else
  {
    Assert(m_pIVD[d].ContainsValue(RegionId, Corner2&1, CornerKey));
    Value2 = m_pIVD[d].GetValue(RegionId, Corner2&1, CornerKey);

    if (!m_pIVD[d].GetNormal(RegionId, Corner2&1, CornerKey, Normal2))
    {
      Normal2 = GetCornerNormal(N,Corner2,Neighbours);
      m_pIVD[d].SetNormal(RegionId, Corner2&1, CornerKey, Normal2);
    }
  }

  switch(Orientation)
  {
  case 0:
    Position[1] = Center[1] - Width/2 + Width*i1;
    Position[2] = Center[2] - Width/2 + Width*i2;
    break;
  case 1:
    Position[0] = Center[0] - Width/2 + Width*i1;
    Position[2] = Center[2] - Width/2 + Width*i2;
    break;
  case 2:
    Position[0] = Center[0] - Width/2 + Width*i1;
    Position[1] = Center[1] - Width/2 + Width*i2;
    break;
  }

  dx1 = Normal1[Orientation];
  dx2 = Normal2[Orientation];
  FLOAT ScaleFactor = (Value2-Value1) / ((dx2+dx1) / 2);
  dx1 *= ScaleFactor;
  dx2 *= ScaleFactor;

  // Hermite Spline
  PolynomialN<2,DOUBLE> P;
  P[0] = Value1;
  P[1] = dx1;
  P[2] = 3*(Value2-Value1) - dx2 - 2*dx1;

  // Get the solutions
  UINT RootCount = 0;
  DOUBLE AverageRoot = 0.0;

  Array<DOUBLE> Roots = P.GetSolutions(m_IsoValue);
  for(SIZE_T i=0; i<Roots.Length(); i++)
  {
    if(Roots[i] >=0 && Roots[i] <= 1)
    {
      AverageRoot += Roots[i];
      RootCount++;
    }
  }

  #if defined(DEBUG)
  if(!RootCount)
    TraceWarning(this, "No roots found for: %f [%f,%f]\n", m_IsoValue, Value1, Value2);
  #endif

  if(!RootCount)
    AverageRoot = (Value1 - m_IsoValue) / (Value1 - Value2);
  else
    AverageRoot /= RootCount;

  Position[Orientation] = (FLOAT)(Center[Orientation] - Width/2 + Width*AverageRoot);
  Position = m_Transform * Position;
  Vector3F Normal = Vector3F();
  InterpolatedAttribute CustomAttribute = InterpolatedAttributeEmpty;//GetEdgeCustomAttribute(N, Neighbours, Position);

  Vertex V = Vertex(Position, EmitVertex(Position, Normal, CustomAttribute));

  {
  Lock<CriticalSection> Lock(m_AddVertexLock);
  if(Orientation == 0)
    m_pIVD[d].SetVertex(1, EdgeKey, V);
  else if(ForceFront)
    m_pIVD[d].SetVertex(2, EdgeKey, V);
  else
    m_pIVD[d].SetVertex(i1<<1,EdgeKey,V);
  }

  return IVertex(V, EdgeKey);
}
//---------------------------------------------------------------------------------------------------
#if defined(CONFIG_CUSTOMATTRIBUTE)
InterpolatedAttribute MarchingOctreeExtractor::GetEdgeCustomAttribute(TreeNode& Node, TreeNode::NeighborCache& Neighbours, Vector3F Position)
{
	InterpolatedAttribute CustomAttribute = InterpolatedAttributeEmpty;
	DOUBLE WeightSum = 0.0;
	
  Neighbours.ComputeNeighbors(Node);

  for(USHORT i=0;i<=Node.Depth();i++)
  {
		for(UINT j=0;j<3;j++)
    {
			for(UINT k=0;k<3;k++)
      {
				for(UINT l=0;l<3;l++)
				{
          if (Neighbours(i,j,k,l) != TreeNode::INVALIDINDEX)
          {
            TreeNode NeighbouringNode = m_pTree->GetNode(i, Neighbours(i,j,k,l));

            DOUBLE Weight = 1.0;
						INT Depth, Offset[3];
						
            NeighbouringNode.GetDepthAndOffset(Depth, Offset);
						
            for(UINT m=0; m<3; m++)
						{
              UINT Index = BinaryNode<FLOAT>::Index(Depth, Offset[m]);
              Weight *= m_FDI.FunctionData.BaseFunctions[Index](Position[m]);
						}

            CustomAttribute += NeighbouringNode.CustomAttribute() * Weight;
            WeightSum += NeighbouringNode.CustomAttributeWeight() * Weight;
					}
				}
      }
    }
  }
	
  CustomAttribute *= (1.0 / WeightSum);
  return CustomAttribute;
}
#endif
//---------------------------------------------------------------------------------------------------
