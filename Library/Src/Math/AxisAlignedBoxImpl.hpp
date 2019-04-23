/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/AxisAlignedBoxImpl.hpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 1738 $
 * Last Updated: $Date: 2008-12-23 12:31:52 -0800 (Tue, 23 Dec 2008) $
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
AxisAlignedBox2<T>::AxisAlignedBox2()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
AxisAlignedBox2<T>::AxisAlignedBox2(CONST Vector2<T>& Min, CONST Vector2<T>& Max)
{
  m_Minimum = Min;
  m_Maximum = Max;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
AxisAlignedBox3<T>::AxisAlignedBox3()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
AxisAlignedBox3<T>::AxisAlignedBox3(CONST Vector3<T>& Min, CONST Vector3<T>& Max)
{
  m_Minimum = Min;
  m_Maximum = Max;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID AxisAlignedBox3<T>::Union(CONST Vector3<T>& P)
{
  if (P[0] < m_Minimum[0])
    m_Minimum[0] = P[0];
  if (P[1] < m_Minimum[1])
    m_Minimum[1] = P[1];
  if (P[2] < m_Minimum[2])
    m_Minimum[2] = P[2];

  if (P[0] > m_Maximum[0])
    m_Maximum[0] = P[0];
  if (P[1] > m_Maximum[1])
    m_Maximum[1] = P[1];
  if (P[2] > m_Maximum[2])
    m_Maximum[2] = P[2];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID AxisAlignedBox3<T>::Union(CONST AxisAlignedBox3<T>& B)
{
  if (B.m_Minimum[0] < m_Minimum[0])
    m_Minimum[0] = B.m_Minimum[0];
  if (B.m_Minimum[1] < m_Minimum[1])
    m_Minimum[1] = B.m_Minimum[1];
  if (B.m_Minimum[2] < m_Minimum[2])
    m_Minimum[2] = B.m_Minimum[2];

  if (B.m_Maximum[0] > m_Maximum[0])
    m_Maximum[0] = B.m_Maximum[0];
  if (B.m_Maximum[1] > m_Maximum[1])
    m_Maximum[1] = B.m_Maximum[1];
  if (B.m_Maximum[2] > m_Maximum[2])
    m_Maximum[2] = B.m_Maximum[2];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
BOOL AxisAlignedBox3<T>::Intersect(Ray3<T>& R, Vector3<T>& IntersectionPosition) CONST
{
  CONST UINT RIGHT = 0;
  CONST UINT LEFT = 1;
  CONST UINT MIDDLE = 2;

  BOOL Inside = TRUE;
  UINT Quadrant[3];
  T MaxT[3];
  T CandidatePlane[3];

  /* Find candidate planes; this loop can be avoided if rays cast all from the eye(assume perpsective view) */
  for (UINT i=0; i<3; i++)
  {
    if(R.Point[i] < m_Minimum[i])
    {
      Quadrant[i] = LEFT;
      CandidatePlane[i] = m_Minimum[i];
      Inside = FALSE;
    }
    else if (R.Point[i] > m_Maximum[i]) 
    {
      Quadrant[i] = RIGHT;
      CandidatePlane[i] = m_Maximum[i];
      Inside = FALSE;
    }
    else
    {
      Quadrant[i] = MIDDLE;
    }
  }

  /* Ray origin inside bounding box */
  if(Inside)
  {
    IntersectionPosition = R.Point;
    return TRUE;
  }


  /* Calculate T distances to candidate planes */
  for (UINT i=0; i<3; i++)
    if (Quadrant[i] != MIDDLE && R.Direction[i] != 0.0f)
      MaxT[i] = (CandidatePlane[i] - R.Point[i]) / R.Direction[i];
    else
      MaxT[i] = -1.0f;

  /* Get largest of the maxT's for final choice of intersection */
  UINT WhichPlane = 0;
  for (UINT i=1; i<3; i++)
    if (MaxT[WhichPlane] < MaxT[i])
      WhichPlane = i;

  /* Check final candidate actually inside box */
  if (MaxT[WhichPlane] < 0.) 
    return FALSE;
  
  for (UINT i=0; i<3; i++)
  {
    if (WhichPlane != i) 
    {
      IntersectionPosition[i] = R.Point[i] + MaxT[WhichPlane] * R.Direction[i];
      if (IntersectionPosition[i] < m_Minimum[i] || IntersectionPosition[i] > m_Maximum[i])
        return FALSE;
    } 
    else 
    {
      IntersectionPosition[i] = CandidatePlane[i];
    }
  }
  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST AxisAlignedBox3<T> AxisAlignedBox3<T>::Empty = AxisAlignedBox3<T>(Vector3F(RealTraits<FLOAT>::MaxValue,  RealTraits<FLOAT>::MaxValue,  RealTraits<FLOAT>::MaxValue), Vector3F(-RealTraits<FLOAT>::MaxValue,  -RealTraits<FLOAT>::MaxValue,  -RealTraits<FLOAT>::MaxValue));
//-----------------------------------------------------------------------------------------------------------------------------------------------------

