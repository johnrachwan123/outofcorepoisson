/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/Vector3Impl.hpp $
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
Vector3<T>::Vector3() : VectorN<3,T>()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector3<T>::Vector3(T X, T Y, T Z)
{
  m_V[0] = X;
  m_V[1] = Y;
  m_V[2] = Z;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector3<T>::Vector3(CONST T* pV) : VectorN<3,T>(pV)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector3<T>::Vector3(CONST Vector3<T>& V) : VectorN<3,T>(V)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector3<T>::Vector3(CONST VectorN<3,T>& V) : VectorN<3,T>(V)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
template<class U>
Vector3<T>::Vector3(CONST Vector3<U>& V) : VectorN<3,T>()
{
  m_V[0] = (T)V(0);
  m_V[1] = (T)V(1);
  m_V[2] = (T)V(2);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
template<class U>
Vector3<T>::Vector3(CONST VectorN<3,U>& V) : VectorN<3,T>()
{
  m_V[0] = (T)V(0);
  m_V[1] = (T)V(1);
  m_V[2] = (T)V(2);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector3<T>::~Vector3()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector3<T>& Vector3<T>::operator= (CONST VectorN<3,T>& V)
{
  CopyMemory(m_V, V, sizeof(T)*3);
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
template<class U>
Vector3<T>& Vector3<T>::operator= (CONST VectorN<3,U>& V)
{
  m_V[0] = (T)V(0);
  m_V[1] = (T)V(1);
  m_V[2] = (T)V(2);
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& Vector3<T>::X() CONST
{
  return m_V[0];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T& Vector3<T>::X()
{
  return m_V[0];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& Vector3<T>::Y() CONST
{
  return m_V[1];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T& Vector3<T>::Y()
{
  return m_V[1];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& Vector3<T>::Z() CONST
{
  return m_V[2];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T& Vector3<T>::Z()
{
  return m_V[2];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Vector3<T>::SetX(T x)
{
  m_V[0] = x;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Vector3<T>::SetY(T y)
{
  m_V[1] = y;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Vector3<T>::SetZ(T z)
{
  m_V[2] = z;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T Vector3<T>::GetX() CONST
{
  return m_V[0];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T Vector3<T>::GetY() CONST
{
  return m_V[1];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T Vector3<T>::GetZ() CONST
{
  return m_V[2];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_DIRECTX9)
template<class T>
Vector3<T>::operator D3DVECTOR() CONST
{
  D3DVECTOR V;
  V[0] = (FLOAT)m_V[0];
  V[1] = (FLOAT)m_V[1];
  V.z = (FLOAT)m_V[2];
  return V;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<>
inline Vector3<FLOAT>::operator D3DVECTOR() CONST
{
  return *(CONST D3DVECTOR*)&m_V[0];
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector3<T> Vector3<T>::Cross(CONST Vector3<T>& V) CONST
{
  return Vector3(m_V[1]*V.m_V[2] - m_V[2]*V.m_V[1],
                  m_V[2]*V.m_V[0] - m_V[0]*V.m_V[2],
                  m_V[0]*V.m_V[1] - m_V[1]*V.m_V[0] );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<>
inline String Vector3<FLOAT>::ToString() CONST
{
  return String::Format(TEXT("(%f, %f, %f)"), m_V[0], m_V[1], m_V[2]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<>
inline String Vector3<DOUBLE>::ToString() CONST
{
  return String::Format(TEXT("(%lf, %lf, %lf)"), m_V[0], m_V[1], m_V[2]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<>
inline String Vector3<INT>::ToString() CONST
{
  return String::Format(TEXT("(%d, %d, %d)"), m_V[0], m_V[1], m_V[2]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

#if defined(USE_LUA)
template<class T>
Lua::Bind::Scope Vector3<T>::GenerateBindings(LPCTSTR Name)
{
  using namespace Lua::Bind;

  return
    Class<Vector3<T>>(Name)
      .Def(Constructor<>())
      .Def(Constructor<T,T,T>())
      .Def(Constructor<CONST Vector3<T>&>())

      .Property("X", &Vector3<T>::GetX, &Vector3<T>::SetX)
      .Property("Y", &Vector3<T>::GetY, &Vector3<T>::SetY)
      .Property("Z", &Vector3<T>::GetZ, &Vector3<T>::SetZ)

      .Def("Dot", &Vector3<T>::Dot)
      .Def("Cross", &Vector3<T>::Cross)

      .Property("Length", &Vector3<T>::Length)

      .Property("SquareLength", &Vector3<T>::SquareLength)
      .Def("Norm", &Vector3<T>::Norm)
      .Def("Normalize", &Vector3<T>::Normalize)

      .Def(Self + Vector3<T>())
      .Def(Self - Vector3<T>())
      .Def(-Self)
      .Def(Self * T())
      .Def(Self / T())

      .Def(tostring(Self));

}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------
