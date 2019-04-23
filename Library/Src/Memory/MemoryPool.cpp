/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Memory/MemoryPool.cpp $
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




#include "CodeLibrary.hpp"

#include "Memory/MemoryPool.hpp"

using namespace Bolitho;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho,MemoryPool,Object)
//-----------------------------------------------------------------------------------------------------------------------------------------------------
MemoryPool::MemoryPool(SIZE_T BlockSize, SIZE_T SegmentSize, SIZE_T MaximumSegments)
{
  if (SegmentSize == 0)
    SegmentSize = 1048576 / BlockSize;

  m_BlockSize = BlockSize;
  m_DefaultSegmentSize = SegmentSize;
  m_NumberOfSegments = 0;
  m_MaximumNumberOfSegments = MaximumSegments;
  m_FreeCount = 0;
  m_pSegments = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------		
MemoryPool::~MemoryPool()
{
  Segment* pSegment = m_pSegments;
  while (pSegment != 0)
  {
    ::VirtualFree(pSegment->m_BaseAddress, pSegment->m_Length * m_BlockSize, MEM_RELEASE);
    Segment* pNextSegment = pSegment->m_pNext;
    free(pSegment);
    pSegment = pNextSegment;
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------		
PVOID MemoryPool::Allocate()
{
  if (m_pSegments == 0)
    m_pSegments = CreateSegment(m_DefaultSegmentSize);

  Segment* pSegment = m_pSegments;

  while (pSegment != 0 && pSegment->m_FreeCount == 0)
    pSegment = pSegment->m_pNext;

  if (pSegment == 0)
  {
    if (m_NumberOfSegments == m_MaximumNumberOfSegments && m_MaximumNumberOfSegments > 0)
      throw OutOfMemoryException(this, TEXT("The pool has reached the maximum number of segments"));

    pSegment = CreateSegment(m_DefaultSegmentSize);
    m_NumberOfSegments++;
    m_FreeCount += m_DefaultSegmentSize;
    pSegment->m_pNext = m_pSegments;
    m_pSegments = pSegment;
  }

  ULONG Bit = 0;
  for (SIZE_T BitmapWord=0; BitmapWord  < (pSegment->m_Length + 31) / 32; BitmapWord++)
  {
    DWORD* Bitmap = (&pSegment->m_AllocBitmap + BitmapWord);
    if (_BitScanForward(&Bit, ~*Bitmap))
    {
      *Bitmap = (*Bitmap | 1<<Bit);
      pSegment->m_FreeCount--;
      m_FreeCount--;
      return (BYTE*)pSegment->m_BaseAddress + ((BitmapWord * 32) + Bit) * m_BlockSize; 
    }
  }

  Assert(false);
  return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MemoryPool::Free(PVOID Block)
{
  Segment* pSegment = m_pSegments;

  while (pSegment != 0)
  {
    SSIZE_T d = (BYTE*)Block - (BYTE*)pSegment->m_BaseAddress;
    d /= m_BlockSize;
    if (d >= 0 && (SIZE_T)d < pSegment->m_Length)
    {
      DWORD* Bitmap = (&pSegment->m_AllocBitmap + (d/32));
      SSIZE_T Bit = d%32;
      *Bitmap = (*Bitmap) & ~(1<<Bit);
      pSegment->m_FreeCount++;
      m_FreeCount++;
      return;
    }
    pSegment = pSegment->m_pNext;
  }
  throw InvalidOperationException(this,TEXT("The memory block being freed does not belong to a segment in this MemoryPool"));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
MemoryPool::Segment* MemoryPool::CreateSegment(SIZE_T SegmentSize)
{
  SIZE_T BitmapSize = (SegmentSize + 31) / 8;
  Segment* pSegment = (Segment*)malloc(sizeof(Segment) + BitmapSize - 4);
  memset(pSegment, 0, sizeof(Segment) + BitmapSize - 4);

  pSegment->m_FreeCount = pSegment->m_Length = SegmentSize;
  pSegment->m_BaseAddress = ::VirtualAlloc(0, SegmentSize * m_BlockSize, MEM_COMMIT, PAGE_READWRITE); 
  return pSegment;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T MemoryPool::TotalBlocks()
{
  return m_NumberOfSegments * m_DefaultSegmentSize;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T MemoryPool::FreeBlocks()
{
  return m_FreeCount;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MemoryPool::FreeAll()
{
  Segment* pSegment = m_pSegments;
  while (pSegment != 0)
  {
    ::VirtualFree(pSegment->m_BaseAddress, pSegment->m_Length * m_BlockSize, MEM_RELEASE);
    Segment* pNextSegment = pSegment->m_pNext;
    free(pSegment);
    pSegment = pNextSegment;
  }
  m_pSegments = 0;
  m_NumberOfSegments = 0;
  m_FreeCount = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MemoryPool::FreeEmpty()
{
  Segment* pSegment = m_pSegments;
  Segment* pPrevSegment = 0;
  while (pSegment != 0)
  {
    Segment* pNextSegment = pSegment->m_pNext;

    if (pSegment->m_FreeCount == pSegment->m_Length)
    {
      ::VirtualFree(pSegment->m_BaseAddress, pSegment->m_Length * m_BlockSize, MEM_RELEASE);
      free(pSegment);
      m_NumberOfSegments--;
      m_FreeCount -= pSegment->m_Length;
      if (pPrevSegment == 0)
        m_pSegments = pNextSegment;
      else
        pPrevSegment->m_pNext = pNextSegment;    
    }
    else
      pPrevSegment = pSegment;
    pSegment = pNextSegment;
  }
}
