/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/Matrix4Impl.hpp $
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
Matrix4<T>::Matrix4() : MatrixN<4,T>()
{
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix4<T>::Matrix4(CONST MatrixN<4,T>& M) : MatrixN<4,T>(M)
{
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix4<T>::Matrix4(CONST MatrixN<3,T>& M)
{
	m_M[ 0] = M[0];
  m_M[ 1] = M[1];
  m_M[ 2] = M[2];
  m_M[ 3] = 0.0;
  m_M[ 4] = M[3];
  m_M[ 5] = M[4];
  m_M[ 6] = M[5];
  m_M[ 7] = 0.0;
  m_M[ 8] = M[6];
  m_M[ 9] = M[7];
  m_M[10] = M[8];
  m_M[11] = 0.0;
  m_M[12] = 0.0;
  m_M[13] = 0.0;
  m_M[14] = 0.0;
  m_M[15] = 1.0;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix4<T>::Matrix4(CONST T* pM) : MatrixN<4,T>(pM)
{
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix4<T>::Matrix4(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13, T m20, T m21, T m22, T m23, T m30, T m31, T m32, T m33)
{
  m_M[ 0] = m00;
  m_M[ 1] = m01;
  m_M[ 2] = m02;
  m_M[ 3] = m03;
  m_M[ 4] = m10;
  m_M[ 5] = m11;
  m_M[ 6] = m12;
  m_M[ 7] = m13;
  m_M[ 8] = m20;
  m_M[ 9] = m21;
  m_M[10] = m22;
  m_M[11] = m23;
  m_M[12] = m30;
  m_M[13] = m31;
  m_M[14] = m32;
  m_M[15] = m33;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix4<T>::Matrix4(CONST VectorN<3,T>& Translation, CONST Quaternion<T>& Rotation)
{
  Matrix3<T> R;
  Rotation.ToRotationMatrix(R);
  SetRotation(R);
  SetTranslation(Translation);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix4<T>::~Matrix4()
{
}

//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix3<T> Matrix4<T>::GetSubMatrix(SIZE_T i) CONST
{
  SIZE_T x = 2*(i / 2) + i % 2;
  Matrix3<T> M;

  M[0] = m_M[ 0 + x];
  M[1] = m_M[ 1 + x];
  M[2] = m_M[ 2 + x];
  M[3] = m_M[ 4 + x];
  M[4] = m_M[ 5 + x];
  M[5] = m_M[ 6 + x];
  M[6] = m_M[ 8 + x];
  M[7] = m_M[ 9 + x];
  M[8] = m_M[10 + x];
  
  return M;
}

//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VectorN<3,T> Matrix4<T>::GetTranslation() CONST
{
  VectorN<3,T> V;
	V[0] = m_M[ 3];
	V[1] = m_M[ 7];
	V[2] = m_M[11];

	return V;
}

//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix3<T> Matrix4<T>::GetRotation() CONST
{
  return GetSubMatrix(0);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix4<T>::SetSubMatrix(SIZE_T i, CONST Matrix3<T>& M)
{
  SIZE_T x = 2*(i / 2) + i % 2;

  m_M[ 0 + x] = M[0];
  m_M[ 1 + x] = M[1];
  m_M[ 2 + x] = M[2];
  m_M[ 4 + x] = M[3];
  m_M[ 5 + x] = M[4];
  m_M[ 6 + x] = M[5];
  m_M[ 8 + x] = M[6];
  m_M[ 9 + x] = M[7];
  m_M[10 + x] = M[8];
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix4<T>::SetTranslation(CONST VectorN<3,T>& V)
{
  m_M[ 3] = V[0];
  m_M[ 7] = V[1];
  m_M[11] = V[2];
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Matrix4<T>::SetRotation(CONST Matrix3<T>& R)
{
  SetSubMatrix(0,R);
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
VectorN<3,T> Matrix4<T>::operator * (CONST VectorN<3,T>& V) CONST
{
  VectorN<3,T> R;

  R[0] = m_M[ 0]*V[0] + m_M[ 1]*V[1] + m_M[ 2]*V[2] + m_M[ 3];
  R[1] = m_M[ 4]*V[0] + m_M[ 5]*V[1] + m_M[ 6]*V[2] + m_M[ 7];
  R[2] = m_M[ 8]*V[0] + m_M[ 9]*V[1] + m_M[10]*V[2] + m_M[11];

  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
Matrix4<T> Matrix4<T>::Inverse() CONST
{
  T A0 = m_M[ 0]*m_M[ 5] - m_M[ 1]*m_M[ 4];
  T A1 = m_M[ 0]*m_M[ 6] - m_M[ 2]*m_M[ 4];
  T A2 = m_M[ 0]*m_M[ 7] - m_M[ 3]*m_M[ 4];
  T A3 = m_M[ 1]*m_M[ 6] - m_M[ 2]*m_M[ 5];
  T A4 = m_M[ 1]*m_M[ 7] - m_M[ 3]*m_M[ 5];
  T A5 = m_M[ 2]*m_M[ 7] - m_M[ 3]*m_M[ 6];
  T B0 = m_M[ 8]*m_M[13] - m_M[ 9]*m_M[12];
  T B1 = m_M[ 8]*m_M[14] - m_M[10]*m_M[12];
  T B2 = m_M[ 8]*m_M[15] - m_M[11]*m_M[12];
  T B3 = m_M[ 9]*m_M[14] - m_M[10]*m_M[13];
  T B4 = m_M[ 9]*m_M[15] - m_M[11]*m_M[13];
  T B5 = m_M[10]*m_M[15] - m_M[11]*m_M[14];

  DOUBLE Det = A0*B5-A1*B4+A2*B3+A3*B2-A4*B1+A5*B0;
  if (Abs(Det) <= 1E-12)
	{
		Matrix4<T> R;
		R.SetZero();
    return R;
	}

  Matrix4 Inv;
  Inv(0,0) = + m_M[ 5]*B5 - m_M[ 6]*B4 + m_M[ 7]*B3;
  Inv(1,0) = - m_M[ 4]*B5 + m_M[ 6]*B2 - m_M[ 7]*B1;
  Inv(2,0) = + m_M[ 4]*B4 - m_M[ 5]*B2 + m_M[ 7]*B0;
  Inv(3,0) = - m_M[ 4]*B3 + m_M[ 5]*B1 - m_M[ 6]*B0;
  Inv(0,1) = - m_M[ 1]*B5 + m_M[ 2]*B4 - m_M[ 3]*B3;
  Inv(1,1) = + m_M[ 0]*B5 - m_M[ 2]*B2 + m_M[ 3]*B1;
  Inv(2,1) = - m_M[ 0]*B4 + m_M[ 1]*B2 - m_M[ 3]*B0;
  Inv(3,1) = + m_M[ 0]*B3 - m_M[ 1]*B1 + m_M[ 2]*B0;
  Inv(0,2) = + m_M[13]*A5 - m_M[14]*A4 + m_M[15]*A3;
  Inv(1,2) = - m_M[12]*A5 + m_M[14]*A2 - m_M[15]*A1;
  Inv(2,2) = + m_M[12]*A4 - m_M[13]*A2 + m_M[15]*A0;
  Inv(3,2) = - m_M[12]*A3 + m_M[13]*A1 - m_M[14]*A0;
  Inv(0,3) = - m_M[ 9]*A5 + m_M[10]*A4 - m_M[11]*A3;
  Inv(1,3) = + m_M[ 8]*A5 - m_M[10]*A2 + m_M[11]*A1;
  Inv(2,3) = - m_M[ 8]*A4 + m_M[ 9]*A2 - m_M[11]*A0;
  Inv(3,3) = + m_M[ 8]*A3 - m_M[ 9]*A1 + m_M[10]*A0;

  Inv *= ((T)1.0)/(FLOAT)Det;

  return Inv;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
T Matrix4<T>::Determinant() CONST
{
  T A0 = m_M[ 0]*m_M[ 5] - m_M[ 1]*m_M[ 4];
  T A1 = m_M[ 0]*m_M[ 6] - m_M[ 2]*m_M[ 4];
  T A2 = m_M[ 0]*m_M[ 7] - m_M[ 3]*m_M[ 4];
  T A3 = m_M[ 1]*m_M[ 6] - m_M[ 2]*m_M[ 5];
  T A4 = m_M[ 1]*m_M[ 7] - m_M[ 3]*m_M[ 5];
  T A5 = m_M[ 2]*m_M[ 7] - m_M[ 3]*m_M[ 6];
  T B0 = m_M[ 8]*m_M[13] - m_M[ 9]*m_M[12];
  T B1 = m_M[ 8]*m_M[14] - m_M[10]*m_M[12];
  T B2 = m_M[ 8]*m_M[15] - m_M[11]*m_M[12];
  T B3 = m_M[ 9]*m_M[14] - m_M[10]*m_M[13];
  T B4 = m_M[ 9]*m_M[15] - m_M[11]*m_M[13];
  T B5 = m_M[10]*m_M[15] - m_M[11]*m_M[14];

  T Det = A0*B5-A1*B4+A2*B3+A3*B2-A4*B1+A5*B0;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class T>
T Matrix4<T>::Trace() CONST
{
  return m_M[0] + m_M[5] + m_M[10] + m_M[15];
}
//-----------------------------------------------------------------------------------------------------------------------------
template <class T>
Matrix4<T> Matrix4<T>::PerspectiveFovLH(T FovY, T Aspect, T zn, T zf)
{
  T f =  1 / Tan(DegToRad(FovY/2));

	return Matrix4<T>
     (f/Aspect,        0,		0,					       0,
		          0,				f,		0,					       0,
							0,				0,		(zf+zn)/(zn-zf),	 -1,
							0,				0,		(2*zf*zn)/(zn-zf), 0).Transpose();

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
Matrix4<T> Matrix4<T>::PerspectiveFovRH(T FovY, T Aspect, T zn, T zf)
{
	T f = 1/Tan(DegToRad(FovY/2));

	return Matrix4<T>(	f/Aspect,		0,		0,					0,
							0,				f,		0,					0,
							0,				0,		(zf+zn)/(zf-zn),	1,
							0,				0,		(2*zf*zn)/(zf-zn),	0).Transpose();

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
Matrix4<T> Matrix4<T>::LookAtLH(VectorN<3,T> Eye, VectorN<3,T> At, VectorN<3,T> Up)
{
	VectorN<3,T> z = (Eye - At);
	VectorN<3,T> x = Up.Cross(z);
	VectorN<3,T> y = z.Cross(x);
    
	z.Normalize();
	x.Normalize();
	y.Normalize();

	return Matrix4<T>(	x[0],		y[0],		z[0],			0,
											x[1],		y[1],		z[1],			0,
											x[2],		y[2],		z[2],			0,
											-x.Dot(Eye),-y.Dot(Eye),-z.Dot(Eye),	1).Transpose();

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
Matrix4<T> Matrix4<T>::LookAtRH(VectorN<3,T> Eye, VectorN<3,T> At, VectorN<3,T> Up)
{
	VectorN<3,T> z = (At - Eye);
	VectorN<3,T> x = Up.Cross(z);
	VectorN<3,T> y = z.Cross(x);
    
	z.Normalize();
	x.Normalize();
	y.Normalize();

	return Matrix4<T>(	x[0],		y[0],		z[0],			0,
							x[1],		y[1],		z[1],			0,
							x[2],		y[2],		z[2],			0,
							-x.Dot(Eye),-y.Dot(Eye),-z.Dot(Eye),	1).Transpose();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
Matrix4<T> Matrix4<T>::OrthoLH(T w, T h, T zn, T zf)
{
  return Matrix4<T>(2.0f / w, 0, 0, 0, 0, 2.0f / h, 0, 0, 0, 0, 1.0f / (zf-zn), 0, 0, 0, zn / (zn-zf), 1).Transpose();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
Matrix4<T> Matrix4<T>::Scale(T Sx, T Sy, T Sz, T Sw)
{
  return Matrix4<T>(Sx, 0 , 0 , 0 , 
                     0 , Sy, 0 , 0 ,
                     0 , 0 , Sz, 0 ,
                     0 , 0 , 0 , Sw);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
Matrix4<T> Matrix4<T>::Translation(T Tx, T Ty, T Tz, T Tw)
{
  return Matrix4<T>(1 , 0 , 0 , Tx , 
                     0 , 1 , 0 , Ty ,
                     0 , 0 , 1 , Tz ,
                     0 , 0 , 0 , Tw);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
Matrix4<T> Matrix4<T>::Translation(CONST VectorN<3,T>& V)
{
  return Translation(V[0], V[1], V[2], 1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
Matrix4<T> Matrix4<T>::Translation(CONST VectorN<4,T>& V)
{
  return Translation(V[0], V[1], V[2], V[3]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
Matrix4<T> Matrix4<T>::Rotation(T Angle, VectorN<3,T>& Axis)
{
	T l = Axis.Length();
	if (l == T(0))
		return MatrixN<4,T>();
	T l1 = T(1)/l, x = Axis[0]*l1, y = Axis[1]*l1, z = Axis[2]*l1;
	T s = Sin(Angle), c = Cos(Angle);
	T xs = x*s, ys = y*s, zs = z*s, c1 = T(1)-c;
	T xx = c1*x*x, yy = c1*y*y, zz = c1*z*z;
	T xy = c1*x*y, xz = c1*x*z, yz = c1*y*z;
  return Matrix4<T>(xx+c, xy-zs, xz+ys, 0, xy+zs, yy+c, yz-xs, 0, xz-ys, yz+xs, zz+c,  0, 0, 0, 0, 1);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
Matrix4<T> Matrix4<T>::Rotation(T Angle, T Ax, T Ay, T Az)
{
  return Rotation(Angle, Vector3<T>(Ax, Ay, Az));
}

#if defined(USE_LUA)
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
Lua::Bind::Scope Matrix4<T>::GenerateBindings(LPCTSTR Name)
{
  using namespace Lua::Bind;

  return
    Class<Matrix4<T>>(Name)
      .Def(Constructor<>())
      .Def(Constructor<CONST Matrix4<T>&>())

      .Property("Rows", &Matrix4<T>::Rows)
      .Property("Columns", &Matrix4<T>::Columns)

      .Def("GetSubMatrix", &Matrix4<T>::GetSubMatrix)
      .Def("GetTranslation", &Matrix4<T>::GetTranslation)
      .Def("GetRotation", &Matrix4<T>::GetRotation)

      .Def("SetSubMatrix", &Matrix4<T>::SetSubMatrix)
      .Def("SetTranslation", &Matrix4<T>::SetTranslation)
      .Def("SetRotation", &Matrix4<T>::SetRotation)

      .Def("GetRow", &Matrix4<T>::GetRow)
      .Def("GetColumn", &Matrix4<T>::GetColumn)
      .Def("GetDiagonal", &Matrix4<T>::GetDiagonal)
      .Def("GetElement", &Matrix4<T>::GetElement)

      .Def("SetRow", (VOID(Matrix4<T>::*)(SIZE_T,CONST VectorN<4,T>&))&Matrix4<T>::SetRow)
      .Def("SetRow", (VOID(Matrix4<T>::*)(SIZE_T,T))&Matrix4<T>::SetRow)
      
      .Def("SetColumn", (VOID(Matrix4<T>::*)(SIZE_T,CONST VectorN<4,T>&))&Matrix4<T>::SetColumn)
      .Def("SetColumn", (VOID(Matrix4<T>::*)(SIZE_T,T))&Matrix4<T>::SetColumn)
      
      .Def("SetDiagonal", (VOID(Matrix4<T>::*)(CONST VectorN<4,T>&))&Matrix4<T>::SetDiagonal)
      .Def("SetDiagonal", (VOID(Matrix4<T>::*)(T))&Matrix4<T>::SetDiagonal)

      .Def("SetElement", &Matrix4<T>::SetElement)

      .Def("SetZero", &Matrix4<T>::SetZero)
      .Def("SetIdentity", &Matrix4<T>::SetIdentity)

      .Def("Norm", &Matrix4<T>::Norm)
      .Def("Normalize", &Matrix4<T>::Normalize)

      //.Def(Self * VectorN<3,T>())
      //.Def(Self * VectorN<4,T>())
      //.Def(Self * MatrixN<4,T>())
      //.Def(Self + MatrixN<4,T>())
      //.Def(Self - MatrixN<4,T>())
      //.Def(-Self)
      //.Def(Self * T())
      //.Def(Self / T())

      .Def(tostring(Self));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#endif
