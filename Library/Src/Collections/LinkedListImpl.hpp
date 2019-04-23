/***************************************************************************************************
* File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Collections/LinkedListImpl.hpp $
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

//---------------------------------------------------------------------------------------------------
ImplementTemplate3RuntimeType(Bolitho,SinglyLinkedList,Object);
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
SinglyLinkedList<T,ElementTraits,ThreadingModel>::Enumerator::Enumerator(CONST SinglyLinkedList* pList)
{
  m_pList = pList;
  m_pCurrentNode = m_pList->m_pHead;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
SinglyLinkedList<T,ElementTraits,ThreadingModel>::Enumerator::operator BOOL() CONST
{
  return m_pCurrentNode != 0;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
typename SinglyLinkedList<T,ElementTraits,ThreadingModel>::Enumerator SinglyLinkedList<T,ElementTraits,ThreadingModel>::Enumerator::operator ++ ()
{
  if (m_pCurrentNode)
    m_pCurrentNode = m_pCurrentNode->m_pNext;
  return this;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
typename SinglyLinkedList<T,ElementTraits,ThreadingModel>::Enumerator& SinglyLinkedList<T,ElementTraits,ThreadingModel>::Enumerator::operator++ (int)
{
  if (m_pCurrentNode)
    m_pCurrentNode = m_pCurrentNode->m_pNext;
  return this;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
BOOL SinglyLinkedList<T,ElementTraits,ThreadingModel>::Enumerator::IsValid() CONST
{
  return m_pCurrentNode != 0;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
T& SinglyLinkedList<T,ElementTraits,ThreadingModel>::Enumerator::CurrentElement() CONST
{
  return m_pCurrentNode->m_Value;
}
//---------------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
SinglyLinkedList<T,ElementTraits,ThreadingModel>::Iterator::Iterator(SinglyLinkedList* pList, Node* pNode)
{
  m_pList = pList;
  m_pCurrentNode = pNode;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
typename SinglyLinkedList<T,ElementTraits,ThreadingModel>::Iterator SinglyLinkedList<T,ElementTraits,ThreadingModel>::Iterator::operator++ ()
{
  if (m_pCurrentNode)
    m_pCurrentNode = m_pCurrentNode->m_pNext;
  return this;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
typename SinglyLinkedList<T,ElementTraits,ThreadingModel>::Iterator& SinglyLinkedList<T,ElementTraits,ThreadingModel>::Iterator::operator++ (int)
{
  if (m_pCurrentNode)
    m_pCurrentNode = m_pCurrentNode->m_pNext;
  return this;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
BOOL SinglyLinkedList<T,ElementTraits,ThreadingModel>::Iterator::operator== (CONST Iterator& I) CONST
{
  return m_pCurrentNode == I.m_pCurrentNode;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
BOOL SinglyLinkedList<T,ElementTraits,ThreadingModel>::Iterator::operator!= (CONST Iterator& I) CONST
{
  return m_pCurrentNode != I.m_pCurrentNode;
}
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
SinglyLinkedList<T,ElementTraits,ThreadingModel>::SinglyLinkedList()
{
  m_pHead = 0;
  m_Count = 0;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
SinglyLinkedList<T,ElementTraits,ThreadingModel>::~SinglyLinkedList()
{
  RemoveAll();
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
VOID SinglyLinkedList<T,ElementTraits,ThreadingModel>::Add(CONST T& Value)
{
  Node* pNode = new Node(Value);
  pNode->m_pNext = m_pHead;
  m_pHead = pNode;
  m_Count++;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
BOOL SinglyLinkedList<T,ElementTraits,ThreadingModel>::RemoveHead()
{
  Node* pNode = m_pHead;
  if (pNode == 0)
    return FALSE;
  m_pHead = pNode->m_pNext;
  delete pNode;
  m_Count--;
  return TRUE;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
VOID SinglyLinkedList<T,ElementTraits,ThreadingModel>::RemoveAll()
{
  Node* pNode = m_pHead;
  while (pNode != 0)
  {
    Node* pNextNode = pNode->m_pNext;
    delete pNode;
    pNode = pNextNode;
  }
  m_pHead = 0;
  m_Count = 0;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
typename SinglyLinkedList<T,ElementTraits,ThreadingModel>::Iterator SinglyLinkedList<T,ElementTraits,ThreadingModel>::Find(CONST T& Value) CONST
{
  Node* pNode = m_pHead;
  while (pNode != 0)
  {
    if (ElementTraits::CompareEqual(pNode->m_Value,Value) == 0)
      return Iterator(this, pNode);
    pNode = pNode->m_pNext;
  }

  return Iterator(this, 0);
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
VOID SinglyLinkedList<T,ElementTraits,ThreadingModel>::Remove(CONST T& Value)
{
  Node* pNode = m_pHead;
  Node* pPrev = 0;

  while (pNode != 0)
  {
    if (ElementTraits::CompareEqual(pNode->m_Value,Value) == 0)
    {
      Node* pNext = pNode->m_pNext;
      if (pPrev == 0)
        m_pHead = pNext;
      else
        pPrev->m_pNext = pNext;
      delete pNode;
      pNode = pNext;
    }
    else
    {
      pPrev = pNode;
      pNode = pNode->m_pNext;
    }
  }
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
SIZE_T SinglyLinkedList<T,ElementTraits,ThreadingModel>::Count() CONST
{
  return m_Count;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
BOOL SinglyLinkedList<T,ElementTraits,ThreadingModel>::IsEmpty() CONST
{
  return m_pHead == 0;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
typename SinglyLinkedList<T,ElementTraits,ThreadingModel>::Enumerator SinglyLinkedList<T,ElementTraits,ThreadingModel>::Elements() CONST
{
  return Enumerator(this);
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
typename SinglyLinkedList<T,ElementTraits,ThreadingModel>::Iterator SinglyLinkedList<T,ElementTraits,ThreadingModel>::Begin()
{
  return Iterator(this, m_pHead);
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
typename SinglyLinkedList<T,ElementTraits,ThreadingModel>::Iterator SinglyLinkedList<T,ElementTraits,ThreadingModel>::End()
{
  return Iterator(this, 0);
}
//---------------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------------
ImplementTemplate3RuntimeType(Bolitho,DoublyLinkedList,Object);
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
DoublyLinkedList<T,ElementTraits,ThreadingModel>::Enumerator::Enumerator(CONST DoublyLinkedList* pList)
{
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
DoublyLinkedList<T,ElementTraits,ThreadingModel>::Enumerator::operator BOOL() CONST
{
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
typename DoublyLinkedList<T,ElementTraits,ThreadingModel>::Enumerator DoublyLinkedList<T,ElementTraits,ThreadingModel>::Enumerator::operator ++ ()
{
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
typename DoublyLinkedList<T,ElementTraits,ThreadingModel>::Enumerator& DoublyLinkedList<T,ElementTraits,ThreadingModel>::Enumerator::operator++ (int)
{
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
typename DoublyLinkedList<T,ElementTraits,ThreadingModel>::Enumerator DoublyLinkedList<T,ElementTraits,ThreadingModel>::Enumerator::operator -- ()
{
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
typename DoublyLinkedList<T,ElementTraits,ThreadingModel>::Enumerator& DoublyLinkedList<T,ElementTraits,ThreadingModel>::Enumerator::operator-- (int)
{
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
BOOL DoublyLinkedList<T,ElementTraits,ThreadingModel>::Enumerator::IsValid() CONST
{
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
T& DoublyLinkedList<T,ElementTraits,ThreadingModel>::Enumerator::CurrentElement() CONST
{
}
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
DoublyLinkedList<T,ElementTraits,ThreadingModel>::Iterator::Iterator(DoublyLinkedList* pList, Node* pNode)
{
  m_pList = pList;
  m_pCurrentNode = pNode;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
typename DoublyLinkedList<T,ElementTraits,ThreadingModel>::Iterator DoublyLinkedList<T,ElementTraits,ThreadingModel>::Iterator::operator ++ ()
{
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
typename DoublyLinkedList<T,ElementTraits,ThreadingModel>::Iterator& DoublyLinkedList<T,ElementTraits,ThreadingModel>::Iterator::operator++ (int)
{
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
typename DoublyLinkedList<T,ElementTraits,ThreadingModel>::Iterator DoublyLinkedList<T,ElementTraits,ThreadingModel>::Iterator::operator -- ()
{
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
typename DoublyLinkedList<T,ElementTraits,ThreadingModel>::Iterator& DoublyLinkedList<T,ElementTraits,ThreadingModel>::Iterator::operator-- (int)
{
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
BOOL DoublyLinkedList<T,ElementTraits,ThreadingModel>::Iterator::operator== (CONST Iterator& I) CONST
{
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
BOOL DoublyLinkedList<T,ElementTraits,ThreadingModel>::Iterator::operator!= (CONST Iterator& I) CONST
{
}
//---------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
DoublyLinkedList<T,ElementTraits,ThreadingModel>::DoublyLinkedList()
{
  m_pHead = 0;
  m_pTail = 0;
  m_Count = 0;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
DoublyLinkedList<T,ElementTraits,ThreadingModel>::~DoublyLinkedList()
{
  RemoveAll();
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
VOID DoublyLinkedList<T,ElementTraits,ThreadingModel>::Add(CONST T& Value)
{
  AddHead(Value);
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
VOID DoublyLinkedList<T,ElementTraits,ThreadingModel>::AddHead(CONST T& Value)
{
  Node* pNode = new Node(Value);
  pNode->m_pNext = m_pHead;
  if (m_pHead != 0)
    m_pHead->m_pPrev = pNode;
  m_pHead = pNode;
  if (m_pTail == 0)
    m_pTail = m_pHead;
  m_Count++;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
VOID DoublyLinkedList<T,ElementTraits,ThreadingModel>::AddTail(CONST T& Value)
{
  Node* pNode = new Node(Value);
  pNode->m_pPrev = m_pTail;
  if (m_pTail != 0)
    m_pTail->m_pNext = pNode;
  m_pTail = pNode;
  if (m_pHead == 0)
    m_pHead = m_pTail;
  m_Count++;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
BOOL DoublyLinkedList<T,ElementTraits,ThreadingModel>::RemoveHead()
{
  Node* pNode = m_pHead;
  if (pNode == 0)
    return FALSE;
  m_pHead = pNode->m_pNext;
  if (m_pHead != 0)
    m_pHead->m_pPrev = 0;
  else
    m_pTail = 0;

  delete pNode;
  m_Count--;
  return TRUE;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
BOOL DoublyLinkedList<T,ElementTraits,ThreadingModel>::RemoveTail()
{
  Node* pNode = m_pTail;
  if (pNode == 0)
    return FALSE;
  m_pTail = pNode->m_pPrev;
  if (m_pTail != 0)
    m_pTail->m_pNext = 0;
  else 
    m_pHead = 0;

  delete pNode;
  m_Count--;
  return TRUE;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
VOID DoublyLinkedList<T,ElementTraits,ThreadingModel>::RemoveAll()
{
  Node* pNode = m_pHead;
  while (pNode != 0)
  {
    Node* pNextNode = pNode->m_pNext;
    delete pNode;
    pNode = pNextNode;
  }
  m_pHead = 0;
  m_pTail = 0;
  m_Count = 0;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
typename DoublyLinkedList<T,ElementTraits,ThreadingModel>::Iterator DoublyLinkedList<T,ElementTraits,ThreadingModel>::Find(CONST T& Value) CONST
{
  Node* pNode = m_pHead;
  while (pNode != 0)
  {
    if (ElementTraits::CompareEqual(pNode->m_Value,Value) == 0)
      return Iterator(this, pNode);
    pNode = pNode->m_pNext;
  }

  return Iterator(this, 0);
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
VOID DoublyLinkedList<T,ElementTraits,ThreadingModel>::Remove(CONST T& Value)
{
  Node* pNode = m_pHead;

  while (pNode != 0)
  {
    if (ElementTraits::CompareEqual(pNode->m_Value,Value) == 0)
    {
      Node* pNext = pNode;
      if (pNode->m_pPrev != 0)
        pNode->m_pPrev->m_pNext = pNode->m_pNext;
      else
        m_pHead = pNode->m_pNext;

      if (pNode->m_pNext != 0)
        pNode->m_pNext->m_pPrev = pNode->m_pPrev;
      else
        m_pTail = pNode->m_pPrev;
      pNode = pNext;
      delete pNode;
      m_Count--;
    }
    else
    {
      pPrev = pNode;
      pNode = pNode->m_pNext;
    }
  }
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
SIZE_T DoublyLinkedList<T,ElementTraits,ThreadingModel>::Count() CONST
{
  return m_Count;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
BOOL DoublyLinkedList<T,ElementTraits,ThreadingModel>::IsEmpty() CONST
{
  return m_pHead == 0;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
typename DoublyLinkedList<T,ElementTraits,ThreadingModel>::Enumerator DoublyLinkedList<T,ElementTraits,ThreadingModel>::Elements() CONST
{
  return Enumerator(this);
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
typename DoublyLinkedList<T,ElementTraits,ThreadingModel>::Iterator DoublyLinkedList<T,ElementTraits,ThreadingModel>::Begin()
{
  return Iterator(this, m_pHead);
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
typename DoublyLinkedList<T,ElementTraits,ThreadingModel>::Iterator DoublyLinkedList<T,ElementTraits,ThreadingModel>::Head()
{
  return Iterator(this, m_pHead);
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
typename DoublyLinkedList<T,ElementTraits,ThreadingModel>::Iterator DoublyLinkedList<T,ElementTraits,ThreadingModel>::Tail()
{
  return Iterator(this, m_pTail);
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
typename DoublyLinkedList<T,ElementTraits,ThreadingModel>::Iterator DoublyLinkedList<T,ElementTraits,ThreadingModel>::End()
{
  return Iterator(this, 0);
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
VOID DoublyLinkedList<T,ElementTraits,ThreadingModel>::Remove(Iterator I)
{
  Node* pNode = I.m_pCurrentNode;
  Assert(pNode != 0);

  if (pNode->m_pPrev != 0)
    pNode->m_pPrev->m_pNext = pNode->m_pNext;
  else
    m_pHead = pNode->m_pNext;

  if (pNode->m_pNext != 0)
    pNode->m_pNext->m_pPrev = pNode->m_pPrev;
  else
    m_pTail = pNode->m_pPrev;

  m_Count--;
  delete pNode;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
VOID DoublyLinkedList<T,ElementTraits,ThreadingModel>::Insert(Iterator I, CONST T& Value)
{
  InsertBefore(I, Value);
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
VOID DoublyLinkedList<T,ElementTraits,ThreadingModel>::InsertBefore(Iterator I, CONST T& Value)
{
  Node* pNode = I.m_pCurrentNode;
  Assert(pNode != 0);

  Node* pNewNode = new Node(Value);
  
  if (pNode->m_pPrev)
    pNode->m_pPrev->m_pNext = pNewNode;
  else
    m_pHead = pNewNode;
  pNewNode->m_pPrev = pNode->m_pPrev;

  pNode->m_pPrev = pNewNode;
  pNewNode->m_pNext = pNode;
  m_Count++;
}
//---------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
VOID DoublyLinkedList<T,ElementTraits,ThreadingModel>::InsertAfter(Iterator I, CONST T& Value)
{
  Node* pNode = I.m_pCurrentNode;
  Assert(pNode != 0);

  Node* pNewNode = new Node(Value);

  if (pNode->m_pNext)
    pNode->m_pNext->m_pPrev = pNewNode;
  else
    m_pTail = pNewNode;
  pNewNode->m_pNext = pNode->m_pNext;

  pNode->m_pNext = pNewNode;
  pNewNode->m_pPrev = pNode;
  m_Count++;
}
//---------------------------------------------------------------------------------------------------

