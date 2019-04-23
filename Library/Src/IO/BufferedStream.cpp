/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Trunk/Src/IO/DebugStream.cpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1588 $
 * Last Updated: $Date: 2008-08-23 12:36:02 -0400 (Sat, 23 Aug 2008) $
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
#include "IO/BufferedStream.hpp"

using namespace Bolitho;
using namespace Bolitho::IO;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::IO,BufferedStream,Stream);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BufferedStream::BufferedStream(Ptr<String> pStream) : m_pStream(pStream)
{
  m_BufferMode = BufferMode::NONE;
  m_BufferLength = 0;
  m_BufferOffset = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BufferedStream::Flush()
{
  FlushBuffer();
  m_pStream->Flush();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BufferedStream::FlushBuffer()
{
  if (m_BufferMode == BufferMode::WRITE)
  {
    m_pStream->Write(m_Buffer.GetBuffer(), m_BufferOffset);
    m_BufferMode = BufferMode::NONE;
    m_BufferOffset = 0;
    m_BufferLength = 0;
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BufferedStream::Close()
{
  FlushBuffer();
  m_pStream->Close();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T BufferedStream::Write(LPCVOID pData, SIZE_T N)
{
  if (N == 0)
    return 0;

  SIZE_T TotalLength = 0;

//  if (m_BufferMode == BufferMode::READ)
//    Seek(m_CurrentPosition);

  if (m_Buffer.Length() == 0)
    return m_pStream->Write(pData, N);

  /* Set buffer to write mode */
  if (m_BufferMode != BufferMode::WRITE)
  {
    m_BufferMode = BufferMode::WRITE;
    m_BufferLength = 0;
    m_BufferOffset = 0;
  }

  /* If there is insufficient space in the buffer, flush it */
  if (N > m_Buffer.Length() - m_BufferOffset)
  {
    m_pStream->Write(&m_Buffer[0], m_BufferOffset);
    m_BufferOffset = 0;
  }
  
  if (N > m_Buffer.Length())
  {
    m_pStream->Write(pData, N);
  }
  else
  {
    CopyMemory(&m_Buffer[m_BufferOffset], pData, N);
    m_BufferOffset += N;
  }

//  m_CurrentPosition += N;

  return N;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T BufferedStream::Read(LPVOID pData, SIZE_T N)
{
  if (N == 0)
    return 0;

  SIZE_T TotalLength = 0;

  /* If the Buffer has unwritten data, flush it. */
  FlushBuffer();

  /* Fill the buffer, if needed */
  if (m_BufferMode == BufferMode::NONE && N < m_Buffer.Length())
  {
    m_BufferLength = m_pStream->Read(&m_Buffer[0], m_Buffer.Length());
    m_BufferOffset = 0;
    m_BufferMode = BufferMode::READ;
  }

  /* Exhaust data in the buffer first */
  if (m_BufferMode == BufferMode::READ)
  {
    SIZE_T Length = N;
    if (Length > (m_BufferLength - m_BufferOffset))
      Length = m_BufferLength - m_BufferOffset;

    CopyMemory(pData, &m_Buffer[m_BufferOffset], Length);
    pData = (LPBYTE)pData + Length;
    N -= Length;
    TotalLength += Length;
    m_BufferOffset += Length;
    if (m_BufferOffset == m_BufferLength)
      m_BufferMode = BufferMode::NONE;
  }
  
  /* Then, complete data from stream */
  if (N > 0)
    TotalLength += m_pStream->Read(pData, N);
  
  return TotalLength;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL BufferedStream::EndOfStream()
{
  return m_BufferLength == 0 && m_pStream->EndOfStream();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
