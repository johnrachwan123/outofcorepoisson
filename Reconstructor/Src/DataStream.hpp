/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/DataStream.hpp $
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

#pragma once

#include "IO/File.hpp"
#include "Memory/VirtualMemory.hpp"
#include "IO/Path.hpp"
#include "Utility/Flags.hpp"
#include "Math/Math.hpp"

using Bolitho::Ptr;
using Bolitho::IO::File;
using Bolitho::IO::FileView;
using Bolitho::IO::Path;
using Bolitho::IO::FileAccess;
using Bolitho::IO::FileMode;
using Bolitho::IO::FileShare;
using Bolitho::IO::FileFlags;
using Bolitho::PageAccess;

using Bolitho::OutOfMemoryException;
using Bolitho::InvalidOperationException;

#include "System/Threading/Thread.hpp"
#include "System/Threading/Event.hpp"
using Bolitho::System::Runnable;
using Bolitho::System::Thread;
using Bolitho::System::Event;

/* Flags for controlling the behaviour of stream initialization */
struct DataAccess
{
  enum
  {
    NONE = 1,       /* No access is needed to the data */
    WRITE = 2,      /* Write access is needed to the data */
    READ = 4,       /* Read access is needed to the data */
    READWRITE = 6,

    AUTOGROW = 64,  /* Automatically grow the stream when moving the head */
  };
};

typedef ULONGLONG STREAMOFFSET;

/* Represents a stream of fixed width records
 * A single region of records is in memory at any time.  The region must move sequentially through the data
 */
class DataStream : public Object
{
public:
  DataStream();
  ~DataStream();

  /* Defines the file used to back the datastream */
  VOID SetBackingStore(Ptr<File> BackingStore, SIZE_T RecordSize);
  VOID SetBackingStore(LPCTSTR Filename, SIZE_T RecordSize);
  
  /* Prepare the datastream for access.  Flags specify the access required */
  VOID Initialize(DWORD Flags);

  /* Free resources allocated in Initialise */
  VOID Finalize(DWORD Flags = 0);

  /* Change Head/Tail positions */
  VOID SetHeadPosition(STREAMOFFSET Head);
  VOID SetTailPosition(STREAMOFFSET Tail);

  /* Advances the head position by Delta records */
  STREAMOFFSET AdvanceHeadPosition(STREAMOFFSET Delta)
  { SIZE_T V = m_Head; SetHeadPosition(m_Head+Delta); return V; }
  /* Advances the tail position by Delta records */
  STREAMOFFSET AdvanceTailPosition(STREAMOFFSET Delta)
  { SIZE_T V = m_Tail; SetTailPosition(m_Tail+Delta); return V; }

  /* Resizes the stream to contain 'Count' records */
  VOID Resize(STREAMOFFSET Count);

  template<class RECORDTYPE>
  RECORDTYPE& Get(STREAMOFFSET i);
  template<class RECORDTYPE>
  CONST RECORDTYPE& Get(STREAMOFFSET i) CONST;

  STREAMOFFSET GetHeadPosition() CONST
  { return m_Head; }
  STREAMOFFSET GetTailPosition() CONST
  { return m_Tail; }

  /* Returns the number of records in the stream */
  STREAMOFFSET Count() CONST
  { return m_Count; }
  /* Returns the number of bytes in the stream */
  ULONGLONG Length() CONST
  { return m_Count * m_RecordSize; }

  /* Returns a pointer to the beginning of the mapped memory region */
  template<class RECORDTYPE>
  CONST RECORDTYPE* GetBasePointer() CONST
  { return (CONST RECORDTYPE*)m_pView->GetBaseAddress(); }

  static CONST STREAMOFFSET INVALIDINDEX = (STREAMOFFSET)-1;
protected:
  /* The file and mapped view of the file */
  Ptr<File> m_pFile;
  Ptr<FileView> m_pView;
  
  DWORD m_PageAccess;
  DWORD m_Flags;
  /* The number of elements in the data stream */
  STREAMOFFSET m_Count;
  STREAMOFFSET m_RecordSize;

  STREAMOFFSET m_Head;
  STREAMOFFSET m_Tail;
  STREAMOFFSET m_WindowHead;
  STREAMOFFSET m_WindowTail;
  STREAMOFFSET m_WindowAdvanceSpeed;

  static CONST STREAMOFFSET WINDOWGRANULARITY = 65536; 
};

#include "DataStreamImpl.hpp"
