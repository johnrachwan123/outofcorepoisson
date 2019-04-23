/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/FunctionData.hpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 879 $
 * Last Updated: $Date: 2008-11-12 18:35:28 -0800 (Wed, 12 Nov 2008) $
 * 
 * 
 * Copyright (c) 2006-2007, Matthew G Bolitho;  Michael Kazhdan
 * All rights reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer. Redistributions in binary form must reproduce
 * the above copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the distribution. 
 * 
 * Neither the name of the Johns Hopkins University nor the names of its contributors
 * may be used to endorse or promote products derived from this software without specific
 * prior written permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 ***************************************************************************************************/



#pragma once

#include "Math/PiecewisePolynomial.hpp"
using Bolitho::Math::PiecewisePolynomialN;

struct FunctionDataUsage 
{ enum { DOT = 1, DDOT = 2, D2DOT = 4, VALUE = 1, DVALUE = 2 }; };

template<INT Degree, class REAL>
class FunctionData
{
public:
  class CompactlySupportedFunctionSamples
  {
    template<INT Degree, class REAL> friend class FunctionData;
  public:
    CompactlySupportedFunctionSamples();
    ~CompactlySupportedFunctionSamples();

    VOID Resize(UINT Count);

    REAL operator() (UINT Index, INT Sample) CONST;

  protected:
    REAL* m_pValues;
    REAL** m_ppPointers;
    INT* m_pStart;
    INT* m_pEnd;
    UINT m_Count;

    VOID Free();
  };

  USHORT Depth() CONST
  { return m_Depth; }
  UINT FunctionCount() CONST
  { return m_FunctionCount; }
  UINT SampleCount() CONST
  { return m_SampleCount; }

private:
  USHORT m_Depth;
  UINT m_FunctionCount;
  UINT m_SampleCount;

public:
  CompactlySupportedFunctionSamples ValueTables;
  CompactlySupportedFunctionSamples DValueTables;
  CompactlySupportedFunctionSamples *DotTables;
  CompactlySupportedFunctionSamples *DDotTables;
  CompactlySupportedFunctionSamples *D2DotTables;

  PiecewisePolynomialN<Degree,REAL> BaseFunction;
  PiecewisePolynomialN<Degree-1,REAL> DBaseFunction;
  PiecewisePolynomialN<Degree+1,REAL>* BaseFunctions;

  FunctionData();
  ~FunctionData();

  VOID InitializeDotTables(DWORD UsageFlags);
  VOID InitializeValueTables(DWORD UsageFlags, REAL ValueSmooth = 0, REAL NormalSmooth = 0);

  VOID Initialize(USHORT Depth, CONST PiecewisePolynomialN<Degree,REAL>& F);

  REAL DotProduct(REAL Center1, REAL Width1, REAL Center2, REAL Width2) CONST;
  REAL DDotProduct(REAL Center1, REAL Width1, REAL Center2, REAL Width2) CONST;
  REAL D2DotProduct(REAL Center1, REAL Width1, REAL Center2, REAL Width2) CONST;
};

#include "FunctionDataImpl.hpp"

