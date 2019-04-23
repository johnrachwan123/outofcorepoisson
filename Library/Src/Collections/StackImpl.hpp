/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Collections/StackImpl.hpp $
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




//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementTemplate3RuntimeType(Bolitho,Stack,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
Stack<T,ElementTraits,ThreadingModel>::Stack()
{
  m_TopOfStack = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
Stack<T,ElementTraits,ThreadingModel>::Stack(SIZE_T N) :
  m_Buffer(N)
{
  m_TopOfStack = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
Stack<T,ElementTraits,ThreadingModel>::Stack(CONST Stack& S)
{
  m_Buffer = S.m_Buffer;
  m_TopOfStack = S.m_TopOfStack;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
Stack<T,ElementTraits,ThreadingModel>::~Stack()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
VOID Stack<T,ElementTraits,ThreadingModel>::Push(CONST T& Item)
{
  ThreadingModel::Lock(m_Lock);

  m_TopOfStack++;
  if (m_TopOfStack >= m_Buffer.Length())
    if (m_TopOfStack < 16)
		  m_Buffer.Resize(16);
    else
		  m_Buffer.Resize(m_TopOfStack * 3 / 2);

  m_Buffer[m_TopOfStack-1] = Item;

  ThreadingModel::Unlock(m_Lock);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
T Stack<T,ElementTraits,ThreadingModel>::Pop()
{
  ThreadingModel::Lock(m_Lock);

  T Item = m_Buffer[m_TopOfStack-1];
  m_TopOfStack--;

  ThreadingModel::Unlock(m_Lock);

  return Item;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
CONST T& Stack<T,ElementTraits,ThreadingModel>::Peek() CONST
{
  ThreadingModel::Lock(m_Lock);
  
  Assert(m_TopOfStack > 0);
  CONST T& Item = m_Buffer[m_TopOfStack-1];

  ThreadingModel::Unlock(m_Lock);

  return Item;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
T& Stack<T,ElementTraits,ThreadingModel>::Peek()
{
  ThreadingModel::Lock(m_Lock);
  
  Assert(m_TopOfStack > 0);
  T& Item = m_Buffer[m_TopOfStack-1];

  ThreadingModel::Unlock(m_Lock);

  return Item;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
SIZE_T Stack<T,ElementTraits,ThreadingModel>::Length() CONST
{
  return m_TopOfStack;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
SIZE_T Stack<T,ElementTraits,ThreadingModel>::Capacity() CONST
{
  return m_Buffer.Length();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
BOOL Stack<T,ElementTraits,ThreadingModel>::IsEmpty() CONST
{
  return m_TopOfStack == 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
VOID Stack<T,ElementTraits,ThreadingModel>::Resize(SIZE_T N)
{
  ThreadingModel::Lock(m_Lock);

  m_Buffer.Resize(N);
  m_TopOfStack = N;

  ThreadingModel::Unlock(m_Lock);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class ThreadingModel>
VOID Stack<T,ElementTraits,ThreadingModel>::Reserve(SIZE_T N)
{
  ThreadingModel::Lock(m_Lock);

  if (N <= m_Buffer.Length())
    return;
  m_Buffer.Resize(N);  

  ThreadingModel::Unlock(m_Lock);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
