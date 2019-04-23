/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/StreamManager.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 914 $
 * Last Updated: $Date: 2008-12-29 13:34:43 -0800 (Mon, 29 Dec 2008) $
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
#include "StreamManager.hpp"

//---------------------------------------------------------------------------------------------------
SIZE_T StreamSizes[] = { sizeof(PARTITION0), sizeof(PARTITION1), sizeof(PARTITION2), sizeof(PARTITION3), sizeof(PARTITION4), sizeof(PARTITION5) };
//---------------------------------------------------------------------------------------------------
StreamManager::StreamManager()
{
  m_pDepthStreams = 0;
}
//---------------------------------------------------------------------------------------------------
VOID StreamManager::Create(LPCTSTR InputFilename, LPCTSTR StreamingFilename, USHORT MaximumDepth, BOOL DeleteOnCompletion)
{
  m_pDepthStreams = new PerDepthStreams[MaximumDepth+1];

  /* Create the initial node index */
  m_IndexCount = ((1u<<(MaximumDepth + 1))-1) * CONFIG_REGIONCOUNT;
  m_pNodeIndex = new SPATIALINDEX[m_IndexCount];

  ZeroMemory(m_pNodeIndex, sizeof(SPATIALINDEX) * m_IndexCount);
  m_pNodeIndex[0].Length = 1;

  /* Initialize the point index */
  m_pPointIndex = new SPATIALINDEX[m_IndexCount];
  ZeroMemory(m_pPointIndex, sizeof(SPATIALINDEX) * m_IndexCount);
  File::ReadStruct(String::Format("%s.PointIndex", Path::Basename(InputFilename).ConstStr()), 0, m_pPointIndex, m_IndexCount);

  /* Initialize the point data stream */
  m_PointData.SetBackingStore(File::Open(InputFilename, FileMode::OPEN, FileAccess::READWRITE, FileShare::READWRITE, FileFlags::SEQUENTIAL), sizeof(ORIENTEDPOINT));

  /* Create the per-depth node streams */
  for (UINT i=0; i<=MaximumDepth; i++)
  {
    SIZE_T ApproximateCount = (i < 5) ? (1<<i)*(1<<i)*(1<<i) : 4*(1<<i)*(1<<i);
    
    for (UINT j=0; j<CONFIG_STREAMCOUNT; j++)
    {
      Ptr<File> pStream = File::Open(String::Format("%s.NodeStream-%d-%d", StreamingFilename, i, j), FileMode::CREATE, FileAccess::READWRITE, FileShare::NONE, FileFlags::DELETEONCLOSE | FileFlags::SEQUENTIAL);
      pStream->Resize(StreamSizes[j] * ApproximateCount);

      m_pDepthStreams[i].m_Streams[j].SetBackingStore(pStream, StreamSizes[j]); 
    }
  }

}
//---------------------------------------------------------------------------------------------------
SPATIALINDEX* StreamManager::GetNodeIndex(USHORT Depth, UINT Index, UINT Region)
{
  UINT BaseIndex = (1<<Depth)-1;
  Index &= 0xfffffffe;
  Assert(Index <= BaseIndex);

  return &m_pNodeIndex[(BaseIndex + Index) * CONFIG_REGIONCOUNT + Region];
}
//----------------------------------------------------------------------------------------------------
SPATIALINDEX* StreamManager::GetPointIndex(USHORT Depth, UINT Index, UINT Region)
{
  UINT BaseIndex = (1<<Depth)-1;
  Assert(Index <= BaseIndex);

  return &m_pPointIndex[(BaseIndex + Index) * CONFIG_REGIONCOUNT + Region];
}
//----------------------------------------------------------------------------------------------------
VOID StreamManager::SetPointStreamWindow(STREAMOFFSET Tail, STREAMOFFSET Head)
{
  m_PointData.SetHeadPosition(Head);
  m_PointData.SetTailPosition(Tail);
}
//----------------------------------------------------------------------------------------------------
VOID StreamManager::SetNodeStreamWindow(USHORT Depth, STREAMOFFSET Tail, STREAMOFFSET Head)
{
  for (UINT i=0; i<CONFIG_STREAMCOUNT; i++)
  {
    m_pDepthStreams[Depth].m_Streams[i].SetHeadPosition(Head);
    m_pDepthStreams[Depth].m_Streams[i].SetTailPosition(Tail);
  }
}
//----------------------------------------------------------------------------------------------------
VOID StreamManager::InitializePointStream(DWORD DataUsage)
{
  m_PointData.Initialize(DataUsage);
}
//----------------------------------------------------------------------------------------------------
VOID StreamManager::InitializeNodeStream(USHORT Depth, DWORD DataUsage)
{
  for (UINT i=0; i<CONFIG_STREAMCOUNT; i++)
    m_pDepthStreams[Depth].m_Streams[i].Initialize(DataUsage);
}
//----------------------------------------------------------------------------------------------------
VOID StreamManager::FinalizePointStream()
{
  m_PointData.Finalize();
}
//----------------------------------------------------------------------------------------------------
VOID StreamManager::FinalizeNodeStream(USHORT Depth)
{
  for (UINT i=0; i<CONFIG_STREAMCOUNT; i++)
    m_pDepthStreams[Depth].m_Streams[i].Finalize();
}
//----------------------------------------------------------------------------------------------------
STREAMOFFSET StreamManager::AdvancePointStreamWindow(STREAMOFFSET Delta)
{
  STREAMOFFSET OldHead = m_PointData.GetHeadPosition();
  m_PointData.AdvanceHeadPosition(Delta);
  m_PointData.AdvanceTailPosition(Delta);
  return OldHead;
}
//----------------------------------------------------------------------------------------------------
STREAMOFFSET StreamManager::AdvanceNodeStreamWindow(USHORT Depth, STREAMOFFSET Delta)
{
  STREAMOFFSET OldHead = m_pDepthStreams[Depth].m_Streams[0].GetHeadPosition();
  for (UINT i=0; i<CONFIG_STREAMCOUNT; i++)
  {
    m_pDepthStreams[Depth].m_Streams[i].AdvanceHeadPosition(Delta);
    m_pDepthStreams[Depth].m_Streams[i].AdvanceTailPosition(Delta);
  }
  return OldHead;
}
//----------------------------------------------------------------------------------------------------
