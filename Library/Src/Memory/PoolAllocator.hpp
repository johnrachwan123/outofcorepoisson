/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Memory/PoolAllocator.hpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1708 $
 * Last Updated: $Date: 2008-11-12 18:35:04 -0800 (Wed, 12 Nov 2008) $
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

#include "Memory/Memory.hpp"

namespace Bolitho
{
  template<class T, class ThreadingModel=SingleThreadedModel>
  class PoolAllocator
  {
  public:
    PoolAllocator();
    PoolAllocator(SIZE_T BlockSize);
    virtual ~PoolAllocator();

    /* Creates an array of new objects */
    T* New(SIZE_T N);
    /* Creates a new object */
    T* New();

    /* Does nothing */
    VOID Delete(T*& pData);
    /* Does nothing */
    VOID Delete(T*& pData, SIZE_T N);

    /* Deletes all memory allocated with this allocator */
    VOID DeleteAll();

    /* Sets the allocation block size */
    VOID SetBlockSize(SIZE_T BlockSize)
    { m_BlockSize = BlockSize; }

  private:
    typename ThreadingModel::LockType m_Lock;

    struct MEMORYBLOCK
    {
      SIZE_T ObjectCount;
      SIZE_T FreeCount;
      T* pFreePointer;
      T* pData;
      MEMORYBLOCK* pNextBlock;
    };

    MEMORYBLOCK* CreateBlock(SIZE_T N, MEMORYBLOCK* pNext);

    SIZE_T m_BlockSize;
    MEMORYBLOCK* m_pBlockChain;

  public:
    class Enumerator
    {
      template<class T, class ThreadingModel> friend class PoolAllocator;
    protected:
      Enumerator(MEMORYBLOCK* pBlock, SIZE_T Index);
    public:
      VOID MoveToNextElement();
      T& CurrentElement();
      BOOL HasMoreElements();

    private:
      MEMORYBLOCK* m_pCurrentBlock;
      SIZE_T m_CurrentBlockIndex;
    };

  };

  #include "PoolAllocatorImpl.hpp"
}
