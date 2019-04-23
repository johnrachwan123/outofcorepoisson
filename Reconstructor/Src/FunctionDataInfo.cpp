/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/FunctionDataInfo.cpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 812 $
 * Last Updated: $Date: 2008-09-15 07:52:40 -0700 (Mon, 15 Sep 2008) $
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

#include "Config.hpp"
#include "FunctionDataInfo.hpp"

#include "Math/MatrixN.hpp"
using Bolitho::Math::MatrixN;
using Bolitho::Math::VectorN;

//---------------------------------------------------------------------------------------------------
VOID FunctionDataInfo2::Initialize(USHORT Depth, FLOAT SampleRadius)
{
  PiecewisePolynomialN<2,DOUBLE> ReconstructionFunction = PiecewisePolynomialN<2,DOUBLE>::GaussianApproximation();
  FunctionData.Initialize(Depth, ReconstructionFunction);


  CONST INT R_SIZE = 1;
  PiecewisePolynomialN<2,DOUBLE> tempPoly[2*R_SIZE+1];
  for(INT i=-R_SIZE; i<=R_SIZE; i++)
    tempPoly[i+R_SIZE] = FunctionData.BaseFunction.Shift(i);
  DOUBLE start = tempPoly[0].Start();
  DOUBLE end = tempPoly[2*R_SIZE].End();

  MatrixN<2*R_SIZE+1,DOUBLE> dotMatrix,U,D;
  VectorN<2*R_SIZE+1,DOUBLE> eVectors[2*R_SIZE+1],eValues;
  for(INT i=0;i<2*R_SIZE+1;i++)
    for(INT j=0;j<2*R_SIZE+1;j++)
      dotMatrix(i,j) = (tempPoly[i] * tempPoly[j]).Integral(start,end);

  dotMatrix.EigenDecomposition(eVectors,eValues);

  for(INT i=0;i<2*R_SIZE+1;i++)
  {
    D(i,i) = 1.0 / eValues(i);
    for(INT j=0; j<2*R_SIZE+1; j++)
      U(i,j) = eVectors[i](j);
  }

  m_InverseDotMatrix = U.Transpose() * D * U;

  CONST DOUBLE EPS = .00000001;
  PiecewisePolynomialN<5,DOUBLE> temp = FunctionData.BaseFunction.MovingAverage(SampleRadius).MovingAverage(SampleRadius).MovingAverage(SampleRadius);
  for(INT i=-R_SIZE;i<=R_SIZE;i++)
    m_Convolution[i+R_SIZE] = temp.Clip(i-0.5-EPS,i+0.5+EPS);

}
//---------------------------------------------------------------------------------------------------
DOUBLE FunctionDataInfo2::GetDivergence(USHORT idx1[3], USHORT depth, USHORT idx2[3], const FLOAT normal[3]) CONST
{
  DOUBLE Dot =
    FunctionData.DotTables[depth](idx1[0],idx2[0]) *
    FunctionData.DotTables[depth](idx1[1],idx2[1]) *
    FunctionData.DotTables[depth](idx1[2],idx2[2]);

  return Dot * 
    FunctionData.DDotTables[depth](idx1[0],idx2[0]) * normal[0] + 
    FunctionData.DDotTables[depth](idx1[1],idx2[1]) * normal[1] + 
    FunctionData.DDotTables[depth](idx1[2],idx2[2]) * normal[2];
}
//---------------------------------------------------------------------------------------------------
DOUBLE FunctionDataInfo2::GetDotProduct(USHORT idx1[3], USHORT depth, USHORT idx2[3]) CONST
{
  return FunctionData.DotTables[depth](idx1[0],idx2[0]) * FunctionData.DotTables[depth](idx1[1],idx2[1]) * FunctionData.DotTables[depth](idx1[2],idx2[2]);
}
//---------------------------------------------------------------------------------------------------
DOUBLE FunctionDataInfo2::GetLaplacian(USHORT idx1[3], USHORT depth, USHORT idx2[3]) CONST
{
  DOUBLE Dot =
    FunctionData.DotTables[depth](idx1[0],idx2[0]) *
    FunctionData.DotTables[depth](idx1[1],idx2[1]) *
    FunctionData.DotTables[depth](idx1[2],idx2[2]);

  return Dot * 
    FunctionData.D2DotTables[depth](idx1[0],idx2[0]) +
    FunctionData.D2DotTables[depth](idx1[1],idx2[1]) +
    FunctionData.D2DotTables[depth](idx1[2],idx2[2]);
}
//---------------------------------------------------------------------------------------------------
VOID FunctionDataInfo2::GetDivergenceAndLaplacian(USHORT idx1[3], USHORT depth, USHORT idx2[3], CONST FLOAT normal[3], DOUBLE& div, DOUBLE& lap)
{
  DOUBLE Dot =
    FunctionData.DotTables[depth](idx1[0],idx2[0]) *
    FunctionData.DotTables[depth](idx1[1],idx2[1]) *
    FunctionData.DotTables[depth](idx1[2],idx2[2]);

  div = Dot * 
    FunctionData.DDotTables[depth](idx1[0],idx2[0]) * normal[0] + 
    FunctionData.DDotTables[depth](idx1[1],idx2[1]) * normal[1] + 
    FunctionData.DDotTables[depth](idx1[2],idx2[2]) * normal[2];

  lap = Dot * 
    FunctionData.D2DotTables[depth](idx1[0],idx2[0]) +
    FunctionData.D2DotTables[depth](idx1[1],idx2[1]) +
    FunctionData.D2DotTables[depth](idx1[2],idx2[2]);
}
//---------------------------------------------------------------------------------------------------
VOID FunctionDataInfo2::GetWeights(FLOAT x, Vector3<DOUBLE>& Weights)
{
  Weights(0) = m_Convolution[0](-1-x);
  Weights(1) = m_Convolution[1]( -x);
  Weights(2) = m_Convolution[2](1-x);
  Weights = m_InverseDotMatrix * Weights;

  DOUBLE WeightSum = Weights(0) + Weights(1) + Weights(2);
  Weights(0) /= WeightSum;
  Weights(1) /= WeightSum;
  Weights(2) /= WeightSum;
}
//---------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------
VOID FunctionDataInfo1::Initialize(USHORT Depth, FLOAT SampleRadius)
{
  PiecewisePolynomialN<1,DOUBLE> ReconstructionFunction = PiecewisePolynomialN<1,DOUBLE>::GaussianApproximation();
  FunctionData.Initialize(Depth, ReconstructionFunction);

  CONST INT R_SIZE = 1;
  PiecewisePolynomialN<1,DOUBLE> tempPoly[2*R_SIZE+1];
  
  for(INT i=-R_SIZE; i<=R_SIZE; i++)
    tempPoly[i+R_SIZE] = FunctionData.BaseFunction.Shift(i);
  
  DOUBLE Start = tempPoly[0].Start();
  DOUBLE End = tempPoly[2*R_SIZE].End();

  MatrixN<2*R_SIZE+1,DOUBLE> DotMatrix,U,D;
  VectorN<2*R_SIZE+1,DOUBLE> EigenVectors[2*R_SIZE+1], EigenValues;
  
  for(INT i=0;i<2*R_SIZE+1;i++)
    for(INT j=0;j<2*R_SIZE+1;j++)
      DotMatrix(i,j) = (tempPoly[i] * tempPoly[j]).Integral(Start, End);

  DotMatrix.EigenDecomposition(EigenVectors, EigenValues);

  for(INT i=0;i<2*R_SIZE+1;i++)
  {
    D(i,i) = 1.0 / EigenValues(i);
    for(INT j=0; j<2*R_SIZE+1; j++)
      U(i,j) = EigenVectors[i](j);
  }

  m_InverseDotMatrix = U.Transpose() * D * U;

  CONST DOUBLE EPS = .00000001;
  PiecewisePolynomialN<4,DOUBLE> temp = FunctionData.BaseFunction.MovingAverage(SampleRadius).MovingAverage(SampleRadius).MovingAverage(SampleRadius);
  for(INT i=-R_SIZE;i<=R_SIZE;i++)
    m_Convolution[i+R_SIZE] = temp.Clip(i-0.5-EPS,i+0.5+EPS);

}
//---------------------------------------------------------------------------------------------------
DOUBLE FunctionDataInfo1::GetDivergence(USHORT idx1[3], USHORT depth, USHORT idx2[3], const FLOAT normal[3]) CONST
{
  DOUBLE Dot =
    FunctionData.DotTables[depth](idx1[0],idx2[0]) *
    FunctionData.DotTables[depth](idx1[1],idx2[1]) *
    FunctionData.DotTables[depth](idx1[2],idx2[2]);

  return Dot * 
    FunctionData.DDotTables[depth](idx1[0],idx2[0]) * normal[0] + 
    FunctionData.DDotTables[depth](idx1[1],idx2[1]) * normal[1] + 
    FunctionData.DDotTables[depth](idx1[2],idx2[2]) * normal[2];
}
//---------------------------------------------------------------------------------------------------
DOUBLE FunctionDataInfo1::GetDotProduct(USHORT idx1[3], USHORT depth, USHORT idx2[3]) CONST
{
  return FunctionData.DotTables[depth](idx1[0],idx2[0]) * FunctionData.DotTables[depth](idx1[1],idx2[1]) * FunctionData.DotTables[depth](idx1[2],idx2[2]);
}
//---------------------------------------------------------------------------------------------------
DOUBLE FunctionDataInfo1::GetLaplacian(USHORT idx1[3], USHORT depth, USHORT idx2[3]) CONST
{
  DOUBLE Dot =
    FunctionData.DotTables[depth](idx1[0],idx2[0]) *
    FunctionData.DotTables[depth](idx1[1],idx2[1]) *
    FunctionData.DotTables[depth](idx1[2],idx2[2]);

  return Dot * 
    FunctionData.D2DotTables[depth](idx1[0],idx2[0]) +
    FunctionData.D2DotTables[depth](idx1[1],idx2[1]) +
    FunctionData.D2DotTables[depth](idx1[2],idx2[2]);
}
//---------------------------------------------------------------------------------------------------
VOID FunctionDataInfo1::GetDivergenceAndLaplacian(USHORT idx1[3], USHORT depth, USHORT idx2[3], CONST FLOAT normal[3], DOUBLE& div, DOUBLE& lap)
{
  DOUBLE Dot =
    FunctionData.DotTables[depth](idx1[0],idx2[0]) *
    FunctionData.DotTables[depth](idx1[1],idx2[1]) *
    FunctionData.DotTables[depth](idx1[2],idx2[2]);

  div = Dot * 
    FunctionData.DDotTables[depth](idx1[0],idx2[0]) * normal[0] + 
    FunctionData.DDotTables[depth](idx1[1],idx2[1]) * normal[1] + 
    FunctionData.DDotTables[depth](idx1[2],idx2[2]) * normal[2];

  lap = Dot * 
    FunctionData.D2DotTables[depth](idx1[0],idx2[0]) +
    FunctionData.D2DotTables[depth](idx1[1],idx2[1]) +
    FunctionData.D2DotTables[depth](idx1[2],idx2[2]);
}
//---------------------------------------------------------------------------------------------------
VOID FunctionDataInfo1::GetWeights(FLOAT x, Vector3<DOUBLE>& Weights)
{
  Weights(0) = m_Convolution[0](-1-x);
  Weights(1) = m_Convolution[1]( -x);
  Weights(2) = m_Convolution[2](1-x);
  Weights = m_InverseDotMatrix * Weights;

  DOUBLE WeightSum = Weights(0) + Weights(1) + Weights(2);
  Weights(0) /= WeightSum;
  Weights(1) /= WeightSum;
  Weights(2) /= WeightSum;
}
//---------------------------------------------------------------------------------------------------

