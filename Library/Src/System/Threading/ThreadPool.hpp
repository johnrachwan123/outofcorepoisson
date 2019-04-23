/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/Threading/ThreadPool.hpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1711 $
 * Last Updated: $Date: 2008-11-17 10:09:48 -0800 (Mon, 17 Nov 2008) $
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

#include "System/Threading/Thread.hpp"
#include "System/Threading/ConditionVariable.hpp"
#include "Collections/Array.hpp"
#include "Collections/Queue.hpp"
#include "Time/Timer.hpp"

namespace Bolitho
{
  namespace System
  {
    class LIBRARY_API ThreadPool : public Object
    {
      DeclareRuntimeType();
    public:
      class LIBRARY_API WorkItem
      {
        friend class ThreadPool;
      protected:
        LPTHREAD_START_ROUTINE m_pProc;
        LPVOID m_pParam;
        DWORD m_ReturnValue;
        BOOL m_Running;
        BOOL m_Complete;
        ConditionVariable m_StateChange;
      public:
        WorkItem(LPTHREAD_START_ROUTINE pProc, LPVOID pParam);

        BOOL Wait(DWORD Timeout = INFINITE);

        DWORD ReturnValue()
        { Wait(); return m_ReturnValue; }
      };

      ThreadPool();
      virtual ~ThreadPool();

      VOID Initialize(LONG MinimumThreadCount, LONG MaximumThreadCount);
      VOID Start();
      VOID Stop();
      VOID Pause();
      VOID Resume();

      BOOL Wait(DWORD Timeout = INFINITE);

      VOID Enqueue(LPTHREAD_START_ROUTINE Proc, LPVOID Param);
      VOID Enqueue(Runnable* pRunnable);

      UINT GetCurrentThreadCount() CONST
      { return m_CurrentThreadCount; }
      UINT GetMinimumThreadCount() CONST
      { return m_MinThreadCount; }
      UINT GetMaximumThreadCount() CONST
      { return m_MaxThreadCount; }

      VOID SetThreadAffinity(UINT ThreadId, DWORD_PTR AffinityMask)
      { m_Threads[ThreadId]->SetThreadAffinity(AffinityMask); }

      STATIC UINT GetThreadId();

    private:
      struct WORKERTHREADSTART
      {
        ThreadPool* pPool;
        UINT ThreadId;
      };

      STATIC DWORD m_hThreadId;

      UINT m_MinThreadCount;
      UINT m_MaxThreadCount;
      UINT m_CurrentThreadCount;

      UINT m_CurrentJobCount;
      UINT m_RunningJobCount;

      ArrayPtr<Thread> m_Threads;
      VOID CreateWorkerThread();

      CriticalSection m_Lock;
      ConditionVariable m_Worker;
      ConditionVariable m_Master;

      Queue<WorkItem*> m_Queue;

      STATIC DWORD WINAPI WorkerThreadProc(LPVOID Param);
    };
  }
}
