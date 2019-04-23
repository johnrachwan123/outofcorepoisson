/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/SphereImpl.hpp $
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

#include "Math/Math.hpp"

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Sphere3<T>::Sphere3()
{
  m_Radius = T(0);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Sphere3<T>::Sphere3(T X, T Y, T Z, T Radius)
{
  m_Center = Vector3<T>(X,Y,Z);
  m_Radius = Radius;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Sphere3<T>::Sphere3(CONST Vector3<T>& Center, T Radius)
{
  m_Center = Center;
  m_Radius = Radius;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Sphere3<T>::Sphere3(CONST Vector3<T>& P0, CONST Vector3<T>& P1, CONST Vector3<T>& P2, CONST Vector3<T>& P3)
{
  Matrix4<T> A;

  A(0,0) = P0[0];
  A(0,1) = P0[1];
  A(0,2) = P0[2];
  A(0,3) = T(1);

  A(1,0) = P1[0];
  A(1,1) = P1[1];
  A(1,2) = P1[2];
  A(1,3) = T(1);

  A(2,0) = P2[0];
  A(2,1) = P2[1];
  A(2,2) = P2[2];
  A(2,3) = T(1);

  A(3,0) = P3[0];
  A(3,1) = P3[1];
  A(3,2) = P3[2];
  A(3,3) = T(1);

  T m11 = A.Determinant();

  A(0,0) = P0[0]*P0[0] + P0[1]*P0[1] + P0[2]*P0[2];
  A(1,0) = P1[0]*P1[0] + P1[1]*P1[1] + P1[2]*P1[2];
  A(2,0) = P2[0]*P2[0] + P2[1]*P2[1] + P2[2]*P2[2];
  A(3,0) = P3[0]*P3[0] + P3[1]*P3[1] + P3[2]*P3[2];

  T m12 = A.Determinant();

  A(0,1) = P0[0];
  A(1,1) = P1[0];
  A(2,1) = P2[0];
  A(3,1) = P3[0];
  
  T m13 = A.Determinant();

  A(0,2) = P0[1];
  A(1,2) = P1[1];
  A(2,2) = P2[1];
  A(3,2) = P3[1];

  T m14 = A.Determinant();

  A(0,3) = P0[2];
  A(1,3) = P1[2];
  A(2,3) = P2[2];
  A(3,3) = P3[2];

  T m15 = A.Determinant();

  if (Math::Abs(m11) == 0.0f)
    throw Math::MathException(this,"Attempted to construct a sphere from 4 co-planar points");

  m_Center[0] =  0.5 * m12 / m11;
  m_Center[1] = -0.5 * m13 / m11;
  m_Center[2] =  0.5 * m14 / m11;

  m_Radius = Math::Sqrt(m_Center[0]*m_Center[0] + m_Center[1]*m_Center[1] + m_Center[2]*m_Center[2] - m15/m11);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& Sphere3<T>::Radius() CONST
{
  return m_Radius;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T& Sphere3<T>::Radius()
{
  return m_Radius;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST Vector3<T>& Sphere3<T>::Center() CONST
{
  return m_Center;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector3<T>& Sphere3<T>::Center()
{
  return m_Center;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T Sphere3<T>::Volume()
{
  return T(4) / T(3) * PI*m_Radius*m_Radius*m_Radius;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T Sphere3<T>::Area()
{
  return T(4)*PI*m_Radius*m_Radius;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
