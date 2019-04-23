/***************************************************************************************************
* File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Collections/LinkedList.hpp $
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

#pragma once

#include "Utility/Pair.hpp"
#include "Utility/Traits.hpp"

namespace Bolitho
{
  template<class T, class ElementTraits=Traits<T>, class ThreadingModel=SingleThreadedModel>
  class SinglyLinkedList : public Object
  {
    DeclareRuntimeType();

    class Enumerator;
    class Iterator;

    /* Represents a node in a singly linked list */
    class Node
    {
      friend class SinglyLinkedList;
      friend class Enumerator;
      friend class Iterator;
    public:
      /* Construct an empty node */
      Node()
      { m_pNext = 0 ; }
      /* Construct an node with a value */
      Node(CONST T& Value) : m_Value(Value)
      { m_pNext = 0; }

      /* Returns a pointer to the next node in the chain */
      Node* Next() CONST
      { return m_pNext; }

      /* Returns the node value */
      CONST T& Value() CONST
      { return m_Value; }
      T& Value() 
      { return m_Value; }
    protected:
      Node* m_pNext;
      T m_Value;
    };

    /* Represents an linked list enumerator */
    class Enumerator
    {
      friend class SinglyLinkedList;
    protected:
      Enumerator(CONST SinglyLinkedList* pList);
    public:
      inline operator BOOL() CONST;

      inline Enumerator operator ++ ();
      inline Enumerator& operator++ (int);

      BOOL IsValid() CONST;
      T& CurrentElement() CONST;

    protected:
      CONST SinglyLinkedList* m_pList;
      Node* m_pCurrentNode;
    };

    /* Represents a linked list iterator */
    class Iterator
    {
      friend class SinglyLinkedList;
    protected:
      Iterator(SinglyLinkedList* pList, Node* pNode);
    
    public:
      CONST T& Value() CONST
      { return m_pNode->m_Value; }
      T& Value()
      { return m_pNode->m_Value; }

      // pointer aritmatic
      INLINE Iterator operator ++ ();
      INLINE Iterator& operator++ (int);

      // comparison operators
      BOOL operator== (CONST Iterator& I) CONST;
      BOOL operator!= (CONST Iterator& I) CONST;

    protected:
      SinglyLinkedList* m_pList;
      Node* m_pCurrentNode;
    };

  public:
    /* Constructs an empty linked list */
    SinglyLinkedList();
    virtual ~SinglyLinkedList();

    /* Adds a value to the head of the list */
    VOID Add(CONST T& Value);
    /* Removes the head of the list */
    BOOL RemoveHead();
    /* Removes all the elements in the list */
    VOID RemoveAll();

    /* Performs an O(n) search of the list and returns an iterator corresponding to its position. */
    Iterator Find(CONST T& Value) CONST;

    /* Performs an O(n) iteration of the list and removes a value if found */
    VOID Remove(CONST T& Value);

    /* Returns the number of elements in the list */
    SIZE_T Count() CONST;
    /* Returns TRUE if the list is empty */
    BOOL IsEmpty() CONST;

    /* Returns an enumerator of all elements */
    Enumerator Elements() CONST;

    /* Returns an iterator to the head of the list */
    Iterator Begin();
    /* Returns an iterator past the end of the list */
    Iterator End();

  private:
    Node* m_pHead;
    SIZE_T m_Count;
  };

  template<class T, class ElementTraits=Traits<T>, class ThreadingModel=SingleThreadedModel>
  class DoublyLinkedList : public Object
  {
    DeclareRuntimeType();

    class Enumerator;
    class Iterator;

    /* Represents a node in a singly linked list */
    class Node
    {
      friend class DoublyLinkedList;
      friend class Enumerator;
      friend class Iterator;
    public:
      /* Construct an empty node */
      Node()
      { m_pNext = 0; m_pPrev = 0; }
      /* Construct an node with a value */
      Node(CONST T& Value) : m_Value(Value)
      { m_pNext = 0; m_pPrev = 0; }

      /* Returns a pointer to the next node in the chain */
      Node* Next() CONST
      { return m_pNext; }

      /* Returns a pointer to the previous node in the chain */
      Node* Previous() CONST
      { return m_pPrev; }

      /* Returns the node value */
      CONST T& Value() CONST
      { return m_Value; }
      T& Value() 
      { return m_Value; }
    protected:
      Node* m_pNext;
      Node* m_pPrev;
      T m_Value;
    };

    /* Represents an linked list enumerator */
    class Enumerator
    {
      friend class DoublyLinkedList;
    protected:
      Enumerator(CONST DoublyLinkedList* pList);
    public:
      inline operator BOOL() CONST;

      INLINE Enumerator operator ++ ();
      INLINE Enumerator& operator++ (int);

      INLINE Enumerator operator -- ();
      INLINE Enumerator& operator-- (int);

      BOOL IsValid() CONST;
      T& CurrentElement() CONST;

    protected:
      CONST DoublyLinkedList* m_pList;
      Node* m_pCurrentNode;
    };

    /* Represents a linked list iterator */
    class Iterator
    {
      friend class DoublyLinkedList;
    protected:
      Iterator(DoublyLinkedList* pList, Node* pNode);
    
    public:
      CONST T& Value() CONST
      { return m_pNode->m_Value; }
      T& Value()
      { return m_pNode->m_Value; }

      INLINE Iterator operator ++ ();
      INLINE Iterator& operator++ (int);

      INLINE Iterator operator -- ();
      INLINE Iterator& operator-- (int);

      BOOL operator== (CONST Iterator& I) CONST;
      BOOL operator!= (CONST Iterator& I) CONST;

    protected:
      DoublyLinkedList* m_pList;
      Node* m_pCurrentNode;
    };

  public:
    /* Constructs an empty linked list */
    DoublyLinkedList();
    virtual ~DoublyLinkedList();

    /* Adds a value to the head of the list */
    VOID Add(CONST T& Value);
    VOID AddHead(CONST T& Value);

    /* Adds a value to the tail of the list */
    VOID AddTail(CONST T& Value);

    /* Removes the head of the list */
    BOOL RemoveHead();
    /* Removes the tail of the list */
    BOOL RemoveTail();
    /* Removes all the elements in the list */
    VOID RemoveAll();

    /* Performs an O(n) search of the list and returns an iterator corresponding to the first instance.  Returns End() if the value is not found */
    Iterator Find(CONST T& Value) CONST;

    /* Performs an O(n) iteration of the list and removes all instances of value if found */
    VOID Remove(CONST T& Value);

    /* Removes the element at a given position */
    VOID Remove(Iterator I);

    VOID Insert(Iterator I, CONST T& Value);
    /* Inserts a value at a position within the list */
    VOID InsertBefore(Iterator I, CONST T& Value);
    /* Inserts a value at a position within the list */
    VOID InsertAfter(Iterator I, CONST T& Value);

    /* Returns the number of elements in the list */
    SIZE_T Count() CONST;
    /* Returns TRUE if the list is empty */
    BOOL IsEmpty() CONST;

    /* Returns an enumerator of all elements */
    Enumerator Elements() CONST;

    /* Returns an iterator to the head of the list */
    Iterator Begin();
    /* Returns an iterator to the head of the list */
    Iterator Head();
    /* Returns an iterator to the tail of the list */
    Iterator Tail();

    /* Returns an iterator past the end of the list */
    Iterator End();

  private:
    Node* m_pHead;
    Node* m_pTail;

    SIZE_T m_Count;
  };

  #include "LinkedListImpl.hpp"

}
