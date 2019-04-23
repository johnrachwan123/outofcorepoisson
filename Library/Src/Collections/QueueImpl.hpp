/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Collections/QueueImpl.hpp $
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
ImplementTemplate3RuntimeType(Bolitho, Queue, Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
Queue<T,ElementTraits,ThreadingModel>::Queue()
{
  m_Head = 0;
  m_Tail = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
Queue<T,ElementTraits,ThreadingModel>::Queue(SIZE_T N) :
  m_Buffer(N)
{
  m_Head = 0;
  m_Tail = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
Queue<T,ElementTraits,ThreadingModel>::Queue(CONST Queue& Q)
{
  m_Buffer = Q.m_Buffer;
  m_Tail = Q.m_Tail;
  m_Head = Q.m_Head;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
Queue<T,ElementTraits,ThreadingModel>::~Queue()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
VOID Queue<T,ElementTraits,ThreadingModel>::Enqueue(CONST T& Item)
{
  if (m_Tail < m_Head)
  {
    if (m_Head < m_Buffer.Length())
      m_Buffer[m_Head++] = Item;
    else
    {
      if (m_Tail > 0)
        m_Head = 0;
      else
      {
        if (m_Buffer.Length() < 16)
          m_Buffer.Resize(16);
        else
          m_Buffer.Resize((SIZE_T)((FLOAT)(m_Buffer.Length()+1) * 1.5));
      }
      m_Buffer[m_Head++] = Item;
    }
  }
  else if (m_Tail > m_Head)
    m_Buffer[m_Head++] = Item;
  else if (m_Tail == m_Head)
  {
    if (m_Tail == 0)
    {
      if (m_Buffer.Length() < 16)
        m_Buffer.Resize(16);
      m_Buffer[m_Head++] = Item;
    }
    else
    {
      SIZE_T TailSize = m_Buffer.Length() - m_Tail;
      Buffer<T> NewBuffer((SIZE_T)((FLOAT)(m_Buffer.Length()+1) * 1.5));
      CopyObjects(&NewBuffer[0] , &m_Buffer[m_Tail], TailSize);
      CopyObjects(&NewBuffer[TailSize] , &m_Buffer[0], m_Head);
      m_Tail = 0;
      m_Head = m_Buffer.Length();
      m_Buffer = NewBuffer;
      m_Buffer[m_Head++] = Item;
    }
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
T Queue<T,ElementTraits,ThreadingModel>::Dequeue()
{
  Assert(!IsEmpty());
  
  if (m_Tail <= m_Head)
  {
    T Item = m_Buffer[m_Tail++];
    if (m_Tail == m_Head)
      m_Tail = m_Head = 0;
    return Item;
  }
  else
  {
    T Item = m_Buffer[m_Tail++];
    if (m_Tail == m_Head)
      m_Tail = m_Head = 0;
    if (m_Tail >= m_Buffer.Length())
      m_Tail = 0;

    return Item;
  }

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
CONST T& Queue<T,ElementTraits,ThreadingModel>::Peek() CONST
{
  Assert(!IsEmpty());
  return m_Buffer[m_Head-1];  
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
T& Queue<T,ElementTraits,ThreadingModel>::Peek()
{
  Assert(!IsEmpty());
  return m_Buffer[m_Head-1];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
SIZE_T Queue<T,ElementTraits,ThreadingModel>::Length() CONST
{
  if (m_Tail <= m_Head)
    return m_Head-m_Tail;
  else
    return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
SIZE_T Queue<T,ElementTraits,ThreadingModel>::Capacity() CONST
{
  return m_Buffer.Length();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
BOOL Queue<T,ElementTraits,ThreadingModel>::IsEmpty() CONST
{
  return (m_Head == m_Tail && m_Head == 0);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
VOID Queue<T,ElementTraits,ThreadingModel>::Reserve(SIZE_T N)
{
  if (N > m_Buffer.Length())
    m_Buffer.Resize(N);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
