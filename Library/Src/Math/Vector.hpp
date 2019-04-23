/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/Vector.hpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1679 $
 * Last Updated: $Date: 2008-10-29 07:35:35 -0700 (Wed, 29 Oct 2008) $
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

#include "IO/File.hpp"

#pragma once

namespace Bolitho
{
  namespace Math
  {
    template<class T>
    class Vector : public Object
    {
      DeclareRuntimeType();
    public:
      Vector();
      Vector(CONST Vector<T>& V);
      Vector(SIZE_T N);
      Vector(SIZE_T N, CONST T* pV);
      virtual ~Vector();

      // Element access
      CONST T& operator () (SIZE_T i) CONST;
      T& operator () (SIZE_T i);
      CONST T& operator [] (SIZE_T i) CONST;
      T& operator [] (SIZE_T i);

      VOID Set(SIZE_T i, CONST T& Element);
      CONST T& Get(SIZE_T i) CONST;
      T& Get(SIZE_T i);

      // Set operations
      VOID SetZero();

      // Storage management
      SIZE_T Dimensions() CONST;
      VOID Resize(SIZE_T N);

      // Vector arithmetic
      Vector operator * (CONST T& A) CONST;
      Vector operator / (CONST T& A) CONST;
      Vector operator - (CONST Vector& V) CONST;
      Vector operator + (CONST Vector& V) CONST;

      Vector& operator *= (CONST T& A);
      Vector& operator /= (CONST T& A);
      Vector& operator += (CONST Vector& V);
      Vector& operator -= (CONST Vector& V);

      // Negation
      Vector operator- () CONST;

      // Assignment operator
      Vector& operator = (CONST Vector& V);

      // Vector operations
      T Dot(CONST Vector& V) CONST;
      T Length() CONST;
      T Norm(SIZE_T Ln) CONST;
      VOID Normalize();

      // Vector comparison
      BOOL operator == (CONST Vector<T>& V);
      BOOL operator != (CONST Vector<T>& V);
      BOOL operator <= (CONST Vector<T>& V);
      BOOL operator >= (CONST Vector<T>& V);
      BOOL operator <  (CONST Vector<T>& V);
      BOOL operator >  (CONST Vector<T>& V);

      VOID Write(LPCTSTR Filename);
      VOID Read(LPCTSTR Filename);
    protected:
      VOID Deallocate();
      INT Compare(T* pV);

      Buffer<T> m_pV;
    };

    template <class T>
    Vector<T> operator* (T fScalar, CONST Vector<T>& V);

    template <class T>
    Vector<T> operator/ (T fScalar, CONST Vector<T>& V);

    typedef Vector<FLOAT> VectorF;
    typedef Vector<DOUBLE> VectorD;
    typedef Vector<INT> VectorI;

    #include "VectorImpl.hpp"
  }
}

