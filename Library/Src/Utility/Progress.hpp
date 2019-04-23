/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Utility/Progress.hpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1650 $
 * Last Updated: $Date: 2008-10-29 06:31:23 -0700 (Wed, 29 Oct 2008) $
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

#include "Time/Timer.hpp"
#include "Collections/Stack.hpp"

namespace Bolitho
{
  class ProgressInformation
  {
  public:
    ProgressInformation()
    {
      CurrentValue = 0;
      StartValue = 0;
      EndValue = 0;
      ProcessingRateValid = FALSE;
      ProcessingRate = 0;
      ElapsedSeconds = 0;
      RemainingSeconds = 0;
      RemainingSecondsValid = FALSE;
      PercentComplete = 0;
    }

    ProgressInformation(CONST ProgressInformation& I)
    {
      CurrentValue = I.CurrentValue;
      StartValue = I.StartValue;
      EndValue = I.EndValue;
      TaskName = I.TaskName;
      Message = I.Message;
      ValueUnits = I.ValueUnits;
      TaskTimer = I.TaskTimer;
      ProcessingRateValid = I.ProcessingRateValid;
      ProcessingRate = I.ProcessingRate;
      ElapsedSeconds = I.ElapsedSeconds;
    }

    ULONGLONG CurrentValue;
    ULONGLONG StartValue;
    ULONGLONG EndValue;
    String TaskName;
    String Message;
    String ValueUnits;
    Timer TaskTimer;

    FLOAT ElapsedSeconds;
    FLOAT RemainingSeconds;
    BOOL RemainingSecondsValid;

    FLOAT PercentComplete;

    FLOAT ProcessingRate;
    BOOL ProcessingRateValid;
    String ProcessingRateUnits;
  };

  class LIBRARY_API ProgressDisplay : public Object
  {
  public:
    virtual VOID Begin(ProgressInformation& I) = 0;
    virtual VOID Update(ProgressInformation& I) = 0;
    virtual VOID End(ProgressInformation& I) = 0;
  };

  class LIBRARY_API Progress : public Object
  {
  public:
    Progress();

    VOID Begin(LPCTSTR TaskName, ULONGLONG StartValue, ULONGLONG EndValue, LPCTSTR Units);
    VOID Begin(LPCTSTR TaskName, ULONGLONG StartValue, ULONGLONG EndValue);
    VOID Begin(LPCTSTR TaskName);

    VOID End();

    VOID Update(ULONGLONG Value);
    VOID Update(LPCTSTR Message);

    VOID SetDisplay(Ptr<ProgressDisplay> pDisplay);

  protected:
    Stack<ProgressInformation> m_Tasks;
    Ptr<ProgressDisplay> m_pDisplay;
  };

  class LIBRARY_API ConsoleProgressDisplay : public ProgressDisplay
  {
  public:
    virtual VOID Begin(ProgressInformation& I);
    virtual VOID Update(ProgressInformation& I);
    virtual VOID End(ProgressInformation& I);
  private:
    VOID Draw(ProgressInformation& I);
  };

  class LIBRARY_API TaskSummaryDisplay : public ProgressDisplay
  {
  public:
    virtual VOID Begin(ProgressInformation& I);
    virtual VOID Update(ProgressInformation& I);
    virtual VOID End(ProgressInformation& I);
  private:
    VOID Draw(ProgressInformation& I);
  };

  /*
  class LIBRARY_API IProgressEvents : public Object
  {
  public:
    virtual VOID OnBeginTask(LPCTSTR TaskName) = 0;
    virtual VOID OnEndTask(FLOAT ElapsedTime) = 0;
    virtual VOID OnMessage(LPCTSTR Message) = 0;
    virtual VOID OnProgress(FLOAT PercentComplete, FLOAT ElapsedSeconds, FLOAT RemainingSeconds) = 0;
  };

  class LIBRARY_API ProgressDisplay : public Object
  {
  public:
    virtual VOID Show(LPCTSTR RootTaskName) = 0;
    virtual VOID Hide() = 0;
    
    virtual VOID SetTaskName(LPCTSTR TaskName) = 0;
    virtual VOID SetIsTaskMeasured(BOOL MeasuredTask) = 0;

    virtual VOID BeginTask() = 0;
    virtual VOID EndTask(FLOAT ElapsedSeconds, BOOL ProcessingRateValid, FLOAT ProcessingRate, LPCTSTR Units) = 0;

    virtual VOID Update(FLOAT PercentComplete, FLOAT ElapsedSeconds, BOOL RemainingEstimateValid, FLOAT RemainingSeconds, BOOL ProcessingRateValid, FLOAT ProcessingRate, LPCTSTR ProcessingRateUnits) = 0;
    virtual VOID Update(LPCTSTR Message, FLOAT ElapsedSeconds) = 0;
  };

  class LIBRARY_API Progress
  {
  public:
    STATIC VOID Begin(LPCTSTR TaskName);
    STATIC VOID End();
    
    STATIC VOID Initialize(ULONGLONG StartValue, ULONGLONG EndValue, LPCTSTR Units);
    STATIC VOID Initialize(ULONGLONG StartValue, ULONGLONG EndValue);
    STATIC VOID Initialize();

    STATIC VOID Update(ULONGLONG Value);
    STATIC VOID Update(LPCTSTR Format, ...);
    
    STATIC VOID SetDisplay(Ptr<ProgressDisplay> pDisplay);
    
    STATIC VOID Enable();
    STATIC VOID Disable();
  private:
    class ThreadProgress
    {
      friend class Progress;
    public:
      ThreadProgress(ProgressDisplay* pDisplay);

      VOID Begin(LPCTSTR TaskName);
      VOID End();
      
      VOID Initialize(ULONGLONG StartValue, ULONGLONG EndValue, LPCTSTR Units);
      VOID Initialize(ULONGLONG StartValue, ULONGLONG EndValue);
      VOID Initialize();
      
      VOID Update(ULONGLONG Value);
      VOID Update(LPCTSTR Message);
      
      VOID Disable();
      VOID Enable();

      VOID Restore();
    protected:
      ULONGLONG m_Value;
      ULONGLONG m_StartValue;
      ULONGLONG m_EndValue;
      
      String m_Name;
      String m_Message;
      
      BOOL m_Enabled;

      String m_ValueUnits;

      Timer m_Timer;

      ProgressDisplay* m_pDisplay;
      ThreadProgress* m_pParentTask;
    };

    STATIC ThreadProgress* GetThreadObject();
    STATIC ThreadProgress* CreateThreadObject();

    STATIC DWORD m_hThreadInfo;
    STATIC DWORD m_hEventSink;
  };

  class LIBRARY_API ConsoleProgressDisplay : public ProgressDisplay
  {
    DeclareRuntimeType();

  public:
    virtual VOID Show(LPCTSTR RootTaskName);
    virtual VOID Hide();
    
    virtual VOID SetTaskName(LPCTSTR TaskName);
    virtual VOID SetIsTaskMeasured(BOOL MeasuredTask);

    virtual VOID BeginTask();
    virtual VOID EndTask(FLOAT ElapsedSeconds, BOOL ProcessingRateValid, FLOAT ProcessingRate, LPCTSTR Units);

    virtual VOID Update(FLOAT PercentComplete, FLOAT ElapsedSeconds, BOOL RemainingEstimateValid, FLOAT RemainingSeconds, BOOL ProcessingRateValid, FLOAT ProcessingRate, LPCTSTR ProcessingRateUnits);
    virtual VOID Update(LPCTSTR Message, FLOAT ElapsedSeconds);

  private:
    VOID Draw();

    String m_TaskName;
    String m_Message;

    FLOAT m_PercentComplete;
    FLOAT m_ElapsedSeconds;

    FLOAT m_RemainingSeconds;
    BOOL m_RemainingSecondsValid;

    FLOAT m_ProcessingRate;
    String m_ProcessingRateUnits;
    BOOL m_ProcessingRateValid;

    BOOL m_IsTaskMeasured;

  };

  class LIBRARY_API TaskSummaryDisplay : public ProgressDisplay
  {
    DeclareRuntimeType();

  public:
    virtual VOID Show(LPCTSTR RootTaskName);
    virtual VOID Hide();
    
    virtual VOID SetTaskName(LPCTSTR TaskName);
    virtual VOID SetIsTaskMeasured(BOOL MeasuredTask);

    virtual VOID BeginTask();
    virtual VOID EndTask(FLOAT ElapsedSeconds, BOOL ProcessingRateValid, FLOAT ProcessingRate, LPCTSTR Units);

    virtual VOID Update(FLOAT PercentComplete, FLOAT ElapsedSeconds, BOOL RemainingEstimateValid, FLOAT RemainingSeconds, BOOL ProcessingRateValid, FLOAT ProcessingRate, LPCTSTR ProcessingRateUnits);
    virtual VOID Update(LPCTSTR Message, FLOAT ElapsedSeconds);

  private:
    VOID Draw();

    String m_TaskName;
    String m_Message;

    FLOAT m_PercentComplete;
    FLOAT m_ElapsedSeconds;

    FLOAT m_RemainingSeconds;
    BOOL m_RemainingSecondsValid;

    FLOAT m_ProcessingRate;
    String m_ProcessingRateUnits;
    BOOL m_ProcessingRateValid;

    BOOL m_IsTaskMeasured;

  };

  class LIBRARY_API ConsoleProgress : public IProgressEvents
  {
    DeclareRuntimeType();
  public:
    virtual VOID OnBeginTask(LPCTSTR TaskName);
    virtual VOID OnEndTask(LPCTSTR TaskName, FLOAT ElapsedSeconds);
    virtual VOID OnMessage(LPCTSTR StepInfo);
    virtual VOID OnProgress(FLOAT PercentComplete, FLOAT ElapsedSeconds, FLOAT RemainingSeconds);
  };
*/
}
