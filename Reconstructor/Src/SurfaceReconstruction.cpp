/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/SurfaceReconstruction.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 917 $
 * Last Updated: $Date: 2008-12-29 14:41:52 -0800 (Mon, 29 Dec 2008) $
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
#include "SurfaceReconstruction.hpp"

#include "StreamingOctreeConstructor.hpp"
#include "StreamingLaplacianSolver.hpp"
#include "StreamingSurfaceExtractor.hpp"
#include "StreamingPointConverter.hpp"

#include "Memory/Md5.hpp"
using Bolitho::Md5;
using Bolitho::Md5Hash;

#include "IO/BinaryStream.hpp"
using Bolitho::IO::BinaryStream;

#include "IO/TextStream.hpp"
using Bolitho::IO::TextStream;

#include "Utility/Progress.hpp"
using Bolitho::Progress;
using Bolitho::ConsoleProgressDisplay;
using Bolitho::TaskSummaryDisplay;

using Bolitho::HashMap;

CriticalSection g_Lock;

//---------------------------------------------------------------------------------------------------
ImplementApplicationEntryPoint(SurfaceReconstruction);
//---------------------------------------------------------------------------------------------------
SurfaceReconstruction::SurfaceReconstruction()
{
}
//---------------------------------------------------------------------------------------------------
BOOL SurfaceReconstruction::OnInit()
{
  Configuration::Declare("Input", "", "The input file", FALSE);
  Configuration::Declare("Output", "", "The output file", FALSE);
  
  Configuration::Declare("Depth", 0, "The maximum depth of the reconstruction", TRUE);
  Configuration::Declare("SamplesPerNode", "1.0", "The number of samples per node", FALSE);
  Configuration::Declare("SampleRadius", "1", "The radius of a splatted sample", FALSE);
  Configuration::Declare("Steps", "7", "The reconstruction steps to perform", FALSE);
  Configuration::Declare("StreamingFilename", String::Format("Reconstructor.%d", Process::GetCurrentProcessId()), "The name of the steams on disk", FALSE);

  Configuration::Declare("MaxThreads", "1", "The maximum number of threads used to perform the reconstruction", FALSE);
  Configuration::Declare("Affinity", NULL, "The affinity mask for the thread pool threads", FALSE);

  Configuration::Declare("Progress", "2", "A flag that shows a UI based progress bar dialog", FALSE);
  Configuration::Declare("NoClipTree", "0", "A flag specifying that the octree should not be trimmed", FALSE);
  Configuration::Declare("NoTransform", "0", "Prevents the transformation of the model into the pointsets original co-ordinate system", FALSE);
  Configuration::Declare("NoNormalize", "0", "Prevents the scale normalization of the model during pre-processing", FALSE);
  Configuration::Declare("SampleRate", "1.0", "Sets the sampling rate on the input points during pre-processing", FALSE);
  Configuration::Declare("Validation", "0", "Enables extra validation tests", FALSE);
  Configuration::Declare("InterpolationAttributeName", "", "The vertex attribute in the input file to interpolte during the reconstruction process", FALSE);
  Configuration::Declare("Verbose", "0", "Increases the verbosity of the output", FALSE);

  Configuration::BindCommandLineValue("Input");
  Configuration::BindCommandLineValue("Output");

  m_pTraceLog = new MemoryStream();
  Trace.AddSink(TextStream(m_pTraceLog));

  if (!ParseConfiguration())
    return FALSE;

  if (!ConsoleApplication::OnInit())
    return FALSE;

  Configuration::TraceSettings();

  if ((!Configuration::IsPresent("Input") || !Configuration::IsPresent("Output")) && !Configuration::IsPresent("StreamingFilename"))
  {
    TraceError(this, "Either the 'Input' and 'Output' values must be present or /StreamingFilename must be present");
    return FALSE;
  }
  
  if (Configuration::GetValue<UINT>("MaxThreads") > CONFIG_MAXTHREADS)
  {
    TraceWarning(this, "MaxThreads (%u) exceeds CONFIG_MAXTHREADS (%u)", Configuration::GetValue<UINT>("MaxThreads"), CONFIG_MAXTHREADS);
    Configuration::SetValue<UINT>("MaxThreads", CONFIG_MAXTHREADS);
  }

  #if defined(PLATFORM_WINDOWS)
  if (Configuration::IsPresent("Profile"))
  {
    m_pProfiler = new ApplicationProfiler(Process::GetCurrentProcess(), File::Open(Configuration::GetValue<String>("Profile.Filename"), FileMode::CREATE, FileAccess::WRITE, FileShare::READ) );
    m_pProfiler->Start(Configuration::GetValue<DWORD>("Profile.Frequency"));
  }
  #endif

  if (Configuration::GetValue<DWORD>("ProgressDisplay") == 1)
    m_Progress.SetDisplay(new ConsoleProgressDisplay());
  else
    m_Progress.SetDisplay(new TaskSummaryDisplay());
 
  m_Input = Configuration::GetValue<String>("Input");
  m_Output =  Configuration::GetValue<String>("Output");
  m_Depth =  Configuration::GetValue<UINT>("Depth");
  m_SamplesPerNode = Configuration::GetValue<FLOAT>("SamplesPerNode");
  m_ClipTree = !Configuration::GetValue<BOOL>("NoClipTree");
  m_SampleRadius = Configuration::GetValue<FLOAT>("SampleRadius");
  m_NoTransform = Configuration::GetValue<BOOL>("NoTransform");
  m_NoNormalize = Configuration::GetValue<BOOL>("NoNormalize");
  m_StreamingFilename = Configuration::GetValue<String>("StreamingFilename");
  m_Steps = Configuration::GetValue<DWORD>("Steps");
  m_Validation = Configuration::GetValue<BOOL>("Validation");
  m_InterpolationAttributeName = Configuration::GetValue<String>("InterpolationAttributeName");
  m_SampleRate = Configuration::GetValue<FLOAT>("SampleRate");
  m_Verbose = Configuration::GetValue<UINT>("Verbose");

  return TRUE;
}
//---------------------------------------------------------------------------------------------------
VOID SurfaceReconstruction::OnExit()
{
  ConsoleApplication::OnExit();
}
//---------------------------------------------------------------------------------------------------
VOID SurfaceReconstruction::ValidateInput()
{
  Md5 Hash;

}
//---------------------------------------------------------------------------------------------------
VOID SurfaceReconstruction::BuildOctree()
{
  m_Progress.Begin("Building Octree");

  m_Progress.Begin("Initializing");
  StreamingOctreeConstructor Soc(m_pTree, m_SamplesPerNode, m_ClipTree, m_SampleRadius);
  Soc.InitConstructor();
  m_Progress.End();

  m_Progress.Begin("Constructing Tree", 0, m_pTree->GetPointStream().Count(), "Points");
  for(UINT Slice=0; Slice<m_pTree->Resolution(); Slice+=2, m_Progress.Update(m_pTree->GetPointStream().GetTailPosition()))
  {
    //Trace("Processing Slice %u", Slice);
    Soc.UpdateConstructor(Slice);
  }
  m_Progress.End();
  
  m_Progress.Begin("Finalizing");
  Soc.FinalizeConstructor();
  m_Progress.End();
  
  m_Progress.End();
}
//---------------------------------------------------------------------------------------------------
VOID SurfaceReconstruction::SolveLaplacian()
{
  m_Progress.Begin("Solving Laplacian");
  
  m_Progress.Begin("Initializing");
  LaplacianMatrixSolver Lms;
  Lms.Init(m_pTree);
  Lms.InitSolver();
  m_Progress.End();
  
  m_Progress.Begin("Solving", 0, m_pTree->GetNodeStream(m_PeakDepth, 0).Count(), "Nodes");
  for (UINT Slice=0; Slice<m_pTree->Resolution(); Slice+=2, m_Progress.Update(m_pTree->GetNodeStream(m_PeakDepth, 0).GetTailPosition()))
    Lms.UpdateSolver(m_pTree->CoarsestDepthChanged(Slice));
  m_Progress.End();
  
  m_Progress.Begin("Finalizing");
  Lms.FinalizeSolver();
  m_Progress.End();
  
  m_Progress.End();
}
//---------------------------------------------------------------------------------------------------
VOID SurfaceReconstruction::ExtractIsoSurface()
{
  m_Progress.Begin("Extracting Iso-Surface");

  m_Progress.Begin("Initializing");
  m_pVertices = File::Open(String::Format("%s.Vertices", m_StreamingFilename.ConstStr()), FileMode::CREATE, FileAccess::READWRITE, FileShare::NONE, FileFlags::DELETEONCLOSE);
  m_pTriangles = File::Open(String::Format("%s.Triangles", m_StreamingFilename.ConstStr()), FileMode::CREATE, FileAccess::READWRITE, FileShare::NONE, FileFlags::DELETEONCLOSE);

  //m_pVertices->SetBufferSize(8192*1024);
  //m_pTriangles->SetBufferSize(8192*1024);

  Matrix4<FLOAT> Transform;
  Transform.SetIdentity();
  
  BinaryStream XF = File::Open(String::Format("%s.Transform", Path::Basename(m_Input).ConstStr()), FileMode::OPEN, FileAccess::READ);
  XF >> Transform;
  XF.Close();
  
  if (m_NoTransform)
    Transform.SetIdentity();
  else
    Transform = Transform.Inverse();
  
  MarchingOctreeExtractor Moe;
  Moe.Init(m_pTree, m_pVertices, m_pTriangles, Transform);
  Moe.InitExtractor();
  m_Progress.End();
  
  m_Progress.Begin("Extracting", 0, m_pTree->GetNodeStream(m_PeakDepth, 0).Count(), "Nodes");
  for (UINT Slice=0; Slice<m_pTree->Resolution(); Slice++, m_Progress.Update(m_pTree->GetNodeStream(m_PeakDepth, 0).GetTailPosition()))
    Moe.UpdateExtractor(m_pTree->CoarsestDepthChanged(Slice));
  m_Progress.End();
  
  m_Progress.Begin("Finalizing");
  Moe.FinalizeExtractor();
  m_Progress.End();
  
  m_Progress.End();
  
}
//---------------------------------------------------------------------------------------------------
VOID SurfaceReconstruction::WritePly()
{
  UINT VertexCount = (UINT)(m_pVertices->Length() / (sizeof(ORIENTEDPOINT)));
  UINT TriangleCount = (UINT)(m_pTriangles->Length() / (sizeof(INT)*3));
  
  Ptr<File> Output = File::Open(m_Output, FileMode::CREATE, FileAccess::WRITE);
  
  TextStream TS(Output);

  /* Write out a ply header */
  TS.WriteLine("ply");
  TS.WriteLine("format binary_little_endian 1.0");

  TS.WriteLine("comment %s %s %s", m_ProductDescription.ConstStr(), m_ProductVersion.ToString().ConstStr(), m_ImageConfiguration.ConstStr());
  TS.WriteLine("comment %s", GetCommandLine().ConstStr());

  CONST Buffer<BYTE>& TraceInfo = m_pTraceLog->GetBuffer();
  String TraceString = String(TraceInfo);
  StringArray TraceStrings = TraceString.Split('\n');

  for (SIZE_T i=0; i<TraceStrings.Length(); i++)
    TS.WriteLine("comment %s", TraceStrings[i].ConstStr());

  TS.WriteLine("element vertex %d", VertexCount);
  TS.WriteLine("property float x");
  TS.WriteLine("property float y");
  TS.WriteLine("property float z");
  TS.WriteLine("element face %d", TriangleCount);
  TS.WriteLine("property list uchar int vertex_indices");
  TS.WriteLine("end_header");
  
  /* Write out Vertex data */
  m_pVertices->Seek(0);
  ORIENTEDPOINT Point;
  for (UINT i=0; i<VertexCount; i++)
  {
    m_pVertices->Read(&Point, sizeof(Point));
    Output->Write(&Point, sizeof(Vector3F));
  }

  /* Write out Triangle data.  The triangles need to be converted to ply format.  Sigh. */
  m_pTriangles->Seek(0);

  #pragma pack(push,1)
  struct PLYTRIANGLE
  {
    UCHAR VertexCount;
    INDEXEDTRIANGLE Vertices;
  };
  #pragma pack(pop)

  CONST UINT BlockSize = 65536;
  PLYTRIANGLE* pTriangles = new PLYTRIANGLE[BlockSize];
  for (SIZE_T i=0; i<BlockSize; i++)
    pTriangles[i].VertexCount = 3;

  for (UINT i=0; i<TriangleCount; i+=BlockSize)
  {
    UINT CurrentBlockSize = (TriangleCount-i) < BlockSize ? (TriangleCount-i) : BlockSize;
    for (UINT j=0; j<CurrentBlockSize; j++)
      m_pTriangles->Read(&pTriangles[j].Vertices.Vertex[0], sizeof(INDEXEDTRIANGLE)); 
    Output->Write(pTriangles, CurrentBlockSize*sizeof(PLYTRIANGLE));
  }

  delete[] pTriangles;
  Output->Close();
} 
//---------------------------------------------------------------------------------------------------
INT SurfaceReconstruction::Main()
{

  /* Generate PointSet / PointIndex, if needed */
  if (Path::Extension(m_Output).Compare("PointSet", FALSE) == 0)
  {
    if (Path::Extension(m_Input).Compare("Bnpts", FALSE) == 0)
      StreamingPointConverter::PreprocessPointset(m_Input, m_Output, m_Depth, m_NoTransform, m_NoNormalize, m_SampleRate);
    return 0;
  }
  
  if (Path::Extension(m_Input).Compare("Bnpts", FALSE) == 0)
  {
    String RealInput = Path::Basename(m_Input) + ".PointSet";
    StreamingPointConverter::PreprocessPointset(m_Input, RealInput, m_Depth, m_NoTransform, m_NoNormalize, m_SampleRate);
    m_Input = RealInput;
  }

  /* Construct the Streams */
  //if (File::Exists(m_StreamingFilename))
  //m_pTree = Tree::Open(m_StreamingFilename, !Configuration::IsPresent("StreamingFilename"));
  //else
  m_pTree = Tree::Create(m_Input, m_StreamingFilename, m_InterpolationAttributeName, m_Depth, !Configuration::IsPresent("StreamingFilename"));


  /* Collect statistics about input data */
  Trace1("Resolution: %u", m_pTree->Resolution());
  Trace1("Input Samples: %llu (%s)", m_pTree->GetPointStream().Count(), String::FormatSize(m_pTree->GetPointStream().Count()*24).ConstStr());
  Trace1("Maximum Slice: %llu (%s), Average Slice: %llu (%s)", m_pTree->GetMaximumPointSliceCount(m_pTree->MaximumDepth()), String::FormatSize(m_pTree->GetMaximumPointSliceCount(m_pTree->MaximumDepth())*24).ConstStr(), m_pTree->GetAveragePointSliceCount(m_pTree->MaximumDepth()), String::FormatSize(m_pTree->GetAveragePointSliceCount(m_pTree->MaximumDepth())*24).ConstStr());

  if (m_Validation)
  {
    Trace("Point Index Checksum: %s", m_pTree->ComputePointIndexHash().ToString().ConstStr());
    Trace("Point Samples Checksum: %s", m_pTree->ComputePointSetHash().ToString().ConstStr());
  }

  if (m_Steps & 0x1)
    BuildOctree();

  /* Record the sizes of the streams */
  #if defined(CONFIG_STOREDNEIGHBOURS)
  UINT NodeSize = (UINT)sizeof(PARTITION0) + (UINT)sizeof(PARTITION1) + (UINT)sizeof(PARTITION2) + (UINT)sizeof(PARTITION3) + (UINT)sizeof(PARTITION4) + (UINT)sizeof(PARTITION5) + (UINT)sizeof(PARTITION6);
  Trace1("Node Size: %u + %u + %u + %u + %u + %u + %u = %u", (UINT)sizeof(PARTITION0), (UINT)sizeof(PARTITION1), (UINT)sizeof(PARTITION2), (UINT)sizeof(PARTITION3), (UINT)sizeof(PARTITION4), (UINT)sizeof(PARTITION5), (UINT)sizeof(PARTITION6), NodeSize);
  #else
  UINT NodeSize = (UINT)sizeof(PARTITION0) + (UINT)sizeof(PARTITION1) + (UINT)sizeof(PARTITION2) + (UINT)sizeof(PARTITION3) + (UINT)sizeof(PARTITION4) + (UINT)sizeof(PARTITION5);
  Trace1("Node Size: %u + %u + %u + %u + %u + %u = %u", (UINT)sizeof(PARTITION0), (UINT)sizeof(PARTITION1), (UINT)sizeof(PARTITION2), (UINT)sizeof(PARTITION3), (UINT)sizeof(PARTITION4), (UINT)sizeof(PARTITION5), NodeSize);
  #endif
  ULONGLONG TotalNodeCount = 0;
  ULONGLONG PeakNodeCount = 0;
  for (USHORT i=0; i<m_pTree->Height(); i++)
  {
    ULONGLONG NodeCount = m_pTree->GetNodeCount(i);

    if (NodeCount > PeakNodeCount)
    {
      PeakNodeCount = NodeCount;
      m_PeakDepth = i;
    }

    TotalNodeCount += NodeCount;
    Trace1("Stream %d: %llu (%s), Maximum Slice: %llu (%s), Average Slice: %llu (%s)", i, NodeCount, String::FormatSize(NodeSize*NodeCount).ConstStr(), m_pTree->GetMaximumNodeSliceCount(i), String::FormatSize(m_pTree->GetMaximumNodeSliceCount(i) * NodeSize).ConstStr(), m_pTree->GetAverageNodeSliceCount(i), String::FormatSize(m_pTree->GetAverageNodeSliceCount(i) * NodeSize).ConstStr());

    //m_pTree->GetPartition0Stream(i).Resize(NodeCount);
    //m_pTree->GetTreeTopologyStream(i).Resize(NodeCount);
  }
  Trace1("Total Nodes: %llu (%s)", TotalNodeCount, String::FormatSize(NodeSize*TotalNodeCount).ConstStr());
  Trace1("Peak Depth: %d", m_PeakDepth);

  /* Solve the Laplacian */
  if (m_Steps & 0x2)
    SolveLaplacian();
  
  /* Extract Iso Surface */
  if (m_Steps & 0x4)
  {
    ExtractIsoSurface();

    Trace("Vertices: %d (%s)", m_pVertices->Length() / sizeof(ORIENTEDPOINT), String::FormatSize(m_pVertices->Length()).ConstStr());
    Trace("Triangles: %d (%s)", m_pTriangles->Length() / (sizeof(UINT)*3), String::FormatSize(m_pTriangles->Length()).ConstStr());

    if (m_Validation)
    {
      Trace("Vertex Data Checksum: %s", m_pVertices->ComputeMd5Hash().ToString().ConstStr()); 
      Trace("Triangle Data Checksum: %s", m_pTriangles->ComputeMd5Hash().ToString().ConstStr()); 
    }

    Process::PROCESSCOUNTERS SC = GetProcessCounters();

    Trace("Peak Working Set: %s", String::FormatSize(SC.PeakWorkingSetSize).ConstStr());
    Trace("User CPU Time: %s", String::FormatTimeInterval(SC.UserTime.TotalSeconds()).ConstStr());
    Trace("Kernel CPU Time: %s", String::FormatTimeInterval(SC.KernelTime.TotalSeconds()).ConstStr());
    Trace("Lost CPU Time: %s", String::FormatTimeInterval(SC.LostTime.TotalSeconds()).ConstStr());
    Trace("Total Wallclock Time: %s", String::FormatTimeInterval(SC.WallClockTime.TotalSeconds()).ConstStr());
    FLOAT TotalCPUTime = (SC.KernelTime + SC.UserTime).TotalSeconds();
    FLOAT TotalWallClockTime = SC.WallClockTime.TotalSeconds();
    Trace("CPU Efficiency: %f", TotalCPUTime / TotalWallClockTime);

    /* Write Output */
    if (Path::Extension(m_Output).Compare("Ply", FALSE) == 0)
      WritePly();
  }

  #if defined(CONFIG_PROFILE)
  if (Configuration::IsSet("FunctionProfile"))
    Bolitho::FunctionTimerManager::ReportTimings();
  #endif

  return 0;
} 
//---------------------------------------------------------------------------------------------------

