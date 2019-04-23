/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/VectorNImpl.hpp $
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




//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T>::VectorN()
{
  SetZero();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T>::VectorN(CONST T* pV)
{
  CopyMemory(m_V, pV, sizeof(T)*N);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T>::VectorN(CONST VectorN<N,T>& V)
{
  CopyMemory(m_V, V.m_V, sizeof(T)*N);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T>::~VectorN()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
CONST T& VectorN<N,T>::operator() (SIZE_T i) CONST
{
  Assert(i < N);
  return m_V[i];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
T& VectorN<N,T>::operator() (SIZE_T i)
{
  Assert(i < N);
  return m_V[i];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T>::operator CONST T*() CONST
{
  return m_V;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T>::operator T*()
{
  return m_V;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VOID VectorN<N,T>::SetZero()
{
  ZeroMemory(m_V, sizeof(T)*N);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
SIZE_T VectorN<N,T>::Dimensions() CONST 
{ 
  return N; 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T> operator * (T Scalar, CONST VectorN<N,T>& V)
{
  return V * Scalar;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T> operator / (T Scalar, CONST VectorN<N,T>& V)
{
  return V / Scalar;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T> VectorN<N,T>::operator * (CONST T& A) CONST
{
  VectorN<N,T> V(*this);
  V *= A;
  return V;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T> VectorN<N,T>::operator / (CONST T& A) CONST
{
  VectorN<N,T> V(*this);
  V /= A;
  return V;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T> VectorN<N,T>::operator - (CONST VectorN<N,T>& U) CONST
{
  VectorN<N,T> V(*this);
  V -= U;
  return V;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T> VectorN<N,T>::operator + (CONST VectorN<N,T>& U) CONST
{
  VectorN<N,T> V(*this);
  V += U;
  return V;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T>& VectorN<N,T>::operator *= (CONST T& A)
{
  for (SIZE_T i=0; i<N; i++)
    m_V[i] *= A;

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T>& VectorN<N,T>::operator /= (CONST T& A)
{
  for (SIZE_T i=0; i<N; i++)
    m_V[i] /= A;

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T>& VectorN<N,T>::operator += (CONST VectorN<N,T>& V)
{
  for (SIZE_T i=0; i<N; i++)
    m_V[i] += V.m_V[i];

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T>& VectorN<N,T>::operator -= (CONST VectorN<N,T>& V)
{
  for (SIZE_T i=0; i<N; i++)
    m_V[i] -= V.m_V[i];

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T> VectorN<N,T>::operator- () CONST
{
  VectorN<N,T> V(*this);

  for (SIZE_T i=0; i<N; i++)
    V.m_V[i] = -m_V[i];
  
  return V;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T>& VectorN<N,T>::operator = (CONST VectorN<N,T>& V)
{
  for (SIZE_T i=0; i<N; i++)
    m_V[i] = V.m_V[i];
  
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
T VectorN<N,T>::Dot(CONST VectorN<N,T>& V) CONST
{
  DOUBLE Dot = 0.0;
  for (SIZE_T i=0; i<N; i++)
    Dot += m_V[i]*V.m_V[i]; 

  return (T)Dot;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
T VectorN<N,T>::Length() CONST
{
  return Math::Sqrt<T>(Dot(*this));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
T VectorN<N,T>::SquareLength() CONST
{
  return Dot(*this);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
T VectorN<N,T>::Norm(UINT Ln) CONST
{
  DOUBLE x = 0.0;
  for (UINT i=0; i<N; i++)
    x += Math::Pow<DOUBLE>(m_V[i], (FLOAT)Ln);
  
  return (T)Math::Pow<DOUBLE>(x, 1.0f/Ln);	
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VOID VectorN<N,T>::Normalize()
{
  (*this) /= Length();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
BOOL VectorN<N,T>::operator == (CONST VectorN<N,T>& V)
{
  return CompareMemory(&m_V, &V.m_V, sizeof(T)*N) == 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
BOOL VectorN<N,T>::operator != (CONST VectorN<N,T>& V)
{
  return CompareMemory(&m_V, &V.m_V, sizeof(T)*N) != 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
BOOL VectorN<N,T>::operator <= (CONST VectorN<N,T>& V)
{
  return CompareMemory(&m_V, &V.m_V, sizeof(T)*N) <= 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
BOOL VectorN<N,T>::operator >= (CONST VectorN<N,T>& V)
{
  return CompareMemory(&m_V, &V.m_V, sizeof(T)*N) >= 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
BOOL VectorN<N,T>::operator <  (CONST VectorN<N,T>& V)
{
  return CompareMemory(&m_V, &V.m_V, sizeof(T)*N) < 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
BOOL VectorN<N,T>::operator >  (CONST VectorN<N,T>& V)
{
  return CompareMemory(&m_V, &V.m_V, sizeof(T)*N) > 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T> VectorN<N,T>::Random()
{
  VectorN<N,T> R;
  for (SIZE_T i=0; i<N; i++)
    R(i) = UniformRandom<T>();

  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
