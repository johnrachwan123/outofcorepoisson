/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/IO/BinaryStream.hpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1627 $
 * Last Updated: $Date: 2008-09-14 05:01:15 -0700 (Sun, 14 Sep 2008) $
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
    class LIBRARY_API BinaryStream : public Stream
    {
      DeclareRuntimeType();
    public:
      BinaryStream();
      virtual ~BinaryStream();

      template<class T>
      BinaryStream(Ptr<T> BaseStream);
      template<class T>
      BinaryStream& operator= (Ptr<T> BaseStream);

      virtual VOID Flush();
      virtual VOID Close();

      virtual SIZE_T Write(LPCVOID pData, SIZE_T N);
      virtual SIZE_T Read(LPVOID pData, SIZE_T N);

      virtual BOOL EndOfStream();

      virtual BOOL CanRead()
      { return m_pBaseStream->CanRead(); }
      virtual BOOL CanWrite()
      { return m_pBaseStream->CanWrite(); }
      virtual BOOL CanSeek()
      { return m_pBaseStream->CanSeek(); }

      virtual ULONGLONG Position()
      { return m_pBaseStream->Position(); }
      virtual ULONGLONG Seek(ULONGLONG NewPosition)
      { return m_pBaseStream->Seek(NewPosition); }
      virtual ULONGLONG Length()
      { return m_pBaseStream->Length(); }
      
      VOID Write(CONST String& S);
      VOID Write(CONST Object& x);
      VOID Write(TCHAR x);
      VOID Write(UINT8 x);
      VOID Write(INT8 x);
      VOID Write(UINT16 x);
      VOID Write(INT16 x);
      VOID Write(UINT32 x);
      VOID Write(INT32 x);
      VOID Write(UINT64 x);
      VOID Write(INT64 x);
      VOID Write(FLOAT x);
      VOID Write(DOUBLE x);

      #if defined(PLATFORM_WINDOWS)
      VOID Write(ULONG x)
      { Write((UINT32)x); }
      #endif

      VOID Read(Object& x);
      TCHAR ReadChar();
      UINT8 ReadUInt8();
      INT8 ReadInt8();
      UINT16 ReadUInt16();
      INT16 ReadInt16();
      UINT32 ReadUInt32();
      INT32 ReadInt32();
      UINT64 ReadUInt64();
      INT64 ReadInt64();
      FLOAT ReadFloat();
      DOUBLE ReadDouble();
      String ReadString();
      SIZE_T ReadSize();

    private:
      Ptr<Stream> m_pBaseStream;

    };

    BinaryStream& operator << (BinaryStream& S, CONST String& x);
    BinaryStream& operator << (BinaryStream& S, TCHAR x);
    BinaryStream& operator << (BinaryStream& S, UINT8 x);
    BinaryStream& operator << (BinaryStream& S, INT8 x);
    BinaryStream& operator << (BinaryStream& S, UINT16 x);
    BinaryStream& operator << (BinaryStream& S, INT16 x);
    BinaryStream& operator << (BinaryStream& S, UINT32 x);
    BinaryStream& operator << (BinaryStream& S, INT32 x);
    BinaryStream& operator << (BinaryStream& S, UINT64 x);
    BinaryStream& operator << (BinaryStream& S, INT64 x);
    BinaryStream& operator << (BinaryStream& S, FLOAT x);
    BinaryStream& operator << (BinaryStream& S, DOUBLE x);

    #if defined(PLATFORM_WINDOWS) && defined(PLATFORM_32BIT)
    BinaryStream& operator << (BinaryStream& S, SIZE_T x);
    BinaryStream& operator >> (BinaryStream& S, SIZE_T& x);
    #endif
    
    BinaryStream& operator >> (BinaryStream& S, String& x);
    BinaryStream& operator >> (BinaryStream& S, TCHAR& x);
    BinaryStream& operator >> (BinaryStream& S, UINT8& x);
    BinaryStream& operator >> (BinaryStream& S, INT8& x);
    BinaryStream& operator >> (BinaryStream& S, UINT16& x);
    BinaryStream& operator >> (BinaryStream& S, INT16& x);
    BinaryStream& operator >> (BinaryStream& S, UINT32& x);
    BinaryStream& operator >> (BinaryStream& S, INT32& x);
    BinaryStream& operator >> (BinaryStream& S, UINT64& x);
    BinaryStream& operator >> (BinaryStream& S, INT64& x);
    BinaryStream& operator >> (BinaryStream& S, FLOAT& x);
    BinaryStream& operator >> (BinaryStream& S, DOUBLE& x);

    #include "BinaryStreamImpl.hpp"
  }
}

