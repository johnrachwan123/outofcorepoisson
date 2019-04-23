/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/Process.cpp $
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

#include "System/Process.hpp"

using namespace Bolitho;
using namespace Bolitho::System;

//----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::System,Process,Object);
//----------------------------------------------------------------------------------------------------------------------------------------------------
Process::Process(PROCESSHANDLE hProcess)
{
  m_hProcess = hProcess;
  m_pMainThread = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_WINDOWS)
Process::~Process()
{
  if (m_hProcess != INVALID_HANDLE_VALUE)
    CloseHandle(m_hProcess);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD Process::WaitForExit() CONST
{
  WaitForSingleObject(m_hProcess, INFINITE);
  return GetExitCode();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Process::HasExited() CONST
{
  return WaitForSingleObject(m_hProcess, 0) == WAIT_TIMEOUT;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD Process::GetExitCode() CONST
{
  DWORD ExitCode;
  GetExitCodeProcess(m_hProcess, &ExitCode);
  return ExitCode;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Process> Process::Attach(PROCESSHANDLE hProcess)
{
  return new Process(hProcess);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Process> Process::Open(DWORD ProcessId, DWORD Access)
{
	PROCESSHANDLE hProcess = OpenProcess(Access, FALSE, ProcessId);
	if (hProcess == NULL)
		SystemException::ThrowError();
	return Attach(hProcess);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Array<VirtualMemorySpan> Process::GetMemoryInfo()
{
  Array<VirtualMemorySpan> Spans;
  MEMORY_BASIC_INFORMATION MemoryInfo;
  PVOID pAddress = 0;
  TCHAR MappingName[1024];
  while (VirtualQueryEx(m_hProcess, pAddress, &MemoryInfo, sizeof(MemoryInfo)))
    {
      VirtualMemorySpan S;
      S.Offset = MemoryInfo.BaseAddress;
      S.Length = MemoryInfo.RegionSize;
      S.Access = MemoryInfo.Protect;
      S.State = MemoryInfo.State;
      S.Type = MemoryInfo.Type;
      
      ZeroMemory(MappingName, 1024*sizeof(TCHAR));
      if (GetMappedFileName(m_hProcess, pAddress, MappingName, 1024))
	S.Name = MappingName;
      
      Spans.Add(S);
      
      pAddress = (BYTE*)pAddress + MemoryInfo.RegionSize;
    }
  
  return Spans;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Process> Process::GetCurrentProcess()
{
  return Attach(::GetCurrentProcess());
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Process> Process::Create(LPCTSTR Application)
{
  return Process::Create(Application, 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Process> Process::Create(LPCTSTR Application, LPTSTR CommandLine)
{
  return Process::Create(Application, CommandLine, (HANDLE)0, (HANDLE)0, (HANDLE)0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Process> Process::Create(LPCTSTR Application, LPTSTR CommandLine, IO::Pipe* pStdIn, IO::Pipe* pStdOut, IO::Pipe* pStdErr)
{
  return Process::Create(Application, CommandLine, pStdIn ? pStdIn->Handle() : 0, pStdOut ? pStdOut->Handle() : 0, pStdErr ? pStdErr->Handle() : 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Process> Process::Create(LPCTSTR Application, LPTSTR CommandLine, IO::File* pStdIn, IO::File* pStdOut, IO::File* pStdErr)
{
  return Process::Create(Application, CommandLine, pStdIn ? pStdIn->Handle() : 0, pStdOut ? pStdOut->Handle() : 0, pStdErr ? pStdErr->Handle() : 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Process> Process::Create(LPCTSTR Application, LPTSTR CommandLine, HANDLE hStdIn, HANDLE hStdOut, HANDLE hStdErr)
{
  PROCESS_INFORMATION ProcessInformation;
  STARTUPINFO StartupInfo;

  ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
  ZeroMemory(&ProcessInformation, sizeof(PROCESS_INFORMATION));

  StartupInfo.hStdInput = hStdIn;
  StartupInfo.hStdOutput = hStdOut;
  StartupInfo.hStdError = hStdErr;

  if (!CreateProcess(Application, CommandLine, 0, 0, FALSE, 0, 0, 0, &StartupInfo, &ProcessInformation))
    SystemException::ThrowError();

  Ptr<Process> pProcess = Process::Attach(ProcessInformation.hProcess);
  pProcess->m_pMainThread = Thread::Attach(ProcessInformation.hThread);

  return pProcess;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Process::PROCESSCOUNTERS Process::GetProcessCounters()
{
  PROCESSCOUNTERS Info;
  ZeroMemory(&Info, sizeof(Info));

  #if PLATFORM_WINDOWS < PLATFORM_WINDOWS_XP
  return Info;  
  #else

  IO_COUNTERS IoCounters;
  PROCESS_MEMORY_COUNTERS_EX MemoryCounters;
  FILETIME CreationTime;
  FILETIME ExitTime;
  FILETIME KernelTime;
  FILETIME UserTime;

  ZeroMemory(&MemoryCounters, sizeof(PROCESS_MEMORY_COUNTERS_EX));
  ZeroMemory(&IoCounters, sizeof(IO_COUNTERS));

  MemoryCounters.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);
  
  GetProcessTimes(m_hProcess, &CreationTime, &ExitTime, &KernelTime, &UserTime);
  GetProcessIoCounters(m_hProcess, &IoCounters);
  GetProcessMemoryInfo(m_hProcess, (PPROCESS_MEMORY_COUNTERS)&MemoryCounters, sizeof(PROCESS_MEMORY_COUNTERS_EX));

  Info.CreationTime = DateTime(CreationTime);
  Info.ExitTime = DateTime(ExitTime);
  Info.WallClockTime = Info.ExitTime.Ticks() == 0 ? DateTime::NowUTC() - Info.CreationTime : Info.ExitTime - Info.CreationTime;
	
  Info.KernelTime = TimeSpan(KernelTime);
  Info.UserTime = TimeSpan(UserTime);
  
	Info.LostTime = Info.WallClockTime - Info.KernelTime - Info.UserTime;
	if (Info.LostTime.Ticks() < 0)
		Info.LostTime = TimeSpan(0);
	
	Info.TotalReadOperations = IoCounters.ReadOperationCount;
  Info.TotalWriteOperations = IoCounters.WriteOperationCount;
  Info.TotalBytesRead = IoCounters.ReadTransferCount;
  Info.TotalBytesWritten = IoCounters.WriteTransferCount;

  Info.PageFaultCount = MemoryCounters.PageFaultCount;
  Info.PeakWorkingSetSize = MemoryCounters.PeakWorkingSetSize;
  Info.WorkingSetSize = MemoryCounters.WorkingSetSize;
  Info.PeakPagedPoolUsage = MemoryCounters.QuotaPeakPagedPoolUsage;
  Info.PagedPoolUsage = MemoryCounters.QuotaPagedPoolUsage;
  Info.PeakNonPagedPoolUsage = MemoryCounters.QuotaPeakNonPagedPoolUsage;
  Info.NonPagedPoolUsage = MemoryCounters.QuotaNonPagedPoolUsage;
  Info.PagefileUsage = MemoryCounters.PagefileUsage;
  Info.PeakPagefileUsage = MemoryCounters.PeakPagefileUsage;
  Info.PrivateUsage = MemoryCounters.PrivateUsage;

  return Info;
  #endif
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
INT Process::ProcessId()
{
  #if PLATFORM_WINDOWS >= PLATFORM_WINDOWS_XP
  return GetProcessId(m_hProcess);
  #else
  return 0;
  #endif
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
INT Process::GetCurrentProcessId()
{
  return (INT)::GetCurrentProcessId();
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_POSIX)
Process::~Process()
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD Process::WaitForExit() CONST
{
  throw NotImplementedException(this);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Process::HasExited() CONST
{
  throw NotImplementedException(this);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD Process::GetExitCode() CONST
{
  throw NotImplementedException(this);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Process> Process::Attach(PROCESSHANDLE hProcess)
{
  return new Process(hProcess);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Process> Process::Create(LPCTSTR Application)
{
  throw NotImplementedException(0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Process> Process::Create(LPCTSTR Application, LPTSTR CommandLine)
{
  throw NotImplementedException(0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Process> Process::Create(LPCTSTR Application, LPTSTR CommandLine, IO::Pipe* pStdIn, IO::Pipe* pStdOut, IO::Pipe* pStdErr)
{
  throw NotImplementedException(0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Process> Process::Create(LPCTSTR Application, LPTSTR CommandLine, IO::File* pStdIn, IO::File* pStdOut, IO::File* pStdErr)
{
  throw NotImplementedException(0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Process::PROCESSCOUNTERS Process::GetProcessCounters()
{
  int pid;
  char comm[1000];
  char state;
  int ppid;
  int pgrp;
  int session;
  int tty_nr;
  int tpgid;
  unsigned long flags;
  unsigned long minflt;
  unsigned long cminflt;
  unsigned long majflt;
  unsigned long cmajflt;
  unsigned long utime;
  unsigned long stime;
  long cutime;
  long cstime;
  long priority;
  long nice;
  int reserved;
  long itrealvalue;
  unsigned long starttime;
  unsigned long vsize;
  long rss;
  unsigned long rlim;
  unsigned long startcode;
  unsigned long endcode;
  unsigned long startstack;
  unsigned long kstkesp;
  unsigned long kstkeip;
  unsigned long signal;
  unsigned long blocked;
  unsigned long sigignore;
  unsigned long sigcatch;
  unsigned long wchan;
  unsigned long nswap;
  unsigned long cnswap;
  int exit_signal;
  int processor;

  static time_t BootTime = 0;
  if (BootTime == 0)
  {
    unsigned long UpTime;
    FILE* f = fopen("/proc/uptime", "rb");
    fscanf(f,"%lu",&UpTime);
    fclose(f);
    BootTime = time(0) - UpTime;
  }

  FILE* f = fopen(String::Format("/proc/%d/stat", getpid()), "rb");
  fscanf(f, "%d %s %c %d %d %d %d %d %lu %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %d %ld %llu %lu %ld %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %d %d", 
    &pid, &comm[0], &state, &ppid, &pgrp, &session, &tty_nr, &tpgid, &flags, &minflt, &cminflt, &majflt, &cmajflt, &utime, &stime, &cutime, &cstime, &priority, &nice, &reserved, &itrealvalue, &starttime, &vsize, &rss, &rlim, &startcode, &endcode, &startstack, &kstkesp, &kstkeip, &signal, &blocked, &sigignore, &sigcatch, &wchan,  &nswap, &cnswap, &exit_signal, &processor);
  fclose(f);

  PROCESSCOUNTERS C;
  ZeroMemory(&C, sizeof(C));
   
  C.CreationTime = DateTime(BootTime + (time_t)(starttime / 100));
  C.KernelTime = TimeSpan(stime * DateTime::TICKSPERSECOND / 100);
  C.UserTime = TimeSpan(utime * DateTime::TICKSPERSECOND / 100);
  C.WallClockTime = DateTime::Now() - C.CreationTime;
  C.LostTime = C.WallClockTime - C.KernelTime - C.UserTime;
  if (C.LostTime.Ticks() < 0)
    C.LostTime = TimeSpan(0);

  C.PageFaultCount = majflt;
  C.WorkingSetSize = rss * 4096;
  C.PagefileUsage = nswap * 4096;

  return C;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
INT Process::ProcessId()
{
  return m_hProcess;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Process> Process::GetCurrentProcess()
{
  return Attach(getpid());
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
INT Process::GetCurrentProcessId()
{
  return getpid();
} 
//----------------------------------------------------------------------------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------



