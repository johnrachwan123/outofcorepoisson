/***************************************************************************************************
* File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/Threading/ThreadPool.cpp $
* Author:       $Author: BOLITHO\matthew $
* Revision:     $Rev: 1712 $
* Last Updated: $Date: 2008-11-17 11:25:49 -0800 (Mon, 17 Nov 2008) $
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
#include "System/Threading/ThreadPool.hpp"
#include "System/Threading/ScopedLock.hpp"

using namespace Bolitho;
using namespace Bolitho::System;

//----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::System,ThreadPool,Object);
//----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD ThreadPool::m_hThreadId = 0;
//----------------------------------------------------------------------------------------------------------------------------------------------------
ThreadPool::ThreadPool() : m_Master(m_Lock), m_Worker(m_Lock)
{
  m_MinThreadCount = 0;
  m_MaxThreadCount = 0;
  m_CurrentThreadCount = 0;

  m_CurrentJobCount = 0;
  m_RunningJobCount = 0;

  if (m_hThreadId == 0)
    m_hThreadId = Thread::AllocateThreadLocalStorage();

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ThreadPool::~ThreadPool()
{
  Wait();

  for (SIZE_T i=0; i<m_Threads.Length(); i++)
    m_Threads[i]->Terminate();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
UINT ThreadPool::GetThreadId()
{
  return (UINT)reinterpret_cast<SIZE_T>(Thread::GetThreadLocalValue(m_hThreadId));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ThreadPool::Initialize(LONG MinimumThreadCount, LONG MaximumThreadCount)
{
  m_MinThreadCount = MinimumThreadCount;
  m_MaxThreadCount = MaximumThreadCount;

  for (LONG i=0; i<MinimumThreadCount; i++)
    CreateWorkerThread();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ThreadPool::Stop()
{

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ThreadPool::Start()
{

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ThreadPool::Pause()
{

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ThreadPool::Resume()
{

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD WINAPI ThreadPool::WorkerThreadProc(LPVOID Param)
{
  WORKERTHREADSTART* pStart = (WORKERTHREADSTART*)Param;
  Thread::SetThreadLocalValue(m_hThreadId, (LPVOID)pStart->ThreadId);

  WorkItem* I;
  ThreadPool* pPool = pStart->pPool;

  while (TRUE)
  {
    Verify(pPool->m_Lock.Lock());

    while (pPool->m_Queue.IsEmpty())
      pPool->m_Worker.Wait();

    //Trace("Dequeue job.  New queue count is %u", pPool->m_Queue.Length());

    I = pPool->m_Queue.Dequeue();
    pPool->m_RunningJobCount++;
    pPool->m_Lock.Unlock();

    if (I->m_pProc != 0)
      (*I->m_pProc)(I->m_pParam);

    delete I;

    Verify(pPool->m_Lock.Lock());
    pPool->m_RunningJobCount--;

    pPool->m_Lock.Unlock();
    pPool->m_Master.Broadcast();
  }

  return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ThreadPool::CreateWorkerThread()
{
  WORKERTHREADSTART* pStart = new WORKERTHREADSTART();
  pStart->ThreadId = m_CurrentThreadCount++;
  pStart->pPool = this;

  Ptr<Thread> pThread = Thread::Create(WorkerThreadProc, pStart);
  m_Threads.Add(pThread);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL ThreadPool::Wait(DWORD Timeout)
{
  m_Lock.Lock();
  while (m_RunningJobCount > 0 || !m_Queue.IsEmpty())
  {
    BOOL Result = m_Master.Wait(Timeout);
    if (!Result)
    {
      m_Lock.Unlock();
      return FALSE;
    }
  }

  m_Lock.Unlock();
  return TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ThreadPool::Enqueue(LPTHREAD_START_ROUTINE Proc, LPVOID Param)
{
  Verify(m_Lock.Lock());
  m_Queue.Enqueue(new WorkItem(Proc,Param));
  m_Worker.Broadcast();
  m_Lock.Unlock();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ThreadPool::Enqueue(Runnable* pRunnable)
{
  Enqueue(&Thread::RunnableThreadProc, pRunnable);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ThreadPool::WorkItem::WorkItem(LPTHREAD_START_ROUTINE pProc, LPVOID pParam)
{
  m_pProc = pProc;
  m_pParam = pParam;
  m_Running = FALSE;
  m_Complete = FALSE;
  m_ReturnValue = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL ThreadPool::WorkItem::Wait(DWORD Timeout)
{
  m_StateChange.Lock();
  while (!m_Complete)
  {
    BOOL Result = m_StateChange.Wait(Timeout);
    if (!Result)
    {
      m_StateChange.Unlock();
      return FALSE;
    }
  }
  m_StateChange.Unlock();
  return TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
