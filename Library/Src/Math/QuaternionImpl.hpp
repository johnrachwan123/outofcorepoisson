/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/QuaternionImpl.hpp $
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


// Geometric Tools, Inc.
// http://www.geometrictools.com
// Copyright (c) 1998-2006.  All Rights Reserved
//
// The Wild Magic Library (WM3) source code is supplied under the terms of
// the license agreement
//     http://www.geometrictools.com/License/WildMagic3License.pdf
// and may not be copied or disclosed except in accordance with the terms
// of that agreement.

//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real>::Quaternion ()
{
  // uninitialized for performance in array construction
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real>::Quaternion (Real fW, Real fX, Real fY, Real fZ)
{
  m_V[0] = fW;
  m_V[1] = fX;
  m_V[2] = fY;
  m_V[3] = fZ;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real>::Quaternion (CONST Quaternion& Q)
{
  CopyMemory(m_V,Q.m_V, 4*sizeof(Real));
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real>::Quaternion (CONST Matrix3<Real>& Rot)
{
  FromRotationMatrix(Rot);
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real>::Quaternion (CONST Vector3<Real>& Axis, Real Angle)
{
  FromAxisAngle(Axis,Angle);
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real>::Quaternion (CONST Vector3<Real> RotColumn[3])
{
  FromRotationMatrix(RotColumn);
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real>::operator CONST Real* () CONST
{
	return m_V;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real>::operator Real* ()
{
	return m_V;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Quaternion<Real>::operator[] (int i) CONST
{
	Assert(0 <= i && i <= 3);
	return m_V[i];
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& Quaternion<Real>::operator[] (int i)
{
	Assert(0 <= i && i <= 3);
	return m_V[i];
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Quaternion<Real>::W () CONST
{
	return m_V[0];
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& Quaternion<Real>::W ()
{
	return m_V[0];
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Quaternion<Real>::X () CONST
{
	return m_V[1];
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& Quaternion<Real>::X ()
{
	return m_V[1];
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Quaternion<Real>::Y () CONST
{
	return m_V[2];
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& Quaternion<Real>::Y ()
{
	return m_V[2];
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Quaternion<Real>::Z () CONST
{
	return m_V[3];
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& Quaternion<Real>::Z ()
{
	return m_V[3];
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real>& Quaternion<Real>::operator= (CONST Quaternion& Q)
{
	CopyMemory(m_V,Q.m_V,4*sizeof(Real));
	return *this;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
int Quaternion<Real>::CompareArrays (CONST Quaternion& Q) CONST
{
	return CompareMemory(m_V,Q.m_V,4*sizeof(Real));
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
BOOL Quaternion<Real>::operator== (CONST Quaternion& Q) CONST
{
	return CompareArrays(Q) == 0;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
BOOL Quaternion<Real>::operator!= (CONST Quaternion& Q) CONST
{
	return CompareArrays(Q) != 0;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
BOOL Quaternion<Real>::operator< (CONST Quaternion& Q) CONST
{
	return CompareArrays(Q) < 0;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
BOOL Quaternion<Real>::operator<= (CONST Quaternion& Q) CONST
{
	return CompareArrays(Q) <= 0;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
BOOL Quaternion<Real>::operator> (CONST Quaternion& Q) CONST
{
	return CompareArrays(Q) > 0;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
BOOL Quaternion<Real>::operator>= (CONST Quaternion& Q) CONST
{
	return CompareArrays(Q) >= 0;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real> Quaternion<Real>::operator+ (CONST Quaternion& Q) CONST
{
	Quaternion Sum;
	for (SIZE_T i=0; i<4; i++)
		Sum.m_V[i] = m_V[i] + Q.m_V[i];
	return Sum;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real> Quaternion<Real>::operator- (CONST Quaternion& Q) CONST
{
	Quaternion Diff;
	for (SIZE_T i=0; i<4; i++)
		Diff.m_V[i] = m_V[i] - Q.m_V[i];
	return Diff;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real> Quaternion<Real>::operator* (CONST Quaternion& Q) CONST
{
	// NOTE:  Multiplication is not generally commutative, so in most
	// cases p*q != q*p.

	Quaternion Prod;

	Prod.m_V[0] =
		m_V[0]*Q.m_V[0] -
		m_V[1]*Q.m_V[1] -
		m_V[2]*Q.m_V[2] -
		m_V[3]*Q.m_V[3];

	Prod.m_V[1] =
		m_V[0]*Q.m_V[1] +
		m_V[1]*Q.m_V[0] +
		m_V[2]*Q.m_V[3] -
		m_V[3]*Q.m_V[2];

	Prod.m_V[2] =
		m_V[0]*Q.m_V[2] +
		m_V[2]*Q.m_V[0] +
		m_V[3]*Q.m_V[1] -
		m_V[1]*Q.m_V[3];

	Prod.m_V[3] =
		m_V[0]*Q.m_V[3] +
		m_V[3]*Q.m_V[0] +
		m_V[1]*Q.m_V[2] -
		m_V[2]*Q.m_V[1];

	return Prod;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real> Quaternion<Real>::operator* (Real Scalar) CONST
{
	Quaternion Prod;
	for (SIZE_T i=0; i<4; i++)
		Prod.m_V[i] = Scalar*m_V[i];
	return Prod;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real> Quaternion<Real>::operator/ (Real Scalar) CONST
{
	Quaternion Quot;
	SIZE_T i;

	if (Scalar != (Real)0.0)
	{
		Real InvScalar = ((Real)1.0)/Scalar;
		for (i=0; i<4; i++)
			Quot.m_V[i] = InvScalar*m_V[i];
	}
	else
	{
		for (i=0; i<4; i++)
			Quot.m_V[i] = RealTraits<Real>::MaxValue;
	}

	return Quot;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real> Quaternion<Real>::operator- () CONST
{
	Quaternion Neg;
	for (SIZE_T i=0; i<4; i++)
		Neg.m_V[i] = -m_V[i];
	return Neg;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real> operator* (Real Scalar, CONST Quaternion<Real>& Q)
{
	Quaternion<Real> Prod;
	for (SIZE_T i=0; i<4; i++)
		Prod[i] = Scalar*Q[i];
	return Prod;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real>& Quaternion<Real>::operator+= (CONST Quaternion& Q)
{
	for (SIZE_T i=0; i<4; i++)
		m_V[i] += Q.m_V[i];
	return *this;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real>& Quaternion<Real>::operator-= (CONST Quaternion& Q)
{
	for (SIZE_T i=0; i<4; i++)
		m_V[i] -= Q.m_V[i];
	return *this;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real>& Quaternion<Real>::operator*= (Real Scalar)
{
	for (SIZE_T i=0; i<4; i++)
		m_V[i] *= Scalar;
	return *this;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real>& Quaternion<Real>::operator/= (Real Scalar)
{
	SIZE_T i;

	if (Scalar != (Real)0.0)
	{
		Real InvScalar = ((Real)1.0)/Scalar;
		for (i=0; i<4; i++)
			m_V[i] *= InvScalar;
	}
	else
	{
		for (i = 0; i < 4; i++)
			m_V[i] = RealTraits<Real>::MaxValue;
	}

	return *this;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real>& Quaternion<Real>::FromRotationMatrix (CONST Matrix3<Real>& Rot)
{
	// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
	// article "Quaternion Calculus and Fast Animation".

	Real Trace = Rot(0,0) + Rot(1,1) + Rot(2,2);
	Real Root;

	if (Trace > (Real)0.0)
	{
		// |w| > 1/2, may as well choose w > 1/2
		Root = Sqrt(Trace + (Real)1.0);  // 2w
		m_V[0] = ((Real)0.5)*Root;
		Root = ((Real)0.5)/Root;  // 1/(4w)
		m_V[1] = (Rot(2,1)-Rot(1,2))*Root;
		m_V[2] = (Rot(0,2)-Rot(2,0))*Root;
		m_V[3] = (Rot(1,0)-Rot(0,1))*Root;
	}
	else
	{
		// |w| <= 1/2
		int i = 0;
		if (Rot(1,1) > Rot(0,0))
			i = 1;
		if (Rot(2,2) > Rot(i,i))
			i = 2;

		int j = m_Next[i];
		int k = m_Next[j];

		Root = Sqrt(Rot(i,i)-Rot(j,j)-Rot(k,k)+(Real)1.0);
		Real* Quat[3] = { &m_V[1], &m_V[2], &m_V[3] };
		*Quat[i] = ((Real)0.5)*Root;
		Root = ((Real)0.5)/Root;
		m_V[0] = (Rot(k,j)-Rot(j,k))*Root;
		*Quat[j] = (Rot(j,i)+Rot(i,j))*Root;
		*Quat[k] = (Rot(k,i)+Rot(i,k))*Root;
	}

	return *this;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
VOID Quaternion<Real>::ToRotationMatrix (Matrix3<Real>& rkRot) CONST
{
	Real fTx  = ((Real)2.0)*m_V[1];
	Real fTy  = ((Real)2.0)*m_V[2];
	Real fTz  = ((Real)2.0)*m_V[3];
	Real fTwx = fTx*m_V[0];
	Real fTwy = fTy*m_V[0];
	Real fTwz = fTz*m_V[0];
	Real fTxx = fTx*m_V[1];
	Real fTxy = fTy*m_V[1];
	Real fTxz = fTz*m_V[1];
	Real fTyy = fTy*m_V[2];
	Real fTyz = fTz*m_V[2];
	Real fTzz = fTz*m_V[3];

	rkRot(0,0) = (Real)1.0-(fTyy+fTzz);
	rkRot(0,1) = fTxy-fTwz;
	rkRot(0,2) = fTxz+fTwy;
	rkRot(1,0) = fTxy+fTwz;
	rkRot(1,1) = (Real)1.0-(fTxx+fTzz);
	rkRot(1,2) = fTyz-fTwx;
	rkRot(2,0) = fTxz-fTwy;
	rkRot(2,1) = fTyz+fTwx;
	rkRot(2,2) = (Real)1.0-(fTxx+fTyy);
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real>& Quaternion<Real>::FromRotationMatrix (CONST Vector3<Real> akRotColumn[3])
{
	Matrix3<Real> kRot;
	for (int iCol = 0; iCol < 3; iCol++)
	{
		kRot(0,iCol) = akRotColumn[iCol][0];
		kRot(1,iCol) = akRotColumn[iCol][1];
		kRot(2,iCol) = akRotColumn[iCol][2];
	}
	return FromRotationMatrix(kRot);
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
VOID Quaternion<Real>::ToRotationMatrix (Vector3<Real> akRotColumn[3]) CONST
{
	Matrix3<Real> kRot;
	ToRotationMatrix(kRot);
	for (int iCol = 0; iCol < 3; iCol++)
	{
		akRotColumn[iCol][0] = kRot(0,iCol);
		akRotColumn[iCol][1] = kRot(1,iCol);
		akRotColumn[iCol][2] = kRot(2,iCol);
	}
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real>& Quaternion<Real>::FromAxisAngle (
	CONST Vector3<Real>& rkAxis, Real fAngle)
{
	// assert:  axis[] is unit length
	//
	// The quaternion representing the rotation is
	//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

	Real fHalfAngle = ((Real)0.5)*fAngle;
	Real fSin = Sin(fHalfAngle);
	m_V[0] = Cos(fHalfAngle);
	m_V[1] = fSin*rkAxis[0];
	m_V[2] = fSin*rkAxis[1];
	m_V[3] = fSin*rkAxis[2];

	return *this;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
VOID Quaternion<Real>::ToAxisAngle (Vector3<Real>& rkAxis, Real& rfAngle)
CONST
{
  // The quaternion representing the rotation is
  //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)
  
  Real fSqrLength = m_V[1]*m_V[1] + m_V[2]*m_V[2] + m_V[3]*m_V[3];
  if (fSqrLength > 1E-06)
    {
      rfAngle = ((Real)2.0)*ACos(m_V[0]);
      Real fInvLength = InvSqrt(fSqrLength);
      rkAxis[0] = m_V[1]*fInvLength;
      rkAxis[1] = m_V[2]*fInvLength;
      rkAxis[2] = m_V[3]*fInvLength;
    }
  else
    {
      // angle is 0 (mod 2*pi), so any axis will do
      rfAngle = (Real)0.0;
      rkAxis[0] = (Real)1.0;
      rkAxis[1] = (Real)0.0;
      rkAxis[2] = (Real)0.0;
    }
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Quaternion<Real>::Length () CONST
{
  return Sqrt(
	      m_V[0]*m_V[0] +
	      m_V[1]*m_V[1] +
	      m_V[2]*m_V[2] +
	      m_V[3]*m_V[3]);
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Quaternion<Real>::SquaredLength () CONST
{
  return
    m_V[0]*m_V[0] +
    m_V[1]*m_V[1] +
    m_V[2]*m_V[2] +
    m_V[3]*m_V[3];
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Quaternion<Real>::Dot (CONST Quaternion& rkQ) CONST
{
  Real fDot = (Real)0.0;
  for (int i = 0; i < 4; i++)
      fDot += m_V[i]*rkQ.m_V[i];

  return fDot;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Quaternion<Real>::Normalize ()
{
  Real fLength = Length();
  int i;
  
  if (fLength > 1E-06)
    {
      Real fInvLength = ((Real)1.0)/fLength;
      for (i = 0; i < 4; i++)
	{
	  m_V[i] *= fInvLength;
	}
    }
  else
    {
      fLength = (Real)0.0;
      for (i = 0; i < 4; i++)
	{
	  m_V[i] = (Real)0.0;
	}
    }
  
  return fLength;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real> Quaternion<Real>::Inverse () CONST
{
  Quaternion kInverse;
  
  Real fNorm = (Real)0.0;
  int i;
  for (i = 0; i < 4; i++)
    {
      fNorm += m_V[i]*m_V[i];
    }
  
  if (fNorm > (Real)0.0)
    {
      Real fInvNorm = ((Real)1.0)/fNorm;
      kInverse.m_V[0] = m_V[0]*fInvNorm;
      kInverse.m_V[1] = -m_V[1]*fInvNorm;
      kInverse.m_V[2] = -m_V[2]*fInvNorm;
      kInverse.m_V[3] = -m_V[3]*fInvNorm;
    }
  else
    {
      // return an invalid result to flag the error
      for (i = 0; i < 4; i++)
	{
	  kInverse.m_V[i] = (Real)0.0;
	}
    }
  
  return kInverse;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real> Quaternion<Real>::Conjugate () CONST
{
	return Quaternion(m_V[0],-m_V[1],-m_V[2], -m_V[3]);
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real> Quaternion<Real>::Exp () CONST
{
	// If q = A*(x*i+y*j+z*k) where (x,y,z) is unit length, then
	// exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).  If sin(A) is near zero,
	// use exp(q) = cos(A)+A*(x*i+y*j+z*k) since A/sin(A) has limit 1.

	Quaternion kResult;

	Real fAngle = Sqrt(m_V[1]*m_V[1] +
		m_V[2]*m_V[2] + m_V[3]*m_V[3]);

	Real fSin = Sin(fAngle);
	kResult.m_V[0] = Cos(fAngle);

	int i;

	if (Abs(fSin) >= 1E-06)
	{
		Real fCoeff = fSin/fAngle;
		for (i = 1; i <= 3; i++)
		{
			kResult.m_V[i] = fCoeff*m_V[i];
		}
	}
	else
	{
		for (i = 1; i <= 3; i++)
		{
			kResult.m_V[i] = m_V[i];
		}
	}

	return kResult;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real> Quaternion<Real>::Log () CONST
{
	// If q = cos(A)+sin(A)*(x*i+y*j+z*k) where (x,y,z) is unit length, then
	// log(q) = A*(x*i+y*j+z*k).  If sin(A) is near zero, use log(q) =
	// sin(A)*(x*i+y*j+z*k) since sin(A)/A has limit 1.

	Quaternion kResult;
	kResult.m_V[0] = (Real)0.0;

	int i;

	if (Abs(m_V[0]) < (Real)1.0)
	{
		Real fAngle = ACos(m_V[0]);
		Real fSin = Sin(fAngle);
		if (Abs(fSin) >= 1E-06)
		{
			Real fCoeff = fAngle/fSin;
			for (i = 1; i <= 3; i++)
			{
				kResult.m_V[i] = fCoeff*m_V[i];
			}
			return kResult;
		}
	}

	for (i = 1; i <= 3; i++)
	{
		kResult.m_V[i] = m_V[i];
	}
	return kResult;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector3<Real> Quaternion<Real>::Rotate (CONST Vector3<Real>& rkVector) CONST
{
	// Given a vector u = (x0,y0,z0) and a unit length quaternion
	// q = <w,x,y,z>, the vector v = (x1,y1,z1) which represents the
	// rotation of u by q is v = q*u*q^{-1} where * indicates quaternion
	// multiplication and where u is treated as the quaternion <0,x0,y0,z0>.
	// Note that q^{-1} = <w,-x,-y,-z>, so no real work is required to
	// invert q.  Now
	//
	//   q*u*q^{-1} = q*<0,x0,y0,z0>*q^{-1}
	//     = q*(x0*i+y0*j+z0*k)*q^{-1}
	//     = x0*(q*i*q^{-1})+y0*(q*j*q^{-1})+z0*(q*k*q^{-1})
	//
	// As 3-vectors, q*i*q^{-1}, q*j*q^{-1}, and 2*k*q^{-1} are the columns
	// of the rotation matrix computed in Quaternion<Real>::ToRotationMatrix.
	// The vector v is obtained as the product of that rotation matrix with
	// vector u.  As such, the quaternion representation of a rotation
	// matrix requires less space than the matrix and more time to compute
	// the rotated vector.  Typical space-time tradeoff...

	Matrix3<Real> kRot;
	ToRotationMatrix(kRot);
	return kRot*rkVector;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real>& Quaternion<Real>::Interpolate (Real fT, CONST Quaternion& rkP,
																					 CONST Quaternion& rkQ)
{
	Real fCos = rkP.Dot(rkQ);
	Real fAngle = ACos(fCos);

	if (Abs(fAngle) >= 1E-06)
	{
		Real fSin = Sin(fAngle);
		Real fInvSin = ((Real)1.0)/fSin;
		Real fCoeff0 = Sin(((Real)1.0-fT)*fAngle)*fInvSin;
		Real fCoeff1 = Sin(fT*fAngle)*fInvSin;
		*this = fCoeff0*rkP + fCoeff1*rkQ;
	}
	else
	{
		*this = rkP;
	}

	return *this;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real>& Quaternion<Real>::Intermediate (CONST Quaternion& rkQ0,
																									CONST Quaternion& rkQ1, CONST Quaternion& rkQ2)
{
	// assert:  Q0, Q1, Q2 all unit-length
	Quaternion kQ1Inv = rkQ1.Conjugate();
	Quaternion kP0 = kQ1Inv*rkQ0;
	Quaternion kP2 = kQ1Inv*rkQ2;
	Quaternion kArg = -((Real)0.25)*(kP0.Log()+kP2.Log());
	Quaternion kA = rkQ1*kArg.Exp();
	*this = kA;

	return *this;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real>& Quaternion<Real>::Squad (Real fT, CONST Quaternion& rkQ0,
																					 CONST Quaternion& rkA0, CONST Quaternion& rkA1, CONST Quaternion& rkQ1)
{
	Real fSlerpT = ((Real)2.0)*fT*((Real)1.0-fT);
	Quaternion kSlerpP = Slerp(fT,rkQ0,rkQ1);
	Quaternion kSlerpQ = Slerp(fT,rkA0,rkA1);
	return Slerp(fSlerpT,kSlerpP,kSlerpQ);
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Quaternion<Real>& Quaternion<Real>::Align (CONST Vector3<Real>& rkV1,
																					 CONST Vector3<Real>& rkV2)
{
	// If V1 and V2 are not parallel, the axis of rotation is the unit-length
	// vector U = Cross(V1,V2)/Length(Cross(V1,V2)).  The angle of rotation,
	// A, is the angle between V1 and V2.  The quaternion for the rotation is
	// q = cos(A/2) + sin(A/2)*(ux*i+uy*j+uz*k) where U = (ux,uy,uz).
	//
	// (1) Rather than extract A = acos(Dot(V1,V2)), multiply by 1/2, then
	//     compute sin(A/2) and cos(A/2), we reduce the computational costs by
	//     computing the bisector B = (V1+V2)/Length(V1+V2), so cos(A/2) =
	//     Dot(V1,B).
	//
	// (2) The rotation axis is U = Cross(V1,B)/Length(Cross(V1,B)), but
	//     Length(Cross(V1,B)) = Length(V1)*Length(B)*sin(A/2) = sin(A/2), in
	//     which case sin(A/2)*(ux*i+uy*j+uz*k) = (cx*i+cy*j+cz*k) where
	//     C = Cross(V1,B).
	//
	// If V1 = V2, then B = V1, cos(A/2) = 1, and U = (0,0,0).  If V1 = -V2,
	// then B = 0.  This can happen even if V1 is approximately -V2 using
	// floating point arithmetic, since Vector3::Normalize checks for
	// closeness to zero and returns the zero vector accordingly.  The test
	// for exactly zero is usually not recommend for floating point
	// arithmetic, but the implementation of Vector3::Normalize guarantees
	// the comparison is robust.  In this case, the A = pi and any axis
	// perpendicular to V1 may be used as the rotation axis.

	Vector3<Real> kBisector = rkV1 + rkV2;
	kBisector.Normalize();

	Real fCosHalfAngle = rkV1.Dot(kBisector);
	Vector3<Real> kCross;

	m_V[0] = fCosHalfAngle;

	if (fCosHalfAngle != (Real)0.0)
	{
		kCross = rkV1.Cross(kBisector);
		m_V[1] = kCross[0]();
		m_V[2] = kCross[1]();
		m_V[3] = kCross.Z();
	}
	else
	{
		Real fInvLength;
		if (Abs(rkV1[0]) >= Abs(rkV1[1]))
		{
			// V1[0] or V1.z is the largest magnitude component
			fInvLength = InvSqrt(rkV1[0]*rkV1[0] + rkV1[2]*rkV1[2]);
			m_V[1] = -rkV1[2]*fInvLength;
			m_V[2] = (Real)0.0;
			m_V[3] = +rkV1[0]*fInvLength;
		}
		else
		{
			// V1[1] or V1.z is the largest magnitude component
			fInvLength = InvSqrt(rkV1[1]*rkV1[1] + rkV1[2]*rkV1[2]);
			m_V[1] = (Real)0.0;
			m_V[2] = +rkV1[2]*fInvLength;
			m_V[3] = -rkV1[1]*fInvLength;
		}
	}

	return *this;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
VOID Quaternion<Real>::DecomposeTwistTimesSwing (CONST Vector3<Real>& rkV1, Quaternion& rkTwist, Quaternion& rkSwing)
{
  Vector3<Real> kV2 = Rotate(rkV1);
  rkSwing = Align(rkV1,kV2);
  rkTwist = (*this)*rkSwing.Conjugate();
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
VOID Quaternion<Real>::DecomposeSwingTimesTwist (CONST Vector3<Real>& rkV1, Quaternion& rkSwing, Quaternion& rkTwist)
{
  Vector3<Real> kV2 = Rotate(rkV1);
  rkSwing = Align(rkV1,kV2);
  rkTwist = rkSwing.Conjugate()*(*this);
}
//------------------------------------------------------------------------------------------------------------------------------


