/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/Threading/Thread.hpp $
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

#pragma once

#include "Collections/Map.hpp"
#include "Time/DateTime.hpp"

namespace Bolitho
{
  namespace System
  {
    class Thread;

    class LIBRARY_API Runnable : virtual public Object
    {
      friend class Thread;
    protected:
      virtual BOOL ThreadInit()
      { return TRUE; }
      virtual INT ThreadMain() = 0;
      virtual VOID ThreadExit()
      { return; }
      virtual VOID ThreadException(Exception& E);
    };

    /* An interface that represents an object that can be waited upon */
    class LIBRARY_API IWaitable
    {
    public:
      virtual BOOL Wait(DWORD Timeout = INFINITE) = 0;
    };
    
    class LIBRARY_API Thread : virtual public Object
    {
      DeclareRuntimeType();
    public:
      virtual ~Thread();

      /* Returns a Thread object that represents an already running thread */
      static Ptr<Thread> Attach(THREADHANDLE hThread);
      /* Returns a Thread object that represents a new thread.  The thread executes a Runnable object */
      static Ptr<Thread> Create(Runnable* pRunnable);
      /* Returns a Thread object that represents a new thread.  The thread executes a thread start routine */
      static Ptr<Thread> Create(LPTHREAD_START_ROUTINE Proc, LPVOID Ptr);

      /* Terminates the thread */
      VOID Terminate();

      /* Returns a numeric identifier for the thead */
      DWORD ThreadId() CONST;
      /* Gets the thread scheduling priority */
      INT GetPriority() CONST;
      /* Sets the thread scheduling priority */
      VOID SetPriority(INT Priority);
      /* Returns TRUE if the thread is currently performing blocking IO */
      BOOL IsBlocking() CONST;
      /* Sets the affinity of this thread to processors */
      VOID SetThreadAffinity(DWORD_PTR ProcessorMask);
      /* Get the processor affinity of the thread */
      DWORD_PTR GetThreadAffinity() CONST;
      
      /* The OS handle for the thread */
      THREADHANDLE Handle() CONST
      { return m_hThread; }

      /* Blocks a calling thread until this thread terminates */
      BOOL Wait(DWORD Timeout = INFINITE);

      /* Causes the calling thread to sleep for a given number of milli-seconds */
      static VOID Sleep(DWORD t = 10);
      /* Returns a Thread object that represents the calling thread */
      static Ptr<Thread> GetCurrentThread();
      /* Returns the numeric identifier for the calling thread */
      static DWORD GetCurrentThreadId();

      /* Allocates a slot in thread local storage for the calling thread */
      static DWORD AllocateThreadLocalStorage();
      /* Frees a previously allocated slot in thread local storage for the calling thread */
      static VOID FreeThreadLocalStorage(DWORD TlsHandle);
      /* Sets the value of a slot in thread local storage for the calling thread */
      static VOID SetThreadLocalValue(DWORD TlsHandle, LPVOID Value);
      /* Gets the value of a slot in thread local storage for the calling thread */
      static LPVOID GetThreadLocalValue(DWORD TlsHandle);

      /* Per-thread statistics */
      struct THREADCOUNTERS
      {
        DateTime CreationTime;
        DateTime ExitTime;

        TimeSpan KernelTime;
        TimeSpan UserTime;
      };

      /* Collects and returns per thread statistics */
      THREADCOUNTERS GetThreadCounters();

      /* The thread entrypoint used to run a Runnable object */
      static DWORD WINAPI RunnableThreadProc(LPVOID pData);
      static DWORD WINAPI ThreadProc(LPVOID pData);
    protected:
		  struct THREADSTART
      {
        LPTHREAD_START_ROUTINE Proc;
        LPVOID Ptr;
        Thread* pThread;
      };

      Thread(THREADHANDLE hThread);

      /* The OS handle for the thread */
      THREADHANDLE m_hThread;

      /* A lookup table of OS thread handles to Thread objects */
      static Map<THREADHANDLE, Thread*> m_ThreadMap;
    };
  }
}
