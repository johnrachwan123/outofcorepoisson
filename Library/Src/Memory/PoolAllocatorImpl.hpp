//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ThreadingModel>
PoolAllocator<T,ThreadingModel>::PoolAllocator() : m_BlockSize(4096)
{
  m_pBlockChain = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ThreadingModel>
PoolAllocator<T,ThreadingModel>::PoolAllocator(SIZE_T BlockSize)
{
  m_BlockSize = BlockSize;
  m_pBlockChain = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ThreadingModel>
PoolAllocator<T,ThreadingModel>::~PoolAllocator()
{
  DeleteAll();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ThreadingModel>
T* PoolAllocator<T,ThreadingModel>::New(SIZE_T N)
{
  ThreadingModel::Lock(m_Lock);

  if (m_pBlockChain == 0 || N > m_pBlockChain->FreeCount)
    if (N <= m_BlockSize)
      m_pBlockChain = CreateBlock(m_BlockSize, m_pBlockChain);
    else
      m_pBlockChain = CreateBlock(N, m_pBlockChain);

  m_pBlockChain->FreeCount -= N;
  T* pObjects = m_pBlockChain->pFreePointer;
  m_pBlockChain->pFreePointer += N;

  if (Traits<T>::IsPrimitive)
    ZeroMemory(pObjects, sizeof(T)*N);
  else
    for (SIZE_T i=0; i<N; i++)
      new (&pObjects[i]) T();

  ThreadingModel::Unlock(m_Lock);

  return pObjects;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ThreadingModel>
T* PoolAllocator<T,ThreadingModel>::New()
{
  ThreadingModel::Lock(m_Lock);

  if (m_pBlockChain == 0 || m_pBlockChain->FreeCount < 1)
    m_pBlockChain = CreateBlock(m_BlockSize, m_pBlockChain);

  m_pBlockChain->FreeCount--;
  T* pObject = m_pBlockChain->pFreePointer;
  m_pBlockChain->pFreePointer++;

  if (Traits<T>::IsPrimitive)
    ZeroMemory(pObject, sizeof(T));
  else
    new (pObject) T();

  ThreadingModel::Unlock(m_Lock);

  return pObject;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ThreadingModel>
VOID PoolAllocator<T,ThreadingModel>::Delete(T*& pData)
{
  /* Freeing an individual object does nothing.  DeleteAll will cause all memory to be freed */
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ThreadingModel>
VOID PoolAllocator<T,ThreadingModel>::Delete(T*& pData, SIZE_T N)
{
  /* Freeing an individual object does nothing.  DeleteAll will cause all memory to be freed */
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ThreadingModel>
VOID PoolAllocator<T,ThreadingModel>::DeleteAll()
{
  ThreadingModel::Lock(m_Lock);

  MEMORYBLOCK* pBlock = m_pBlockChain;
  while (pBlock)
  {
    MEMORYBLOCK* pNext = pBlock->pNextBlock;
    if (!Traits<T>::IsPrimitive)
      for (SIZE_T i=0; i<pBlock->ObjectCount; i++)
        pBlock->pData[i].~T();
    FreeMemory(pBlock);
    pBlock = pNext;
  }
  m_pBlockChain = 0;

  ThreadingModel::Unlock(m_Lock);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ThreadingModel>
typename PoolAllocator<T,ThreadingModel>::MEMORYBLOCK* PoolAllocator<T,ThreadingModel>::CreateBlock(SIZE_T N, typename PoolAllocator<T,ThreadingModel>::MEMORYBLOCK* pNext)
{
  MEMORYBLOCK* pBlock = (MEMORYBLOCK*)AllocateMemory(sizeof(MEMORYBLOCK) + sizeof(T)*N);
  pBlock->ObjectCount = N;
  pBlock->FreeCount = N;
  pBlock->pData = (T*)(pBlock + 1);
  pBlock->pFreePointer = pBlock->pData;
  pBlock->pNextBlock = pNext;
  return pBlock;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

