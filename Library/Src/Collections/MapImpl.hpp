/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Collections/MapImpl.hpp $
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
ImplementTemplate5RuntimeType(Bolitho,Map,Object);
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Map()
{
  m_pNil = new Node();
  m_pNil->m_pParent = m_pNil->m_pLeft = m_pNil->m_pRight = m_pNil;
  m_pRoot = m_pNil;
  m_NodeCount = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::~Map()
{
  RemoveAll();
  delete m_pNil;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
SIZE_T Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Count() CONST
{
  return m_NodeCount;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
BOOL Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::IsEmpty() CONST
{
  return m_NodeCount == 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
VOID Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Add(CONST K& Key, CONST V& Value)
{
  Node* pNode = FindNode(Key);
  if(pNode == 0)
  {
    m_NodeCount++;
    Insert(Key, Value);
  }
  else
    pNode->m_Value = Value;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
BOOL Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Contains(CONST K& Key) CONST
{
  Node* pNode = FindNode(Key);
  return pNode != 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
V& Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Get(CONST K& Key)
{
  Node* pNode = FindNode(Key);
  Assert (pNode != 0);
  return pNode->m_Value;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
CONST V& Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Get(CONST K& Key) CONST
{
  Node* pNode = FindNode(Key);
  Assert (pNode != 0);
  return pNode->m_Value;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
V& Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::operator[] (CONST K& Key)
{
  return Get(Key);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
CONST V& Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::operator[] (CONST K& Key) CONST
{
  return Get(Key);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
BOOL Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Get(CONST K& Key, V& Value) CONST
{
  Node* pNode = FindNode(Key);
  if (pNode != 0)
  {
    Value = pNode->m_Value;
    return true;
  }
  return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
BOOL Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Remove(CONST K& Key)
{
  Node* pNode = FindNode(Key);

  if (pNode == 0)
    return false;

  m_NodeCount--;
  Delete(pNode);
  return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
VOID Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::RemoveAll()
{
  Node* pNode = m_pRoot;
  while (pNode != m_pNil)
  {
    if (!IsNil(pNode->m_pLeft))
    { 
      pNode = pNode->m_pLeft;
      SetNil(&pNode->m_pParent->m_pLeft);
    }
    else if (!IsNil(pNode->m_pRight))
    {
      pNode = pNode->m_pRight;
      SetNil(&pNode->m_pParent->m_pRight);
    }
    else
    {
      Node* pParent = pNode->m_pParent;
      delete pNode;
      pNode = pParent;
    }
  }
  m_NodeCount = 0;
  SetNil(&m_pRoot);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
typename Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Iterator Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Find(CONST K& Key) CONST
{
  Node* pNode = FindNode(Key);

  if (pNode == 0)
    return End();
  else
    return Iterator(this,pNode);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
typename Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Enumerator Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Elements() CONST
{
  return Enumerator(this);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
typename Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Iterator Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Begin()
{
  Node* pNode = Minimum(m_pRoot);

  if (pNode == 0)
    return End();
  else
    return Iterator(this,pNode);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
typename Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Iterator Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::End()
{
  return Iterator(this,0);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Enumerator::Enumerator(CONST Map* pMap)
{
  m_pMap = pMap;
  m_pNode = pMap->Minimum(pMap->m_pRoot);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Enumerator::operator BOOL() CONST
{
  return IsValid();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
typename Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Enumerator Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Enumerator::operator ++ ()
{
  Enumerator E = Enumerator(*this);
  E.m_pNode = m_pMap->Successor(m_pNode);
  return E;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
typename Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Enumerator& Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Enumerator::operator++ (int)
{
  m_pNode = m_pMap->Successor(m_pNode);
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
BOOL Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Enumerator::IsValid() CONST
{
  return m_pNode != 0 && m_pNode != m_pMap->m_pNil;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
Pair<K,V> Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Enumerator::CurrentElement() CONST
{
  return Pair<K,V>(m_pNode->m_Key, m_pNode->m_Value);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Iterator::Iterator(Map* pMap, Node* pNode)
{
  m_pMap = pMap;
  m_pNode = pNode;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
Pair<K,V> Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Iterator::operator* () CONST
{
  return Pair<K,V>(m_pNode->m_Key, m_pNode->m_Value);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
Pair<K,V> Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Iterator::operator-> () CONST
{
  return Pair<K,V>(m_pNode->m_Key, m_pNode->m_Value);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
typename Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Iterator Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Iterator::operator ++ ()
{
  Iterator I = Iterator(*this);
  I.m_pNode = m_pMap->Successor(m_pNode);
  return I;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
typename Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Iterator& Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Iterator::operator++ (int)
{
  m_pNode = m_pMap->Successor(m_pNode);
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
typename Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Iterator Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Iterator::operator -- ()
{
  Iterator I = Iterator(*this);
  I.m_pNode = m_pMap->Predessor(m_pNode);
  return I;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
typename Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Iterator& Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Iterator::operator-- (int)
{
  m_pNode = m_pMap->Predessor(m_pNode);
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
BOOL Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Iterator::operator== (CONST Iterator& I) CONST
{
  return m_pNode == I.m_pNode;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
BOOL Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Iterator::operator!= (CONST Iterator& I) CONST
{
  return m_pNode != I.m_pNode;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
typename Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Node* Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Minimum(Node* pNode) CONST
{
  if (pNode == 0 || IsNil(pNode))
    return 0;

  Node* pMin = pNode;
  while (!IsNil(pMin->m_pLeft))
    pMin = pMin->m_pLeft;

  return pMin;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
typename Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Node* Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Maximum(Node* pNode) CONST
{
  if (pNode == 0 || IsNil(pNode))
    return 0;

  Node* pMax = pNode;
  while (!IsNil(pMax->m_pRight))
    pMax = pMax->m_pRight;

  return pMax;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
typename Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Node* Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Predecessor(Node* pNode) CONST 
{
  if(pNode == 0)
    return 0;

  if(!IsNil(pNode->m_pLeft))
    return Maximum(pNode->m_pLeft);

  Node* pParent = pNode->m_pParent;
  Node* pLeft = pNode;
  while(!IsNil(pParent) && (pLeft == pParent->m_pLeft))
  {
    pLeft = pParent;
    pParent = pParent->m_pParent;
  }

  if (IsNil(pParent))
    pParent = 0;

  return pParent;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
typename Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Node* Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Successor(Node* pNode) CONST
{
  if (pNode == 0)
    return 0;

  if (!IsNil(pNode->m_pRight))
    return Minimum(pNode->m_pRight);

  Node* pParent = pNode->m_pParent;
  Node* pRight = pNode;
  while (!IsNil(pParent) && (pRight == pParent->m_pRight))
  {
    pRight = pParent;
    pParent = pParent->m_pParent;
  }

  if (IsNil(pParent))
    pParent = 0;

  return pParent;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
BOOL Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::IsNil(Node* pNode) CONST
{
  return (pNode == m_pNil);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
VOID Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::SetNil(Node** pNode)
{
  *pNode = m_pNil;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
typename Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Node* Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::FindNode(CONST K& Key) CONST
{
  Node* pKey = 0;
  Node* pNode = m_pRoot;
  while(!IsNil(pNode) && (pKey == NULL))
  {
    if(KeyElementTraits::CompareEqual(pNode->m_Key,Key))
      pKey = pNode;
    else 
      if(KeyElementTraits::CompareLessThan(pNode->m_Key,Key))
        pNode = pNode->m_pRight;
      else
        pNode = pNode->m_pLeft;
  }

  if(pKey == 0)
    return 0;

  while(true)
  {
    Node* pPrev = Predecessor(pKey);
    if(pPrev != 0 &&  KeyElementTraits::CompareEqual(pPrev->m_Key,Key))
      pKey = pPrev;
    else
      return pKey;
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
typename Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Node* Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Insert(CONST K& Key, CONST V& Value)
{
  Node* pNewNode = InsertImpl(Key, Value);

  Node* pX = pNewNode;
  pX->m_Status = Node::Red;
  Node* pY = 0;
  while (pX != m_pRoot && pX->m_pParent->m_Status == Node::Red)
  {
    if (pX->m_pParent == pX->m_pParent->m_pParent->m_pLeft)
    {
      pY = pX->m_pParent->m_pParent->m_pRight;
      if (pY != NULL && pY->m_Status == Node::Black)
      {
        pX->m_pParent->m_Status = Node::Black;
        pY->m_Status = Node::Black;
        pX->m_pParent->m_pParent->m_Status = Node::Red;
        pX = pX->m_pParent->m_pParent;
      }
      else
      {
        if (pX == pX->m_pParent->m_pRight)
        {
          pX = pX->m_pParent;
          LeftRotate(pX);
        }
        pX->m_pParent->m_Status = Node::Black;
        pX->m_pParent->m_pParent->m_Status = Node::Red;
        RightRotate(pX->m_pParent->m_pParent);
      }
    }
    else
    {
      pY = pX->m_pParent->m_pParent->m_pLeft;
      if (pY != NULL && pY->m_Status == Node::Red)
      {
        pX->m_pParent->m_Status = Node::Black;
        pY->m_Status = Node::Black;
        pX->m_pParent->m_pParent->m_Status = Node::Red;
        pX = pX->m_pParent->m_pParent;
      }
      else
      {
        if (pX == pX->m_pParent->m_pLeft)
        {
          pX = pX->m_pParent;
          RightRotate(pX);
        }
        pX->m_pParent->m_Status = Node::Black;
        pX->m_pParent->m_pParent->m_Status = Node::Red;
        LeftRotate(pX->m_pParent->m_pParent);
      }
    }
  }

  m_pRoot->m_Status = Node::Black;
  SetNil(&m_pRoot->m_pParent);

  return pNewNode;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
typename Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Node* Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::InsertImpl(CONST K& Key, CONST V& Value)
{
  Node* pNew = new Node(Key, Value);
  pNew->m_pParent = pNew->m_pLeft = pNew->m_pRight = m_pNil;

  Node* pY = NULL;
  Node* pX = m_pRoot;

  while (!IsNil(pX))
  {
    pY = pX;
    if(KeyElementTraits::CompareLessThan(Key,pX->m_Key))
      pX = pX->m_pLeft;
    else
      pX = pX->m_pRight;
  }

  pNew->m_pParent = pY;
  if (pY == 0)
    m_pRoot = pNew;
  else if(KeyElementTraits::CompareLessThan(Key,pY->m_Key))
    pY->m_pLeft = pNew;
  else
    pY->m_pRight = pNew;

  return pNew;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
typename Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Node* Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::LeftRotate(Node* pNode)
{
  Assert(pNode != 0);
  if(pNode == 0)
    return 0;

  Node* pRight = pNode->m_pRight;
  pNode->m_pRight = pRight->m_pLeft;
  if (!IsNil(pRight->m_pLeft))
    pRight->m_pLeft->m_pParent = pNode;

  pRight->m_pParent = pNode->m_pParent;
  if (IsNil(pNode->m_pParent))
    m_pRoot = pRight;
  else if (pNode == pNode->m_pParent->m_pLeft)
    pNode->m_pParent->m_pLeft = pRight;
  else 
    pNode->m_pParent->m_pRight = pRight;

  pRight->m_pLeft = pNode;
  pNode->m_pParent = pRight;
  return pNode;

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
typename Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Node* Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::RightRotate(Node* pNode)
{
  Assert(pNode != 0);
  if(pNode == 0)
    return 0;

  Node* pLeft = pNode->m_pLeft;
  pNode->m_pLeft = pLeft->m_pRight;
  if (!IsNil(pLeft->m_pRight))
    pLeft->m_pRight->m_pParent = pNode;

  pLeft->m_pParent = pNode->m_pParent;
  if (IsNil(pNode->m_pParent))
    m_pRoot = pLeft;
  else if (pNode == pNode->m_pParent->m_pRight)
    pNode->m_pParent->m_pRight = pLeft;
  else
    pNode->m_pParent->m_pLeft = pLeft;

  pLeft->m_pRight = pNode;
  pNode->m_pParent = pLeft;

  return pNode;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
VOID Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::DeleteFixup(Node* pNode)
{
  Node* pX = pNode;
  Node* pW = NULL;

  while ((pX != m_pRoot) && (pX->m_Status == Node::Black))
  {
    if (pX == pX->m_pParent->m_pLeft)
    {
      pW = pX->m_pParent->m_pRight;
      if (pW->m_Status == Node::Red)
      {
        pW->m_Status = Node::Black;
        pW->m_pParent->m_Status = Node::Red;
        LeftRotate(pX->m_pParent);
        pW = pX->m_pParent->m_pRight;
      }
      if (pW->m_pLeft->m_Status == Node::Black && pW->m_pRight->m_Status == Node::Black)
      {
        pW->m_Status = Node::Red;
        pX = pX->m_pParent;
      }
      else
      {
        if (pW->m_pRight->m_Status == Node::Black)
        {
          pW->m_pLeft->m_Status = Node::Black;
          pW->m_Status = Node::Red;
          RightRotate(pW);
          pW = pX->m_pParent->m_pRight;
        }
        pW->m_Status = pX->m_pParent->m_Status;
        pX->m_pParent->m_Status = Node::Black;
        pW->m_pRight->m_Status = Node::Black;
        LeftRotate(pX->m_pParent);
        pX = m_pRoot;
      }
    }
    else
    {
      pW = pX->m_pParent->m_pLeft;
      if (pW->m_Status == Node::Red)
      {
        pW->m_Status = Node::Black;
        pW->m_pParent->m_Status = Node::Red;
        RightRotate(pX->m_pParent);
        pW = pX->m_pParent->m_pLeft;
      }
      if (pW->m_pRight->m_Status == Node::Black && pW->m_pLeft->m_Status == Node::Black)
      {
        pW->m_Status = Node::Red;
        pX = pX->m_pParent;
      }
      else
      {
        if (pW->m_pLeft->m_Status == Node::Black)
        {
          pW->m_pRight->m_Status = Node::Black;
          pW->m_Status = Node::Red;
          LeftRotate(pW);
          pW = pX->m_pParent->m_pLeft;
        }
        pW->m_Status = pX->m_pParent->m_Status;
        pX->m_pParent->m_Status = Node::Black;
        pW->m_pLeft->m_Status = Node::Black;
        RightRotate(pX->m_pParent);
        pX = m_pRoot;
      }
    }
  }

  pX->m_Status = Node::Black;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
BOOL Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Delete(Node* pZ)
{
  if (pZ == NULL)
    return false;

  Node* pY = NULL;
  Node* pX = NULL;
  if (IsNil(pZ->m_pLeft) || IsNil(pZ->m_pRight))
    pY = pZ;
  else
    pY = Successor(pZ);

  if (!IsNil(pY->m_pLeft))
    pX = pY->m_pLeft;
  else
    pX = pY->m_pRight;

  pX->m_pParent = pY->m_pParent;

  if (IsNil(pY->m_pParent))
    m_pRoot = pX;
  else if (pY == pY->m_pParent->m_pLeft)
    pY->m_pParent->m_pLeft = pX;
  else
    pY->m_pParent->m_pRight = pX;

  if (pY->m_Status == Node::Black)
    DeleteFixup(pX);

  if (pY != pZ)
    SwapNode(pY, pZ);

  if (m_pRoot != NULL)
    SetNil(&m_pRoot->m_pParent);

  delete pZ;

  return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
VOID Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::SwapNode(Node* pDest, Node* pSrc)
{
  pDest->m_pParent = pSrc->m_pParent;
  if (pSrc->m_pParent->m_pLeft == pSrc)
    pSrc->m_pParent->m_pLeft = pDest;
  else
    pSrc->m_pParent->m_pRight = pDest;

  pDest->m_pRight = pSrc->m_pRight;
  pDest->m_pLeft = pSrc->m_pLeft;
  pDest->m_Status = pSrc->m_Status;
  pDest->m_pRight->m_pParent = pDest;
  pDest->m_pLeft->m_pParent = pDest;

  if (m_pRoot == pSrc)
    m_pRoot = pDest;
}
//----------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
INLINE Bolitho::IO::BinaryStream& operator << (Bolitho::IO::BinaryStream& S, CONST Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>& M)
{
  S << (UINT64)M.Count();
  typename Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>::Enumerator E = M.Elements();

  while(E)
  {
    Pair<K,V> P = E.CurrentElement(); 
    S << P.First;
    S << P.Second;
    E++;
  }
  return S;
}
//----------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits, class ThreadingModel>
inline Bolitho::IO::BinaryStream& operator >> (Bolitho::IO::BinaryStream& S, Map<K,V,KeyElementTraits, ValueElementTraits, ThreadingModel>& M)
{
  UINT64 N = S.ReadUInt64();
#if defined(PLATFORM_32BIT)
  if (N > 0x7fffffff)
    throw InvalidOperationException(0, "Unable to load map -- there are more entries than can be addressed on PLATFORM_32BIT");
#endif

  for(SIZE_T i=0; i<(SIZE_T)N; i++)
  {
    K Key;
    V Value;
    S >> Key;
    S >> Value;
    M.Add(Key,Value);
  }
  return S;
}

