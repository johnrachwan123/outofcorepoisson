/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/WindowsApplication.cpp $
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

#include "System/WindowsApplication.hpp"
#include "IO/StdIO.hpp"
#include "IO/DebugStream.hpp"
#include "Text/StringBuilder.hpp"
#include "UI/TaskDialog.hpp"

using namespace Bolitho;
using namespace Bolitho::UI;
using namespace Bolitho::IO;
using namespace Bolitho::System;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::System,WindowsApplication,Application);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
WindowsApplication::WindowsApplication(BOOL EnableIdleProcessing)
{
  m_MessageLoopRunning = FALSE;
  m_EnableIdleProcessing = EnableIdleProcessing;
  m_pAccel = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
WindowsApplication::~WindowsApplication()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL WindowsApplication::PumpMessage()
{
  MSG Message;
  BOOL Return;
  Return = GetMessage(&Message, NULL, 0, 0);

  if (Return == 0)
    return FALSE;

  if (Return == -1)
    Win32Exception::ThrowError(this);
  else
  {
    if (m_pAccel)
    {
      if (!TranslateAccelerator(Message.hwnd, m_pAccel->Handle(), &Message))
      {
        TranslateMessage(&Message); 
        DispatchMessage(&Message); 
      }
    }
    else
    {
      TranslateMessage(&Message);
      DispatchMessage(&Message);
    }
  }

  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
INT WindowsApplication::Main()
{
  m_MessageLoopRunning = TRUE;

  if (m_EnableIdleProcessing)
  {
    MSG Message;
    BOOL Running = TRUE;
    
    do
    {
      if (PeekMessage(&Message, NULL, 0, 0, PM_NOREMOVE))
        Running = PumpMessage();
      OnIdle();
    }
    while (Running);
  }
  else
  {
    while(PumpMessage());
  }
  return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL WindowsApplication::OnInit()
{
  if (!Application::OnInit())
    return FALSE;

  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID WindowsApplication::OnExit()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID WindowsApplication::OnIdle()
{
  Thread::Sleep();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID WindowsApplication::OnException(Exception& E)
{
  #if PLATFORM_WINDOWS >= PLATFORM_WINDOWS_VISTA
  Ptr<Module> pModule = Module::GetProcessModule();
  TaskDialog Dlg;

  Dlg.SetTitle(m_ProductName);
  Dlg.SetBodyTitle("Unhandled Exception");
  Dlg.SetBody(String::Format(TEXT("An unhandled %s has occurred in %s\r\n%s"), E.GetType()->Name(), pModule ? pModule->ImageName().ConstStr() : TEXT("UnknownModule"), E.Description().ConstStr() ));

  StringBuilder StackTrace;
  for(SIZE_T i=0; i<E.StackTrace().Length(); i++)
  {
    Ptr<Module> pFrameModule = Module::GetModuleFromAddress(E.StackTrace()[i].InstructionPtr());
    #if defined(UNICODE)
    if (E.StackTrace()[i].Symbol().SourceFile() != 0)
      StackTrace.AppendFormat(TEXT("%s 0x%p %S() at %S:%d\r\n"), pFrameModule ? Path::FileName(pFrameModule->ImageName()).ConstStr() : TEXT("Unknown"), E.StackTrace()[i].InstructionPtr(), E.StackTrace()[i].Symbol().Name(), E.StackTrace()[i].Symbol().SourceFile(), E.StackTrace()[i].Symbol().SourceLine());
    else
      StackTrace.AppendFormat(TEXT("%s 0x%p %S()\r\n"), pFrameModule ? Path::FileName(pFrameModule->ImageName()).ConstStr() : TEXT("Unknown"), E.StackTrace()[i].InstructionPtr(), E.StackTrace()[i].Symbol().Name());
    #else
    if (E.StackTrace()[i].Symbol().SourceFile() != 0)
      StackTrace.AppendFormat(TEXT("0x%p %s() at %s:%d\r\n"), E.StackTrace()[i].InstructionPtr(), E.StackTrace()[i].Symbol().Name(), E.StackTrace()[i].Symbol().SourceFile(), E.StackTrace()[i].Symbol().SourceLine());
    else
      StackTrace.AppendFormat(TEXT("0x%p %s()\r\n"), E.StackTrace()[i].InstructionPtr(), E.StackTrace()[i].Symbol().Name());
    #endif
  }

  Dlg.SetExpandedBody(StackTrace.ToString(), "Hide Stack Trace", "Show Stack Trace");
  Dlg.SetWidth(450);
  
  Dlg.AddButton(2, "Mini Dump", FALSE);
  Dlg.AddButton(3, "Full Dump", FALSE);
  Dlg.AddButton(1, "Debug", FALSE);
  Dlg.AddButton(0, "Close", TRUE);

  Dlg.SetIcon(Icon::ERROR);
  
  INT Result = Dlg.ShowModal();
  switch (Result)
  {
  case 2:
    Debug::GenerateCoreDump();
    break;
  case 3:
    Debug::GenerateCoreDump(0, 0, MiniDumpWithFullMemory);
    break;
  };
  Application::Exit(-1);
#else
  Application::OnException(E);
#endif

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID WindowsApplication::Exit(INT ExitCode)
{
  PostQuitMessage(ExitCode);
}
