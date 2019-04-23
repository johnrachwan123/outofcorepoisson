/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/BoxImpl.hpp $
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
Box2<T>::Box2()
{
  m_Position = Vector2<T>::Zero;
  m_Axes = Matrix2<T>::Zero;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Box2<T>::Box2(CONST Vector2<T>& P, CONST Matrix2<T> A)
{
  m_Position = P;
  m_Axes = A;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Box2<T>::Box2(CONST Vector2<T>& P, CONST Vector2<T> V0, CONST Vector2<T>& V1)
{
  m_Position = P;
  m_Axes.SetColumn(0,V0);
  m_Axes.SetColumn(1,V1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST Vector2<T>& Box2<T>::Position() CONST
{
  return m_Position;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector2<T>& Box2<T>::Position()
{
  return m_Position;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST Matrix2<T>& Box2<T>::Axes() CONST
{
  return m_Axes;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix2<T>& Box2<T>::Axes()
{
  return m_Axes;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T Box2<T>::Volume()
{
  return Math::Abs(m_Axes.Determinant());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Box3<T>::Box3()
{
  m_Position = Vector3<T>::Zero;
  m_Axes = Matrix3<T>::Zero;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Box3<T>::Box3(CONST Vector3<T>& P, CONST Matrix3<T> A)
{
  m_Position = P;
  m_Axes = A;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Box3<T>::Box3(CONST Vector3<T>& P, CONST Vector3<T> V0, CONST Vector3<T> V1, CONST Vector3<T> V2)
{
  m_Position = P;
  m_Axes.SetColumn(0,V0);
  m_Axes.SetColumn(1,V1);
  m_Axes.SetColumn(2,V2);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST Vector3<T>& Box3<T>::Position() CONST
{
  return m_Position;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector3<T>& Box3<T>::Position()
{
  return m_Position;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST Matrix3<T>& Box3<T>::Axes() CONST
{
  return m_Axes;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix3<T>& Box3<T>::Axes()
{
  return m_Axes;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T Box3<T>::Volume()
{
  return Math::Abs(m_Axes.Determinant());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

