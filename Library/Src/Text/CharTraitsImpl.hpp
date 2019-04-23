/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Text/CharTraitsImpl.hpp $
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




//----------------------------------------------------------------------------------------------------------------------------
template<class T>
T CharTraits<T>::ToUpper(T x)
{
  return toupper(x);
}
//----------------------------------------------------------------------------------------------------------------------------
template<class T>
T CharTraits<T>::ToLower(T x)
{
  return tolower(x);
}
//----------------------------------------------------------------------------------------------------------------------------

#if defined(PLATFORM_WINDOWS)
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline SIZE_T CharTraits<CHAR>::Length(CONST CHAR* pString)
{
  if (pString == 0)
    return 0;
  return strlen(pString);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline SIZE_T CharTraits<CHAR>::FormatLengthV(CONST CHAR* Format, va_list Args)
{
  if (Format == 0)
    return 0;
  return (SIZE_T)vsnprintf(0, 0, Format, Args);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline VOID CharTraits<CHAR>::FormatV(CHAR* Buffer, SIZE_T Count, CONST CHAR* Format, va_list Args)
{
  if (Format == 0 || Buffer == 0)
    return;
  vsnprintf(Buffer, Count, Format, Args);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline CHAR* CharTraits<CHAR>::Duplicate(CONST CHAR* String)
{
	if (String == 0)
    return 0;
  return _strdup(String);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline CHAR* CharTraits<CHAR>::StrStr(CONST CHAR* A, CONST CHAR* B)
{
  if (A == 0 || B == 0)
    return 0;
  return (CHAR*)strstr(A,B);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline INT CharTraits<CHAR>::StrCmp(CONST CHAR* A, CONST CHAR* B, SIZE_T N)
{
  if (A == 0 || B == 0)
    return -1;
  return strncmp(A, B, N);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline INT CharTraits<CHAR>::StrICmp(CONST CHAR* A, CONST CHAR* B, SIZE_T N)
{
  if (A == 0 || B == 0)
    return -1;
  return _strnicmp(A, B, N);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline INT CharTraits<CHAR>::StrCmp(CONST CHAR* A, CONST CHAR* B)
{
  if (A == 0 || B == 0)
    return -1;

  SIZE_T N = Length(A);
  if (Length(B) < N)
    N = Length(B);
  return StrCmp(A, B, N);
}
//----------------------------------------------------------------------------------------------------------------------------
template<class CharType>
inline INT CharTraits<CharType>::StrICmp(CONST CharType* A, CONST CharType* B)
{
  if (A == 0 || B == 0)
    return -1;
  SIZE_T N = Length(A);
  if (Length(B) < N)
    N = Length(B);
  return StrICmp(A,B,N);
}
//----------------------------------------------------------------------------------------------------------------------------

#if defined(USE_UNICODE)
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline SIZE_T CharTraits<WCHAR>::Length(CONST WCHAR* pString)
{
  if (pString == 0)
    return 0;
  return wcslen(pString);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline SIZE_T CharTraits<WCHAR>::FormatLengthV(CONST WCHAR* Format, va_list Args)
{
  if (Format == 0)
    return 0;
  return (SIZE_T)_vscwprintf(Format, Args);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline VOID CharTraits<WCHAR>::FormatV(WCHAR* Buffer, SIZE_T Count, CONST WCHAR* Format, va_list Args)
{
  if (Format == 0 || Buffer == 0)
    return;
  vswprintf(Buffer, Count, Format, Args);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline WCHAR* CharTraits<WCHAR>::Duplicate(CONST WCHAR* String)
{
  if (String == 0)
    return 0;
  return _wcsdup(String);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline INT CharTraits<WCHAR>::StrCmp(CONST WCHAR* A, CONST WCHAR* B, SIZE_T N)
{
  if (A == 0 || B == 0)
    return -1;
  return wcsncmp(A, B, N*2);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline INT CharTraits<WCHAR>::StrICmp(CONST WCHAR* A, CONST WCHAR* B, SIZE_T N)
{
  if (A == 0 || B == 0)
    return -1;
  return _wcsnicmp(A, B, N);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline WCHAR* CharTraits<WCHAR>::StrStr(CONST WCHAR* A, CONST WCHAR* B)
{
  if (A == 0 || B == 0)
    return 0;
  return (WCHAR*)wcsstr(A,B);
}
#endif
#endif

#if defined(PLATFORM_POSIX)
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline SIZE_T CharTraits<CHAR>::Length(CONST CHAR* pString)
{
  if (pString == 0)
    return 0;
  return strlen(pString);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline SIZE_T CharTraits<CHAR>::FormatLengthV(CONST CHAR* Format, va_list Args)
{
  if (Format == 0)
    return 0;
  SIZE_T N = 0;
  va_list Args0;
  va_copy(Args0,Args);
  N = vsnprintf(0, 0, Format, Args0);
  va_end(Args0);
  return N;
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline VOID CharTraits<CHAR>::FormatV(CHAR* Buffer, SIZE_T Count, CONST CHAR* Format, va_list Args)
{
  if (Buffer == 0 || Format == 0)
    return;
  va_list Args0;
  va_copy(Args0,Args);
  vsnprintf(Buffer, Count, Format, Args0);
  va_end(Args0);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline CHAR* CharTraits<CHAR>::Duplicate(CONST CHAR* String)
{
  if (String == 0)
    return 0;
  return strdup(String);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline CHAR* CharTraits<CHAR>::StrStr(CONST CHAR* A, CONST CHAR* B)
{
  if (A == 0 || B == 0)
    return 0;
  return (CHAR*)strstr(A,B);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline INT CharTraits<CHAR>::StrCmp(CONST CHAR* A, CONST CHAR* B, SIZE_T N)
{
  if (A == 0 || B == 0)
    return -1;
  return strncmp(A, B, N);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline INT CharTraits<CHAR>::StrCmp(CONST CHAR* A, CONST CHAR* B)
{
  if (A == 0 || B == 0)
    return -1;

  SIZE_T N = Length(A);
  if (Length(B) < N)
    N = Length(B);
  return StrCmp(A, B, N);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline INT CharTraits<CHAR>::StrICmp(CONST CHAR* A, CONST CHAR* B, SIZE_T N)
{
  if (A == 0 || B == 0)
    return -1;
  return strncasecmp(A, B, N);
}
//----------------------------------------------------------------------------------------------------------------------------
template<class CharType>
inline INT CharTraits<CharType>::StrICmp(CONST CharType* A, CONST CharType* B)
{
  if (A == 0 || B == 0)
    return -1;
  SIZE_T N = Length(A);
  if (Length(B) < N)
    N = Length(B);
  return StrICmp(A,B,N);
}
//----------------------------------------------------------------------------------------------------------------------------

#if defined(USE_UNICODE)
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline SIZE_T CharTraits<WCHAR>::Length(CONST WCHAR* pString)
{
  if (pString == 0)
    return 0;
  return wcslen(pString);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline SIZE_T CharTraits<WCHAR>::FormatLengthV(CONST WCHAR* Format, va_list Args)
{
  if (Format == 0)
    return 0;

  SIZE_T N = 0;
  va_list Args0;
  va_copy(Args0,Args);
  N = vswprintf(0, 0, Format, Args0);
  va_end(Args0);
  
  return N;
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline VOID CharTraits<WCHAR>::FormatV(WCHAR* Buffer, SIZE_T Count, CONST WCHAR* Format, va_list Args)
{
  if (Format == 0 || Buffer == 0)
    return;

  va_list Args0;
  va_copy(Args0,Args);
  vswprintf(Buffer, Count, Format, Args0);
  va_end(Args0);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline WCHAR* CharTraits<WCHAR>::Duplicate(CONST WCHAR* String)
{
  if (String == 0)
    return 0;
  return wcsdup(String);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline INT CharTraits<WCHAR>::StrCmp(CONST WCHAR* A, CONST WCHAR* B, SIZE_T N)
{
  if (A == 0 || B == 0)
    return -1;
  return wcsncmp(A, B, N*2);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline INT CharTraits<WCHAR>::StrICmp(CONST WCHAR* A, CONST WCHAR* B, SIZE_T N)
{
  if (A == 0 || B == 0)
    return -1;
  return wcsncasecmp(A, B, N);
}
//----------------------------------------------------------------------------------------------------------------------------
template<>
inline WCHAR* CharTraits<WCHAR>::StrStr(CONST WCHAR* A, CONST WCHAR* B)
{
  if (A == 0 || B == 0)
    return 0;
  return (WCHAR*)wcsstr(A,B);
}
#endif
#endif

