/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/IO/MemoryStream.cpp $
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





#include "CodeLibrary.hpp"

#include "IO/MemoryStream.hpp"

using namespace Bolitho::IO;
using namespace Bolitho;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::IO,MemoryStream,Stream);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
MemoryStream::MemoryStream() : m_Buffer(m_InternalBuffer)
{
  m_BufferOffset = 0;
  m_BufferLength = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
MemoryStream::MemoryStream(Buffer<BYTE>& B) : m_Buffer(B)
{
  m_BufferOffset = 0;
  m_BufferLength = B.Length();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
MemoryStream::~MemoryStream()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MemoryStream::Flush()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MemoryStream::Close()
{
  m_Buffer.Free();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T MemoryStream::Write(LPCVOID pData, SIZE_T N)
{
  if (N == 0)
    return 0;

  if (m_Buffer.Length() < m_BufferOffset + N)
    if (m_Buffer.Length() == 0)
      m_Buffer.Resize(65536);
    else
      m_Buffer.Resize(2*m_Buffer.Length());

  CopyObjects(&m_Buffer[m_BufferOffset], (LPCBYTE)pData, N);
  
  m_BufferOffset += N;
  if (m_BufferOffset > m_BufferLength)
    m_BufferLength = m_BufferOffset;

  return N;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T MemoryStream::Read(LPVOID pData, SIZE_T N)
{
  if (m_Buffer.Length() < m_BufferOffset + N)
    N = m_Buffer.Length() - m_BufferOffset;
  CopyObjects((LPBYTE)pData, &m_Buffer[m_BufferOffset], N);
  m_BufferOffset += N;
  return N;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG MemoryStream::Length()
{
  return m_BufferLength;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG MemoryStream::Position()
{
  return m_BufferOffset;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG MemoryStream::Seek(ULONGLONG NewPosition)
{
  m_BufferOffset = (SIZE_T)NewPosition;
  return NewPosition;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL MemoryStream::EndOfStream()
{
  return m_BufferOffset > m_BufferLength;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MemoryStream::Clear()
{
  m_Buffer.Resize(0);
  m_BufferOffset = 0;
  m_BufferLength = 0;
}

