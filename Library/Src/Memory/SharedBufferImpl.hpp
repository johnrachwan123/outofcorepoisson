/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Memory/SharedBufferImpl.hpp $
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

//------------------------------------------------------------------------------------------------------------------------------
ImplementTemplate2RuntimeType(Bolitho,SharedBuffer,Object);
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
SharedBuffer<T,Allocator>::SharedBuffer()
{
  m_pInfo = 0;
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
SharedBuffer<T,Allocator>::SharedBuffer(SIZE_T N)
{
  m_pInfo = 0;
  Allocate(N);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
SharedBuffer<T,Allocator>::SharedBuffer(CONST T* pData, SIZE_T N)
{
  m_pInfo = 0;
  Allocate(N);
  DataCopy(m_pInfo->pData, pData, N); 
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
SharedBuffer<T,Allocator>::SharedBuffer(CONST SharedBuffer& B)
{
  B.AddRef();
  m_pInfo = B.m_pInfo;
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
SharedBuffer<T,Allocator>::SharedBuffer(Allocator& A) : m_Allocator(A)
{
  m_pInfo = 0;
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
SharedBuffer<T,Allocator>::SharedBuffer(SIZE_T N, Allocator& A) : m_Allocator(A)
{
  m_pInfo = 0;
  Allocate(N);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
SharedBuffer<T,Allocator>::SharedBuffer(CONST T* pData, SIZE_T N, Allocator& A) : m_Allocator(A)
{
  m_pInfo = 0;
  Allocate(N);
  DataCopy(m_pInfo->pData, pData, N); 
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
SharedBuffer<T,Allocator>::SharedBuffer(CONST SharedBuffer& B, Allocator& A) : m_Allocator(A)
{
  B.AddRef();
  m_pInfo = B.m_pInfo;
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
SharedBuffer<T,Allocator>::~SharedBuffer()
{
  Free();
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
VOID SharedBuffer<T,Allocator>::AddRef() CONST
{
  if (m_pInfo != NULL)
  {
    #ifdef DEBUG
    unsigned int RefCount = m_pInfo->RefCount;
    ++m_pInfo->RefCount;
    Assert(m_pInfo->RefCount > RefCount);
    #else
    ++m_pInfo->RefCount;
    #endif
  }
}

//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
VOID SharedBuffer<T,Allocator>::ReleaseRef()
{
  if (m_pInfo != NULL)
  {
    #ifdef DEBUG
    unsigned int RefCount = m_pInfo->RefCount;
    --m_pInfo->RefCount;
    Assert(m_pInfo->RefCount < RefCount);
    #else
    --m_pInfo->RefCount;
    #endif
    
    if (m_pInfo->RefCount == 0)
    {
      m_Allocator.Delete(m_pInfo->pData,m_pInfo->DataSize);
      delete m_pInfo;
    }
    m_pInfo = NULL;
  }
}

//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
DWORD SharedBuffer<T,Allocator>::RefCount() CONST
{
  if (m_pInfo == NULL)
    return 0;
  else
    return m_pInfo->RefCount;    
}

//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
VOID SharedBuffer<T,Allocator>::Allocate(SIZE_T N)
{
  // Allocate may kill data.
  // COW mode will allocate for only me, and fork the SharedBuffer if needed.
  if (m_pInfo != NULL)
  {
    // Allocate a new COW SharedBuffer, and release the old one
    
    if (m_pInfo->RefCount > 1 || AllocateSize(N) != AllocateSize(m_pInfo->DataSize))
    {
      ReleaseRef();
      m_pInfo = new SharedBufferInfo();
      Assert(m_pInfo != NULL);
      m_pInfo->RefCount = 1;
      m_pInfo->pData = m_Allocator.New(AllocateSize(N));
      InitializeObjects((T*)m_pInfo->pData, AllocateSize(N));
      Assert(m_pInfo->pData != NULL);
      m_pInfo->DataSize = N;

    }

  }
  else if(m_pInfo == NULL)
  {
    // A new SharedBuffer.
    m_pInfo = new SharedBufferInfo();
    Assert(m_pInfo != NULL);
    m_pInfo->RefCount = 1;
    m_pInfo->pData = m_Allocator.New(AllocateSize(N));
    InitializeObjects((T*)m_pInfo->pData, AllocateSize(N));
    m_pInfo->DataSize = N;
    Assert(m_pInfo->pData != NULL);
  }

}

//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
SIZE_T SharedBuffer<T,Allocator>::Length() CONST
{
  if (m_pInfo == NULL)
    return 0;
  else
    return m_pInfo->DataSize;
}

//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
VOID SharedBuffer<T,Allocator>::Resize(SIZE_T N)
{
	// Shared mode will allocate for everyone
  // COW mode will allocate for only me, and fork the SharedBuffer if needed.

  if (m_pInfo == NULL)
    Allocate(N);
  else
  {
    // Allocate a new COW SharedBuffer, and release the old one
    if (m_pInfo->RefCount > 1 || AllocateSize(N) != AllocateSize(m_pInfo->DataSize))
    {
      SharedBufferInfo* pInfo = new SharedBufferInfo();
      Assert(pInfo != NULL);
      pInfo->RefCount = 1;
      pInfo->pData = m_Allocator.New(AllocateSize(N));
			InitializeObjects((T*)pInfo->pData, AllocateSize(N));
			Assert(pInfo->pData != NULL);
      pInfo->DataSize = N;

      SIZE_T CopyLength = N;
      if (CopyLength > m_pInfo->DataSize)
        CopyLength = m_pInfo->DataSize;

      CopyObjects((T*)pInfo->pData, (CONST T*)m_pInfo->pData, CopyLength); 
      ReleaseRef();

      m_pInfo = pInfo;
    }

  }

}

//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
VOID SharedBuffer<T,Allocator>::Free()
{
  ReleaseRef();
}

//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
CONST T& SharedBuffer<T,Allocator>::operator () (SIZE_T i) CONST
{
  Assert(m_pInfo != NULL);

  if (i >= m_pInfo->DataSize)
		throw Bolitho::InvalidIndexException(this, TEXT("i"), i);

  return ((CONST T*)m_pInfo->pData)[i];
}

//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
T& SharedBuffer<T,Allocator>::operator () (SIZE_T i)
{
  Assert(m_pInfo != NULL);

  if (i >= m_pInfo->DataSize)
		throw Bolitho::InvalidIndexException(this, TEXT("i"), i);

  //CopyOnWrite();
  return ((T*)m_pInfo->pData)[i];
}
//------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
SharedBuffer<T,Allocator>::operator CONST T* () CONST
{
  if (m_pInfo == 0)
    return 0;

  return (CONST T*)m_pInfo->pData;
}

//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
SharedBuffer<T,Allocator>::operator T* ()
{
  if (m_pInfo == 0)
    return 0;

  CopyOnWrite();
  return (T*)m_pInfo->pData;
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
CONST T* SharedBuffer<T,Allocator>::GetSharedBuffer() CONST
{
  if (m_pInfo == 0)
    return 0;

  return (CONST T*)m_pInfo->pData;
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
T* SharedBuffer<T,Allocator>::GetSharedBuffer()
{
  if (m_pInfo == 0)
    return 0;

  CopyOnWrite();
  return (T*)m_pInfo->pData;
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
SharedBuffer<T,Allocator>& SharedBuffer<T,Allocator>::operator= (CONST SharedBuffer& rkSharedBuffer)
{
  if (m_pInfo != rkSharedBuffer.m_pInfo)
  {
    if (rkSharedBuffer.m_pInfo)
      rkSharedBuffer.AddRef();

    if (m_pInfo)
      ReleaseRef();

    m_pInfo = rkSharedBuffer.m_pInfo;
		CopyOnWrite();
  }
  return *this;
}

//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
BOOL SharedBuffer<T,Allocator>::operator== (T* pkData) CONST
{
  return (m_pInfo->pData == pkData);
}

//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
BOOL SharedBuffer<T,Allocator>::operator!= (T* pkData) CONST
{
  return (m_pInfo->pData != pkData);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
BOOL SharedBuffer<T,Allocator>::operator== (CONST SharedBuffer& rkSharedBuffer) CONST
{
  return (m_pInfo == rkSharedBuffer.m_pInfo);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
BOOL SharedBuffer<T,Allocator>::operator!= (CONST SharedBuffer& rkSharedBuffer) CONST
{
  return (m_pInfo != rkSharedBuffer.m_pInfo);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
inline VOID SharedBuffer<T,Allocator>::CopyOnWrite()
{
  if (m_pInfo != NULL && m_pInfo->RefCount > 1)
  {
    SIZE_T Size = AllocateSize(m_pInfo->DataSize);

    SharedBufferInfo* pInfo = new SharedBufferInfo();
    Assert(pInfo != NULL);

    pInfo->RefCount = 1;
    pInfo->DataSize = m_pInfo->DataSize;

    pInfo->pData = m_Allocator.New(Size);
    Assert(pInfo->pData != NULL);

    CopyObjects((T*)pInfo->pData, (CONST T*)m_pInfo->pData, Size);

    ReleaseRef();
    m_pInfo = pInfo;
  }
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
inline SIZE_T SharedBuffer<T,Allocator>::AllocateSize(SIZE_T N)
{
  if (N == 0)
    return 0;

  CONST SIZE_T Align = 64;
  return (Align*N + Align - 1) / Align;
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
VOID SharedBuffer<T,Allocator>::Write(Ptr<IO::Stream> pStream) CONST
{
  pStream->Write(m_pInfo->pData, sizeof(T)*m_pInfo->DataSize);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
VOID SharedBuffer<T,Allocator>::Read(Ptr<IO::Stream> pStream)
{
  pStream->Read(m_pInfo->pData, sizeof(T)*m_pInfo->DataSize);
}

#if defined(USE_ZLIB)
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
SharedBuffer<BYTE> SharedBuffer<T,Allocator>::Compress(DWORD Type) CONST
{
  return CompressData(m_pInfo->pData, sizeof(T)*m_pInfo->DataSize, Type);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
VOID SharedBuffer<T,Allocator>::Decompress(DWORD Type, CONST SharedBuffer<BYTE>& Source)
{
  return DecompressData(m_pInfo->pData, sizeof(T)*m_pInfo->DataSize, Type);
}
//------------------------------------------------------------------------------------------------------------------------------
#endif

//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
UINT32 SharedBuffer<T,Allocator>::ComputeCrc32() CONST
{
  Crc32 Checksum;
  Checksum.Update(m_pInfo->pData, sizeof(T)*Length());
  return Checksum.Value();
}
//------------------------------------------------------------------------------------------------------------------------------
template<class T, class Allocator>
Md5Hash SharedBuffer<T,Allocator>::ComputeMd5() CONST
{
  Md5 Checksum;
  Checksum.Update(m_pInfo->pData, sizeof(T)*Length());
  return Checksum.Value();
}
//------------------------------------------------------------------------------------------------------------------------------

