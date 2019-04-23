/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/ComplexImpl.hpp $
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





//----------------------------------------------------------------------------------------------------------------------------
template<class T>
Complex<T>::Complex()
{
  m_Real = m_Imaginary = 0;
}
//----------------------------------------------------------------------------------------------------------------------------
template<class T>
Complex<T>::Complex(T Real, T Imaginary)
{
  m_Real = Real;
  m_Imaginary = Imaginary;
}
//----------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& Complex<T>::Real() CONST
{
  return m_Real;
}
//----------------------------------------------------------------------------------------------------------------------------
template<class T>
T& Complex<T>::Real()
{
  return m_Real;
}
//----------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& Complex<T>::Imaginary() CONST
{
  return m_Imaginary;
}
//----------------------------------------------------------------------------------------------------------------------------
template<class T>
T& Complex<T>::Imaginary()
{
  return m_Imaginary;
}
//----------------------------------------------------------------------------------------------------------------------------
template<class T>
Complex<T> Complex<T>::operator + (CONST Complex<T>& C) CONST
{
  return Complex<T>(m_Real + C.m_Real, m_Imaginary + C.m_Imaginary);
}
//----------------------------------------------------------------------------------------------------------------------------
template<class T>
Complex<T> Complex<T>::operator - (CONST Complex<T>& C) CONST
{
  return Complex<T>(m_Real - C.m_Real, m_Imaginary - C.m_Imaginary);
}
//----------------------------------------------------------------------------------------------------------------------------
template<class T>
Complex<T> Complex<T>::operator * (CONST Complex<T>& C) CONST
{
  return Complex<T>(m_Real*C.m_Real - m_Imaginary*C.m_Imaginary, m_Real*C.m_Imaginary + m_Imaginary*C.m_Real);
}
//----------------------------------------------------------------------------------------------------------------------------
template<class T>
Complex<T> Complex<T>::operator / (CONST Complex<T>& C) CONST
{
  T D = C.m_Real*C.m_Real + C.m_Imaginary*C.m_Imaginary;
  return Complex<T>((m_Real*C.m_Real + m_Imaginary*C.m_Imaginary)/D, (m_Imaginary*C.m_Real - m_Real*C.m_Imaginary)/D);
}
//----------------------------------------------------------------------------------------------------------------------------
template<class T>
Complex<T>& Complex<T>::operator += (CONST Complex<T>& C)
{
  m_Real += C.m_Real;
  m_Imaginary += C.m_Imaginary;
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------
template<class T>
Complex<T>& Complex<T>::operator -= (CONST Complex<T>& C)
{
  m_Real -= C.m_Real;
  m_Imaginary -= C.m_Imaginary;
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------
template<class T>
Complex<T>& Complex<T>::operator *= (CONST Complex<T>& C)
{
  T R = m_Real*C.m_Real - m_Imaginary*C.m_Imaginary;
  T I = m_Real*C.m_Imaginary + m_Imaginary*C.m_Real;
  m_Real = R;
  m_Imaginary = I;
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------
template<class T>
Complex<T>& Complex<T>::operator /= (CONST Complex<T>& C)
{
  T D = C.m_Real*C.m_Real + C.m_Imaginary*C.m_Imaginary;
  T R = (m_Real*C.m_Real + m_Imaginary*C.m_Imaginary)/D;
  T I = (m_Imaginary*C.m_Real - m_Real*C.m_Imaginary)/D;
  m_Real = R;
  m_Imaginary = I;
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------
template<class T>
Complex<T> Complex<T>::Conjugate() CONST
{
  return Complex<T>(m_Real, -m_Imaginary);
}
//----------------------------------------------------------------------------------------------------------------------------
template<class T>
T Complex<T>::Norm() CONST
{
  return Real()*Real() + Imaginary()*Imaginary();
}
//----------------------------------------------------------------------------------------------------------------------------
template<class T>
T Complex<T>::Arg() CONST
{
  return Math::ATan2(m_Imaginary, m_Real);
}
//----------------------------------------------------------------------------------------------------------------------------
template<class T>
inline T Abs(Complex<T> X)
{
  return Math::Sqrt(X.Real()*X.Real() + X.Imaginary()*X.Imaginary());
}
//----------------------------------------------------------------------------------------------------------------------------
template<class T>
IO::Stream& operator << (IO::Stream& S, CONST Complex<T>& Cmplx)
{
  switch(S.Mode())
  {
  case IO::Stream::Text:
    S << "(" << Cmplx.Real() << "," << Cmplx.Imaginary() << ")";
    break;
  case IO::Stream::Binary:
    S << Cmplx.Real() << Cmplx.Imaginary();
    break;

  };
  return S;
}
