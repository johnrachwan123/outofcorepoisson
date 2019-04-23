/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/SurfaceReconstruction.hpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 894 $
 * Last Updated: $Date: 2008-12-21 03:47:31 -0800 (Sun, 21 Dec 2008) $
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

#include "Tree.hpp"

#include "System/ConsoleApplication.hpp"
using Bolitho::System::ConsoleApplication;

#include "System/Profiler.hpp"
using Bolitho::System::ApplicationProfiler;

#include "IO/MemoryStream.hpp"
using Bolitho::IO::MemoryStream;

#include "Utility/Progress.hpp"
using Bolitho::Progress;

class SurfaceReconstruction : public ConsoleApplication
{
public:
  SurfaceReconstruction();

  virtual BOOL OnInit();
  virtual INT Main();
  virtual VOID OnExit();

private:
  VOID ValidateInput();

  VOID BuildOctree();
  VOID ExtractVolume();
  VOID SolveLaplacian();
  VOID ExtractIsoSurface();
  VOID WritePly();

  String m_StreamingFilename;
  String m_Input;
  String m_Output;
  String m_InterpolationAttributeName;

  INT m_Depth;
  FLOAT m_SamplesPerNode;
  BOOL m_ClipTree;
  BOOL m_NoTransform;
  BOOL m_NoNormalize;
  FLOAT m_SampleRadius;
  INT m_PeakDepth;
  DWORD m_Steps;
  FLOAT m_SampleRate;
  UINT m_Verbose;

  BOOL m_Validation;

  Ptr<File> m_pVertices;
  Ptr<File> m_pTriangles;

  Ptr<Tree> m_pTree;

  Ptr<ApplicationProfiler> m_pProfiler;
  Ptr<MemoryStream> m_pTraceLog;

  Progress m_Progress;
};

