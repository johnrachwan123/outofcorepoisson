/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/Threading/CriticalSection.cpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1631 $
 * Last Updated: $Date: 2008-09-15 07:52:23 -0700 (Mon, 15 Sep 2008) $
 * 
 * 
 * Copyright (c) 2004-2007, Matthew G Bolitho
 * All rights reserved.
 *
 *
 * Microsoft Reference License (Ms-RL)
 *   
 * This license governs use of the accompanying software. If you use the software, you accept 
 * this license. If you do not accept the license, do not use the software.
 *
 * 1. Definitions
 *  - The terms "reproduce," "reproduction" and "distribution" have the same meaning here as under 
 *    U.S. copyright law.
 *  - "You" means the licensee of the software.
 *  - "Your company" means the company you worked for when you downloaded the software.
 *  - "Reference use" means use of the software within your company as a reference, in read only 
 *    form, for the sole purposes of debugging your products, maintaining your products, or 
 *    enhancing the interoperability of your products with the software, and specifically excludes 
 *    the right to distribute the software outside of your company.
 *  - "Licensed patents" means any Licensor patent claims which read directly on the software as 
 *    distributed by the Licensor under this license.
 * 
 * 2. Grant of Rights
 *  (A) Copyright Grant- Subject to the terms of this license, the Licensor grants you a non-transferable, 
 *      non-exclusive, worldwide, royalty-free copyright license to reproduce the software for reference use.
 *  (B) Patent Grant- Subject to the terms of this license, the Licensor grants you a non-transferable,
 *      non-exclusive, worldwide, royalty-free patent license under licensed patents for reference use.
 * 
 * 3. Limitations
 *  (A) No Trademark License - This license does not grant you any rights to use the Licensor's name
 *      logo, or trademarks.
 *  (B) If you begin patent litigation against the Licensor over patents that you think may apply 
 *      to the software (including a cross-claim or counterclaim in a lawsuit), your license to the 
 *      software ends automatically.
 *  (C) The software is licensed "as-is." You bear the risk of using it. The Licensor gives no express 
 *      warranties, guarantees or conditions. You may have additional consumer rights under your local 
 *      laws which this license cannot change. To the extent permitted under your local laws, the 
 *      Licensor excludes the implied warranties of merchantability, fitness for a particular purpose 
 *      and non-infringement.
 *
 ***************************************************************************************************/




#include "CodeLibrary.hpp"

#include "Debug/Trace.hpp"
#include "System/Threading/CriticalSection.hpp"

using namespace Bolitho;
using namespace Bolitho::System;

//----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::System,CriticalSection,Object);
//----------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_WINDOWS)
CriticalSection::CriticalSection()
{
  InitializeCriticalSection(&m_hCriticalSection);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
CriticalSection::~CriticalSection()
{
  DeleteCriticalSection(&m_hCriticalSection);
  ZeroMemory(&m_hCriticalSection, sizeof(m_hCriticalSection));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CriticalSection::Lock(DWORD MaxTime)
{
  #if defined(TRACE_LOCKING)
  if (MaxTime == INFINITE)
  {
    Trace(this, 0, "Thread %p attempting to lock CriticalSection %p", Thread::GetCurrentThreadId(), &m_hCriticalSection);
    EnterCriticalSection(&m_hCriticalSection);
    Trace(this, 0, "Thread %p aquired CriticalSection %p", Thread::GetCurrentThreadId(), &m_hCriticalSection);
  }
  else
  {
    Trace(this, 0, "Thread %p attempting to lock CriticalSection %p", Thread::GetCurrentThreadId(), &m_hCriticalSection);
    BOOL OK = (TryEnterCriticalSection(&m_hCriticalSection) == TRUE);
    if (OK)
      Trace(this, 0, "Thread %p aquired CriticalSection %p", Thread::GetCurrentThreadId(), &m_hCriticalSection);
    else
      Trace(this, TraceLevel::WARNING, "Thread %p failed to aquire CriticalSection %p", Thread::GetCurrentThreadId(), &m_hCriticalSection);
    m_Timer.Start();
    return OK;
  }
  return TRUE;
  #else
  if (MaxTime == INFINITE)
    EnterCriticalSection(&m_hCriticalSection);
  else
    return TryEnterCriticalSection(&m_hCriticalSection) == TRUE;
  return TRUE;
  #endif
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID CriticalSection::Unlock()
{
  #if defined(TRACE_LOCKING)
  m_Timer.Stop();
  LeaveCriticalSection(&m_hCriticalSection);
  Trace("Thread %p released CriticalSection %p. Lock duration: %f", Thread::GetCurrentThreadId(), &m_hCriticalSection, m_Timer.ElapsedSeconds());

  #else
  LeaveCriticalSection(&m_hCriticalSection);
  #endif
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_POSIX)
CriticalSection::CriticalSection()
{
  pthread_mutex_init(&m_hCriticalSection, 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
CriticalSection::~CriticalSection()
{
  pthread_mutex_destroy(&m_hCriticalSection); 
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CriticalSection::Lock(DWORD MaxTime)
{
  return pthread_mutex_lock(&m_hCriticalSection) == 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID CriticalSection::Unlock()
{
  pthread_mutex_unlock(&m_hCriticalSection);
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------
