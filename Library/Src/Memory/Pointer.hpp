/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Memory/Pointer.hpp $
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

#include "Utility/NonCopyable.hpp"

namespace Bolitho
{

  template<class T>
  class ScopedPtr : public NonCopyable
  {
  public:
    ScopedPtr();
    template<class U>
    ScopedPtr(U* p);
    ~ScopedPtr();

    template<class U>
    VOID Reset(U* p = 0);

    template<class U>
    ScopedPtr& operator= (U* P);

    T& operator*();
    CONST T& operator*() CONST;
    T* operator->();
    CONST T* operator->() CONST;
    T* Get();
    CONST T* Get() CONST;

    operator BOOL() CONST;
    BOOL operator!() CONST;

    VOID Swap(ScopedPtr& b);
  private:
    T* m_Ptr;
  };

  template<class T>
  VOID Swap(ScopedPtr<T>& a, ScopedPtr<T>& b);

  template<class T, class U>
  BOOL operator== (CONST ScopedPtr<T>& A, CONST U* B);

  template<class T, class U>
  BOOL operator!= (CONST ScopedPtr<T>& A, CONST U* B);

  template<class T>
  class ScopedArray : NonCopyable
  {
    ScopedArray(T* p = 0);
    ~ScopedArray();

    VOID Reset(T* p = 0);

    T& operator[] (SIZE_T i); 
    CONST T& operator[] (SIZE_T i) CONST; 
    
    T* Get();
    CONST T* Get() CONST;

    operator BOOL() CONST;
    BOOL operator!() CONST;

    VOID Swap(ScopedArray& b);
  private:
    T* m_Ptr;
  };

  template<class T>
  VOID Swap(ScopedArray<T>& a, ScopedArray<T>& b);

  /* Represents a smart pointer to an object.  The object is free'd once the reference count reaches zero */
  template<class T>
  class Ptr
  {
  public:
    /* Represents a NULL pointer */
    STATIC CONST Ptr<T> Null;
    /* Constructs a NULL pointer */
    Ptr();
    /* Constructs a new pointer to an existing object.  The objects reference count is increased by one */
    Ptr(CONST Ptr& P);
    template<class U>
    Ptr(U* p);

    template<class U>
    Ptr(CONST Ptr<U>& P);

    ~Ptr();

    Ptr& operator= (CONST Ptr& P);
    template<class U>
    Ptr& operator= (CONST Ptr<U>& P);
    template<class U>
    Ptr& operator= (U* P);
    Ptr& operator= (T* p);

    VOID AddRef();
    VOID Release();

    VOID Reset(T* p = 0);

    T& operator*();
    CONST T& operator*() CONST;
    T* operator->();
    CONST T* operator->() CONST;
    T* Get();
    CONST T* Get() CONST;

    operator BOOL() CONST;
    BOOL operator!() CONST;

  protected:
    template<class Y> friend class Ptr;

   T* m_Ptr;
  };

  template<class T, class U>
  BOOL operator== (CONST Ptr<T>& A, CONST Ptr<U>& B);

  template<class T, class U>
  BOOL operator!= (CONST Ptr<T>& A, CONST Ptr<U>& B);

  template<class T, class U>
  BOOL operator== (CONST Ptr<T>& A, CONST U* B);

  template<class T, class U>
  BOOL operator!= (CONST Ptr<T>& A, CONST U* B);
 
  template<class T>
  class SharedArray
  {
  public:
    SharedArray();
    SharedArray(CONST SharedArray& P);
    template<class U>
    SharedArray(U* p);
    template<class U>
    SharedArray(CONST SharedArray<U>& P);
    ~SharedArray();

    SharedArray& operator= (CONST SharedArray<T>& P);
    template<class U>
    SharedArray& operator= (CONST SharedArray<U>& P);
    template<class U>
    SharedArray& operator= (U* P);

    VOID AddRef();
    VOID Release();

    VOID Reset(T* p = 0);

    T& operator*();
    CONST T& operator*() CONST;
    T* operator->();
    CONST T* operator->() CONST;
    T* Get();
    CONST T* Get() CONST;

    operator BOOL() CONST;
    BOOL operator!() CONST;

  protected:
    template<class Y> friend class SharedArray;

    LONG* m_pRefCount;
    T* m_Ptr;
  };

  template<class T, class U>
  BOOL operator== (CONST SharedArray<T>& A, CONST SharedArray<U>& B);

  template<class T, class U>
  BOOL operator!= (CONST SharedArray<T>& A, CONST SharedArray<U>& B);

  template<class T>
  class ArrayPtr : public Array< Ptr<T> >
  {};

  #include "PointerImpl.hpp"
}
