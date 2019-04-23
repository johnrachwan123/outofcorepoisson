/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Collections/CircularArrayImpl.hpp $
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
ImplementTemplate4RuntimeType(Bolitho,CircularArray,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
CircularArray<T,ElementTraits,Allocator,ThreadingModel>::CircularArray()
{
  m_Length = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
CircularArray<T,ElementTraits,Allocator,ThreadingModel>::CircularArray(SIZE_T Length)
{
  m_Length = 0;
  Resize(Length);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
CircularArray<T,ElementTraits,Allocator,ThreadingModel>::CircularArray(CONST CircularArray& A)
{
  m_Length = A.m_Length;
  m_Buffer = A.m_Buffer;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
CircularArray<T,ElementTraits,Allocator,ThreadingModel>::CircularArray(CONST Array<T,ElementTraits>& A)
{
  m_Length = A.m_Length;
  m_Buffer = A.m_Buffer;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
CircularArray<T,ElementTraits,Allocator,ThreadingModel>& CircularArray<T,ElementTraits,Allocator,ThreadingModel>::operator= (CONST CircularArray& A)
{
  m_Length = A.m_Length;
  m_Buffer = A.m_Buffer;

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
CircularArray<T,ElementTraits,Allocator,ThreadingModel>::~CircularArray()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
VOID CircularArray<T,ElementTraits,Allocator,ThreadingModel>::Resize(SIZE_T Length)
{
  m_Length = Length;
  m_Buffer.Resize(Length);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
SIZE_T CircularArray<T,ElementTraits,Allocator,ThreadingModel>::Length() CONST
{
  return m_Length;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
BOOL CircularArray<T,ElementTraits,Allocator,ThreadingModel>::IsEmpty() CONST
{
  return m_Length == 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
CONST T& CircularArray<T,ElementTraits,Allocator,ThreadingModel>::operator[] (PTRDIFF_T i) CONST
{
  if(i < 0)
    return m_Buffer[(m_Length-i) % i];
  else
    return m_Buffer[i % m_Length];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
T& CircularArray<T,ElementTraits,Allocator,ThreadingModel>::operator[] (PTRDIFF_T i)
{
  if(i < 0)
    return m_Buffer[(m_Length-i) % i];
  else
    return m_Buffer[i % m_Length];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
CONST T& CircularArray<T,ElementTraits,Allocator,ThreadingModel>::operator() (PTRDIFF_T i) CONST
{
  if(i < 0)
    return m_Buffer[(m_Length-i) % i];
  else
    return m_Buffer[i % m_Length];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
T& CircularArray<T,ElementTraits,Allocator,ThreadingModel>::operator() (PTRDIFF_T i)
{
  if(i < 0)
    return m_Buffer[(m_Length-i) % i];
  else
    return m_Buffer[i % m_Length];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
T CircularArray<T,ElementTraits,Allocator,ThreadingModel>::Get(PTRDIFF_T i) CONST
{
  if(i < 0)
    return m_Buffer[(m_Length-i) % i];
  else
    return m_Buffer[i % m_Length];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
VOID CircularArray<T,ElementTraits,Allocator,ThreadingModel>::Set(PTRDIFF_T i, CONST T& Item)
{
  if(i < 0)
    m_Buffer[(m_Length-i) % i] = Item;
  else
    m_Buffer[i % m_Length] = Item;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

