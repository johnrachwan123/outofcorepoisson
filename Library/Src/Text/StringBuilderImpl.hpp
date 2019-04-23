/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Text/StringBuilderImpl.hpp $
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




//--------------------------------------------------------------------------------------------------------------------------
ImplementTemplateRuntimeType(Bolitho,TStringBuilder,Object);
//--------------------------------------------------------------------------------------------------------------------------
template<typename CharType>
TStringBuilder<CharType>::TStringBuilder()
{
  m_Position = 0;
}
//--------------------------------------------------------------------------------------------------------------------------
template<typename CharType>
TStringBuilder<CharType>::TStringBuilder(CONST TStringBuilder& B)
{
  m_Buffer = B.m_Buffer;
  m_Position = B.m_Position;
}
//--------------------------------------------------------------------------------------------------------------------------
template<typename CharType>
TStringBuilder<CharType>::TStringBuilder(LPCTSTR S)
{
  SIZE_T Length = CharTraits<CharType>::Length(S);
  m_Buffer.Resize(ComputeAllocationSize(Length));
  CopyMemory(&m_Buffer[0], S, Length*sizeof(CharType));
  m_Position = Length;
}
//--------------------------------------------------------------------------------------------------------------------------
template<typename CharType>
TStringBuilder<CharType>& TStringBuilder<CharType>::operator= (CONST TStringBuilder<CharType>& B)
{
  m_Buffer = B.m_Buffer;
  m_Position = B.m_Position;
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------
template<typename CharType>
TStringBuilder<CharType>& TStringBuilder<CharType>::operator= (LPCTSTR S)
{
  SIZE_T Length = CharTraits<CharType>::Length(S);
  m_Buffer.Resize(ComputeAllocationSize(Length));
  CopyMemory(&m_Buffer[0], S, Length*sizeof(CharType));
  m_Position = Length;
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------
template<typename CharType>
VOID TStringBuilder<CharType>::Clear()
{
  m_Buffer.Resize(0);
  m_Position = 0;
}
//--------------------------------------------------------------------------------------------------------------------------
template<typename CharType>
SIZE_T TStringBuilder<CharType>::Length()
{
  return m_Position;
}
//--------------------------------------------------------------------------------------------------------------------------
template<typename CharType>
VOID TStringBuilder<CharType>::Append(LPCTSTR S)
{
  SIZE_T Length = CharTraits<CharType>::Length(S);
  m_Buffer.Resize(ComputeAllocationSize(m_Position+Length));
  CopyMemory(&m_Buffer[m_Position], S, Length*sizeof(CharType));
  m_Position += Length;
}
//--------------------------------------------------------------------------------------------------------------------------
template<typename CharType>
TStringBuilder<CharType>& TStringBuilder<CharType>::operator+= (LPCTSTR S)
{
  Append(S);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------
template<typename CharType>
VOID TStringBuilder<CharType>::AppendLine(LPCTSTR S)
{
  SIZE_T Length = CharTraits<CharType>::Length(S);
  m_Buffer.Resize(ComputeAllocationSize(m_Position+Length+2));
  CopyMemory(&m_Buffer[m_Position], S, Length*sizeof(CharType));
  m_Position += Length;
  m_Buffer[m_Position++] = '\r';
  m_Buffer[m_Position++] = '\n';
}
//--------------------------------------------------------------------------------------------------------------------------
template<typename CharType>
VOID TStringBuilder<CharType>::AppendFormat(LPCTSTR Format, ...)
{
  va_list Args;
  va_start(Args,Format);
  TString<CharType> Str = TString<CharType>::FormatV(Format, Args);
  va_end(Args);
  Append(Str);
}
//--------------------------------------------------------------------------------------------------------------------------
template<typename CharType>
VOID TStringBuilder<CharType>::AppendFormatLine(LPCTSTR Format, ...)
{
  va_list Args;
  va_start(Args,Format);
  TString<CharType> Str = TString<CharType>::FormatV(Format, Args);
  va_end(Args);
  AppendLine(Str);
}
//--------------------------------------------------------------------------------------------------------------------------
template<typename CharType>
TString<TCHAR> TStringBuilder<CharType>::ToString() CONST
{
  return TString<TCHAR>(m_Buffer);
}
//--------------------------------------------------------------------------------------------------------------------------
template<typename CharType>
TString<CHAR> TStringBuilder<CharType>::ToStringA() CONST
{
  return TString<CHAR>(m_Buffer);
}
//--------------------------------------------------------------------------------------------------------------------------
#if defined(USE_UNICODE)
template<typename CharType>
TString<WCHAR> TStringBuilder<CharType>::ToStringW() CONST
{
  return TString<WCHAR>(m_Buffer);
}
#endif
//--------------------------------------------------------------------------------------------------------------------------
template<typename CharType>
SIZE_T TStringBuilder<CharType>::ComputeAllocationSize(SIZE_T N)
{
  return N+1;
}
//--------------------------------------------------------------------------------------------------------------------------
