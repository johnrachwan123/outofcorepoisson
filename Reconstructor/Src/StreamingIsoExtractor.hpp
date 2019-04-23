/*
Copyright (c) 2006, Michael Kazhdan and Matthew Bolitho
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer. Redistributions in binary form must reproduce
the above copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the distribution. 

Neither the name of the Johns Hopkins University nor the names of its contributors
may be used to endorse or promote products derived from this software without specific
prior writften permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.
*/

#pragma once

#include "FunctionDataInfo.hpp"
#include "Tree.hpp"
#include "StreamingOctreeTraversal.hpp"

#include "Math/Matrix4.hpp"

using Bolitho::Math::Matrix4;
using Bolitho::Math::Vector3;

class StreamingVolumeExtractor
{
  USHORT m_Depth;
  Ptr<Tree> m_pTree;

  String m_VolumeFilename;

  NodeFunctionDataInfo m_FDI;
  Buffer<FLOAT> m_VolumeData;

  class VolumeExtractionFunction
  {
    friend class StreamingVolumeExtractor;
  public:
    VolumeExtractionFunction(NodeFunctionDataInfo& FDI, Buffer<FLOAT>& VolumeData) : m_FDI(FDI), m_VolumeData(VolumeData)
    {}

  protected:
    UINT m_Resolution;
    USHORT m_Depth;

    NodeFunctionDataInfo& m_FDI;
    Buffer<FLOAT>& m_VolumeData;

  public:
    struct THREADLOCAL { UINT ThreadId; UINT RegionId; UINT PartitionId; };
    VOID InitThreadLocal(Tree* pTree, THREADLOCAL& L) {}
    CONST STATIC BOOL ThreadSafe = FALSE;

    VOID ProcessNode(Tree* pTree, TreeNode& N, THREADLOCAL& L);

  };

  VolumeExtractionFunction m_VEF;

public:
  StreamingVolumeExtractor();
  ~StreamingVolumeExtractor();
  VOID Init(Ptr<Tree> pTree, USHORT Depth, LPCTSTR Filename);

  VOID InitExtractor();
  VOID UpdateExtractor(INT Depth);
  VOID FinalizeExtractor();
};

