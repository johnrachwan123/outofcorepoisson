/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/NewDataStreamImpl.hpp $
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

using Bolitho::SystemException;

//---------------------------------------------------------------------------------------------------
ImplementTemplateRuntimeType(,DataStream,Object);
//---------------------------------------------------------------------------------------------------
template<class RECORDTYPE>
DataStream<RECORDTYPE>::DataStream()
{
  m_Count = 0;
  m_WindowHead = 0;
  m_WindowTail = 0;
  m_Head = 0;
  m_Tail = 0;
}
//---------------------------------------------------------------------------------------------------
template<class RECORDTYPE>
VOID DataStream<RECORDTYPE>::SetBackingStore(LPCTSTR Filename)
{
  SetBackingStore(File::Open(Filename, FileMode::OPEN, FileAccess::READWRITE, FileShare::READ, FileFlags::SEQUENTIAL));
}
//---------------------------------------------------------------------------------------------------
template<class RECORDTYPE>
VOID DataStream<RECORDTYPE>::SetBackingStore(Ptr<File> BackingStore)
{
  m_pFile = BackingStore;
  m_Count = m_pFile->Length() / sizeof(RECORDTYPE);
}
//---------------------------------------------------------------------------------------------------
template<class RECORDTYPE>
DataStream<RECORDTYPE>::~DataStream()
{
  Finalise();

  m_pFile->Close();
  m_pFile = 0;
}
//---------------------------------------------------------------------------------------------------
template<class RECORDTYPE>
VOID DataStream<RECORDTYPE>::Initialize(DWORD Flags)
{
  if (IsFlagSet(Flags,DataAccess::NONE))
  {
    m_Flags = Flags;
    m_Head = 0;
    m_Tail = 0;
    return;
  }

  if (m_pFile == 0 && !IsFlagSet(Flags,DataAccess::READWRITE))
    throw InvalidOperationException(this, TEXT("DataStream without backing store must be READWRITE"));

  if (IsFlagSet(Flags, DataAccess::READWRITE) || IsFlagSet(Flags, DataAccess::WRITE))
    m_PageAccess = PageAccess::READWRITE;
  else
    m_PageAccess = PageAccess::READ;

  m_pData = VirtualReserve(m_Count*sizeof(RECORDTYPE));

  m_Flags = Flags;
  m_Head = 0;
  m_Tail = 0;

  m_WindowAdvanceSpeed = 0;
  m_WindowHead = 0;
  m_WindowTail = 0;

}
//---------------------------------------------------------------------------------------------------
template<class RECORDTYPE>
VOID DataStream<RECORDTYPE>::Finalise(DWORD Flags)
{
  using Bolitho::Math::RoundUp;

  SIZE_T BlockSize = m_WindowHead-m_WindowTail;
  if (m_WindowTail + BlockSize > m_Count)
    BlockSize = m_Count - m_WindowTail;
  BlockSize *= sizeof(RECORDTYPE);
  LPVOID pBlock = (LPRECORDTYPE)m_pData + m_WindowTail;

  if (IsFlagSet(m_Flags, DataAccess::WRITE))
  {
    m_pFile->Seek(m_WindowTail*sizeof(RECORDTYPE));
    m_pFile->Write(pBlock, BlockSize);
  }

  m_Head = m_Tail = m_Count;
  m_WindowHead = m_WindowTail = RoundUp(m_Count, WINDOWGRANULARITY);

  VirtualRelease(m_pData);
  m_pData = 0;
}
//---------------------------------------------------------------------------------------------------
template<class RECORDTYPE>
VOID DataStream<RECORDTYPE>::SetHeadPosition(ULONGLONG Head)
{
  using Bolitho::Math::RoundUp;

  m_Head = Head;

  if (m_Head > m_Count)
  {
    if (!IsFlagSet(m_Flags, DataAccess::AUTOGROW))
      throw InvalidOperationException(this, TEXT("Head advanced beyond stream extent"));

    SIZE_T BlockSize = m_WindowHead-m_WindowTail;
    if (m_WindowTail + BlockSize > m_Count)
      BlockSize = m_Count - m_WindowTail;
    BlockSize *= sizeof(RECORDTYPE);
    LPVOID pBlock = (LPRECORDTYPE)m_pData + m_WindowTail;

    if (IsFlagSet(m_Flags, DataAccess::WRITE))
    {
      m_pFile->Seek(m_WindowTail*sizeof(RECORDTYPE));
      m_pFile->Write(pBlock, BlockSize);
    }

    VirtualRelease(m_pData);
    m_pData = 0;
    
    ULONGLONG OldCount = m_Count;
    ULONGLONG GrowSize = m_Count / 16;
    ULONGLONG Length = m_pFile->Resize(RoundUp(m_Head+GrowSize, WINDOWGRANULARITY)*sizeof(RECORDTYPE));

    #if defined(PLATFORM_32BIT)
    if (Length > MAXLONG)
      throw OutOfMemoryException(this, TEXT("The DataStream is larger that addressable memory"));
    m_Count = (ULONGLONG)(Length / sizeof(RECORDTYPE));
    #endif

    #if defined(PLATFORM_64BIT)
    m_Count = Length / sizeof(RECORDTYPE);
    #endif

    TraceWarning(this, "Stream size grown from %llu to %llu records", OldCount, m_Count);

    m_pData = VirtualReserve(m_Count*sizeof(RECORDTYPE));

    BlockSize = m_WindowHead-m_WindowTail;
    if (m_WindowTail + BlockSize > m_Count)
      BlockSize = m_Count - m_WindowTail;
    BlockSize *= sizeof(RECORDTYPE);
    pBlock = (LPRECORDTYPE)m_pData + m_WindowTail;

    if (VirtualCommit(pBlock, BlockSize, PageAccess::READWRITE) == 0)
      SystemException::ThrowError();
    
    m_pFile->Seek(m_WindowTail*sizeof(RECORDTYPE));
    m_pFile->Read(pBlock, BlockSize);
  }

  ULONGLONG NewWindowHead = RoundUp(Head, WINDOWGRANULARITY);
  if (NewWindowHead > m_WindowHead)
  {
    LPVOID pNewBlock = (LPRECORDTYPE)m_pData + m_WindowHead;
    SIZE_T BlockSize = NewWindowHead - m_WindowHead;
    if (m_WindowHead + BlockSize > m_Count)
      BlockSize = m_Count - m_WindowHead;
    BlockSize *= sizeof(RECORDTYPE);

    if (VirtualCommit(pNewBlock, BlockSize, PageAccess::READWRITE) == 0)
      SystemException::ThrowError();

    if (IsFlagSet(m_Flags, DataAccess::READ))
    {
      m_pFile->Seek(sizeof(RECORDTYPE) * m_WindowHead);
      m_pFile->Read(pNewBlock, BlockSize);
    }
  }
  m_WindowHead = NewWindowHead;

}
//---------------------------------------------------------------------------------------------------
template<class RECORDTYPE>
VOID DataStream<RECORDTYPE>::SetTailPosition(ULONGLONG Tail)
{
  using Bolitho::Math::RoundDown;

  m_Tail = Tail;
  ULONGLONG NewWindowTail = RoundDown(m_Tail, WINDOWGRANULARITY);

  if (NewWindowTail > m_WindowTail)
  {
    LPVOID pOldBlock = (LPRECORDTYPE)m_pData + m_WindowTail;
    SIZE_T BlockSize = NewWindowTail - m_WindowTail;
    if (m_WindowTail + BlockSize > m_Count)
      BlockSize = m_WindowTail + BlockSize - m_Count;
    BlockSize *= sizeof(RECORDTYPE);

    if (IsFlagSet(m_Flags, DataAccess::WRITE))
    {
      m_pFile->Seek(sizeof(RECORDTYPE) * m_WindowTail);
      m_pFile->Write(pOldBlock, BlockSize);
    }

    VirtualDecommit(pOldBlock, BlockSize);
  }

  m_WindowTail = NewWindowTail;
}
//---------------------------------------------------------------------------------------------------
template<class RECORDTYPE>
VOID DataStream<RECORDTYPE>::Resize(ULONGLONG Count)
{
  using Bolitho::Math::RoundUp;

  Assert(m_pData == 0);

  m_Count = RoundUp(Count, WINDOWGRANULARITY);
  m_pFile->Resize(Count*sizeof(RECORDTYPE));
}
//---------------------------------------------------------------------------------------------------
template<class RECORDTYPE>
RECORDTYPE& DataStream<RECORDTYPE>::operator[] (ULONGLONG i)
{
  Assert(i >= m_Tail);
  Assert(i < m_Head);

  return ((LPRECORDTYPE)m_pData)[i];
}
//---------------------------------------------------------------------------------------------------
template<class RECORDTYPE>
CONST RECORDTYPE& DataStream<RECORDTYPE>::operator[] (ULONGLONG i) const
{
  Assert(i >= m_Tail);
  Assert(i < m_Head);

  return ((LPRECORDTYPE)m_pData)[i];
}
//---------------------------------------------------------------------------------------------------


#if 0
//---------------------------------------------------------------------------------------------------
/* Disable the optimizer to prevent the read-ahead loop from being optimized away */
#pragma optimize("", off)
template<class RECORDTYPE>
INT DataStream<RECORDTYPE>::ThreadMain()
{
  using Bolitho::Min;
  using Bolitho::Math::RoundUp;

  ULONGLONG Head = 0;
  ULONGLONG Tail = 0;

  while (TRUE)
  {
    /* Wait for the Head to advance, or the DataStream to be finalized */
    m_StreamAdvance.Wait();

    /* If the window head and tail are at the final position, exit */
    if (m_WindowHead == RoundUp(m_Count, WINDOWGRANULARITY) && m_WindowTail == RoundUp(m_Count, WINDOWGRANULARITY))
      break;

    /* Read ahead by at least m_WindowAdvanceSpeed size.  That is the size of the previous advance */
    ULONGLONG MaxHead = Min(m_WindowHead + m_WindowAdvanceSpeed, m_Count);
    if (Head < MaxHead)
    {
      /* Touch memory to pre-fault it */
      for (ULONGLONG i=Head; i<MaxHead; i+= 4096 / sizeof(RECORDTYPE))
        RECORDTYPE R = ((LPRECORDTYPE)m_pView->GetBaseAddress())[i];

      Head = MaxHead;
    }

    if (!IsFlagSet(m_Flags, DataAccess::WRITE))
      continue;

    /* Evict trailing pages from memory */
    if (Tail < m_WindowTail)
    {
      m_pView->Unlock((SIZE_T)Tail*sizeof(RECORDTYPE), (SIZE_T)(m_WindowTail - Tail)*sizeof(RECORDTYPE));
      Tail = m_WindowTail;
    }

  }

  return 0;
}
#pragma optimize("", on)
//---------------------------------------------------------------------------------------------------
#endif