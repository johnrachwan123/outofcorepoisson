/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/IO/TextStream.hpp $
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

#pragma once

#include "IO/Stream.hpp"
#include "Memory/Pointer.hpp"

namespace Bolitho
{
  namespace IO
  {
    template<class CharType>
    class TTextStream : public Stream
    {
      DeclareRuntimeType();
    public:
      TTextStream();
      TTextStream(Ptr<Stream> BaseStream);
      virtual ~TTextStream();
      
      TTextStream& operator= (Ptr<Stream> BaseStream);

      virtual VOID Flush();
      virtual VOID Close();
      
      virtual SIZE_T Write(LPCVOID pData, SIZE_T N);
      virtual SIZE_T Read(LPVOID pData, SIZE_T N);
      
      virtual BOOL EndOfStream();
      
      virtual BOOL CanRead() 
      { return m_pStream->CanRead(); }
      virtual BOOL CanWrite()
      { return m_pStream->CanWrite(); }
      virtual BOOL CanSeek()
      { return m_pStream->CanSeek(); }

      virtual ULONGLONG Position()
      { return m_pStream->Position(); }
      virtual ULONGLONG Seek(ULONGLONG NewPosition)
      { return m_pStream->Seek(NewPosition); }
      virtual ULONGLONG Length()
      { return m_pStream->Length(); }

      VOID Write(CONST CharType* Format, ...);
      template<class T>
      VOID Write(CONST T& x);
      VOID Write(CONST TString<CharType>& x);
      
      VOID WriteLine(CONST CharType* Format, ...);
      template<class T>
      VOID WriteLine(CONST T& x);
      VOID WriteLine(CONST TString<CharType>& x);
      
      TString<CharType> ReadLine();
    protected:
      Ptr<Stream> m_pStream;
    };
    
#include "TextStreamImpl.hpp"
    
    typedef TTextStream<TCHAR> TextStream;
    typedef TTextStream<CHAR> TextStreamA;
    
#if defined(USE_UNICODE)
    typedef TTextStream<WCHAR> TextStreamW;
#endif
    
  }

	template<class CharType, class T>
  IO::TTextStream<CharType>& operator << (IO::TTextStream<CharType>& Stream, CONST T& x)
  {
    Stream.Write(ToString(x));
    return Stream;
  }

  template<class CharType>
  inline IO::TTextStream<CharType>& operator << (IO::TTextStream<CharType>& Stream, CONST Bolitho::TString<CharType>& Str)
  { 
    Stream.Write(Str.ConstStr());
    return Stream; 
  }

  template<class CharType>
  inline IO::TTextStream<CharType>& operator << (IO::TTextStream<CharType>& Stream, CONST CharType* Str)
  { 
    Stream.Write(Str);
    return Stream; 
  }
}

