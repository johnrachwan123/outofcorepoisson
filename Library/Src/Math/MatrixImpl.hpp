/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/MatrixImpl.hpp $
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
template<class T>
Matrix<T>::Matrix()
{
	m_N = 0;
	m_M = 0;
	m_pM = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix<T>::Matrix(CONST Matrix& M) 
{
  m_pM = 0;
  Resize(M.m_N, M.m_M);
	CopyMemory<T>(m_pM, M.m_pM, m_M*m_N);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix<T>::Matrix(SIZE_T N, SIZE_T M)
{
  m_pM = 0;
  Resize(N,M);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix<T>::Matrix(SIZE_T N, SIZE_T M, T* pM)
{
  m_pM = 0;
  Resize(N,M);
  CopyMemory(m_pM, pM, N*M);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix<T>::~Matrix()
{
  Deallocate();
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
SIZE_T Matrix<T>::Rows() CONST
{
	return m_N;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
SIZE_T Matrix<T>::Columns() CONST
{
	return m_M;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix<T>::Resize(SIZE_T N, SIZE_T M)
{
  Deallocate();
  m_N = N;
  m_M = M;

  m_pM = new T[N*M];
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix<T>::Deallocate()
{
	if (m_pM)
	{
		delete [] m_pM;
		m_pM = 0;
	}
	m_N = 0;
	m_M = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& Matrix<T>::operator () (SIZE_T i, SIZE_T j) CONST
{
	Assert(i < Rows());
	Assert(j < Columns());

	return m_pM[i*Columns() + j];
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
T& Matrix<T>::operator () (SIZE_T i, SIZE_T j)
{
	Assert(i < Rows());
	Assert(j < Columns());

	return m_pM[i*Columns() + j];
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& Matrix<T>::operator [] (SIZE_T ij) CONST
{
  Assert(ij < Rows()*Columns());
  return m_pM[ij];
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
T& Matrix<T>::operator [] (SIZE_T ij)
{
  Assert(ij < Rows()*Columns());
  return m_pM[ij];
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector<T> Matrix<T>::GetRow(SIZE_T i) CONST
{
  Assert(i < Rows());

  Vector<T> V(Columns());
  for (SIZE_T j=0; j<Columns(); j++)
    V(j) = (*this)(i,j);
  return V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector<T> Matrix<T>::GetColumn(SIZE_T j) CONST
{
  Assert(j < Columns());

  Vector<T> V(Rows());
  for (SIZE_T i=0; i<Rows(); i++)
    V(i) = (*this)(i,j);
  return V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector<T> Matrix<T>::GetDiagonal(INT n) CONST
{
  INT d = (INT)m_N-(INT)m_M;
  
  SIZE_T i = (n > 0) ? n : 0;
  SIZE_T j = (n < 0) ? Abs(n) : 0;

  Assert(i < Rows());
  Assert(j < Columns());

  SIZE_T l = Min(m_N, m_M) - Abs(n);
  if (n > 0 && d > 0)
    l = ((INT)i <= d) ? Min(m_N, m_M) : Min(m_N, m_M) - Abs(n) + Abs(d);
  else if (n < 0 && d < 0)
    l = ((INT)j <= -d) ? Min(m_N, m_M) : Min(m_N, m_M) - Abs(n) + Abs(d);

  Vector<float> V(l);

  for (SIZE_T k=0; k<l; k++)
    V(k) = (*this)(i++,j++);

  return V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
T Matrix<T>::GetElement(SIZE_T i, SIZE_T j) CONST
{
  Assert(i < Rows());
	Assert(j < Columns());

	return m_pM[i*Columns() + j];
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix<T>::SetRow(SIZE_T i, CONST Vector<T>& V)
{
  Assert(i < Rows());

  for (SIZE_T j=0; j<Columns(); j++)
     (*this)(i,j) = V(j);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix<T>::SetRow(SIZE_T i, T& V)
{
  Assert(i < Rows());

  for (SIZE_T j=0; j<Columns(); j++)
     (*this)(i,j) = V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix<T>::SetColumn(SIZE_T j, CONST Vector<T>& V)
{
  Assert(j < Columns());

  for (SIZE_T i=0; i<Rows(); i++)
    (*this)(i,j) = V(i);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix<T>::SetColumn(SIZE_T j, T& V)
{
  Assert(j < Columns());

  for (SIZE_T i=0; i<Rows(); i++)
    (*this)(i,j) = V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix<T>::SetDiagonal(INT n, CONST Vector<T>& V)
{
  INT d = m_N-m_M;
  
  SIZE_T i = (n > 0) ? n : 0;
  SIZE_T j = (n < 0) ? Abs(n) : 0;

  Assert(i < Rows());
  Assert(j < Columns());

  SIZE_T l = Min(m_N, m_M) - Abs(n);
  if (n > 0 && d > 0)
    l = (i <= d) ? Min(m_N, m_M) : Min(m_N, m_M) - Abs(n) + Abs(d);
  else if (n < 0 && d < 0)
    l = (j <= -d) ? Min(m_N, m_M) : Min(m_N, m_M) - Abs(n) + Abs(d);

  Assert(V.Dimensions() == l);

  for (SIZE_T k=0; k<l; k++)
    (*this)(i++,j++) = V(k);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix<T>::SetDiagonal(INT n, T& V)
{
  INT d = m_N-m_M;
  
  SIZE_T i = (n > 0) ? n : 0;
  SIZE_T j = (n < 0) ? Abs(n) : 0;

  Assert(i < Rows());
  Assert(j < Columns());

  SIZE_T l = Min(m_N, m_M) - Abs(n);
  if (n > 0 && d > 0)
    l = (i <= d) ? Min(m_N, m_M) : Min(m_N, m_M) - Abs(n) + Abs(d);
  else if (n < 0 && d < 0)
    l = (j <= -d) ? Min(m_N, m_M) : Min(m_N, m_M) - Abs(n) + Abs(d);

  for (SIZE_T k=0; k<l; k++)
    (*this)(i++,j++) = V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix<T>::SetElement(SIZE_T i, SIZE_T j, T& V)
{
  Assert(i < Rows());
	Assert(j < Columns());

	m_pM[i*Columns() + j] = V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix<T>::SetZero()
{
  memset(m_pM, 0, m_N*m_M*sizeof(T));
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix<T>::SetIdentity()
{
  SetZero();
  for(SIZE_T ij =0; ij< Min(Rows(), Columns()); ij++)
    (*this)(ij,ij) = T(1.0)
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
T Matrix<T>::Norm(SIZE_T Ln) CONST
{
	T N = T();
	for (SIZE_T i=0; i<Rows(); i++)
		for (SIZE_T j=0; j<Columns(); j++)
       N += pow((*this)(i,j) , (float)Ln);
	return pow(N, 1.0f/Ln);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix<T>::Normalize()
{
	T N = 1.0f/Norm(2);
	for (SIZE_T i=0; i<Rows(); i++)
		for (SIZE_T j=0; j<Columns(); j++)
			(*this)(i,j) *= N;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix<T> Matrix<T>::operator + (CONST Matrix<T>& V) CONST
{
  Matrix<T> R(*this);
  for (SIZE_T i=0; i<Rows(); i++)
		for (SIZE_T j=0; j<Columns(); j++)
			R(i,j) = (*this)(i,j) + M(i,j);
  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix<T> Matrix<T>::operator - (CONST Matrix<T>& V) CONST
{
  Matrix<T> R(*this);
  for (SIZE_T i=0; i<Rows(); i++)
		for (SIZE_T j=0; j<Columns(); j++)
			R(i,j) = (*this)(i,j) - M(i,j);
  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix<T> Matrix<T>::operator * (CONST T& V) CONST
{
  Matrix<T> M(*this);
  for (SIZE_T i=0; i<Rows(); i++)
		for (SIZE_T j=0; j<Columns(); j++)
			M(i,j) *= V;
  return M;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix<T> Matrix<T>::operator / (CONST T& V) CONST
{
  Matrix<T> M(*this);
  for (SIZE_T i=0; i<Rows(); i++)
		for (SIZE_T j=0; j<Columns(); j++)
			M(i,j) /= V;
  return M;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix<T> Matrix<T>::operator * (CONST Matrix<T>& M) CONST
{
  Assert(Columns() == M.Rows());
  Matrix<T> R(Rows(), M.Columns());
  R.SetZero();

  CONST SIZE_T n = Columns();

   for (SIZE_T i=0; i<R.Rows(); i++)
    for (SIZE_T j=0; j<R.Columns(); j++)
      for (SIZE_T k=0; k<n; k++)
        R(i,j) += (*this)(i,k) * M(k,j);
  
  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector<T> Matrix<T>::operator * (CONST Vector<T>& V) CONST
{
  Assert(V.Dimensions() == Columns());

  Vector<T> R(V.Dimensions());
  R.SetZero();

  for (SIZE_T i=0; i<Rows(); i++)
    for (SIZE_T j=0; j<Columns(); j++)
      R(j) += M(i,j)*V(j);
  return R;
}

//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix<T>& Matrix<T>::operator += (T V)
{
	for (SIZE_T i=0; i<Rows(); i++)
		for (SIZE_T j=0; j<Columns(); j++)
      (*this)(i,j) += V;
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix<T>& Matrix<T>::operator -= (T V)
{
	for (SIZE_T i=0; i<Rows(); i++)
		for (SIZE_T j=0; j<Columns(); j++)
      (*this)(i,j) -= V;
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix<T>& Matrix<T>::operator += (CONST Matrix<T>& M)
{
	for (SIZE_T i=0; i<Rows(); i++)
		for (SIZE_T j=0; j<Columns(); j++)
      (*this)(i,j) += M(i,j);
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix<T>& Matrix<T>::operator -= (CONST Matrix<T>& M)
{
	for (SIZE_T i=0; i<Rows(); i++)
		for (SIZE_T j=0; j<Columns(); j++)
      (*this)(i,j) -= M(i,j);
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix<T>& Matrix<T>::operator *= (CONST T& V)
{
	for (SIZE_T i=0; i<Rows(); i++)
		for (SIZE_T j=0; j<Columns(); j++)
      (*this)(i,j) *= V;
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix<T>& Matrix<T>::operator /= (CONST T& V)
{
	for (SIZE_T i=0; i<Rows(); i++)
		for (SIZE_T j=0; j<Columns(); j++)
      (*this)(i,j) /= V;
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix<T> Matrix<T>::Inverse() CONST
{
  Assert (Rows() == Columns());
  Matrix<T> M(Rows(), Columns());
  
  //TODO Implement
  Assert(false);

  return M;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix<T> Matrix<T>::Transpose() CONST
{
  Matrix<T> R(Columns(), Rows());
  
  for (SIZE_T i=0; i<M.Rows(); i++)
    for (SIZE_T j=0; j<M.Columns(); j++)
      R(i,j) = (*this)(j,i);

  return M;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
T Matrix<T>::Determinant() CONST
{
  Assert(false);
  //TODO Implement
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
IO::Stream& operator << (IO::Stream& S, CONST Matrix<T>& M)
{
  S.WriteBytes((CONST BYTE*)&M.Rows(), sizeof(UINT32));
  S.WriteBytes((CONST BYTE*)&M.Columns(), sizeof(UINT32));
  S.WriteBytes((CONST BYTE*)&M[0], sizeof(T)*M.Rows()*M.Columns());

  return S;
}

