/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Memory/PointerImpl.hpp $
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
template<class T>
ScopedPtr<T>::ScopedPtr()
{
  m_Ptr = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
template<class U>
ScopedPtr<T>::ScopedPtr(U* p)
{
  m_Ptr = p;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
ScopedPtr<T>::~ScopedPtr()
{
  if (m_Ptr)
    delete m_Ptr;
  m_Ptr = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
template<class U>
VOID ScopedPtr<T>::Reset(U* p)
{
  Assert(p != m_Ptr);

  if (m_Ptr)
    delete m_Ptr;
  m_Ptr = p;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
template<class U>
ScopedPtr<T>& ScopedPtr<T>::operator= (U* p)
{
  Assert(p != m_Ptr);

  if (m_Ptr)
    delete m_Ptr;
  m_Ptr = p;
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& ScopedPtr<T>::operator*() CONST
{
  return *m_Ptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T& ScopedPtr<T>::operator*()
{
  return *m_Ptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T* ScopedPtr<T>::operator->() CONST
{
  return m_Ptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T* ScopedPtr<T>::operator->()
{
  return m_Ptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T* ScopedPtr<T>::Get() CONST
{
  return m_Ptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T* ScopedPtr<T>::Get()
{
  return m_Ptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
ScopedPtr<T>::operator BOOL() CONST
{
  return m_Ptr ? TRUE : FALSE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
BOOL ScopedPtr<T>::operator!() CONST
{
  return m_Ptr ? TRUE : FALSE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID ScopedPtr<T>::Swap(ScopedPtr& b)
{
  T* Ptr = m_Ptr;
  m_Ptr = b.m_Ptr;
  b.m_Ptr = Ptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Swap(ScopedPtr<T>& a, ScopedPtr<T>& b)
{
  a.Swap(b);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class U>
BOOL operator== (CONST ScopedPtr<T>& A, CONST U* B)
{
  return A.m_Ptr == B;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class U>
BOOL operator!= (CONST ScopedPtr<T>& A, CONST U* B)
{
  return A.m_Ptr != B;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
ScopedArray<T>::ScopedArray(T* p)
{
  m_Ptr = p;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
ScopedArray<T>::~ScopedArray()
{
  if (m_Ptr)
    delete[] m_Ptr;
  m_Ptr = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID ScopedArray<T>::Reset(T* p)
{
  if (m_Ptr)
    delete[] m_Ptr;
  m_Ptr = p;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T& ScopedArray<T>::operator[] (SIZE_T i)
{
  return m_Ptr[i];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& ScopedArray<T>::operator[] (SIZE_T i) CONST
{
  return m_Ptr[i];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T* ScopedArray<T>::Get()
{
  return m_Ptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T* ScopedArray<T>::Get() CONST
{
  return m_Ptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
ScopedArray<T>::operator BOOL() CONST
{
  return m_Ptr ? TRUE : FALSE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
BOOL ScopedArray<T>::operator!() CONST
{
  return m_Ptr ? TRUE : FALSE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID ScopedArray<T>::Swap(ScopedArray& b)
{
  T* Ptr = m_Ptr;
  m_Ptr = b.m_Ptr;
  b.m_Ptr = Ptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Swap(ScopedArray<T>& a, ScopedArray<T>& b)
{
  a.Swap(b);  
}
//----------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST Ptr<T> Ptr<T>::Null = Ptr<T>((T*)0);
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Ptr<T>::Ptr()
{
  m_Ptr = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Ptr<T>::Ptr(CONST Ptr& P)
{
  m_Ptr = P.m_Ptr;
  AddRef();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
template<class U>
Ptr<T>::Ptr(U* p)
{
  m_Ptr = dynamic_cast<T*>(p);
  AddRef();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
template<class U>
Ptr<T>::Ptr(CONST Ptr<U>& P)
{
  m_Ptr = dynamic_cast<T*>(P.m_Ptr);
  AddRef();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Ptr<T>::~Ptr()
{
  Release();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Ptr<T>::AddRef()
{
  if (m_Ptr)
    m_Ptr->AddRef();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Ptr<T>::Release()
{
  if (m_Ptr)
  {
    m_Ptr->Release();
    m_Ptr = 0;
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Ptr<T>& Ptr<T>::operator= (CONST Ptr& P)
{
  if (P.m_Ptr == m_Ptr)
    return *this;

  if (m_Ptr != 0)
    Release();

  if (P.m_Ptr)
  {
    m_Ptr = P.m_Ptr;
    AddRef();
  }

  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
template<class U>
Ptr<T>& Ptr<T>::operator= (CONST Ptr<U>& P)
{
  if (P.m_Ptr == m_Ptr)
    return *this;

  Release();

  if (P.m_Ptr)
  {
    m_Ptr = (T*)P.m_Ptr;
    AddRef();
  }

  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
template<class U>
Ptr<T>& Ptr<T>::operator= (U* P)
{
  Release();

  m_Ptr = P;

  if (P)
    AddRef(); 

  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Ptr<T>& Ptr<T>::operator= (T* P)
{
  Release();

  m_Ptr = P;
  if (P)
    AddRef(); 

  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Ptr<T>::Reset(T* p)
{
  Assert(FALSE);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T& Ptr<T>::operator*()
{
  return *m_Ptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& Ptr<T>::operator*() CONST
{
  return *m_Ptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T* Ptr<T>::operator->()
{
  return m_Ptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T* Ptr<T>::operator->() CONST
{
  return m_Ptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T* Ptr<T>::Get()
{
  return m_Ptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T* Ptr<T>::Get() CONST
{
  return m_Ptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Ptr<T>::operator BOOL() CONST
{
  return m_Ptr != 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
BOOL Ptr<T>::operator!() CONST
{
  return m_Ptr != 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class U>
BOOL operator== (CONST Ptr<T>& A, CONST Ptr<U>& B)
{ 
  return A.Get() == B.Get(); 
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class U>
BOOL operator!= (CONST Ptr<T>& A, CONST Ptr<U>& B)
{ 
  return A.Get() != B.Get(); 
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class U>
BOOL operator== (CONST Ptr<T>& A, CONST U* B)
{ 
  return A.Get() == B; 
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class U>
BOOL operator!= (CONST Ptr<T>& A, CONST U* B)
{ 
  return A.Get() != B; 
}
//----------------------------------------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
SharedArray<T>::SharedArray()
{
  m_Ptr = 0;
  m_pRefCount = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
SharedArray<T>::SharedArray(CONST SharedArray& P)
{
  m_pRefCount = P.m_pRefCount;
  m_Ptr = P.m_Ptr;

  AddRef();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
template<class U>
SharedArray<T>::SharedArray(U* p = 0)
{
  m_Ptr = p;
  m_pRefCount = new LONG();

  AddRef();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
template<class U>
SharedArray<T>::SharedArray(CONST SharedArray<U>& P)
{
  m_pRefCount = P.m_pRefCount;
  m_Ptr = P.m_Ptr;

  AddRef();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
SharedArray<T>::~SharedArray()
{
  Release();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID SharedArray<T>::AddRef()
{
  if (!m_pRefCount)
    return;

  (*m_pRefCount)++;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID SharedArray<T>::Release()
{
  if (!m_pRefCount)
    return;

  (*m_pRefCount)--;
  if (*m_pRefCount == 0)
  {
    delete[] m_Ptr;
    delete m_pRefCount;

    m_Ptr = 0;
    m_pRefCount = 0;
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
SharedArray<T>& SharedArray<T>::operator= (CONST SharedArray<T>& P)
{
  if (P.m_Ptr == m_Ptr)
    return *this;

  if (m_Ptr != 0)
    Release();

  if (P.m_Ptr)
  {
    P.AddRef();
    m_Ptr = P.m_Ptr;
    m_pRefCount = P.m_pRefCount;
  }

  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
template<class U>
SharedArray<T>& SharedArray<T>::operator= (CONST SharedArray<U>& P)
{
  if (P.m_Ptr == m_Ptr)
    return *this;

  if (m_Ptr != 0)
    Release();

  if (P.m_Ptr)
  {
    m_Ptr = P.m_Ptr;
    m_pRefCount = P.m_pRefCount;
    AddRef();
  }

  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
template<class U>
SharedArray<T>& SharedArray<T>::operator= (U* P)
{
  m_Ptr = P;
  m_pRefCount = new LONG();

  AddRef(); 

  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID SharedArray<T>::Reset(T* p)
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T& SharedArray<T>::operator*()
{
  return *m_Ptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& SharedArray<T>::operator*() CONST
{
  return *m_Ptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T* SharedArray<T>::operator->()
{
  return m_Ptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T* SharedArray<T>::operator->() CONST
{
  return m_Ptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T* SharedArray<T>::Get()
{
  return m_Ptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T* SharedArray<T>::Get() CONST
{
  return m_Ptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
SharedArray<T>::operator BOOL() CONST
{
  return m_Ptr != 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
BOOL SharedArray<T>::operator!() CONST
{
  return m_Ptr != 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class U>
BOOL operator== (CONST SharedArray<T>& A, CONST SharedArray<U>& B)
{ 
  return A.Get() == B.Get(); 
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class U>
BOOL operator!= (CONST SharedArray<T>& A, CONST SharedArray<U>& B)
{ 
  return A.Get() == B.Get(); 
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
