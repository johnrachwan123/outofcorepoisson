/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Com/Variant.cpp $
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




#include "CodeLibrary.hpp"
#include "Com/Variant.hpp"

using namespace Bolitho;
using namespace Bolitho::Com;


#include <comutil.h>

//-----------------------------------------------------------------------------------------------------------------------------------------------------
Variant::Variant()
{
  VariantInit(this);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Variant::~Variant()
{
  Clear();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Variant::Variant(CONST VARIANT& V)
{
  VariantCopy(this, (VARIANTARG*)&V);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Variant::Variant(CONST VARIANT* pV)
{
  VariantCopy(this, (VARIANTARG*)pV);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Variant::Variant(CONST Variant& V)
{
  VariantCopy(this, (VARIANTARG*)&V);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Variant::Variant(BYTE b)
{
  vt = VT_UI1;
  bVal = b;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Variant::Variant(SHORT s)
{
  vt = VT_I2;
  iVal = s;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Variant::Variant(USHORT us)
{
  vt = VT_UI2;
  uiVal = us;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Variant::Variant(INT i)
{
  vt = VT_I4;
  lVal = i;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Variant::Variant(UINT ui)
{
  vt = VT_UI4;
  ulVal = ui;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Variant::Variant(LONG l)
{
  vt = VT_I4;
  lVal = l;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Variant::Variant(ULONG ul)
{
  vt = VT_UI4;
  ulVal = ul;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Variant::Variant(LONGLONG ll)
{
  vt = VT_I8;
  llVal = ll;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Variant::Variant(ULONGLONG ull)
{
  vt = VT_UI8;
  ullVal = ull;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Variant::Variant(BSTR Str)
{
  vt = VT_BSTR;
  bstrVal = SysAllocString(Str);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Variant::Variant(LPCTSTR Str)
{
  vt = VT_BSTR;

#if defined(UNICODE)
  bstrVal = SysAllocString(Str);
#else
  String S(Str);
  bstrVal = SysAllocString(S.ToStringW());
#endif

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Variant::Variant(bool b)
{
  vt = VT_BOOL;
  boolVal = b ? -1 : 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Variant::Variant(CHAR c)
{
  vt = VT_UI1;
  cVal = c;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Variant::Variant(FLOAT f)
{
  vt = VT_R4;
  fltVal = f;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Variant::Variant(DOUBLE d)
{
  vt = VT_R8;
  dblVal = d;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Variant::Variant(IUnknown* p)
{
  p->AddRef();
  vt = VT_UNKNOWN;
  this->punkVal = p;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Variant::Clear()
{
  VariantClear(this);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Variant::ToString() CONST
{
  switch(vt)
  {
  case VT_BSTR:
    return String(bstrVal);
  default:
    return String();
  };
}