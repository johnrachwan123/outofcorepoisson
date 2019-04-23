/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Memory/MemoryCacheImpl.hpp $
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




//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementTemplateRuntimeType(Library,MemoryCache,Object)
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K>
MemoryCache<K>::MemoryCache(SIZE_T BlockSize, SIZE_T MaximumCacheSize) :
  m_Pool(BlockSize),
  Create(0),
  Reclaim(0)
{
  m_LogicalClock = 0;
  m_MaximumSize = MaximumCacheSize;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K>
MemoryCache<K>::~MemoryCache()
{
  FreeAll();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K>
VOID MemoryCache<K>::Check()
{
  Assert(m_LRU.Count() <= m_Blocks.Count());
  /*
  //Trace("MemoryCache<K>::Check()");
  Map<K,BlockHeader*>::Iterator i = m_Blocks.Begin();
  while (i != m_Blocks.End())
  {
    Pair<K,BlockHeader*> P = *i;
    //Trace("  %d -> %p", P.First(), P.Second());
    if (P.First() > 10000)
      Assert(false);
    i++;
  }

  Set<BlockHeader*,BlockHeaderTraits>::Iterator j = m_LRU.Begin();
  while (j != m_LRU.End())
  {
    if (j->m_Key > 10000)
      Assert(false);
    j++;
  }
  */
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K>
PVOID MemoryCache<K>::Get(CONST K& Key)
{
  //Trace("%d", Key);
  Check();

  BlockHeader* pHeader;
  if (m_Blocks.Get(Key, pHeader))
  {
    Verify(m_LRU.Remove(pHeader));
    pHeader->m_Clock = m_LogicalClock++;
    pHeader->m_LockCount++;
    m_LRU.Add(pHeader);
    return pHeader->m_pBlock;
  }
  else
  {
    /*
    if (m_Blocks.Count() >= m_MaximumSize && m_MaximumSize > 0 && m_LRU.Count() > 0)
    {
      Set<BlockHeader*,BlockHeaderTraits>::Iterator i = m_LRU.Begin();
      pHeader = *i;
      m_LRU.Remove(pHeader);
      m_Blocks.Remove(pHeader->m_Key);
      Reclaim.Invoke(pHeader->m_Key, pHeader->m_pBlock);
      m_Pool.Free(pHeader->m_pBlock);
      delete pHeader;
    }
    */
    
    pHeader = new BlockHeader();
    pHeader->m_pBlock = m_Pool.Allocate();
    
    pHeader->m_Key = Key;
    pHeader->m_Clock = m_LogicalClock++;
    pHeader->m_LockCount = 1;
    Create.Invoke(Key, pHeader->m_pBlock);
    //m_LRU.Add(pHeader);
    m_Blocks.Add(Key,pHeader);
    return pHeader->m_pBlock;
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K>
VOID MemoryCache<K>::Lock(CONST K& Key)
{
  Check();

  BlockHeader* pHeader;
  if (m_Blocks.Get(Key, pHeader))
  {
    if (pHeader->m_LockCount == 0)
      m_LRU.Remove(pHeader);
    pHeader->m_LockCount++;
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K>
VOID MemoryCache<K>::Unlock(CONST K& Key)
{
  Check();

  BlockHeader* pHeader;
  if (m_Blocks.Get(Key, pHeader))
    if (pHeader->m_LockCount != 0)
    {
      pHeader->m_LockCount--;
      if (pHeader->m_LockCount == 0)
      {
        //pHeader->m_Clock = m_LogicalClock++;
        m_LRU.Add(pHeader);
      }
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K>
VOID MemoryCache<K>::FreeAll()
{
  Check();

  for (Map<K,BlockHeader*>::Iterator i = m_Blocks.Begin(); i != m_Blocks.End(); i++)
  {
    Pair<K,BlockHeader*> P = *i;
    Reclaim.Invoke(P.First(), P.Second()->m_pBlock);
    delete P.Second();
  }
  m_Blocks.RemoveAll();
  m_LRU.RemoveAll();
}
