/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/FunctionDataImpl.hpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 785 $
 * Last Updated: $Date: 2008-07-31 21:16:10 -0700 (Thu, 31 Jul 2008) $
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

#include "BinaryNode.hpp"

//---------------------------------------------------------------------------------------------------
template<INT Degree,class Real>
FunctionData<Degree,Real>::CompactlySupportedFunctionSamples::CompactlySupportedFunctionSamples()
{
  m_pValues = 0;
  m_pStart = 0;
  m_pEnd = 0;
  m_ppPointers = 0;
  m_Count = 0;
}
//---------------------------------------------------------------------------------------------------
template<INT Degree,class Real>
FunctionData<Degree,Real>::CompactlySupportedFunctionSamples::~CompactlySupportedFunctionSamples()
{
  Free();
}
//---------------------------------------------------------------------------------------------------
template<INT Degree,class Real>
VOID FunctionData<Degree,Real>::CompactlySupportedFunctionSamples::Free()
{
  if(m_pValues)
    delete[] m_pValues;
  if(m_pStart)
    delete[] m_pStart;
  if(m_pEnd)
    delete[] m_pEnd;
  if(m_ppPointers)
    delete[] m_ppPointers;

  m_pValues = 0;
  m_pStart = 0;
  m_pEnd = 0;
  m_ppPointers = 0;
  m_Count = 0;
}
//---------------------------------------------------------------------------------------------------
template<INT Degree,class Real>
VOID FunctionData<Degree,Real>::CompactlySupportedFunctionSamples::Resize(UINT Count)
{
  Free();
  m_Count = Count;

  m_pStart = new INT[Count];
  m_pEnd = new INT[Count];
  m_ppPointers = new Real*[Count];
}
//---------------------------------------------------------------------------------------------------
template<INT Degree,class Real>
Real FunctionData<Degree,Real>::CompactlySupportedFunctionSamples::operator() (UINT Index, INT Sample) CONST
{
  if(Sample >= m_pStart[Index] && Sample < m_pEnd[Index])
    return m_ppPointers[Index][Sample];
  return (Real)0;
}
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
template<INT Degree,class Real>
FunctionData<Degree,Real>::FunctionData()
{
  DotTables = 0;
  DDotTables = 0;
  D2DotTables = 0;
  BaseFunctions = 0;
  m_FunctionCount = 0;
}
//---------------------------------------------------------------------------------------------------
template<INT Degree,class Real>
FunctionData<Degree,Real>::~FunctionData()
{
  if(DotTables)
    delete[] DotTables;
  if(DDotTables)
    delete[] DDotTables;
  if(D2DotTables)
    delete[] D2DotTables;
  DotTables = DDotTables = D2DotTables = 0;

  if(BaseFunctions)
    delete[] BaseFunctions;
  BaseFunctions = 0;

  m_FunctionCount = 0;
}
//---------------------------------------------------------------------------------------------------
template<INT Degree,class Real>
VOID FunctionData<Degree,Real>::Initialize(USHORT Depth, CONST PiecewisePolynomialN<Degree,Real>& F)
{
  m_Depth = Depth;
  m_FunctionCount = (UINT)Bolitho::Tree<2>::GetCumulativeNodeCount(m_Depth); //BinaryNode<DOUBLE>::CumulativeCenterCount(m_Depth);
  m_SampleCount = (1<<(m_Depth+1))+1;

  ValueTables.Resize(m_FunctionCount);
  DValueTables.Resize(m_FunctionCount);

  DotTables = new CompactlySupportedFunctionSamples[m_Depth+1];
  DDotTables = new CompactlySupportedFunctionSamples[m_Depth+1];
  D2DotTables = new CompactlySupportedFunctionSamples[m_Depth+1];

  for(USHORT i=0; i<=m_Depth; i++)
  {
    DotTables[i].Resize(m_FunctionCount);
    DDotTables[i].Resize(m_FunctionCount);
    D2DotTables[i].Resize(m_FunctionCount);
  }

  BaseFunctions = new PiecewisePolynomialN<Degree+1,Real>[m_FunctionCount];
  BaseFunction = F / F(0);
  DBaseFunction = BaseFunction.Derivative();
  Real c1,w1;

  for(UINT i=0; i<m_FunctionCount; i++)
  {
    BinaryNode<Real>::CenterAndWidth(i,c1,w1);
    BaseFunctions[i] = BaseFunction.Scale(w1).Shift(c1);
  }
}
//---------------------------------------------------------------------------------------------------
template<INT Degree,class Real>
VOID FunctionData<Degree,Real>::InitializeDotTables(DWORD flags)
{
  DOUBLE t1,t2;
  for(INT i=0; i<=m_Depth; i++)
  {
    if(DotTables[i].m_pValues)
      delete[] DotTables[i].m_pValues;
    if(DDotTables[i].m_pValues)
      delete[] DDotTables[i].m_pValues;
    if(D2DotTables[i].m_pValues)
      delete[] D2DotTables[i].m_pValues;
    DotTables[i].m_pValues = DDotTables[i].m_pValues = D2DotTables[i].m_pValues = 0;
  }

  SIZE_T count;
  t1 = BaseFunction[0].Start;
  t2 = BaseFunction[BaseFunction.Count()-1].Start;
  for(INT d=0;d<=m_Depth;d++)
  {
    count=0;
    for(UINT i=0; i<m_FunctionCount; i++)
    {
      DotTables[d].m_pStart[i] = DotTables[d].m_pEnd[i] = -1;

      DOUBLE c1,c2,w1,w2;
      BinaryNode<DOUBLE>::CenterAndWidth(i,c1,w1);
      DOUBLE start1	= t1*w1+c1;
      DOUBLE end1		= t2*w1+c1;

      for(INT j=(1<<d)-1;j<(1<<(d+1))-1;j++)
      {
        BinaryNode<DOUBLE>::CenterAndWidth(j,c2,w2);
        DOUBLE start	=t1*w2+c2;
        DOUBLE end		=t2*w2+c2;
        if(start<start1){start=start1;}
        if(end>end1)	{end=end1;}
        if(start<end)
        {
          if(DotTables[d].m_pStart[i]==-1)
            DotTables[d].m_pStart[i]=j;
          DotTables[d].m_pEnd[i]=j+1;
        }
      }
      DotTables[d].m_pStart[i]--;
      DotTables[d].m_pEnd[i]++;
      count += DotTables[d].m_pEnd[i] - DotTables[d].m_pStart[i];

      DDotTables[d].m_pStart[i] = D2DotTables[d].m_pStart[i] = DotTables[d].m_pStart[i];
      DDotTables[d].m_pEnd[i] = D2DotTables[d].m_pEnd[i] = DotTables[d].m_pEnd[i];
    }

    if(flags & FunctionDataUsage::DOT)
      DotTables[d].m_pValues = new Real[count];
    if(flags &  FunctionDataUsage::DDOT)
      DDotTables[d].m_pValues = new Real[count];
    if(flags & FunctionDataUsage::D2DOT)
      D2DotTables[d].m_pValues= new Real[count];
  }

  t1 = BaseFunction[0].Start;
  t2 = BaseFunction[BaseFunction.Count()-1].Start;

  for(INT d=0;d<=m_Depth;d++)
  {
    count=0;
    for(UINT i=0; i<m_FunctionCount; i++)
    {
      DOUBLE c1,c2,w1,w2;
      BinaryNode<DOUBLE>::CenterAndWidth(i,c1,w1);
      DOUBLE start1	=t1*w1+c1;
      DOUBLE end1		=t2*w1+c1;

      if(flags & FunctionDataUsage::DOT)
        DotTables[d].m_ppPointers[i] = DotTables[d].m_pValues + count - DotTables[d].m_pStart[i];
      if(flags & FunctionDataUsage::DDOT)
        DDotTables[d].m_ppPointers[i] = DDotTables[d].m_pValues + count - DDotTables[d].m_pStart[i];
      if(flags & FunctionDataUsage::D2DOT)
        D2DotTables[d].m_ppPointers[i] = D2DotTables[d].m_pValues + count - D2DotTables[d].m_pStart[i];

      count += DotTables[d].m_pEnd[i] - DotTables[d].m_pStart[i];

      for(INT j=DotTables[d].m_pStart[i]; j<DotTables[d].m_pEnd[i]; j++)
      {
        DOUBLE dot;
        BinaryNode<DOUBLE>::CenterAndWidth(j,c2,w2);
        DOUBLE start = t1*w2+c2;
        DOUBLE end = t2*w2+c2;
        if(start < start1)
          start = start1;
        if(end > end1)
          end = end1;
        if(start > end)
        {
          if(flags & FunctionDataUsage::DOT)
            DotTables[d].m_ppPointers[i][j] = 0;
          if(flags & FunctionDataUsage::DDOT)
            DDotTables[d].m_ppPointers[i][j] = 0;
          if(flags & FunctionDataUsage::D2DOT)
            D2DotTables[d].m_ppPointers[i][j] = 0;
          continue;
        }

        dot = DotProduct(c1,w1,c2,w2);

        if(fabs(dot)<1e-15)
        {
          if(flags & FunctionDataUsage::DOT)
            DotTables[d].m_ppPointers[i][j] = 0;
          if(flags & FunctionDataUsage::DDOT)
            DDotTables[d].m_ppPointers[i][j] = 0;
          if(flags & FunctionDataUsage::D2DOT)
            D2DotTables[d].m_ppPointers[i][j] = 0;
          continue;
        }


        if(flags & FunctionDataUsage::DOT)
          DotTables[d].m_ppPointers[i][j] = dot;
        if(flags & FunctionDataUsage::DDOT)
          DDotTables[d].m_ppPointers[i][j] = -DDotProduct(c1,w1,c2,w2)/dot;
        if(flags & FunctionDataUsage::D2DOT)
          D2DotTables[d].m_ppPointers[i][j] = D2DotProduct(c1,w1,c2,w2)/dot;
      }
    }
  }
}
//---------------------------------------------------------------------------------------------------
template<INT Degree,class Real>
VOID FunctionData<Degree,Real>::InitializeValueTables(DWORD flags, Real valueSmooth, Real normalSmooth)
{
  if(ValueTables.m_pValues)
    delete[] ValueTables.m_pValues;
  if(DValueTables.m_pValues)
    delete[] DValueTables.m_pValues;
  ValueTables.m_pValues = DValueTables.m_pValues = 0;

  PiecewisePolynomialN<Degree+1,Real> function;
  PiecewisePolynomialN<Degree,Real> dFunction;

  SIZE_T count = 0;
  SIZE_T dCount = 0;
  for(UINT i=0; i<m_FunctionCount; i++)
  {
    if(valueSmooth > 0)
      function = BaseFunctions[i].MovingAverage(valueSmooth);
    else
      function = BaseFunctions[i];

    if(normalSmooth > 0)
      dFunction = BaseFunctions[i].Derivative().MovingAverage(normalSmooth);
    else
      dFunction = BaseFunctions[i].Derivative();

    ValueTables.m_pStart[i] = ValueTables.m_pEnd[i]=-1;
    DValueTables.m_pStart[i] = DValueTables.m_pEnd[i]=-1;

    for(UINT j=0; j<m_SampleCount; j++)
    {
      DOUBLE x = DOUBLE(j) / (m_SampleCount-1);

      if(x > function[0].Start && x < function[function.Count()-1].Start)
      {
        if(ValueTables.m_pStart[i] == -1)
          ValueTables.m_pStart[i] = j;
        ValueTables.m_pEnd[i] = j+1;
      }

      if(x > dFunction[0].Start && x < dFunction[dFunction.Count()-1].Start)
      {
        if(DValueTables.m_pStart[i] == -1)
          DValueTables.m_pStart[i] = j;
        DValueTables.m_pEnd[i] = j+1;
      }

    }

    ValueTables.m_pStart[i]--;
    ValueTables.m_pEnd[i]++;
    count += ValueTables.m_pEnd[i] - ValueTables.m_pStart[i];

    DValueTables.m_pStart[i]--;
    DValueTables.m_pEnd[i]++;
    dCount += DValueTables.m_pEnd[i] - DValueTables.m_pStart[i];
  }

  if(flags & FunctionDataUsage::VALUE)
    ValueTables.m_pValues = new Real[count];

  if(flags & FunctionDataUsage::DVALUE)
    DValueTables.m_pValues = new Real[dCount];

  count = 0;
  dCount = 0;
  for(UINT i=0; i<m_FunctionCount; i++)
  {
    if(valueSmooth > 0)
      function = BaseFunctions[i].MovingAverage(valueSmooth);
    else
      function = BaseFunctions[i];
    if(normalSmooth > 0)
      dFunction = BaseFunctions[i].Derivative().MovingAverage(normalSmooth);
    else
      dFunction = BaseFunctions[i].Derivative();

    if(flags & FunctionDataUsage::VALUE)
      ValueTables.m_ppPointers[i] = ValueTables.m_pValues + count - ValueTables.m_pStart[i];
    if(flags & FunctionDataUsage::DVALUE)
      DValueTables.m_ppPointers[i] = DValueTables.m_pValues + dCount - DValueTables.m_pStart[i];

    count += ValueTables.m_pEnd[i] - ValueTables.m_pStart[i];
    dCount += DValueTables.m_pEnd[i] - DValueTables.m_pStart[i];

    for(INT j=ValueTables.m_pStart[i]; j<ValueTables.m_pEnd[i]; j++)
    {
      Real x = Real(j)/(m_SampleCount-1);

      if(flags & FunctionDataUsage::VALUE)
        ValueTables.m_ppPointers[i][j] = function(x);
    }

    for(INT j = DValueTables.m_pStart[i]; j<DValueTables.m_pEnd[i]; j++)
    {
      Real x = Real(j)/(m_SampleCount-1);

      if(flags & FunctionDataUsage::DVALUE)
        DValueTables.m_ppPointers[i][j] = dFunction(x);
    }
  }
}
//---------------------------------------------------------------------------------------------------
template<INT Degree,class Real>
Real FunctionData<Degree,Real>::DotProduct(Real center1, Real width1, Real center2, Real width2) CONST
{
  Real r = fabs(BaseFunction[0].Start);
  return Real((BaseFunction * BaseFunction.Scale(width2 / width1).Shift((center2 - center1) / width1)).Integral(-2*r,2*r) * width1);
}
//---------------------------------------------------------------------------------------------------
template<INT Degree,class Real>
Real FunctionData<Degree,Real>::DDotProduct(Real center1, Real width1, Real center2, Real width2) CONST
{
  Real r = fabs(BaseFunction[0].Start);
  return Real((DBaseFunction*BaseFunction.Scale(width2/width1).Shift((center2-center1)/width1)).Integral(-2*r,2*r));
}
//---------------------------------------------------------------------------------------------------
template<INT Degree,class Real>
Real FunctionData<Degree,Real>::D2DotProduct(Real center1, Real width1, Real center2, Real width2) CONST
{
  Real r = fabs(BaseFunction[0].Start);
  return Real((DBaseFunction*DBaseFunction.Scale(width2/width1).Shift((center2-center1)/width1)).Integral(-2*r,2*r)/width2);
}
//---------------------------------------------------------------------------------------------------

