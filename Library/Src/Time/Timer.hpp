/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Time/Timer.hpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1588 $
 * Last Updated: $Date: 2008-08-23 09:36:02 -0700 (Sat, 23 Aug 2008) $
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

namespace Bolitho
{
  class LIBRARY_API Timer
  {
  public:
    /* Constructs a new timer */
    Timer();

    /* Resets the timer to zero */
    VOID Reset();
    /* Begins timing */
    VOID Start();
    /* Stops timing and returns the number of elapsed ticks */
    ULONGLONG Stop();

    /* Returns the number of elapsed ticks */
    ULONGLONG ElapsedTicks();
    /* Returns the number of elapsed nano-seconds */
    ULONGLONG ElapsedNanoSeconds();
    /* Returns the number of elapsed seconds */
    FLOAT ElapsedSeconds();

  private:
    ULONGLONG m_StartTicks;
    ULONGLONG m_StopTicks;
    ULONGLONG m_ElapsedTicks;
  };

  class LIBRARY_API DebugTimer
  {
  public:
    DebugTimer(LPCTSTR Description);
    ~DebugTimer();

  private:
    String m_Description;
    Timer m_Timer;
  };

  class LIBRARY_API FunctionTimer
  {
  public:
    FunctionTimer(LPCTSTR FunctionSignature);
    ~FunctionTimer();

    VOID Report(ULONGLONG CompareTime);
    VOID AccumulateCall(ULONGLONG Time);
  
    String m_FunctionSignature;
    
    ULONGLONG m_TotalTime;
    ULONGLONG m_TotalTime2;
    ULONGLONG m_MinTime;
    ULONGLONG m_MaxTime;

    ULONGLONG m_CallCount;
  };

  class LIBRARY_API FunctionTimerManager
  {
  private:
    STATIC Array<FunctionTimer*, PointerTraits<FunctionTimer> > m_Timers;

  public:
    STATIC VOID RegisterTimer(FunctionTimer* pTimer)
    { m_Timers.Add(pTimer); }
    
    STATIC VOID ReportTimings();
  };


  class LIBRARY_API FunctionTimerAccumulator
  {
  public:
    FunctionTimerAccumulator(FunctionTimer& F) : m_Func(F)
    { m_Timer.Start(); }
    ~FunctionTimerAccumulator()
    { m_Timer.Stop(); m_Func.AccumulateCall(m_Timer.ElapsedNanoSeconds()); }
  private:
    FunctionTimer& m_Func;
    Timer m_Timer;
  };

  #if defined(COMPILER_MSVC)
  #define TimeFunction() Bolitho::DebugTimer tmp##__COUNTER__(__FUNCSIG__);
  #define TimeAllFunctionCalls() static Bolitho::FunctionTimer ft##__COUNTER__(__FUNCSIG__); Bolitho::FunctionTimerAccumulator fta##__COUNTER__(ft##__COUNTER__);
  #define TimeFunctionCall(x) { Bolitho::DebugTimer tmp##__COUNTER__(#x); x; }
  #endif

  #if defined(COMPILER_GCC)
  #define TimeFunction() Bolitho::DebugTimer tmp##__COUNTER__(__PRETTY_FUNCTION__);
  #define TimeAllFunctionCalls() static Bolitho::FunctionTimer ft##__COUNTER__(__PRETTY_FUNCTION__); Bolitho::FunctionTimerAccumulator fta##__COUNTER__(ft##__COUNTER__);
  #define TimeFunctionCall(x) { Bolitho::DebugTimer tmp##__COUNTER__(#x); x; }
  #endif
}

