/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/Quaternion.hpp $
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



#pragma once

#include "Math/Vector3.hpp"
#include "Math/Matrix3.hpp"
#include "Math/Math.hpp"

namespace Bolitho
{
  namespace Math
  {
		template <class T>
		class Quaternion
		{
		public:
			Quaternion();			
      Quaternion(T W, T X, T Y, T Z);
			Quaternion(CONST Quaternion& Q);
			Quaternion(CONST Matrix3<T>& R);
			Quaternion(CONST Vector3<T>& Axis, T Angle);
			Quaternion(CONST Vector3<T> RotationColumns[3]);

			operator CONST T* () CONST;
			operator T* ();
			T operator[] (int i) CONST;
			T& operator[] (int i);
			T W () CONST;
			T& W ();
			T X () CONST;
			T& X ();
			T Y () CONST;
			T& Y ();
			T Z () CONST;
			T& Z ();

			Quaternion& operator= (CONST Quaternion& Q);

			// comparison
			BOOL operator== (CONST Quaternion& Q) CONST;
			BOOL operator!= (CONST Quaternion& Q) CONST;
			BOOL operator<  (CONST Quaternion& Q) CONST;
			BOOL operator<= (CONST Quaternion& Q) CONST;
			BOOL operator>  (CONST Quaternion& Q) CONST;
			BOOL operator>= (CONST Quaternion& Q) CONST;

			Quaternion operator+ (CONST Quaternion& Q) CONST;
			Quaternion operator- (CONST Quaternion& Q) CONST;
			Quaternion operator* (CONST Quaternion& Q) CONST;
			Quaternion operator* (T c) CONST;
			Quaternion operator/ (T c) CONST;
			Quaternion operator- () CONST;

			// arithmetic updates
			Quaternion& operator+= (CONST Quaternion& Q);
			Quaternion& operator-= (CONST Quaternion& Q);
			Quaternion& operator*= (T c);
			Quaternion& operator/= (T c);

			Quaternion& FromRotationMatrix (CONST Matrix3<T>& rkRot);
			VOID ToRotationMatrix (Matrix3<T>& rkRot) CONST;
			Quaternion& FromRotationMatrix (CONST Vector3<T> akRotColumn[3]);
			VOID ToRotationMatrix (Vector3<T> akRotColumn[3]) CONST;
			Quaternion& FromAxisAngle (CONST Vector3<T>& Axis, T fAngle);
			VOID ToAxisAngle (Vector3<T>& Axis, T& rfAngle) CONST;

			// functions of a quaternion
			T Length() CONST;
			T SquaredLength() CONST;
			T Dot(CONST Quaternion& Q) CONST;
			T Normalize();
			Quaternion Inverse() CONST;
			Quaternion Conjugate() CONST;
			Quaternion Exp() CONST;
			Quaternion Log() CONST;

			// rotation of a vector by a quaternion
			Vector3<T> Rotate(CONST Vector3<T>& V) CONST;

			Quaternion& Interpolate(T t, CONST Quaternion& P, CONST Quaternion& Q);

			// intermediate terms for spherical quadratic interpolation
			Quaternion& Intermediate (CONST Quaternion& rkQ0, CONST Quaternion& rkQ1, CONST Quaternion& rkQ2);

			// spherical quadratic interpolation
			Quaternion& Squad (T fT, CONST Quaternion& rkQ0, CONST Quaternion& rkA0, CONST Quaternion& rkA1, CONST Quaternion& rkQ1);

			// Compute a quaternion that rotates unit-length vector V1 to unit-length
			// vector V2.  The rotation is about the axis perpendicular to both V1 and
			// V2, with angle of that between V1 and V2.  If V1 and V2 are parallel,
			// any axis of rotation will do, such as the permutation (z2,x2,y2), where
			// V2 = (x2,y2,z2).
			Quaternion& Align (CONST Vector3<T>& U, CONST Vector3<T>& V);

			// Decompose a quaternion into q = q_twist * q_swing, where q is 'this'
			// quaternion.  If V1 is the input axis and V2 is the rotation of V1 by
			// q, q_swing represents the rotation about the axis perpendicular to
			// V1 and V2 (see Quaternion::Align), and q_twist is a rotation about V1.
			VOID DecomposeTwistTimesSwing (CONST Vector3<T>& U, Quaternion& Twist, Quaternion& Swing);

			// Decompose a quaternion into q = q_swing * q_twist, where q is 'this'
			// quaternion.  If V1 is the input axis and V2 is the rotation of V1 by
			// q, q_swing represents the rotation about the axis perpendicular to
			// V1 and V2 (see Quaternion::Align), and q_twist is a rotation about V1.
			VOID DecomposeSwingTimesTwist (CONST Vector3<T>& V, Quaternion& Swing, Quaternion& Twist);

			// special values
			static CONST Quaternion Identity;  // the identity rotation
			static CONST Quaternion Zero;

		private:
			// support for comparisons
			int CompareArrays (CONST Quaternion& rkQ) CONST;

			// support for FromRotationMatrix
			static int m_Next[3];

			T m_V[4];
		};

		template <class T>
		Quaternion<T> operator* (T c, CONST Quaternion<T>& Q);

    #include "QuaternionImpl.hpp"

		typedef Quaternion<FLOAT> QuaternionF;
		typedef Quaternion<DOUBLE> QuaternionD;
  }
}

