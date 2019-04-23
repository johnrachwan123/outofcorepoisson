/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Text/StringImpl.hpp $
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

//----------------------------------------------------------------------------------------------------
ImplementTemplateRuntimeType(Bolitho::Internal,TString,Object);
//----------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------
template<>
inline TString<CHAR> TString<CHAR>::ToStringA() CONST
{
  return *this;
}
//-----------------------------------------------------------------------------------------------------
#if defined(USE_UNICODE)
template<>
inline TString<WCHAR> TString<WCHAR>::ToStringW() CONST
{
  return *this;
}
//-----------------------------------------------------------------------------------------------------
template<>
inline TString<WCHAR> TString<CHAR>::ToStringW() CONST
{
  TString<WCHAR> Wide;

  if (Length() > 0)
  {
    SIZE_T WideLength = mbstowcs(0, ConstStr(), Length());
    Wide.Resize(WideLength);
    mbstowcs(Wide.Str(), ConstStr(), Length());
  }
  return Wide;
}
//-----------------------------------------------------------------------------------------------------
template<>
inline TString<CHAR> TString<WCHAR>::ToStringA() CONST
{
  TString<CHAR> Ansi;

  SIZE_T AnsiLength = wcstombs(0, ConstStr(), 0);
  Ansi.Resize(AnsiLength);
  wcstombs(Ansi.Str(), ConstStr(), AnsiLength);

  return Ansi;
}
//-----------------------------------------------------------------------------------------------------
template<>
inline TString<TCHAR> TString<WCHAR>::ToString() CONST
{
#if defined(UNICODE)
  return ToStringW();
#else
  return ToStringA();
#endif
}
#endif
//-----------------------------------------------------------------------------------------------------
template<>
inline TString<TCHAR> TString<CHAR>::ToString() CONST
{
#if defined(UNICODE)
  return ToStringW();
#else
  return ToStringA();
#endif
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>::TString()
{
  m_Length = 0;
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>::TString(SIZE_T N)
{
  m_Length = 0;
  Resize(N);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>::TString(CONST CHAR* pStr)
{
  Assign(pStr);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>::TString(CONST CHAR* pStr, SIZE_T Length)
{
  Assign(pStr,Length);
}
//----------------------------------------------------------------------------------------------------
#if defined(USE_UNICODE)
template<class CharType>
TString<CharType>::TString(CONST WCHAR* pStr)
{
  Assign(pStr);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>::TString(CONST WCHAR* pStr, SIZE_T Length)
{
  Assign(pStr,Length);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>::TString(CONST TString<WCHAR>& Str) 
{
  Assign(Str);
}
#endif
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>::TString(CONST TString<CHAR>& Str) 
{
  Assign(Str);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>::TString(CharType Ch, SIZE_T N)
{
  m_Str.Allocate(N+1);
  for (SIZE_T i=0; i<N; i++)
    m_Str[i] = Ch;
  m_Str[N] = 0;
  m_Length = N;
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>::TString(CONST Array<TString>& A, CONST CharType* Sep)    
{
  Resize(0);
  for (SIZE_T i=0; i<A.Length()-1; i++)
    Append(A[i] + Sep);
  Append(A[A.Length()-1]);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>::TString(CONST Buffer<CharType>& B)
{
  m_Length = CharTraits<CharType>::Length(&B[0]);
  m_Str = B;
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>::TString(CONST Buffer<BYTE>& B)
{
#if defined(UNICODE)
  Assert(FALSE);
#else
  Resize(B.Length());
  CopyMemory(Str(), B.GetBuffer(), B.Length());
#endif
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>::~TString()
{
  m_Length = 0;
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
DWORD TString<CharType>::GetHash() CONST
{
  DWORD H = 0;
  for (SIZE_T i=0; i<m_Length; i++)
    H += m_Str[i];
  return H;
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType> TString<CharType>::FormatV(CONST CharType* Format, va_list Args)
{
  SIZE_T Length = CharTraits<CharType>::FormatLengthV(Format, Args);
  TString<CharType> S;
  S.Resize(Length);
  CharTraits<CharType>::FormatV(S.Str(), Length+1, Format, Args);
  return S;
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType> TString<CharType>::Format(CONST CharType* Format, ...)
{
  va_list Args;
  va_start(Args,Format);
  String s = FormatV(Format, Args);
  va_end(Args);

  return s;
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType> TString<CharType>::FormatSize(ULONGLONG Bytes)
{
  FLOAT Factor = 0;
  CONST CharType* Suffix = 0;

  if (Bytes > 1024.0f*1024.0f*1024.0f*1024.0f)
  { Factor = 1.0f / (1024.0f*1024.0f*1024.0f*1024.0f); Suffix = TEXT("TB"); }
  else if (Bytes > 1024.0f*1024.0f*1024.0f)
  { Factor = 1.0f / (1024.0f*1024.0f*1024.0f); Suffix = TEXT("GB"); }
  else if (Bytes > 1024.0f*1024.0f)
  { Factor = 1.0f / (1024.0f*1024.0f); Suffix = TEXT("MB"); }
  else if (Bytes > 1024.0f)
  { Factor = 1.0f / (1024.0f); Suffix = TEXT("KB"); }
  else
  { Factor = 1.0f; Suffix = TEXT(" bytes"); }

  FLOAT Value = Bytes * Factor;

  if (Factor == 1.0f)
    return TString<CharType>::Format(TEXT("%d bytes"), Bytes);  
  else
    return TString<CharType>::Format(TEXT("%.2f%s"), Value, Suffix);  
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType> TString<CharType>::FormatSizeKB(ULONGLONG Bytes)
{
  FLOAT Factor = 0;
  CONST CharType* Suffix = 0;

  Factor = 1.0f / (1024.0f); Suffix = TEXT("KB");
  FLOAT Value = Bytes * Factor;

  return TString<CharType>::Format(TEXT("%.2f%s"), Value, Suffix);  
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType> TString<CharType>::FormatTimeInterval(ULONGLONG NanoSeconds, DWORD Verbosity)
{
  if (NanoSeconds > 1000000000L)
    return TString<CharType>::Format(TEXT("%.2fs"), NanoSeconds / 1000000000.0f);
  else if (NanoSeconds > 1000000L)
    return TString<CharType>::Format(TEXT("%.2fms"), NanoSeconds/1000000.0f);
  else if (NanoSeconds > 1000L)
    return TString<CharType>::Format(TEXT("%.2fus"), NanoSeconds/1000.0f);
  else
    return TString<CharType>::Format(TEXT("%.2fns"), NanoSeconds/1.0f);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType> TString<CharType>::FormatTimeInterval(DWORD Time, DWORD Verbosity)
{
  DWORD Seconds = Time % 60;
  Time /= 60;
  DWORD Minutes = Time % 60;
  Time /= 60;
  DWORD Hours = Time;

  if (Verbosity == 0)
    return TString<CharType>::Format(TEXT("%02d:%02d:%02d"), Hours, Minutes, Seconds);

  return TEXT("");
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType> TString<CharType>::FormatTimeInterval(FLOAT Time, DWORD Verbosity)
{
  DWORD Fractional = (DWORD)((Time - (DWORD)Time) * 10);
  DWORD Seconds = (DWORD)Time % 60;
  Time /= 60;
  DWORD Minutes = (DWORD)Time % 60;
  Time /= 60;
  DWORD Hours = (DWORD)Time;

  if (Verbosity == 0)
    if (Minutes == 0 && Hours == 0)
      return TString<CharType>::Format(TEXT("%02d:%02d:%02d.%1d"), Hours, Minutes, Seconds, Fractional);
    else
      return TString<CharType>::Format(TEXT("%02d:%02d:%02d"), Hours, Minutes, Seconds);


  return TEXT("");
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
VOID TString<CharType>::Free()
{
  m_Str.Free();
  m_Length = 0;
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
VOID TString<CharType>::Reserve(SIZE_T N)
{
  if (N > m_Length)
    m_Str.Resize(N);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
VOID TString<CharType>::Resize(SIZE_T N)
{
  m_Length = N;
  m_Str.Resize(N+1);
  InitializeObjects<CharType>(m_Str.GetBuffer(), N+1);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>& TString<CharType>::operator= (CONST TString<CHAR>& S)
{
  Assign(S);
  return *this;
}
//----------------------------------------------------------------------------------------------------
#if defined(USE_UNICODE)
template<class CharType>
TString<CharType>& TString<CharType>::operator= (CONST TString<WCHAR>& S)
{
  Assign(S);
  return *this;
}
#endif
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>& TString<CharType>::operator= (CONST CHAR* S)
{
  Assign(S);
  return *this;
}
//----------------------------------------------------------------------------------------------------
#if defined(USE_UNICODE)
template<class CharType>
TString<CharType>& TString<CharType>::operator= (CONST WCHAR* S)
{
  Assign(S);
  return *this;
}
#endif
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>& TString<CharType>::operator= (CHAR C)
{
  Assign(C);
  return *this;
}
//----------------------------------------------------------------------------------------------------
#if defined(USE_UNICODE)
template<class CharType>
TString<CharType>& TString<CharType>::operator= (WCHAR C)
{
  Assign(C);
  return *this;
}
#endif
//----------------------------------------------------------------------------------------------------
template<>
inline VOID TString<CHAR>::Assign(CONST TString<CHAR>& S)
{
  m_Str = S.m_Str;
  m_Length = S.m_Length;
}
//----------------------------------------------------------------------------------------------------
template<>
inline VOID TString<CHAR>::Assign(CONST CHAR* S, SIZE_T Length)
{
  if (S == 0 || Length == 0)
  {
    m_Length = 0;
    m_Str.Free();
    return;
  }

  m_Str.Resize(Length+1);
  CopyObjects(m_Str.GetBuffer(), S, Length);
  m_Str[Length] = 0;
  m_Length = Length;
}
//----------------------------------------------------------------------------------------------------
template<>
inline VOID TString<CHAR>::Assign(CHAR C)
{
  m_Str.Resize(2);
  m_Str[0] = C;
  m_Str[1] = 0;
  m_Length = 1;
}
//----------------------------------------------------------------------------------------------------
template<>
inline VOID TString<CHAR>::Assign(CONST CHAR* S)
{
  return Assign(S, CharTraits<CHAR>::Length(S));
}
//----------------------------------------------------------------------------------------------------
#if defined(USE_UNICODE)
//----------------------------------------------------------------------------------------------------
template<>
inline VOID TString<WCHAR>::Assign(CONST WCHAR* S, SIZE_T Length)
{
  if (S == 0 || Length == 0)
    return;

  m_Str.Resize(Length+1);
  CopyObjects(m_Str.GetBuffer(), S, Length);
  m_Str[Length] = 0;
  m_Length = Length;
}
//----------------------------------------------------------------------------------------------------
template<>
inline VOID TString<WCHAR>::Assign(WCHAR C)
{
  m_Str.Resize(2);
  m_Str[0] = C;
  m_Str[1] = 0;
  m_Length = 1;
}
//----------------------------------------------------------------------------------------------------
template<>
inline VOID TString<WCHAR>::Assign(CONST TString<WCHAR>& S)
{
  m_Str = S.m_Str;
  m_Length = S.m_Length;
}
//----------------------------------------------------------------------------------------------------
template<>
inline VOID TString<CHAR>::Assign(CONST WCHAR* S)
{
  if (S == 0)
    return;

  TString<WCHAR> Str(S);
  Assign(Str.ToStringA());
}
//----------------------------------------------------------------------------------------------------
template<>
inline VOID TString<CHAR>::Assign(CONST WCHAR* S, SIZE_T Length)
{
  if (S == 0)
    return;

  TString<WCHAR> Str(S, Length);
  Assign(Str.ToStringA());
}
//----------------------------------------------------------------------------------------------------
template<>
inline VOID TString<CHAR>::Assign(WCHAR Ch)
{
  TString<WCHAR> Str(1);
  Str(0) = Ch;
  Assign(Str.ToStringA());
}
//----------------------------------------------------------------------------------------------------
template<>
inline VOID TString<WCHAR>::Assign(CONST TString<CHAR>& Str)
{
  Assign(Str.ToStringW());
}
//----------------------------------------------------------------------------------------------------
template<>
inline VOID TString<WCHAR>::Assign(CONST CHAR* S)
{
  if (S == 0)
    return;

  TString<CHAR> Str(S);
  Assign(Str.ToStringW());
}
//----------------------------------------------------------------------------------------------------
template<>
inline VOID TString<WCHAR>::Assign(CONST CHAR* S, SIZE_T Length)
{
  if (S == 0)
    return;

  TString<CHAR> Str(S,Length);
  Assign(Str.ToStringW());
}
//----------------------------------------------------------------------------------------------------
template<>
inline VOID TString<WCHAR>::Assign(CHAR Ch)
{
  TString<CHAR> Str(1);
  Str(0) = Ch;
  Assign(Str.ToStringW());
}
//----------------------------------------------------------------------------------------------------
template<>
inline VOID TString<WCHAR>::Assign(CONST WCHAR* S)
{
  return Assign(S, CharTraits<WCHAR>::Length(S));
}
//----------------------------------------------------------------------------------------------------
#endif
template<>
inline VOID TString<CHAR>::Assign(CONST TString<WCHAR>& Str)
{
  Assign(Str.ToStringA());
}


//----------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::operator== (CONST CharType* pString) CONST
{
  SIZE_T Length = CharTraits<CharType>::Length(pString);
  if (Length != m_Length)
    return false;
  int c = CharTraits<CharType>::StrCmp(ConstStr(), pString, Length); 
  return (c == 0);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::operator!= (CONST CharType* pString) CONST
{
  SIZE_T Length = CharTraits<CharType>::Length(pString);
  if (Length != m_Length)
    return true;
  int c = CharTraits<CharType>::StrCmp(ConstStr(), pString, Length); 
  return (c != 0);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::operator<  (CONST CharType* pString) CONST
{
  SIZE_T Length = CharTraits<CharType>::Length(pString);
  if (Length > m_Length)
    Length = m_Length;
  int c = CharTraits<CharType>::StrCmp(ConstStr(), pString, Length); 
  return (c < 0);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::operator>  (CONST CharType* pString) CONST
{
  SIZE_T Length = CharTraits<CharType>::Length(pString);
  if (Length > m_Length)
    Length = m_Length;
  int c = CharTraits<CharType>::StrCmp(ConstStr(), pString, Length); 
  return (c > 0);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::operator<= (CONST CharType* pString) CONST
{
  SIZE_T Length = CharTraits<CharType>::Length(pString);
  if (Length > m_Length)
    Length = m_Length;
  int c = CharTraits<CharType>::StrCmp(ConstStr(), pString, Length); 
  return (c <= 0);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::operator>= (CONST CharType* pString) CONST
{
  SIZE_T Length = CharTraits<CharType>::Length(pString);
  if (Length > m_Length)
    Length = m_Length;
  int c = CharTraits<CharType>::StrCmp(ConstStr(), pString, Length); 
  return (c >= 0);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::operator== (CONST TString& S) CONST
{
  SIZE_T Length = S.Length();
  if (Length > m_Length)
    Length = m_Length;
  int c = CharTraits<CharType>::StrCmp(ConstStr(), S.ConstStr(), Length); 
  return (c == 0);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::operator!= (CONST TString& S) CONST
{
  SIZE_T Length = S.Length();
  if (Length > m_Length)
    Length = m_Length;
  int c = CharTraits<CharType>::StrCmp(ConstStr(), S.ConstStr(), Length); 
  return (c != 0);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::operator<  (CONST TString& S) CONST
{
  SIZE_T Length = S.Length();
  if (Length > m_Length)
    Length = m_Length;
  int c = CharTraits<CharType>::StrCmp(ConstStr(), S.ConstStr(), Length); 
  return (c < 0);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::operator>  (CONST TString& S) CONST
{
  SIZE_T Length = S.Length();
  if (Length > m_Length)
    Length = m_Length;
  int c = CharTraits<CharType>::StrCmp(ConstStr(), S.ConstStr(), Length); 
  return (c > 0);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::operator<= (CONST TString& S) CONST
{
  SIZE_T Length = S.Length();
  if (Length > m_Length)
    Length = m_Length;
  int c = CharTraits<CharType>::StrCmp(ConstStr(), S.ConstStr(), Length); 
  return (c <= 0);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::operator>= (CONST TString& S) CONST
{
  SIZE_T Length = S.Length();
  if (Length > m_Length)
    Length = m_Length;
  int c = CharTraits<CharType>::StrCmp(ConstStr(), S.ConstStr(), Length); 
  return (c >= 0);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
INT TString<CharType>::Compare(CONST TString& S, BOOL CaseSensitive) CONST
{
  SIZE_T Length = S.m_Length;
  if (Length > m_Length)
    Length = m_Length;

  if (CaseSensitive)
    return CharTraits<CharType>::StrCmp(ConstStr(), S.ConstStr(), Length);
  else
    return CharTraits<CharType>::StrICmp(ConstStr(), S.ConstStr(), Length);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
INT TString<CharType>::Compare(CONST CharType* S, BOOL CaseSensitive) CONST
{
  SIZE_T Length = CharTraits<CharType>::Length(S);
  if (Length > m_Length)
    Length = m_Length;

  if (CaseSensitive)
    return CharTraits<CharType>::StrCmp(ConstStr(), S, Length);
  else
    return CharTraits<CharType>::StrICmp(ConstStr(), S, Length);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType> TString<CharType>::Substr(SIZE_T Index, SIZE_T Length) CONST
{
  Assert(Index + Length <= m_Length);
  String s;
  s.m_Str.Allocate(Length+1);
  CopyObjects(s.Str(), m_Str.GetBuffer() + Index, Length);
  s.m_Length = Length;
  s.m_Str[Length] = 0;

  return s;
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType> TString<CharType>::Substr(SIZE_T Index) CONST
{
  SIZE_T Length = m_Length - Index;

  return Substr(Index, Length);
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType> TString<CharType>::Substr(CONST typename TString<CharType>::Iterator Begin, CONST typename TString<CharType>::Iterator End) CONST
{
  Assert(IsInString(Begin.m_pStr));
  Assert(IsInString(End.m_pStr));

  String s;
  SIZE_T Length = End.m_pStr - Begin.m_pStr;
  s.m_Str.Allocate(Length+1);
  CopyObjects(s.Str(), (CONST CharType*)Begin.m_pStr, Length);
  s.m_Length = Length;
  s.m_Str[Length] = 0;

  return s;
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType> TString<CharType>::Substr(CONST typename TString<CharType>::Iterator Begin) CONST
{
  return Substr(Begin, End());
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
Array< TString<CharType> > TString<CharType>::Split(CharType Sep) CONST
{
  Array<String> S;
  SIZE_T Start = 0;
  SIZE_T i = 0;

  for(i=0; i<m_Length; i++)
  {
    if (Sep == m_Str[i])
    {
      if (i-Start > 0)
      {
        String s;
        s.Assign(&m_Str[Start], i-Start);
        S.Add(s);
      }
      Start = i+1;
    }
  }

  if (i-Start > 0)
  {
    String s;
    s.Assign(&m_Str[Start], i-Start);
    S.Add(s);
  }

  return S;
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType> TString<CharType>::Left(SIZE_T N)
{
  Assert(N <= m_Length);
  String s;
  s.m_Str.Allocate(N+1);
  CopyObjects(s.Str(), ConstStr(), N);
  s.m_Length = N;
  s.m_Str[N] = 0;

  return s;
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType> TString<CharType>::Right(SIZE_T N)
{
  Assert(N <= m_Length);
  String s;
  s.m_Str.Allocate(N+1);
  CopyObjects(s.Str(), ConstStr() + m_Length - N, N);
  s.m_Length = N;
  s.m_Str[N] = 0;
  return s;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Iterator TString<CharType>::Find(CharType Needle) CONST
{
  return Find(Needle, Begin(), End());
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Iterator TString<CharType>::Find(CharType Needle, typename TString<CharType>::Iterator Begin, typename TString<CharType>::Iterator End) CONST
{
  Assert(IsInString(Begin.m_pStr));
  Assert(IsInString(End.m_pStr));

  while (Begin != End)
  {
    if (*Begin == Needle)
      return Begin;
    Begin++;
  }
  return End;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Iterator TString<CharType>::FindLast(CharType Needle) CONST
{
  return FindLast(Needle, Begin(), End());
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Iterator TString<CharType>::FindLast(CharType Needle, typename TString<CharType>::Iterator Begin, typename TString<CharType>::Iterator End) CONST
{
  Assert(IsInString(Begin.m_pStr));
  Assert(IsInString(End.m_pStr));

  typename TString<CharType>::Iterator Found = End;
  while (Begin != End)
  {
    if (*Begin == Needle)
      Found = Begin;
    Begin++;
  }
  return Found;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
Array<typename TString<CharType>::Iterator> TString<CharType>::FindAll(CharType Needle, typename TString<CharType>::Iterator Begin, typename TString<CharType>::Iterator End, BOOL Overlapping) CONST
{
  Assert(IsInString(Begin.m_pStr));
  Assert(IsInString(End.m_pStr));

  Array<typename TString<CharType>::Iterator> A;
  while (Begin != End)
  {
    if (*Begin == Needle)
      A.Add(Begin);
    Begin++;
  }	
  return A;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
Array<typename TString<CharType>::Iterator> TString<CharType>::FindAll(CharType Needle, BOOL Overlapping) CONST
{
  return FindAll(Needle, Begin(), End(), Overlapping);
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Iterator TString<CharType>::Find(CONST CharType* Needle) CONST
{
  return Find(Needle, Begin(), End());
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Iterator TString<CharType>::Find(CONST CharType* Needle, typename TString<CharType>::Iterator Begin, typename TString<CharType>::Iterator End) CONST
{
  Assert(IsInString(Begin.m_pStr));
  Assert(IsInString(End.m_pStr));

  CharType* Haystack = (CharType*)Begin.m_pStr;
  CharType* Found = CharTraits<CharType>::StrStr(Haystack, Needle);
  if (Found == NULL || Found >= End.m_pStr)
    return End;
  else
    return typename TString<CharType>::Iterator(Found);
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Iterator TString<CharType>::FindLast(CONST CharType* Needle) CONST
{
  return FindLast(Needle, Begin(), End());
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Iterator TString<CharType>::FindLast(CONST CharType* Needle, typename TString<CharType>::Iterator Begin, typename TString<CharType>::Iterator End) CONST
{
  Assert(IsInString(Begin.m_pStr));
  Assert(IsInString(End.m_pStr));

  CharType* Haystack = (CharType*)Begin.m_pStr;
  CharType* PrevFound = 0;
  CharType* Found = CharTraits<CharType>::StrStr(Haystack, Needle);
  while (Found != 0)
  {
    PrevFound = Found;
    Found = CharTraits<CharType>::StrStr(PrevFound+1, Needle);
  }
  if (PrevFound == 0 || PrevFound >= End.m_pStr)
    return End;
  else
    return typename TString<CharType>::Iterator(PrevFound);
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
Array<typename TString<CharType>::Iterator> TString<CharType>::FindAll(CONST CharType* Needle, typename TString<CharType>::Iterator Begin, typename TString<CharType>::Iterator End, BOOL Overlapping) CONST
{
  Assert(IsInString(Begin.m_pStr));
  Assert(IsInString(End.m_pStr));

  Array<Iterator> A;
  SIZE_T Length = CharTraits<CharType>::Length(Needle);
  CharType* Haystack = (CharType*)Begin.m_pStr;
  CharType* Found = CharTraits<CharType>::StrStr(Haystack, Needle);
  while (Found != 0)
  {
    A.Add(TString<CharType>::Iterator(Found));
    if (Overlapping)
      Found = CharTraits<CharType>::StrStr(Found+1, Needle);
    else
      Found = CharTraits<CharType>::StrStr(Found+Length, Needle);
  }
  return A;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
Array<typename TString<CharType>::Iterator> TString<CharType>::FindAll(CONST CharType* Needle, BOOL Overlapping) CONST
{
  return FindAll(Needle, Begin(), End(), Overlapping);
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
VOID TString<CharType>::Erase(Iterator Begin, Iterator End)
{
  Assert(IsInString(Begin.m_pStr));
  Assert(IsInString(End.m_pStr));
  Assert(Begin.m_pStr <= End.m_pStr);

  SIZE_T Length = End.m_pStr - Begin.m_pStr;
  if (Length == 0)
    return;

  SIZE_T CopyLength = m_Length-(End.m_pStr-m_Str);
  memmove(Begin.m_pStr, End.m_pStr, CopyLength);
  m_Length -= Length;
  if (m_Length == 0)
    m_Str.Free();  
  else
  {
    m_Str[m_Length] = 0;
    m_Str.Resize(m_Length + 1);
  }
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
VOID TString<CharType>::Insert(Iterator Begin, CONST CharType* pStr)
{
  Assert(IsInString(Begin.m_pStr));

  SIZE_T Length = CharTraits<CharType>::Length(pStr);
  SIZE_T Pos = Begin.m_pStr-m_Str;
  m_Str.Resize(m_Length + Length + 1);
  Buffer<CharType>::DataCopy(m_Str+Pos+Length, m_Str+Pos, m_Length-Pos+1);
  Buffer<CharType>::DataCopy(m_Str+Pos, pStr, Length);
  m_Length += Length;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
VOID TString<CharType>::Insert(Iterator Begin, CONST TString& S)
{
  Assert(IsInString(Begin.m_pStr));

  SIZE_T Length = S.m_Length;
  SIZE_T Pos = Begin.m_pStr-m_Str;
  m_Str.Resize(m_Length + Length + 1);
  Buffer<CharType>::DataCopy(m_Str+Pos+Length, m_Str+Pos, m_Length-Pos+1);
  Buffer<CharType>::DataCopy(m_Str+Pos, S.m_Str, Length);
  m_Length += Length;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
VOID TString<CharType>::Erase(Iterator Begin)
{
  Erase(Begin, End());
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::BeginsWith(CONST CharType* S, BOOL CaseSensitive) CONST
{
  SIZE_T Length = CharTraits<CharType>::Length(S);
  if (Length > m_Length)
    return false;
  if (CaseSensitive)
    return CharTraits<CharType>::StrCmp(ConstStr(), S, Length) == 0;
  else
    return CharTraits<CharType>::StrICmp(ConstStr(), S, Length) == 0;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::BeginsWith(CONST TString& S, BOOL CaseSensitive) CONST
{
  if (!S.IsEmpty())
    return BeginsWith(ConstStr(), CaseSensitive);
  else
    return false;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::EndsWith(CONST CharType* S, BOOL CaseSensitive) CONST
{
  SIZE_T Length = CharTraits<CharType>::Length(S);
  if (Length > m_Length)
    return false;
  if (CaseSensitive)
    return CharTraits<CharType>::StrCmp(&m_Str[m_Length-Length],S,Length) == 0;
  else
    return CharTraits<CharType>::StrICmp(&m_Str[m_Length-Length],S,Length) == 0;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::EndsWith(CONST TString& S, BOOL CaseSensitive) CONST
{
  if (!S.IsEmpty())
    return EndsWith(S.m_Str,CaseSensitive);
  else
    return false;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
VOID TString<CharType>::Trim()
{
  TrimLeft();
  TrimRight();
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
VOID TString<CharType>::TrimLeft()
{
  if (m_Length == 0)
    return;

  INT i = 0;
  for(i=0; i<(INT)m_Length; i++)
    if (!IsWhitespace(m_Str[i]))
      break;

  memmove(&m_Str[0], &m_Str[i], m_Length-i);
  m_Length -= i;

}
//---------------------------------------------------------------------------------------------------
template<class CharType>
VOID TString<CharType>::TrimRight()
{
  if (m_Length == 0)
    return;

  INT i = (INT)(m_Length-1);
  for(i=(INT)m_Length-1; i >= 0; i--)
    if (!IsWhitespace(m_Str[i]))
      break;
  m_Length = i+1;
  m_Str[m_Length] = 0;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
VOID TString<CharType>::ToUpper()
{
  for (SIZE_T i=0; i<m_Length; i++)
    m_Str[i] = CharTraits<CharType>::ToUpper(m_Str[i]);
}
//---------------------------------------------------------------------------------------------------

template<class CharType>
VOID TString<CharType>::ToLower()
{
  for (SIZE_T i=0; i<m_Length; i++)
    m_Str[i] = CharTraits<CharType>::ToLower(m_Str[i]);
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::IsAlpha(CharType C)
{
  if (C >= 'A' && C <= 'Z')
    return true;
  if (C >= 'a' && C <= 'z')
    return true;
  return false;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::IsNumeric(CharType C)
{
  if (C >= '0' && C <= '9')
    return true;
  return false;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::IsWhitespace(CharType C)
{
  if (C <= ' ')
    return true;
  return false;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::IsPunctuation(CharType C)
{
  CONST CharType* S = ";:.,\"\'`";
  return false;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::IsAlphaNumeric(CharType C)
{
  return IsAlpha(C) || IsNumeric(C);
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::IsUpperCase(CharType C)
{
  if (C >= 'A' && C <= 'Z')
    return true;
  return false;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::IsLowerCase(CharType C)
{
  if (C >= 'a' && C <= 'z')
    return true;
  return false;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType> TString<CharType>::operator + (CONST TString& S) CONST
{
  String s(*this);
  s.Append(S);  
  return s;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>& TString<CharType>::operator += (CONST TString& S)
{
  Append(S);  
  return *this;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType> TString<CharType>::operator + (CONST CharType* S) CONST
{
  String s(*this);
  s.Append(S);  
  return s;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>& TString<CharType>::operator += (CONST CharType* S)
{
  Append(S);
  return *this;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType> TString<CharType>::operator + (CharType C) CONST
{
  String s(*this);
  s.Append(C);  
  return s;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>& TString<CharType>::operator += (CharType C)
{
  Append(C);  
  return *this;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType> operator+ (LPCSTR A, TString<CharType> B)
{
  TString<CharType> S(A);
  S.Append(B);
  return S;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType> operator+ (LPCWSTR A, TString<CharType> B)
{
  TString<CharType> S(A);
  S.Append(B);
  return S;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
VOID TString<CharType>::Append(CONST TString& S)
{
  if (m_Length == 0)
    Assign(S);
  else
  {
    if (S.m_Length == 0)
      return;
    m_Str.Resize(m_Length + S.m_Length + 1);
    CopyObjects(m_Str.GetBuffer() + m_Length, S.ConstStr(), S.m_Length + 1);
    m_Length += S.m_Length;
  }
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
VOID TString<CharType>::Append(CONST CharType* S)
{
  if (S == 0)
    return;

  if (m_Length == 0)
    Assign(S);
  else
  {
    SIZE_T Length = CharTraits<CharType>::Length(S);
    if (Length == 0)
      return;
    m_Str.Resize(m_Length + Length + 1);
    CopyObjects(m_Str.GetBuffer() + m_Length, S, Length + 1);
    m_Length += Length;
  }
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
VOID TString<CharType>::Append(CharType C)
{
  if (m_Length == 0)
    Assign(C);
  else
  {
    m_Str.Resize(m_Length + 2);
    m_Str[m_Length] = C;
    m_Str[m_Length+1] = 0;
    m_Length += 1;
  }
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
CONST typename TString<CharType>::Iterator TString<CharType>::Begin() CONST
{
  return typename TString<CharType>::Iterator(const_cast<CharType*>(m_Str.GetBuffer()));
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
CONST typename TString<CharType>::Iterator TString<CharType>::End() CONST
{
  return typename TString<CharType>::Iterator(const_cast<CharType*>(m_Str.GetBuffer() + m_Length));
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Iterator TString<CharType>::Begin() 
{
  return typename TString<CharType>::Iterator(const_cast<CharType*>(m_Str.GetBuffer()));
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Iterator TString<CharType>::End()
{
  return typename TString<CharType>::Iterator(const_cast<CharType*>(m_Str.GetBuffer() + m_Length));
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
CONST typename TString<CharType>::Enumerator TString<CharType>::Characters() CONST
{
  return Enumerator(const_cast<String*>(this),0);
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Enumerator TString<CharType>::Characters()
{
  return Enumerator(this,0);
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>::Iterator::Iterator()
{
  m_pStr = 0;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>::Iterator::Iterator(CharType* pStr)
{
  m_pStr = pStr;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>::Iterator::Iterator(CONST TString& S, SIZE_T i)
{
  m_pStr = (CharType*)&S.ConstStr()[i];
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::Iterator::operator== (CONST Iterator& S) CONST
{
  return m_pStr == S.m_pStr;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::Iterator::operator!= (CONST Iterator& S) CONST
{
  return m_pStr != S.m_pStr;
}
#ifdef DEBUG
//---------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::IsInString(CONST CharType* pStr) CONST
{
  return pStr >= m_Str.GetBuffer() && pStr < (m_Str.GetBuffer() + m_Length + 1);
}
#endif
//---------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>::Enumerator::Enumerator(TString* pString, INT Index)
{
  m_pString = pString;
  m_Index = Index;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType>::Enumerator::operator BOOL()
{ 
  return HasMoreElements(); 
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Enumerator TString<CharType>::Enumerator::operator ++ ()
{
  return Enumerator(m_pString, m_Index+1);
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Enumerator& TString<CharType>::Enumerator::operator++ (int)
{
  m_Index++;
  return *this;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Enumerator TString<CharType>::Enumerator::operator -- ()
{
  return Enumerator(m_pString, m_Index-1);
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Enumerator& TString<CharType>::Enumerator::operator-- (int)
{
  m_Index--;
  return *this;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Enumerator TString<CharType>::Enumerator::operator  +  (int N) CONST
{
  return Enumerator(m_pString, m_Index + N);
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Enumerator& TString<CharType>::Enumerator::operator += (int N)
{
  m_Index += N;
  return *this;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Enumerator TString<CharType>::Enumerator::operator  -  (int N) CONST
{
  return Enumerator(m_pString, m_Index - N);
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Enumerator& TString<CharType>::Enumerator::operator -= (int N)
{
  m_Index -= N;
  return *this;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
INT TString<CharType>::Enumerator::operator - (CONST typename TString<CharType>::Enumerator& E) CONST
{
  Assert(m_pString == E.m_pString);
  return m_Index - E.m_Index;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::Enumerator::operator== (CONST typename TString<CharType>::Enumerator& E) CONST
{
  return m_pString == E.m_pString && m_Index == E.m_Index;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::Enumerator::operator!= (CONST typename TString<CharType>::Enumerator& E) CONST
{
  return m_pString != E.m_pString || m_Index != E.m_Index;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::Enumerator::operator<  (CONST typename TString<CharType>::Enumerator& E) CONST
{
  return m_pString == E.m_pString && m_Index < E.m_Index;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::Enumerator::operator>  (CONST typename TString<CharType>::Enumerator& E) CONST
{
  return m_pString == E.m_pString && m_Index > E.m_Index;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::Enumerator::operator<= (CONST typename TString<CharType>::Enumerator& E) CONST
{
  return m_pString == E.m_pString && m_Index <= E.m_Index;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::Enumerator::operator>= (CONST typename TString<CharType>::Enumerator& E) CONST
{
  return m_pString == E.m_pString && m_Index >= E.m_Index;
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TString<CharType>::Enumerator::HasMoreElements()
{
  return m_Index >= 0 && m_Index < (INT)m_pString->Length();
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
CONST CharType& TString<CharType>::Enumerator::CurrentElement() CONST
{
  return m_pString->Get(m_Index);
}
//---------------------------------------------------------------------------------------------------
template<class CharType>
CharType& TString<CharType>::Enumerator::CurrentElement()
{
  return m_pString->Get(m_Index);
}
//---------------------------------------------------------------------------------------------------

/*
ImplementStringConvert(INT8, "%d");
ImplementStringConvert(UINT8, "%u");
ImplementStringConvert(INT16, "%d");
ImplementStringConvert(UINT16, "%u");
ImplementStringConvert(INT32, "%d");
ImplementStringConvert(UINT32, "%u");
ImplementStringConvert(LONG, "%d");
ImplementStringConvert(ULONG, "%u");
ImplementStringConvert(INT64, "%I64d");
ImplementStringConvert(UINT64, "%I64u");
ImplementStringConvert(FLOAT, "%f");
ImplementStringConvert(DOUBLE, "%lf");
ImplementStringConvert(CharType, "%c");
*/

//---------------------------------------------------------------------------------------------------
#if defined(PLATFORM_WINDOWS)
template<class CharType>
TString<CharType> TString<CharType>::LoadFromResource(UINT Id)
{
  String S;
  INT Length = LoadString(NULL, Id, 0, 0);
  S.Resize(Length);
  LoadString(NULL, Id, &S[0], Length);

  return S;
}
//----------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType> TString<CharType>::LoadFromResource(LPCTSTR Type, LPCTSTR Name)
{
  HRSRC hResInfo = FindResource(NULL, Name, Type);
  if (hResInfo == NULL)
    return TString<CharType>();

  HGLOBAL hResource = LoadResource(NULL, hResInfo);
  if (hResource == NULL)
    return TString<CharType>();

  LPVOID pResource = LockResource(hResource);
  DWORD Size = SizeofResource(NULL, hResInfo);

  return TString<CharType>((LPCTSTR)pResource, (SIZE_T)Size);
}
#endif
//----------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------
template<class CharType>
CONST CharType& TString<CharType>::Iterator::operator* () CONST
{
  return m_pStr[0];
}
//-----------------------------------------------------------------------------------------------------
template<class CharType>
CharType& TString<CharType>::Iterator::operator* ()
{
  return m_pStr[0];
}
//-----------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Iterator TString<CharType>::Iterator::operator++ ()
{
  Iterator i(*this);
  m_pStr++;

  return i;
}
//-----------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Iterator& TString<CharType>::Iterator::operator++ (int)
{
  m_pStr++;
  return *this;
}
//-----------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Iterator TString<CharType>::Iterator::operator + (SIZE_T N) CONST
{
  return Iterator(m_pStr + N);
}
//-----------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Iterator& TString<CharType>::Iterator::operator += (SIZE_T N)
{
  m_pStr += N;
  return *this;
}
//-----------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Iterator TString<CharType>::Iterator::operator - (SIZE_T N) CONST
{
  return Iterator(m_pStr - N);
}
//-----------------------------------------------------------------------------------------------------
template<class CharType>
typename TString<CharType>::Iterator& TString<CharType>::Iterator::operator -= (SIZE_T N)
{
  m_pStr -= N;
  return *this;
}
//-----------------------------------------------------------------------------------------------------
template<class CharType>
template<class Predicate>
Array< TString<CharType> > TString<CharType>::Split(Predicate P) CONST
{
  Array<String> S;
  SIZE_T Start = 0;
  SIZE_T i = 0;

  for(i=0; i<m_Length; i++)
  {
    if (P(m_Str[i]))
    {
      if (i-Start > 0)
        S.Add(Substr(Start, i-Start));
      Start = i+1;
    }
  }
  if (i-Start > 0)
    S.Add(Substr(Start, i-Start));

  return S;
}
//-----------------------------------------------------------------------------------------------------
template<class T>
T ParseSize(LPCTSTR From)
{
  T Value = T(0);
  
  while (*From != 0)
  {
    if (String::IsNumeric(*From))
      Value = (Value * 10) + (*From - '0');
    else
      break;
    From++;
  }

  if (*From != 0)
  {
    if (*From == 'K')
      Value *= 1024;
    else if (*From == 'M')
      Value *= 1048576;
    else if (*From == 'G')
      Value *= 1073741824;
  } 

  return Value;
}
//-----------------------------------------------------------------------------------------------------
template<class T>
T ParseHex(LPCTSTR From)
{
  LPCTSTR HexDigits = "0123456789ABCDEF";

  T Value = T(0);
  
  while (*From != 0)
  {
    TCHAR Ch = CharTraits<TCHAR>::ToUpper(*From);

    for (UINT i=0; i<16; i++)
    {
      if (Ch == HexDigits[i])
      {
        Value = (Value * 16) + i;
        From++;
        continue;
      }
    }
    break;
  }

  return Value;
}
//-----------------------------------------------------------------------------------------------------
