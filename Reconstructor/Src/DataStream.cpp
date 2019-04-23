/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/DataStream.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 913 $
 * Last Updated: $Date: 2008-12-29 13:34:18 -0800 (Mon, 29 Dec 2008) $
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
#include "DataStream.hpp"

//---------------------------------------------------------------------------------------------------
//ImplementRuntimeType(,DataStream,Object);
//---------------------------------------------------------------------------------------------------
DataStream::DataStream()
{
  m_Count = 0;
  m_WindowHead = 0;
  m_WindowTail = 0;
  m_Head = 0;
  m_Tail = 0;
  m_RecordSize = 0;
}
//---------------------------------------------------------------------------------------------------
VOID DataStream::SetBackingStore(LPCTSTR Filename, SIZE_T RecordSize)
{
  SetBackingStore(File::Open(Filename, FileMode::OPEN, FileAccess::READWRITE, FileShare::READ, FileFlags::SEQUENTIAL), RecordSize);
}
//---------------------------------------------------------------------------------------------------
VOID DataStream::SetBackingStore(Ptr<File> BackingStore, SIZE_T RecordSize)
{
  m_pFile = BackingStore;
  m_RecordSize = RecordSize;

#if defined(PLATFORM_32BIT)
  if (m_pFile->Length() > MAXLONG)
    throw OutOfMemoryException(this, TEXT("The DataStream is larger that addressable memory"));
  m_Count = (STREAMOFFSET)(m_pFile->Length() / m_RecordSize);
  #endif

  #if defined(PLATFORM_64BIT)
  m_Count = m_pFile->Length() / m_RecordSize;
  #endif

}
//---------------------------------------------------------------------------------------------------
DataStream::~DataStream()
{
  Finalize();

  m_pView = 0;
  m_pFile->Close();
  m_pFile = 0;
}
//---------------------------------------------------------------------------------------------------
VOID DataStream::Initialize(DWORD Flags)
{
  if (IsFlagSet(Flags,DataAccess::NONE))
  {
    m_Flags = Flags;
    m_Head = 0;
    m_Tail = 0;
    return;
  }

  if (m_RecordSize == 0)
    throw InvalidOperationException(this, TEXT("The Record Size has not been set"));

  if (m_pFile == 0 && !IsFlagSet(Flags,DataAccess::READWRITE))
    throw InvalidOperationException(this, TEXT("DataStream without backing store must be READWRITE"));

  if (IsFlagSet(Flags, DataAccess::READWRITE) || IsFlagSet(Flags, DataAccess::WRITE))
    m_PageAccess = PageAccess::READWRITE;
  else
    m_PageAccess = PageAccess::READ;

  m_pView = m_pFile->MapView(m_PageAccess);

  m_Flags = Flags;
  m_Head = 0;
  m_Tail = 0;

  m_WindowAdvanceSpeed = 0;
  m_WindowHead = 0;
  m_WindowTail = 0;

  #if defined(CONFIG_ASYNCDATASTREAM)
  m_pThread = Thread::Create(this);
  #endif
}
//---------------------------------------------------------------------------------------------------
VOID DataStream::Finalize(DWORD Flags)
{
  using Bolitho::Math::RoundUp;

  m_Head = m_Tail = m_Count;
  m_WindowHead = m_WindowTail = RoundUp(m_Count, WINDOWGRANULARITY);

  #if defined(CONFIG_ASYNCDATASTREAM)
  if (!m_pThread->Wait(10000))
    m_pThread->Terminate();
  #endif

  m_pView = 0;
}
//---------------------------------------------------------------------------------------------------
VOID DataStream::SetHeadPosition(STREAMOFFSET Head)
{
  using Bolitho::Math::RoundUp;

  m_Head = Head;

  if (m_Head > m_Count)
  {
    if (!IsFlagSet(m_Flags, DataAccess::AUTOGROW))
      throw InvalidOperationException(this, TEXT("Head advanced beyond stream extent"));

    m_pView = 0;

    STREAMOFFSET OldCount = m_Count;
    STREAMOFFSET GrowSize = m_Count / 16;
    STREAMOFFSET Length = m_pFile->Resize(RoundUp(m_Head+GrowSize, WINDOWGRANULARITY)*m_RecordSize);

    #if defined(PLATFORM_32BIT)
    if (Length > MAXLONG)
      throw OutOfMemoryException(this, TEXT("The DataStream is larger that addressable memory"));
    m_Count = (STREAMOFFSET)(Length / m_RecordSize);
    #endif

    #if defined(PLATFORM_64BIT)
    m_Count = Length / m_RecordSize;
    #endif

    m_pView = m_pFile->MapView(m_PageAccess);
  }

  STREAMOFFSET NewWindowHead = RoundUp(Head, WINDOWGRANULARITY);
  if (NewWindowHead > m_WindowHead)
    m_WindowAdvanceSpeed = NewWindowHead - m_WindowHead;
  m_WindowHead = NewWindowHead;

}
//---------------------------------------------------------------------------------------------------
VOID DataStream::SetTailPosition(STREAMOFFSET Tail)
{
  using Bolitho::Math::RoundDown;

  m_Tail = Tail;
  STREAMOFFSET NewWindowTail = RoundDown(m_Tail, WINDOWGRANULARITY);

  if (NewWindowTail > m_WindowTail)
    VirtualHint((LPBYTE)m_pView->GetBaseAddress() + m_WindowTail*m_RecordSize, (NewWindowTail-m_WindowTail)*m_RecordSize, Bolitho::PageHint::PAGEOUT);

  m_WindowTail = NewWindowTail;
}
//---------------------------------------------------------------------------------------------------
VOID DataStream::Resize(STREAMOFFSET Count)
{
  Assert(m_pView == 0);

  m_Count = Count;
  m_pFile->Resize(Count*m_RecordSize);
}
//---------------------------------------------------------------------------------------------------


