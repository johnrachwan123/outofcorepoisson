/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/Matrix3Impl.hpp $
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
Matrix3<T>::Matrix3() : MatrixN<3,T>()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix3<T>::Matrix3(CONST Matrix3<T>& M) : MatrixN<3,T>(M)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix3<T>::Matrix3(CONST MatrixN<3,T>& M) : MatrixN<3,T>(M)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
template<class U>
Matrix3<T>::Matrix3(CONST Matrix3<U>& M) : MatrixN<3,T>()
{
  m_M[0] = (T)M[0];
  m_M[1] = (T)M[1];
  m_M[2] = (T)M[2];
  m_M[3] = (T)M[3];
  m_M[4] = (T)M[4];
  m_M[5] = (T)M[5];
  m_M[6] = (T)M[6];
  m_M[7] = (T)M[7];
  m_M[8] = (T)M[8];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
template<class U>
Matrix3<T>::Matrix3(CONST MatrixN<3,U>& M) : MatrixN<3,T>()
{
  m_M[0] = (T)M[0];
  m_M[1] = (T)M[1];
  m_M[2] = (T)M[2];
  m_M[3] = (T)M[3];
  m_M[4] = (T)M[4];
  m_M[5] = (T)M[5];
  m_M[6] = (T)M[6];
  m_M[7] = (T)M[7];
  m_M[8] = (T)M[8];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix3<T>::Matrix3(CONST T* pM) : MatrixN<3,T>(pM)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix3<T>::Matrix3(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22)
{
  m_M[0] = m00;
  m_M[1] = m01;
  m_M[2] = m02;
  m_M[3] = m10;
  m_M[4] = m11;
  m_M[5] = m12;
  m_M[6] = m20;
  m_M[7] = m21;
  m_M[8] = m22;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix3<T>::~Matrix3()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix3<T>::SetDiagonal(T D0, T D1, T D2)
{
  m_M[0] = D0;
  m_M[4] = D1;
  m_M[8] = D2;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix3<T> Matrix3<T>::Inverse() CONST
{
  Matrix3<T> R;

  R[0] = m_M[4]*m_M[8] - m_M[5]*m_M[7];
  R[1] = m_M[2]*m_M[7] - m_M[1]*m_M[8];
  R[2] = m_M[1]*m_M[5] - m_M[2]*m_M[4];
  R[3] = m_M[5]*m_M[6] - m_M[3]*m_M[8];
  R[4] = m_M[0]*m_M[8] - m_M[2]*m_M[6];
  R[5] = m_M[2]*m_M[3] - m_M[0]*m_M[5];
  R[6] = m_M[3]*m_M[7] - m_M[4]*m_M[6];
  R[7] = m_M[1]*m_M[6] - m_M[0]*m_M[7];
  R[8] = m_M[0]*m_M[4] - m_M[1]*m_M[3];

  T Det = m_M[0]*R[0] + m_M[1]*R[3]+m_M[2]*R[6];

  if (Math::Abs<T>(Det) <= 1E-06)
    return Matrix3<T>();

  R /= Det;
  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T Matrix3<T>::Determinant() CONST
{
  T Co00 = m_M[4]*m_M[8] - m_M[5]*m_M[7];
  T Co10 = m_M[5]*m_M[6] - m_M[3]*m_M[8];
  T Co20 = m_M[3]*m_M[7] - m_M[4]*m_M[6];
  T Det = m_M[0]*Co00 + m_M[1]*Co10 + m_M[2]*Co20;
  return Det;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T Matrix3<T>::Trace() CONST
{
  return m_M[0] + m_M[4] + m_M[8];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
BOOL Matrix3<T>::Tridiagonalize(T Diag[3], T Subd[2])
{
	// Householder reduction T = Q^t M Q
	//   Input:   
	//     mat, symmetric 3x3 matrix M
	//   Output:  
	//     mat, orthogonal matrix Q (a reflection)
	//     diag, diagonal entries of T
	//     subd, subdiagonal entries of T (T is symmetric)

	T M00 = m_M[0];
	T M01 = m_M[1];
	T M02 = m_M[2];
	T M11 = m_M[4];
	T M12 = m_M[5];
	T M22 = m_M[8];

	Diag[0] = M00;
	if (Abs(M02) >= 1E-10)
	{
		Subd[0] = Sqrt(M01*M01+M02*M02);
		T InvLength = ((T)1.0)/Subd[0];
		M01 *= InvLength;
		M02 *= InvLength;
		T Tmp = ((T)2.0)*M01*M12+M02*(M22-M11);
		Diag[1] = M11+M02*Tmp;
		Diag[2] = M22-M02*Tmp;
		Subd[1] = M12-M01*Tmp;

		m_M[0] = (T)1.0;
		m_M[1] = (T)0.0;
		m_M[2] = (T)0.0;
		m_M[3] = (T)0.0;
		m_M[4] = M01;
		m_M[5] = M02;
		m_M[6] = (T)0.0;
		m_M[7] = M02;
		m_M[8] = -M01;

		return TRUE;
	}
	else
	{
		Diag[1] = M11;
		Diag[2] = M22;
		Subd[0] = M01;
		Subd[1] = M12;

		m_M[0] = (T)1.0;
		m_M[1] = (T)0.0;
		m_M[2] = (T)0.0;
		m_M[3] = (T)0.0;
		m_M[4] = (T)1.0;
		m_M[5] = (T)0.0;
		m_M[6] = (T)0.0;
		m_M[7] = (T)0.0;
		m_M[8] = (T)1.0;

		return FALSE;
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
BOOL Matrix3<T>::QLAlgorithm(T Diag[3], T Subd[2])
{
	// This is an implementation of the symmetric QR algorithm from the book
	// "Matrix Computations" by Gene H. Golub and Charles F. Van Loan, second
	// edition.  The algorithm is 8.2.3.  The implementation has a slight
	// variation to actually make it a QL algorithm, and it traps the case
	// when either of the subdiagonal terms s0 or s1 is zero and reduces the
	// 2-by-2 subblock directly.

	CONST SIZE_T Max = 32;
	for (SIZE_T i=0; i<Max; i++)
	{
		T Sum, Diff, Discr, EValue0, EValue1, Cos, Sin, Tmp;
		int Row;

		Sum = Abs(Diag[0]) + Abs(Diag[1]);
		if (Abs(Subd[0]) + Sum == Sum)
		{
			// The matrix is effectively
			//       +-        -+
			//   M = | d0  0  0 |
			//       | 0  d1 s1 |
			//       | 0  s1 d2 |
			//       +-        -+

			// Compute the eigenvalues as roots of a quadratic equation.
			Sum = Diag[1] + Diag[2];
			Diff = Diag[1] - Diag[2];
			Discr = Sqrt(Diff*Diff + ((T)4.0)*Subd[1]*Subd[1]);
			EValue0 = ((T)0.5)*(Sum - Discr);
			EValue1 = ((T)0.5)*(Sum + Discr);

			// Compute the Givens rotation.
			if (Diff >= (T)0.0)
			{
				Cos = Subd[1];
				Sin = Diag[1] - EValue0;
			}
			else
			{
				Cos = Diag[2] - EValue0;
				Sin = Subd[1];
			}
			Tmp = InvSqrt(Cos*Cos + Sin*Sin);
			Cos *= Tmp;
			Sin *= Tmp;

			// Postmultiply the current orthogonal matrix with the Givens
			// rotation.
			for (Row=0; Row<3; Row++)
			{
				Tmp = m_M[2+3*Row];
				m_M[2+3*Row] = Sin*m_M[1+3*Row] + Cos*Tmp;
				m_M[1+3*Row] = Cos*m_M[1+3*Row] - Sin*Tmp;
			}

			// Update the tridiagonal matrix.
			Diag[1] = EValue0;
			Diag[2] = EValue1;
			Subd[0] = (T)0.0;
			Subd[1] = (T)0.0;
			return TRUE;
		}

		Sum = Abs(Diag[1]) + Abs(Diag[2]);
		if (Abs(Subd[1]) + Sum == Sum)
		{
			// The matrix is effectively
			//       +-         -+
			//   M = | d0  s0  0 |
			//       | s0  d1  0 |
			//       | 0   0  d2 |
			//       +-         -+

			// Compute the eigenvalues as roots of a quadratic equation.
			Sum = Diag[0] + Diag[1];
			Diff = Diag[0] - Diag[1];
			Discr = Sqrt(Diff*Diff + ((T)4.0)*Subd[0]*Subd[0]);
			EValue0 = ((T)0.5)*(Sum - Discr);
			EValue1 = ((T)0.5)*(Sum + Discr);

			// Compute the Givens rotation.
			if (Diff >= (T)0.0)
			{
				Cos = Subd[0];
				Sin = Diag[0] - EValue0;
			}
			else
			{
				Cos = Diag[1] - EValue0;
				Sin = Subd[0];
			}
			Tmp = InvSqrt(Cos*Cos + Sin*Sin);
			Cos *= Tmp;
			Sin *= Tmp;

			// Postmultiply the current orthogonal matrix with the Givens
			// rotation.
			for (Row=0; Row<3; Row++)
			{
				Tmp = m_M[1+3*Row];
				m_M[1+3*Row] = Sin*m_M[0+3*Row] + Cos*Tmp;
				m_M[0+3*Row] = Cos*m_M[0+3*Row] - Sin*Tmp;
			}

			// Update the tridiagonal matrix.
			Diag[0] = EValue0;
			Diag[1] = EValue1;
			Subd[0] = (T)0.0;
			Subd[1] = (T)0.0;
			
			return TRUE;
		}

		// The matrix is
		//       +-        -+
		//   M = | d0 s0  0 |
		//       | s0 d1 s1 |
		//       | 0  s1 d2 |
		//       +-        -+

		// Set up the parameters for the first pass of the QL step.  The
		// value for A is the difference between diagonal term D[2] and the
		// implicit shift suggested by Wilkinson.
		T Ratio = (Diag[1]-Diag[0])/(((T)2.0f)*Subd[0]);
		T Root = Sqrt((T)1.0 + Ratio*Ratio);
		T B = Subd[1];
		T A = Diag[2] - Diag[0];
		if (Ratio >= (T)0.0)
			A += Subd[0]/(Ratio + Root);
		else
			A += Subd[0]/(Ratio - Root);

		// Compute the Givens rotation for the first pass.
		if (Abs(B) >= Abs(A))
		{
			Ratio = A/B;
			Sin = InvSqrt((T)1.0 + Ratio*Ratio);
			Cos = Ratio*Sin;
		}
		else
		{
			Ratio = B/A;
			Cos = InvSqrt((T)1.0 + Ratio*Ratio);
			Sin = Ratio*Cos;
		}

		// Postmultiply the current orthogonal matrix with the Givens
		// rotation.
		for (Row=0; Row<3; Row++)
		{
			Tmp = m_M[2+3*Row];
			m_M[2+3*Row] = Sin*m_M[1+3*Row]+Cos*Tmp;
			m_M[1+3*Row] = Cos*m_M[1+3*Row]-Sin*Tmp;
		}

		// Set up the parameters for the second pass of the QL step.  The
		// values tmp0 and tmp1 are required to fully update the tridiagonal
		// matrix at the end of the second pass.
		T Tmp0 = (Diag[1] - Diag[2])*Sin + ((T)2.0)*Subd[1]*Cos;
		T Tmp1 = Cos*Subd[0];
		B = Sin*Subd[0];
		A = Cos*Tmp0 - Subd[1];
		Tmp0 *= Sin;

		// Compute the Givens rotation for the second pass.  The subdiagonal
		// term S[1] in the tridiagonal matrix is updated at this time.
		if (Abs(B) >= Abs(A))
		{
			Ratio = A/B;
			Root = Sqrt((T)1.0 + Ratio*Ratio);
			Subd[1] = B*Root;
			Sin = ((T)1.0)/Root;
			Cos = Ratio*Sin;
		}
		else
		{
			Ratio = B/A;
			Root = Sqrt((T)1.0 + Ratio*Ratio);
			Subd[1] = A*Root;
			Cos = ((T)1.0)/Root;
			Sin = Ratio*Cos;
		}

		// Postmultiply the current orthogonal matrix with the Givens
		// rotation.
		for (Row=0; Row<3; Row++)
		{
			Tmp = m_M[1+3*Row];
			m_M[1+3*Row] = Sin*m_M[0+3*Row]+Cos*Tmp;
			m_M[0+3*Row] = Cos*m_M[0+3*Row]-Sin*Tmp;
		}

		// Complete the update of the tridiagonal matrix.
		T Tmp2 = Diag[1] - Tmp0;
		Diag[2] += Tmp0;
		Tmp0 = (Diag[0] - Tmp2)*Sin + ((T)2.0)*Tmp1*Cos;
		Subd[0] = Cos*Tmp0 - Tmp1;
		Tmp0 *= Sin;
		Diag[1] = Tmp2 + Tmp0;
		Diag[0] -= Tmp0;
	}

	return FALSE;

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix3<T>::EigenDecomposition (Matrix3<T>& R, Matrix3<T>& D) CONST
{

  // Factor M = R*D*R^T.  The columns of R are the eigenvectors.  The
  // diagonal entries of D are the corresponding eigenvalues.
  T Diag[3], Subd[2];
  R = *this;
  BOOL Reflection = R.Tridiagonalize(Diag,Subd);
  BOOL Converged = R.QLAlgorithm(Diag,Subd);
  Assert(Converged);
  
  // (insertion) sort eigenvalues in decreasing order, d0 >= d1 >= d2
  int i;
  T fSave;
  
  if (Diag[1] > Diag[0])
    {
      // swap d0 and d1
      fSave = Diag[0];
      Diag[0] = Diag[1];
      Diag[1] = fSave;
      
      // swap V0 and V1
      for (i = 0; i < 3; i++)
	{
	  fSave = R(i,0);
	  R(i,0) = R(i,1);
	  R(i,1) = fSave;
	}
      Reflection = !Reflection;
    }
  
  if (Diag[2] > Diag[1])
    {
      // swap d1 and d2
      fSave = Diag[1];
      Diag[1] = Diag[2];
      Diag[2] = fSave;
      
      // swap V1 and V2
      for (i = 0; i < 3; i++)
	{
	  fSave = R(i,1);
	  R(i,1) = R(i,2);
	  R(i,2) = fSave;
	}
      Reflection = !Reflection;
    }
  
  if (Diag[1] > Diag[0])
    {
      // swap d0 and d1
      fSave = Diag[0];
      Diag[0] = Diag[1];
      Diag[1] = fSave;
      
      // swap V0 and V1
      for (i = 0; i < 3; i++)
	{
	  fSave = R(i,0);
	  R(i,0) = R(i,1);
	  R(i,1) = fSave;
	}
      Reflection = !Reflection;
    }
  
  D.SetZero();
  D.SetDiagonal(Diag[0],Diag[1],Diag[2]);
  
  if (Reflection)
    {
      // The orthogonal transformation that diagonalizes M is a reflection.
      // Make the eigenvectors a right--handed system by changing sign on
      // the last column.
      R(0,2) = -R(0,2);
      R(1,2) = -R(1,2);
      R(2,2) = -R(2,2);
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<>
inline String Matrix3<FLOAT>::ToString() CONST
{
  return String::Format(TEXT("(%f, %f, %f; %f, %f, %f; %f, %f, %f)"), m_M[0], m_M[1], m_M[2], m_M[3], m_M[4], m_M[5], m_M[6], m_M[7], m_M[8]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix3<T> Rotation(T Angle, VectorN<3,T>& Axis)
{
	T l = Axis.Length();
	if (l == T(0))
		return Matrix3<T>();
	T l1 = T(1)/l, x = Axis[0]*l1, y = Axis[1]*l1, z = Axis[2]*l1;
	T s = Sin(Angle), c = Cos(Angle);
	T xs = x*s, ys = y*s, zs = z*s, c1 = T(1)-c;
	T xx = c1*x*x, yy = c1*y*y, zz = c1*z*z;
	T xy = c1*x*y, xz = c1*x*z, yz = c1*y*z;

  return Matrix3<T>(xx+c, xy-zs, xz+ys, xy+zs, yy+c, yz-xs, xz-ys, yz+xs, zz+c);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix3<T> Matrix3<T>::RotationX(T Angle)
{
	T s = Sin(Angle), c = Cos(Angle);
  return Matrix3<T>(1, 0, 0, 0, c, s, 0, -s, c);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix3<T> Matrix3<T>::RotationY(T Angle)
{
	T s = Sin(Angle), c = Cos(Angle);
  return Matrix3<T>(c, 0, -s, 0, 1, 0, s, 0, c);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix3<T> Matrix3<T>::RotationZ(T Angle)
{
	T s = Sin(Angle), c = Cos(Angle);
  return Matrix3<T>(c, s, 0, -s, c, 0, 0, 0, 1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix3<T> Matrix3<T>::Scale(FLOAT x, FLOAT y, FLOAT z)
{
  return Matrix3<T>(x, 0, 0, 0, y, 0, 0, 0, z);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix3<T> Matrix3<T>::Scale(CONST VectorN<3,T>& S)
{
  return Matrix3<T>(S[0], 0, 0, 0, S[1], 0, 0, 0, S[2]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
