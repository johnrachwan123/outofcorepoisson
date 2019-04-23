/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/StreamingOctreeConstructorImpl.hpp $
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

//----------------------------------------------------------------------------------------------------
#if defined(CONFIG_MULTITHREADED)
template<class NODEFUNCTION>
DWORD StreamingOctreeConstructor::IterateNodesProc(IterateNodesInfo<NODEFUNCTION>* pInfo)
{
  typename NODEFUNCTION::THREADLOCAL L;
  pInfo->Function.InitThreadLocal(pInfo->pTree, pInfo->pRoot, L);

  for (UINT i=0; i<pInfo->Count; i++)
    pInfo->Function.ProcessNode(pInfo->Slice, pInfo->Depth, pInfo->ppNodes[i], L);

  /* Memory was allocated in IterateNodes */
  delete pInfo;
  return 0;
}
#endif
//----------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
template<class NODEFUNCTION>
VOID StreamingOctreeConstructor::IterateNodes(UINT Slice, USHORT Depth, BOOL IterateBroods, NODEFUNCTION& F)
{
  #if defined(CONFIG_PROFILE)
  TimeAllFunctionCalls();
  #endif

  using Bolitho::Min;

  UINT Resolution = 1u << Depth;
  if (Slice >= Resolution)
    return;

//  TraceDEnter("%u, %u, %u, %p", Slice, Depth, IterateBroods, F);

  Array<OctNode*> Nodes;
  GetSliceNodeList(&m_Root, Resolution >> 1, Resolution >> 2, Slice, Depth, IterateBroods, Nodes);

  #if defined(CONFIG_MULTITHREADED)
  CONST UINT BlockCount = m_pTree->m_ThreadCount;
  if (F.ThreadSafe)
  {
    UINT CountPerBlock = (UINT)Nodes.Length() / BlockCount;
    if (CountPerBlock == 0)
      CountPerBlock = (UINT)Nodes.Length();

    UINT Offset = 0;
    while (Offset < Nodes.Length())
    {
      IterateNodesInfo<NODEFUNCTION>* pInfo = new IterateNodesInfo<NODEFUNCTION>(F);
      UINT Count = Min(CountPerBlock, (UINT)Nodes.Length() - Offset);

      pInfo->pTree = m_pTree.Get();
      pInfo->pRoot = &m_Root;
      pInfo->Count = Count;
      pInfo->Depth = Depth;
      pInfo->Slice = Slice;
      pInfo->ppNodes = &Nodes[Offset];
      Offset += Count;

      DWORD (WINAPI *pFunc)(IterateNodesInfo<NODEFUNCTION>*) = &StreamingOctreeConstructor::IterateNodesProc<NODEFUNCTION>;
      m_pTree->m_IteratePool.Enqueue((LPTHREAD_START_ROUTINE)pFunc, pInfo);
    }

    m_pTree->m_IteratePool.Wait();
  }
  else
  {
    typename NODEFUNCTION::THREADLOCAL L;
    F.InitThreadLocal(m_pTree.Get(), &m_Root, L);

    for (UINT i=0; i<Nodes.Length(); i++)
      F.ProcessNode(Slice, Depth, Nodes[i], L);
  }
  #else
  typename NODEFUNCTION::THREADLOCAL L;
  F.InitThreadLocal(m_pTree.Get(), &m_Root, L);

  for (UINT i=0; i<Nodes.Length(); i++)
    F.ProcessNode(Slice, Depth, Nodes[i], L);
  #endif
}

//----------------------------------------------------------------------------------------------------
#if defined(CONFIG_MULTITHREADED)
template<class NODEFUNCTION>
DWORD StreamingOctreeConstructor::IterateUnprocessedPointsProc(IterateUnprocessedPointsInfo<NODEFUNCTION>* pInfo)
{
  typename NODEFUNCTION::THREADLOCAL L;
  pInfo->Function.InitThreadLocal(pInfo->pTree, pInfo->pRoot, L);

  for (UINT i=0; i<pInfo->Count; i++)
    pInfo->Function.ProcessPoint(pInfo->Slice, pInfo->Depth, pInfo->pPoints[i], L);

  /* Memory was allocated in IterateNodes */
  delete pInfo;
  return 0;
}
#endif
//----------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
template<class POINTFUNCTION>
VOID StreamingOctreeConstructor::IterateUnprocessedPoints(UINT Slice, USHORT Depth, POINTFUNCTION& F)
{
  #if defined(CONFIG_PROFILE)
  TimeAllFunctionCalls();
  #endif

  using Bolitho::Min;

  if (Slice >= (1u << Depth))
    return;

  for (UINT i=0; i<m_pTree->m_ThreadCount; i++)
  {
    if (m_pPerDepthData[Depth].m_UnprocessedPoints[i][Slice].IsEmpty())
      continue;

    IterateUnprocessedPointsInfo<POINTFUNCTION>* pInfo = new IterateUnprocessedPointsInfo<POINTFUNCTION>(F);

    pInfo->pTree = m_pTree.Get();
    pInfo->pRoot = &m_Root;
    pInfo->Count = m_pPerDepthData[Depth].m_UnprocessedPoints[i][Slice].Length();
    pInfo->Depth = Depth;
    pInfo->Slice = Slice;
    pInfo->pPoints = &m_pPerDepthData[Depth].m_UnprocessedPoints[i][Slice][0];

    DWORD (WINAPI *pFunc)(IterateUnprocessedPointsInfo<POINTFUNCTION>*) = &StreamingOctreeConstructor::IterateUnprocessedPointsProc<POINTFUNCTION>;
    m_pTree->m_IteratePool.Enqueue((LPTHREAD_START_ROUTINE)pFunc, pInfo);
  }
  m_pTree->m_IteratePool.Wait();

  //Array<UnprocessedPoint>& Points = m_pPerDepthData[Depth].m_UnprocessedPoints[Slice];
  /*
  #if defined(CONFIG_MULTITHREADED)
  CONST UINT BlockCount = CONFIG_REGIONCOUNT;
  if (F.ThreadSafe)
  {
    UINT CountPerBlock = (UINT)Points.Length() / BlockCount;
    if (CountPerBlock == 0)
      CountPerBlock = (UINT)Points.Length();

    UINT Offset = 0;
    while (Offset < Points.Length())
    {
      IterateUnprocessedPointsInfo<POINTFUNCTION>* pInfo = new IterateUnprocessedPointsInfo<POINTFUNCTION>(F);
      UINT Count = Min(CountPerBlock, (UINT)Points.Length() - Offset);

      pInfo->pTree = m_pTree.Get();
      pInfo->pRoot = &m_Root;
      pInfo->Count = Count;
      pInfo->Depth = Depth;
      pInfo->Slice = Slice;
      pInfo->pPoints = &Points[Offset];
      Offset += Count;

      DWORD (WINAPI *pFunc)(IterateUnprocessedPointsInfo<POINTFUNCTION>*) = &StreamingOctreeConstructor::IterateUnprocessedPointsProc<POINTFUNCTION>;
      m_pTree->m_IteratePool.Enqueue((LPTHREAD_START_ROUTINE)pFunc, pInfo);
    }

    m_pTree->m_IteratePool.Wait();
  }
  else
  {
    typename POINTFUNCTION::THREADLOCAL L;
    F.InitThreadLocal(m_pTree.Get(), &m_Root, L);

    for (UINT i=0; i<Points.Length(); i++)
      F.ProcessPoint(Slice, Depth, Points[i], L);
  }
  #else
  typename POINTFUNCTION::THREADLOCAL L;
  F.InitThreadLocal(m_pTree.Get(), &m_Root, L);

  for (UINT i=0; i<Points.Length(); i++)
    F.ProcessPoint(Slice, Depth, Points[i], L);
  #endif
  */

}
//---------------------------------------------------------------------------------------------------


