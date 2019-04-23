/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/Registry.cpp $
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

#include "System/Registry.hpp"

using namespace Bolitho;
using namespace Bolitho::System;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
RegistryKey RegistryKey::LocalMachine = RegistryKey(HKEY_LOCAL_MACHINE);
RegistryKey RegistryKey::CurrentUser = RegistryKey(HKEY_CURRENT_USER);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
RegistryKey::RegistryKey(HKEY hKey)
{
  m_hKey = hKey;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
RegistryKey::~RegistryKey()
{
  Close();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID RegistryKey::Close()
{
  if (m_hKey != 0 && m_hKey != HKEY_LOCAL_MACHINE && m_hKey != HKEY_CURRENT_USER)
  {
    RegCloseKey(m_hKey);
    m_hKey = 0;
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<RegistryKey> RegistryKey::CreateSubkey(LPCTSTR Name, REGSAM Access)
{
  HKEY hKey;
  DWORD Result = RegCreateKeyEx(m_hKey, Name, 0, 0, REG_OPTION_NON_VOLATILE, Access, NULL, &hKey, NULL);
  if (Result != ERROR_SUCCESS)
    Win32Exception::ThrowError(0,Result);
  return new RegistryKey(hKey);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<RegistryKey> RegistryKey::OpenSubkey(LPCTSTR Name, REGSAM Access)
{
  HKEY hKey;
  DWORD Result = RegOpenKeyEx(m_hKey, Name, 0, Access, &hKey);
  if (Result != ERROR_SUCCESS)
    Win32Exception::ThrowError(0,Result);
  return new RegistryKey(hKey);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<RegistryKey> RegistryKey::TryCreateSubkey(LPCTSTR Name, REGSAM Access)
{
  HKEY hKey;
  DWORD Result = RegCreateKeyEx(m_hKey, Name, 0, 0, REG_OPTION_NON_VOLATILE, Access, NULL, &hKey, NULL);
  if (Result != ERROR_SUCCESS)
    return Ptr<RegistryKey>::Null;
  return new RegistryKey(hKey);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<RegistryKey> RegistryKey::TryOpenSubkey(LPCTSTR Name, REGSAM Access)
{
  HKEY hKey;
  DWORD Result = RegOpenKeyEx(m_hKey, Name, 0, Access, &hKey);
  if (Result != ERROR_SUCCESS)
    return Ptr<RegistryKey>::Null;
  return new RegistryKey(hKey);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID RegistryKey::DeleteSubKey(LPCTSTR Name)
{
  DWORD Result = RegDeleteKey(m_hKey, Name);
  if (Result != ERROR_SUCCESS)
    Win32Exception::ThrowError(0,Result);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID RegistryKey::DeleteValue(LPCTSTR Name)
{
  DWORD Result = RegDeleteValue(m_hKey, Name);
  if (Result != ERROR_SUCCESS)
    Win32Exception::ThrowError(0,Result);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID RegistryKey::SetValue(LPCTSTR Name, LPCVOID Data, SIZE_T Length)
{
  DWORD Result = RegSetValueEx(m_hKey, Name, 0, REG_BINARY, (LPCBYTE)Data, (DWORD)Length);
  if (Result != ERROR_SUCCESS)
    Win32Exception::ThrowError(0,Result);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID RegistryKey::SetValue(LPCTSTR Name, CONST Buffer<BYTE>& Value)
{
  DWORD Result = RegSetValueEx(m_hKey, Name, 0, REG_BINARY, (LPCBYTE)Value.GetBuffer(), (DWORD)Value.Length());
  if (Result != ERROR_SUCCESS)
    Win32Exception::ThrowError(0,Result);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID RegistryKey::SetValue(LPCTSTR Name, DWORD Value)
{
  DWORD Result = RegSetValueEx(m_hKey, Name, 0, REG_DWORD, (LPCBYTE)&Value, (DWORD)sizeof(DWORD));
  if (Result != ERROR_SUCCESS)
    Win32Exception::ThrowError(0,Result);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID RegistryKey::SetValue(LPCTSTR Name, LPCTSTR Value)
{
  DWORD Result = RegSetValueEx(m_hKey, Name, 0, REG_SZ, (LPCBYTE)Value, (DWORD)CharTraits<TCHAR>::Length(Value));
  if (Result != ERROR_SUCCESS)
    Win32Exception::ThrowError(0,Result);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL RegistryKey::GetValue(LPCTSTR Name, Buffer<BYTE>& Value)
{
  DWORD Size = 0;
  DWORD Type = 0;
  RegQueryValueEx(m_hKey, Name, 0, &Type, NULL, &Size);
  
  if (Type != REG_BINARY)
    return FALSE;

  Value.Resize(Size);
  DWORD Result = RegQueryValueEx(m_hKey, Name, 0, &Type, Value.GetBuffer(), &Size);
  if (Result != ERROR_SUCCESS)
    Win32Exception::ThrowError(0,Result);
  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL RegistryKey::GetValue(LPCTSTR Name, DWORD& Value)
{
  DWORD Size = 0;
  DWORD Type = 0;

  DWORD Result = RegQueryValueEx(m_hKey, Name, 0, &Type, (LPBYTE)&Value, &Size);

  if (Type != REG_DWORD)
    return FALSE;
  if (Result != ERROR_SUCCESS)
    Win32Exception::ThrowError(0,Result);

  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL RegistryKey::GetValue(LPCTSTR Name, String& Value)
{
  DWORD Size = 0;
  DWORD Type = 0;
  RegQueryValueEx(m_hKey, Name, 0, &Type, NULL, &Size);
  
  if (Type != REG_SZ)
    return FALSE;

  Value.Resize(Size-1);
  DWORD Result = RegQueryValueEx(m_hKey, Name, 0, &Type, (LPBYTE)Value.Str(), &Size);
  if (Result != ERROR_SUCCESS)
    Win32Exception::ThrowError(0,Result);
  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
