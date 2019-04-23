/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/FunctionDataInfo.hpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 829 $
 * Last Updated: $Date: 2008-10-29 06:41:39 -0700 (Wed, 29 Oct 2008) $
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

#include "FunctionData.hpp"

#include "Math/Matrix3.hpp"
using Bolitho::Math::Matrix3;
using Bolitho::Math::Vector3;

class FunctionDataInfo2
{
public:
  #if defined(CONFIG_SOLVEDOUBLE)
  ::FunctionData<2,DOUBLE> FunctionData;
  #else
  ::FunctionData<2,FLOAT> FunctionData;
  #endif

  VOID Initialize(USHORT Depth, FLOAT SampleRadius = 1.0f);

  DOUBLE GetDotProduct(USHORT Index1[3], USHORT Depth, USHORT Index2[3]) CONST;
  DOUBLE GetLaplacian(USHORT Index1[3], USHORT Depth, USHORT Index2[3]) CONST;
  DOUBLE GetDivergence(USHORT Index1[3], USHORT Depth, USHORT Index2[3], CONST FLOAT Normal[3]) CONST;
  VOID GetDivergenceAndLaplacian(USHORT Index1[3], USHORT Depth, USHORT Index2[3], CONST FLOAT Normal[3], DOUBLE& Divergence, DOUBLE& Laplacian);

  VOID GetWeights(FLOAT x, Vector3<DOUBLE>& weights);

private:
  Matrix3<DOUBLE> m_InverseDotMatrix;
  PiecewisePolynomialN<5,DOUBLE> m_Convolution[3];
};

class FunctionDataInfo1
{
public:
  #if defined(CONFIG_SOLVEDOUBLE)
  ::FunctionData<1,DOUBLE> FunctionData;
  #else
  ::FunctionData<1,FLOAT> FunctionData;
  #endif

  VOID Initialize(USHORT Depth, FLOAT SampleRadius = 1.0f);

  DOUBLE GetDotProduct(USHORT Index1[3], USHORT Depth, USHORT Index2[3]) CONST;
  DOUBLE GetLaplacian(USHORT Index1[3], USHORT Depth, USHORT Index2[3]) CONST;
  DOUBLE GetDivergence(USHORT Index1[3], USHORT Depth, USHORT Index2[3], CONST FLOAT Normal[3]) CONST;
  VOID GetDivergenceAndLaplacian(USHORT Index1[3], USHORT Depth, USHORT Index2[3], CONST FLOAT Normal[3], DOUBLE& Divergence, DOUBLE& Laplacian);

  VOID GetWeights(FLOAT x, Vector3<DOUBLE>& weights);

private:
  Matrix3<DOUBLE> m_InverseDotMatrix;
  PiecewisePolynomialN<5,DOUBLE> m_Convolution[3];
};

typedef FunctionDataInfo2 NodeFunctionDataInfo;
typedef FunctionDataInfo1 CornerFunctionDataInfo;