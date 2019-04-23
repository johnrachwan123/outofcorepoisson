/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/IO/BinaryStream.cpp $
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
#include "Memory/Memory.hpp"

using namespace Bolitho;
using namespace Bolitho::IO;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::IO,BinaryStream,Stream);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream::BinaryStream()
{
  m_pBaseStream = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream::~BinaryStream()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BinaryStream::Flush()
{
  if (m_pBaseStream)
    m_pBaseStream->Flush();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BinaryStream::Close()
{
  if (m_pBaseStream)
    m_pBaseStream->Close();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T BinaryStream::Write(LPCVOID pData, SIZE_T N)
{
  if (m_pBaseStream)
    return m_pBaseStream->Write(pData, N);
  else
    return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T BinaryStream::Read(LPVOID pData, SIZE_T N)
{
  if (m_pBaseStream)
    return m_pBaseStream->Read(pData, N);
  else
    return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL BinaryStream::EndOfStream()
{
  if (m_pBaseStream)
    return m_pBaseStream->EndOfStream();
  else
    return true;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BinaryStream::Write(CONST String& x)
{
  SIZE_T Length = x.Length();
  Write((UINT64)Length);
  Write((LPCBYTE)x.ConstStr(), sizeof(TCHAR)*Length);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BinaryStream::Write(TCHAR x)
{
  Write(&x, sizeof(TCHAR));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BinaryStream::Write(UINT8 x)
{
  Write(&x, sizeof(UINT8));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BinaryStream::Write(INT8 x)
{
  Write(&x, sizeof(INT8));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BinaryStream::Write(UINT16 x)
{
  #if defined(PLATFORM_BIGENDIAN)
  SwapByteOrder(&x);
  #endif
  Write(&x, sizeof(UINT16));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BinaryStream::Write(INT16 x)
{
  #if defined(PLATFORM_BIGENDIAN)
  SwapByteOrder(&x);
  #endif
  Write(&x, sizeof(INT16));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BinaryStream::Write(UINT32 x)
{
  #if defined(PLATFORM_BIGENDIAN)
  SwapByteOrder(&x);
  #endif
  Write(&x, sizeof(UINT32));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BinaryStream::Write(INT32 x)
{
  #if defined(PLATFORM_BIGENDIAN)
  SwapByteOrder(&x);
  #endif
  Write(&x, sizeof(INT32));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BinaryStream::Write(UINT64 x)
{
  #if defined(PLATFORM_BIGENDIAN)
  SwapByteOrder(&x);
  #endif
  Write(&x, sizeof(UINT64));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BinaryStream::Write(INT64 x)
{
  #if defined(PLATFORM_BIGENDIAN)
  SwapByteOrder(&x);
  #endif
  Write(&x, sizeof(INT64));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BinaryStream::Write(FLOAT x)
{
  #if defined(PLATFORM_BIGENDIAN)
  SwapByteOrder(&x);
  #endif
  Write(&x, sizeof(FLOAT));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BinaryStream::Write(DOUBLE x)
{
  #if defined(PLATFORM_BIGENDIAN)
  SwapByteOrder(&x);
  #endif
  Write(&x, sizeof(DOUBLE));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String BinaryStream::ReadString()
{
  String x;
  UINT64 Length = ReadUInt64();
  x.Resize((SIZE_T)Length);
  Read(x.Str(), sizeof(TCHAR)*(SIZE_T)Length);
  return x;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
TCHAR BinaryStream::ReadChar()
{
  TCHAR x;
  Read(&x, sizeof(TCHAR));
  return x;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
UINT8 BinaryStream::ReadUInt8()
{
  UINT8 x;
  Read(&x, sizeof(UINT8));
  return x;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
INT8 BinaryStream::ReadInt8()
{
  INT8 x;
  Read(&x, sizeof(INT8));
  return x;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
UINT16 BinaryStream::ReadUInt16()
{
  UINT16 x;
  Read(&x, sizeof(UINT16));

  #if defined(PLATFORM_BIGENDIAN)
  SwapByteOrder(&x);
  #endif
  return x;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
INT16 BinaryStream::ReadInt16()
{
  INT16 x;
  Read(&x, sizeof(INT16));

  #if defined(PLATFORM_BIGENDIAN)
  SwapByteOrder(&x);
  #endif
  return x;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
UINT32 BinaryStream::ReadUInt32()
{
  UINT32 x;
  Read(&x, sizeof(UINT32));

  #if defined(PLATFORM_BIGENDIAN)
  SwapByteOrder(&x);
  #endif
  return x;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
INT32 BinaryStream::ReadInt32()
{
  INT32 x;
  Read(&x, sizeof(INT32));

  #if defined(PLATFORM_BIGENDIAN)
  SwapByteOrder(&x);
  #endif
  return x;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
UINT64 BinaryStream::ReadUInt64()
{
  UINT64 x;
  Read(&x, sizeof(UINT64));

  #if defined(PLATFORM_BIGENDIAN)
  SwapByteOrder(&x);
  #endif
  return x;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
INT64 BinaryStream::ReadInt64()
{
  INT64 x;
  Read(&x, sizeof(INT64));

  #if defined(PLATFORM_BIGENDIAN)
  SwapByteOrder(&x);
  #endif
  return x;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT BinaryStream::ReadFloat()
{
  FLOAT x;
  Read(&x, sizeof(FLOAT));
  #if defined(PLATFORM_BIGENDIAN)
  SwapByteOrder(&x);
  #endif
  return x;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
DOUBLE BinaryStream::ReadDouble()
{
  DOUBLE x;
  Read(&x, sizeof(DOUBLE));
  #if defined(PLATFORM_BIGENDIAN)
  SwapByteOrder(&x);
  #endif
  return x;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T BinaryStream::ReadSize()
{
  SIZE_T x;
  Read(&x, sizeof(SIZE_T));
  #if defined(PLATFORM_BIGENDIAN)
  SwapByteOrder(&x);
  #endif
  return x;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator << (BinaryStream& S, CONST String& x)
{
  S.Write(x);
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator << (BinaryStream& S, TCHAR x)
{
  S.Write(x);
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator << (BinaryStream& S, UINT8 x)
{
  S.Write(x);
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator << (BinaryStream& S, INT8 x)
{
  S.Write(x);
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator << (BinaryStream& S, UINT16 x)
{
  S.Write(x);
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator << (BinaryStream& S, INT16 x)
{
  S.Write(x);
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator << (BinaryStream& S, UINT32 x)
{
  S.Write(x);
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator << (BinaryStream& S, INT32 x)
{
  S.Write(x);
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator << (BinaryStream& S, UINT64 x)
{
  S.Write(x);
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator << (BinaryStream& S, INT64 x)
{
  S.Write(x);
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator << (BinaryStream& S, FLOAT x)
{
  S.Write(x);
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator << (BinaryStream& S, DOUBLE x)
{
  S.Write(x);
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_WINDOWS) && defined(PLATFORM_32BIT)
BinaryStream& Bolitho::IO::operator << (BinaryStream& S, SIZE_T x)
{
  S.Write(x);
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator >> (BinaryStream& S, SIZE_T& x)
{
  x = S.ReadSize();
  return S;
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator >> (BinaryStream& S, String& x)
{
  x = S.ReadString();
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator >> (BinaryStream& S, TCHAR& x)
{
  x = S.ReadChar();
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator >> (BinaryStream& S, UINT8& x)
{
  x = S.ReadUInt8();
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator >> (BinaryStream& S, INT8& x)
{
  x = S.ReadInt8();
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator >> (BinaryStream& S, UINT16& x)
{
  x = S.ReadUInt16();
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator >> (BinaryStream& S, INT16& x)
{
  x = S.ReadInt16();
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator >> (BinaryStream& S, UINT32& x)
{
  x = S.ReadUInt32();
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator >> (BinaryStream& S, INT32& x)
{
  x = S.ReadInt32();
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator >> (BinaryStream& S, UINT64& x)
{
  x = S.ReadUInt64();
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator >> (BinaryStream& S, INT64& x)
{
  x = S.ReadInt64();
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator >> (BinaryStream& S, FLOAT& x)
{
  x = S.ReadFloat();
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BinaryStream& Bolitho::IO::operator >> (BinaryStream& S, DOUBLE& x)
{
  x = S.ReadDouble();
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
