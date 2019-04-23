/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/IO/Stream.cpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1630 $
 * Last Updated: $Date: 2008-09-14 05:21:32 -0700 (Sun, 14 Sep 2008) $
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

using namespace Bolitho;
using namespace Bolitho::IO;
//------------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::IO,Stream,Object);
//------------------------------------------------------------------------------------------------------------------------------------------------------
/*
ULONGLONG Stream::WriteFromStream(Ptr<Stream> pStream, ULONGLONG N)
{
  CONST SIZE_T BlockSize = 1048576;
  BYTE* pBuffer = new BYTE[BlockSize];

  for (ULONGLONG i=0; i<N; i+=BlockSize)
  {
    SIZE_T CurrentBlockSize = (N-i) < BlockSize ? (SIZE_T)(N-i) : BlockSize;
    pStream->Read(pBuffer, CurrentBlockSize);
    Write(pBuffer, CurrentBlockSize);
  } 

  delete[] pBuffer;
  return N;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG Stream::ReadToStream(Ptr<Stream> pStream, ULONGLONG N)
{
  CONST SIZE_T BlockSize = 1048576;
  BYTE* pBuffer = new BYTE[BlockSize];

  for (ULONGLONG i=0; i<N; i+=BlockSize)
  {
    SIZE_T CurrentBlockSize = (N-i) < BlockSize ? (SIZE_T)(N-i) : BlockSize;
    Read(pBuffer, CurrentBlockSize);
    pStream->Write(pBuffer, CurrentBlockSize);
  } 

  delete[] pBuffer;
  return N;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Stream::WriteFromMemory(LPCVOID pSource, SIZE_T N)
{
  CONST SIZE_T BlockSize = 1048576;
  LPCBYTE pBuffer = (LPCBYTE)pSource;

  for (SIZE_T i=0; i<N; i+=BlockSize)
  {
    SIZE_T CurrentBlockSize = (N-i) < BlockSize ? (SIZE_T)(N-i) : BlockSize;
    Write(pBuffer, CurrentBlockSize);
    pBuffer += CurrentBlockSize;
  } 

  return N;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Stream::ReadToMemory(LPVOID pDestination, SIZE_T N)
{
  CONST SIZE_T BlockSize = 1048576;
  LPBYTE pBuffer = (LPBYTE)pDestination;

  for (SIZE_T i=0; i<N; i+=BlockSize)
  {
    SIZE_T CurrentBlockSize = (N-i) < BlockSize ? (SIZE_T)(N-i) : BlockSize;
    Read(pBuffer, CurrentBlockSize);
    pBuffer += CurrentBlockSize;
  } 

  return N;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
*/

//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG Stream::Copy(Ptr<Stream> pTo, Ptr<Stream> pFrom, ULONGLONG N, Progress* pProgress)
{
  CONST SIZE_T BlockSize = 1048576;
  BYTE* pBuffer = new BYTE[BlockSize];

  for (ULONGLONG i=0; i<N; i+=BlockSize)
  {
    SIZE_T CurrentBlockSize = (N-i) < BlockSize ? (SIZE_T)(N-i) : BlockSize;
    pFrom->Read(pBuffer, CurrentBlockSize);
    pTo->Write(pBuffer, CurrentBlockSize);
  } 

  delete[] pBuffer;
  return N;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Stream::Copy(Ptr<Stream> pTo, LPCVOID pFrom, SIZE_T N, Progress* pProgress)
{
  CONST SIZE_T BlockSize = 1048576;
  LPCBYTE pBuffer = (LPCBYTE)pFrom;

  for (SIZE_T i=0; i<N; i+=BlockSize)
  {
    SIZE_T CurrentBlockSize = (N-i) < BlockSize ? (SIZE_T)(N-i) : BlockSize;
    pTo->Write(pBuffer, CurrentBlockSize);
    pBuffer += CurrentBlockSize;
  } 

  return N;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Stream::Copy(LPVOID pTo, Ptr<Stream> pFrom, SIZE_T N, Progress* pProgress)
{
  CONST SIZE_T BlockSize = 1048576;
  LPBYTE pBuffer = (LPBYTE)pTo;

  for (SIZE_T i=0; i<N; i+=BlockSize)
  {
    SIZE_T CurrentBlockSize = (N-i) < BlockSize ? (SIZE_T)(N-i) : BlockSize;
    pFrom->Read(pBuffer, CurrentBlockSize);
    pBuffer += CurrentBlockSize;
  } 

  return N;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------

/*
#include "Utility/Progress.hpp"
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG Stream::WriteFromStreamWithProgress(Ptr<Stream> pStream, ULONGLONG N)
{
  pProgress->Initialize(0, N, "Bytes");
  CONST SIZE_T BlockSize = 1048576;
  BYTE* pBuffer = new BYTE[BlockSize];

  for (ULONGLONG i=0; i<N; i+=BlockSize)
  {
    SIZE_T CurrentBlockSize = (N-i) < BlockSize ? (SIZE_T)(N-i) : BlockSize;
    pStream->Read(pBuffer, CurrentBlockSize);
    Write(pBuffer, CurrentBlockSize);
    pProgress->Update(i);
  } 

  delete[] pBuffer;
  return N;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG Stream::ReadToStreamWithProgress(Ptr<Stream> pStream, ULONGLONG N)
{
  pProgress->Initialize(0, N, "Bytes");
  CONST SIZE_T BlockSize = 1048576;
  BYTE* pBuffer = new BYTE[BlockSize];

  for (ULONGLONG i=0; i<N; i+=BlockSize)
  {
    SIZE_T CurrentBlockSize = (N-i) < BlockSize ? (SIZE_T)(N-i) : BlockSize;
    Read(pBuffer, CurrentBlockSize);
    pStream->Write(pBuffer, CurrentBlockSize);
    pProgress->Update(i);
  } 

  delete[] pBuffer;
  return N;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Stream::WriteFromMemoryWithProgress(LPCVOID pSource, SIZE_T N)
{
  pProgress->Initialize(0, N, "Bytes");
  CONST SIZE_T BlockSize = 1048576;
  LPCBYTE pBuffer = (LPCBYTE)pSource;

  for (SIZE_T i=0; i<N; i+=BlockSize)
  {
    SIZE_T CurrentBlockSize = (N-i) < BlockSize ? (SIZE_T)(N-i) : BlockSize;
    Write(pBuffer, CurrentBlockSize);
    pBuffer += CurrentBlockSize;
    pProgress->Update(i);
  } 

  return N;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Stream::ReadToMemoryWithProgress(LPVOID pDestination, SIZE_T N)
{
  pProgress->Initialize(0, N, "Bytes");
  CONST SIZE_T BlockSize = 1048576;
  LPBYTE pBuffer = (LPBYTE)pDestination;

  for (SIZE_T i=0; i<N; i+=BlockSize)
  {
    SIZE_T CurrentBlockSize = (N-i) < BlockSize ? (SIZE_T)(N-i) : BlockSize;
    Read(pBuffer, CurrentBlockSize);
    pBuffer += CurrentBlockSize;
    pProgress->Update(i);
  } 

  return N;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
*/

//------------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::IO,NullStream,Stream);
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID NullStream::Flush()
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID NullStream::Close()
{
}
//-----------------------------------------------------------------------------------------------------------------------------
SIZE_T NullStream::Write(LPCVOID pData, SIZE_T N)
{
	return N;
}
//-----------------------------------------------------------------------------------------------------------------------------
SIZE_T NullStream::Read(LPVOID pData, SIZE_T N)
{
	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------
BOOL NullStream::EndOfStream()
{
	return true;
}
