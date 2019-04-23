/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/Threading/Thread.cpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1700 $
 * Last Updated: $Date: 2008-10-31 07:12:32 -0700 (Fri, 31 Oct 2008) $
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

#include "System/Threading/Thread.hpp"
#include "IO/Path.hpp"
#include "IO/StdIO.hpp"
#include "Text/StringBuilder.hpp"
#include "Debug/Trace.hpp"

using namespace Bolitho;
using namespace Bolitho::System;
using namespace Bolitho::IO;

//----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::System,Thread,Object);
//----------------------------------------------------------------------------------------------------------------------------------------------------
Map<THREADHANDLE, Thread*> Thread::m_ThreadMap;
//----------------------------------------------------------------------------------------------------------------------------------------------------
Thread::Thread(THREADHANDLE hThread)
{
  m_hThread = hThread;
  m_ThreadMap.Add(m_hThread, this);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Thread> Thread::Create(Runnable *pRunnable)
{
  return Thread::Create(Thread::RunnableThreadProc, pRunnable);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD Thread::ThreadProc(LPVOID p)
{
  THREADSTART* pStartInfo = (THREADSTART*)p;
  Assert(pStartInfo != 0);
  
  try
  {
    return pStartInfo->Proc(pStartInfo->Ptr);
  }
  catch(Exception& E)
  {
    StringBuilder Message;

    Message.AppendLine(TEXT("UNHANDLED EXCEPTION"));
    Message.AppendFormatLine(TEXT("Exception Type: %s::%s"), E.GetType()->Namespace(), E.GetType()->Name());
    Message.AppendFormatLine(TEXT("Description: %s"), E.Description().ConstStr());

    Message.AppendLine(TEXT("\nStack Trace:"));

    for(SIZE_T i=0; i<E.StackTrace().Length(); i++)
    {
#if defined(UNICODE)
      if (E.StackTrace()[i].Symbol().SourceFile() != 0)
        Message.AppendFormatLine(TEXT("0x%p %S() at %S:%d"), E.StackTrace()[i].InstructionPtr(), E.StackTrace()[i].Symbol().Name(), E.StackTrace()[i].Symbol().SourceFile(), E.StackTrace()[i].Symbol().SourceLine());
      else
        Message.AppendFormatLine(TEXT("0x%p %S()"), E.StackTrace()[i].InstructionPtr(), E.StackTrace()[i].Symbol().Name());
#else
      if (E.StackTrace()[i].Symbol().SourceFile() != 0)
        Message.AppendFormatLine(TEXT("0x%p %s()\r\n  at %s:%d"), E.StackTrace()[i].InstructionPtr(), E.StackTrace()[i].Symbol().Name(), Path::Filename(E.StackTrace()[i].Symbol().SourceFile()).ConstStr(), E.StackTrace()[i].Symbol().SourceLine());
      else
        Message.AppendFormatLine(TEXT("0x%p %s()"), E.StackTrace()[i].InstructionPtr(), E.StackTrace()[i].Symbol().Name());
#endif
    }

    Trace(TraceLevel::ERROR, pStartInfo->pThread, Message.ToString());
    delete pStartInfo;
    return 0xFFFFFFFF;
  }
  delete pStartInfo;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD Thread::RunnableThreadProc(LPVOID p)
{
  Runnable* pRunnable = (Runnable*)p;
  
  Assert(pRunnable != 0);
  
  try
  {
    if (!pRunnable->ThreadInit())
      return 0xFFFFFFFF;
    INT R = pRunnable->ThreadMain();
    pRunnable->ThreadExit();
    return R;
  }
  catch(Exception& E)
  {
    pRunnable->ThreadException(E);
    return 0xFFFFFFFF;
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Runnable::ThreadException(Exception& E)
{
  StringBuilder Message;

  Message.AppendLine(TEXT("UNHANDLED EXCEPTION"));
  Message.AppendFormatLine(TEXT("Exception Type: %s::%s"), E.GetType()->Namespace(), E.GetType()->Name());
  Message.AppendFormatLine(TEXT("Description: %s"), E.Description().ConstStr());

  Message.AppendLine(TEXT("\nStack Trace:"));
  
  for(SIZE_T i=0; i<E.StackTrace().Length(); i++)
  {
#if defined(UNICODE)
    if (E.StackTrace()[i].Symbol().SourceFile() != 0)
      Message.AppendFormatLine(TEXT("0x%p %S() at %S:%d"), E.StackTrace()[i].InstructionPtr(), E.StackTrace()[i].Symbol().Name(), E.StackTrace()[i].Symbol().SourceFile(), E.StackTrace()[i].Symbol().SourceLine());
    else
      Message.AppendFormatLine(TEXT("0x%p %S()"), E.StackTrace()[i].InstructionPtr(), E.StackTrace()[i].Symbol().Name());
#else
    if (E.StackTrace()[i].Symbol().SourceFile() != 0)
      Message.AppendFormatLine(TEXT("0x%p %s()\r\n  at %s:%d"), E.StackTrace()[i].InstructionPtr(), E.StackTrace()[i].Symbol().Name(), Path::Filename(E.StackTrace()[i].Symbol().SourceFile()).ConstStr(), E.StackTrace()[i].Symbol().SourceLine());
    else
      Message.AppendFormatLine(TEXT("0x%p %s()"), E.StackTrace()[i].InstructionPtr(), E.StackTrace()[i].Symbol().Name());
#endif
  }

	Trace(TraceLevel::ERROR, this, Message.ToString());
  Debug::GenerateCoreDump();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_WINDOWS)
Thread::~Thread()
{
  m_ThreadMap.Remove(m_hThread);
  CloseHandle(m_hThread);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Thread> Thread::Attach(THREADHANDLE hThread)
{
  Thread* pThread = 0;
  if (m_ThreadMap.Get(hThread, pThread))
    return pThread;

  return new Thread(hThread);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Thread> Thread::Create(LPTHREAD_START_ROUTINE Proc, LPVOID Ptr)
{
  Thread* pThread = new Thread(0);
  THREADSTART* pStartInfo = new THREADSTART();

  pStartInfo->Proc = Proc;
  pStartInfo->Ptr = Ptr;
  pStartInfo->pThread = pThread;

  HANDLE hThread = CreateThread(0, 0, &Thread::ThreadProc, pStartInfo, 0, 0);
  if (!hThread)
    SystemException::ThrowError();
  
  pThread->m_hThread = hThread;
  
  return pThread;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD Thread::ThreadId() CONST
{
#if PLATFORM_WINDOWS >= PLATFORM_WINDOWS_SERVER2003
  return ::GetThreadId(m_hThread);
#else
  // TODO: Fix this
  return 0;
#endif
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Thread::Terminate()
{
  TerminateThread(m_hThread, 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Thread::Sleep(ULONG t)
{
  ::Sleep(t);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Thread> Thread::GetCurrentThread()
{
  HANDLE hThread;
  DuplicateHandle(GetCurrentProcess(), ::GetCurrentThread(), GetCurrentProcess(), &hThread, GENERIC_READ | GENERIC_WRITE, FALSE, 0); 
  
  Thread* pThread = 0;
  if (m_ThreadMap.Get(hThread, pThread))
    return pThread;

  return Thread::Attach(hThread);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD Thread::GetCurrentThreadId()
{
  return ::GetCurrentThreadId();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
INT Thread::GetPriority() CONST
{
  return GetThreadPriority(m_hThread);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Thread::SetPriority(INT TP)
{
  SetThreadPriority(m_hThread, TP);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Thread::IsBlocking() CONST
{
#if PLATFORM_WINDOWS < PLATFORM_WINDOWS_XP
  return FALSE;
#else
  BOOL Pending;
  GetThreadIOPendingFlag(m_hThread, (PBOOL)&Pending);

  return Pending;
#endif
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Thread::SetThreadAffinity(DWORD_PTR ProcessorMask)
{
  SetThreadAffinityMask(m_hThread, ProcessorMask);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD_PTR Thread::GetThreadAffinity() CONST
{
  DWORD_PTR Thread = 0;
  DWORD_PTR System = 0;
  GetProcessAffinityMask(m_hThread, &Thread, &System);

  return Thread;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Thread::Wait(DWORD Timeout)
{
  return WaitForSingleObject(m_hThread, Timeout) == WAIT_OBJECT_0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD Thread::AllocateThreadLocalStorage()
{
  return TlsAlloc();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Thread::FreeThreadLocalStorage(DWORD TlsHandle)
{
  TlsFree(TlsHandle);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Thread::SetThreadLocalValue(DWORD TlsHandle, LPVOID Value)
{
  TlsSetValue(TlsHandle, Value);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LPVOID Thread::GetThreadLocalValue(DWORD TlsHandle)
{
  return TlsGetValue(TlsHandle);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Thread::THREADCOUNTERS Thread::GetThreadCounters()
{
  THREADCOUNTERS C;

  FILETIME CreationTime;
  FILETIME ExitTime;
  FILETIME KernelTime;
  FILETIME UserTime;

  GetThreadTimes(m_hThread, &CreationTime, &ExitTime, &KernelTime, &UserTime);

  C.CreationTime = DateTime(CreationTime);
  C.ExitTime = DateTime(ExitTime);
  C.KernelTime = TimeSpan(KernelTime);
  C.UserTime = TimeSpan(UserTime);

  return C;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_POSIX)
Thread::~Thread()
{
  m_ThreadMap.Remove(m_hThread);
  pthread_detach(m_hThread);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Thread> Thread::Attach(THREADHANDLE hThread)
{
  Thread* pThread = 0;
  if (m_ThreadMap.Get(hThread, pThread))
    return pThread;

  return new Thread(hThread);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
struct ThreadStartInfo
{
  LPTHREAD_START_ROUTINE Proc;
  LPVOID Ptr;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
LPVOID ThreadStartShim(LPVOID ThreadInfo)
{
  ThreadStartInfo* pTSI = (ThreadStartInfo*)ThreadInfo;
  Assert(pTSI != 0);
  Assert(pTSI->Proc != 0);
  DWORD Result = pTSI->Proc(pTSI->Ptr);
  delete pTSI;
  return (LPVOID)Result;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Thread> Thread::Create(LPTHREAD_START_ROUTINE Proc, LPVOID Ptr)
{
  THREADHANDLE hThread;
  ThreadStartInfo* pThreadStartInfo = new ThreadStartInfo();
  pThreadStartInfo->Proc = Proc;
  pThreadStartInfo->Ptr = Ptr;
  
  if (pthread_create(&hThread, 0, &ThreadStartShim, pThreadStartInfo))
    SystemException::ThrowError(0);
  return new Thread(hThread);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD Thread::ThreadId() CONST
{
  return (DWORD)m_hThread;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Thread::Terminate()
{
  //pthread_kill(m_hThread, SIGTERM); 
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Thread::Sleep(DWORD t)
{
  usleep(t);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Thread> Thread::GetCurrentThread()
{
  THREADHANDLE hThread = pthread_self();
  
  Thread* pThread = 0;
  if (m_ThreadMap.Get(hThread, pThread))
    return pThread;
  
  return Thread::Attach(hThread);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD Thread::GetCurrentThreadId()
{
  return (DWORD)pthread_self();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
INT Thread::GetPriority() CONST
{
  struct sched_param Param;
  INT Policy;
  if (pthread_getschedparam (m_hThread, &Policy, &Param))
    SystemException::ThrowError(this);
  
  return Param.sched_priority;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Thread::SetPriority(INT Priority)
{
  struct sched_param Param;
  Param.sched_priority = Priority;
  if (pthread_setschedparam(m_hThread, SCHED_OTHER, &Param))
    SystemException::ThrowError(this);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Thread::IsBlocking() CONST
{
  return FALSE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_LINUX)
VOID Thread::SetThreadAffinity(DWORD_PTR ProcessorMask)
{
  if(sched_setaffinity(0, sizeof(ProcessorMask), (CONST cpu_set_t*)&ProcessorMask) == -1)
    SystemException::ThrowError(this);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD_PTR Thread::GetThreadAffinity() CONST
{
  DWORD_PTR ProcessorMask = 0;
  if(sched_getaffinity(0, sizeof(ProcessorMask), (cpu_set_t*)&ProcessorMask) == -1)
    SystemException::ThrowError(this);
  return ProcessorMask;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
#else
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Thread::SetThreadAffinity(DWORD_PTR ProcessorMask)
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD_PTR Thread::GetThreadAffinity() CONST
{
  return 0xffffffff;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
#endif

//----------------------------------------------------------------------------------------------------------------------------------------------------
LPVOID Thread::GetThreadLocalValue(DWORD Key)
{
  return pthread_getspecific((pthread_key_t)Key);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Thread::SetThreadLocalValue(DWORD Key, LPVOID Data)
{
  pthread_setspecific((pthread_key_t)Key, Data);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD Thread::AllocateThreadLocalStorage()
{
  pthread_key_t Key;
  if (pthread_key_create(&Key, NULL))
    SystemException::ThrowError(0);
  return (DWORD)Key;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Thread::FreeThreadLocalStorage(DWORD Key)
{
  pthread_key_delete((pthread_key_t)Key);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Thread::Wait(DWORD Timeout)
{
  pthread_join(m_hThread, NULL);
  return TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

#endif
