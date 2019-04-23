/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Type.hpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1696 $
 * Last Updated: $Date: 2008-10-31 06:29:29 -0700 (Fri, 31 Oct 2008) $
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

#ifdef USE_TYPESYSTEM

#define DeclareIntrinsicRuntimeType(T)                                                          \
  LIBRARY_API CONST Bolitho::Type* GetType(T x);                                              \

#define ImplementIntrinsicRuntimeType(T)                                                        \
  LIBRARY_API CONST Bolitho::Type Type##T(TEXT(""), TEXT(#T), 0, 0, 0);													\
  LIBRARY_API CONST Bolitho::Type* GetType(T x)																								\
  {                                                                                             \
    return &Type##T;                                                                            \
  }                                                                                             \

#define DeclareRuntimeType()                                                                    \
public:                                                                                         \
  virtual CONST Bolitho::Type* GetType() CONST;                                                 \
  static CONST Bolitho::Type RuntimeType                                                         \

#define ImplementRuntimeType(ns,n,base)                                                         \
  CONST Bolitho::Type ns::n::RuntimeType = Bolitho::Type(TEXT(#ns), TEXT(#n), &base::RuntimeType, 0, 0); \
  CONST Bolitho::Type* ns::n::GetType() CONST				\
  {                                                                                             \
    return &n::RuntimeType;                                                                     \
  }                                                                                             \

#define ImplementTemplateRuntimeType(ns,n,base)                                                 \
  template<class T>                                                                             \
  CONST Bolitho::Type n<T>::RuntimeType =                                                       \
    Bolitho::Type(TEXT(#ns), TEXT(#n), &base::RuntimeType, Bolitho::GetType(T()), 0); \
                                                                                                \
  template<class T>                                                                             \
  CONST Bolitho::Type* n<T>::GetType() CONST                                                    \
  {                                                                                             \
    return &n::RuntimeType;                                                                     \
  }                                                                                             \

#define ImplementTemplate2RuntimeType(ns,n,base)                                                \
  template<class T, class U>                                                                    \
  CONST Bolitho::Type n<T,U>::RuntimeType =                                                     \
    Bolitho::Type(TEXT(#ns), TEXT(#n), &base::RuntimeType, Bolitho::GetType(T()), Bolitho::GetType(U()));   \
                                                                                                \
  template<class T, class U>                                                                    \
  CONST Bolitho::Type* n<T,U>::GetType() CONST                                                  \
  {                                                                                             \
    return &n::RuntimeType;                                                                     \
  }                                                                                             \

#define ImplementTemplate3RuntimeType(ns,n,base)                                                \
  template<class T, class U, class V>                                                           \
  CONST Bolitho::Type n<T,U,V>::RuntimeType =                                                   \
    Bolitho::Type(TEXT(#ns), TEXT(#n), &base::RuntimeType, Bolitho::GetType(T()), Bolitho::GetType(U()));   \
                                                                                                \
  template<class T, class U, class V>                                                           \
  CONST Bolitho::Type* n<T,U,V>::GetType() CONST                                                \
  {                                                                                             \
    return &n::RuntimeType;                                                                     \
  }                                                                                             \

#define ImplementTemplate4RuntimeType(ns,n,base)                                                \
  template<class T, class U, class V, class W>                                                  \
  CONST Bolitho::Type n<T,U,V,W>::RuntimeType =                                                 \
    Bolitho::Type(TEXT(#ns), TEXT(#n), &base::RuntimeType, Bolitho::GetType(T()), Bolitho::GetType(U()));   \
                                                                                                \
  template<class T, class U, class V, class W>                                                  \
  CONST Bolitho::Type* n<T,U,V,W>::GetType() CONST                                              \
  {                                                                                             \
    return &n::RuntimeType;                                                                     \
  }                                                                                             \

#define ImplementTemplate5RuntimeType(ns,n,base)                                                \
  template<class T, class U, class V, class W, class X>                                                  \
  CONST Bolitho::Type n<T,U,V,W,X>::RuntimeType =                                                 \
    Bolitho::Type(TEXT(#ns), TEXT(#n), &base::RuntimeType, Bolitho::GetType(T()), Bolitho::GetType(U()));   \
                                                                                                \
  template<class T, class U, class V, class W, class X>                                                  \
  CONST Bolitho::Type* n<T,U,V,W,X>::GetType() CONST                                              \
  {                                                                                             \
    return &n::RuntimeType;                                                                     \
  }                                                                                             \


#define ImplementTemplate6RuntimeType(ns,n,base)                                                \
  template<class T, class U, class V, class W, class X, class Y>                                \
  CONST Bolitho::Type n<T,U,V,W,X,Y>::RuntimeType =                                             \
    Bolitho::Type(TEXT(#ns), TEXT(#n), &base::RuntimeType, Bolitho::GetType(T()), Bolitho::GetType(U()));   \
                                                                                                \
  template<class T, class U, class V, class W, class X, class Y>                                \
  CONST Bolitho::Type* n<T,U,V,W,X,Y>::GetType() CONST                                          \
  {                                                                                             \
    return &n::RuntimeType;                                                                     \
  }                                                                                             \

#else

#define DeclareIntrinsicRuntimeType(T)                                                          \
  CONST Bolitho::Type* GetType(T x);                                                          \

#define ImplementIntrinsicRuntimeType(T)                                                        \
  LIBRARY_API CONST Bolitho::Type* Bolitho::GetType(T x)                                      \
  {                                                                                             \
    return &Bolitho::UnknownType;                                                               \
  }                                                                                             \

#define DeclareRuntimeType()                                                                    \
public:                                                                                         \
  virtual CONST Bolitho::Type* GetType() CONST;                                                 \

#define ImplementRuntimeType(ns,n,base)                                                         \
  CONST Bolitho::Type* n::GetType() CONST                                                       \
  {                                                                                             \
    return &Bolitho::UnknownType;                                                               \
  }                                                                                             \

#define ImplementTemplateRuntimeType(ns,n,base)                                                 \
  template<class T>                                                                             \
  CONST Type* n<T>::GetType() CONST                                                             \
  {                                                                                             \
    return &Bolitho::UnknownType;                                                               \
  }                                                                                             \

#define ImplementTemplate2RuntimeType(ns,n,base)                                                \
  template<class T, class U>                                                                    \
  CONST Type* n<T,U>::GetType() CONST                                                           \
  {                                                                                             \
    return &Bolitho::UnknownType;                                                               \
  }                                                                                             \

#define ImplementTemplate3RuntimeType(ns,n,base)                                                \
  template<class T, class U>                                                                    \
  CONST Type* n<T,U>::GetType() CONST                                                           \
  {                                                                                             \
    return &Bolitho::UnknownType;                                                               \
  }                                                                                             \

#endif

namespace Bolitho
{
  class LIBRARY_API Type
  {
  public:
    virtual CONST Type* GetType() CONST;
    static CONST Type RuntimeType;
  public:
    Type(CONST TCHAR* Namespace, CONST TCHAR* Name, CONST Type* BaseClass0, CONST Type* TemplateParameter0 = 0, CONST Type* TemplateParameter1 = 0, CONST Type* TemplateParameter2 = 0);
    
    Object* CreateInstance() CONST;

    CONST TCHAR* Namespace() CONST;
    CONST TCHAR* Name() CONST;
    CONST Type* BaseClass(SIZE_T i = 0) CONST;
    CONST Type* Template(SIZE_T i = 0) CONST;
    
    BOOL IsDerivedFrom(CONST Type* T) CONST;
    
    String ToString() CONST;
    
  private:
    CONST TCHAR* m_Namespace;
    CONST TCHAR* m_Name;
    CONST Type* m_pBaseClass[4];
    CONST Type* m_pTemplate[4];
    Object* (*m_pFactory)(VOID);
  };

  CONST Type* GetType(CONST Object& O);

  DeclareIntrinsicRuntimeType(CHAR);
  DeclareIntrinsicRuntimeType(WCHAR);
  DeclareIntrinsicRuntimeType(BYTE);
  DeclareIntrinsicRuntimeType(SHORT);
  DeclareIntrinsicRuntimeType(USHORT);
  DeclareIntrinsicRuntimeType(INT);
  DeclareIntrinsicRuntimeType(UINT);
  DeclareIntrinsicRuntimeType(LONG);
  DeclareIntrinsicRuntimeType(ULONG);
  DeclareIntrinsicRuntimeType(LONGLONG);
  DeclareIntrinsicRuntimeType(ULONGLONG);
  DeclareIntrinsicRuntimeType(FLOAT);
  DeclareIntrinsicRuntimeType(DOUBLE);

  extern LIBRARY_API CONST Type UnknownType;
  template<class T>
  CONST Type* GetType(T)
  { return &UnknownType; }

  inline BOOL IsDerivedFrom(Object* o, CONST Type* pType)
  { return o->GetType()->IsDerivedFrom(pType); }

}

