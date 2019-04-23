/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Text/String.hpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1708 $
 * Last Updated: $Date: 2008-11-12 18:35:04 -0800 (Wed, 12 Nov 2008) $
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

#pragma once

#include "Utility/Traits.hpp"
#include "Utility/Pair.hpp"
#include "Text/CharTraits.hpp"
#include "Memory/Buffer.hpp"
#include "Collections/Array.hpp"
#include "Debug/Assert.hpp"

namespace Bolitho
{
  template<typename CharType>
  class TString : public Object
  {
    StaticAssert(Traits<CharType>::IsCharacter);

    DeclareRuntimeType();
  public:
    // iterator class

    class Iterator
    {
      template<typename CharType> friend class TString;
    public:
      // CONSTruction
      Iterator();
      explicit Iterator(CharType* pStr);
      Iterator(CONST TString& S, SIZE_T i);

      // implicit conversions
      inline CONST CharType& operator* () CONST;
      inline CharType& operator* ();

      // pointer aritmatic
      inline Iterator operator++ ();
      inline Iterator& operator++ (int);

      inline Iterator operator + (SIZE_T N) CONST;
      inline Iterator& operator += (SIZE_T N);

      inline Iterator operator - (SIZE_T N) CONST;
      inline Iterator& operator -= (SIZE_T N);

      // comparison operators
      BOOL operator== (CONST Iterator& S) CONST;
      BOOL operator!= (CONST Iterator& S) CONST;

    protected:
      CharType* m_pStr;
    };

    class Enumerator
    {
      template<class CharType> friend class TString;
    protected:
      Enumerator(TString* pString, INT Index);
    public:
      inline operator BOOL();

      inline Enumerator operator ++ ();
      inline Enumerator& operator++ (int);

      inline Enumerator operator -- ();
      inline Enumerator& operator-- (int);

      inline Enumerator operator  +  (int N) CONST;
      inline Enumerator& operator += (int N);

      inline Enumerator operator  -  (int N) CONST;
      inline Enumerator& operator -= (int N);

      inline int operator - (CONST Enumerator& E) CONST;

      // comparison operators
      BOOL operator== (CONST Enumerator& E) CONST;
      BOOL operator!= (CONST Enumerator& E) CONST;
      BOOL operator<  (CONST Enumerator& E) CONST;
      BOOL operator>  (CONST Enumerator& E) CONST;
      BOOL operator<= (CONST Enumerator& E) CONST;
      BOOL operator>= (CONST Enumerator& E) CONST;

      BOOL HasMoreElements();
      CONST CharType& CurrentElement() CONST;
      CharType& CurrentElement();

    protected:
      TString* m_pString;
      INT m_Index;

    };

  public:
    /* Constructs an empty string */
    TString();
    /* Constructs an empty string, reserving space for N characters */ 
    explicit TString(SIZE_T N);

    /* Constructs a string as a copy of a single byte character string */
    TString(CONST CHAR* pStr);
    /* Constructs a string as a copy of a single byte character string up to N characters long */
    TString(CONST CHAR* pStr, SIZE_T N);
    /* Constructs a string as a copy of a single byte character string */
    TString(CONST TString<CHAR>& Str);

    #if defined(USE_UNICODE)
    /* Constructs a string as a copy of a wide character string */
    TString(CONST WCHAR* pStr);
    /* Constructs a string as a copy of a wide character string up to N characters long*/
    TString(CONST WCHAR* pStr, SIZE_T N);
    /* Constructs a string as a copy of a wide character string */
    TString(CONST TString<WCHAR>& Str);
    #endif

    /* Constructs a string of N repeating characters */
    TString(CharType C, SIZE_T N);

    /* Constructs a string by concatenating an array of strings with a given separator */
    explicit TString(CONST Array<TString>& A, CONST CharType* Sep);
    /* Constructs a string from a character buffer */
    explicit TString(CONST Buffer<CharType>& B);
    /* Constructs a string from a byte buffer */
    explicit TString(CONST Buffer<BYTE>& B);

    virtual ~TString();

    /* Returns the length of the string */
    INLINE SIZE_T Length() CONST
    { return m_Length; }
    /* Returns TRUE if the string is empty */
    INLINE BOOL IsEmpty() CONST
    { return m_Length == 0; }

    /* Frees the string.  The string onbject becomes 'empty' */
    VOID Free();
    /* Reserves N-characters for string data storage */
    VOID Reserve(SIZE_T N);
    /* Creates an N-character string */
    VOID Resize(SIZE_T N);

    /* Returns the Buffer used to store the string data */
    INLINE Buffer<CharType>& GetBuffer()
    { return m_Str; }
    /* Returns the Buffer used to store the string data */
    INLINE CONST Buffer<CharType>& GetBuffer() CONST
    { return m_Str; }

    /* Returns a character pointer to the string */
    INLINE CharType* Str()
    { return m_Str.GetBuffer(); }
    /* Returns a character pointer to the string */
    INLINE CONST CharType* ConstStr() CONST
    { return m_Str.GetBuffer(); }

    /* Returns a character pointer to the string */
    INLINE operator CONST CharType* () CONST
    { return m_Str.GetBuffer(); }
    /* Returns a character pointer to the string */
    INLINE operator CharType* ()
    { return m_Str.GetBuffer(); }

    /* Returns a reference to the i-th character in the string */
    INLINE CONST CharType& operator() (SIZE_T i) CONST
    { return Get(i); }
    /* Returns a reference to the i-th character in the string */
    INLINE CharType& operator() (SIZE_T i)
    { return Get(i); }

    /* Returns a reference to the i-th character in the string */
    INLINE CONST CharType& Get(SIZE_T i) CONST
    { Assert(i < m_Length); return m_Str[i]; }
    /* Returns a reference to the i-th character in the string */
    INLINE CharType& Get(SIZE_T i)
    { Assert(i < m_Length); return m_Str[i]; }

    /* Returns a reference to the i-th element of the array */
    INLINE CharType& Get(PTRDIFF_T i)
    { if (i >= 0) { Assert(i < m_Length); return m_Str[i]; } else { Assert(i > -m_Length); return m_Str[m_Length-i]; } }
    /* Returns a reference to the i-th element of the array */
    INLINE CONST CharType& Get(PTRDIFF_T i) CONST
    { if (i >= 0) { Assert(i < m_Length); return m_Str[i]; } else { Assert(i > -m_Length); return m_Str[m_Length-i]; } }

    /* Copy from a narrow character string */
    TString& operator= (CONST TString<CHAR>& S);
    /* Copy from a narrow character string */
    TString& operator= (CONST CHAR* S);
    /* Copy from a narrow character */
    TString& operator= (CHAR C);

    #if defined(USE_UNICODE)
    /* Copy from a wide character string */
    TString& operator= (CONST TString<WCHAR>& Str);
    /* Copy from a wide character string */
    TString& operator= (CONST WCHAR* pStr);
    /* Copy from a wide character */
    TString& operator= (WCHAR Ch);
    #endif

    /* Copy from a narrow character string */
    VOID Assign(CONST TString<CHAR>& Str);
    /* Copy from a narrow character string */
    VOID Assign(CONST CHAR* pStr);
    /* Copy from a narrow character string of up to N characters*/
    VOID Assign(CONST CHAR* pStr, SIZE_T N);
    /* Copy from a narrow character */
    VOID Assign(CHAR Ch);

    /* Copy from a wide character string */
    VOID Assign(CONST TString<WCHAR>& Str);
    /* Copy from a wide character string */
    VOID Assign(CONST WCHAR* pStr);
    /* Copy from a wide character string of up to N characters*/
    VOID Assign(CONST WCHAR* pStr, SIZE_T N);
    /* Copy from a wide character */
    VOID Assign(WCHAR Ch);

    /* Compare the string for lexical equality */
    BOOL operator== (CONST CharType* S) CONST;
    /* Compare the string for lexical inequality */
    BOOL operator!= (CONST CharType* S) CONST;
    /* Compare the string for lexical equality */
    BOOL operator<  (CONST CharType* S) CONST;
    /* Compare the string for lexical equality */
    BOOL operator>  (CONST CharType* S) CONST;
    /* Compare the string for lexical equality */
    BOOL operator<= (CONST CharType* S) CONST;
    /* Compare the string for lexical equality */
    BOOL operator>= (CONST CharType* S) CONST;

    /* Compare the string for lexical equality */
    BOOL operator== (CONST TString& S) CONST;
    /* Compare the string for lexical inequality */
    BOOL operator!= (CONST TString& S) CONST;
    /* Compare the string for lexical equality */
    BOOL operator<  (CONST TString& S) CONST;
    /* Compare the string for lexical equality */
    BOOL operator>  (CONST TString& S) CONST;
    /* Compare the string for lexical equality */
    BOOL operator<= (CONST TString& S) CONST;
    /* Compare the string for lexical equality */
    BOOL operator>= (CONST TString& S) CONST;

    /* Compare the string for lexical equality.  Case sensitivity can be controlled.  
    * Returns 0 is the string a lexically equal */
    INT Compare(CONST TString& S, BOOL CaseSensitive = TRUE) CONST;
    /* Compare the string for lexical equality.  Case sensitivity can be controlled.  
    * Returns 0 is the string a lexically equal */
    INT Compare(CONST CharType* S, BOOL CaseSensitive = TRUE) CONST;

    /* Extracts a substring starting at a given character index, for some length */
    TString Substr(SIZE_T Index, SIZE_T Length) CONST;
    /* Extracts a substring starting at a given character index for the rest of the string */
    TString Substr(SIZE_T Index) CONST;

    /* Extracts a substring starting at a given iterator up to (not including) an ending iterator */
    TString Substr(CONST Iterator Begin, CONST Iterator End) CONST;
    /* Extracts a substring starting at a given character index for the rest of the string */
    TString Substr(CONST Iterator Begin) CONST;

    /* Returns the first N characters in the string */	
    TString Left(SIZE_T N);
    /* Returns the last N characters in the string */
    TString Right(SIZE_T N);

    VOID Erase(Iterator Begin, Iterator End);
    VOID Erase(Iterator Begin);

    /* Inserts another string at a specific location */
    VOID Insert(Iterator Begin, CONST CharType* S);
    /* Inserts another string at a specific location */
    VOID Insert(Iterator Begin, CONST TString& S);

    /* Splits the string into tokens separated by a given character */
    Array<TString> Split(CharType Sep) CONST;
    /* Splits the string into tokens separated by a given predicate */
    template<class Predicate>
    Array<TString> Split(Predicate P) CONST;

    /* Returns TRUE if the string begins with the specified prefix */
    BOOL BeginsWith(CONST CharType* S, BOOL CaseSensitive = TRUE) CONST;
    /* Returns TRUE if the string begins with the specified prefix */
    BOOL BeginsWith(CONST TString& S, BOOL CaseSensitive = TRUE) CONST;

    /* Returns TRUE if the string ends with the specified suffix */
    BOOL EndsWith(CONST CharType* S, BOOL CaseSensitive = TRUE) CONST;
    /* Returns TRUE if the string ends with the specified suffix */
    BOOL EndsWith(CONST TString& S, BOOL CaseSensitive = TRUE) CONST;

    // append, concatenation
    TString operator + (CONST TString& S) CONST;
    TString& operator += (CONST TString& S);

    TString operator + (CONST CharType* S) CONST;
    TString& operator += (CONST CharType* S);

    TString operator + (CharType C) CONST;
    TString& operator += (CharType C);

    VOID Append(CONST TString& S);
    VOID Append(CONST CharType* S);
    VOID Append(CharType C);

#ifdef DEBUG
    // sanity / debug operations
    BOOL IsInString(CONST CharType* pStr) CONST;
#endif

    /* Finds the first instance of a string within a given character range */
    Iterator Find(CONST CharType* Find, Iterator Begin, Iterator End) CONST;
    /* Finds the first instance of a string within the whole string */
    Iterator Find(CONST CharType* Find) CONST;

    /* Finds the last instance of a string within a given character range */
    Iterator FindLast(CONST CharType* Find, Iterator Begin, Iterator End) CONST;
    /* Finds the last instance of a string within the whole string */
    Iterator FindLast(CONST CharType* Find) CONST;

    /* Finds all instances of a string within a given character range */
    Array<Iterator> FindAll(CONST CharType* Find, Iterator Begin, Iterator End, BOOL Overlapping = TRUE) CONST;
    /* Finds all instances of a string within the whole string */
    Array<Iterator> FindAll(CONST CharType* Find, BOOL Overlapping = TRUE) CONST;

    Iterator Find(CharType Find, Iterator Begin, Iterator End) CONST;
    Iterator Find(CharType Find) CONST;

    Iterator FindLast(CharType Find, Iterator Begin, Iterator End) CONST;
    Iterator FindLast(CharType Find) CONST;

    Array<Iterator> FindAll(CharType Find, Iterator Begin, Iterator End, BOOL Overlapping = TRUE) CONST;
    Array<Iterator> FindAll(CharType Find, BOOL Overlapping = TRUE) CONST;

    // Array<Iterator> FindAll(Iterator Begin, Iterator End, CONST CharType* Find);
    // Array<RegularExpressionMatch> Match(CONST CharType* RegularExpression)

    /* Constructs a string from a printf-style format operation */
    STATIC TString Format(CONST CharType* Format, ...);
    /* Constructs a string from a printf-style format operation */
    STATIC TString FormatV(CONST CharType* Format, va_list Args);

    /* Formats a byte quanitity as a string with units.  The units are chosen automatically by the size of the value */
    STATIC TString FormatSize(ULONGLONG Bytes);
    /* Formats a byte quantity as a string with units.  The units are always KB */
    STATIC TString FormatSizeKB(ULONGLONG Bytes);
    /* Formats a time quantity (in nanoseconds) as a string with uints.  The units are chosen automatically by the size of the value */
    STATIC TString FormatTimeInterval(ULONGLONG NanoSeconds, DWORD Verbosity = 0);
    /* Formats a time quantity (in seconds) as a string with uints.  The units are chosen automatically by the size of the value */
    STATIC TString FormatTimeInterval(DWORD Seconds, DWORD Verbosity = 0);
    /* Formats a time quantity (in seconds) as a string with uints.  The units are chosen automatically by the size of the value */
    STATIC TString FormatTimeInterval(FLOAT Seconds, DWORD Verbosity = 0);

    /* Returns a copy of the string */
    virtual TString<TCHAR> ToString() CONST;
    /* Returns a copy of the string as a narrow character string */
    TString<CHAR> ToStringA() CONST;
    
    #if defined(USE_UNICODE)
    /* Returns a copy of the string as a wide character string */
    TString<WCHAR> ToStringW() CONST;
    #endif

    /* Returns a numeric value computed from the string suitable for use in a hash table */
    DWORD GetHash() CONST;

    /* Returns TRUE if the character is a character in the latin alphabet */
    STATIC BOOL IsAlpha(CharType);
    /* Returns TRUE if the character is a numeral */
    STATIC BOOL IsNumeric(CharType);
    /* Returns TRUE if the character is whitespace */
    STATIC BOOL IsWhitespace(CharType);
    /* Returns TRUE if the character is a punctuation mark */
    STATIC BOOL IsPunctuation(CharType);
    /* Returns TRUE if the character IsAlpha or IsNumeric */
    STATIC BOOL IsAlphaNumeric(CharType);
    /* Returns TRUE if the character is an upper case character in the latin alphabet */
    STATIC BOOL IsUpperCase(CharType);
    /* Returns TRUE if the character is a lower case character in the latin alphabet */
    STATIC BOOL IsLowerCase(CharType);

    /* Removes leading and trailing whitespace from the string */
    VOID Trim();
    /* Removes leading whitespace from the string */
    VOID TrimLeft();
    /* Removes trailing whitespace fropm the string */
    VOID TrimRight();

    /* Converts all characters in the latin alphabet to uppercase */
    VOID ToUpper();
    /* Converts all characters in the latin alphabet to lower case */
    VOID ToLower();

    /* Returns an iterator refering to the first character in the string */
    Iterator Begin();
    /* Returns an iterator refering to the first character in the string */
    CONST Iterator Begin() CONST;
    /* Returns an iterator refering to the end of the string */
    Iterator End();
    /* Returns an iterator refering to the end of the string */
    CONST Iterator End() CONST;

    /* Returns an enumerator of all characters in the string */
    CONST Enumerator Characters() CONST;
    /* Returns an enumerator of all characters in the string */
    Enumerator Characters();

    #if defined(PLATFORM_WINDOWS)
    /* Constructs a string from a value in a resource table */
    STATIC TString LoadFromResource(UINT Id);
    /* Constructs a string from a value in a resource table */
    STATIC TString LoadFromResource(LPCTSTR Type, LPCTSTR Name);
    #endif

    // Some common strings
    static LIBRARY_API CONST TString Empty;
    static LIBRARY_API CONST TString NewLine;

    #if defined(USE_ZLIB)
    /* Compresses the string data and returns the compressed data */
    Buffer<BYTE> Compress() CONST
    { return m_Str.Compress(); }

    /* Decompresses binary data as a string */
    VOID Decompress(CONST Buffer<BYTE>& Data)
    { m_Str.Decompress(Data); m_Length = m_Str.Length(); }
    #endif

  private:
    /* The length of the string */
    SIZE_T m_Length;
    /* The memory buffer storing the string */
    Buffer<CharType> m_Str;
  };

  template<class CharType>
  TString<CharType> operator+ (LPCSTR A, TString<CharType> B);
  template<class CharType>
  TString<CharType> operator+ (LPCWSTR A, TString<CharType> B);
  
  #include "StringImpl.hpp"

  typedef Pair<String,String> StringPair;
  typedef Array<String> StringArray;

  String ToString(LPVOID pData, SIZE_T Length);

  template<class T>
  T Parse(CONST CHAR*);

  template<class T>
  T Parse(CONST WCHAR*);

  template<>
  INLINE DWORD Traits<StringA>::Hash(CONST StringA& S)
  { return S.GetHash(); }

  #if defined(USE_UNICODE)
  template<>
  INLINE DWORD Traits<StringW>::Hash(CONST StringW& S)
  { return S.GetHash(); }
  #endif

  template<class T>
  inline String ToString(CONST T& From)
  { return From.ToString(); }

  template<>
  INLINE String ToString(CONST TString<CHAR>& From)
  { return String(From); }

  #if defined(USE_UNICODE)
  template<>
  INLINE String ToString(CONST TString<WCHAR>& From)
  { return String(From); }

  INLINE String ToString(CONST WCHAR* From)
  { return String(From); }

  template<>
  INLINE TString<WCHAR> Parse(LPCSTR Str)
  { return TString<WCHAR>(Str); }

  template<>
  INLINE TString<WCHAR> Parse(LPCWSTR Str)
  { return TString<CHAR>(Str); }

  template<>
  INLINE TString<CHAR> Parse(LPCWSTR Str)
  { return TString<CHAR>(Str); }
  #endif

  INLINE String ToString(CONST CHAR* From)
  { return String(From); }

  template<>
  INLINE TString<CHAR> Parse(LPCSTR Str)
  { return TString<CHAR>(Str); }


  #define DeclareStringFormatConvert(f,afmt)                                                        \
    template<>                                                                                      \
    INLINE String ToString(CONST f& From)                                                           \
    { return String::Format(TEXT(afmt) ,From); }                                                    \


  #define DeclareStringParseConvert(t,afmt,wfmt)                                                    \
    template<>                                                                                      \
    INLINE t Parse(CONST CHAR* From)                                                                \
    { t To; Verify(sscanf(From, afmt, &To) == 1); return To; }                                      \
    template<>                                                                                      \
    INLINE t Parse(CONST WCHAR* From)                                                               \
    { t To; Verify(swscanf(From, L##wfmt, &To) == 1); return To; }                                  \


  #if defined(UNICODE)
  #define DeclareStringConvert(t,afmt)                                                            \
    DeclareStringFormatConvert(t,afmt);                                                           \
    DeclareStringParseConvert(t,afmt,afmt);                                                       
  #else
  #define DeclareStringConvert(t,afmt)                                                            \
    DeclareStringFormatConvert(t,afmt);                                                           \
    DeclareStringParseConvert(t,afmt,afmt);                                                       
  #endif

  DeclareStringConvert(bool, "%d");
  DeclareStringConvert(INT8, "%hc");
  DeclareStringConvert(UINT8, "%hc");
  DeclareStringConvert(SHORT, "%hd");
  DeclareStringConvert(USHORT, "%hu");
  DeclareStringConvert(INT, "%d");
  DeclareStringConvert(UINT, "%u");
  DeclareStringConvert(LONG, "%ld");
  DeclareStringConvert(ULONG, "%lu");
  DeclareStringConvert(LONGLONG, "%lld");
  DeclareStringConvert(ULONGLONG, "%llu");
  DeclareStringConvert(FLOAT, "%g");
  DeclareStringConvert(DOUBLE, "%lg");
  DeclareStringConvert(CHAR, "%c");
  #if defined(UNICODE)
  DeclareStringConvert(WCHAR, "%C");
  #endif

  template<class T>
  T ParseSize(LPCTSTR From);

  template<class T>
  T ParseHex(LPCTSTR From);
}

