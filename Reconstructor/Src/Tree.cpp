/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/Tree.cpp $
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

#include "Config.hpp"
#include "Tree.hpp"
#include "MarchingCubes.hpp"

#include "Utility/Bits.hpp"
using Bolitho::Interleave;
using Bolitho::IO::BinaryStream;
using Bolitho::Array;
using Bolitho::Parse;

//----------------------------------------------------------------------------------------------------
Ptr<Tree> Tree::Create(LPCTSTR InputFilename, LPCTSTR StreamingFilename, LPCTSTR InterpolationAttributeName, USHORT MaximumDepth, BOOL DeleteOnClose)
{
  Tree* pTree = new Tree(StreamingFilename);
  pTree->m_State.TreeHeight = MaximumDepth+1;
  pTree->m_State.IsoValue = 0.0f;
  pTree->m_State.InterpolationAttributeOffset = 0;
  pTree->m_State.WeightSum = 0.0f;
  pTree->m_StreamManager.Create(InputFilename, StreamingFilename, MaximumDepth, DeleteOnClose);

  return pTree;
}
//----------------------------------------------------------------------------------------------------
/*
Ptr<Tree> Tree::Open(LPCTSTR StreamingFilename, BOOL DeleteOnCompletion)
{
  DWORD DeleteFlag = (DeleteOnCompletion) ? FileFlags::DELETEONCLOSE : 0;

  Tree* pTree = new Tree(StreamingFilename);

  pTree->m_pStateFile = File::Open(StreamingFilename, FileMode::OPEN, FileAccess::READWRITE, FileShare::NONE, DeleteFlag);
  BinaryStream S(pTree->m_pStateFile);
  S.Read(&pTree->m_State, sizeof(pTree->m_State));
  String InputFilename;
  S >> InputFilename;

  pTree->m_pPartition0 = new DataStream[pTree->m_State.TreeHeight];
  pTree->m_pPartition1 = new DataStream[pTree->m_State.TreeHeight];
  pTree->m_pPartition2 = new DataStream[pTree->m_State.TreeHeight];
  pTree->m_pPartition3 = new DataStream[pTree->m_State.TreeHeight];
  pTree->m_pPartition4 = new DataStream[pTree->m_State.TreeHeight];
  pTree->m_pPartition5 = new DataStream[pTree->m_State.TreeHeight];

  for (USHORT i=0; i<pTree->m_State.TreeHeight; i++)
  {
    pTree->m_pPartition0[i].SetBackingStore(File::Open(String::Format("%s.Partition%d-0", StreamingFilename, i), FileMode::OPEN, FileAccess::READWRITE, FileShare::NONE, DeleteFlag | FileFlags::SEQUENTIAL), sizeof(PARTITION0));
    pTree->m_pPartition1[i].SetBackingStore(File::Open(String::Format("%s.Partition%d-1", StreamingFilename, i), FileMode::OPEN, FileAccess::READWRITE, FileShare::NONE, DeleteFlag | FileFlags::SEQUENTIAL), sizeof(PARTITION1));
    pTree->m_pPartition2[i].SetBackingStore(File::Open(String::Format("%s.Partition%d-2", StreamingFilename, i), FileMode::OPEN, FileAccess::READWRITE, FileShare::NONE, DeleteFlag | FileFlags::SEQUENTIAL), sizeof(PARTITION2));
    pTree->m_pPartition3[i].SetBackingStore(File::Open(String::Format("%s.Partition%d-3", StreamingFilename, i), FileMode::OPEN, FileAccess::READWRITE, FileShare::NONE, DeleteFlag | FileFlags::SEQUENTIAL), sizeof(PARTITION3));
    pTree->m_pPartition4[i].SetBackingStore(File::Open(String::Format("%s.Partition%d-4", StreamingFilename, i), FileMode::OPEN, FileAccess::READWRITE, FileShare::NONE, DeleteFlag | FileFlags::SEQUENTIAL), sizeof(PARTITION4));
    pTree->m_pPartition5[i].SetBackingStore(File::Open(String::Format("%s.Partition%d-5", StreamingFilename, i), FileMode::OPEN, FileAccess::READWRITE, FileShare::NONE, DeleteFlag | FileFlags::SEQUENTIAL), sizeof(PARTITION5));
  }

  Ptr<File> pNodeIndex = File::Open(String::Format("%s.NodeIndex", StreamingFilename), FileMode::OPEN, FileAccess::READ, FileShare::NONE, DeleteFlag);
  pTree->m_NodeIndex = new SPATIALINDEX[pNodeIndex->Length() / sizeof(SPATIALINDEX)];
  pNodeIndex->Read(pTree->m_NodeIndex, pNodeIndex->Length());

  Ptr<File> pPointIndex = File::Open(String::Format("%s.PointIndex", Path::Basename(InputFilename).ConstStr()), FileMode::OPEN, FileAccess::READ);
  pTree->m_PointIndex = new SPATIALINDEX[pPointIndex->Length() / sizeof(SPATIALINDEX)];
  pPointIndex->Read(pTree->m_PointIndex, pPointIndex->Length());

  pTree->m_PointData.SetBackingStore(File::Open(InputFilename, FileMode::OPEN, FileAccess::READWRITE, FileShare::READWRITE, FileFlags::SEQUENTIAL), sizeof(ORIENTEDPOINT));

  return pTree;
}
*/
//----------------------------------------------------------------------------------------------------
Tree::Tree(LPCTSTR StreamingFilename)
{
  m_StreamingFilename = StreamingFilename;
  m_pCurrentNodeIndex = NULL;

  #if defined(CONFIG_MULTITHREADED)
  m_ThreadCount = Configuration::GetValue<LONG>("MaxThreads");
  m_IteratePool.Initialize(m_ThreadCount, m_ThreadCount);
  if (Configuration::IsPresent("Affinity"))
  {
    String Affinity = Configuration::GetValue<String>("Affinity");
    Array<String> AffinityList = Affinity.Split(',');

    for (SIZE_T i=0; i<AffinityList.Length(); i++)
    {
      DWORD Mask = Parse<DWORD>(AffinityList[i]);
      m_IteratePool.SetThreadAffinity((UINT)i, Mask);
    }
  }

  m_IteratePool.Start();
  #endif

}
//----------------------------------------------------------------------------------------------------
Tree::~Tree()
{
}
//----------------------------------------------------------------------------------------------------
Md5Hash Tree::ComputePointSetHash()
{
  Md5 Hash;
  DataStream& PointData = m_StreamManager.GetPointStream();

  PointData.Initialize(DataAccess::READ);
  for (ULONGLONG i = 0; i<PointData.Count(); i++)
  {
    PointData.SetHeadPosition(i+1);
    PointData.SetTailPosition(i);
    Hash.Update(&PointData.Get<ORIENTEDPOINT>(i), sizeof(ORIENTEDPOINT));    
  }

  return Hash.Value();
}
//----------------------------------------------------------------------------------------------------
Md5Hash Tree::ComputePointIndexHash() CONST
{
  Md5 Hash;
  Hash.Update(m_StreamManager.GetPointIndex(), m_StreamManager.GetIndexCount());
  return Hash.Value();
}
//----------------------------------------------------------------------------------------------------
TreeNode::TreeNode()
{
  m_pPartition0 = 0;
  m_pPartition1 = 0;
  m_pPartition2 = 0;
  m_pPartition3 = 0;
  m_pPartition4 = 0;
  m_pPartition5 = 0;
  #if defined(CONFIG_STOREDNEIGHBOURS)
  m_pPartition6 = 0;
  #endif

  m_pTree = 0;
}
//----------------------------------------------------------------------------------------------------
TreeNode::TreeNode(CONST TreeNode& N)
{
  m_pPartition0 = N.m_pPartition0;
  m_pPartition1 = N.m_pPartition1;
  m_pPartition2 = N.m_pPartition2;
  m_pPartition3 = N.m_pPartition3;
  m_pPartition4 = N.m_pPartition4;
  m_pPartition5 = N.m_pPartition5;
  #if defined(CONFIG_STOREDNEIGHBOURS)
  m_pPartition6 = N.m_pPartition6;
  #endif
  m_pTree = N.m_pTree;
}

//----------------------------------------------------------------------------------------------------
VOID TreeNode::GetDepthAndOffset(INT& Depth, INT Offset[]) CONST
{
  Depth = this->Depth();

  Offset[0] = (OffsetX()+1) & (~(1 << Depth));
  Offset[1] = (OffsetY()+1) & (~(1 << Depth));
  Offset[2] = (OffsetZ()+1) & (~(1 << Depth));
}
//----------------------------------------------------------------------------------------------------
VOID TreeNode::GetDepthAndOffset(USHORT& Depth, UINT Offset[]) CONST
{
  Depth = this->Depth();

  Offset[0] = (OffsetX()+1) & (~(1 << Depth));
  Offset[1] = (OffsetY()+1) & (~(1 << Depth));
  Offset[2] = (OffsetZ()+1) & (~(1 << Depth));
}
//----------------------------------------------------------------------------------------------------
VOID TreeNode::GetCenterAndWidth(FLOAT Center[3], FLOAT& Width) CONST
{
  INT Depth,Offset[3];

  Depth = this->Depth();
  Offset[0] = (INT(OffsetX())+1)&(~(1<<Depth));
  Offset[1] = (INT(OffsetY())+1)&(~(1<<Depth));
  Offset[2] = (INT(OffsetZ())+1)&(~(1<<Depth));

  Width = 1.0f / (1<<Depth);
  Center[0] = FLOAT(0.5+Offset[0])*Width;
  Center[1] = FLOAT(0.5+Offset[1])*Width;
  Center[2] = FLOAT(0.5+Offset[2])*Width;
}
//----------------------------------------------------------------------------------------------------
BOOL TreeNode::Child(TreeNode& Node, USHORT i) CONST
{
  if (Children() == INVALIDINDEX)
    return FALSE;

  USHORT Depth = this->Depth()+1;
  ULONGLONG Offset = Children()+i;

  Node.m_pPartition0 = &m_pTree->GetNodeStream(Depth, 0).Get<PARTITION0>(Offset);
  Node.m_pPartition1 = &m_pTree->GetNodeStream(Depth, 1).Get<PARTITION1>(Offset);
  Node.m_pPartition2 = &m_pTree->GetNodeStream(Depth, 2).Get<PARTITION2>(Offset);
  Node.m_pPartition3 = &m_pTree->GetNodeStream(Depth, 3).Get<PARTITION3>(Offset);
  Node.m_pPartition4 = &m_pTree->GetNodeStream(Depth, 4).Get<PARTITION4>(Offset);
  Node.m_pPartition5 = &m_pTree->GetNodeStream(Depth, 5).Get<PARTITION5>(Offset);
  #if defined(CONFIG_STOREDNEIGHBOURS)
  Node.m_pPartition6 = &m_pTree->GetNodeStream(Depth, 6).Get<PARTITION6>(Offset);
  #endif
  
  return TRUE;
}
//----------------------------------------------------------------------------------------------------
BOOL TreeNode::MoveToChild(USHORT i)
{
  if (Children() == INVALIDINDEX)
    return FALSE;

  USHORT Depth = this->Depth()+1;
  ULONGLONG Offset = Children()+i;

  m_pPartition0 = &m_pTree->GetNodeStream(Depth, 0).Get<PARTITION0>(Offset);
  m_pPartition1 = &m_pTree->GetNodeStream(Depth, 1).Get<PARTITION1>(Offset);
  m_pPartition2 = &m_pTree->GetNodeStream(Depth, 2).Get<PARTITION2>(Offset);
  m_pPartition3 = &m_pTree->GetNodeStream(Depth, 3).Get<PARTITION3>(Offset);
  m_pPartition4 = &m_pTree->GetNodeStream(Depth, 4).Get<PARTITION4>(Offset);
  m_pPartition5 = &m_pTree->GetNodeStream(Depth, 5).Get<PARTITION5>(Offset);
  #if defined(CONFIG_STOREDNEIGHBOURS)
  m_pPartition6 = &m_pTree->GetNodeStream(Depth, 6).Get<PARTITION6>(Offset);
  #endif
  
  return TRUE;
}
//----------------------------------------------------------------------------------------------------
TreeNode::TreeNode(Tree* pTree, USHORT Depth, ULONGLONG Offset)
{
  Assert(pTree != 0);
  Assert(Depth < pTree->m_State.TreeHeight);

  m_pTree = pTree;

  m_pPartition0 = &m_pTree->GetNodeStream(Depth, 0).Get<PARTITION0>(Offset);
  m_pPartition1 = &m_pTree->GetNodeStream(Depth, 1).Get<PARTITION1>(Offset);
  m_pPartition2 = &m_pTree->GetNodeStream(Depth, 2).Get<PARTITION2>(Offset);
  m_pPartition3 = &m_pTree->GetNodeStream(Depth, 3).Get<PARTITION3>(Offset);
  m_pPartition4 = &m_pTree->GetNodeStream(Depth, 4).Get<PARTITION4>(Offset);
  m_pPartition5 = &m_pTree->GetNodeStream(Depth, 5).Get<PARTITION5>(Offset);
  #if defined(CONFIG_STOREDNEIGHBOURS)
  m_pPartition6 = &m_pTree->GetNodeStream(Depth, 6).Get<PARTITION6>(Offset);
  #endif

}
//----------------------------------------------------------------------------------------------------
ULONGLONG TreeNode::StreamOffset() CONST
{
  DataStream& Stream0 = m_pTree->GetNodeStream(Depth(), 0);
  return m_pPartition0 - Stream0.GetBasePointer<PARTITION0>();
}
//----------------------------------------------------------------------------------------------------
VOID TreeNode::MoveToNext()
{
  m_pPartition0++;
  m_pPartition1++;
  m_pPartition2++;
  m_pPartition3++;
  m_pPartition4++;
  m_pPartition5++;
  #if defined(CONFIG_STOREDNEIGHBOURS)
  m_pPartition6++;
  #endif
}
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
SPATIALINDEX* Tree::GetNodeIndex(USHORT Depth, UINT Index, UINT Region)
{
  return m_StreamManager.GetNodeIndex(Depth, Index, Region);
}
//----------------------------------------------------------------------------------------------------
SPATIALINDEX* Tree::GetPointIndex(USHORT Depth, UINT Index, UINT Region)
{
  return m_StreamManager.GetPointIndex(Depth, Index, Region);
}
//----------------------------------------------------------------------------------------------------
ULONGLONG Tree::GetMaximumNodeSliceCount(USHORT Depth)
{
  using Bolitho::Max;

  ULONGLONG Maximum = 0;
  for (UINT i=0; i<(UINT)(1<<Depth); i++)
    Maximum = Max(Maximum, GetNodeSliceCount(Depth, i));

  return Maximum;
}
//----------------------------------------------------------------------------------------------------
ULONGLONG Tree::GetNodeSliceCount(USHORT Depth, UINT Index)
{
  ULONGLONG Length = 0;
  for (UINT i=0; i<CONFIG_REGIONCOUNT; i++)
    Length += GetNodeIndex(Depth, Index, i)->Length;

  return Length;
}

//----------------------------------------------------------------------------------------------------
ULONGLONG Tree::GetMaximumNodeSliceCount()
{
  using Bolitho::Max;

  ULONGLONG Maximum = 0;
  for (USHORT i=0; i<m_State.TreeHeight; i++)
    Maximum = Max(Maximum, GetMaximumNodeSliceCount(i));

  return Maximum;
}
//----------------------------------------------------------------------------------------------------
USHORT Tree::CoarsestDepthChanged(UINT SliceIndex)
{
  USHORT d = m_State.TreeHeight;
  UINT i0 = SliceIndex-1;
  UINT i1 = SliceIndex;

  for (INT i = 0; i<m_State.TreeHeight; i++)
  {
    if(i0 == i1)
      break;
    d--;
    i0 >>= 1;
    i1 >>= 1;
  }
  return d;
}
//----------------------------------------------------------------------------------------------------
ULONGLONG Tree::GetNodeCount(USHORT Depth)
{
  ULONGLONG N = 0;
  for (UINT i=0; i < (1u << Depth); i+=2)
    N += GetNodeSliceCount(Depth, i);

  return N;
}
//----------------------------------------------------------------------------------------------------
ULONGLONG Tree::GetAverageNodeSliceCount(USHORT Depth)
{
  ULONGLONG N = GetNodeCount(Depth);
  return N / (ULONGLONG)(1u << Depth);
}
//----------------------------------------------------------------------------------------------------
ULONGLONG Tree::GetPointSliceCount(USHORT Depth, UINT SliceIndex)
{
  ULONGLONG Length = 0;
  for (UINT i=0; i<CONFIG_REGIONCOUNT; i++)
    Length += GetPointIndex(Depth, SliceIndex, i)->Length;
  return Length;
}
//----------------------------------------------------------------------------------------------------
ULONGLONG Tree::GetMaximumPointSliceCount(USHORT Depth)
{
  using Bolitho::Max;

  ULONGLONG MaxPoints = 0;
  for (UINT i=0; i < (1u << Depth); i++)
    MaxPoints = Max(MaxPoints, GetPointSliceCount(Depth, i));

  return MaxPoints;
}
//----------------------------------------------------------------------------------------------------
ULONGLONG Tree::GetAveragePointSliceCount(USHORT Depth)
{
  ULONGLONG N = 0;
  UINT IndexBase = (1u << Depth) - 1;
  for (UINT i=0; i < (1u << Depth); i++)
    N += GetPointSliceCount(Depth, i);

  return N / (ULONGLONG)(1u << Depth);
}
//----------------------------------------------------------------------------------------------------
VOID Tree::AdvancePoints(UINT AdvanceAmount)
{
  using Bolitho::Min;
  using Bolitho::Max;

  m_CurrentPointIndex += AdvanceAmount;

  if (m_CurrentPointIndex >= Resolution())
    return;

  USHORT Depth = MaximumDepth();
  UINT Index = m_CurrentPointIndex >> (m_State.TreeHeight-Depth-1);

  UINT MaxIndex = (UINT)Min<INT>(Index + m_pNeighbourhoods[Depth].Second, (1 << Depth) - 1);
  UINT MinIndex = (UINT)Max<INT>(Index - m_pNeighbourhoods[Depth].First, 0);

  ULONGLONG Tail = GetPointIndex(Depth, MinIndex)->Offset;
  ULONGLONG Head = GetPointIndex(Depth, MaxIndex, CONFIG_REGIONCOUNT - 1)->Offset + GetPointIndex(Depth, MaxIndex, CONFIG_REGIONCOUNT - 1)->Length;

  m_StreamManager.SetPointStreamWindow(Tail, Head);

}
//----------------------------------------------------------------------------------------------------
VOID Tree::AdvanceNodes(USHORT Depth, UINT AdvanceAmount)
{
  using Bolitho::Min;
  using Bolitho::Max;

  m_pCurrentNodeIndex[Depth] += AdvanceAmount;
  if (m_pCurrentNodeIndex[Depth] >= (UINT)(1<<Depth))
  {
    m_pCurrentNodeIndex[Depth] = (UINT)(1<<Depth);
    return;
  }

  UINT MaxIndex = (UINT)Min<INT>(m_pCurrentNodeIndex[Depth] + m_pNeighbourhoods[Depth].Second, (1<<Depth)-1);
  UINT MinIndex = (UINT)Max<INT>(m_pCurrentNodeIndex[Depth] - m_pNeighbourhoods[Depth].First, 0);

  m_StreamManager.SetNodeStreamWindow(Depth, GetNodeIndex(Depth, MinIndex)->Offset, GetNodeIndex(Depth, MinIndex)->Offset + GetNodeIndex(Depth, MinIndex)->Length);
}
//----------------------------------------------------------------------------------------------------
TreeNode Tree::Root()
{
  return TreeNode(this, 0, 0);
}
//----------------------------------------------------------------------------------------------------
TreeNode Tree::GetNode(USHORT Depth, ULONGLONG Offset)
{
  return TreeNode(this, Depth, Offset);
}
//----------------------------------------------------------------------------------------------------
VOID Tree::Initialise(INT Neighbourhood, DWORD TopologyFlags, DWORD DataFlags, DWORD PointFlags)
{
  m_pNeighbourhoods = new Pair<INT,INT>[m_State.TreeHeight];
  for (INT i=0; i<m_State.TreeHeight; i++)
    m_pNeighbourhoods[i] = Pair<INT,INT>(Neighbourhood,Neighbourhood);
  Initialise(TopologyFlags, DataFlags, PointFlags);
}
//----------------------------------------------------------------------------------------------------
VOID Tree::Initialise(INT BackNeighbourhood, INT ForwardNeighbourhood, DWORD TopologyFlags, DWORD DataFlags, DWORD PointFlags)
{
  m_pNeighbourhoods = new Pair<INT,INT>[m_State.TreeHeight];
  for (INT i=0; i<m_State.TreeHeight; i++)
    m_pNeighbourhoods[i] = Pair<INT,INT>(BackNeighbourhood,ForwardNeighbourhood);
  Initialise(TopologyFlags, DataFlags, PointFlags);
}
//----------------------------------------------------------------------------------------------------
VOID Tree::Initialise(Pair<INT,INT>* pNeighbourhoods, DWORD TopologyFlags, DWORD DataFlags, DWORD PointFlags)
{
  m_pNeighbourhoods = new Pair<INT,INT>[m_State.TreeHeight];
  CopyMemory(m_pNeighbourhoods, pNeighbourhoods, sizeof(Pair<INT,INT>)*m_State.TreeHeight);
  Initialise(TopologyFlags, DataFlags, PointFlags);
}
//----------------------------------------------------------------------------------------------------
VOID Tree::Initialise(DWORD TopologyFlags, DWORD DataFlags, DWORD PointFlags)
{
  m_CurrentPointIndex = 0;
  m_pCurrentNodeIndex = new UINT[m_State.TreeHeight];
  
  for(INT i=0; i<m_State.TreeHeight; i++)
    m_pCurrentNodeIndex[i] = 0;

  m_StreamManager.InitializePointStream(PointFlags);

  for (USHORT Depth=0; Depth<m_State.TreeHeight; Depth++)
  {
    m_StreamManager.InitializeNodeStream(Depth, DataFlags);

    STREAMOFFSET Tail = 0;
    STREAMOFFSET Head = 0;

    UINT MaxIndex = (1<<Depth)-1;

    if (1<<Depth <= m_pNeighbourhoods[Depth].Second)
      Head = GetNodeIndex(Depth, MaxIndex)->Offset + GetNodeIndex(Depth, MaxIndex)->Length;
    else
      Head = GetNodeIndex(Depth, m_pNeighbourhoods[Depth].Second)->Offset + GetNodeIndex(Depth, m_pNeighbourhoods[Depth].Second)->Length;

    m_StreamManager.SetNodeStreamWindow(Depth, Tail, Head);

    if (Depth != MaximumDepth())
      continue;

    if (1<<Depth <= m_pNeighbourhoods[Depth].Second)
      Head = GetPointIndex(Depth, MaxIndex)->Offset + GetPointIndex(Depth, MaxIndex)->Length;
    else
      Head = GetPointIndex(Depth, m_pNeighbourhoods[Depth].Second, CONFIG_REGIONCOUNT - 1)->Offset + GetPointIndex(Depth, m_pNeighbourhoods[Depth].Second, CONFIG_REGIONCOUNT - 1)->Length;

    m_StreamManager.SetPointStreamWindow(Tail, Head);
  }
}
//----------------------------------------------------------------------------------------------------
VOID Tree::Finalise()
{
  delete[] m_pNeighbourhoods;
  m_pNeighbourhoods = 0;

  m_StreamManager.FinalizePointStream();

  for (USHORT Depth=0; Depth<m_State.TreeHeight; Depth++)
    m_StreamManager.FinalizeNodeStream(Depth);
}
//----------------------------------------------------------------------------------------------------
STREAMOFFSET Tree::InsertBrood(USHORT Depth, CONST PARTITION0 Partition0[8], CONST PARTITION1 Partition1[8], CONST PARTITION2 Partition2[8], CONST PARTITION3 Partition3[8], CONST PARTITION4 Partition4[8], CONST PARTITION5 Partition5[8])
{
  STREAMOFFSET Position = m_StreamManager.AdvanceNodeStreamWindow(Depth, 8);
  
  CopyMemory(&GetNodeStream(Depth, 0).Get<PARTITION0>(Position), &Partition0[0], sizeof(PARTITION0)*8);  
  CopyMemory(&GetNodeStream(Depth, 1).Get<PARTITION1>(Position), &Partition1[0], sizeof(PARTITION1)*8);  
  CopyMemory(&GetNodeStream(Depth, 2).Get<PARTITION2>(Position), &Partition2[0], sizeof(PARTITION2)*8);  
  CopyMemory(&GetNodeStream(Depth, 3).Get<PARTITION3>(Position), &Partition3[0], sizeof(PARTITION3)*8);  
  CopyMemory(&GetNodeStream(Depth, 4).Get<PARTITION4>(Position), &Partition4[0], sizeof(PARTITION4)*8);  
  CopyMemory(&GetNodeStream(Depth, 5).Get<PARTITION5>(Position), &Partition5[0], sizeof(PARTITION5)*8);  

  UINT Region = 0;
  if (Depth > CONFIG_REGIONDEPTH)
  {
    UINT d = Depth - CONFIG_REGIONDEPTH;
    UINT x = (Partition0[0].Offset[0]+1) & (~(1 << Depth));
    UINT y = (Partition0[0].Offset[1]+1) & (~(1 << Depth)); 
    UINT z = (Partition0[0].Offset[2]+1) & (~(1 << Depth));

    x >>= d;
    y >>= d;
    z >>= d;

    Region = (UINT)Interleave(y,z);
  }

  SPATIALINDEX* pIndex = GetNodeIndex(Depth, m_pCurrentNodeIndex[Depth], Region);
  pIndex->Length += 8;

  return Position;
}
//----------------------------------------------------------------------------------------------------
VOID Tree::FixupNodeIndexOffsets(USHORT Depth)
{
  ULONGLONG Offset = 0;
  for (UINT Slice=0; Slice<(1u<<Depth); Slice+=2)
  {
    for (UINT Region=0; Region<CONFIG_REGIONCOUNT; Region++)
    {
      SPATIALINDEX* pIndex = GetNodeIndex(Depth, Slice, Region);
      pIndex->Offset = Offset;
      Offset += pIndex->Length;
    }
  }
}
//----------------------------------------------------------------------------------------------------
VOID Tree::InsertRoot(CONST PARTITION0& Partition0, CONST PARTITION1& Partition1, CONST PARTITION2& Partition2, CONST PARTITION3& Partition3, CONST PARTITION4& Partition4, CONST PARTITION5& Partition5)
{
  CopyMemory(&GetNodeStream(0, 0).Get<PARTITION0>(0), &Partition0, sizeof(PARTITION0));  
  CopyMemory(&GetNodeStream(0, 1).Get<PARTITION1>(0), &Partition1, sizeof(PARTITION1));  
  CopyMemory(&GetNodeStream(0, 2).Get<PARTITION2>(0), &Partition2, sizeof(PARTITION2));  
  CopyMemory(&GetNodeStream(0, 3).Get<PARTITION3>(0), &Partition3, sizeof(PARTITION3));  
  CopyMemory(&GetNodeStream(0, 4).Get<PARTITION4>(0), &Partition4, sizeof(PARTITION4));  
  CopyMemory(&GetNodeStream(0, 5).Get<PARTITION5>(0), &Partition5, sizeof(PARTITION5));  
}
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
TreeNode::Neighbors::Neighbors()
{
  Clear();
}
//----------------------------------------------------------------------------------------------------
VOID TreeNode::Neighbors::Clear()
{
  for(INT i=0;i<3;i++)
    for(INT j=0;j<3;j++)
      for(INT k=0;k<3;k++)
        m_pNeighbors[i][j][k] = TreeNode::INVALIDINDEX;
}
//----------------------------------------------------------------------------------------------------
TreeNode::NeighborCache::NeighborCache()
{
  m_pNeighbors = 0;
}
//----------------------------------------------------------------------------------------------------
TreeNode::NeighborCache::~NeighborCache()
{
  if(m_pNeighbors)
    delete[] m_pNeighbors;
  m_pNeighbors = 0;
}
//----------------------------------------------------------------------------------------------------
VOID TreeNode::NeighborCache::Initialize(Ptr<Tree> pTree)
{
  USHORT d = pTree->MaximumDepth();

  if(m_pNeighbors)
    delete[] m_pNeighbors;

  m_pNeighbors = NULL;

  m_Depth = d;
  m_pNeighbors = new Neighbors[d+1];
  m_pTree = pTree;
  m_pNeighbors[0].m_pNeighbors[1][1][1] = 0;
}
//----------------------------------------------------------------------------------------------------
TreeNode::Neighbors& TreeNode::NeighborCache::ComputeNeighbors(TreeNode& node)
{
  #if defined(CONFIG_PROFILE) && defined(CONFIG_PROFILE_NEIGHBORS)
  TimeAllFunctionCalls();
  #endif

  INT d=node.Depth();
  if(node.StreamOffset() != m_pNeighbors[d].m_pNeighbors[1][1][1])
    for(INT i=d;i<=m_Depth;i++)
      m_pNeighbors[i].Clear();
  else
    return m_pNeighbors[d];

  INT startDepth;
  INT depth;
  INT offset[3];
  INT ancDepth;
  INT ancOffset[3];

  node.GetDepthAndOffset(depth,offset);

  // Find the first depth at which an ancestor exists
  for(startDepth=d-1;startDepth>=0;startDepth--)
  {
    if(m_pNeighbors[startDepth].m_pNeighbors[1][1][1] == TreeNode::INVALIDINDEX)
      continue;

    // Get the potential ancestor
    TreeNode ancestor = m_pTree->GetNode(startDepth,m_pNeighbors[startDepth].m_pNeighbors[1][1][1]);
    ancestor.GetDepthAndOffset(ancDepth,ancOffset);

    // Test if it is an ancestor
    if(	ancOffset[0]==(offset[0]>>(depth-ancDepth)) &&
      ancOffset[1]==(offset[1]>>(depth-ancDepth)) &&
      ancOffset[2]==(offset[2]>>(depth-ancDepth)))
      break;
  }
  for(INT i=startDepth+1;i<depth;i++)
  {
    // Get the valid parent node
    TreeNode Parent = m_pTree->GetNode(i-1,m_pNeighbors[i-1].m_pNeighbors[1][1][1]);

    // Get the child node containing the node of interest
    Parent.GetDepthAndOffset(ancDepth,ancOffset);

    // Get the relative index
    INT x,y,z;
    x=(offset[0]>>(depth-i))-(ancOffset[0]<<1);
    y=(offset[1]>>(depth-i))-(ancOffset[1]<<1);
    z=(offset[2]>>(depth-i))-(ancOffset[2]<<1);

    TreeNode Child = m_pTree->GetNode(i,Parent.Children()+Cube::CornerIndex(x,y,z));
    InternalComputeNeighbors(Child);
  }

  TreeNode::Neighbors& N = InternalComputeNeighbors(node);
  return N;
}
//----------------------------------------------------------------------------------------------------
TreeNode::Neighbors& TreeNode::NeighborCache::InternalComputeNeighbors(TreeNode& node)
{
  INT d = node.Depth();
  if(node.StreamOffset() != m_pNeighbors[d].m_pNeighbors[1][1][1])
  {
    m_pNeighbors[d].Clear();

    TreeNode P = m_pTree->GetNode(d-1,m_pNeighbors[d-1].m_pNeighbors[1][1][1]);
    if(!d)
    {
      m_pNeighbors[0].m_pNeighbors[1][1][1] = node.StreamOffset();
    }
    else
    {
      // The assumption here is that the parent node has already been set...
      Neighbors& temp=m_pNeighbors[d-1];
      INT i,j,k;
      UINT x1,y1,z1;
      UINT x2,y2,z2;
      INT idx=INT(node.StreamOffset()-P.Children());
      Cube::FactorCornerIndex( idx   ,x1,y1,z1);
      Cube::FactorCornerIndex((~idx)&7,x2,y2,z2);
      for(i=0;i<2;i++)
        for(j=0;j<2;j++)
          for(k=0;k<2;k++)
            m_pNeighbors[d].m_pNeighbors[x2+i][y2+j][z2+k]=P.Children()+Cube::CornerIndex(i,j,k);

      // Set the m_pNeighbors from across the faces
      i=x1<<1;
      if(temp.m_pNeighbors[i][1][1] != TreeNode::INVALIDINDEX)
      {
        P = m_pTree->GetNode(d-1,temp.m_pNeighbors[i][1][1]);
        if(P.Children() != TreeNode::INVALIDINDEX)
          for(j=0;j<2;j++)
            for(k=0;k<2;k++)
              m_pNeighbors[d].m_pNeighbors[i][y2+j][z2+k]=P.Children()+Cube::CornerIndex(x2,j,k);
      }

      j=y1<<1;
      if(temp.m_pNeighbors[1][j][1] != TreeNode::INVALIDINDEX)
      {
        P = m_pTree->GetNode(d-1,temp.m_pNeighbors[1][j][1]);
        if(P.Children() != TreeNode::INVALIDINDEX)
          for(i=0;i<2;i++)
            for(k=0;k<2;k++)
              m_pNeighbors[d].m_pNeighbors[x2+i][j][z2+k]=P.Children()+Cube::CornerIndex(i,y2,k);
      }

      k=z1<<1;
      if(temp.m_pNeighbors[1][1][k] != TreeNode::INVALIDINDEX)
      {
        P = m_pTree->GetNode(d-1,temp.m_pNeighbors[1][1][k]);
        if(P.Children() != TreeNode::INVALIDINDEX)
          for(i=0;i<2;i++)
            for(j=0;j<2;j++)
              m_pNeighbors[d].m_pNeighbors[x2+i][y2+j][k]=P.Children()+Cube::CornerIndex(i,j,z2);
      }

      // Set the m_pNeighbors from across the edges
      i=x1<<1;	j=y1<<1;
      if(temp.m_pNeighbors[i][j][1] != TreeNode::INVALIDINDEX)
      {
        P = m_pTree->GetNode(d-1,temp.m_pNeighbors[i][j][1]);
        if(P.Children() != TreeNode::INVALIDINDEX)
          for(k=0;k<2;k++)
            m_pNeighbors[d].m_pNeighbors[i][j][z2+k]=P.Children()+Cube::CornerIndex(x2,y2,k);
      }

      i=x1<<1;	k=z1<<1;
      if(temp.m_pNeighbors[i][1][k] != TreeNode::INVALIDINDEX)
      {
        P = m_pTree->GetNode(d-1,temp.m_pNeighbors[i][1][k]);
        if(P.Children() != TreeNode::INVALIDINDEX)
          for(j=0;j<2;j++)
            m_pNeighbors[d].m_pNeighbors[i][y2+j][k]=P.Children()+Cube::CornerIndex(x2,j,z2);
      }

      j=y1<<1;	k=z1<<1;
      if(temp.m_pNeighbors[1][j][k] != TreeNode::INVALIDINDEX)
      {
        P = m_pTree->GetNode(d-1,temp.m_pNeighbors[1][j][k]);
        if(P.Children() != TreeNode::INVALIDINDEX)
          for(i=0;i<2;i++)
            m_pNeighbors[d].m_pNeighbors[x2+i][j][k]=P.Children()+Cube::CornerIndex(i,y2,z2);

      }

      // Set the neighbor from across the corner
      i=x1<<1;	j=y1<<1;	k=z1<<1;
      if(temp.m_pNeighbors[i][j][k] != TreeNode::INVALIDINDEX)
      {
        P = m_pTree->GetNode(d-1,temp.m_pNeighbors[i][j][k]);
        if(P.Children() != TreeNode::INVALIDINDEX)
          m_pNeighbors[d].m_pNeighbors[i][j][k]=P.Children()+Cube::CornerIndex(x2,y2,z2);
      }
    }
  }
  return m_pNeighbors[d];
}
//----------------------------------------------------------------------------------------------------
TreeNode::Neighbors2::Neighbors2()
{
  Clear();
}
//----------------------------------------------------------------------------------------------------
VOID TreeNode::Neighbors2::Clear()
{
  for(INT i=0;i<5;i++)
    for(INT j=0;j<5;j++)
      for(INT k=0;k<5;k++)
        m_pNeighbors[i][j][k] = TreeNode::INVALIDINDEX;
}
//----------------------------------------------------------------------------------------------------
TreeNode::NeighborCache2::NeighborCache2()
{
  m_pNeighbors=NULL;
}
//----------------------------------------------------------------------------------------------------
TreeNode::NeighborCache2::~NeighborCache2()
{
  if(m_pNeighbors){delete[] m_pNeighbors;}
  m_pNeighbors=NULL;
}
//----------------------------------------------------------------------------------------------------
VOID TreeNode::NeighborCache2::Initialize(Ptr<Tree> pTree)
{
  INT d=pTree->Height()-1;
  if(m_pNeighbors)
    delete[] m_pNeighbors;
  m_pNeighbors=NULL;
  if(d<0){return;}
  m_Depth=d;
  m_pNeighbors=new Neighbors2[d+1];
  m_pTree=pTree;
  m_pNeighbors[0].m_pNeighbors[2][2][2] = 0;
}
//----------------------------------------------------------------------------------------------------
TreeNode::Neighbors2& TreeNode::NeighborCache2::ComputeNeighbors(TreeNode& node)
{
  #if defined(CONFIG_PROFILE) && defined(CONFIG_PROFILE_NEIGHBORS)
  TimeAllFunctionCalls();
  #endif


  INT d=node.Depth();
  if(node.StreamOffset() != m_pNeighbors[d].m_pNeighbors[2][2][2])
    for(INT i=d;i<=m_Depth; i++)
      m_pNeighbors[i].Clear();
  else
    return m_pNeighbors[d];

  INT startDepth;
  INT depth;
  INT offset[3];
  INT ancDepth;
  INT ancOffset[3];

  node.GetDepthAndOffset(depth,offset);

  // Find the first depth at which an ancestor exists
  for(startDepth=d-1;startDepth>=0;startDepth--)
  {
    if(m_pNeighbors[startDepth].m_pNeighbors[2][2][2] == TreeNode::INVALIDINDEX)
      continue;

    // Get the potential ancestor
    TreeNode ancestor = m_pTree->GetNode(startDepth,m_pNeighbors[startDepth].m_pNeighbors[2][2][2]);
    ancestor.GetDepthAndOffset(ancDepth,ancOffset);

    // Test if it is an ancestor
    if(	ancOffset[0]==(offset[0]>>(depth-ancDepth)) && ancOffset[1]==(offset[1]>>(depth-ancDepth)) && ancOffset[2]==(offset[2]>>(depth-ancDepth)))
      break;
  }

  for(INT i=startDepth+1;i<depth;i++)
  {
    // Get the valid parent node
    TreeNode Parent = m_pTree->GetNode(i-1,m_pNeighbors[i-1].m_pNeighbors[2][2][2]);

    // Get the child node containing the node of interest
    Parent.GetDepthAndOffset(ancDepth,ancOffset);

    // Get the relative index
    INT x,y,z;    x=(offset[0]>>(depth-i))-(ancOffset[0]<<1);
    y=(offset[1]>>(depth-i))-(ancOffset[1]<<1);
    z=(offset[2]>>(depth-i))-(ancOffset[2]<<1);

    TreeNode Child = m_pTree->GetNode(i,Parent.Children()+Cube::CornerIndex(x,y,z));
    InternalComputeNeighbors(Child);
  }
  TreeNode::Neighbors2& N = InternalComputeNeighbors(node);
  return N;
}
//----------------------------------------------------------------------------------------------------
TreeNode::Neighbors2& TreeNode::NeighborCache2::InternalComputeNeighbors(TreeNode& node)
{
  INT d=node.Depth();
  if(node.StreamOffset() != m_pNeighbors[d].m_pNeighbors[2][2][2])
  {
    m_pNeighbors[d].Clear();

    TreeNode P = m_pTree->GetNode(d-1,m_pNeighbors[d-1].m_pNeighbors[2][2][2]);
    if(!d)
      m_pNeighbors[0].m_pNeighbors[2][2][2] = node.StreamOffset();
    else
    {
      // The assumption here is that the parent node has already been set...
      Neighbors2& temp = m_pNeighbors[d-1];
      INT ii,jj,kk;
      UINT x1,y1,z1;
      UINT x2,y2,z2;

      INT idx = INT(node.StreamOffset() - P.Children());
      Cube::FactorCornerIndex( idx   ,x1,y1,z1);

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
            if(temp.m_pNeighbors[x2][y2][z2] != TreeNode::INVALIDINDEX)
            {
              P = m_pTree->GetNode(d-1,temp.m_pNeighbors[x2][y2][z2]);
              if(P.Children() != TreeNode::INVALIDINDEX)
                m_pNeighbors[d].m_pNeighbors[i][j][k]=P.Children()+Cube::CornerIndex(ii,jj,kk);
            }
          }
        }
      }
    }
  }
  return m_pNeighbors[d];
}
//----------------------------------------------------------------------------------------------------

