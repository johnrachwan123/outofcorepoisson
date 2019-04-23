/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Utility/AlgorithmImpl.hpp $
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

//---------------------------------------------------------------------------------------------------
template<class T>
Range<T>::Range()
{
  m_Begin = m_End = m_Grain = 0;
}
//---------------------------------------------------------------------------------------------------
template<class T>
Range<T>::Range(T Begin, T End)
{
  m_Begin = Begin;
  m_End = End;
  m_Grain = End-Begin;
}
//---------------------------------------------------------------------------------------------------
template<class T>
Range<T>::Range(T Begin, T End, T Grain)
{
  m_Begin = Begin;
  m_End = End;
  m_Grain = Grain;
}
//---------------------------------------------------------------------------------------------------
template<class T>
BOOL Range<T>::Split(Range<T>& Upper)
{
  if (!IsDivisible())
    return FALSE;
  T Middle = RoundUp(m_Begin + (m_End-m_Begin) / 2, m_Grain);
  
  Upper.m_Begin = Middle;
  Upper.m_End = m_End;
  Upper.m_Grain = m_Grain;
  m_End = Middle;

  return TRUE;
}
//---------------------------------------------------------------------------------------------------
template<class T>
BOOL Range<T>::Split(Array<Range<T>>& R)
{
  BOOL SplitPerformed = FALSE;
  SIZE_T Length = R.Length();
  for (SIZE_T i=0; i<Length; i++)
  {
    Range<T> Upper;
    if (R[i].Split(Upper))
    {
      R.Add(Upper);
      SplitPerformed |= TRUE;
    }
  }

  return SplitPerformed;
}
//---------------------------------------------------------------------------------------------------
template<class T>
Range2<T>::Range2(T StartX, T EndX, T StartY, T EndY)
{
}
//---------------------------------------------------------------------------------------------------
template<class T>
Range3<T>::Range3(T StartX, T EndX, T StartY, T EndY, T StartZ, T EndZ)
{
}
//---------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------
template<class RANGE, class FUNCTION>
VOID For(RANGE R, FUNCTION& F)
{
  Array<RANGE> Ranges;
  Ranges.Add(R);
  while (RANGE::Split(Ranges));
  
  for (SIZE_T i=0; i<Ranges.Length(); i++)
    F(Ranges[i]);
}
//---------------------------------------------------------------------------------------------------
template<class RANGE, class FUNCTION>
VOID For(RANGE R0, RANGE R1, FUNCTION& F)
{
  Array<RANGE> Ranges0;
  Ranges0.Add(R0);
  while (RANGE::Split(Ranges0));
  
  Array<RANGE> Ranges1;
  Ranges1.Add(R1);
  while (RANGE::Split(Ranges1));
  
  for (SIZE_T i=0; i<Ranges0.Length(); i++)
    for (SIZE_T j=0; j<Ranges1.Length(); j++)
      F(Ranges0[i], Ranges1[j]);
}
//---------------------------------------------------------------------------------------------------
template<class RANGE, class FUNCTION>
VOID For(RANGE R0, RANGE R1, RANGE R2, FUNCTION& F)
{
  Array<RANGE> Ranges0;
  Ranges0.Add(R0);
  while (Range::Split(Ranges0));
  
  Array<RANGE> Ranges1;
  Ranges1.Add(R1);
  while (Range::Split(Ranges1));
  
  Array<RANGE> Ranges2;
  Ranges2.Add(R2);
  while (Range::Split(Ranges2));

  for (SIZE_T i=0; i<Ranges0.Length(); i++)
    for (SIZE_T j=0; j<Ranges1.Length(); j++)
      for (SIZE_T k=0; k<Ranges2.Length(); k++)
        F(Ranges0[i], Ranges1[j], Ranges2[k]);
}
//---------------------------------------------------------------------------------------------------
