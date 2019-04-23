/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/StreamingOctreeConstructor.cpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 888 $
 * Last Updated: $Date: 2008-11-20 10:59:50 -0800 (Thu, 20 Nov 2008) $
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

#include "StreamingOctreeConstructor.hpp"
#include "StreamingOctreeTraversal.hpp"

#include "Math/Math.hpp"
using Bolitho::Math::Pow;
using Bolitho::Math::Log10;
using Bolitho::Math::KahanSumStep;

using Bolitho::Clamp;

//#define KahanSumStep(x,y,z) x += z

#include "System/Threading/Interlocked.hpp"
using Bolitho::InterlockedAccumulate;

//---------------------------------------------------------------------------------------------------
StreamingOctreeConstructor::UnprocessedPoint::UnprocessedPoint()
{
  m_Weight = -1.0f;
  m_pNode = 0;
}
//---------------------------------------------------------------------------------------------------
StreamingOctreeConstructor::UnprocessedPoint::UnprocessedPoint(Vector3F& Position, Vector3F& Normal, InterpolatedAttribute& CustomAttribute, FLOAT Weight, OctreeNode* pNode)
{
  m_Position = Position;
  m_Normal = Normal;
  m_CustomAttribute = CustomAttribute;
  m_Weight = Weight;
  m_pNode = pNode;
}
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
StreamingOctreeConstructor::PerDepthData::PerDepthData()
{
  for (UINT i=0; i<CONFIG_MAXTHREADS; i++)
  {
    m_NodeAllocators[i].Resize(6);
    for (SIZE_T j=0; j<m_NodeAllocators[i].Length(); j++)
      m_NodeAllocators[i][j].SetBlockSize(1024);
    m_UnprocessedPoints[i].Resize(2);
  }

}
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
VOID StreamingOctreeConstructor::PointFunction::ProcessPoint(Tree* pTree, Vector3F Position, Vector3F Normal, THREADLOCAL& L)
{
  FLOAT Width = 1.0f;
  Vector3F Center(0.5f, 0.5f, 0.5f);

  #if defined(DEBUG)
  if(Position[0] < 0 || Position[0] > 1 ||
     Position[1] < 0 || Position[1] > 1 ||
     Position[2] < 0 || Position[2] > 1)
    throw InvalidOperationException(pTree, String::Format("The point %s falls out of bounds", Position.ToString().ConstStr()));
  #endif

  OctreeNode* pNode = m_pRoot;
  for(INT d=0; d<pTree->MaximumDepth(); d++)
  {
    INT CornerIndex = OctantIndex(Center, Position);

    pNode = pNode->GetChild(CornerIndex);
    Assert(pNode != 0);

    Width /= 2;

    if(CornerIndex & 1)
      Center[0] += Width/2;
    else
      Center[0] -= Width/2;

    if(CornerIndex & 2)
      Center[1] += Width/2;
    else
      Center[1] -= Width/2;

    if(CornerIndex & 4)
      Center[2] += Width/2;
    else
      Center[2] -= Width/2;
  }

  Normal *= (FLOAT)(2 << pTree->MaximumDepth());

  InterpolatedAttribute Attribute;
  m_pPerDepthData[pTree->MaximumDepth()-1].m_UnprocessedPoints[L.ThreadId][m_Slice >> 1].Add(UnprocessedPoint(Position, Normal, Attribute, -1.0f, pNode));
}
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
VOID StreamingOctreeConstructor::WeightFunction::ProcessPoint(Tree* pTree, Vector3F& Position, Vector3F& Normal, THREADLOCAL& L)
{
  FLOAT Width = 1.0f;
  Vector3F Center(0.5f, 0.5f, 0.5f);

  #if defined(DEBUG)
  if(Position[0] < 0 || Position[0] > 1 ||
     Position[1] < 0 || Position[1] > 1 ||
     Position[2] < 0 || Position[2] > 1)
    throw InvalidOperationException(pTree, String::Format("The point %s falls out of bounds", Position.ToString().ConstStr()));
  #endif

  OctreeNode* pNode = m_pRoot;
  for(INT d=0; d<pTree->MaximumDepth(); d++)
  {
    INT CornerIndex = OctantIndex(Center, Position);
    
    pNode = pNode->GetOrCreateChild(CornerIndex, m_pSOC);

    Width /= 2;

    if(CornerIndex & 1)
      Center[0] += Width/2;
    else
      Center[0] -= Width/2;

    if(CornerIndex & 2)
      Center[1] += Width/2;
    else
      Center[1] -= Width/2;

    if(CornerIndex & 4)
      Center[2] += Width/2;
    else
      Center[2] -= Width/2;
  }

  UpdateWeightContribution(pNode, Position, 1.0f, L);

}
//---------------------------------------------------------------------------------------------------
VOID StreamingOctreeConstructor::WeightFunction::UpdateWeightContribution(OctreeNode* pNode, CONST Vector3F& Position, FLOAT Weight, THREADLOCAL& L)
{
  L.Neighbors.CreateNeighbors(pNode, m_pSOC);

  Vector3<DOUBLE> dx[3];
  FLOAT Width;
  Vector3F Center;

  for(INT d = pNode->Depth(); d>=0; d--)
  {
    OctreeNode::Neighbors& Neighbors = L.Neighbors(d);
    Neighbors(1,1,1)->GetCenterAndWidth(Center,Width);

    for(UINT i=0; i<3; i++)
      m_pSOC->m_FDI.GetWeights((Position[i]-Center[i]) / Width, dx[i]);

    for(UINT i=0; i<3; i++)
    {
      for(UINT j=0; j<3; j++)
      {
        DOUBLE dxdy = dx[0](i) * dx[1](j) * Weight;
        for(UINT k=0; k<3; k++)
        {
          if(Neighbors(i,j,k))
            Neighbors(i,j,k)->CenterWeightContribution(L.ThreadId) += (FLOAT)(dxdy * dx[2](k));
        }
      }
    }
  }

}
//---------------------------------------------------------------------------------------------------




//---------------------------------------------------------------------------------------------------
StreamingOctreeConstructor::StreamingOctreeConstructor(Ptr<Tree> pTree, FLOAT SamplesPerNode, BOOL ClipTree, FLOAT SampleRadius) 
  : m_UnprocessedPointFunction(m_FDI, SamplesPerNode),
    m_NormalFunction(m_FDI, pTree->Height()),
    m_WriteNodesFunction(pTree, ClipTree),
    m_WeightFunction(&m_Root),
    m_PointFunction(&m_Root)
{
  m_pTree = pTree;

  m_pPerDepthData = new PerDepthData[pTree->Height()+1];
  m_UnprocessedPointFunction.m_pPerDepthData = m_pPerDepthData;
  m_PointFunction.m_pPerDepthData = m_pPerDepthData;
  m_InterpolationAttributeName = m_InterpolationAttributeName;
  m_WeightFunction.m_pSOC = this;

  m_FDI.Initialize(pTree->Height(), SampleRadius);
  m_FDI.FunctionData.InitializeDotTables(FunctionDataUsage::DOT | FunctionDataUsage::DDOT);
  m_FDI.FunctionData.InitializeValueTables(FunctionDataUsage::VALUE);

  m_pTree->Initialise(0, 2, DataAccess::WRITE | DataAccess::AUTOGROW, DataAccess::WRITE | DataAccess::AUTOGROW, DataAccess::READ);
}
//---------------------------------------------------------------------------------------------------
StreamingOctreeConstructor::~StreamingOctreeConstructor()
{
  if (m_pPerDepthData)
  {
    delete[] m_pPerDepthData;
    m_pPerDepthData = 0;
  }
}
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
DOUBLE StreamingOctreeConstructor::UnprocessedPointFunction::GetSampleWeight(OctreeNode* pNode, CONST Vector3F& Position, THREADLOCAL& L)
{
  OctreeNode::Neighbors& Neighbors = L.Neighbors.ComputeNeighbors(pNode);

  DOUBLE Weight = 0;
  DOUBLE dx[3][3];

  Vector3F Center;
  FLOAT Width;
  pNode->GetCenterAndWidth(Center, Width);

  for(UINT i=0; i<3; i++)
  {
    DOUBLE x = (Center[i]-Position[i]-Width)/Width;
    dx[i][0] = 1.125 + 1.500*x + 0.500*x*x;
    x = (Center[i]-Position[i])/Width;
    dx[i][1] = 0.750 - x*x;
    dx[i][2] = 1.0 - dx[i][1] - dx[i][0];
  }

  for(UINT i=0; i<3; i++)
  {
    for(UINT j=0; j<3; j++)
    {
      DOUBLE dxdy = dx[0][i] * dx[1][j];
      for(UINT k=0; k<3; k++)
      {
        if(Neighbors(i,j,k))
          Weight += dxdy * dx[2][k] * Neighbors(i,j,k)->CenterWeightContribution();
      }
    }
  }

  return Weight;
}
//---------------------------------------------------------------------------------------------------
VOID StreamingOctreeConstructor::UnprocessedPointFunction::ProcessPoint(UINT Slice, USHORT Depth, UnprocessedPoint& Point, THREADLOCAL& L)
{
  if(Point.m_Weight < 0)
  {
    DOUBLE Weight = GetSampleWeight(Point.m_pNode, Point.m_Position, L);
    if(Weight >= m_SamplesPerNode+1)
    {
      DOUBLE Depth = Point.m_pNode->Depth() + Log10(Weight / (m_SamplesPerNode + 1)) / Log10(4.0);
      DOUBLE Wt = Pow(4.0, -DOUBLE(Depth) + Point.m_pNode->Depth() * 1.5);

      Point.m_Normal *= (FLOAT)Wt;
      Point.m_CustomAttribute *= (FLOAT)Wt;

      SplatOrientedPoint(Point.m_pNode, Point.m_Position, Point.m_Normal, Point.m_CustomAttribute, (FLOAT)Wt, L);
      return;
    }
    else
      Point.m_Weight = (FLOAT)Weight;
  }

  Assert(Point.m_pNode->GetParent() != 0);

  DOUBLE Weight = GetSampleWeight(Point.m_pNode->GetParent(), Point.m_Position, L);

  // If we can add the point and normal to the tree do so
  if(Weight >= m_SamplesPerNode+1)
  {
    Vector3F Normal;
    InterpolatedAttribute CustomAttribute;

    DOUBLE Depth = Point.m_pNode->Depth() - 1 + Log10(Weight / (m_SamplesPerNode+1)) / Log10(Weight / Point.m_Weight);
    FLOAT dx = (FLOAT)Depth - (Point.m_pNode->Depth()-1);

    dx = Clamp(dx, 0.0f, 1.0f);

    DOUBLE Wt = Pow(4.0, -DOUBLE(Depth) + Point.m_pNode->Depth() * 1.5);
    Normal = Point.m_Normal * (FLOAT)Wt * dx;
    CustomAttribute = Point.m_CustomAttribute * (FLOAT)Wt * dx;

    SplatOrientedPoint(Point.m_pNode, Point.m_Position, Normal, CustomAttribute, (FLOAT)Wt * dx, L);

    Wt = Pow(4.0, -DOUBLE(Depth) + (Point.m_pNode->Depth()-1) * 1.5);
    Normal = Point.m_Normal * (FLOAT)Wt * (1.0f-dx);
    CustomAttribute = Point.m_CustomAttribute * (FLOAT)Wt * (1.0f-dx);
    SplatOrientedPoint(Point.m_pNode->GetParent(), Point.m_Position, Normal, CustomAttribute, (FLOAT)Wt * (1.0f-dx), L);
  }
  // Otherwise, pass it down the tree
  else
  {
    if(Depth > 0)
    {
      //m_Lock.Lock();
      //Trace("%u", L.ThreadId);
      m_pPerDepthData[Depth-1].m_UnprocessedPoints[L.ThreadId][Slice >> 1].Add(UnprocessedPoint(Point.m_Position, Point.m_Normal, Point.m_CustomAttribute, (FLOAT)Weight, Point.m_pNode->GetParent()));
      //m_Lock.Unlock();
    }
    else
      throw InvalidOperationException(0, "Could not process the point.  It has reached the root of the tree.");
  }

}
//---------------------------------------------------------------------------------------------------
VOID StreamingOctreeConstructor::UnprocessedPointFunction::SplatOrientedPoint(OctreeNode* pNode, CONST Vector3F& Position, CONST Vector3F& Normal, CONST InterpolatedAttribute& CustomAttribute, FLOAT CustomAttributeWeight, THREADLOCAL& L)
{
  OctreeNode::Neighbors& Neighbors = L.Neighbors.ComputeNeighbors(pNode);

  Vector3<DOUBLE> dx[3];
  Vector3F Center;
  FLOAT Width;
  pNode->GetCenterAndWidth(Center,Width);

  for(UINT i=0; i<3; i++)
    m_FDI.GetWeights((Position[i]-Center[i]) / Width, dx[i]);

  for(UINT i=0; i<3; i++)
  {
    for(UINT j=0; j<3; j++)
    {
      DOUBLE dxdy = dx[0](i) * dx[1](j);
      for(UINT k=0; k<3; k++)
      {
        if(Neighbors(i,j,k))
        {
          DOUBLE dxdydz = dxdy * dx[2](k);
          Neighbors(i,j,k)->Normal(L.ThreadId) += Normal * (FLOAT)dxdydz;
          Neighbors(i,j,k)->CustomAttribute(L.ThreadId) += CustomAttribute * (FLOAT)dxdydz;
          Neighbors(i,j,k)->CustomAttributeWeight(L.ThreadId) += CustomAttributeWeight * (FLOAT)dxdydz;
        }
      }
    }
  }
}
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
VOID StreamingOctreeConstructor::ProcessPoints(USHORT Depth, INT Slice)
{
  if(Slice < 0)
    return;

//  TraceDEnter("%d, %d", Depth, Slice);

  IterateUnprocessedPoints(Slice, Depth, m_UnprocessedPointFunction);

  for (UINT i=0; i<CONFIG_MAXTHREADS; i++)
    m_pPerDepthData[Depth].m_UnprocessedPoints[i][Slice].Resize(0);
}
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
VOID StreamingOctreeConstructor::NormalFunction::ProcessNode(UINT Slice, USHORT Depth, OctreeNode* pNode, THREADLOCAL& L)
{
  Vector3F Normal = pNode->Normal();

  if(Normal.SquareLength() <= MINFLOAT)
  {
    pNode->Normal().SetZero();
    return;
  }

  USHORT d;
  UINT off[3];

  DOUBLE Length = Normal.Length();
  DOUBLE WeightCorrection = 1.0 / Pow(4.0, Depth*1.5);

  Length *= WeightCorrection;
  m_WeightSum[L.ThreadId] += Length;

  pNode->GetDepthAndOffset(d,off);

  UINT CenterIndex[3];
  VertexData::CenterKey(d, off, m_MaxDepth, CenterIndex);

  INT start[3];
  INT end[3];
  UINT cIndex[3];

  DOUBLE vXY;
  DOUBLE dotXY;
  DOUBLE dDotXY;

  DOUBLE v[3][5];
  DOUBLE dot[3][5];
  DOUBLE dDot[3][5];

  start[0] = start[1] = start[2] = 0;
  end[0] = end[1] = end[2] = 5;

  L.Neighbors2.ComputeNeighbors(pNode);


  for(INT d = pNode->Depth(); d>=0; d--)
  {

    OctreeNode* node = L.Neighbors2(d,2,2,2);

    for(INT ii=0; ii<3; ii++)
    {
      ZeroMemory(v[ii], sizeof(DOUBLE)*5);
      ZeroMemory(dot[ii], sizeof(DOUBLE)*5);
      ZeroMemory(dDot[ii], sizeof(DOUBLE)*5);

      for(INT jj=0; jj<5; jj++)
      {
        INT idx = node->Offset(ii) - 2 + jj;
        INT tIdx = idx - ((1<<d)-1);
        if(tIdx < (1 << d))
        {
          dot[ii][jj] = m_FDI.FunctionData.DotTables[d](pNode->Offset(ii),idx);
          dDot[ii][jj] = m_FDI.FunctionData.DDotTables[d](pNode->Offset(ii),idx);
        }
      }

      for(INT jj=1; jj<4; jj++)
      {
        INT idx = node->Offset(ii) - 2 + jj;
        INT tIdx = idx - ((1<<d) - 1);
        if(tIdx < (SIZE_T)(1<<d))
          v[ii][jj] = m_FDI.FunctionData.ValueTables(idx, CenterIndex[ii]);
      }

    }

    for(INT ii=start[0]; ii < end[0]; ii++)
    {
      for(INT jj=start[1]; jj < end[1]; jj++)
      {
        vXY = v[0][ii] * v[1][jj];
        dotXY = dot[0][ii] * dot[1][jj];
        dDotXY = dDot[0][ii] * Normal[0] + dDot[1][jj] * Normal[1];

        for(INT kk=start[2]; kk < end[2]; kk++)
        {
          if(L.Neighbors2(d,ii,jj,kk))
          {
            OctreeNode* pNode = L.Neighbors2(d,ii,jj,kk);
            pNode->Value(L.ThreadId) += dotXY * dot[2][kk] * (dDotXY + dDot[2][kk] * Normal[2]);
            pNode->WeightContribution(L.ThreadId) += Length * vXY * v[2][kk];
          }
        }
      }

      if(d > 0)
      {
        Cube::FactorCornerIndex(node->GetChildIndex(), cIndex[0], cIndex[1], cIndex[2]);
        for(SIZE_T ii=0; ii<3; ii++)
        {
          if (cIndex[ii])
            start[ii] = 1;
          else
            end[ii] = 4;
        }
      }
    }
  }

}
//---------------------------------------------------------------------------------------------------
VOID StreamingOctreeConstructor::ProcessNormals(USHORT Depth, INT Slice)
{
  if(Slice < 0)
    return;

//  TraceDEnter("%d, %d", Depth, Slice);

  IterateNodes(Slice, Depth, FALSE, m_NormalFunction);
}
//---------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------
NodeAllocator& StreamingOctreeConstructor::GetAllocator(OctreeNode* N, UINT ThreadId)
{
  INT d;
  INT off[3];

  N->GetDepthAndOffset(d,off);

  return m_pPerDepthData[d].m_NodeAllocators[ThreadId][off[0]];
}

//---------------------------------------------------------------------------------------------------
VOID StreamingOctreeConstructor::WriteNodesFunction::ProcessNode(UINT Slice, USHORT Depth, OctreeNode* pNode, THREADLOCAL& L)
{
  /* If we are clipping the tree, skip nodes with a small weight contribution */
  if(m_ClipTree)
  {
    BOOL WriteNodes = FALSE;
    for(INT j=0; j<8 && !WriteNodes; j++)
      if(pNode[j].WeightContribution() > MINFLOAT)
        WriteNodes = TRUE;

    if(!WriteNodes)
    {
      m_ClippedNodes++;
      pNode->GetParent()->DeleteChildren();
      return;
    }
  }

  /* Copy the brood into the streaming format */
  for(INT j=0; j<8; j++)
  {
    /* Tree Topology */
    L.Partition0[j].Children = pNode[j].GetChildrenIndex();
    L.Partition0[j].Depth = pNode[j].Depth();
    L.Partition0[j].Offset[0] = pNode[j].Offset(0);
    L.Partition0[j].Offset[1] = pNode[j].Offset(1);
    L.Partition0[j].Offset[2] = pNode[j].Offset(2);

    /* Tree Data */
    L.Partition2[j].WeightContribution = pNode[j].WeightContribution();
    L.Partition1[j].Normal = pNode[j].Normal();
    L.Partition4[j].Value = pNode[j].Value();
    L.Partition5[j].Solution = 0.0f;
    #if defined(CONFIG_CUSTOMATTRIBUTE)
    L.Partition1[j].CustomAttribute = pNode[j].CustomAttribute();
    L.Partition2[j].CustomAttributeWeight = pNode[j].CustomAttributeWeight();
    #endif
  }
  
  //m_Lock.Lock();
  /* Write the nodes into the stream, and set their parent's ChildrenIndex to point to them */
  pNode->GetParent()->SetChildrenIndex(m_pTree->InsertBrood(Depth+1, L.Partition0, L.Partition1, L.Partition2, L.Partition3, L.Partition4, L.Partition5));
  pNode->GetParent()->DeleteChildren();
  //m_Lock.Unlock();
}
//---------------------------------------------------------------------------------------------------
VOID StreamingOctreeConstructor::WriteOutNodes(USHORT Depth, INT Slice)
{
  if (Slice < 0)
    return;

  IterateNodes(Slice, Depth, TRUE, m_WriteNodesFunction);

  for (UINT i=0; i<CONFIG_MAXTHREADS; i++)
  {
    NodeAllocator& Allocator = m_pPerDepthData[Depth].m_NodeAllocators[i][Slice];
    Allocator.DeleteAll();
  }
  
  m_pTree->FixupNodeIndexOffsets(Depth+1);
  m_pTree->AdvanceNodes(Depth+1, 2);
}

//---------------------------------------------------------------------------------------------------
VOID StreamingOctreeConstructor::InitConstructor()
{
  #if defined(CONFIG_PROFILE)
  TimeAllFunctionCalls();
  #endif

  m_pTree->IteratePoints(0, m_WeightFunction);

  m_PointFunction.m_Slice = 0;

  m_pTree->IteratePoints(0, m_PointFunction);

  m_pTree->AdvancePoints(2);
}
//---------------------------------------------------------------------------------------------------
VOID StreamingOctreeConstructor::UpdateConstructor(UINT Slice)
{
  #if defined(CONFIG_PROFILE)
  TimeAllFunctionCalls();
  #endif

  if(Slice & 1)
    return;

  ThreadProfileFunction("%u", Slice);

  m_pTree->IteratePoints(0, m_WeightFunction);

  INT Depth = m_pTree->MaximumDepth();
  INT Slc = Slice;
  do
  {
    Slice >>= 1;
    Depth--;
    ProcessPoints(Depth, Slice-1);
    ProcessNormals(Depth, Slice-2);
    WriteOutNodes(Depth, Slice-3);
  }
  while(Slice & 1);

  m_PointFunction.m_Slice = Slc + 2;
  m_pTree->IteratePoints(0, m_PointFunction);

  m_pTree->AdvancePoints(2);

}
//---------------------------------------------------------------------------------------------------
VOID StreamingOctreeConstructor::FinalizeConstructor()
{
  #if defined(CONFIG_PROFILE)
  TimeAllFunctionCalls();
  #endif

  for(INT Depth = m_pTree->MaximumDepth()-1; Depth>=0; Depth--)
  {
    ProcessPoints(Depth, (1<<Depth)-1);
    ProcessNormals(Depth,(1<<Depth)-2);
    ProcessNormals(Depth,(1<<Depth)-1);

    WriteOutNodes(Depth,(1<<Depth)-3);
    WriteOutNodes(Depth,(1<<Depth)-2);
    WriteOutNodes(Depth,(1<<Depth)-1);
  }

  PARTITION0 Partition0;
  PARTITION1 Partition1;
  PARTITION2 Partition2;
  PARTITION3 Partition3;
  PARTITION4 Partition4;
  PARTITION5 Partition5;

  Partition0.Children = m_Root.GetChildrenIndex();
  Partition0.Depth = m_Root.Depth();
  Partition0.Offset[0] = m_Root.Offset(0);
  Partition0.Offset[1] = m_Root.Offset(1);
  Partition0.Offset[2] = m_Root.Offset(2);

  Partition2.WeightContribution = m_Root.WeightContribution();
  Partition1.Normal = m_Root.Normal();
  Partition4.Value = m_Root.Value();
  Partition5.Solution = 0;

  m_pTree->InsertRoot(Partition0, Partition1, Partition2, Partition3, Partition4, Partition5);
  m_pTree->Finalise();

  m_pTree->m_State.WeightSum = Sum(&m_NormalFunction.m_WeightSum[0], CONFIG_MAXTHREADS);

  Trace("Weight Sum: %lf %llx", m_pTree->m_State.WeightSum, m_pTree->m_State.WeightSum);
  Trace("Clipped Nodes: %u", m_WriteNodesFunction.m_ClippedNodes);

}
//---------------------------------------------------------------------------------------------------
INT StreamingOctreeConstructor::OctantIndex(CONST Vector3F& Center, CONST Vector3F& P)
{
  INT OctantIndex = 0;
  if(P[0] > Center[0])
    OctantIndex |= 1;
  if(P[1] > Center[1])
    OctantIndex |= 2;
  if(P[2] > Center[2])
    OctantIndex |= 4;

  return OctantIndex;
}
//---------------------------------------------------------------------------------------------------
VOID StreamingOctreeConstructor::GetSliceNodeList(OctNode* pNode, UINT CurrentSlice, UINT Width, UINT Slice, USHORT MaxDepth, BOOL IterateBroods, Array<OctNode*>& Nodes)
{
  if (!pNode->HasChildren())
    return;

  if (pNode->Depth() == MaxDepth)
  {
    UINT MaxIteration = IterateBroods ? 1 : 8;
    for (UINT i=0; i<MaxIteration; i++)
      Nodes.Add(pNode->GetChild(i));
    return;
  }
  
  OctNode* pChildren = pNode->GetChild(0);
  if (Slice < CurrentSlice)
  {
    GetSliceNodeList(&pChildren[0], CurrentSlice - Width, Width >> 1, Slice, MaxDepth, IterateBroods, Nodes);
    GetSliceNodeList(&pChildren[2], CurrentSlice - Width, Width >> 1, Slice, MaxDepth, IterateBroods, Nodes);
    GetSliceNodeList(&pChildren[4], CurrentSlice - Width, Width >> 1, Slice, MaxDepth, IterateBroods, Nodes);
    GetSliceNodeList(&pChildren[6], CurrentSlice - Width, Width >> 1, Slice, MaxDepth, IterateBroods, Nodes);
  }
  else
  {
    GetSliceNodeList(&pChildren[1], CurrentSlice + Width, Width >> 1, Slice, MaxDepth, IterateBroods, Nodes);
    GetSliceNodeList(&pChildren[3], CurrentSlice + Width, Width >> 1, Slice, MaxDepth, IterateBroods, Nodes);
    GetSliceNodeList(&pChildren[5], CurrentSlice + Width, Width >> 1, Slice, MaxDepth, IterateBroods, Nodes);
    GetSliceNodeList(&pChildren[7], CurrentSlice + Width, Width >> 1, Slice, MaxDepth, IterateBroods, Nodes);
  }

}
//---------------------------------------------------------------------------------------------------

