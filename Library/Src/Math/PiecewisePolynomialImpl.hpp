/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/PiecewisePolynomialImpl.hpp $
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
template<INT Degree, class T>
template<INT Degree2>
typename PiecewisePolynomialN<Degree+Degree2,T>::SEGMENT PiecewisePolynomialN<Degree,T>::SEGMENT::Multiply(CONST typename PiecewisePolynomialN<Degree2,T>::SEGMENT& P) CONST
{
  typename PiecewisePolynomialN<Degree+Degree2,T>::SEGMENT sp;
  if(Start > P.Start)
    sp.Start = Start;
  else
    sp.Start = P.Start;
  
  sp.Polynomial = Polynomial * P.Polynomial;
  return sp;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
typename PiecewisePolynomialN<Degree,T>::SEGMENT PiecewisePolynomialN<Degree,T>::SEGMENT::Scale(T s) CONST
{
  SEGMENT q;
  q.Start = Start*s;
  q.Polynomial = Polynomial.Scale(s);
  return q;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
typename PiecewisePolynomialN<Degree,T>::SEGMENT PiecewisePolynomialN<Degree,T>::SEGMENT::Shift(T s) CONST
{
  SEGMENT q;
  q.Start = Start+s;
  q.Polynomial = Polynomial.Shift(s);
  return q;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
BOOL PiecewisePolynomialN<Degree,T>::SEGMENT::operator < (CONST typename PiecewisePolynomialN<Degree,T>::SEGMENT& P) CONST
{
  return Start < P.Start;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree,T>::PiecewisePolynomialN()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree,T>::PiecewisePolynomialN(CONST PiecewisePolynomialN<Degree,T>& P)
{
  m_Segments = P.m_Segments;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree,T>::~PiecewisePolynomialN()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
VOID PiecewisePolynomialN<Degree,T>::Optimize()
{
  m_Segments.Sort();

  SIZE_T c = 0;
	for(SIZE_T i=0; i<m_Segments.Length(); i++)
  {
    if(c == 0 || m_Segments[i].Start != m_Segments[c-1].Start)
      m_Segments[c++] = m_Segments[i];
		else
      m_Segments[c-1].Polynomial += m_Segments[i].Polynomial;
  }
	m_Segments.Resize(c);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
VOID PiecewisePolynomialN<Degree,T>::Resize(SIZE_T Count)
{
  m_Segments.Resize(Count);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
SIZE_T PiecewisePolynomialN<Degree,T>::Count() CONST
{
  return m_Segments.Length();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
typename PiecewisePolynomialN<Degree,T>::SEGMENT& PiecewisePolynomialN<Degree,T>::operator[] (SIZE_T i)
{
  return m_Segments[i];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
CONST typename PiecewisePolynomialN<Degree,T>::SEGMENT& PiecewisePolynomialN<Degree,T>::operator[] (SIZE_T i) CONST
{
  return m_Segments[i];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree,T>& PiecewisePolynomialN<Degree,T>::operator = (CONST PiecewisePolynomialN<Degree,T>& P)
{
  m_Segments = P.m_Segments;
	return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
template<INT Degree2>
PiecewisePolynomialN<Degree,T>& PiecewisePolynomialN<Degree,T>::operator  = (CONST PiecewisePolynomialN<Degree2,T>& P)
{
  m_Segments.Resize(P.Count());

	for(SIZE_T i=0; i<m_Segments.Length(); i++)
	{
		m_Segments[i].Start = P[i].Start;
		m_Segments[i].Polynomial = P[i].Polynomial;
	}
	return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
T PiecewisePolynomialN<Degree,T>::operator ()(T t) CONST
{
	T Value = 0;
	for(SIZE_T i=0; i<m_Segments.Length() && t > m_Segments[i].Start; i++)
    Value += m_Segments[i].Polynomial(t);
	return Value;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
T PiecewisePolynomialN<Degree,T>::Integral(T Min, T Max) CONST
{
  INT m = 1;
  T Start = Min;
  T End = Max;
  T s = 0;
  T v = 0;

  if(Min > Max)
    {
      Swap(Start,End);
      m = -1;
    }
  
  for(SIZE_T i=0; i<m_Segments.Length() && m_Segments[i].Start < End; i++)
    {
      if(Start < m_Segments[i].Start)
	s = m_Segments[i].Start;
      else
	s = Start;
      v += m_Segments[i].Polynomial.Integral(s,End);
    }
  return v*m;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree,T> PiecewisePolynomialN<Degree,T>::operator + (CONST PiecewisePolynomialN<Degree,T>& p) CONST
{
  PiecewisePolynomialN q;
  /* 
     INT i,j;
     size_t idx=0;
     q.Resize(polyCount+p.polyCount);
     i=j=-1;
     
     while(idx<q.polyCount)
     {
     if		(j>=INT(p.polyCount)-1)				{q.polys[idx]=  polys[++i];}
     else if	(i>=INT( polyCount)-1)				{q.polys[idx]=p.polys[++j];}
     else if(polys[i+1].start<p.polys[j+1].start){q.polys[idx]=  polys[++i];}
     else										{q.polys[idx]=p.polys[++j];}
     //		if(idx && polys[idx].start==polys[idx-1].start)	{polys[idx-1].p+=polys[idx].p;}
     //		else{idx++;}
     idx++;
     }
  */     
  return q;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree,T> PiecewisePolynomialN<Degree,T>::operator - (CONST PiecewisePolynomialN<Degree,T>& p) CONST
{
  PiecewisePolynomialN q;
  /*
    INT i,j;
    size_t idx=0;
    q.set(polyCount+p.polyCount);
    i=j=-1;
    
    while(idx<q.polyCount)
    {
    if		(j>=INT(p.polyCount)-1)				{q.polys[idx]=  polys[++i];}
    else if	(i>=INT( polyCount)-1)				{q.polys[idx].start=p.polys[++j].start;q.polys[idx].p=p.polys[j].p*(-1.0);}
    else if(polys[i+1].start<p.polys[j+1].start){q.polys[idx]=  polys[++i];}
    else										{q.polys[idx].start=p.polys[++j].start;q.polys[idx].p=p.polys[j].p*(-1.0);}
    //		if(idx && polys[idx].start==polys[idx-1].start)	{polys[idx-1].p+=polys[idx].p;}
    //		else{idx++;}
    idx++;
    }
  */
  return q;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree,T>& PiecewisePolynomialN<Degree,T>::AddScaled(CONST PiecewisePolynomialN<Degree,T>& p, T Scale)
{
  /*
    INT i,j;
    SEGMENT<Degree>* oldPolys=polys;
    size_t idx=0,cnt=0,oldPolyCount=polyCount;
    polyCount=0;
    polys=NULL;
    set(oldPolyCount+p.polyCount);
    i=j=-1;
    while(cnt<polyCount){
    if		(j>=INT(p.polyCount)-1)				{polys[idx]=oldPolys[++i];}
    else if	(i>=INT(oldPolyCount)-1)				{polys[idx].start= p.polys[++j].start;polys[idx].p=p.polys[j].p*scale;}
    else if	(oldPolys[i+1].start<p.polys[j+1].start){polys[idx]=oldPolys[++i];}
    else											{polys[idx].start= p.polys[++j].start;polys[idx].p=p.polys[j].p*scale;}
    if(idx && polys[idx].start==polys[idx-1].start)	{polys[idx-1].p+=polys[idx].p;}
    else{idx++;}
    cnt++;
    }
    free(oldPolys);
    reset(idx);
  */
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
template<INT Degree2>
PiecewisePolynomialN<Degree+Degree2,T> PiecewisePolynomialN<Degree,T>::operator * (CONST PiecewisePolynomialN<Degree2,T>& P) CONST
{
  PiecewisePolynomialN<Degree+Degree2,T> Q;
  Q.Resize(Count()*P.Count());
  
  for(SIZE_T i=0; i<Count(); i++)
    for(SIZE_T j=0; j<P.Count(); j++)
      {
	typename PiecewisePolynomialN<Degree+Degree2,T>::SEGMENT sp;
	
	if(m_Segments[i].Start > P[j].Start)
	  sp.Start = m_Segments[i].Start;
	else
	  sp.Start = P[j].Start;

	PolynomialN<Degree,T> P0 = m_Segments[i].Polynomial;
	PolynomialN<Degree2,T> P1 = P[j].Polynomial;
       
	sp.Polynomial = P0.template Multiply<Degree2>(P1);
	
	Q[i*P.Count()+j] = sp;
      }
  Q.Optimize();
  
  return Q;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
template<INT Degree2>
PiecewisePolynomialN<Degree+Degree2,T> PiecewisePolynomialN<Degree,T>::operator * (CONST PolynomialN<Degree2,T>& P) CONST
{
  PiecewisePolynomialN<Degree+Degree2,T> q;
  /*  
      q.set(polyCount);
      for(INT i=0;i<INT(polyCount);i++){
      q.polys[i].start=polys[i].start;
      q.polys[i].p=polys[i].p*p;
      }
  */
  return q;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree,T> PiecewisePolynomialN<Degree,T>::Scale(T s) CONST
{
  PiecewisePolynomialN Q;
  Q.Resize(Count());
  for(SIZE_T i=0; i<Count(); i++)
    Q[i] = m_Segments[i].Scale(s);
  
  return Q;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree,T> PiecewisePolynomialN<Degree,T>::Shift(T s) CONST
{
  PiecewisePolynomialN Q;
  Q.Resize(Count());
  for(SIZE_T i=0; i<Count(); i++)
    Q[i] = m_Segments[i].Shift(s);
  
  return Q;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree-1,T> PiecewisePolynomialN<Degree,T>::Derivative() CONST
{
  PiecewisePolynomialN<Degree-1,T> Q;
  Q.Resize(Count());
  
  for(SIZE_T i=0; i<Count(); i++)
    {
      Q[i].Start = m_Segments[i].Start;
      Q[i].Polynomial = m_Segments[i].Polynomial.Derivative();
    }
  
  return Q;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree+1,T> PiecewisePolynomialN<Degree,T>::Integral() CONST
{
  INT i;
  PiecewisePolynomialN<Degree+1,T> q;
  /*  
      q.set(polyCount);
      for(i=0;i<INT(polyCount);i++){
      q.polys[i].start=polys[i].start;
      q.polys[i].p=polys[i].p.Integral();
      q.polys[i].p-=q.polys[i].p(q.polys[i].start);
      }
  */
  return q;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree,T>& PiecewisePolynomialN<Degree,T>::operator  += (T s)
{
  /* polys[0].p+=s;  */
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree,T>& PiecewisePolynomialN<Degree,T>::operator  -= (T s)
{
  /* polys[0].p-=s; */
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree,T>& PiecewisePolynomialN<Degree,T>::operator  *= (T s)
{
  for(SIZE_T i=0; i<Count(); i++)
    m_Segments[i].Polynomial *= s;
  
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree,T>& PiecewisePolynomialN<Degree,T>::operator  /= (T s)
{
  for(SIZE_T i=0; i<Count(); i++)
    m_Segments[i].Polynomial /= s;
  
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree,T> PiecewisePolynomialN<Degree,T>::operator + (T s) CONST
{
  PiecewisePolynomialN q=*this;
  q += s;
  return q;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree,T> PiecewisePolynomialN<Degree,T>::operator - (T s) CONST
{
  PiecewisePolynomialN q=*this;
  q -= s;
  return q;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree,T> PiecewisePolynomialN<Degree,T>::operator * (T s) CONST
{
  PiecewisePolynomialN q=*this;
  q *= s;
  return q;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree,T> PiecewisePolynomialN<Degree,T>::operator / (T s) CONST
{
  PiecewisePolynomialN q=*this;
  q /= s;
  return q;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree,T> PiecewisePolynomialN<Degree,T>::ConstantFunction(T Radius)
{
  StaticAssert(Degree >= 0);
  
  PiecewisePolynomialN Q;
  Q.Resize(2);
  
  Q[0].Start = -Radius;
  Q[1].Start = Radius;
  
  Q[0].Polynomial[0] = 1.0;
  Q[1].Polynomial[0] = -1.0;
  
  return Q;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<>
inline PiecewisePolynomialN<0,FLOAT> PiecewisePolynomialN<0,FLOAT>::GaussianApproximation(FLOAT Width)
{
  return ConstantFunction(Width);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<>
inline PiecewisePolynomialN<0,DOUBLE> PiecewisePolynomialN<0,DOUBLE>::GaussianApproximation(DOUBLE Width)
{
  return ConstantFunction(Width);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree,T> PiecewisePolynomialN<Degree,T>::GaussianApproximation(T Width)
{
  return PiecewisePolynomialN<Degree-1,T>::GaussianApproximation().MovingAverage(Width);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree+1,T> PiecewisePolynomialN<Degree,T>::MovingAverage(T Radius)
{
  PiecewisePolynomialN<Degree+1,T> A;
  PolynomialN<Degree+1,T> P;
  A.Resize(Count()*2);
  
  for(SIZE_T i=0; i<Count(); i++)
    {
      A[2*i  ].Start = m_Segments[i].Start - Radius;
      A[2*i+1].Start = m_Segments[i].Start + Radius;
      P = m_Segments[i].Polynomial.Integral() - m_Segments[i].Polynomial.Integral()(m_Segments[i].Start);
      A[2*i  ].Polynomial = P.Shift(-Radius);
      A[2*i+1].Polynomial = P.Shift(Radius)*-1;
    }
  A.Optimize();
  
  return A * 1.0/(2*Radius);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
Array<T> PiecewisePolynomialN<Degree,T>::GetSolutions(T c, T Epsilon, T Min, T Max) CONST
{
  /*
    PolynomialN<Degree> p;
    
    std::vector<DOUBLE> tempRoots;
    
    p.setZero();
    for(size_t i=0;i<polyCount;i++){
    p+=polys[i].p;
    if(polys[i].start>max){break;}
    if(i<polyCount-1 && polys[i+1].start<min){continue;}
    p.getSolutions(c,tempRoots,EPS);
    for(size_t j=0;j<tempRoots.size();j++){
    if(tempRoots[j]>polys[i].start && (i+1==polyCount || tempRoots[j]<=polys[i+1].start)){
    if(tempRoots[j]>min && tempRoots[j]<max){roots.push_back(tempRoots[j]);}
    }
    }
    }
  */
  return Array<T>();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
T PiecewisePolynomialN<Degree,T>::Start() CONST
{
  Assert(!m_Segments.IsEmpty());
  return m_Segments[0].Start;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
T PiecewisePolynomialN<Degree,T>::End() CONST
{
  Assert(!m_Segments.IsEmpty());
  return m_Segments[Count()-1].Start;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<INT Degree, class T>
PiecewisePolynomialN<Degree,T> PiecewisePolynomialN<Degree,T>::Clip(T Start, T End) CONST
{
  BOOL Reset = TRUE;
  PiecewisePolynomialN P;
  PolynomialN<Degree,DOUBLE> Poly;
  INT count=0;
  
  SIZE_T s = 0;
  SIZE_T e = Count();
  SIZE_T i = 0;
  
  for(i=0; i<Count(); i++)
    if(m_Segments[i].Start > Start)
      break;
  s = i-1;
  
  for(i=Count()-1; i<Count(); i--)
    if(m_Segments[i].Start <= End)
      break;
  e = i+1;
  
  if(s > e)
    {
      s = 0;
      Reset = FALSE;
    }
  
  if(e > Count())
    e = Count();
  if(e - s == 0)
    return P;
  
  P.Resize(e-s);
  for(SIZE_T i=0; i<=s; i++)
    P[0].Polynomial = P[0].Polynomial + m_Segments[i].Polynomial;
  
  P[0].Start = (Reset) ? Start : m_Segments[0].Start;
  
  for(SIZE_T i=1; i<(e-s); i++)
    P[i] = m_Segments[i+s];
  
  P.Optimize();
  return P;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
