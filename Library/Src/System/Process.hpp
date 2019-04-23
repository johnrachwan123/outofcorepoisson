/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/Process.hpp $
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

#include "IO/Pipe.hpp"
#include "IO/File.hpp"
#include "System/Threading/Thread.hpp"
#include "Memory/VirtualMemory.hpp"

namespace Bolitho
{
  namespace System
  {
    class LIBRARY_API Process : virtual public Object
    {
      DeclareRuntimeType();
    public:
      virtual ~Process();

      #if defined(PLATFORM_WINDOWS)
      /* Returns a Process object that represents an already running process */
      static Ptr<Process> Open(DWORD ProcessId, DWORD Access = PROCESS_ALL_ACCESS);
      #endif
      
      /* Returns a Process object that represents an already running process */
      static Ptr<Process> Attach(PROCESSHANDLE hProcess);
      
      /* Returns a Process object that represents a new process */ 
      static Ptr<Process> Create(LPCTSTR Application);
      static Ptr<Process> Create(LPCTSTR Application, LPTSTR CommandLine);
      static Ptr<Process> Create(LPCTSTR Application, LPTSTR CommandLine, IO::Pipe* pStdIn, IO::Pipe* pStdOut, IO::Pipe* pStdErr);
      static Ptr<Process> Create(LPCTSTR Application, LPTSTR CommandLine, IO::File* pStdIn, IO::File* pStdOut, IO::File* pStdErr);

      /* Returns a Process object that represents the calling process */
      static Ptr<Process> GetCurrentProcess();
      /* Returns a numeric identifier for the calling process */
      static INT GetCurrentProcessId();

      /* Blocks the calling thread until the process exits */
      DWORD WaitForExit() CONST;
      /* Returns TRUE if the process has exited */
      BOOL HasExited() CONST;
      /* Returns the exit code of the process */
      DWORD GetExitCode() CONST;
      /* Returns virtual memory space information for the process */
      Array<VirtualMemorySpan> GetMemoryInfo();

      /* Returns a numeric identifier for the process */
      INT ProcessId();

      /* Returns the OS handle for the process */
      PROCESSHANDLE Handle() CONST
      { return m_hProcess; }

      /* Returns a Thread object representing the main thread for the process */
      Ptr<Thread> GetMainThread()
      { return m_pMainThread; }

      /* Per-process statistics */
      struct PROCESSCOUNTERS
      {
        DateTime CreationTime;
        DateTime ExitTime;

        TimeSpan KernelTime;
        TimeSpan UserTime;
        TimeSpan WallClockTime;
				TimeSpan LostTime;

        ULONGLONG TotalReadOperations;
        ULONGLONG TotalWriteOperations;
        ULONGLONG TotalBytesRead;
        ULONGLONG TotalBytesWritten;

        DWORD PageFaultCount;
        SIZE_T PeakWorkingSetSize;
        SIZE_T WorkingSetSize;
        SIZE_T PeakPagedPoolUsage;
        SIZE_T PagedPoolUsage;
        SIZE_T PeakNonPagedPoolUsage;
        SIZE_T NonPagedPoolUsage;
        SIZE_T PagefileUsage;
        SIZE_T PeakPagefileUsage;
        SIZE_T PrivateUsage;
      };

      /* Collects and returns the current statistics for the process */
      PROCESSCOUNTERS GetProcessCounters();

    protected:
      #if defined(PLATFORM_WINDOWS)
      static Ptr<Process> Create(LPCTSTR Application, LPTSTR CommandLine, HANDLE hStdIn, HANDLE hStdOut, HANDLE hStdErr);
      #endif
      Process(PROCESSHANDLE hProcess);
      
      /* The OS handle for the process */
      PROCESSHANDLE m_hProcess;
      /* The main thread for the process */
      Ptr<Thread> m_pMainThread;
    };
  }
}

