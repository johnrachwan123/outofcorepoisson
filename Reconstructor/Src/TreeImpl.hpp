/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/TreeImpl.hpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 908 $
 * Last Updated: $Date: 2008-12-29 12:19:41 -0800 (Mon, 29 Dec 2008) $
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

//----------------------------------------------------------------------------------------------------
#if defined(CONFIG_MULTITHREADED)
template<class NODEFUNCTION>
DWORD Tree::IterateNodesProc(IterateNodesInfo<NODEFUNCTION>* pInfo)
{
  typename NODEFUNCTION::THREADLOCAL L;
  pInfo->F.InitThreadLocal(pInfo->pTree, L);
  #if defined(CONFIG_MULTITHREADED)
  L.ThreadId = ThreadPool::GetThreadId();
  #else
  L.ThreadId = 0;
  #endif
  ThreadProfileFunction("%u", L.ThreadId); 

	SPATIALINDEX* pIndex = pInfo->pIndex;
	L.PartitionId = pInfo->PartitionId;
  for (UINT i=pInfo->StartRegion; i<pInfo->EndRegion; i++)
  {
    L.RegionId = i;
    for (ULONGLONG j=0; j<pIndex[i].Length; j++)
    {
      ULONGLONG Offset = pIndex[i].Offset+j;
      TreeNode N(pInfo->pTree, pInfo->Depth, Offset);
      if (((pInfo->Slice % 2) == 0) && !((Offset % 8) & 1))
        pInfo->F.ProcessNode(pInfo->pTree, N, L);
      if (((pInfo->Slice % 2) == 1) &&  ((Offset % 8) & 1))
        pInfo->F.ProcessNode(pInfo->pTree, N, L);
    }
  }

  delete pInfo;
  return 0;
}
//----------------------------------------------------------------------------------------------------
template<class NODEFUNCTION>
VOID Tree::IterateNodesMT(USHORT Depth, UINT Slice, NODEFUNCTION& F)
{
  ULONGLONG TotalLength = GetNodeSliceCount(Depth, Slice);
  ULONGLONG TargetLength = TotalLength / m_ThreadCount;

  ThreadProfileFunction("%u %llu", Slice, TotalLength); 

  ULONGLONG ThreadWork[CONFIG_MAXTHREADS];
  UINT ThreadBeginRegion[CONFIG_MAXTHREADS];
  UINT ThreadEndRegion[CONFIG_MAXTHREADS];
  UINT CurrentThread = 0;
  
  ZeroMemory(ThreadWork, sizeof(ThreadWork));
  ZeroMemory(ThreadBeginRegion, sizeof(ThreadBeginRegion));
  ZeroMemory(ThreadEndRegion, sizeof(ThreadEndRegion));
  for (UINT i=0; i<CONFIG_REGIONCOUNT; i++)
  {
    SPATIALINDEX* pIndex = GetNodeIndex(Depth, Slice, i);
    
    ThreadWork[CurrentThread] += pIndex->Length;
    
    if (ThreadWork[CurrentThread] > TargetLength)
    {
      ThreadEndRegion[CurrentThread] = i;
      CurrentThread++;
      ThreadBeginRegion[CurrentThread] = i;
    }
  }
  ThreadEndRegion[CurrentThread] = CONFIG_REGIONCOUNT;

  ULONGLONG Offset = 0;
  for (UINT i=0; i<m_ThreadCount; i++)
  {
    IterateNodesInfo<NODEFUNCTION>* pInfo = new IterateNodesInfo<NODEFUNCTION>(this, F);
    pInfo->pIndex = GetNodeIndex(Depth, Slice, 0);
    pInfo->StartRegion = ThreadBeginRegion[i];
    pInfo->EndRegion = ThreadEndRegion[i];
    pInfo->Depth = Depth;
    pInfo->Slice = Slice;
		pInfo->PartitionId = i;

    DWORD (WINAPI *pFunc)(IterateNodesInfo<NODEFUNCTION>*) = &Tree::IterateNodesProc<NODEFUNCTION>;
    m_IteratePool.Enqueue((LPTHREAD_START_ROUTINE)pFunc, pInfo);
  }
  m_IteratePool.Wait();
 }
//----------------------------------------------------------------------------------------------------
#endif

//----------------------------------------------------------------------------------------------------
template<class NODEFUNCTION>
VOID Tree::IterateNodes(INT RelativeIndex, USHORT Depth, NODEFUNCTION& F)
{
  #if defined(CONFIG_PROFILE)
  TimeAllFunctionCalls();
  #endif

  using Bolitho::Min;

  AssertBounds(RelativeIndex, -m_pNeighbourhoods[Depth].First, m_pNeighbourhoods[Depth].Second+1);
  
  UINT Slice = m_pCurrentNodeIndex[Depth] + RelativeIndex;
  ThreadProfileFunction("%u", Slice); 

  if (Slice >= (UINT)(1 << Depth))
    return;
  
  if (F.ThreadSafe)
    IterateNodesMT(Depth, Slice, F);
  else
    IterateNodesST(Depth, Slice, F);

}
//----------------------------------------------------------------------------------------------------
template<class NODEFUNCTION>
VOID Tree::IterateNodesST(USHORT Depth, UINT Slice, NODEFUNCTION& F)
{
  typename NODEFUNCTION::THREADLOCAL L;
  F.InitThreadLocal(this, L);
  L.ThreadId = 0;
  SPATIALINDEX* pIndex = GetNodeIndex(Depth, Slice, 0);
  for (UINT Region=0; Region<CONFIG_REGIONCOUNT; Region++)
  {
    L.RegionId = Region;
    for (ULONGLONG i=0; i<pIndex[Region].Length; i++)
    {
      TreeNode N(this, Depth, pIndex[Region].Offset + i);
      if (((Slice % 2) == 0) && !((i % 8) & 1))
        F.ProcessNode(this, N, L);
      if (((Slice % 2) == 1) &&  ((i % 8) & 1))
        F.ProcessNode(this, N, L);
    }
  }
}
//----------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------
#if defined(CONFIG_MULTITHREADED)
template<class NODEFUNCTION>
DWORD Tree::IterateNodesBroodProc(IterateNodesInfo<NODEFUNCTION>* pInfo)
{
  typename NODEFUNCTION::THREADLOCAL L;
  pInfo->F.InitThreadLocal(pInfo->pTree, L);
  #if defined(CONFIG_MULTITHREADED)
  L.ThreadId = ThreadPool::GetThreadId();
  #else
  L.ThreadId = 0;
  #endif

  ThreadProfileFunction("%u", L.ThreadId); 
  SPATIALINDEX* pIndex = pInfo->pIndex;
  L.PartitionId = pInfo->PartitionId;
  for (UINT i=pInfo->StartRegion; i<pInfo->EndRegion; i++)
  {
    TreeNode N(pInfo->pTree, pInfo->Depth, pIndex[i].Offset);
    L.RegionId = i;
    for (ULONGLONG j=0; j<pIndex[i].Length; j++, N.MoveToNext())
    {
      pInfo->F.ProcessNode(pInfo->pTree, N, L);
    }
  }

  delete pInfo;
  return 0;
}
//----------------------------------------------------------------------------------------------------
template<class NODEFUNCTION>
VOID Tree::IterateNodesBroodMT(USHORT Depth, UINT Slice, NODEFUNCTION& F)
{
  ULONGLONG TotalLength = GetNodeSliceCount(Depth, Slice);
  ULONGLONG TargetLength = TotalLength / m_ThreadCount;

  ThreadProfileFunction("%u %llu", Slice, TotalLength); 

  ULONGLONG ThreadWork[CONFIG_MAXTHREADS];
  UINT ThreadBeginRegion[CONFIG_MAXTHREADS];
  UINT ThreadEndRegion[CONFIG_MAXTHREADS];
  UINT CurrentThread = 0;
  
  ZeroMemory(ThreadWork, sizeof(ThreadWork));
  ZeroMemory(ThreadBeginRegion, sizeof(ThreadBeginRegion));
  ZeroMemory(ThreadEndRegion, sizeof(ThreadEndRegion));
  for (UINT i=0; i<CONFIG_REGIONCOUNT; i++)
  {
    SPATIALINDEX* pIndex = GetNodeIndex(Depth, Slice, i);
    
    ThreadWork[CurrentThread] += pIndex->Length;
    
    if (ThreadWork[CurrentThread] > TargetLength)
    {
      ThreadEndRegion[CurrentThread] = i;
      CurrentThread++;
      ThreadBeginRegion[CurrentThread] = i;
    }
  }
  ThreadEndRegion[CurrentThread] = CONFIG_REGIONCOUNT;

  ULONGLONG Offset = 0;
  for (UINT i=0; i<m_ThreadCount; i++)
  {
    IterateNodesInfo<NODEFUNCTION>* pInfo = new IterateNodesInfo<NODEFUNCTION>(this, F);
    pInfo->pIndex = GetNodeIndex(Depth, Slice, 0);
    pInfo->StartRegion = ThreadBeginRegion[i];
    pInfo->EndRegion = ThreadEndRegion[i];
    pInfo->Depth = Depth;
    pInfo->Slice = Slice;
    pInfo->PartitionId = i;

    DWORD (WINAPI *pFunc)(IterateNodesInfo<NODEFUNCTION>*) = &Tree::IterateNodesBroodProc<NODEFUNCTION>;
    m_IteratePool.Enqueue((LPTHREAD_START_ROUTINE)pFunc, pInfo);
  }

  m_IteratePool.Wait();
 }
//----------------------------------------------------------------------------------------------------
#endif

//----------------------------------------------------------------------------------------------------
template<class NODEFUNCTION>
VOID Tree::IterateNodesBrood(INT RelativeIndex, USHORT Depth, NODEFUNCTION& F)
{
  #if defined(CONFIG_PROFILE)
  TimeAllFunctionCalls();
  #endif

  using Bolitho::Min;

  AssertBounds(RelativeIndex, -m_pNeighbourhoods[Depth].First, m_pNeighbourhoods[Depth].Second+1);
  
  UINT Slice = m_pCurrentNodeIndex[Depth] + RelativeIndex;
  ThreadProfileFunction("%u", Slice); 

  if (Slice >= (UINT)(1 << Depth))
    return;
  
  if (F.ThreadSafe)
    IterateNodesBroodMT(Depth, Slice, F);
  else
    IterateNodesBroodST(Depth, Slice, F);

}
//----------------------------------------------------------------------------------------------------
template<class NODEFUNCTION>
VOID Tree::IterateNodesBroodST(USHORT Depth, UINT Slice, NODEFUNCTION& F)
{
  typename NODEFUNCTION::THREADLOCAL L;
  F.InitThreadLocal(this, L);
  L.ThreadId = 0;
  L.PartitionId = 0;
  SPATIALINDEX* pIndex = GetNodeIndex(Depth, Slice, 0);
  for (UINT Region=0; Region<CONFIG_REGIONCOUNT; Region++)
  {
    L.RegionId = Region;
    for (ULONGLONG i=0; i<pIndex[Region].Length; i++)
    {
      TreeNode N(this, Depth, pIndex[Region].Offset + i);
      if (((Slice % 2) == 0) && !((i % 8) & 1))
        F.ProcessNode(this, N, L);
      if (((Slice % 2) == 1) &&  ((i % 8) & 1))
        F.ProcessNode(this, N, L);
    }
  }
}
//----------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------
template<class POINTFUNCTION>
VOID Tree::IteratePoints(INT RelativeIndex, POINTFUNCTION& F)
{
  #if defined(CONFIG_PROFILE)
  TimeAllFunctionCalls();
  #endif

  using Bolitho::Min;

  USHORT Depth = MaximumDepth();
  VerifyBounds(RelativeIndex, -m_pNeighbourhoods[Depth].First, m_pNeighbourhoods[Depth].Second+1);

  UINT Slice = m_CurrentPointIndex + RelativeIndex;

  if (Slice >= (1u<<Depth))
    return;

  ThreadProfileFunction("%u, %u", Depth, Slice); 

  Depth--;
  Slice >>= 1;

  if (F.ThreadSafe && GetPointSliceCount(Depth, Slice) > 100)
    IteratePointsMT(Depth, Slice, F);
  else
    IteratePointsST(Depth, Slice, F);

}
//----------------------------------------------------------------------------------------------------
template<class POINTFUNCTION>
VOID Tree::IteratePointsST(USHORT Depth, UINT Slice, POINTFUNCTION& F)
{
  typename POINTFUNCTION::THREADLOCAL L;
  F.InitThreadLocal(this, L);
  L.ThreadId = 0;

  SPATIALINDEX* pIndex = GetPointIndex(Depth, Slice);
  for (UINT Region=0; Region<CONFIG_REGIONCOUNT; Region++)
  {
    L.RegionId = Region;
    for (ULONGLONG i=0; i<pIndex[Region].Length; i++)
    {
      ORIENTEDPOINT& P = GetPointStream().Get<ORIENTEDPOINT>(pIndex[Region].Offset+i);
      F.ProcessPoint(this, P.Position, P.Normal, L);
    }
  }
}
//----------------------------------------------------------------------------------------------------
#if defined(CONFIG_MULTITHREADED)
template<class POINTFUNCTION>
DWORD Tree::IteratePointsProc(IteratePointsInfo<POINTFUNCTION>* pInfo)
{
  typename POINTFUNCTION::THREADLOCAL L;
  pInfo->F.InitThreadLocal(pInfo->pTree, L);
  #if defined(CONFIG_MULTITHREADED)
  L.ThreadId = ThreadPool::GetThreadId();
  #else
  L.ThreadId = 0;
  #endif

  ThreadProfileFunction("%u", L.ThreadId); 
  SPATIALINDEX* pIndex = pInfo->pIndex;

  for (UINT i=pInfo->StartRegion; i<pInfo->EndRegion; i++)
  {
    L.RegionId = i;
    for (ULONGLONG j=0; j<pIndex[i].Length; j++)
      pInfo->F.ProcessPoint(pInfo->pTree, pInfo->pPointData[pIndex[i].Offset+j].Position, pInfo->pPointData[pIndex[i].Offset+j].Normal, L);
  }

  delete pInfo;
  return 0;
}
//----------------------------------------------------------------------------------------------------
template<class POINTFUNCTION>
VOID Tree::IteratePointsMT(USHORT Depth, UINT Slice, POINTFUNCTION& F)
{
  ULONGLONG TotalLength = GetPointSliceCount(Depth, Slice);
  ULONGLONG TargetLength = TotalLength / m_ThreadCount;

  ThreadProfileFunction("%u %llu", Slice, TotalLength); 

  ULONGLONG ThreadWork[CONFIG_MAXTHREADS];
  UINT ThreadBeginRegion[CONFIG_MAXTHREADS];
  UINT ThreadEndRegion[CONFIG_MAXTHREADS];
  UINT CurrentThread = 0;
  
  ZeroMemory(ThreadWork, sizeof(ThreadWork));
  ZeroMemory(ThreadBeginRegion, sizeof(ThreadBeginRegion));
  ZeroMemory(ThreadEndRegion, sizeof(ThreadEndRegion));
  for (UINT i=0; i<CONFIG_REGIONCOUNT; i++)
  {
    SPATIALINDEX* pIndex = GetPointIndex(Depth, Slice, i);
    
    ThreadWork[CurrentThread] += pIndex->Length;
    
    if (ThreadWork[CurrentThread] > TargetLength)
    {
      ThreadEndRegion[CurrentThread] = i;
      CurrentThread++;
      ThreadBeginRegion[CurrentThread] = i;
    }
  }
  ThreadEndRegion[CurrentThread] = CONFIG_REGIONCOUNT;

  ULONGLONG Offset = 0;
  for (UINT i=0; i<m_ThreadCount; i++)
  {
    IteratePointsInfo<POINTFUNCTION>* pInfo = new IteratePointsInfo<POINTFUNCTION>(this, F);
    pInfo->pIndex = GetPointIndex(Depth, Slice, 0);
    pInfo->StartRegion = ThreadBeginRegion[i];
    pInfo->EndRegion = ThreadEndRegion[i];
    pInfo->pPointData = &GetPointStream().Get<ORIENTEDPOINT>(0);

    DWORD (WINAPI *pFunc)(IteratePointsInfo<POINTFUNCTION>*) = &Tree::IteratePointsProc<POINTFUNCTION>;
    m_IteratePool.Enqueue((LPTHREAD_START_ROUTINE)pFunc, pInfo);
  }
  m_IteratePool.Wait();
}
//----------------------------------------------------------------------------------------------------
#endif
