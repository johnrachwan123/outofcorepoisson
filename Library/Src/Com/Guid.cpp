/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Com/Guid.cpp $
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

#include "Com/Guid.hpp"

using namespace Bolitho;

#pragma comment(lib,"rpcrt4.lib")

//----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho,Guid,Object);
//----------------------------------------------------------------------------------------------------------------------------------------------------
Guid::Guid()
{
  m_Data1 = 0;
  m_Data2 = 0;
  m_Data3 = 0;
  ZeroMemory(&m_Data4[0], 8);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Guid::Guid(ULONG Data1, USHORT Data2, USHORT Data3, BYTE Data4[8])
{
  m_Data1 = Data1;
  m_Data2 = Data2;
  m_Data3 = Data3;
  CopyMemory(&m_Data4[0], &Data4[0], 8); 
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Guid::Guid(CONST Guid& G)
{
  m_Data1 = G.m_Data1;
  m_Data2 = G.m_Data2;
  m_Data3 = G.m_Data3;
  CopyMemory(&m_Data4[0], &G.m_Data4[0], 8); 
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Guid::Guid(CONST GUID& G)
{
  m_Data1 = G.Data1;
  m_Data2 = G.Data2;
  m_Data3 = G.Data3;
  CopyMemory(&m_Data4[0], &G.Data4, 8);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Guid::Guid(CONST TCHAR* String)
{
#if defined(UNICODE)
  UuidFromString((RPC_WSTR)String, (UUID*) &m_Data1);
#else
  UuidFromString((RPC_CSTR)String, (UUID*) &m_Data1);
#endif
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Guid Guid::CreateNew()
{
  Guid G;
  UuidCreate((UUID*) &G.m_Data1);
  return G;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
String Guid::ToString() CONST
{
#if defined(UNICODE)
  RPC_WSTR pStr = 0;
  UuidToString((UUID*)&m_Data1, &pStr);
  String Str((CONST TCHAR*)pStr);
  RpcStringFree(&pStr);
  return Str;
#else
  RPC_CSTR pStr = 0;
  UuidToString((UUID*)&m_Data1, &pStr);
  String Str((CONST TCHAR*)pStr);
  RpcStringFree(&pStr);
  return Str;
#endif
}