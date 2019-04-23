/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Utility/Traits.hpp $
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

#pragma once

#include "IntegralConstant.hpp"
#include "StaticAssert.hpp"

namespace Bolitho
{

  namespace Internal
  {
    /* IsPointer */
    template <typename T> 
    struct IsPointer : public FalseType{};
    template <typename T> 
    struct IsPointer<T*> : public TrueType{};
    template <typename T> 
    struct IsPointer<CONST T*> : public TrueType{};
    template <typename T> 
    struct IsPointer<volatile T*> : public TrueType{};
    template <typename T> 
    struct IsPointer<CONST volatile T*> : public TrueType{};

    /* IsVoid */
    template <typename T> 
    struct IsVoid : public FalseType{};
    template <> 
    struct IsVoid<VOID> : public TrueType{};

    /* IsReference */
    template <typename T> 
    struct IsReference : public FalseType{};
    template <typename T> 
    struct IsReference<T&> : public TrueType{};
    template <typename T> 
    struct IsReference<CONST T&> : public TrueType{};

    /* IsInteger */
    template <typename T> 
    struct IsInteger : public FalseType{};
    template <> 
    struct IsInteger<INT> : public TrueType{};
    template <> 
    struct IsInteger<UINT> : public TrueType{};
    #if defined(PLATFORM_WINDOWS)
    template <> 
    struct IsInteger<LONG> : public TrueType{};
    template <> 
    struct IsInteger<ULONG> : public TrueType{};
    #endif
    template <> 
    struct IsInteger<LONGLONG> : public TrueType{};
    template <> 
    struct IsInteger<ULONGLONG> : public TrueType{};
    template <> 
    struct IsInteger<SHORT> : public TrueType{};
    template <> 
    struct IsInteger<USHORT> : public TrueType{};
    template <> 
    struct IsInteger<CHAR> : public TrueType{};
    template <> 
    struct IsInteger<UCHAR> : public TrueType{};

    /* IsSignedInteger */
    template <typename T> 
    struct IsSignedInteger : public FalseType{};
    template <> 
    struct IsSignedInteger<INT> : public TrueType{};
    #if defined(PLATFORM_WINDOWS)
    template <> 
    struct IsSignedInteger<LONG> : public TrueType{};
    #endif
    template <> 
    struct IsSignedInteger<LONGLONG> : public TrueType{};
    template <> 
    struct IsSignedInteger<SHORT> : public TrueType{};
    template <> 
    struct IsSignedInteger<CHAR> : public TrueType{};


    /* IsReal */
    template <typename T> 
    struct IsReal : public FalseType{};
    template <> 
    struct IsReal<FLOAT> : public TrueType{};
    template <> 
    struct IsReal<DOUBLE> : public TrueType{};

    /* IsPrimitive */
    template <typename T> 
    struct IsPrimitive : public FalseType{};
    template <typename T> 
    struct IsPrimitive<T*> : public TrueType{};
    template <> 
    struct IsPrimitive<CHAR> : public TrueType{};
    template <> 
    struct IsPrimitive<UCHAR> : public TrueType{};
    template <> 
    struct IsPrimitive<WCHAR> : public TrueType{};
    template <> 
    struct IsPrimitive<SHORT> : public TrueType{};
    template <> 
    struct IsPrimitive<USHORT> : public TrueType{};
    template <> 
    struct IsPrimitive<INT> : public TrueType{};
    template <> 
    struct IsPrimitive<UINT> : public TrueType{};
    #if defined(PLATFORM_WINDOWS)
    template <> 
    struct IsPrimitive<LONG> : public TrueType{};
    template <> 
    struct IsPrimitive<ULONG> : public TrueType{};
    #endif
    template <> 
    struct IsPrimitive<LONGLONG> : public TrueType{};
    template <> 
    struct IsPrimitive<ULONGLONG> : public TrueType{};
    template <> 
    struct IsPrimitive<FLOAT> : public TrueType{};
    template <> 
    struct IsPrimitive<DOUBLE> : public TrueType{};
    template <> 
    struct IsPrimitive<bool> : public TrueType{};

    /* IsCharacter */
    template <typename T> 
    struct IsCharacter : public FalseType{};
    template <> 
    struct IsCharacter<CHAR> : public TrueType{};
    template <> 
    struct IsCharacter<WCHAR> : public TrueType{};
    /* IsSerializable */

  }

  template<class T>
  struct Traits
  {
    static BOOL CompareLessThan(CONST T& A, CONST T& B)
    { return A < B; }
    static BOOL CompareEqual(CONST T& A, CONST T& B)
    { return A == B; }
    static DWORD Hash(CONST T& A)
    { return (DWORD)A; }
    static VOID Swap(T& A, T& B)
    { T C = A; A = B; B = C; }

    static const bool IsPrimitive = Bolitho::Internal::IsPrimitive<T>::Value;
    static const bool IsVoid = Bolitho::Internal::IsVoid<T>::Value;
    static const bool IsPointer = Bolitho::Internal::IsPointer<T>::Value;
    static const bool IsReference = Bolitho::Internal::IsReference<T>::Value;
    static const bool IsNumber = Bolitho::Internal::IsInteger<T>::Value || Bolitho::Internal::IsReal<T>::Value;
    static const bool IsInteger = Bolitho::Internal::IsInteger<T>::Value;
    static const bool IsSignedInteger = Bolitho::Internal::IsSignedInteger<T>::Value;
    static const bool IsUnsignedInteger = Bolitho::Internal::IsSignedInteger<T>::Value && !Bolitho::Internal::IsSignedInteger<T>::Value;
    static const bool IsReal = Bolitho::Internal::IsReal<T>::Value;
    static const bool IsCharacter = Bolitho::Internal::IsCharacter<T>::Value;
  };

  template<class T>
  struct PointerTraits
  {
    static BOOL CompareLessThan(CONST T* A, CONST T* B)
    { return Traits<T>::CompareLessThan(*A,*B); }
    static BOOL CompareEqual(CONST T& A, CONST T& B)
    { return Traits<T>::CompareEqual(*A,*B); }
    static DWORD Hash(CONST T* A)
    { return Traits<T>::Hash(*A); }
    static VOID Swap(T*& A, T*& B)
    { T* C = A; A = B; B = C; }

    static const bool IsPrimitive = Bolitho::Internal::IsPrimitive<T>::Value;
    static const bool IsVoid = Bolitho::Internal::IsVoid<T>::Value;
    static const bool IsPointer = Bolitho::Internal::IsPointer<T*>::Value;
    static const bool IsReference = Bolitho::Internal::IsReference<T>::Value;
    static const bool IsNumber = Bolitho::Internal::IsInteger<T>::Value || Bolitho::Internal::IsReal<T>::Value;
    static const bool IsInteger = Bolitho::Internal::IsInteger<T>::Value;
    static const bool IsSignedInteger = Bolitho::Internal::IsSignedInteger<T>::Value;
    static const bool IsUnsignedInteger = Bolitho::Internal::IsSignedInteger<T>::Value && !Bolitho::Internal::IsSignedInteger<T>::Value;
    static const bool IsReal = Bolitho::Internal::IsReal<T>::Value;
    static const bool IsCharacter = Bolitho::Internal::IsCharacter<T>::Value;
  };


  template<class T>
  struct IntegerTraits
  {
    StaticAssert(Traits<T>::IsInteger);

    static LIBRARY_API CONST T MinValue;
    static LIBRARY_API CONST T MaxValue;
  };

  template<class T>
  struct RealTraits
  {
    StaticAssert(Traits<T>::IsReal);

    static LIBRARY_API CONST T MinValue;
    static LIBRARY_API CONST T MaxValue;
    static LIBRARY_API CONST T Epsilon;

  };


  template<>
  INLINE DWORD Traits<FLOAT>::Hash(CONST FLOAT& A)
  { return *(DWORD*)(&A); }

  template<class T>
  VOID Swap(T& A, T& B)
  { Traits<T>::Swap(A,B); }

 template<class T>
  T Min(CONST T& A, CONST T& B)
  { return (A < B) ? A : B; }
  template<class T>
  T Min(CONST T& A, CONST T& B, CONST T& C)
  { return Min(Min(A,B), C); }
  template<class T>
  T Max(CONST T& A, CONST T& B)
  { return (A > B) ? A : B; }
  template<class T>
  T Max(CONST T& A, CONST T& B, CONST T& C)
  { return Max(Max(A,B),C); }

  template<class T>
  T Clamp(CONST T& X, CONST T& Min, CONST T& Max)
  {
    if (X < Min)
      return Min;
    if (X > Max)
      return Max;
    return X;
  } 

  template<class T>
  T Sum(CONST T& X0, CONST T& X1)
  { return X0 + X1; }
  template<class T>
  T Sum(CONST T& X0, CONST T& X1, CONST T& X2)
  { return X0 + X1 + X2; }
  template<class T>
  T Sum(CONST T& X0, CONST T& X1, CONST T& X2, CONST T& X3)
  { return X0 + X1 + X2 + X3; }
  template<class T>
  T Sum(CONST T* X, SIZE_T N)
  { T S = T(); for (SIZE_T i=0; i<N; i++) S += X[i]; return S; }
  template<class T, SIZE_T N>
  T Sum(CONST T* X)
  { T S = T(); for (SIZE_T i=0; i<N; i++) S += X[i]; return S; } 
}


