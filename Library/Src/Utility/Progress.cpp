/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Utility/Progress.cpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1687 $
 * Last Updated: $Date: 2008-10-29 11:46:59 -0700 (Wed, 29 Oct 2008) $
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
#include "Utility/Progress.hpp"
#include "Debug/Trace.hpp"
#include "System/Console.hpp"

#include "System/Threading/Thread.hpp"

using namespace Bolitho;
using namespace Bolitho::System;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
Progress::Progress()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Progress::Begin(LPCTSTR TaskName, ULONGLONG StartValue, ULONGLONG EndValue, LPCTSTR Units)
{
  ProgressInformation& I = m_Tasks.Push();
  I.TaskName = TaskName;
  I.StartValue = StartValue;
  I.CurrentValue = StartValue;
  I.EndValue = EndValue;
  I.ValueUnits = Units;
  I.Message = String::Empty;
  I.TaskTimer.Start();

  if (m_pDisplay)
    m_pDisplay->Begin(I);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Progress::Begin(LPCTSTR TaskName, ULONGLONG StartValue, ULONGLONG EndValue)
{
  ProgressInformation& I = m_Tasks.Push();
  I.TaskName = TaskName;
  I.StartValue = StartValue;
  I.CurrentValue = StartValue;
  I.EndValue = EndValue;
  I.ValueUnits = String::Empty;
  I.Message = String::Empty;
  I.TaskTimer.Start();

  if (m_pDisplay)
    m_pDisplay->Begin(I);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Progress::Begin(LPCTSTR TaskName)
{
  ProgressInformation& I = m_Tasks.Push();
  I.TaskName = TaskName;
  I.StartValue = 0;
  I.CurrentValue = 0;
  I.EndValue = 0;
  I.ValueUnits = String::Empty;
  I.Message = String::Empty;
  I.TaskTimer.Start();

  if (m_pDisplay)
    m_pDisplay->Begin(I);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Progress::End()
{
  ProgressInformation& I = m_Tasks.Peek();
  
  I.TaskTimer.Stop();
  I.ElapsedSeconds = I.TaskTimer.ElapsedSeconds();

  I.ProcessingRate = (I.EndValue - I.StartValue) / I.ElapsedSeconds;
  String Units = I.ValueUnits;
  if (I.ProcessingRate > 1000000)
  { I.ProcessingRate /= 1000000; I.ProcessingRateUnits = String("M") + Units; }
  else if (I.ProcessingRate > 1000)
  { I.ProcessingRate /= 1000; I.ProcessingRateUnits = String("K") + Units; }
  
  I.ProcessingRateValid = (I.StartValue != I.EndValue);
  I.RemainingSecondsValid = FALSE;
  
  if (m_pDisplay)
    m_pDisplay->End(I);

  m_Tasks.Pop();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Progress::Update(ULONGLONG Value)
{
  ProgressInformation& I = m_Tasks.Peek();
  I.CurrentValue = Value;
  I.ElapsedSeconds = I.TaskTimer.ElapsedSeconds();

  I.ProcessingRate = (I.EndValue - I.StartValue) / I.ElapsedSeconds;
  String Units = I.ValueUnits;
  if (I.ProcessingRate > 1000000)
  { I.ProcessingRate /= 1000000; I.ProcessingRateUnits = String("M") + Units; }
  else if (I.ProcessingRate > 1000)
  { I.ProcessingRate /= 1000; I.ProcessingRateUnits = String("K") + Units; }
  I.PercentComplete = (FLOAT)(Value - I.StartValue) / (I.EndValue - I.StartValue);
  I.RemainingSeconds = ((I.ElapsedSeconds / I.PercentComplete) - I.ElapsedSeconds);

  if (I.PercentComplete > 0.05)
  {
    I.ProcessingRateValid = TRUE;
    I.RemainingSecondsValid = TRUE;
  }

  if (m_pDisplay)
    m_pDisplay->Update(I);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Progress::Update(LPCTSTR Message)
{
  ProgressInformation& I = m_Tasks.Peek();
  I.Message = Message;
  if (m_pDisplay)
    m_pDisplay->Update(I);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Progress::SetDisplay(Ptr<ProgressDisplay> pDisplay)
{
  m_pDisplay = pDisplay;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsoleProgressDisplay::Begin(ProgressInformation& I)
{
  Trace("%s...", I.TaskName.ConstStr());
  Trace.Indent();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsoleProgressDisplay::Update(ProgressInformation& I)
{
  Draw(I);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsoleProgressDisplay::End(ProgressInformation& I)
{
  #if defined(PLATFORM_WINDOWS)
  StdOut << String(' ', Console::GetWindowWidth() - 1) << "\r";
  #else
  StdOut << "\r";
  #endif

  Trace.UnIndent();
  if (I.ProcessingRateValid)
    Trace("Finished %s. Elapsed: %s Rate: %.2f%s/s", I.TaskName.ConstStr(), String::FormatTimeInterval(I.ElapsedSeconds).ConstStr(), I.ProcessingRate, I.ProcessingRateUnits.ConstStr());
  else
    Trace("Finished %s. Elapsed: %s", I.TaskName.ConstStr(), String::FormatTimeInterval(I.ElapsedSeconds).ConstStr());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsoleProgressDisplay::Draw(ProgressInformation& I)
{
  CONST UINT BarWidth = 25;
  if (I.EndValue > I.StartValue)
  {
    String Indent = String(' ', Trace.GetIndentLevel());

    String Bar = "[" + String('#', (SIZE_T)(I.PercentComplete * BarWidth)) + String(' ', BarWidth - (SIZE_T)(I.PercentComplete * BarWidth)) + ']';

    String Time;
    if (!I.RemainingSecondsValid)
      Time = " Elapsed: " + String::FormatTimeInterval(I.ElapsedSeconds);
    else
      Time = " Elapsed: " + String::FormatTimeInterval(I.ElapsedSeconds) + " Remaining: " + String::FormatTimeInterval(I.RemainingSeconds);
    
    String Rate;
    if (I.ProcessingRateValid)
      Rate = String::Format(" Rate: %.2f%s/s", I.ProcessingRate, I.ProcessingRateUnits.ConstStr());

    String Message;
    if (!I.Message.IsEmpty())
      Message = " (" + I.Message + ")";
    
    SIZE_T Length = Indent.Length() + Bar.Length() + Time.Length() + Rate.Length() + Message.Length();

    StdOut << Indent << Bar << Time << Rate << Message;
    #if defined(PLATFORM_WINDOWS)
    StdOut << String(' ', Console::GetWindowWidth() - Length - 1) << "\r";
    #else
    StdOut << "\r";
    #endif
  }
  else
  {
    
    
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskSummaryDisplay::Begin(ProgressInformation& I)
{
  Trace("%s...", I.TaskName.ConstStr());
  Trace.Indent();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskSummaryDisplay::Update(ProgressInformation& I)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskSummaryDisplay::End(ProgressInformation& I)
{
  #if defined(PLATFORM_WINDOWS)
  StdOut << String(' ', Console::GetWindowWidth() - 1) << "\r";
  #else
  StdOut << "\r";
  #endif

  Trace.UnIndent();
  if (I.ProcessingRateValid)
    Trace("Finished %s. Elapsed: %s Rate: %.2f%s/s", I.TaskName.ConstStr(), String::FormatTimeInterval(I.ElapsedSeconds).ConstStr(), I.ProcessingRate, I.ProcessingRateUnits.ConstStr());
  else
    Trace("Finished %s. Elapsed: %s", I.TaskName.ConstStr(), String::FormatTimeInterval(I.ElapsedSeconds).ConstStr());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------------------------------
/*
DWORD pProgress->m_hThreadInfo = 0;
DWORD pProgress->m_hEventSink = 0;
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID pProgress->Enable()
{
  ThreadProgress* pProgress = GetThreadObject();
  if (pProgress)
    pProgress->m_Enabled = TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID pProgress->Disable()
{
  ThreadProgress* pProgress = GetThreadObject();
  if (pProgress)
    pProgress->m_Enabled = FALSE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID pProgress->Begin(LPCTSTR TaskName)
{
  ThreadProgress* pProgress = CreateThreadObject();
  if (pProgress)
    pProgress->Begin(TaskName);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID pProgress->End()
{
  ThreadProgress* pProgress = GetThreadObject();
  if (!pProgress)
    return;
  
  pProgress->End();

  Thread::SetThreadLocalValue(m_hThreadInfo, pProgress->m_pParentTask);
  delete pProgress;

  pProgress = GetThreadObject();
  
  if (pProgress)
    pProgress->Restore();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID pProgress->Initialize(ULONGLONG MinValue, ULONGLONG MaxValue, LPCTSTR Units)
{
  ThreadProgress* pProgress = GetThreadObject();
  if (pProgress)
    pProgress->Initialize(MinValue, MaxValue, Units);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID pProgress->Initialize(ULONGLONG MinValue, ULONGLONG MaxValue)
{
  ThreadProgress* pProgress = GetThreadObject();
  if (pProgress)
    pProgress->Initialize(MinValue, MaxValue);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID pProgress->Initialize()
{
  ThreadProgress* pProgress = GetThreadObject();
  if (pProgress)
    pProgress->Initialize();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID pProgress->Update(ULONGLONG Value)
{
  ThreadProgress* pProgress = GetThreadObject();
  if (pProgress)
    pProgress->Update(Value);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID pProgress->Update(LPCTSTR Format, ...)
{
  va_list Args;
  va_start(Args,Format);
  String S = String::FormatV(Format, Args);
  va_end(Args);

  ThreadProgress* pProgress = GetThreadObject();
  if (pProgress)
    pProgress->Update(S);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID pProgress->SetDisplay(Ptr<ProgressDisplay> pDisplay)
{
  if (m_hEventSink == 0)
    m_hEventSink = Thread::AllocateThreadLocalStorage();
  ProgressDisplay* pOldDisplay = (ProgressDisplay*)Thread::GetThreadLocalValue(m_hEventSink);

  if (pOldDisplay)
    pOldDisplay->Release();
  
  pDisplay.AddRef();
  Thread::SetThreadLocalValue(m_hEventSink, pDisplay.Get());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
pProgress->ThreadpProgress->ThreadProgress(ProgressDisplay* pDisplay) : m_pDisplay(pDisplay)
{
  m_StartValue = 0;
  m_EndValue = 0;
  m_Value = 0;
  m_Enabled = TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID pProgress->ThreadpProgress->Restore()
{
  if (m_pDisplay == 0)
    return;

  m_pDisplay->SetTaskName(m_Name);
  m_pDisplay->SetIsTaskMeasured(m_EndValue != m_StartValue);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID pProgress->ThreadpProgress->Begin(LPCTSTR TaskName)
{
  if (!m_Enabled)
    return;

  m_Timer.Reset();
  m_Timer.Start();
  m_Name = TaskName;

  if (m_pDisplay == 0)
    return;

  if (m_pParentTask == 0)
    m_pDisplay->Show(TaskName);
  else
    m_pDisplay->SetTaskName(TaskName);

  m_pDisplay->BeginTask();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID pProgress->ThreadpProgress->End()
{
  if (!m_Enabled)
    return;

  m_Timer.Stop();
  FLOAT ElapsedSeconds = m_Timer.ElapsedSeconds();

  FLOAT Rate = (m_EndValue - m_StartValue) / ElapsedSeconds;
  String Units = m_ValueUnits;
  if (Rate > 1000000)
  { Rate /= 1000000; Units = String("M") + Units; }
  else if (Rate > 1000)
  { Rate /= 1000; Units = String("K") + Units; }

  if (m_pDisplay == 0)
    return;

  m_pDisplay->EndTask(ElapsedSeconds, !m_ValueUnits.IsEmpty(), Rate, Units);
  if (m_pParentTask == 0)
    m_pDisplay->Hide();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID pProgress->ThreadpProgress->Initialize(ULONGLONG StartValue, ULONGLONG EndValue, LPCTSTR Units)
{
  if (!m_Enabled)
    return;

  m_StartValue = StartValue;
  m_EndValue = EndValue;
  m_Value = StartValue;
  m_ValueUnits = Units;

  if (m_pDisplay == 0)
    return;

  m_pDisplay->SetIsTaskMeasured(TRUE);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID pProgress->ThreadpProgress->Initialize(ULONGLONG StartValue, ULONGLONG EndValue)
{
  if (!m_Enabled)
    return;

  m_StartValue = StartValue;
  m_EndValue = EndValue;
  m_Value = StartValue;

  if (m_pDisplay == 0)
    return;

  m_pDisplay->SetIsTaskMeasured(TRUE);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID pProgress->ThreadpProgress->Initialize()
{
  if (!m_Enabled)
    return;

  m_StartValue = 0;
  m_EndValue = 0;
  m_Value = 0;

  if (m_pDisplay == 0)
    return;

  m_pDisplay->SetIsTaskMeasured(FALSE);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID pProgress->ThreadpProgress->Update(ULONGLONG Value)
{
  if (!m_Enabled)
    return;

  m_Value = Value;
  
  FLOAT PercentComplete = (FLOAT)(m_Value - m_StartValue) / (m_EndValue - m_StartValue);

  FLOAT ElapsedTime = m_Timer.ElapsedSeconds();
  FLOAT RemainingTime = ((ElapsedTime / PercentComplete) - ElapsedTime);

  if (m_pDisplay == 0)
    return;

  if (m_ValueUnits.Length() == 0)
  {
    m_pDisplay->Update(PercentComplete, ElapsedTime, PercentComplete > 10, RemainingTime, FALSE, 0, 0);
  }
  else
  {
    FLOAT Rate = (m_Value - m_StartValue) / ElapsedTime;
    String Units = m_ValueUnits;
    if (Rate > 1000000)
    { Rate /= 1000000; Units = String("M") + Units; }
    else if (Rate > 1000)
    { Rate /= 1000; Units = String("K") + Units; }

    m_pDisplay->Update(PercentComplete, ElapsedTime, PercentComplete > 10, RemainingTime, TRUE, Rate, Units);
  }

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID pProgress->ThreadpProgress->Update(LPCTSTR Message)
{
  if (!m_Enabled)
    return;
  
  if (m_pDisplay == 0)
    return;

  m_pDisplay->Update(Message, m_Timer.ElapsedSeconds());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
pProgress->ThreadProgress* pProgress->GetThreadObject()
{
  if (m_hThreadInfo == 0)
    m_hThreadInfo = Thread::AllocateThreadLocalStorage();
  ThreadProgress* pProgress = (ThreadProgress*)Thread::GetThreadLocalValue(m_hThreadInfo);
  return pProgress;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
pProgress->ThreadProgress* pProgress->CreateThreadObject()
{
  ProgressDisplay* pDisplay = 0;
  if (m_hEventSink != 0)
    pDisplay = (ProgressDisplay*)Thread::GetThreadLocalValue(m_hEventSink);

  ThreadProgress* pProgress = GetThreadObject();
  ThreadProgress* pChildTask = new ThreadProgress(pDisplay);
  pChildTask->m_pParentTask = pProgress;

  Thread::SetThreadLocalValue(m_hThreadInfo, pChildTask);

  return pChildTask;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho,ConsoleProgressDisplay,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsoleProgressDisplay::Show(LPCTSTR RootTaskName)
{
  m_PercentComplete = 0.0f;
  m_ElapsedSeconds = 0.0f;
  m_RemainingSeconds = 0.0f;
  m_RemainingSecondsValid = FALSE;

  m_ProcessingRate = 0.0f;
  m_ProcessingRateUnits = "";
  m_ProcessingRateValid = FALSE;
  
  m_TaskName = RootTaskName;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsoleProgressDisplay::Hide()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsoleProgressDisplay::SetTaskName(LPCTSTR TaskName)
{
  m_TaskName = TaskName;

  m_Message = "";
  m_PercentComplete = 0.0f;
  m_ElapsedSeconds = 0.0f;
  m_RemainingSeconds = 0.0f;
  m_RemainingSecondsValid = FALSE;

  m_ProcessingRate = 0.0f;
  m_ProcessingRateUnits = "";
  m_ProcessingRateValid = FALSE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsoleProgressDisplay::SetIsTaskMeasured(BOOL MeasuredTask)
{
  m_IsTaskMeasured = MeasuredTask;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsoleProgressDisplay::BeginTask()
{
  Trace("%s...", m_TaskName.ConstStr());
  Trace.Indent();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsoleProgressDisplay::EndTask(FLOAT ElapsedSeconds, BOOL ProcessingRateValid, FLOAT ProcessingRate, LPCTSTR Units)
{
  #if defined(PLATFORM_WINDOWS)
  StdOut << String(' ', Console::GetWindowWidth() - 1) << "\r";
  #else
  StdOut << "\r";
  #endif

  Trace.UnIndent();
  if (ProcessingRateValid)
    Trace("Finished %s. Elapsed: %s Rate: %.2f%s/s", m_TaskName.ConstStr(), String::FormatTimeInterval(ElapsedSeconds).ConstStr(), ProcessingRate, Units);
  else
    Trace("Finished %s. Elapsed: %s", m_TaskName.ConstStr(), String::FormatTimeInterval(ElapsedSeconds).ConstStr());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsoleProgressDisplay::Update(FLOAT PercentComplete, FLOAT ElapsedSeconds, BOOL RemainingSecondsValid, FLOAT RemainingSeconds, BOOL ProcessingRateValid, FLOAT ProcessingRate, LPCTSTR ProcessingRateUnits)
{
  m_PercentComplete = PercentComplete;
  m_ElapsedSeconds = ElapsedSeconds;
  m_RemainingSeconds = RemainingSeconds;
  m_RemainingSecondsValid = RemainingSecondsValid;

  m_ProcessingRate = ProcessingRate;
  m_ProcessingRateUnits = ProcessingRateUnits;
  m_ProcessingRateValid = ProcessingRateValid;

  Draw();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsoleProgressDisplay::Update(LPCTSTR Message, FLOAT ElapsedSeconds)
{
  m_Message = Message;
  m_ElapsedSeconds = ElapsedSeconds;

  Draw();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsoleProgressDisplay::Draw()
{
  CONST UINT BarWidth = 25;
  if (m_IsTaskMeasured)
  {
    String Indent = String(' ', Trace.GetIndentLevel());

    String Bar = "[" + String('#', (SIZE_T)(m_PercentComplete * BarWidth)) + String(' ', BarWidth - (SIZE_T)(m_PercentComplete * BarWidth)) + ']';

    String Time;
    if (m_RemainingSecondsValid)
      Time = " Elapsed: " + String::FormatTimeInterval(m_ElapsedSeconds);
    else
      Time = " Elapsed: " + String::FormatTimeInterval(m_ElapsedSeconds) + " Remaining: " + String::FormatTimeInterval(m_RemainingSeconds);
    
    String Rate;
    if (m_ProcessingRateValid)
      Rate = String::Format(" Rate: %.2f%s/s", m_ProcessingRate, m_ProcessingRateUnits.ConstStr());

    String Message;
    if (!m_Message.IsEmpty())
      Message = " (" + m_Message + ")";
    
    SIZE_T Length = Indent.Length() + Bar.Length() + Time.Length() + Rate.Length() + Message.Length();

    StdOut << Indent << Bar << Time << Rate << Message;
    #if defined(PLATFORM_WINDOWS)
    StdOut << String(' ', Console::GetWindowWidth() - Length - 1) << "\r";
    #else
    StdOut << "\r";
    #endif
  }
  else
  {
    
    
  }
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho,TaskSummaryDisplay,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskSummaryDisplay::Show(LPCTSTR RootTaskName)
{
  m_PercentComplete = 0.0f;
  m_ElapsedSeconds = 0.0f;
  m_RemainingSeconds = 0.0f;
  m_RemainingSecondsValid = FALSE;

  m_ProcessingRate = 0.0f;
  m_ProcessingRateUnits = "";
  m_ProcessingRateValid = FALSE;
  
  m_TaskName = RootTaskName;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskSummaryDisplay::Hide()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskSummaryDisplay::SetTaskName(LPCTSTR TaskName)
{
  m_TaskName = TaskName;

  m_Message = "";
  m_PercentComplete = 0.0f;
  m_ElapsedSeconds = 0.0f;
  m_RemainingSeconds = 0.0f;
  m_RemainingSecondsValid = FALSE;

  m_ProcessingRate = 0.0f;
  m_ProcessingRateUnits = "";
  m_ProcessingRateValid = FALSE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskSummaryDisplay::SetIsTaskMeasured(BOOL MeasuredTask)
{
  m_IsTaskMeasured = MeasuredTask;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskSummaryDisplay::BeginTask()
{
  Trace("%s...", m_TaskName.ConstStr());
  Trace.Indent();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskSummaryDisplay::EndTask(FLOAT ElapsedSeconds, BOOL ProcessingRateValid, FLOAT ProcessingRate, LPCTSTR Units)
{
  #if defined(PLATFORM_WINDOWS)
  StdOut << String(' ', Console::GetWindowWidth() - 1) << "\r";
  #else
  StdOut << "\r";
  #endif

  Trace.UnIndent();
  if (ProcessingRateValid)
    Trace("Finished %s. Elapsed: %s Rate: %.2f%s/s", m_TaskName.ConstStr(), String::FormatTimeInterval(ElapsedSeconds).ConstStr(), ProcessingRate, Units);
  else
    Trace("Finished %s. Elapsed: %s", m_TaskName.ConstStr(), String::FormatTimeInterval(ElapsedSeconds).ConstStr());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskSummaryDisplay::Update(FLOAT PercentComplete, FLOAT ElapsedSeconds, BOOL RemainingSecondsValid, FLOAT RemainingSeconds, BOOL ProcessingRateValid, FLOAT ProcessingRate, LPCTSTR ProcessingRateUnits)
{
  m_PercentComplete = PercentComplete;
  m_ElapsedSeconds = ElapsedSeconds;
  m_RemainingSeconds = RemainingSeconds;
  m_RemainingSecondsValid = RemainingSecondsValid;

  m_ProcessingRate = ProcessingRate;
  m_ProcessingRateUnits = ProcessingRateUnits;
  m_ProcessingRateValid = ProcessingRateValid;

  //Draw();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskSummaryDisplay::Update(LPCTSTR Message, FLOAT ElapsedSeconds)
{
  m_Message = Message;
  m_ElapsedSeconds = ElapsedSeconds;

  //Draw();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskSummaryDisplay::Draw()
{
  CONST UINT BarWidth = 25;
  if (m_IsTaskMeasured)
  {
    String Indent = String(' ', Trace.GetIndentLevel());

    String Bar = "[" + String('#', (SIZE_T)(m_PercentComplete * BarWidth)) + String(' ', BarWidth - (SIZE_T)(m_PercentComplete * BarWidth)) + ']';

    String Time;
    if (m_RemainingSecondsValid)
      Time = " Elapsed: " + String::FormatTimeInterval(m_ElapsedSeconds);
    else
      Time = " Elapsed: " + String::FormatTimeInterval(m_ElapsedSeconds) + " Remaining: " + String::FormatTimeInterval(m_RemainingSeconds);
    
    String Rate;
    if (m_ProcessingRateValid)
      Rate = String::Format(" Rate: %.2f%s/s", m_ProcessingRate, m_ProcessingRateUnits.ConstStr());

    String Message;
    if (!m_Message.IsEmpty())
      Message = " (" + m_Message + ")";
    
    SIZE_T Length = Indent.Length() + Bar.Length() + Time.Length() + Rate.Length() + Message.Length();

    StdOut << Indent << Bar << Time << Rate << Message;
    #if defined(PLATFORM_WINDOWS)
    StdOut << String(' ', Console::GetWindowWidth() - Length - 1) << "\r";
    #else
    StdOut << "\r";
    #endif
  }
  else
  {
    
    
  }
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho,ConsoleProgress,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsolepProgress->OnBeginTask(LPCTSTR TaskName)
{
  Trace(TEXT("%s..."), TaskName);
  Trace.Indent();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsolepProgress->OnEndTask(LPCTSTR TaskName, FLOAT ElapsedSeconds)
{
  if (!StdOut.IsRedirected())
    StdOut << String(' ', 79) << "\r";

  Trace.UnIndent();
  Trace(TEXT("Finished %s. Total Time: %s"), TaskName, String::FormatTimeInterval(ElapsedSeconds).ConstStr());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsolepProgress->OnMessage(LPCTSTR StepInfo)
{
  Trace("%s", StepInfo);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsolepProgress->OnProgress(FLOAT PercentComplete, FLOAT ElapsedSeconds, FLOAT RemainingSeconds)
{
  CONST INT ProgressBarLength = 30;
  INT DoneBars = (INT)(PercentComplete * ProgressBarLength);
  INT TodoBars = ProgressBarLength - DoneBars;

  if (!StdOut.IsRedirected())
  {
    if (ElapsedSeconds > 10)
      StdOut << " [" << String('#', DoneBars) << String(' ', TodoBars) << "]  Elapsed: " << String::FormatTimeInterval(ElapsedSeconds) << "  Remaining: " << String::FormatTimeInterval(RemainingSeconds) << " \r";
    else
      StdOut << " [" << String('#', DoneBars) << String(' ', TodoBars) << "]  Elapsed: " << String::FormatTimeInterval(ElapsedSeconds) << " \r";
  }
  else
  {
    /*
    if (m_CurrentValue > m_NextMilestone)
    {
      Trace(TEXT("%d%% complete"), (INT)(PercentComplete*100.0f));
      m_NextMilestone += (m_MaximumValue / 20);
    }
    ///
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
*/