/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/MatrixNImpl.hpp $
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




//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
MatrixN<N,T>::MatrixN()
{
  SetIdentity();
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
MatrixN<N,T>::MatrixN(CONST MatrixN<N,T>& M) 
{
  CopyMemory(m_M, M.m_M, sizeof(T)*N*N);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
MatrixN<N,T>::MatrixN(CONST T* pM)
{
  CopyMemory(m_M, pM, sizeof(T)*N*N);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
MatrixN<N,T>::~MatrixN()
{
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
SIZE_T MatrixN<N,T>::Rows() CONST
{
	return N;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
SIZE_T MatrixN<N,T>::Columns() CONST
{
	return N;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
CONST T& MatrixN<N,T>::operator () (SIZE_T i, SIZE_T j) CONST
{
	Assert(i < N);
	Assert(j < N);

	return m_M[i*N + j];
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
T& MatrixN<N,T>::operator () (SIZE_T i, SIZE_T j)
{
	Assert(i < N);
	Assert(j < N);

	return m_M[i*N + j];
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
MatrixN<N,T>::operator CONST T* () CONST
{
  return m_M;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
MatrixN<N,T>::operator T* ()
{
  return m_M;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T> MatrixN<N,T>::GetRow(SIZE_T i) CONST
{
  Assert(i < N);

  VectorN<N,T> V;
  for (SIZE_T j=0; j<N; j++)
    V(j) = (*this)(i,j);  
  
  return V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T> MatrixN<N,T>::GetColumn(SIZE_T j) CONST
{
  Assert(j < N);

  VectorN<N,T> V;
  for (SIZE_T i=0; i<N; i++)
    V(i) = (*this)(i,j);

  return V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T> MatrixN<N,T>::GetDiagonal() CONST
{
  VectorN<N,T> V;
  for (SIZE_T k=0; k<N; k++)
    V(k) = (*this)(k,k);
  
  return V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
T MatrixN<N,T>::GetElement(SIZE_T i, SIZE_T j) CONST
{
  Assert(i < N);
	Assert(j < N);

	return m_M[i*N + j];
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VOID MatrixN<N,T>::SetRow(SIZE_T i, CONST VectorN<N,T>& V)
{
  Assert(i < N);

  for (SIZE_T j=0; j<N; j++)
     (*this)(i,j) = V(j);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VOID MatrixN<N,T>::SetRow(SIZE_T i, T V)
{
  Assert(i < N);

  for (SIZE_T j=0; j<N; j++)
     (*this)(i,j) = V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VOID MatrixN<N,T>::SetColumn(SIZE_T j, CONST VectorN<N,T>& V)
{
  Assert(j < N);

  for (SIZE_T i=0; i<N; i++)
    (*this)(i,j) = V(i);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VOID MatrixN<N,T>::SetColumn(SIZE_T j, T V)
{
  Assert(j < N);

  for (SIZE_T i=0; i<N; i++)
    (*this)(i,j) = V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VOID MatrixN<N,T>::SetDiagonal(CONST VectorN<N,T>& V)
{
  for (SIZE_T i=0; i<N; i++)
    m_M[i*N+i] = V(i);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VOID MatrixN<N,T>::SetDiagonal(T V)
{
  for (SIZE_T i=0; i<N; i++)
    m_M[i*N+i] = V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VOID MatrixN<N,T>::SetElement(SIZE_T i, SIZE_T j, T V)
{
  Assert(i < N);
	Assert(j < N);

	m_M[i*N + j] = V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VOID MatrixN<N,T>::SetZero()
{
  ZeroMemory(m_M, sizeof(T)*N*N);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VOID MatrixN<N,T>::SetIdentity()
{
  SetZero();
  SetDiagonal(1);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
MatrixN<N,T> MatrixN<N,T>::Identity()
{
  MatrixN I;
  I.SetIdentity();
  return I;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
T MatrixN<N,T>::Norm(UINT Ln) CONST
{
  DOUBLE Sum = 0;
  for (SIZE_T i=0; i<N*N; i++)
    Sum += Math::Pow<DOUBLE>(m_M[i], (DOUBLE)Ln);
  
  return (T)Math::Pow<DOUBLE>(Sum, 1.0/Ln);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VOID MatrixN<N,T>::Normalize()
{
  T x = 1.0f / Norm(2);
  for (SIZE_T i=0; i<N*N; i++)
    m_M[i] *= x;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
MatrixN<N,T> MatrixN<N,T>::operator + (CONST MatrixN<N,T>& M) CONST
{
  MatrixN<N,T> R(*this);
  
  for (SIZE_T i=0; i<N*N; i++)
    R.m_M[i] = m_M[i] + M.m_M[i];

  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
MatrixN<N,T> MatrixN<N,T>::operator - (CONST MatrixN<N,T>& M) CONST
{
  MatrixN<N,T> R(*this);

  for (SIZE_T i=0; i<N*N; i++)
    R.m_M[i] = m_M[i] - M.m_M[i];

  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
MatrixN<N,T> MatrixN<N,T>::operator * (T V) CONST
{
  MatrixN<N,T> R(*this);

  for (SIZE_T i=0; i<N*N; i++)
    R.m_M[i] = m_M[i] * V;

  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
MatrixN<N,T> MatrixN<N,T>::operator / (T V) CONST
{
  MatrixN<N,T> R(*this);

  for (SIZE_T i=0; i<N*N; i++)
    R.m_M[i] = m_M[i] / V;
  
  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
MatrixN<N,T>& MatrixN<N,T>::operator *= (CONST MatrixN<N,T>& M)
{
  MatrixN<N,T> R(*this);
  (*this) = R * M;
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
MatrixN<N,T> MatrixN<N,T>::operator * (CONST MatrixN<N,T>& M) CONST
{
  MatrixN<N,T> R;
  R.SetZero();
  
  for(SIZE_T i=0; i<N; i++)
    for(SIZE_T j=0; j<N; j++)
      for(SIZE_T k=0; k<N; k++)
        R.m_M[i*N+j] += m_M[i*N+k] * M.m_M[k*N+j];
  
  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VectorN<N,T> MatrixN<N,T>::operator * (CONST VectorN<N,T>& V) CONST
{
  VectorN<N,T> R;

  for (SIZE_T i=0; i<N; i++)
    for (SIZE_T j=0; j<N; j++)
      R(i) += m_M[i*N+j] * V(j);

  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
MatrixN<N,T>& MatrixN<N,T>::operator += (CONST MatrixN<N,T>& M)
{
  for (SIZE_T i=0; i<N*N; i++)
    m_M[i] += M.m_M[i];

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
MatrixN<N,T>& MatrixN<N,T>::operator -= (CONST MatrixN<N,T>& M)
{
  for (SIZE_T i=0; i<N*N; i++)
    m_M[i] -= M.m_M[i];

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
MatrixN<N,T>& MatrixN<N,T>::operator *= (T V)
{
  for (SIZE_T i=0; i<N*N; i++)
    m_M[i] *= V;

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
MatrixN<N,T>& MatrixN<N,T>::operator /= (T V)
{
  for (SIZE_T i=0; i<N*N; i++)
    m_M[i] /= V;

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
MatrixN<N,T> operator * (T Scalar, CONST MatrixN<N,T>& M)
{
  return M * Scalar;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
MatrixN<N,T> operator / (T Scalar, CONST MatrixN<N,T>& M)
{
  return M / Scalar;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
MatrixN<N,T> operator * (CONST VectorN<N,T>& V, CONST MatrixN<N,T>& M)
{
  return M * V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
MatrixN<N,T> MatrixN<N,T>::Transpose() CONST
{
  MatrixN<N,T> R;
  
  for (SIZE_T i=0; i<N; i++)
    for (SIZE_T j=0; j<N; j++)
      R.m_M[i*N+j] = m_M[j*N+i];

  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VOID MatrixN<N,T>::EigenDecomposition (VectorN<N,T> EigenVectors[N], VectorN<N,T>& EigenValues) CONST
{
	for(SIZE_T i=0; i<N; i++)
	{
    EigenVectors[i] = VectorN<N,T>::Random();

    for(SIZE_T j=0; j<i; j++)
			EigenVectors[i] -= EigenVectors[j] * EigenVectors[i].Dot(EigenVectors[j]);

    EigenVectors[i].Normalize();

		for(SIZE_T j=0; j<50; j++)
		{
			EigenVectors[i] = (*this) * EigenVectors[i];

			for(SIZE_T k=0; k<i; k++)
				EigenVectors[i] -= EigenVectors[k] * EigenVectors[i].Dot(EigenVectors[k]);
			
      EigenVectors[i].Normalize();
		}
		EigenValues(i) = EigenVectors[i].Dot((*this)*EigenVectors[i]);
	}
}

//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VOID MatrixN<N,T>::EigenDecomposition (MatrixN<N,T>& R, MatrixN<N,T>& D) CONST
{
	VectorN<N,T> EigenVectors[N];
	VectorN<N,T> EigenValues;

	EigenDecomposition(EigenVectors, EigenValues);
	
	D.SetZero();
	D.SetDiagonal(EigenValues);

  for (SIZE_T i=0; i<N; i++)
	  R.SetColumn(i,EigenVectors[i]);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
IO::BinaryStream& operator << (IO::BinaryStream& Stream, CONST MatrixN<N,T>& Matrix)
{
	Stream.Write(&Matrix(0,0), sizeof(T)*N*N);
	return Stream;
}
//------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
IO::BinaryStream& operator >> (IO::BinaryStream& Stream, MatrixN<N,T>& Matrix)
{
	Stream.Read(&Matrix(0,0), sizeof(T)*N*N);
	return Stream;
}
//------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
MatrixN<N,T> MatrixN<N,T>::Read(LPCTSTR Filename)
{
  MatrixN<N,T> M;
  Ptr<IO::File> F = IO::File::Open(Filename, IO::FileMode::OPEN, IO::FileAccess::READ);
  F->Read(&M(0,0), sizeof(T)*N*N);
  return M;
}
//------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class T>
VOID MatrixN<N,T>::Write(LPCTSTR Filename)
{
  Ptr<IO::File> F = IO::File::Open(Filename, IO::FileMode::OPENORCREATE, IO::FileAccess::WRITE);
  F->Write(m_M, sizeof(T)*N*N);
}
//------------------------------------------------------------------------------------------------------------------------------
