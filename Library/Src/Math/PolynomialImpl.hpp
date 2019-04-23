/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/PolynomialImpl.hpp $
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




//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PolynomialN<Degree,T>::PolynomialN()
{
	SetZero();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
template<SIZE_T Degree2>
PolynomialN<Degree,T>::PolynomialN(CONST PolynomialN<Degree2,T>& P)
{
	SetZero();

	for(SIZE_T i=0;i<=Degree && i<=Degree2; i++)
		m_Coefficients[i] = P.m_Coefficients[i];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
template<INT Degree2>
PolynomialN<Degree,T>& PolynomialN<Degree,T>::operator = (CONST PolynomialN<Degree2,T> & P)
{
	INT d = Degree < Degree2 ? Degree : Degree2;
  ZeroMemory(m_Coefficients, sizeof(T)*(Degree+1));
	CopyMemory(m_Coefficients, &P[0], sizeof(T)*(d+1));
	return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PolynomialN<Degree-1,T> PolynomialN<Degree,T>::Derivative() CONST
{
	PolynomialN<Degree-1,T> P;
	
	for(SIZE_T i=0; i<Degree; i++)
		P[i] = m_Coefficients[i+1]*(i+1);

	return P;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PolynomialN<Degree+1,T> PolynomialN<Degree,T>::Integral() CONST
{
	PolynomialN<Degree+1,T> P;
	
	P[0] = 0;
	for(INT i=0; i<=Degree; i++)
		P[i+1] = m_Coefficients[i] / (i+1);
	
	return P;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
T PolynomialN<Degree,T>::Evaluate(T x) CONST
{
	T Temp = (T)1;
	T Value = (T)0;

	for(INT i=0; i<=Degree; i++)
	{
		Value += Temp*m_Coefficients[i];
		Temp *= x;
	}

	return Value;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
T PolynomialN<Degree,T>::operator() (T x) CONST
{
	return Evaluate(x);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
T PolynomialN<Degree,T>::Integral(T Min, T Max) CONST
{
	T v = (T)0;
	T t1 = Min;
	T t2 = Max;

	for(INT i=0; i<=Degree; i++)
	{
		v += m_Coefficients[i] * (t2-t1) / (i+1);

		if(t1 != Bolitho::NumberTraits<T>::MinValue && t1 != Bolitho::NumberTraits<T>::MaxValue)
			t1 *= Min;
		if(t2 != Bolitho::NumberTraits<T>::MinValue && t2 != Bolitho::NumberTraits<T>::MaxValue)
			t2 *= Max;
	}
	return v;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
BOOL PolynomialN<Degree,T>::operator == (CONST PolynomialN<Degree,T>& P) CONST
{
	return CompareMemory(m_Coefficients, P.m_Coefficients, sizeof(T)*(Degree+1)) == 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
BOOL PolynomialN<Degree,T>::operator != (CONST PolynomialN<Degree,T>& P) CONST
{
	return CompareMemory(m_Coefficients, P.m_Coefficients, sizeof(T)*(Degree+1)) != 0;
}
//--------------- -----------------------------------------------------------------------------------
template<INT Degree, class T>
BOOL PolynomialN<Degree,T>::IsZero() CONST
{
	for(INT i=0; i<=Degree; i++)
		if(m_Coefficients[i] != 0)
			return FALSE;

	return TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
VOID PolynomialN<Degree,T>::SetZero()
{
	ZeroMemory(m_Coefficients, sizeof(T)*(Degree+1));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PolynomialN<Degree,T>& PolynomialN<Degree,T>::AddScaled(CONST PolynomialN& P, T s)
{
	for(INT i=0; i<=Degree; i++)
		m_Coefficients[i] += P.m_Coefficients[i]*s;
	
	return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PolynomialN<Degree,T>& PolynomialN<Degree,T>::operator += (CONST PolynomialN<Degree,T>& P)
{
	for(INT i=0; i<=Degree; i++)
		m_Coefficients[i] += P.m_Coefficients[i];
	
	return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PolynomialN<Degree,T>& PolynomialN<Degree,T>::operator -= (CONST PolynomialN<Degree,T>& P)
{
	for(INT i=0; i<=Degree; i++)
		m_Coefficients[i] -= P.m_Coefficients[i];
	
	return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PolynomialN<Degree,T> PolynomialN<Degree,T>::operator + (CONST PolynomialN<Degree,T>& P) CONST
{
	PolynomialN Q;
	for(INT i=0; i<=Degree; i++)
		Q.m_Coefficients[i] = m_Coefficients[i] + P.m_Coefficients[i];
	
	return Q;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PolynomialN<Degree,T> PolynomialN<Degree,T>::operator - (CONST PolynomialN<Degree,T>& P) CONST
{
	PolynomialN Q;	
	for(INT i=0; i<=Degree; i++)
		Q.m_Coefficients[i] = m_Coefficients[i] - P.m_Coefficients[i];
	
	return Q;
}
/*
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
VOID PolynomialN<Degree,T>::Scale(CONST PolynomialN& P, T w, Polynomial& Q)
{
	for(INT i=0;i<=Degree;i++)
		Q.m_Coefficients[i] = P.m_Coefficients[i] * w;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
VOID PolynomialN<Degree,T>::AddScaled(CONST PolynomialN& p1, DOUBLE w1, CONST PolynomialN& p2, DOUBLE w2, PolynomialN& q)
{
	for(INT i=0; i<=Degree; i++)
		Q.m_Coefficients[i] = p1.m_Coefficients[i]*w1 + p2.m_Coefficients[i]*w2;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
VOID Polynomial<Degree>::AddScaled(CONST Polynomial& p1,DOUBLE w1,CONST Polynomial& p2,Polynomial& q){
	for(INT i=0;i<=Degree;i++){q.coefficients[i]=p1.coefficients[i]*w1+p2.coefficients[i];}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
VOID PolynomialN<Degree>::AddScaled(CONST Polynomial& p1,CONST Polynomial& p2,DOUBLE w2,Polynomial& q){
	for(INT i=0;i<=Degree;i++){q.coefficients[i]=p1.coefficients[i]+p2.coefficients[i]*w2;}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree>
VOID Polynomial<Degree>::Subtract(CONST Polynomial &p1,CONST Polynomial& p2,Polynomial& q){
	for(INT i=0;i<=Degree;i++){q.coefficients[i]=p1.coefficients[i]-p2.coefficients[i];}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree>
VOID Polynomial<Degree>::Negate(CONST Polynomial& in,Polynomial& out){
	out=in;
	for(INT i=0;i<=Degree;i++){out.coefficients[i]=-out.coefficients[i];}
}
*/
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PolynomialN<Degree,T> PolynomialN<Degree,T>::operator- () CONST
{
	PolynomialN Q;
	for(INT i=0; i<=Degree; i++)
		Q.m_Coefficients[i] = -m_Coefficients[i];
	
	return Q;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
template<SIZE_T Degree2>
PolynomialN<Degree+Degree2,T> PolynomialN<Degree,T>::Multiply(CONST PolynomialN<Degree2,T>& P) CONST
{
	PolynomialN<Degree+Degree2,T> Q;

	for(INT i=0; i<=Degree; i++)
		for(SIZE_T j=0; j<=Degree2; j++)
			Q[i+j] += m_Coefficients[i] * P[j];
	
	return Q;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PolynomialN<Degree,T>& PolynomialN<Degree,T>::operator += (T s)
{
	m_Coefficients[0] += s;
	return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PolynomialN<Degree,T>& PolynomialN<Degree,T>::operator -= (T s)
{
	m_Coefficients[0] -= s;
	return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PolynomialN<Degree,T>& PolynomialN<Degree,T>::operator *= (T s)
{
	for(INT i=0; i<=Degree; i++)
		m_Coefficients[i] *= s;
	
	return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PolynomialN<Degree,T>& PolynomialN<Degree,T>::operator /= (T s)
{
	for(INT i=0; i<=Degree; i++)
		m_Coefficients[i] /= s;
	
	return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PolynomialN<Degree,T> PolynomialN<Degree,T>::operator + (T s) CONST
{
	PolynomialN Q(*this);
	Q.m_Coefficients[0] += s;
	
	return Q;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PolynomialN<Degree,T> PolynomialN<Degree,T>::operator - (T s) CONST
{
	PolynomialN Q(*this);
	Q.m_Coefficients[0] -= s;
	
	return Q;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PolynomialN<Degree,T> PolynomialN<Degree,T>::operator * (T s) CONST
{
	PolynomialN Q;
	for(INT i=0; i<=Degree; i++)
		Q.m_Coefficients[i] = m_Coefficients[i] * s;
	
	return Q;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PolynomialN<Degree,T> PolynomialN<Degree,T>::operator / (T s) CONST
{
	PolynomialN Q;
	for(INT i=0; i<=Degree; i++)
		Q.m_Coefficients[i] = m_Coefficients[i] / s;

	return Q;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PolynomialN<Degree,T> PolynomialN<Degree,T>::Scale(T s) CONST
{
	PolynomialN Q(*this);

	T s2 = (T)1;
	for(INT i=0; i<=Degree; i++)
	{
		Q.m_Coefficients[i] *= s2;
		s2 /= s;
	}

	return Q;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PolynomialN<Degree,T> PolynomialN<Degree,T>::Shift(T t) CONST
{
	PolynomialN<Degree,T> Q;
	
	for(INT i=0; i<=Degree; i++)
	{
		T Temp = (T)1;
		for(INT j=i; j>=0; j--)
		{
			Q.m_Coefficients[j] += m_Coefficients[i] * Temp;
			Temp *= -t*j;
			Temp /= (i-j+1);
		}
	}
	
	return Q;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<>
inline Array<FLOAT> PolynomialN<1,FLOAT>::GetSolutions(FLOAT c, FLOAT Epsilon) CONST 
{
	return SolveLinear(m_Coefficients[1], m_Coefficients[0]-c, Epsilon);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<>
inline Array<DOUBLE> PolynomialN<1,DOUBLE>::GetSolutions(DOUBLE c, DOUBLE Epsilon) CONST 
{
	return SolveLinear(m_Coefficients[1], m_Coefficients[0]-c, Epsilon);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<>
inline Array<FLOAT> PolynomialN<2,FLOAT>::GetSolutions(FLOAT c, FLOAT Epsilon) CONST 
{
	return SolveQuadratic(m_Coefficients[2], m_Coefficients[1], m_Coefficients[0]-c, Epsilon);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<>
inline Array<DOUBLE> PolynomialN<2,DOUBLE>::GetSolutions(DOUBLE c, DOUBLE Epsilon) CONST 
{
	return SolveQuadratic(m_Coefficients[2], m_Coefficients[1], m_Coefficients[0]-c, Epsilon);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
Array<T> PolynomialN<Degree,T>::GetSolutions(T c, T Epsilon) CONST 
{
	StaticAssert(FALSE);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
