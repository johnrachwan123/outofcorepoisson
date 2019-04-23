/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Memory/BufferImpl.hpp $
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




//------------------------------------------------------------------------------------------------------------------------------
ImplementTemplate2RuntimeType(Bolitho,Buffer,Object);
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
Buffer<T,Allocator>::Buffer()
{
  m_pData = 0;
  m_Length = 0;
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
Buffer<T,Allocator>::Buffer(SIZE_T N)
{
  m_pData = 0;
  m_Length = 0;
  Allocate(N);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
Buffer<T,Allocator>::Buffer(CONST T* pData, SIZE_T N)
{
  m_pData = 0;
  m_Length = 0;

  Allocate(N);
  CopyObjects(m_pData, pData, N); 
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
Buffer<T,Allocator>::Buffer(CONST Buffer& B)
{
  m_pData = 0;
  m_Length = 0;

  (*this) = B;
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
Buffer<T,Allocator>::Buffer(Allocator& A) : m_Allocator(A)
{
  m_pData = 0;
  m_Length = 0;
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
Buffer<T,Allocator>::Buffer(SIZE_T N, Allocator& A) : m_Allocator(A)
{
  m_pData = 0;
  m_Length = 0;

  Allocate(N);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
Buffer<T,Allocator>::Buffer(CONST T* pData, SIZE_T N, Allocator& A) : m_Allocator(A)
{
  m_pData = 0;
  m_Length = 0;

  Allocate(N);
  CopyObjects(m_pData, pData, N); 
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
Buffer<T,Allocator>::Buffer(CONST Buffer& B, Allocator& A) : m_Allocator(A)
{
  m_pData = 0;
  m_Length = 0;

  (*this) = B;
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
Buffer<T,Allocator>::~Buffer()
{
  Free();
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
VOID Buffer<T,Allocator>::Allocate(SIZE_T N)
{
  if (m_pData)
    m_Allocator.Delete(m_pData, m_Length);
  
  if (N > 0)
    m_pData = m_Allocator.New(AllocateSize(N));
  else
    m_pData = 0;
  m_Length = N;
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
VOID Buffer<T,Allocator>::Resize(SIZE_T N)
{
  SIZE_T NewLength = AllocateSize(N);
  T* pData = m_Allocator.New(NewLength);
  CopyObjects(pData, m_pData, Min(m_Length, NewLength));
  m_Allocator.Delete(m_pData, m_Length);
  m_pData = pData;
  m_Length = NewLength;
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
VOID Buffer<T,Allocator>::Free()
{
  if (m_pData)
    m_Allocator.Delete(m_pData, m_Length);
  m_pData = 0;
  m_Length = 0;
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
Buffer<T,Allocator>& Buffer<T,Allocator>::operator= (CONST Buffer& B)
{
  Allocate(B.Length());
  CopyObjects(m_pData, B.m_pData, m_Length);
  return *this;
}

//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
BOOL Buffer<T,Allocator>::operator== (T* pData) CONST
{
  return (m_pData == pData);
}

//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
BOOL Buffer<T,Allocator>::operator!= (T* pData) CONST
{
  return (m_pData != pData);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
BOOL Buffer<T,Allocator>::operator== (CONST Buffer& B) CONST
{
  return (m_pData == B.m_pData);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
BOOL Buffer<T,Allocator>::operator!= (CONST Buffer& B) CONST
{
  return (m_pData != B.m_pData);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
inline SIZE_T Buffer<T,Allocator>::AllocateSize(SIZE_T N)
{
  if (N == 0)
    return 0;

  CONST SIZE_T Align = 64;
  return (Align*N + Align - 1) / Align;
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
VOID Buffer<T,Allocator>::Write(Ptr<IO::Stream> pStream) CONST
{
  pStream->Write(m_pData, sizeof(T)*m_Length);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
VOID Buffer<T,Allocator>::Read(Ptr<IO::Stream> pStream)
{
  pStream->Read(m_pData, sizeof(T)*m_Length);
}

#if defined(USE_ZLIB)
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
Buffer<BYTE> Buffer<T,Allocator>::Compress(DWORD Type) CONST
{
  return CompressData(m_pData, sizeof(T)*m_Length, Type);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
VOID Buffer<T,Allocator>::Decompress(DWORD Type, CONST Buffer<BYTE>& Source)
{
  return DecompressData(m_pData, sizeof(T)*m_Length, Type);
}
//------------------------------------------------------------------------------------------------------------------------------
#endif

//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
UINT32 Buffer<T,Allocator>::ComputeCrc32() CONST
{
  Crc32 Checksum;
  Checksum.Update(m_pData, sizeof(T)*m_Length);
  return Checksum.Value();
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
Md5Hash Buffer<T,Allocator>::ComputeMd5() CONST
{
  Md5 Checksum;
  Checksum.Update(m_pData, sizeof(T)*m_Length);
  return Checksum.Value();
}
//------------------------------------------------------------------------------------------------------------------------------

