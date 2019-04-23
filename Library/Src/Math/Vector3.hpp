/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/Vector3.hpp $
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
#include "Math/VectorN.hpp"

namespace Bolitho
{
  namespace Math
  {
    template<class T>
    class Vector3 : public VectorN<3,T>
    {
      using VectorN<3,T>::m_V;
   public:
      Vector3();
      Vector3(T X, T Y, T Z);
      Vector3(CONST T* pV);
      Vector3(CONST Vector3<T>& V);
      Vector3(CONST VectorN<3,T>& V);
      template<class U>
      Vector3(CONST Vector3<U>& V);
      template<class U>
      Vector3(CONST VectorN<3,U>& V);
      ~Vector3();
      
      Vector3<T>& operator= (CONST VectorN<3,T>& V);
      template<class U>
      Vector3<T>& operator= (CONST VectorN<3,U>& V);
      
      // element access
      CONST T& X() CONST;
      T& X();
      CONST T& Y() CONST;
      T& Y();
      CONST T& Z() CONST;
      T& Z();
      
      VOID SetX(T x);
      VOID SetY(T y);
      VOID SetZ(T z);
      
      T GetX() CONST;
      T GetY() CONST;
      T GetZ() CONST;
      
   
      Vector3<T> Cross(CONST Vector3<T>& V) CONST;
      
      String ToString() CONST;

    };
    
#include "Vector3Impl.hpp"
    
    typedef Vector3<FLOAT> Vector3F;
    typedef Vector3<DOUBLE> Vector3D;
    typedef Vector3<INT> Vector3I;
    
    typedef Vector3<FLOAT> Point3F;
    typedef Vector3<DOUBLE> Point3D;
    typedef Vector3<INT> Point3I;
  }
}
