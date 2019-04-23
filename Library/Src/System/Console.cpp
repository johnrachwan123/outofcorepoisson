/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/Console.cpp $
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

#include "System/Console.hpp"
#include "IO/StdIO.hpp"

using namespace Bolitho;
using namespace Bolitho::System;
using namespace Bolitho::IO;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::System,Console,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_WINDOWS)
HANDLE Console::m_hOutput;
HANDLE Console::m_hInput;
WORD Console::m_SavedAttribute;
COORD Console::m_SavedPosition;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Console::Show()
{
  AllocConsole();
  m_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  m_hInput = GetStdHandle(STD_INPUT_HANDLE);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Console::Hide()
{
  FreeConsole();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Console::SetTitle(LPCTSTR Title)
{
  SetConsoleTitle(Title);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Console::GetTitle()
{
  SIZE_T Length = GetConsoleTitle(0,0);
  String S(Length);
  GetConsoleTitle(S.Str(), (DWORD)Length);
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Point Console::GetCursorPosition()
{
  CONSOLE_SCREEN_BUFFER_INFO Info;
  ZeroMemory(&Info, sizeof(Info));
  GetConsoleScreenBufferInfo(m_hOutput, &Info);
  
  return Point(Info.dwCursorPosition.X, Info.dwCursorPosition.Y);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Console::SetCursorPosition(Point Position)
{
  COORD P = { (SHORT)Position.x, (SHORT)Position.y };
  SetConsoleCursorPosition(m_hOutput, P);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Console::SaveCursorPosition()
{
  CONSOLE_SCREEN_BUFFER_INFO Info;
  ZeroMemory(&Info, sizeof(Info));
  GetConsoleScreenBufferInfo(m_hOutput, &Info);
  m_SavedPosition = Info.dwCursorPosition;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Console::RestoreCursorPosition()
{
  SetConsoleCursorPosition(m_hOutput, m_SavedPosition);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Size Console::GetWindowSize()
{
  CONSOLE_SCREEN_BUFFER_INFO Info;
  ZeroMemory(&Info, sizeof(Info));
  GetConsoleScreenBufferInfo(m_hOutput, &Info);
  
  return Size(Info.srWindow.Right-Info.srWindow.Left+1, Info.srWindow.Bottom-Info.srWindow.Top+1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Rect Console::GetWindowRect()
{
  CONSOLE_SCREEN_BUFFER_INFO Info;
  ZeroMemory(&Info, sizeof(Info));
  
  GetConsoleScreenBufferInfo(m_hOutput, &Info);
  return Rect(Info.srWindow.Left, Info.srWindow.Top, Info.srWindow.Right, Info.srWindow.Bottom);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LONG Console::GetWindowWidth()
{
  CONSOLE_SCREEN_BUFFER_INFO Info;
  ZeroMemory(&Info, sizeof(Info));
  GetConsoleScreenBufferInfo(m_hOutput, &Info);
  
  return Info.srWindow.Right-Info.srWindow.Left+1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LONG Console::GetWindowHeight()
{
  CONSOLE_SCREEN_BUFFER_INFO Info;
  ZeroMemory(&Info, sizeof(Info));
  GetConsoleScreenBufferInfo(m_hOutput, &Info);
  
  return Info.srWindow.Bottom-Info.srWindow.Top;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Console::SetWindowRect(Rect R)
{
  SMALL_RECT SR = { (SHORT)R.top, (SHORT)R.left, (SHORT)R.right, (SHORT)R.bottom };
  SetConsoleWindowInfo(m_hOutput, TRUE, &SR);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Size Console::GetBufferSize()
{
  CONSOLE_SCREEN_BUFFER_INFO Info;
  ZeroMemory(&Info, sizeof(Info));
  GetConsoleScreenBufferInfo(m_hOutput, &Info);
  
  return Size(Info.dwSize.X, Info.dwSize.Y);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Console::SetBufferSize(Size S)
{
  COORD C = { (SHORT)S.cx, (SHORT)S.cy };
  SetConsoleScreenBufferSize(m_hOutput, C);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Console::SetAttribute(DWORD Foreground, DWORD Background)
{
  WORD Attribute = (WORD)((Foreground & 0xf) + ((Background & 0xf) << 4));
  SetConsoleTextAttribute(m_hOutput, Attribute);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Console::SaveAttribute()
{
  CONSOLE_SCREEN_BUFFER_INFO Info;
  ZeroMemory(&Info, sizeof(Info));
  GetConsoleScreenBufferInfo(m_hOutput, &Info);
  m_SavedAttribute = Info.wAttributes;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Console::RestoreAttribute()
{
  SetConsoleTextAttribute(m_hOutput, m_SavedAttribute);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
