/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Type.cpp $
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

using namespace Bolitho;

//----------------------------------------------------------------------------------------------------
LIBRARY_API CONST Type Bolitho::UnknownType = Type(TEXT("Bolitho"),TEXT("UnknownType"),0,0,0);
//----------------------------------------------------------------------------------------------------
LIBRARY_API CONST Type Type::RuntimeType = Type(TEXT("Bolitho"), TEXT("Type"), &Object::RuntimeType, 0, 0);
//----------------------------------------------------------------------------------------------------
CONST Type* Type::GetType() CONST
{
  return &Type::RuntimeType;
}
//----------------------------------------------------------------------------------------------------
Type::Type(CONST TCHAR* Namespace, CONST TCHAR* Name, CONST Type* pBaseClass0, CONST Type* pTemplate0, CONST Type* pTemplate1, CONST Type* pTemplate2)
{
  m_Name = Name;
  m_Namespace = Namespace;
  m_pBaseClass[0] = pBaseClass0;
  m_pTemplate[0] = pTemplate0;
  m_pTemplate[1] = pTemplate1;
}
//----------------------------------------------------------------------------------------------------
CONST TCHAR* Type::Namespace() CONST
{
  return m_Namespace;
}
//----------------------------------------------------------------------------------------------------
CONST TCHAR* Type::Name() CONST
{
  return m_Name;
}
//----------------------------------------------------------------------------------------------------
CONST Type* Type::BaseClass(SIZE_T i) CONST
{
  Assert(i < 4);
  return m_pBaseClass[i];
}
//----------------------------------------------------------------------------------------------------
CONST Type* Type::Template(SIZE_T i) CONST
{
  Assert(i < 4);
  return m_pTemplate[i];
}
//----------------------------------------------------------------------------------------------------
String Type::ToString() CONST
{
  if (m_pTemplate[0] == 0)
    if (m_Namespace == 0 || m_Namespace[0] == 0)
      return String::Format(TEXT("%s"), m_Name);
    else
      return String::Format(TEXT("%s::%s"), m_Namespace, m_Name);
  else if (m_pTemplate[1] == 0)
    if (m_Namespace == 0 || m_Namespace[0] == 0)
      return String::Format(TEXT("%s<%s>"), m_Name, m_pTemplate[0]->ToString().ConstStr());
    else
      return String::Format(TEXT("%s::%s<%s>"), m_Namespace, m_Name, m_pTemplate[0]->ToString().ConstStr());
  else
    if (m_Namespace == 0 || m_Namespace[0] == 0)
      return String::Format(TEXT("%s<%s,%s>"), m_Name, m_pTemplate[0]->ToString().ConstStr(), m_pTemplate[1]->ToString().ConstStr());
    else
      return String::Format(TEXT("%s::%s<%s,%s>"), m_Namespace, m_Name, m_pTemplate[0]->ToString().ConstStr(), m_pTemplate[1]->ToString().ConstStr());
}
//----------------------------------------------------------------------------------------------------
Object* Type::CreateInstance() CONST
{
  if (m_pFactory == 0)
    return 0;
  else 
    return m_pFactory();
}
//----------------------------------------------------------------------------------------------------
CONST Type* Bolitho::GetType(CONST Object& O)
{
  return O.GetType();
}
//----------------------------------------------------------------------------------------------------
BOOL Type::IsDerivedFrom(CONST Type* Base) CONST
{
  CONST Type* T = this;
  do
  {
    if (T == Base)
      return true;
    T = T->BaseClass();
  }
  while(T != 0);
  return false;
}
//----------------------------------------------------------------------------------------------------
namespace Bolitho
{
  ImplementIntrinsicRuntimeType(CHAR);
  ImplementIntrinsicRuntimeType(WCHAR);
  ImplementIntrinsicRuntimeType(BYTE);
  ImplementIntrinsicRuntimeType(SHORT);
  ImplementIntrinsicRuntimeType(USHORT);
  ImplementIntrinsicRuntimeType(INT);
  ImplementIntrinsicRuntimeType(UINT);
  ImplementIntrinsicRuntimeType(LONG);
  ImplementIntrinsicRuntimeType(ULONG);
  ImplementIntrinsicRuntimeType(LONGLONG);
  ImplementIntrinsicRuntimeType(ULONGLONG);
  ImplementIntrinsicRuntimeType(FLOAT);
  ImplementIntrinsicRuntimeType(DOUBLE);
}
//----------------------------------------------------------------------------------------------------

