/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/VectorImpl.hpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1681 $
 * Last Updated: $Date: 2008-10-29 07:37:06 -0700 (Wed, 29 Oct 2008) $
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





//-----------------------------------------------------------------------------------------------------------------------------
ImplementTemplateRuntimeType(Bolitho::Math, Vector, Object);
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector<T>::Vector()
{
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector<T>::Vector(CONST Vector<T>& V)
{
  m_pV = V.m_pV;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector<T>::Vector(SIZE_T N) :
  m_pV(N)
{
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector<T>::Vector(SIZE_T N, CONST T* pV) :
  m_pV(N)
{
  Buffer<T>::DataCopy(m_pV.GetBuffer(), pV, N);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector<T>::~Vector()
{
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Vector<T>::Write(LPCTSTR Filename)
{
  using namespace Bolitho::IO;

  Ptr<File> pFile = File::Open(Filename, FileMode::CREATE, FileAccess::WRITE);

  UINT Length = (UINT)m_pV.Length();
  pFile->Write(&Length, sizeof(Length));
  pFile->Write(m_pV.GetBuffer(), sizeof(T)*Length);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Vector<T>::Read(LPCTSTR Filename)
{
  using namespace Bolitho::IO;

  Ptr<File> pFile = File::Open(Filename, FileMode::OPEN, FileAccess::READ);

  UINT Length = 0;
  pFile->Read(&Length, sizeof(Length));
  Resize(Length);
  pFile->Read(m_pV.GetBuffer(), sizeof(T)*Length);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Vector<T>::Resize(SIZE_T N)
{
  m_pV.Resize(N);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Vector<T>::Deallocate()
{
  m_pV.Free();
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
INT Vector<T>::Compare(T* pV)
{
  return memcmp(m_pV, pV, m_pV.Length());
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
BOOL Vector<T>::operator == (CONST Vector<T>& V)
{
  if (m_pV.Length() != V.m_pV.Length())
    return false;
  return Compare(V.m_pV) == 0;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
BOOL Vector<T>::operator != (CONST Vector<T>& V)
{
  return !(*this) == V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
BOOL Vector<T>::operator <= (CONST Vector<T>& V)
{
  if (m_pV.Length() > V.m_pV.Length())
    return false;
  return Compare(V.m_pV) <= 0;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
BOOL Vector<T>::operator >= (CONST Vector<T>& V)
{
  if (m_pV.Length() < V.m_pV.Length())
    return false;
  return Compare(V.m_pV) >= 0;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
BOOL Vector<T>::operator <  (CONST Vector<T>& V)
{
  if (m_pV.Length() >= V.m_pV.Length())
    return false;
  return Compare(V.m_pV) < 0;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
BOOL Vector<T>::operator >  (CONST Vector<T>& V)
{
  if (m_pV.Length() <= V.m_pV.Length())
    return false;
  return Compare(V.m_pV) > 0;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector<T>& Vector<T>::operator = (CONST Vector& V)
{
  m_pV = V.m_pV;
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
SIZE_T Vector<T>::Dimensions() CONST
{
	return m_pV.Length();
}

//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Vector<T>::SetZero()
{
  for (SIZE_T i=0; i<m_pV.Length(); i++)
    m_pV[i] = T(0);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& Vector<T>::operator () (SIZE_T i) CONST
{
	return Get(i);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
T& Vector<T>::operator () (SIZE_T i)
{
	return Get(i);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& Vector<T>::operator [] (SIZE_T i) CONST
{
	return Get(i);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
T& Vector<T>::operator [] (SIZE_T i)
{
	return Get(i);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Vector<T>::Set(SIZE_T i, CONST T& Element)
{
	Assert(i < m_pV.Length());
	m_pV[i] = Element;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& Vector<T>::Get(SIZE_T i) CONST
{
	Assert(i < m_pV.Length());
	return m_pV[i];
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
T& Vector<T>::Get(SIZE_T i)
{
	Assert(i < m_pV.Length());
	return m_pV[i];
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector<T> Vector<T>::operator * (CONST T& A) CONST
{
	Vector V(*this);
	for (SIZE_T i=0; i<m_pV.Length(); i++)
		V.m_pV[i] *= A;
	return V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template <class T>
Vector<T> operator* (T fScalar, CONST Vector<T>& V)
{
  return V*fScalar;
}
//-----------------------------------------------------------------------------------------------------------------------------
template <class T>
Vector<T> operator/ (T fScalar, CONST Vector<T>& V)
{
  return V/fScalar;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector<T>& Vector<T>::operator *= (CONST T& A)
{
	for (SIZE_T i=0; i<m_pV.Length(); i++)
		m_pV[i] *= A;
	return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector<T> Vector<T>::operator / (CONST T& A) CONST
{
	Vector V(*this);
	for (SIZE_T i=0; i<m_pV.Length(); i++)
		V.m_pV[i] /= A;
	return V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector<T>& Vector<T>::operator /= (CONST T& A)
{
	for (SIZE_T i=0; i<m_pV.Length(); i++)
		m_pV[i] /= A;
	return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector<T> Vector<T>::operator + (CONST Vector<T>& V0) CONST
{
	Assert(m_pV.Length() == V0.m_pV.Length());

	Vector<T> V(m_pV.Length());
	for (SIZE_T i=0; i<m_pV.Length(); i++)
		V.m_pV[i] = m_pV[i] + V0.m_pV[i];

	return V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector<T>& Vector<T>::operator += (CONST Vector<T>& V)
{
  Assert(m_pV.Length() == V.m_pV.Length());
  
  for (SIZE_T i=0; i<m_pV.Length(); i++)
    m_pV[i] += V.m_pV[i];
  
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector<T> Vector<T>::operator - (CONST Vector<T>& V0) CONST
{
  Assert(m_pV.Length() == V0.m_pV.Length());
  
  Vector<T> V(m_pV.Length());
  for (SIZE_T i=0; i<m_pV.Length(); i++)
    V.m_pV[i] = m_pV[i] - V0.m_pV[i];
  
  return V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector<T>& Vector<T>::operator -= (CONST Vector<T>& V)
{
  Assert(m_pV.Length() == V.m_pV.Length());
  
  for (SIZE_T i=0; i<m_pV.Length(); i++)
    m_pV[i] -= V.m_pV[i];
  
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
T Vector<T>::Norm(SIZE_T Ln) CONST
{
  T N = T();
  for (SIZE_T i = 0; i<m_pV.Length(); i++)
    N += pow(m_pV[i], (float)Ln);
  return pow(N, 1.0f/Ln);	
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Vector<T>::Normalize()
{
  T N = 1.0f/Norm(2);
  for (SIZE_T i = 0; i<m_pV.Length(); i++)
    m_pV[i] *= N;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
T Vector<T>::Length() CONST
{
  T N = T();
  for (SIZE_T i = 0; i<m_pV.Length(); i++)
    N += m_pV[i]*m_pV[i];
  return sqrt(N);	
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
T Vector<T>::Dot(CONST Vector<T>& V) CONST
{
  Assert(V.m_pV.Length() == m_pV.Length());
  
  T V0 = T();
  for (SIZE_T i=0; i<m_pV.Length(); i++)
    V0 += m_pV[i]*V.m_pV[i];
  
  return V0;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
IO::Stream& operator << (IO::Stream& S, CONST Vector<T>& V)
{
  S.Write(&V.Dimensions(), sizeof(SIZE_T));
  S.Write(&V[0], sizeof(T)*V.Dimensions());
  return S;
}

