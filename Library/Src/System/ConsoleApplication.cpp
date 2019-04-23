/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/ConsoleApplication.cpp $
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

#include "System/ConsoleApplication.hpp"
#include "System/Console.hpp"

#include "IO/StdIO.hpp"
#include "IO/DebugStream.hpp"
#include "Debug/Trace.hpp"

using namespace Bolitho;
using namespace Bolitho::IO;
using namespace Bolitho::System;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::System,ConsoleApplication,Application);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
ConsoleApplication* ConsoleApplication::m_pConsoleApplication;
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_WINDOWS)
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL ConsoleApplication::OnInit()
{
  BOOL Result = Application::OnInit();
  if (!Result)
    return Result;
  
  if (!m_ProductName.IsEmpty())
  {
    Trace(String::Format(TEXT("%s %s %s"), m_ProductName.ConstStr(), m_ProductVersion.ToString().ConstStr(), m_ImageConfiguration.ConstStr()));
    Console::SetTitle(m_ProductName + TEXT(" ") + m_ProductVersion.ToString());
  }
  else  
  {
    Trace(String::Format(TEXT("%s"), m_ImageName.ConstStr()));
  }

  Trace(GetCommandLine());
  Trace("Start Time: %s", DateTime::Now().ToString().ConstStr()); 

  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsoleApplication::InitApplication()
{
  Application::InitApplication();

  m_pConsoleApplication = this;
  SetConsoleCtrlHandler(&ConsoleApplication::ConsoleCtrlHandler, TRUE);

  Console::Show();

  StdIn = StandardStream::GetStandardIn();
  StdOut = StandardStream::GetStandardOut();
  StdErr = StandardStream::GetStandardError();
  Trace.AddConsoleSink();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL ConsoleApplication::ConsoleCtrlHandler(DWORD Event)
{
  if (m_pConsoleApplication != 0)
    return m_pConsoleApplication->OnConsoleEvent(Event);
  else
    return FALSE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL ConsoleApplication::OnConsoleEvent(DWORD Event)
{
  return FALSE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsoleApplication::OnExit()
{
  return Application::OnExit();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsoleApplication::OnException(Exception& E)
{
  Application::OnException(E);

  /*
  if (!StdOut.IsRedirected())
  {
    StdOut.Write(TEXT("Press any key to terminate"));
    StdIn.ReadLine();
  }
  */
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_POSIX)
BOOL ConsoleApplication::OnInit()
{
  BOOL Result = Application::OnInit();
  if (!Result)
    return Result;
  
  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsoleApplication::InitApplication(LPCTSTR VersionString)
{
  Application::InitApplication(VersionString);

  m_pConsoleApplication = this;

  StdIn = StandardStream::GetStandardIn();
  StdOut = StandardStream::GetStandardOut();
  StdErr = StandardStream::GetStandardError();

  Trace.AddSink(StdOut);

  if (!m_ProductName.IsEmpty())
  {
    Trace(String::Format(TEXT("%s %s %s"), m_ProductName.ConstStr(), m_ProductVersion.ToString().ConstStr(), m_ImageConfiguration.ConstStr()));
  }
  else  
  {
    Trace(String::Format(TEXT("%s"), m_ImageName.ConstStr()));
  }

  Trace(GetCommandLine());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsoleApplication::OnExit()
{
  return Application::OnExit();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsoleApplication::OnException(Exception& E)
{
  Application::OnException(E);
	StdOut.Write(TEXT("Press any key to terminate"));
  StdIn.ReadLine();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL ConsoleApplication::OnConsoleEvent(DWORD Event)
{
  return FALSE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#endif


