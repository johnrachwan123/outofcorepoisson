/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/PlaneImpl.hpp $
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

//---------------------------------------------------------------------------------------------------
template<class T>
Plane<T>::Plane()
{
}
//---------------------------------------------------------------------------------------------------
template<class T>
Plane<T>::Plane(CONST Plane& P) : m_Normal(P.m_Normal), m_Distance(P.m_Distance)
{
}
//---------------------------------------------------------------------------------------------------
template<class T>
Plane<T>::Plane(CONST Vector3<T>& N, T D) : m_Normal(N), m_Distance(D)
{
}
//---------------------------------------------------------------------------------------------------
template<class T>
Plane<T>::Plane(CONST Vector3<T>& P, CONST Vector3<T>& N) : m_Normal(N)
{
  m_Distance = m_Normal.Dot(P);
}
//---------------------------------------------------------------------------------------------------
template<class T>
Plane<T>::Plane(CONST Vector3<T>& P0, CONST Vector3<T>& P1, CONST Vector3<T>& P2)
{
  Vector3<T> Edge1 = P1 - P0;
  Vector3<T> Edge2 = P2 - P0;
  m_Normal = Edge1.Cross(Edge2);
  m_Normal.Normalize();
  m_Distance = m_Normal.Dot(P0);
}
//---------------------------------------------------------------------------------------------------
template<class T>
Plane<T>::Plane(T A, T B, T C, T D) : m_Normal(A,B,C), m_Distance(D)
{
}
//---------------------------------------------------------------------------------------------------
template<class T>
Plane<T>& Plane<T>::operator = (CONST Plane& P)
{
  m_Normal = P.m_Normal;
  m_Distance = P.m_Distance;
}
//---------------------------------------------------------------------------------------------------
template<class T>
T Plane<T>::Distance(CONST Vector3<T>& P)
{
  return m_Normal.Dot(P) - m_Distance;
}
//---------------------------------------------------------------------------------------------------
template<class T>
INT Plane<T>::Side(CONST Vector3<T>& P)
{
  T D = Distance(P);

  if (D < (T)0.0)
    return -1;
  else if (D > (T)0.0)
    return +1;
  else
    return 0;
}
//---------------------------------------------------------------------------------------------------
template<class T>
BOOL IntersectXPlane(CONST Ray3<T>& R, T d, Vector3<T>& Position)
{
}
//---------------------------------------------------------------------------------------------------
template<class T>
BOOL IntersectYPlane(CONST Ray3<T>& R, T d, Vector3<T>& Position)
{

}
//---------------------------------------------------------------------------------------------------
template<class T>
BOOL IntersectZPlane(CONST Ray3<T>& R, T d, Vector3<T>& Position)
{

}
//---------------------------------------------------------------------------------------------------
