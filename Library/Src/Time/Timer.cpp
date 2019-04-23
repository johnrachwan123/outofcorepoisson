/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Time/Timer.cpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1701 $
 * Last Updated: $Date: 2008-11-03 07:49:59 -0800 (Mon, 03 Nov 2008) $
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

#include "Time/Timer.hpp"
#include "Debug/Trace.hpp"

using namespace Bolitho;
//------------------------------------------------------------------------------------------------------------------------------------------------------
Timer::Timer()
{
  Reset();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID Timer::Reset()
{
  m_StartTicks = 0;
  m_StopTicks = 0;
  m_ElapsedTicks = 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_POSIX)
VOID Timer::Start()
{
  struct timeval T;
  gettimeofday(&T, 0);

  m_StartTicks = (ULONGLONG)T.tv_sec;
  m_StartTicks += 111208150;
  m_StartTicks *= 1000000;
  m_StartTicks += (ULONGLONG)T.tv_usec;
  m_StartTicks *= 100;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG Timer::Stop()
{
  struct timeval T;
  gettimeofday(&T, 0);

  m_StopTicks = (ULONGLONG)T.tv_sec;
  m_StopTicks += 111208150;
  m_StopTicks *= 1000000;
  m_StopTicks += (ULONGLONG)T.tv_usec;
  m_StopTicks *= 100;

  m_ElapsedTicks += m_StopTicks-m_StartTicks;
  return m_ElapsedTicks;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG Timer::ElapsedTicks()
{
  if (m_StartTicks == 0)
    return m_ElapsedTicks;

  if (m_StopTicks == 0)
  {
    ULONGLONG CurrentTicks;
    struct timeval T;
    gettimeofday(&T, 0);

    CurrentTicks = (ULONGLONG)T.tv_sec;
    CurrentTicks += 111208150;
    CurrentTicks *= 1000000;
    CurrentTicks += (ULONGLONG)T.tv_usec;
    CurrentTicks *= 100;

    return m_ElapsedTicks + CurrentTicks-m_StartTicks;
  }
  else
    return m_ElapsedTicks;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG Timer::ElapsedNanoSeconds()
{
  ULONGLONG Ticks = ElapsedTicks();
  return Ticks;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT Timer::ElapsedSeconds()
{
  ULONGLONG Ticks = ElapsedTicks();
  return (FLOAT)Ticks / 100000000.0f;
}
#endif
//------------------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_WINDOWS)
VOID Timer::Start()
{
  QueryPerformanceCounter((LARGE_INTEGER*)&m_StartTicks);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG Timer::Stop()
{
  QueryPerformanceCounter((LARGE_INTEGER*)&m_StopTicks);
  m_ElapsedTicks += m_StopTicks-m_StartTicks;
  return m_ElapsedTicks;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG Timer::ElapsedTicks()
{
  if (m_StartTicks == 0)
    return m_ElapsedTicks;

  if (m_StopTicks == 0)
  {
    LONGLONG CurrentTicks;
    ::QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTicks);
    return m_ElapsedTicks + CurrentTicks-m_StartTicks;
  }
  else
    return m_ElapsedTicks;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG Timer::ElapsedNanoSeconds()
{
  ULONGLONG TicksPerSecond;
  QueryPerformanceFrequency((LARGE_INTEGER*)&TicksPerSecond);

  ULONGLONG Ticks = ElapsedTicks();

  return (Ticks * 1000000000) / TicksPerSecond;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT Timer::ElapsedSeconds()
{
  ULONGLONG TicksPerSecond;
  QueryPerformanceFrequency((LARGE_INTEGER*)&TicksPerSecond);

  ULONGLONG Ticks = ElapsedTicks();

  return (FLOAT)Ticks / TicksPerSecond;
}
#endif
//------------------------------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------------------------------
DebugTimer::DebugTimer(LPCTSTR Description)
{
  m_Timer.Start();
  m_Description = Description;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
DebugTimer::~DebugTimer()
{
  ULONGLONG ns = m_Timer.ElapsedNanoSeconds();
  if (ns > 1000000000L)
    Trace(TEXT("%s elapsed time: %.2fs"), m_Description.ConstStr(), ns/1000000000.0f);
  else if (ns > 1000000L)
    Trace(TEXT("%s elapsed time: %.2fms"), m_Description.ConstStr(), ns/1000000.0f);
  else if (ns > 1000L)
    Trace(TEXT("%s elapsed time: %.2fus"), m_Description.ConstStr(), ns/1000.0f);
  else
    Trace(TEXT("%s elapsed time: %.2fns"), m_Description.ConstStr(), ns/1.0f);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
FunctionTimer::FunctionTimer(LPCTSTR FunctionSignature)
{
  m_CallCount = 0;
  m_TotalTime = 0;
  m_TotalTime2 = 0;
  m_FunctionSignature = FunctionSignature;
  m_MinTime = MAXULONGLONG;
  m_MaxTime = 0;

  FunctionTimerManager::RegisterTimer(this);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
FunctionTimer::~FunctionTimer()
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID FunctionTimer::Report(ULONGLONG CompareTime)
{
  String AverageTime = String::FormatTimeInterval(m_TotalTime / m_CallCount);
  String TotalTime = String::FormatTimeInterval(m_TotalTime);
  String MinTime = String::FormatTimeInterval(m_MinTime);
  String MaxTime = String::FormatTimeInterval(m_MaxTime);

  FLOAT Percent = m_TotalTime / CompareTime * 100.0f;

  String FunctionName = Debug::Symbol::ParseFunctionName(m_FunctionSignature);

  //Trace.Lock();
  Trace("Function: %s", FunctionName.ConstStr());
  Trace.Indent();
  Trace("Call Count: %d", m_CallCount);
  Trace("Total Execution Time: %s", TotalTime.ConstStr(), Percent);
  Trace("Average Execution Time: %s", AverageTime.ConstStr());
  Trace("Maximum Execution Time: %s", MaxTime.ConstStr());
  Trace("Minimum Execution Time: %s", MinTime.ConstStr());
  Trace.UnIndent();
  //Trace.Unlock();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID FunctionTimer::AccumulateCall(ULONGLONG Time)
{
  m_CallCount++;
  m_TotalTime += Time;
  m_TotalTime2 += Time*Time;
  if (Time < m_MinTime)
    m_MinTime = Time;
  if (Time > m_MaxTime)
    m_MaxTime = Time;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
template<>
BOOL Traits<FunctionTimer>::CompareLessThan(CONST FunctionTimer& A, CONST FunctionTimer& B)
{
  return A.m_TotalTime < B.m_TotalTime;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Array<FunctionTimer*, PointerTraits<FunctionTimer> > FunctionTimerManager::m_Timers;
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID FunctionTimerManager::ReportTimings()
{
  Trace("Function Times:");
  Trace.Indent();

  ULONGLONG CompareTime = 0;
  for (SIZE_T i=0; i<m_Timers.Length(); i++)
    CompareTime += m_Timers[i]->m_TotalTime;

  m_Timers.Sort();
  for (SIZE_T i=0; i<m_Timers.Length(); i++)
    m_Timers[i]->Report(CompareTime);

  Trace.UnIndent();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------

