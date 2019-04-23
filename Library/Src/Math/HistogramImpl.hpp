/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/HistogramImpl.hpp $
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
ImplementTemplateRuntimeType(Bolitho::Math, Histogram1, Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Histogram1<T>::Histogram1(UINT BinCount, T MinValue, T MaxValue)
{
  m_MinBinValue = MinValue;
  m_MaxBinValue = MaxValue;
  m_BinSize = MaxValue - MinValue / BinCount;

  m_Sum = 0;
  m_Sum2 = 0;
  m_MinValue = 0;
  m_MaxValue = 0;
  m_SampleCount = 0;
  m_OutOfBandLow = 0;
  m_OutOfBandHigh = 0;
  
  m_Bins.Resize(BinCount);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Histogram1<T>::Histogram1(CONST Histogram1& H)
{
  m_MinBinValue = H.m_MinBinValue;
  m_MaxBinValue = H.m_MaxBinValue;
  m_BinSize = H.m_BinSize;

  m_Bins = H.m_Bins;
  
  m_Sum = H.m_Sum;
  m_Sum2 = H.m_Sum2;
  m_MinValue = H.m_MinValue;
  m_MaxValue = H.m_MaxValue;
  m_SampleCount = H.m_SampleCount;
  m_OutOfBandLow = H.m_OutOfBandLow;
  m_OutOfBandHigh = H.m_OutOfBandHigh;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Histogram1<T>::~Histogram1()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
UINT Histogram1<T>::operator() (INT i)
{
  return m_Bins[i];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID Histogram1<T>::Add(T X)
{
  if (m_SampleCount == 0)
    m_MaxValue = m_MinValue = X;
  
  m_SampleCount++;
  m_Sum += X;
  m_Sum2 += X*X;
  
  m_MaxValue = Max(m_MaxValue, X);
  m_MinValue = Min(m_MinValue, X);
  
  
  if (X < m_MinBinValue)
  {
    m_OutOfBandLow++;
    return;
  }
  
  if (X >= m_MaxBinValue)
  {
    m_OutOfBandHigh++;
    return;
  }

  UINT IndexX = (UINT)(m_Bins.Length() * (FLOAT)(X - m_MinBinValue) / (m_MaxBinValue - m_MinBinValue));
  m_Bins[IndexX]++;
  
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
