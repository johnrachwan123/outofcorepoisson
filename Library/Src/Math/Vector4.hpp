/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/Vector4.hpp $
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

#include "Math/VectorN.hpp"

namespace Bolitho
{
  namespace Math
  {
    template<class T>
    class Vector4 : public VectorN<4,T>
    {
      using VectorN<4,T>::m_V;
    public:
      Vector4();
      Vector4(T X, T Y, T Z, T W = T(1));
      Vector4(CONST T* pV);
      Vector4(CONST Vector4<T>& V);
      Vector4(CONST VectorN<4,T>& V);
      ~Vector4();
      
      Vector4<T>& operator= (CONST VectorN<4,T>& V);
      
      // element access
      CONST T& X() CONST;
      T& X();
      CONST T& Y() CONST;
      T& Y();
      CONST T& Z() CONST;
      T& Z();
      CONST T& W() CONST;
      T& W();
      
      VOID SetX(T x);
      VOID SetY(T y);
      VOID SetZ(T z);
      VOID SetW(T w);
      
      T GetX() CONST;
      T GetY() CONST;
      T GetZ() CONST;
      T GetW() CONST;

      String ToString() CONST;

      #if defined(USE_LUA)
      static Lua::Bind::Scope GenerateBindings(LPCTSTR Name);
      #endif

		};

    #include "Vector4Impl.hpp"

		typedef Vector4<FLOAT> Vector4F;
		typedef Vector4<DOUBLE> Vector4D;
		typedef Vector4<INT> Vector4I;

  }
}

