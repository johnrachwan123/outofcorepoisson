/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/Vector4Impl.hpp $
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




//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector4<T>::Vector4() : VectorN<4,T>()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector4<T>::Vector4(T X, T Y, T Z, T W)
{
  m_V[0] = X;
  m_V[1] = Y;
  m_V[2] = Z;
  m_V[3] = W;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector4<T>::Vector4(CONST T* pV) : VectorN<4,T>(pV)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector4<T>::Vector4(CONST Vector4<T>& V) : VectorN<4,T>(V)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector4<T>::Vector4(CONST VectorN<4,T>& V) : VectorN<4,T>(V)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector4<T>::~Vector4()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector4<T>& Vector4<T>::operator= (CONST VectorN<4,T>& V)
{
  CopyMemory(m_V, V, sizeof(T)*4);
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& Vector4<T>::X() CONST
{
  return m_V[0];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T& Vector4<T>::X()
{
  return m_V[0];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& Vector4<T>::Y() CONST
{
  return m_V[1];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T& Vector4<T>::Y()
{
  return m_V[1];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& Vector4<T>::Z() CONST
{
  return m_V[2];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T& Vector4<T>::Z()
{
  return m_V[2];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& Vector4<T>::W() CONST
{
  return m_V[3];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T& Vector4<T>::W()
{
  return m_V[3];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Vector4<T>::SetX(T x)
{
  m_V[0] = x;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Vector4<T>::SetY(T y)
{
  m_V[1] = y;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Vector4<T>::SetZ(T z)
{
  m_V[2] = z;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Vector4<T>::SetW(T w)
{
  m_V[3] = w;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T Vector4<T>::GetX() CONST
{
  return m_V[0];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T Vector4<T>::GetY() CONST
{
  return m_V[1];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T Vector4<T>::GetZ() CONST
{
  return m_V[2];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T Vector4<T>::GetW() CONST
{
  return m_V[3];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<>
inline String Vector4<FLOAT>::ToString() CONST
{
  return String::Format(TEXT("(%f,%f,%f,%f)"), m_V[0], m_V[1], m_V[2], m_V[3]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<>
inline String Vector4<DOUBLE>::ToString() CONST
{
  return String::Format(TEXT("(%lf,%lf,%lf,%lf)"), m_V[0], m_V[1], m_V[2], m_V[3]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<>
inline String Vector4<INT>::ToString() CONST
{
  return String::Format(TEXT("(%d,%d,%d,%d)"), m_V[0], m_V[1], m_V[2], m_V[3]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

#if defined(USE_LUA)
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Lua::Bind::Scope Vector4<T>::GenerateBindings(LPCTSTR Name)
{
  using namespace Lua::Bind;

  return
    Class<Vector4<T>>(Name)
      .Def(Constructor<>())
      .Def(Constructor<T,T,T,T>())
      .Def(Constructor<CONST Vector4<T>&>())

      .Property("X", &Vector4<T>::GetX, &Vector4<T>::SetX)
      .Property("Y", &Vector4<T>::GetY, &Vector4<T>::SetY)
      .Property("Z", &Vector4<T>::GetZ, &Vector4<T>::SetZ)
      .Property("W", &Vector4<T>::GetW, &Vector4<T>::SetW)

      .Def("Dot", &Vector4<T>::Dot)

      .Property("Length", &Vector4<T>::Length)

      .Property("SquareLength", &Vector4<T>::SquareLength)
      .Def("Norm", &Vector4<T>::Norm)
      .Def("Normalize", &Vector4<T>::Normalize)

      .Def(Self + Vector4<T>())
      .Def(Self - Vector4<T>())
      .Def(-Self)
      .Def(Self * T())
      .Def(Self / T())

      .Def(tostring(Self));
}
#endif
