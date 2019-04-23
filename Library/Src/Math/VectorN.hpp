/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/VectorN.hpp $
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

#include "Math/Math.hpp"

namespace Bolitho
{
  namespace Math
  {
    struct Swizzle
    { 
      enum 
      {
        X = 0x0000,
        Y = 0x0001,
        Z = 0x0002,
        W = 0x0003,
        
        XX = 0x0000,
        YY = 0x0011,
        ZZ = 0x0022,
        WW = 0x0033,

        XY = 0x0010,
        YX = 0x0001,
      };
    };

    template<SIZE_T N, class T>
    class VectorN  
    {
    public:
      VectorN();
      VectorN(CONST T* pV);
      VectorN(CONST VectorN<N,T>& V);
      ~VectorN();
      
      CONST T& operator() (SIZE_T i) CONST;
      T& operator() (SIZE_T i);
      
      
      operator CONST T*() CONST;
      operator T*();
      
      VOID SetZero();
      
      SIZE_T Dimensions() CONST;
      
      // Vector arithmetic
      VectorN<N,T> operator * (CONST T& A) CONST;
      VectorN<N,T> operator / (CONST T& A) CONST;
      VectorN<N,T> operator - (CONST VectorN<N,T>& V) CONST;
      VectorN<N,T> operator + (CONST VectorN<N,T>& V) CONST;
      
      VectorN<N,T>& operator *= (CONST T& A);
      VectorN<N,T>& operator /= (CONST T& A);
      VectorN<N,T>& operator += (CONST VectorN<N,T>& V);
      VectorN<N,T>& operator -= (CONST VectorN<N,T>& V);
      
      // Negation
      VectorN<N,T> operator- () CONST;
      
      // Assignment operator
      VectorN<N,T>& operator = (CONST VectorN<N,T>& V);
      
      // Vector operations
      T Dot(CONST VectorN<N,T>& V) CONST;
      T Length() CONST;
      T SquareLength() CONST;
      T Norm(UINT Ln) CONST;
      VOID Normalize();
      
      // Vector comparison
      BOOL operator == (CONST VectorN<N,T>& V);
      BOOL operator != (CONST VectorN<N,T>& V);
      BOOL operator <= (CONST VectorN<N,T>& V);
      BOOL operator >= (CONST VectorN<N,T>& V);
      BOOL operator <  (CONST VectorN<N,T>& V);
      BOOL operator >  (CONST VectorN<N,T>& V);
      
      String ToString() CONST;
      
      static VectorN<N,T> Random();

    protected:
      T m_V[N];
    };
    
    template<SIZE_T N, class T>
    VectorN<N,T> operator * (T Scalar, CONST VectorN<N,T>& V);
    
    template<SIZE_T N, class T>
    VectorN<N,T> operator / (T Scalar, CONST VectorN<N,T>& V);
    
#include "VectorNImpl.hpp"
    
    template<SIZE_T N, class T>
    Bolitho::IO::BinaryStream& operator << (Bolitho::IO::BinaryStream& S, CONST VectorN<N,T> x)
    { S.Write(&x[0], sizeof(T)*N); return S; }
    
    template<SIZE_T N, class T>
    Bolitho::IO::BinaryStream& operator >> (Bolitho::IO::BinaryStream& S, VectorN<N,T> x)
    { S.Read(&x[0], sizeof(T)*N); return S; }
    
  }
}
