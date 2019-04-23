/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/Histogram.hpp $
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

#pragma once

#include "Graphics/Grid.hpp"

namespace Bolitho
{
	namespace Math
	{
    template<class T>
		class Histogram1 : public Object
		{
      DeclareRuntimeType();
		public:
			Histogram1(UINT BinCount, T Min, T Max);
			Histogram1(CONST Histogram1& H);
			~Histogram1();

			UINT operator() (INT i);

      VOID Add(T x);

			ULONGLONG Count()
			{ return m_SampleCount; }

			DOUBLE Mean()
			{ return (DOUBLE)m_Sum / m_SampleCount; }
			DOUBLE StandardDeviation()
			{ return Sqrt(((DOUBLE)m_SampleCount * m_Sum2 - m_Sum * m_Sum) / ((DOUBLE)m_SampleCount * (DOUBLE)(m_SampleCount - 1))); }
			T MaximumValue()
			{ return m_MaxValue; }
			T MinimumValue()
			{ return m_MinValue; }

		private:
			ULONGLONG m_SampleCount;
      Buffer<UINT> m_Bins;
      UINT m_OutOfBandLow;
      UINT m_OutOfBandHigh;

			T m_Sum;
			T m_Sum2;
			T m_MinValue;
			T m_MaxValue;
		
			T m_MinBinValue;
			T m_MaxBinValue;
      T m_BinSize;
			
		};

    template<class T>
		class Histogram2 : public Object
		{
      DeclareRuntimeType();
		public:
			Histogram2(UINT BinCountX, UINT BinCountY, T MinX, T MaxX, T MinY, T MaxY);
			Histogram2(CONST Histogram2& H);
			~Histogram2();

			UINT operator() (INT i, INT j);

      VOID Add(T x, T y);

			ULONGLONG Count()
			{ return m_SampleCount; }

			DOUBLE MeanX()
			{ return (DOUBLE)m_Sum[0] / m_SampleCount; }
			DOUBLE StandardDeviationX()
			{ return Sqrt(((DOUBLE)m_SampleCount * m_Sum2[0] - m_Sum[0] * m_Sum[0]) / ((DOUBLE)m_SampleCount * (DOUBLE)(m_SampleCount - 1))); }
			DOUBLE MeanY()
			{ return (DOUBLE)m_Sum[1] / m_SampleCount; }
			DOUBLE StandardDeviationY()
			{ return Sqrt(((DOUBLE)m_SampleCount * m_Sum2[1] - m_Sum[1] * m_Sum[1]) / ((DOUBLE)m_SampleCount * (DOUBLE)(m_SampleCount - 1))); }

      T MaximumValueX()
			{ return m_MaxValue[0]; }
			T MinimumValueX()
			{ return m_MinValue[0]; }

      T MaximumValueY()
			{ return m_MaxValue[1]; }
			T MinimumValueY()
			{ return m_MinValue[1]; }

		private:
			ULONGLONG m_SampleCount;
      Buffer<UINT> m_Bins;

			T m_Sum[2];
			T m_Sum2[2];
			T m_MinValue[2];
			T m_MaxValue[2];

			T m_MinBinValue[2];
			T m_MaxBinValue[2];
      T m_BinSize[2];
			
		};

    #include "HistogramImpl.hpp"

	}
}

