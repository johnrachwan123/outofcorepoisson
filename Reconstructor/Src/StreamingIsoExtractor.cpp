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
prior written permission. 

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

#include "Config.hpp"

#include "StreamingIsoExtractor.hpp"
#include "StreamingOctreeTraversal.hpp"

VOID StreamingVolumeExtractor::VolumeExtractionFunction::ProcessNode(Tree* pTree,TreeNode& N, THREADLOCAL& L)
{
  int start[3],end[3],idx[3],d,off[3];
  N.GetDepthAndOffset(d,off);
  
  for(int i=0;i<3;i++)
  {
    start[i]=off[i]-1;
    end[i]=off[i]+1;
    
    if(start[i]<0)
      start[i]=0;
    
    if(end[i]>=(1<<d))
      end[i]=(1<<d)-1;

    if(d > m_Depth)
    {
      start[i]>>=(d-m_Depth);
      end[i]	>>=(d-m_Depth);
    }
    else if (d<m_Depth)
    {
      start[i]<<=(m_Depth-d);
      end[i] <<=(m_Depth-d);
      end[i]+=(1<<(m_Depth-d))-1;
    }

  }
  DOUBLE x,xy;
  for(int i=start[0];i<=end[0];i++)
  {
    idx[0]=(i<<(pTree->Height()-m_Depth))+(1<<(pTree->Height()-m_Depth))-1;
    x=N.WeightContribution() * m_FDI.FunctionData.ValueTables(N.OffsetX(),idx[0]);
    for(int j=start[1];j<=end[1];j++)
    {
      idx[1]=(j<<(pTree->Height()-m_Depth))+(1<<(pTree->Height()-m_Depth))-1;
      xy=x*m_FDI.FunctionData.ValueTables(N.OffsetY(),idx[1]);
      for(int k=start[2];k<=end[2];k++)
      {
        idx[2]=(k<<(pTree->Height()-m_Depth))+(1<<(pTree->Height()-m_Depth))-1;
        m_VolumeData[i*m_Resolution*m_Resolution+j*m_Resolution+k] += (FLOAT)(xy * m_FDI.FunctionData.ValueTables(N.OffsetZ(),idx[2]));
      }
    }
  }
  
  /*
  Trace("Node: %d {%d %d %d}", d, off[0], off[1], off[2]);
  values[off[0]*res*res+off[1]*res+off[2]] = 1.0f; // +=xy*m_FDI.FunctionData.ValueTables(N.OffsetZ(),idx[2]);
  */

}

StreamingVolumeExtractor::StreamingVolumeExtractor() : m_VEF(m_FDI, m_VolumeData)
{
}

StreamingVolumeExtractor::~StreamingVolumeExtractor()
{
}

VOID StreamingVolumeExtractor::Init(Ptr<Tree> pTree, USHORT Depth, LPCTSTR Filename)
{
  m_pTree = pTree;
  m_VolumeFilename = Filename;
  m_Depth = Depth;
  m_FDI.Initialize(pTree->Height()-1);
  m_FDI.FunctionData.InitializeValueTables(FunctionDataUsage::VALUE);

  if(m_Depth > pTree->MaximumDepth())
    m_Depth = pTree->MaximumDepth();
  
  m_VEF.m_Depth = Depth;
  m_VEF.m_Resolution = 1 << m_Depth;
  m_VolumeData.Resize(m_VEF.m_Resolution * m_VEF.m_Resolution * m_VEF.m_Resolution);

  pTree->Initialise(0, 0, DataAccess::READ, DataAccess::READ, DataAccess::NONE);

}

VOID StreamingVolumeExtractor::InitExtractor()
{
}

VOID StreamingVolumeExtractor::UpdateExtractor(INT depth)
{
  //for(INT d=m_pTree->MaximumDepth();d >= depth; d--)
  //  m_pTree->IterateNodes(0,d,m_IVF);
  m_pTree->IterateNodes(0, m_Depth, m_VEF);
  m_pTree->AdvanceNodes(m_Depth);
}

VOID StreamingVolumeExtractor::FinalizeExtractor(VOID)
{
  //for(INT d=m_pTree->Height()-1;d>=0;d--)
  //  m_pTree->IterateNodes(0,d,m_IVF);
  m_pTree->IterateNodes(0, m_Depth, m_VEF);
  m_pTree->Finalise();

  Ptr<File> pVolume = File::Open(m_VolumeFilename, FileMode::CREATE);
  pVolume->Write(&m_VEF.m_Resolution, sizeof(UINT));
  pVolume->Write(m_VolumeData.GetBuffer(), m_VolumeData.Length() * sizeof(FLOAT));
}
