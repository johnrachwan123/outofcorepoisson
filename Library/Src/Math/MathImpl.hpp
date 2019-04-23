/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/MathImpl.hpp $
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





//------------------------------------------------------------------------------------------------------------------------------
template<class Real>
inline Real Sin(Real Rad)
{
  return (Real)sin(Rad);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class Real>
inline Real Cos(Real Rad)
{
  return (Real)cos(Rad);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class Real>
inline Real Tan(Real Rad)
{
  return (Real)tan(Rad);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class Real>
inline Real Abs(Real X)
{
  return (Real)fabs((double)X);
}
//------------------------------------------------------------------------------------------------------------------------------
template<>
inline INT Abs(INT X)
{
  return abs(X);
}
//------------------------------------------------------------------------------------------------------------------------------
template<>
inline LONG Abs(LONG X)
{
#if defined(COMPILER_GCC)
	return abs((int)X);
#else
	return abs(X);
#endif
}
//------------------------------------------------------------------------------------------------------------------------------
template<class Real>
inline INT Sign(Real X)
{
  if (X < Real(0))
    return -1;
  if (X > Real(0))
    return 1;
  return 0;
}
//------------------------------------------------------------------------------------------------------------------------------
template<class Real>
inline Real Sqrt(Real X)
{
  return (Real)sqrt((double)X);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class Real>
inline Real InvSqrt(Real X)
{
  return (Real)1 / (Real)sqrt((double)X);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class Real>
inline Real Sqr(Real X)
{
  return X*X;
}
//------------------------------------------------------------------------------------------------------------------------------
template<class Real>
inline Real ASin(Real X)
{
  return (Real)asin(X);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class Real>
inline Real ACos(Real X)
{
  return (Real)acos(X);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class Real>
inline Real ATan(Real X)
{
  return (Real)atan(X);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class Real>
inline Real ATan2(Real Y, Real X)
{
  return (Real)atan2(Y,X);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class Real>
inline Real Ceil(Real X)
{
  return (Real)ceil(X);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class Real>
inline Real Floor(Real X)
{
  return (Real)floor(X);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class Real>
inline Real Exp(Real X)
{
  return (Real)exp(X);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class Real>
inline Real Log10(Real X)
{
  return (Real)log10(X);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class Real>
inline Real Ln(Real X)
{
  return (Real)log(X);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class Real>
inline Real Log2(Real X)
{
  return (Real)log(X) / (Real)log(2.0);
}
//------------------------------------------------------------------------------------------------------------------------------
template<>
inline INT Log2(INT X)
{
  if (X == 0)
    return 0;
  for (int i=0; i<sizeof(INT)*8; i++)
  {
    if (X == 0)
      return i-1;
    X >>= 1;
  }
  return 0;
}
//------------------------------------------------------------------------------------------------------------------------------
template<>
inline LONG Log2(LONG X)
{
  if (X == 0)
    return 0;
  for (int i=0; i<sizeof(LONG)*8; i++)
  {
    if (X == 0)
      return i-1;
    X >>= 1;
  }
  return 0;
}
//------------------------------------------------------------------------------------------------------------------------------
template<>
inline LONGLONG Log2(LONGLONG X)
{
  if (X == 0)
    return 0;
  for (int i=0; i<sizeof(LONGLONG)*8; i++)
  {
    if (X == 0)
      return i-1;
    X >>= 1;
  }
  return 0;
}
//------------------------------------------------------------------------------------------------------------------------------
template<>
inline UINT Log2(UINT X)
{
  if (X == 0)
    return 0;
  for (int i=0; i<sizeof(UINT)*8; i++)
  {
    if (X == 0)
      return i-1;
    X >>= 1;
  }
  return 0;
}
//------------------------------------------------------------------------------------------------------------------------------
template<>
inline ULONG Log2(ULONG X)
{
  if (X == 0)
    return 0;
  for (int i=0; i<sizeof(ULONG)*8; i++)
  {
    if (X == 0)
      return i-1;
    X >>= 1;
  }
  return 0;
}
//------------------------------------------------------------------------------------------------------------------------------
template<>
inline ULONGLONG Log2(ULONGLONG X)
{
  if (X == 0)
    return 0;
  for (int i=0; i<sizeof(ULONGLONG)*8; i++)
  {
    if (X == 0)
      return i-1;
    X >>= 1;
  }
  return 0;
}
//------------------------------------------------------------------------------------------------------------------------------
template<class Real>
inline BOOL IsPower2(Real X)
{
  INT Exp;
  DOUBLE Mantissa = frexp((DOUBLE)X, &Exp);
  return (Mantissa == 0.5);
}
//------------------------------------------------------------------------------------------------------------------------------
template<>
inline UINT NextPowerOf2(UINT X)
{
  return 1 << (Log2(X) + 1);
}
//------------------------------------------------------------------------------------------------------------------------------
template<class Real>
inline Real NextPowerOf2(Real X)
{
  return X;
}
//------------------------------------------------------------------------------------------------------------------------------
template<class Real>
inline Real Mod(Real A, Real B)
{
  return (Real)fmod(A,B);
}
//------------------------------------------------------------------------------------------------------------------------------
template<>
inline INT Mod(INT A, INT B)
{
  return A % B;
}
//------------------------------------------------------------------------------------------------------------------------------
template<>
inline LONG Mod(LONG A, LONG B)
{
  return A % B;
}
//------------------------------------------------------------------------------------------------------------------------------
template<>
inline LONGLONG Mod(LONGLONG A, LONGLONG B)
{
  return A % B;
}
//------------------------------------------------------------------------------------------------------------------------------
template<class Real>
inline Real Pow(Real X, Real Y)
{
  return (Real)pow(X,Y);
}
//------------------------------------------------------------------------------------------------------------------------------
template<>
inline INT Pow(INT X, INT Y)
{
  INT V = 1;
  for (INT i=0; i<Y; i++)
    V *= X;
  return V;
}
//------------------------------------------------------------------------------------------------------------------------------
template<>
inline LONG Pow(LONG X, LONG Y)
{
  LONG V = 1;
  for (LONG i=0; i<Y; i++)
    V *= X;
  return V;
}
//------------------------------------------------------------------------------------------------------------------------------
template<>
inline LONGLONG Pow(LONGLONG X, LONGLONG Y)
{
  LONGLONG V = 1;
  for (LONGLONG i=0; i<Y; i++)
    V *= X;
  return V;
}
//------------------------------------------------------------------------------------------------------------------------------
template<>
inline ULONGLONG Pow(ULONGLONG X, ULONGLONG Y)
{
  ULONGLONG V = 1;
  for (ULONGLONG i=0; i<Y; i++)
    V *= X;
  return V;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real FastSin0(Real fAngle)
{
  Real fASqr = fAngle*fAngle;
  Real fResult = (Real)7.61e-03;
  fResult *= fASqr;
  fResult -= (Real)1.6605e-01;
  fResult *= fASqr;
  fResult += (Real)1.0;
  fResult *= fAngle;
  return fResult;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real FastSin1(Real fAngle)
{
  Real fASqr = fAngle*fAngle;
  Real fResult = -(Real)2.39e-08;
  fResult *= fASqr;
  fResult += (Real)2.7526e-06;
  fResult *= fASqr;
  fResult -= (Real)1.98409e-04;
  fResult *= fASqr;
  fResult += (Real)8.3333315e-03;
  fResult *= fASqr;
  fResult -= (Real)1.666666664e-01;
  fResult *= fASqr;
  fResult += (Real)1.0;
  fResult *= fAngle;
  return fResult;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real FastCos0(Real fAngle)
{
  Real fASqr = fAngle*fAngle;
  Real fResult = (Real)3.705e-02;
  fResult *= fASqr;
  fResult -= (Real)4.967e-01;
  fResult *= fASqr;
  fResult += (Real)1.0;
  return fResult;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real FastCos1(Real fAngle)
{
  Real fASqr = fAngle*fAngle;
  Real fResult = -(Real)2.605e-07;
  fResult *= fASqr;
  fResult += (Real)2.47609e-05;
  fResult *= fASqr;
  fResult -= (Real)1.3888397e-03;
  fResult *= fASqr;
  fResult += (Real)4.16666418e-02;
  fResult *= fASqr;
  fResult -= (Real)4.999999963e-01;
  fResult *= fASqr;
  fResult += (Real)1.0;
  return fResult;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real FastTan0(Real fAngle)
{
  Real fASqr = fAngle*fAngle;
  Real fResult = (Real)2.033e-01;
  fResult *= fASqr;
  fResult += (Real)3.1755e-01;
  fResult *= fASqr;
  fResult += (Real)1.0;
  fResult *= fAngle;
  return fResult;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real FastTan1(Real fAngle)
{
  Real fASqr = fAngle*fAngle;
  Real fResult = (Real)9.5168091e-03;
  fResult *= fASqr;
  fResult += (Real)2.900525e-03;
  fResult *= fASqr;
  fResult += (Real)2.45650893e-02;
  fResult *= fASqr;
  fResult += (Real)5.33740603e-02;
  fResult *= fASqr;
  fResult += (Real)1.333923995e-01;
  fResult *= fASqr;
  fResult += (Real)3.333314036e-01;
  fResult *= fASqr;
  fResult += (Real)1.0;
  fResult *= fAngle;
  return fResult;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real FastInvSin0(Real fValue)
{
  Real fRoot = Sqrt(((Real)1.0)-fValue);
  Real fResult = -(Real)0.0187293;
  fResult *= fValue;
  fResult += (Real)0.0742610;
  fResult *= fValue;
  fResult -= (Real)0.2121144;
  fResult *= fValue;
  fResult += (Real)1.5707288;
	fResult = HALFPI - fRoot*fResult;
  return fResult;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real FastInvSin1(Real fValue)
{
  Real fRoot = Sqrt(Abs(((Real)1.0)-fValue));
  Real fResult = -(Real)0.0012624911;
  fResult *= fValue;
  fResult += (Real)0.0066700901;
  fResult *= fValue;
  fResult -= (Real)0.0170881256;
  fResult *= fValue;
  fResult += (Real)0.0308918810;
  fResult *= fValue;
  fResult -= (Real)0.0501743046;
  fResult *= fValue;
  fResult += (Real)0.0889789874;
  fResult *= fValue;
  fResult -= (Real)0.2145988016;
  fResult *= fValue;
  fResult += (Real)1.5707963050;
  fResult = HALFPI - fRoot*fResult;
  return fResult;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real FastInvCos0(Real fValue)
{
  Real fRoot = Sqrt(((Real)1.0)-fValue);
  Real fResult = -(Real)0.0187293;
  fResult *= fValue;
  fResult += (Real)0.0742610;
  fResult *= fValue;
  fResult -= (Real)0.2121144;
  fResult *= fValue;
  fResult += (Real)1.5707288;
  fResult *= fRoot;
  return fResult;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real FastInvCos1(Real fValue)
{
  Real fRoot = Sqrt(Abs(((Real)1.0)-fValue));
  Real fResult = -(Real)0.0012624911;
  fResult *= fValue;
  fResult += (Real)0.0066700901;
  fResult *= fValue;
  fResult -= (Real)0.0170881256;
  fResult *= fValue;
  fResult += (Real)0.0308918810;
  fResult *= fValue;
  fResult -= (Real)0.0501743046;
  fResult *= fValue;
  fResult += (Real)0.0889789874;
  fResult *= fValue;
  fResult -= (Real)0.2145988016;
  fResult *= fValue;
  fResult += (Real)1.5707963050;
  fResult *= fRoot;
  return fResult;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real FastInvTan0(Real fValue)
{
  Real fVSqr = fValue*fValue;
  Real fResult = (Real)0.0208351;
  fResult *= fVSqr;
  fResult -= (Real)0.085133;
  fResult *= fVSqr;
  fResult += (Real)0.180141;
  fResult *= fVSqr;
  fResult -= (Real)0.3302995;
  fResult *= fVSqr;
  fResult += (Real)0.999866;
  fResult *= fValue;
  return fResult;
}
//------------------------------------------------------------------------------------------------------------------------------
template <class Real>
Real FastInvTan1(Real fValue)
{
  Real fVSqr = fValue*fValue;
  Real fResult = (Real)0.0028662257;
  fResult *= fVSqr;
  fResult -= (Real)0.0161657367;
  fResult *= fVSqr;
  fResult += (Real)0.0429096138;
  fResult *= fVSqr;
  fResult -= (Real)0.0752896400;
  fResult *= fVSqr;
  fResult += (Real)0.1065626393;
  fResult *= fVSqr;
  fResult -= (Real)0.1420889944;
  fResult *= fVSqr;
  fResult += (Real)0.1999355085;
  fResult *= fVSqr;
  fResult -= (Real)0.3333314528;
  fResult *= fVSqr;
  fResult += (Real)1.0;
  fResult *= fValue;
  return fResult;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T UniformRandom()
{
  return (T)rand() / (T)RAND_MAX;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T Random(T MaxValue)
{
  return (T)rand() * MaxValue / (T)RAND_MAX;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T Random(T MinValue, T MaxValue)
{
  return Random(MaxValue-MinValue) + MinValue;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T KahanSum(T* pData, SIZE_T N)
{
	T Sum = pData[0];
	T c = T(0);
	for (SIZE_T i=1; i<N; i++)
	{
		T y = pData[i] - c;
		T t = Sum + y;
		c = (t - Sum) - y;
		Sum = t;
	}
	return Sum;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID KahanSumStep(T& BigSum, T& SmallSum, T x)
{
  T y = x - SmallSum;
  T t = BigSum + y;
  SmallSum = (t - BigSum) - y;
  BigSum = t;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Shuffle(T* X, SIZE_T N, SIZE_T Iterations)
{
  for (SIZE_T i=0; i<Iterations; i++)
  {
    SIZE_T j = Random<SIZE_T>(N-1);
    Swap(X[i], X[j]);
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T RoundUp(T Value, T Granularity)
{
  if ((Value % Granularity) == 0)
    return Value;
  else
    return Value + Granularity - (Value % Granularity);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T RoundDown(T Value, T Granularity)
{
  return (Value / Granularity) * Granularity;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Array<T> SolveLinear(T a1, T a0, T Epsilon)
{
  Array<T> Solutions;
  
  if(Abs(a1) > Epsilon)
    Solutions.Add(-a0/a1);
  
  return Solutions;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Array<T> SolveQuadratic(T A, T B, T C, T Epsilon)
{
  if(Abs(A) <= Epsilon)
    return SolveLinear(B, C, Epsilon);
  
  T D = B*B - 4*A*C;
  
  Array<T> Solutions;
  if (D < Epsilon)
    return Solutions;
  
  D = Sqrt(D);
  
  Solutions.Add((-B - D) / (2*A));
  Solutions.Add((-B + D) / (2*A));
  
  return Solutions;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Array<T> SolveCubic(T a3, T a2, T a1, T a0, T Epsilon)
{
  return Array<T>();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------

/*

// Solution taken from: http://mathworld.wolfram.com/CubicFormula.html
// and http://www.csit.fsu.edu/~burkardt/f_src/subpak/subpak.f90
INT Factor(DOUBLE a3,DOUBLE a2,DOUBLE a1,DOUBLE a0,DOUBLE roots[3][2],DOUBLE EPS){
	DOUBLE q,r,r2,q3;

	if(fabs(a3)<=EPS){return Factor(a2,a1,a0,roots,EPS);}
	a2/=a3;
	a1/=a3;
	a0/=a3;

	q=-(3*a1-a2*a2)/9;
	r=-(9*a2*a1-27*a0-2*a2*a2*a2)/54;
	r2=r*r;
	q3=q*q*q;

	if(r2<q3){
		DOUBLE sqrQ=sqrt(q);
		DOUBLE theta = acos (r / (sqrQ*q));
		DOUBLE cTheta=cos(theta/3)*sqrQ;
		DOUBLE sTheta=sin(theta/3)*sqrQ*SQRT_3/2;
		roots[0][1]=roots[1][1]=roots[2][1]=0;
		roots[0][0]=-2*cTheta;
		roots[1][0]=-2*(-cTheta*0.5-sTheta);
		roots[2][0]=-2*(-cTheta*0.5+sTheta);
	}
	else{
		DOUBLE s1,s2,sqr=sqrt(r2-q3);
		DOUBLE t;
		t=-r+sqr;
		if(t<0){s1=-pow(-t,1.0/3);}
		else{s1=pow(t,1.0/3);}
		t=-r-sqr;
		if(t<0){s2=-pow(-t,1.0/3);}
		else{s2=pow(t,1.0/3);}
		roots[0][1]=0;
		roots[0][0]=s1+s2;
		s1/=2;
		s2/=2;
		roots[1][0]= roots[2][0]=-s1-s2;
		roots[1][1]= SQRT_3*(s1-s2);
		roots[2][1]=-roots[1][1];
	}
	roots[0][0]-=a2/3;
	roots[1][0]-=a2/3;
	roots[2][0]-=a2/3;
	return 3;
}

*/
