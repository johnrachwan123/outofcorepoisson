/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/Matrix2Impl.hpp $
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
Matrix2<T>::Matrix2()
{
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix2<T>::Matrix2(CONST Matrix2<T>& M) 
{
  CopyMemory<T>(m_M, M.m_M, 4);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix2<T>::Matrix2(CONST T* pM)
{
  CopyMemory<T>(m_M, pM, 4);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix2<T>::Matrix2(T m00, T m01, T m10, T m11)
{
  m_M[0] = m00;
  m_M[1] = m01;
  m_M[2] = m10;
  m_M[3] = m11;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix2<T>::~Matrix2()
{
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
SIZE_T Matrix2<T>::Rows() CONST
{
	return 2;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
SIZE_T Matrix2<T>::Columns() CONST
{
	return 2;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& Matrix2<T>::operator () (SIZE_T i, SIZE_T j) CONST
{
	Assert(i < Rows());
	Assert(j < Columns());

	return m_M[i*Columns() + j];
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
T& Matrix2<T>::operator () (SIZE_T i, SIZE_T j)
{
	Assert(i < Rows());
	Assert(j < Columns());

	return m_M[i*Columns() + j];
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& Matrix2<T>::operator [] (SIZE_T ij) CONST
{
  Assert(ij < Rows()*Columns());
  return m_M[ij];
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
T& Matrix2<T>::operator [] (SIZE_T ij)
{
  Assert(ij < Rows()*Columns());
  return m_M[ij];
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector2<T> Matrix2<T>::GetRow(SIZE_T i) CONST
{
  Assert(i < Rows());

  Vector2<T> V(Columns());
  for (SIZE_T j=0; j<Columns(); j++)
    V(j) = (*this)(i,j);  
  return V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector2<T> Matrix2<T>::GetColumn(SIZE_T j) CONST
{
  Assert(j < Columns());

  Vector2<T> V(Rows());
  for (SIZE_T i=0; i<Rows(); i++)
    V(i) = (*this)(i,j);
  return V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector2<T> Matrix2<T>::GetDiagonal() CONST
{
  Vector2<T> V;
  for (SIZE_T k=0; k<Rows(); k++)
    V(k) = (*this)(k,k);
  return V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
T Matrix2<T>::GetElement(SIZE_T i, SIZE_T j) CONST
{
  Assert(i < Rows());
	Assert(j < Columns());

	return m_M[i*Columns() + j];
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix2<T>::SetRow(SIZE_T i, CONST Vector2<T>& V)
{
  Assert(i < Rows());

  for (SIZE_T j=0; j<Columns(); j++)
     (*this)(i,j) = V(j);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix2<T>::SetRow(SIZE_T i, T& V)
{
  Assert(i < Rows());

  for (SIZE_T j=0; j<Columns(); j++)
     (*this)(i,j) = V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix2<T>::SetColumn(SIZE_T j, CONST Vector2<T>& V)
{
  Assert(j < Columns());

  for (SIZE_T i=0; i<Rows(); i++)
    (*this)(i,j) = V(i);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix2<T>::SetColumn(SIZE_T j, T& V)
{
  Assert(j < Columns());

  for (SIZE_T i=0; i<Rows(); i++)
    (*this)(i,j) = V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix2<T>::SetDiagonal(CONST Vector2<T>& V)
{
  m_M[0] = V.m_V[0];
  m_M[3] = V.m_V[1];
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix2<T>::SetDiagonal(CONST T& V)
{
  m_M[0] = V;
  m_M[3] = V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix2<T>::SetDiagonal(T V0, T V1)
{
  m_M[0] = V0;
  m_M[3] = V1;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix2<T>::SetElement(SIZE_T i, SIZE_T j, T& V)
{
  Assert(i < Rows());
	Assert(j < Columns());

	m_M[i*Columns() + j] = V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix2<T>::SetZero()
{
  m_M[0] = T(0);
  m_M[1] = T(0);
  m_M[2] = T(0);
  m_M[3] = T(0);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix2<T>::SetIdentity()
{
  m_M[0] = T(1);
  m_M[1] = T(0);
  m_M[2] = T(0);
  m_M[3] = T(1);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
T Matrix2<T>::Norm(SIZE_T Ln) CONST
{
	T N = Math::Pow<T>(m_M[0], Ln) + Math::Pow<T>(m_M[1], Ln) + Math::Pow<T>(m_M[2], Ln) + Math::Pow<T>(m_M[3], Ln);
  return Math::Pow<T>(N, 1.0f/Ln);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix2<T>::Normalize()
{
	T N = 1.0f/Norm(2);
  m_M[0] *= N;
  m_M[1] *= N;
  m_M[2] *= N;
  m_M[3] *= N;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix2<T> Matrix2<T>::operator + (CONST Matrix2<T>& M) CONST
{
  Matrix2<T> R(*this);
  
  R.m_M[0] = m_M[0] + M.m_M[0];
  R.m_M[1] = m_M[1] + M.m_M[1];
  R.m_M[2] = m_M[2] + M.m_M[2];
  R.m_M[3] = m_M[3] + M.m_M[3];

  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix2<T> Matrix2<T>::operator - (CONST Matrix2<T>& M) CONST
{
  Matrix2<T> R(*this);

  R.m_M[0] = m_M[0] - M.m_M[0];
  R.m_M[1] = m_M[1] - M.m_M[1];
  R.m_M[2] = m_M[2] - M.m_M[2];
  R.m_M[3] = m_M[3] - M.m_M[3];
  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix2<T> Matrix2<T>::operator * (CONST T& V) CONST
{
  Matrix2<T> R(*this);

  R.m_M[0] = m_M[0] * V;
  R.m_M[1] = m_M[1] * V;
  R.m_M[2] = m_M[2] * V;
  R.m_M[3] = m_M[3] * V;

  return M;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix2<T> Matrix2<T>::operator / (CONST T& V) CONST
{
  Matrix2<T> R(*this);

  R.m_M[0] = m_M[0] / V;
  R.m_M[1] = m_M[1] / V;
  R.m_M[2] = m_M[2] / V;
  R.m_M[3] = m_M[3] / V;

  return M;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix2<T> Matrix2<T>::operator * (CONST Matrix2<T>& M) CONST
{
  Assert(Columns() == M.Rows());
  Matrix2<T> R(Rows(), M.Columns());
  R.SetZero();

  R.m_M[0] = m_M[0]*M.m_M[0] + m_M[1]*M.m_M[2];
  R.m_M[1] = m_M[0]*M.m_M[1] + m_M[1]*M.m_M[3];
  R.m_M[2] = m_M[2]*M.m_M[0] + m_M[3]*M.m_M[2];
  R.m_M[3] = m_M[2]*M.m_M[1] + m_M[3]*M.m_M[3];
  
  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Vector2<T> Matrix2<T>::operator * (CONST Vector2<T>& V) CONST
{
  Assert(V.Dimensions() == Columns());

  Vector2<T> R(V.Dimensions());
  R.SetZero();

  R.m_V[0] = m_M[0]*V.m_V[0] + m_M[1]*V.m_V[1];
  R.m_V[1] = m_M[2]*V.m_V[0] + m_M[3]*V.m_V[1];

  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix2<T>& Matrix2<T>::operator += (CONST Matrix2<T>& M)
{
  m_M[0] += M.m_M[0];
  m_M[1] += M.m_M[1];
  m_M[2] += M.m_M[2];
  m_M[3] += M.m_M[3];

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix2<T>& Matrix2<T>::operator -= (CONST Matrix2<T>& M)
{
  m_M[0] -= M.m_M[0];
  m_M[1] -= M.m_M[1];
  m_M[2] -= M.m_M[2];
  m_M[3] -= M.m_M[3];

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix2<T>& Matrix2<T>::operator *= (CONST T& V)
{
  m_M[0] *= V;
  m_M[1] *= V;
  m_M[2] *= V;
  m_M[3] *= V;

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix2<T>& Matrix2<T>::operator /= (CONST T& V)
{
  m_M[0] /= V;
  m_M[1] /= V;
  m_M[2] /= V;
  m_M[3] /= V;

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix2<T> operator * (T Scalar, CONST Matrix2<T>& M)
{
  return M*Scalar;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix2<T> operator / (T Scalar, CONST Matrix2<T>& M)
{
  return M/Scalar;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix2<T> operator * (CONST Vector2<T>& V, CONST Matrix2<T>& M)
{
  return M*V;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix2<T> Matrix2<T>::Inverse() CONST
{
  Matrix2<T> R;

  T Det = m_M[0]*m_M[3] - m_M[1]*m_M[2];
  if (Math::Abs<T>(Det) > Math::Constants<T>::ZeroTolerance)
  {
    T InvDet = ((T)1.0)/Det;
    R[0]     =  m_M[3]*InvDet;
    R[1]     = -m_M[1]*InvDet;
    R[2]     = -m_M[2]*InvDet;
    R[3]     =  m_M[0]*InvDet;
  }
  else
    R.SetZero();

  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix2<T> Matrix2<T>::Transpose() CONST
{
  Matrix2<T> R;
  
  R.m_M[0] = m_M[0];
  R.m_M[1] = m_M[2];
  R.m_M[2] = m_M[1];
  R.m_M[3] = m_M[3];

  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
T Matrix2<T>::Determinant() CONST
{
  return m_M[0]*m_M[3] - m_M[1]*m_M[2];
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
T Matrix2<T>::Trace() CONST
{
  return m_M[0] + m_M[3];
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix2<T>::EigenDecomposition (Matrix2<T>& R, Matrix2<T>& D) CONST
{
  T Trace = m_M[0] + m_M[3];
  T Diff = m_M[0] - m_M[3];
  T Discr = Math::Sqrt<T>(Diff*Diff + ((Real)4.0)*m_M[1]*m_M[1]);
  T EVal0 = ((Real)0.5)*(Trace-Discr);
  T EVal1 = ((Real)0.5)*(Trace+Discr);
  
  D.SetDiagonal(EVal0, EVal1);

  Real Cos, Sin;
  if (Diff >= (T)0.0)
  {
    Cos = m_M[1];
    Sin = EVal0 - m_M[0];
  }
  else
  {
    Cos = EVal0 - m_M[3];
    Sin = m_M[1];
  }
  T Tmp = Math::InvSqrt<T>(Cos*Cos + Sin*Sin);
  Cos *= Tmp;
  Sin *= Tmp;

  R.m_M[0] = Cos;
  R.m_M[1] = -Sin;
  R.m_M[2] = Sin;
  R.m_M[3] = Cos;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix2<T> Matrix2<T>::Rotation(T Rads)
{
  return Matrix2<T>(Math::Cos<T>(Rads), -Math::Sin<T>(Rads), Math::Sin<T>(Rads), Math::Cos<T>(Rads));
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix2<T> Matrix2<T>::Scale(T Sx, T Sy)
{
  return Matrix2<T>(Sx, 0, 0, Sy);
}
