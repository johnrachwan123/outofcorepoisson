/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Memory/ObjectPoolImpl.hpp $
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




//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, SIZE_T N>
ObjectPool<T,N>::ObjectPool(SIZE_T SegmentSize, SIZE_T MaximumSegments) :
  m_MemoryPool(sizeof(T)*N, SegmentSize, MaximumSegments)
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, SIZE_T N>
ObjectPool<T,N>::~ObjectPool()
{
  DeleteAll();
  m_MemoryPool.FreeAll();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, SIZE_T N>
T* ObjectPool<T,N>::New()
{
  T* pObject = (T*)m_MemoryPool.Allocate();

  for (SIZE_T i=0; i<N; i++)
    new (pObject + i) T();

  return pObject;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, SIZE_T N>
VOID ObjectPool<T,N>::Delete(T*& pObject)
{
  for (SIZE_T i=0; i<N; i++)
    (pObject + i)->~T();
  m_MemoryPool.Free(pData);
  pObject = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, SIZE_T N>
VOID ObjectPool<T,N>::DeleteAll()
{
  MemoryPool::Segment* pSegment = m_MemoryPool.m_pSegments;
  while (pSegment != 0)
  {
    UInt32_t* pBitmap = &pSegment->m_AllocBitmap;
    T* pObject = (T*)pSegment->m_BaseAddress;
    for (SIZE_T i=0; i<(pSegment->m_Length+31)/32; i++)
    {
      for (SIZE_T j=0; j<32; j++)
      {
        if (*pBitmap & (1<<j))
          for (SIZE_T i=0; i<N; i++)
            (pObject + i)->~T();
        pObject += N;
      }
      pBitmap++;
    }
    pSegment = pSegment->m_pNext;
  }

  m_MemoryPool.FreeAll();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, SIZE_T N>
SIZE_T ObjectPool<T,N>::TotalObjects()
{
  return m_MemoryPool.TotalBlocks();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, SIZE_T N>
SIZE_T ObjectPool<T,N>::FreeObject()
{
  return m_MemoryPool.FreeBlocks();
}
