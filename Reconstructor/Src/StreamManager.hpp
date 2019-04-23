/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/StreamManager.hpp $
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

#pragma once

#include "DataStream.hpp"

#if defined(CONFIG_STOREDNEIGHBOURS)
#define CONFIG_STREAMCOUNT 7
#else
#define CONFIG_STREAMCOUNT 6
#endif

/* Partition 0 represents the topology of the octree */
struct PARTITION0
{
  ULONGLONG Children;
  USHORT Depth;
  USHORT Offset[3];
};

/* Partitions 1-5 represent the node data of the octree */
struct PARTITION1
{
  Vector3F Normal;
  #if defined(CONFIG_CUSTOMATTRIBUTE)
  InterpolatedAttribute CustomAttribute;
  #endif
};

struct PARTITION2
{
  DOUBLE WeightContribution;
  #if defined(CONFIG_CUSTOMATTRIBUTE)
  FLOAT CustomAttributeWeight;
  #endif
};

struct PARTITION3
{
  union 
  {
    INT MarchingCubeIndex;
    INT NodeIndex;
  };
  CHAR MCIndex[2];
};

struct PARTITION4
{
  FLOAT Value;
};

struct PARTITION5
{
  FLOAT Solution;
};

#if defined(CONFIG_STOREDNEIGHBOURS)
/* Partition 6 stores neighbor information */
struct PARTITION6
{
  STREAMOFFSET Neighbor[27];
};
#endif

struct ORIENTEDPOINT
{
  Vector3F Position;
  Vector3F Normal;
};

typedef Vector3F VERTEX;

/* Represents a node in the index tree.  
 * The Length and Offset correspond to the range of records within the topology and data streams that are active 
 * for a given sweep tree node
 */
struct SPATIALINDEX
{
  STREAMOFFSET Length;
  STREAMOFFSET Offset;
};

template <> 
struct Bolitho::Internal::IsPrimitive<SPATIALINDEX> : public TrueType{};

class PerDepthStreams
{
public:

  DataStream m_Streams[CONFIG_STREAMCOUNT];
};

class StreamManager
{
public:
  StreamManager();
  
  /* Creates a new set of streams */
  VOID Create(LPCTSTR InputFilename, LPCTSTR StreamingFilename, USHORT MaximumDepth, BOOL DeleteOnCompletion);

  /* Returns a reference to the data stream for a given depth and region */
  CONST DataStream& GetNodeStream(USHORT Depth, UINT Partition) CONST
  { return m_pDepthStreams[Depth].m_Streams[Partition]; }
  /* Returns a reference to the data stream for a given depth and region */
  DataStream& GetNodeStream(USHORT Depth, UINT Partition)
  { return m_pDepthStreams[Depth].m_Streams[Partition]; }

  /* Returns a reference to the point data stream */
  CONST DataStream& GetPointStream() CONST
  { return m_PointData; }
  /* Returns a reference to the point data stream */
  DataStream& GetPointStream()
  { return m_PointData; }

  /* Returns a pointer to the start of the node index */
  CONST SPATIALINDEX* GetNodeIndex() CONST
  { return m_pNodeIndex; }
  /* Returns a pointer to the start of the point index */
  CONST SPATIALINDEX* GetPointIndex() CONST
  { return m_pPointIndex; }
  /* Returns the total number of elements in the node and point indices */
  UINT GetIndexCount() CONST
  { return m_IndexCount; }

  /* Returns a pointer to the node index for a given depth, slice and region */
  SPATIALINDEX* GetNodeIndex(USHORT Depth, UINT Index, UINT Region);
  /* Returns a pointer to the point index for a given depth, slice and region */
  SPATIALINDEX* GetPointIndex(USHORT Depth, UINT Index, UINT Region);

  /* Sets the desired window on the point data */
  VOID SetPointStreamWindow(STREAMOFFSET Begin, STREAMOFFSET End);
  /* Sets the desired window on the node data */
  VOID SetNodeStreamWindow(USHORT Depth, STREAMOFFSET Begin, STREAMOFFSET End);

  /* Prepares the point data for streaming */
  VOID InitializePointStream(DWORD DataUsage);
  /* Prepares the node data for streaming */
  VOID InitializeNodeStream(USHORT Depth, DWORD DataUsage);

  /* Finalizes the point data after streaming */
  VOID FinalizePointStream();
  /* Finalizes the node data after streaming */
  VOID FinalizeNodeStream(USHORT Depth);

  /* Advances the point data stream */
  STREAMOFFSET AdvancePointStreamWindow(STREAMOFFSET Delta);
  /* Advances the node data stream */
  STREAMOFFSET AdvanceNodeStreamWindow(USHORT Depth, STREAMOFFSET Delta);

private:
  /* The number of entries in the node and point indices */
  UINT m_IndexCount;
  /* The node index */
  SPATIALINDEX* m_pNodeIndex;
  /* The point index */
  SPATIALINDEX* m_pPointIndex;

  DataStream m_PointData;
  PerDepthStreams* m_pDepthStreams;  
};

