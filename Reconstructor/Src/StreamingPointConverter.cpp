/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/StreamingPointConverter.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 908 $
 * Last Updated: $Date: 2008-12-29 12:19:41 -0800 (Mon, 29 Dec 2008) $
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
#include "Tree.hpp"

#include "StreamingPointConverter.hpp"

#include "Math/Vector3.hpp"
#include "Math/Matrix3.hpp"
#include "Math/Matrix4.hpp"
#include "Math/Math.hpp"

using Bolitho::Traits;
using Bolitho::RealTraits;
using Bolitho::Array;
using Bolitho::IO::BinaryStream;

using Bolitho::Math::Log2;
using Bolitho::Math::Matrix3;
using Bolitho::Math::Matrix4;
using Bolitho::Math::Vector3;
using Bolitho::Math::UniformRandom;

#include "Utility/Bits.hpp"
using Bolitho::Interleave;

UINT Resolution;
//---------------------------------------------------------------------------------------------------
UINT GetPointZIndex(CONST Vector3F& Position)
{
  UINT SliceResolution = CONFIG_REGIONRES;
  UINT Ai = (UINT)(Position.X() * Resolution);
  UINT Aj = (UINT)(Position.Y() * SliceResolution);
  UINT Ak = (UINT)(Position.Z() * SliceResolution);
  UINT Ajk = Interleave(Aj, Ak);
  
  //Trace("%f %f  %u %u %u", Position.Y(), Position.Z(), Aj, Ak, Ajk);
  UINT AA = Ajk | (Ai << 2*CONFIG_REGIONDEPTH);
  

  return AA;
}
//---------------------------------------------------------------------------------------------------
template<>
BOOL Traits<ORIENTEDPOINT>::CompareLessThan(CONST ORIENTEDPOINT& A, CONST ORIENTEDPOINT& B)
{
  UINT AA = GetPointZIndex(A.Position);
  UINT BB = GetPointZIndex(B.Position);

  if (AA == BB)
    return A.Position.X() < B.Position.X();
  else
    return AA < BB;
}
//---------------------------------------------------------------------------------------------------
VOID StreamingPointConverter::PreprocessMesh(LPCTSTR MeshFilename, LPCTSTR PoinsetFilename, USHORT MaximumDepth, BOOL NoRotate, BOOL NoNormalize)
{
  
}
//---------------------------------------------------------------------------------------------------
VOID StreamingPointConverter::PreprocessPointset(LPCTSTR BNPTSFilename, LPCTSTR PointsetFilename, USHORT MaximumDepth, BOOL NoRotate, BOOL NoNormalize, FLOAT SampleRate)
{
  DataStream Bnpts;
  Bnpts.SetBackingStore(BNPTSFilename, sizeof(ORIENTEDPOINT));
  
  Matrix3<FLOAT> Covariance;
  Matrix4<FLOAT> Transform;
  Matrix4<FLOAT> NormalTransform;
  Matrix3<FLOAT> R,D;
  Vector3<FLOAT> MinPosition(RealTraits<FLOAT>::MaxValue,  RealTraits<FLOAT>::MaxValue,  RealTraits<FLOAT>::MaxValue);
  Vector3<FLOAT> MaxPosition(-RealTraits<FLOAT>::MaxValue, -RealTraits<FLOAT>::MaxValue, -RealTraits<FLOAT>::MaxValue);

  Covariance.SetZero();
  Transform.SetIdentity();
  
  if (!NoRotate)
  {
    Vector3<FLOAT> Centroid;
    
    Trace("Computing Centroid");
    
    Bnpts.Initialize(DataAccess::READ);
    for (SIZE_T i=0; i<Bnpts.Count(); i++)
    {
      Bnpts.SetHeadPosition(i+1);
      Bnpts.SetTailPosition(i);
      
      ORIENTEDPOINT& V = Bnpts.Get<ORIENTEDPOINT>(i);
      Centroid += V.Position;
    }
    Bnpts.Finalize();
    
    Centroid /= (FLOAT)Bnpts.Count();
    Trace(Centroid.ToString());

    Trace("Computing Covariance");
    
    Bnpts.Initialize(DataAccess::READ);
    for (SIZE_T i=0; i<Bnpts.Count(); i++)
    {
      Bnpts.SetHeadPosition(i+1);
      Bnpts.SetTailPosition(i);
      
      ORIENTEDPOINT V = Bnpts.Get<ORIENTEDPOINT>(i);
      V.Position -= Centroid;
      
      Covariance(0,0) += V.Position(0)*V.Position(0);
      Covariance(1,0) += V.Position(1)*V.Position(0);
      Covariance(2,0) += V.Position(2)*V.Position(0);
      
      Covariance(0,1) += V.Position(0)*V.Position(1);
      Covariance(1,1) += V.Position(1)*V.Position(1);
      Covariance(2,1) += V.Position(2)*V.Position(1);
      
      Covariance(0,2) += V.Position(0)*V.Position(2);
      Covariance(1,2) += V.Position(1)*V.Position(2);
      Covariance(2,2) += V.Position(2)*V.Position(2);
    }
    Bnpts.Finalize();
      
    Covariance.Normalize();
    Trace(Covariance.ToString());
    Covariance.EigenDecomposition(R, D);
    Transform.SetRotation(R);
  }
  
  FLOAT Scale = 1.0f;
  
  if (!NoNormalize)
  {
    Trace("Computing Bounding Box");
    
    Bnpts.Initialize(DataAccess::READ);
    for (SIZE_T i=0; i<Bnpts.Count(); i++)
    {
      Bnpts.SetHeadPosition(i+1);
      Bnpts.SetTailPosition(i);
      
      ORIENTEDPOINT V = Bnpts.Get<ORIENTEDPOINT>(i);
      
      V.Position = Transform * V.Position;
      
      for (SIZE_T j=0; j<3; j++)
      {
        if (V.Position(j) < MinPosition(j))
          MinPosition(j) = V.Position(j);
        if (V.Position(j) > MaxPosition(j))
          MaxPosition(j) = V.Position(j);
      }
    }
    Bnpts.Finalize();

    Trace(MinPosition.ToString());
    Trace(MaxPosition.ToString());

    Scale = MaxPosition.X() - MinPosition.X();
    if ((MaxPosition.Y() - MinPosition.Y()) > Scale)
      Scale = MaxPosition.Y() - MinPosition.Y();
    if ((MaxPosition.Z() - MinPosition.Z()) > Scale)
      Scale = MaxPosition.Z() - MinPosition.Z();
    
    Scale /= .8f;
  }
  
  Trace("Creating Buckets");
  
  UINT BucketDepth = Log2<UINT>((UINT)Bnpts.Length() / (512*1048576)) + 3;
  
  Trace("Bucket Count: %d", 1 << BucketDepth);

  Array<UINT> BucketSize(1 << BucketDepth);
  Array< Ptr<File> > Bucket(1 << BucketDepth);
  for (SIZE_T i=0; i<Bucket.Length(); i++)
  {
    Bucket[i] = File::OpenTemporary();
    Bucket[i]->SetBufferSize(65536);
  }
  
  NormalTransform.SetIdentity();
  Transform.SetIdentity();
  
  if (!NoNormalize)
  {
    Transform *= Matrix4<FLOAT>::Translation(0.1f, 0.1f, 0.1f);
    Transform *= Matrix4<FLOAT>::Scale((1.0f/Scale), (1.0f/Scale), (1.0f/Scale)); 
    Transform *= Matrix4<FLOAT>::Translation(-MinPosition);

    if (!NoRotate)
    {
      Transform *= Matrix4<FLOAT>(R);
      NormalTransform = Matrix4<FLOAT>(R);
    }
  }

  BinaryStream XF = File::Open(String::Format("%s.Transform", Path::Basename(PointsetFilename).ConstStr()), FileMode::CREATE, FileAccess::WRITE);
  XF << Transform;
  XF.Close();

  Resolution = 1u << MaximumDepth;
  
  UINT SelectedPoints = 0;
  Bnpts.Initialize(DataAccess::READ);
  for (SIZE_T i=0; i<Bnpts.Count(); i++)
  {
    Bnpts.SetHeadPosition(i+1);
    Bnpts.SetTailPosition(i);
    
    if (UniformRandom<FLOAT>() > SampleRate)
      continue;
    
    ORIENTEDPOINT V = Bnpts.Get<ORIENTEDPOINT>(i);

    V.Position = Transform * V.Position;
    V.Normal = NormalTransform * V.Normal;
    V.Normal.Normalize();

    SIZE_T j = (SIZE_T)(V.Position.X() * Bucket.Length());
    Bucket[j]->Write(&V, sizeof(ORIENTEDPOINT));
    BucketSize[j]++;
    SelectedPoints++;
  }
  Bnpts.Finalize();

  Trace("Selected %u points from %u input points", SelectedPoints, (UINT)Bnpts.Count());

  Trace("Sorting Buckets");

  Array<SPATIALINDEX> PointIndex( ((1<<(MaximumDepth+1))-1) * CONFIG_REGIONCOUNT );
  Ptr<File> Output = File::Open(PointsetFilename, FileMode::CREATE, FileAccess::WRITE);

  Array<ORIENTEDPOINT> Points;
  for (SIZE_T i=0; i<Bucket.Length(); i++)
  {
    if (BucketSize[i] == 0)
      continue;

    Points.Resize(0);
    Points.Resize(BucketSize[i]);
    Trace("Bucket[%d]  Size: %d", i, BucketSize[i]);

    Bucket[i]->Seek(0);
    Bucket[i]->Read(&Points[0], BucketSize[i]*sizeof(ORIENTEDPOINT));

    Points.Sort();

    Output->Write(&Points[0], BucketSize[i]*sizeof(ORIENTEDPOINT));

    for (SIZE_T j=0; j<Points.Length(); j++)
    {
      UINT ZIndex = GetPointZIndex(Points[j].Position);
      UINT X = ZIndex >> (2*CONFIG_REGIONDEPTH);
      UINT YZ = ZIndex & ((1 << (2*CONFIG_REGIONDEPTH)) - 1);
      if (CONFIG_REGIONCOUNT == 0)
          YZ = 0;
      
      //Trace("(%f,%f,%f) %x  %x %x", Points[j].Position[0], Points[j].Position[1], Points[j].Position[2], ZIndex, X, YZ);
      for (SIZE_T k=0; k<=MaximumDepth; k++)
      {
        UINT Index = (X >> (MaximumDepth-k));
        UINT BaseIndex = ((1<<k)-1);
        if (k < CONFIG_REGIONDEPTH)
          PointIndex[(BaseIndex + Index) * CONFIG_REGIONCOUNT].Length++;
        else
          PointIndex[(BaseIndex + Index) * CONFIG_REGIONCOUNT + YZ].Length++;
      }
    }
  }

  for (USHORT i=0; i<=MaximumDepth; i++)
  {
    ULONGLONG Offset = 0;
    for (UINT j=0; j < (1u<<i)*CONFIG_REGIONCOUNT; j++)
    {
      UINT BaseIndex = ((1<<i)-1) * CONFIG_REGIONCOUNT;
      PointIndex[BaseIndex + j].Offset = Offset;
      Offset += PointIndex[BaseIndex + j].Length;
    }
  }

  /*
  Bnpts.Initialize(DataAccess::READWRITE);
  for (UINT i=0; i<1u << MaximumDepth; i++)
  {
    UINT BaseOffset = 1u << MaximumDepth - 1;
    Bnpts.SetTailPosition(PointIndex[BaseOffset + i].Offset);
    Bnpts.SetHeadPosition(PointIndex[BaseOffset + i].Offset + PointIndex[BaseOffset + i].Length);
  }
  Bntps.Finalize();
  */

  File::WriteStruct(String::Format("%s.PointIndex", Path::Basename(PointsetFilename).ConstStr()), 0, &PointIndex[0], PointIndex.Length()); 
}
//---------------------------------------------------------------------------------------------------


