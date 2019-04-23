/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/Module.cpp $
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

#include "System/Module.hpp"
#include "Memory/Memory.hpp"

using namespace Bolitho;
using namespace Bolitho::System;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::System,Module,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Map<MODULEHANDLE,Module*> Module::m_HandleToObjectMap;
//-----------------------------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------------------------------
Module::Module()
{}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Module::~Module()
{
  m_HandleToObjectMap.Remove(m_hModule);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Module::InitModule(MODULEHANDLE hModule)
{
  m_hModule = hModule;
  m_HandleToObjectMap.Add(m_hModule, this);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Module> Module::GetModule(MODULEHANDLE hModule)
{
  Module* pModule = 0;
  if (m_HandleToObjectMap.Get(hModule, pModule))
    return pModule;
  return Ptr<Module>::Null;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_WINDOWS)
Ptr<Module> Module::GetModule(LPCTSTR Filename)
{
  return GetModule(GetModuleHandle(Filename));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Module> Module::GetProcessModule()
{
  return GetModule(GetModuleHandle(0));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Module> Module::GetModuleFromAddress(LPVOID p)
{
  for (Map<MODULEHANDLE,Module*>::Iterator i = m_HandleToObjectMap.Begin(); i != m_HandleToObjectMap.End(); i++)
  {
    Pair<MODULEHANDLE,Module*> P = *i;
    if (P.Second->IsPointerInModule(p))
      return P.Second;
  }
  return Ptr<Module>::Null;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Module::IsPointerInModule(LPVOID p) CONST
{
  return (p >= m_pBaseAddress && p < (BYTE*)m_pBaseAddress+m_ImageSize);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Module::GetModuleInformation()
{
  TCHAR FileName[1024];
  GetModuleFileName(m_hModule, FileName, 1024);

  m_ImageName = FileName;

  MODULEINFO Info;
	::GetModuleInformation(GetCurrentProcess(), m_hModule, &Info, sizeof(Info));

  m_pBaseAddress = Info.lpBaseOfDll;
  m_ImageSize = Info.SizeOfImage;

  DWORD Handle = 0;
  DWORD VersionLength = GetFileVersionInfoSize(FileName, &Handle);
  
  if (VersionLength == 0)
    return;
  
  PVOID pVersionInfo = AllocateMemory(VersionLength + 1);
  GetFileVersionInfo(FileName, Handle, VersionLength, pVersionInfo);

  VS_FIXEDFILEINFO* pFixedInfo = 0;
  UINT Length = 0;
  VerQueryValue(pVersionInfo, TEXT("\\"), (LPVOID*)&pFixedInfo, &Length);

  m_ProductVersion = Version(pFixedInfo->dwProductVersionMS, pFixedInfo->dwProductVersionLS);
  m_ImageVersion = Version(pFixedInfo->dwFileVersionMS, pFixedInfo->dwFileVersionLS);

  struct
  {
 	  WORD LangID;
 	  WORD CharSet;
  }* pTranslation = 0;

  VerQueryValue(pVersionInfo, TEXT("\\VarFileInfo\\Translation"), (LPVOID*)&pTranslation, &Length);

  TCHAR* pString = 0;

  VerQueryValue(pVersionInfo, String::Format(TEXT("\\StringFileInfo\\%04x%04x\\ProductName"), pTranslation->LangID, pTranslation->CharSet), (LPVOID*)&pString, &Length); 
  if (Length > 0)
    m_ProductName = pString;
  else
    m_ProductName = String::Empty;

  VerQueryValue(pVersionInfo, String::Format(TEXT("\\StringFileInfo\\%04x%04x\\FileDescription"), pTranslation->LangID, pTranslation->CharSet), (LPVOID*)&pString, &Length); 
  if (Length > 0)
    m_ProductDescription = pString;
  else
    m_ProductDescription = String::Empty;

  VerQueryValue(pVersionInfo, String::Format(TEXT("\\StringFileInfo\\%04x%04x\\CompanyName"), pTranslation->LangID, pTranslation->CharSet), (LPVOID*)&pString, &Length); 
  if (Length > 0)
    m_ProductCompany = pString;
  else
    m_ProductCompany = String::Empty;

  VerQueryValue(pVersionInfo, String::Format(TEXT("\\StringFileInfo\\%04x%04x\\Build Time"), pTranslation->LangID, pTranslation->CharSet), (LPVOID*)&pString, &Length); 
  if (Length > 0)
    m_ImageBuildDate = pString;
  else
    m_ImageBuildDate = String::Empty;

  VerQueryValue(pVersionInfo, String::Format(TEXT("\\StringFileInfo\\%04x%04x\\Commited Time"), pTranslation->LangID, pTranslation->CharSet), (LPVOID*)&pString, &Length); 
  if (Length > 0)
    m_ImageCommitDate = pString;
  else
    m_ImageCommitDate = String::Empty;

  VerQueryValue(pVersionInfo, String::Format(TEXT("\\StringFileInfo\\%04x%04x\\Configuration"), pTranslation->LangID, pTranslation->CharSet), (LPVOID*)&pString, &Length); 
  if (Length > 0)
    m_ImageConfiguration = pString;
  else 
    m_ImageConfiguration = String::Empty;

  VerQueryValue(pVersionInfo, String::Format(TEXT("\\StringFileInfo\\%04x%04x\\LegalCopyright"), pTranslation->LangID, pTranslation->CharSet), (LPVOID*)&pString, &Length); 
  if (Length > 0)
    m_ProductCopyright = pString;
  else 
    m_ProductCopyright = String::Empty;

  FreeMemory(pVersionInfo);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Module::GetResourceString(UINT ResourceId)
{
  return String::LoadFromResource(ResourceId);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
StringA Module::GetResourceStringA(LPCTSTR Type, UINT ResourceId)
{
  return GetResourceStringA(Type, MAKEINTRESOURCE(ResourceId)); 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
StringA Module::GetResourceStringA(LPCTSTR Type, LPCTSTR ResourceName)
{
  HRSRC hResource = FindResource(m_hModule, ResourceName, Type);
  DWORD Length = SizeofResource(m_hModule, hResource);
  HGLOBAL hGlobal = LoadResource(m_hModule, hResource);
  CHAR* pString = (CHAR*)LockResource(hGlobal);
  StringA S(pString, Length);
  FreeResource(hGlobal);

  return S;
}
#if defined(USE_UNICODE)
//-----------------------------------------------------------------------------------------------------------------------------------------------------
StringW Module::GetResourceStringW(LPCTSTR Type, UINT ResourceId)
{
  return GetResourceStringW(Type, MAKEINTRESOURCE(ResourceId)); 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
StringW Module::GetResourceStringW(LPCTSTR Type, LPCTSTR ResourceName)
{
  HRSRC hResource = FindResource(m_hModule, ResourceName, Type);
  DWORD Length = SizeofResource(m_hModule, hResource);
  HGLOBAL hGlobal = LoadResource(m_hModule, hResource);
  WCHAR* pString = (WCHAR*)LockResource(hGlobal);
  StringW S(pString, Length);
  FreeResource(hGlobal);

  return S;
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Module::GetResourceString(LPCTSTR Type, UINT ResourceId)
{
  return GetResourceString(Type, MAKEINTRESOURCE(ResourceId)); 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Module::GetResourceString(LPCTSTR Type, LPCTSTR ResourceName)
{
  HRSRC hResource = FindResource(m_hModule, ResourceName, Type);
  DWORD Length = SizeofResource(m_hModule, hResource);
  HGLOBAL hGlobal = LoadResource(m_hModule, hResource);
  TCHAR* pString = (TCHAR*)LockResource(hGlobal);
  String S(pString, Length);
  FreeResource(hGlobal);

  return S;
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_POSIX)
Ptr<Module> Module::GetModule(LPCTSTR Filename)
{
  throw NotImplementedException(0);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Module> Module::GetProcessModule()
{
  throw NotImplementedException(0);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Module> Module::GetModuleFromAddress(LPVOID p)
{
  throw NotImplementedException(0);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Module::IsPointerInModule(LPVOID p) CONST
{
  throw NotImplementedException(this);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Module::GetModuleInformation()
{
  throw NotImplementedException(this);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Module::GetResourceString(UINT ResourceId)
{
  throw NotImplementedException(this);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
StringA Module::GetResourceStringA(LPCTSTR Type, UINT ResourceId)
{
  throw NotImplementedException(this);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
StringA Module::GetResourceStringA(LPCTSTR Type, LPCTSTR ResourceName)
{
  throw NotImplementedException(this);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_UNICODE)
StringW Module::GetResourceStringW(LPCTSTR Type, UINT ResourceId)
{
  throw NotImplementedException(this);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
StringW Module::GetResourceStringW(LPCTSTR Type, LPCTSTR ResourceName)
{
  throw NotImplementedException(this);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#endif
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------
