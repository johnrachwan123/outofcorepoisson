/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/BlockMeshAlgorithms.cpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1588 $
 * Last Updated: $Date: 2008-08-23 09:36:02 -0700 (Sat, 23 Aug 2008) $
 * 
 * 
 * Copyright (c) 2004-2007, Matthew G Bolitho
 * All rights reserved.
 *
 *
 * Microsoft Reference License (Ms-RL)
 *   
 * This license governs use of the accompanying software. If you use the software, you accept 
 * this license. If you do not accept the license, do not use the software.
 *
 * 1. Definitions
 *  - The terms "reproduce," "reproduction" and "distribution" have the same meaning here as under 
 *    U.S. copyright law.
 *  - "You" means the licensee of the software.
 *  - "Your company" means the company you worked for when you downloaded the software.
 *  - "Reference use" means use of the software within your company as a reference, in read only 
 *    form, for the sole purposes of debugging your products, maintaining your products, or 
 *    enhancing the interoperability of your products with the software, and specifically excludes 
 *    the right to distribute the software outside of your company.
 *  - "Licensed patents" means any Licensor patent claims which read directly on the software as 
 *    distributed by the Licensor under this license.
 * 
 * 2. Grant of Rights
 *  (A) Copyright Grant- Subject to the terms of this license, the Licensor grants you a non-transferable, 
 *      non-exclusive, worldwide, royalty-free copyright license to reproduce the software for reference use.
 *  (B) Patent Grant- Subject to the terms of this license, the Licensor grants you a non-transferable,
 *      non-exclusive, worldwide, royalty-free patent license under licensed patents for reference use.
 * 
 * 3. Limitations
 *  (A) No Trademark License - This license does not grant you any rights to use the Licensor's name
 *      logo, or trademarks.
 *  (B) If you begin patent litigation against the Licensor over patents that you think may apply 
 *      to the software (including a cross-claim or counterclaim in a lawsuit), your license to the 
 *      software ends automatically.
 *  (C) The software is licensed "as-is." You bear the risk of using it. The Licensor gives no express 
 *      warranties, guarantees or conditions. You may have additional consumer rights under your local 
 *      laws which this license cannot change. To the extent permitted under your local laws, the 
 *      Licensor excludes the implied warranties of merchantability, fitness for a particular purpose 
 *      and non-infringement.
 *
 ***************************************************************************************************/

#include "CodeLibrary.hpp"

#include "Collections/Stack.hpp"
#include "Graphics/BlockMesh.hpp"
#include "IO/Path.hpp"
#include "Utility/Flags.hpp"
#include "Utility/Sort.hpp"
#include "Utility/Bits.hpp"
#include "Debug/Trace.hpp"

using namespace Bolitho;
using namespace Bolitho::Math;
using namespace Bolitho::Math::Geometry;
using namespace Bolitho::Graphics;
using namespace Bolitho::IO;

//----------------------------------------------------------------------------------------------------------------------------------------------------
class SortIndexComparator
{
public:
  SortIndexComparator(UINT Offset)
  { m_Offset = Offset; }

  BOOL CompareLessThan(LPVOID pA, LPVOID pB)
  {
    FLOAT* pAA = (FLOAT*)(LPBYTE)pA + m_Offset;
    FLOAT* pBB = (FLOAT*)(LPBYTE)pB + m_Offset;
    return *pAA <*pBB;
  }

private:
  UINT m_Offset;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::ConstructAxialSortIndex(UINT Resolution)
{
#if 0
  if (m_pHeader->IndexType != IndexType::AXIALINDEX || m_pHeader->IndexResolution < Resolution)
  {
    SetVertexIndices();
    SetVertexZCurveIndex(Resolution);

    AxisAlignedBox3F BoundingBox = GetBoundingBox();

    /* Place points into buckets */
    AxialIndexPartitionFunction PartitionFunction(2, Resolution, VertexData.GetElementSize());
    IterateVertices(PartitionFunction, VertexAttribute::ZINDEX);

    /* Sort the buckets and reload the table data */
    Progress::Begin("Sorting Vertex Data Partitions");
    ZIndexComparator C(VertexData.GetAttributeOffset(VertexAttribute::ZINDEX));
    UINT VertexSize = VertexData.GetElementSize();
    UINT Offset = 0;
    Progress::Initialize(0, VertexData.GetCount(), "Vertices");
    for (SIZE_T i=0; i<PartitionFunction.Buckets.Length(); i++)
    {
      UINT Length = (UINT)PartitionFunction.Buckets[i]->Length();
      UINT Count = Length / VertexSize;

      LPVOID pBlock = AllocateMemory(Length);
      PartitionFunction.Buckets[i]->Seek(0);
      PartitionFunction.Buckets[i]->ReadToMemory(pBlock, Length);

      Sort(pBlock, Count, VertexSize, C);
      VertexData.Load(pBlock, Offset, Count);

      Offset += Count;
      FreeMemory(pBlock);

      Progress::Update(Offset);
    }
    Progress::End();

    Progress::Begin("Remapping Triangle Indices");
    /* Remap the triangle indices */
    UINT* IndexMapping = new UINT[VertexData.GetCount()];
    ZeroMemory(IndexMapping, VertexData.GetCount()*sizeof(UINT));

    CreateVertexIndexMappingFunction  CreateMappingFunction(IndexMapping);
    IterateVertices(CreateMappingFunction, VertexAttribute::INDEX);

    RemapVertexIndexFunction RemappingFunction(IndexMapping);
    IterateTriangles(RemappingFunction, TriangleAttribute::VERTICES);

    delete[] IndexMapping;
    Progress::End();

    /* Re-order the triangle data */
    SortTriangleData();

    VertexData.RemoveAttribute(VertexAttribute::INDEX);

    m_pHeader->IndexType = IndexType::ZINDEX;
    m_pHeader->IndexResolution = Resolution;
  }
#endif
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
class CreateVertexIndexMappingFunction
{
public:
  CreateVertexIndexMappingFunction(UINT* pMapping)
  { m_pMapping = pMapping; }

  DWORD GetTriangleAccess()
  { return PageAccess::NOACCESS; }
  DWORD GetVertexAccess()
  { return PageAccess::READ; }
  LPCTSTR GetTaskName()
  { return "Computing Vertex Index Remapping"; }

  typedef UINT ELEMENTTYPE;

  VOID ProcessVertex(UINT i, UINT& Index, LPVOID pVertexData)
  {
    m_pMapping[Index] = i;
  }

  UINT* m_pMapping;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
class RemapVertexIndexFunction
{
public:
  RemapVertexIndexFunction(UINT* pMapping)
  { m_pMapping = pMapping; }

  DWORD GetTriangleAccess()
  { return PageAccess::READWRITE; }
  DWORD GetVertexAccess()
  { return PageAccess::NOACCESS; }
  LPCTSTR GetTaskName()
  { return "Remapping Vertex Indices"; }

  typedef INDEXEDTRIANGLE ELEMENTTYPE;

  VOID ProcessTriangle(UINT i, INDEXEDTRIANGLE& T, LPVOID pTriangleData)
  {
    T.Vertex[0] = m_pMapping[T.Vertex[0]];
    T.Vertex[1] = m_pMapping[T.Vertex[1]];
    T.Vertex[2] = m_pMapping[T.Vertex[2]];
  }

  UINT* m_pMapping;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
class BuildOctreeFunction
{
public:
  BuildOctreeFunction(UINT OctreeResolution, UINT ZIndexResolution);

  DWORD GetTriangleAccess()
  { return PageAccess::NOACCESS; }
  DWORD GetVertexAccess()
  { return PageAccess::READ; }
  LPCTSTR GetTaskName()
  { return "Building Octree"; }

  typedef UINT ELEMENTTYPE0;
  typedef Vector3F ELEMENTTYPE1;

  VOID ProcessVertex(UINT i, UINT Index, Vector3F& Position, LPVOID pVertexData);
  VOID FinalizeVertexProcessing();
  VOID AggregateTree(BlockMesh::INDEXNODE* pNode);

  BlockMesh::INDEXNODE* m_pRoot;
private:
  VOID InsertNode(UINT ZIndex, UINT Offset, UINT Count, AxisAlignedBox3F BoundingBox);

  AxisAlignedBox3F m_BoundingBox;
  UINT m_Mask;
  UINT m_Shift;
  UINT m_Resolution;
  UINT m_LastZIndex;
  UINT m_Offset;
  UINT m_Length;
  UINT m_MaxZIndex;
  UINT m_ReduceShift;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
BuildOctreeFunction::BuildOctreeFunction(UINT OctreeResolution, UINT ZIndexResolution)
{
  m_Offset = 0;
  m_Length = 0;
  m_LastZIndex = 0;
  m_Resolution = OctreeResolution;
  m_BoundingBox = AxisAlignedBox3F::Empty;
  m_pRoot = new BlockMesh::INDEXNODE();

  m_ReduceShift = (Log2(ZIndexResolution) - Log2(OctreeResolution)) * 3;
  m_MaxZIndex = m_Resolution * m_Resolution * m_Resolution;
  m_Mask = (m_MaxZIndex >> 1) | (m_MaxZIndex >> 2) | (m_MaxZIndex >> 3);
  m_Shift = Log2(m_MaxZIndex) - 3;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BuildOctreeFunction::ProcessVertex(UINT i, UINT Index, Vector3F& Position, LPVOID pVertexData)
{
  Index >>= m_ReduceShift;
  Index &= m_MaxZIndex - 1;


  if (Index != m_LastZIndex)
  {
    InsertNode(m_LastZIndex, m_Offset, m_Length, m_BoundingBox);
    m_Offset += m_Length;
    m_Length = 0;
    m_BoundingBox = AxisAlignedBox3F::Empty;
    m_LastZIndex = Index;
  }

  m_BoundingBox.Combine(Position);
  m_Length++;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BuildOctreeFunction::FinalizeVertexProcessing()
{
  InsertNode(m_LastZIndex, m_Offset, m_Length, m_BoundingBox);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BuildOctreeFunction::InsertNode(UINT ZIndex, UINT Offset, UINT Length, AxisAlignedBox3F BoundingBox)
{
  if (Length == 0)
    return;

  UINT Mask = m_Mask;
  UINT Shift = m_Shift;

  BlockMesh::INDEXNODE* pNode = m_pRoot;
  while (Mask != 0)
  {
    UINT i = (ZIndex & Mask) >> Shift;

    if (pNode->pChildren == 0)
    {
      pNode->pChildren = new BlockMesh::INDEXNODE[8];
      Vector3F Center = pNode->VolumeBounds.Center();

      for (UINT i=0; i<8;i++)
      {
        pNode->pChildren[i].VolumeBounds = pNode->VolumeBounds;

        if (i & 1)
          pNode->pChildren[i].VolumeBounds.Min(0) = Center(0);
        else
          pNode->pChildren[i].VolumeBounds.Max(0) = Center(0);

        if (i & 2)
          pNode->pChildren[i].VolumeBounds.Min(1) = Center(1);
        else
          pNode->pChildren[i].VolumeBounds.Max(1) = Center(1);

        if (i & 4)
          pNode->pChildren[i].VolumeBounds.Min(2) = Center(2);
        else
          pNode->pChildren[i].VolumeBounds.Max(2) = Center(2);
      }
    }
    pNode = &pNode->pChildren[i];

    Shift -= 3;
    Mask = Mask >> 3;
  }

  pNode->VertexDataCount = Length;
  pNode->VertexDataOffset = Offset;
  pNode->VertexDataBounds = BoundingBox;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BuildOctreeFunction::AggregateTree(BlockMesh::INDEXNODE* pNode)
{
  STATIC UINT Offset = 0;

  if (pNode->pChildren)
  {
    for (UINT i=0; i<8; i++)
      AggregateTree(&pNode->pChildren[i]);
  
    pNode->VertexDataOffset = pNode->pChildren[0].VertexDataOffset;
    pNode->TriangleDataOffset = pNode->pChildren[0].TriangleDataOffset;
    pNode->VertexDataBounds = AxisAlignedBox3F::Empty;
    pNode->TriangleDataBounds = AxisAlignedBox3F::Empty;
    
    for (UINT i=0; i<8; i++)
    {
      pNode->VertexDataCount += pNode->pChildren[i].VertexDataCount;
      pNode->TriangleDataCount += pNode->pChildren[i].TriangleDataCount;
      if (pNode->pChildren[i].VertexDataCount > 0)
        pNode->VertexDataBounds.Combine(pNode->pChildren[i].VertexDataBounds);
      if (pNode->pChildren[i].TriangleDataCount > 0)
        pNode->TriangleDataBounds.Combine(pNode->pChildren[i].TriangleDataBounds);
    }
  }
  else
  {
    if (pNode->VertexDataOffset == 0)
      pNode->VertexDataOffset = Offset;
    else
      Offset = pNode->VertexDataOffset + pNode->VertexDataCount;

  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
class BuildTriangleBoundingVolumeFunction
{
public:
  BuildTriangleBoundingVolumeFunction(BlockMesh::INDEXNODE* pRoot) 
  {
    m_pRoot = pRoot;
    m_pNode = pRoot;
    while (m_pNode->pChildren)
    {
      m_Parent.Push(m_pNode);
      m_pNode = m_pNode->pChildren;
    }

    while (m_pNode->VertexDataCount == 0)
      m_pNode = GetNextLeafNode();
    m_pNode->TriangleDataBounds = AxisAlignedBox3F::Empty;
  }

  DWORD GetTriangleAccess()
  { return PageAccess::READ; }
  DWORD GetVertexAccess()
  { return PageAccess::READ; }
  LPCTSTR GetTaskName()
  { return "Building Bounding Volume Heirarchy"; }

  typedef Vector3F VERTEXELEMENTTYPE;

  VOID ProcessTriangle(UINT i, INDEXEDTRIANGLE& T, Vector3F& V0, Vector3F& V1, Vector3F& V2);

private:
  BlockMesh::INDEXNODE* m_pNode;
  BlockMesh::INDEXNODE* GetNextLeafNode();
  Stack<BlockMesh::INDEXNODE*> m_Parent;
  BlockMesh::INDEXNODE* m_pRoot;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BuildTriangleBoundingVolumeFunction::ProcessTriangle(UINT i, INDEXEDTRIANGLE& T, Vector3F& V0, Vector3F& V1, Vector3F& V2)
{
  UINT MinVertex = Min(T.Vertex[0], T.Vertex[1], T.Vertex[2]);

  if (MinVertex >= (m_pNode->VertexDataOffset + m_pNode->VertexDataCount))
  {
    do
    {
      m_pNode = GetNextLeafNode();
      if (m_pNode == NULL)
        break;
      m_pNode->TriangleDataOffset = i;
      m_pNode->TriangleDataBounds = AxisAlignedBox3F::Empty;
    }
    while (m_pNode->VertexDataCount == 0);
  }

  m_pNode->TriangleDataBounds.Combine(V0);
  m_pNode->TriangleDataBounds.Combine(V1);
  m_pNode->TriangleDataBounds.Combine(V2);
  m_pNode->TriangleDataCount++;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BlockMesh::INDEXNODE* BuildTriangleBoundingVolumeFunction::GetNextLeafNode()
{
  BlockMesh::INDEXNODE* pNode = m_pNode;

  while(TRUE)
  {
    if (m_Parent.IsEmpty())
      return NULL;
    BlockMesh::INDEXNODE* pParent = m_Parent.Peek();
    PTRDIFF_T i = pNode - pParent->pChildren;
    if (i < 7)
    {
      pNode = &pParent->pChildren[i+1];
      break;
    }
    pNode = m_Parent.Pop();
  }

  while (pNode->pChildren)
  {
    m_Parent.Push(pNode);
    pNode = pNode->pChildren;
  }
  
  return pNode;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
class ZIndexComparator
{
public:
  ZIndexComparator(UINT Offset)
  { m_Offset = Offset; }

  BOOL CompareLessThan(LPVOID pA, LPVOID pB)
  {
    UINT A = *(UINT*)((LPBYTE)pA + m_Offset);
    UINT B = *(UINT*)((LPBYTE)pB + m_Offset);
    
    return A < B;
  }

private:
  UINT m_Offset;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
class ZIndexPartitionFunction
{
public:
  ZIndexPartitionFunction(UINT PartitionResolution, UINT ZIndexResolution, UINT VertexSize)
  { 
    m_Resolution = PartitionResolution; 
    m_VertexSize = VertexSize;
    m_Divide = ZIndexResolution / PartitionResolution;
    m_Divide = m_Divide * m_Divide * m_Divide;

    Buckets.Resize(m_Resolution*m_Resolution*m_Resolution);
    for (SIZE_T i=0; i<Buckets.Length(); i++)
      Buckets[i] = File::OpenTemporary();
  }

  DWORD GetTriangleAccess()
  { return PageAccess::NOACCESS; }
  DWORD GetVertexAccess()
  { return PageAccess::READ; }
  LPCTSTR GetTaskName()
  { return "Partitioning Vertex Data"; }

  typedef UINT ELEMENTTYPE;

  VOID ProcessVertex(UINT i, UINT Index, LPVOID pVertexData)
  {
    Buckets[Index / m_Divide]->Write(pVertexData, m_VertexSize);
  }

  Array<Ptr<File> > Buckets;

private:
  UINT m_Divide;
  UINT m_VertexSize;
  UINT m_Resolution;
  
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::SortZCurve(UINT Resolution)
{
  if (m_pHeader->IndexType != IndexType::ZINDEX || m_pHeader->IndexResolution < Resolution)
  {
    SetVertexIndices();
    SetVertexZCurveIndex(Resolution);

    AxisAlignedBox3F BoundingBox = GetBoundingBox();

    /* Place points into buckets */
    ZIndexPartitionFunction PartitionFunction(2, Resolution, VertexData.GetElementSize());
    IterateVertices(PartitionFunction, VertexAttribute::ZINDEX);

    /* Sort the buckets and reload the table data */
    Progress::Begin("Sorting Vertex Data Partitions");
    ZIndexComparator C(VertexData.GetAttributeOffset(VertexAttribute::ZINDEX));
    UINT VertexSize = VertexData.GetElementSize();
    UINT Offset = 0;
    Progress::Initialize(0, VertexData.GetCount(), "Vertices");
    for (SIZE_T i=0; i<PartitionFunction.Buckets.Length(); i++)
    {
      UINT Length = (UINT)PartitionFunction.Buckets[i]->Length();
      UINT Count = Length / VertexSize;

      LPVOID pBlock = AllocateMemory(Length);
      PartitionFunction.Buckets[i]->Seek(0);
      PartitionFunction.Buckets[i]->ReadToMemory(pBlock, Length);

      Sort(pBlock, Count, VertexSize, C);
      VertexData.Load(pBlock, Offset, Count);

      Offset += Count;
      FreeMemory(pBlock);

      Progress::Update(Offset);
    }
    Progress::End();

    Progress::Begin("Remapping Triangle Indices");
    /* Remap the triangle indices */
    UINT* IndexMapping = new UINT[VertexData.GetCount()];
    ZeroMemory(IndexMapping, VertexData.GetCount()*sizeof(UINT));

    CreateVertexIndexMappingFunction  CreateMappingFunction(IndexMapping);
    IterateVertices(CreateMappingFunction, VertexAttribute::INDEX);

    RemapVertexIndexFunction RemappingFunction(IndexMapping);
    IterateTriangles(RemappingFunction, TriangleAttribute::VERTICES);

    delete[] IndexMapping;
    Progress::End();

    /* Re-order the triangle data */
    SortTriangleData();

    VertexData.RemoveAttribute(VertexAttribute::INDEX);

    m_pHeader->IndexType = IndexType::ZINDEX;
    m_pHeader->IndexResolution = Resolution;
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
class SliceZIndexPartitionFunction
{
public:
  SliceZIndexPartitionFunction(UINT PartitionResolution, UINT ZIndexResolution, UINT VertexSize)
  { 
    m_Resolution = PartitionResolution; 
    m_VertexSize = VertexSize;
    m_Divide = Log2(ZIndexResolution) - Log2(PartitionResolution);

    Buckets.Resize(m_Resolution);
    for (SIZE_T i=0; i<Buckets.Length(); i++)
      Buckets[i] = File::OpenTemporary();
  }

  DWORD GetTriangleAccess()
  { return PageAccess::NOACCESS; }
  DWORD GetVertexAccess()
  { return PageAccess::READ; }
  LPCTSTR GetTaskName()
  { return "Partitioning Vertex Data"; }

  typedef ULONGLONG ELEMENTTYPE;

  VOID ProcessVertex(UINT i, ULONGLONG Index, LPVOID pVertexData)
  {
    Buckets[Index >> (32 + m_Divide)]->Write(pVertexData, m_VertexSize);
  }

  Array<Ptr<File> > Buckets;

private:
  UINT m_Divide;
  UINT m_VertexSize;
  UINT m_Resolution;
  
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::SortSliceZCurve(UINT Resolution)
{
  if (m_pHeader->IndexType != IndexType::SLICEZINDEX || m_pHeader->IndexResolution < Resolution)
  {
    SetVertexIndices();
    SetVertexSliceZCurveIndex(Resolution);

    AxisAlignedBox3F BoundingBox = GetBoundingBox();

    /* Place points into buckets */
    SliceZIndexPartitionFunction PartitionFunction(32, Resolution, VertexData.GetElementSize());
    IterateVertices(PartitionFunction, VertexAttribute::SLICEZINDEX);

    /* Sort the buckets and reload the table data */
    Progress::Begin("Sorting Vertex Data Partitions");
    ZIndexComparator C(VertexData.GetAttributeOffset(VertexAttribute::SLICEZINDEX));
    UINT VertexSize = VertexData.GetElementSize();
    UINT Offset = 0;
    Progress::Initialize(0, VertexData.GetCount(), "Vertices");
    for (SIZE_T i=0; i<PartitionFunction.Buckets.Length(); i++)
    {
      UINT Length = (UINT)PartitionFunction.Buckets[i]->Length();
      UINT Count = Length / VertexSize;

      LPVOID pBlock = AllocateMemory(Length);
      PartitionFunction.Buckets[i]->Seek(0);
      PartitionFunction.Buckets[i]->ReadToMemory(pBlock, Length);

      Trace("%u", Count);
      Sort(pBlock, Count, VertexSize, C);
      VertexData.Load(pBlock, Offset, Count);

      Offset += Count;
      FreeMemory(pBlock);

      Progress::Update(Offset);
    }
    Progress::End();

    if (TriangleData.GetCount() > 0)
    {
      Progress::Begin("Remapping Triangle Indices");
      /* Remap the triangle indices */
      UINT* IndexMapping = new UINT[VertexData.GetCount()];
      ZeroMemory(IndexMapping, VertexData.GetCount()*sizeof(UINT));

      CreateVertexIndexMappingFunction  CreateMappingFunction(IndexMapping);
      IterateVertices(CreateMappingFunction, VertexAttribute::INDEX);

      RemapVertexIndexFunction RemappingFunction(IndexMapping);
      IterateTriangles(RemappingFunction, TriangleAttribute::VERTICES);

      delete[] IndexMapping;
      Progress::End();

      /* Re-order the triangle data */
      SortTriangleData();
    }

    VertexData.RemoveAttribute(VertexAttribute::INDEX);

    m_pHeader->IndexType = IndexType::SLICEZINDEX;
    m_pHeader->IndexResolution = Resolution;
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::ConstructOctreeIndex(UINT Resolution)
{
  Progress::Begin("Constructing Octree Index");

  SortZCurve(1024);

  if (!VertexData.HasAttribute(VertexAttribute::ZINDEX))
    SetVertexZCurveIndex(Resolution);

  /* Build octree */
  BuildOctreeFunction OctreeFunction(Resolution, m_pHeader->IndexResolution);
  m_pIndex = OctreeFunction.m_pRoot;
  m_pIndex->VolumeBounds = GetBoundingBox();
  IterateVertices(OctreeFunction, VertexAttribute::ZINDEX, VertexAttribute::POSITION3);
  OctreeFunction.FinalizeVertexProcessing();

  BuildTriangleBoundingVolumeFunction TriangleBoundingVolumeFunction(m_pIndex); 
  IterateTriangleVertices(TriangleBoundingVolumeFunction, VertexAttribute::POSITION3);
  OctreeFunction.AggregateTree(m_pIndex);

  Progress::End();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
class TriangleIndexComparator
{
public:
  TriangleIndexComparator(UINT Offset)
  { m_Offset = Offset; }

  BOOL CompareLessThan(LPVOID pA, LPVOID pB)
  {
    INDEXEDTRIANGLE& A = *(INDEXEDTRIANGLE*)((LPBYTE)pA + m_Offset);
    INDEXEDTRIANGLE& B = *(INDEXEDTRIANGLE*)((LPBYTE)pB + m_Offset);

    UINT MinA = Min(A.Vertex[0], A.Vertex[1], A.Vertex[2]);
    UINT MinB = Min(B.Vertex[0], B.Vertex[1], B.Vertex[2]);

    return MinA < MinB;
  }

private:
  UINT m_Offset;
  AxisAlignedBox3F m_BoundingBox;
  UINT m_Count;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
class TriangleIndexPartitionFunction
{
public:
  TriangleIndexPartitionFunction(UINT PartitionResolution, UINT VertexCount, UINT TriangleSize)
  { 
    m_Resolution = PartitionResolution; 
    m_VertexCount = VertexCount;
    m_TriangleSize = TriangleSize;
  
    Buckets.Resize(m_Resolution);
    for (SIZE_T i=0; i<Buckets.Length(); i++)
      Buckets[i] = File::OpenTemporary();
  }

  DWORD GetTriangleAccess()
  { return PageAccess::READ; }
  DWORD GetVertexAccess()
  { return PageAccess::NOACCESS; }
  LPCTSTR GetTaskName()
  { return "Partitioning Triangle Data"; }

  typedef INDEXEDTRIANGLE ELEMENTTYPE;

  VOID ProcessTriangle(UINT i, INDEXEDTRIANGLE& T, LPVOID pTriangleData)
  {
    UINT MinIndex = Min(T.Vertex[0], T.Vertex[1], T.Vertex[2]);

    UINT BucketNumber = MinIndex / (m_VertexCount / m_Resolution);
    Buckets[BucketNumber]->Write(pTriangleData, m_TriangleSize);
  }

  Array<Ptr<File> > Buckets;

private:
  UINT m_TriangleSize;
  UINT m_VertexCount;
  UINT m_Resolution;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::SortTriangleData()
{
  Progress::Begin("Sorting Triangle Data");

  /* Place triangles into buckets */
  TriangleIndexPartitionFunction PartitionFunction(4, VertexData.GetCount(), TriangleData.GetElementSize());
  IterateTriangles(PartitionFunction, TriangleAttribute::VERTICES);

  /* Sort the buckets and reload the table data */
  Progress::Begin("Sorting Triangle Data Partitions");
  TriangleIndexComparator C(TriangleData.GetAttributeOffset(TriangleAttribute::VERTICES));
  UINT TriangleSize = TriangleData.GetElementSize();
  UINT Offset = 0;
  Progress::Initialize(0, TriangleData.GetCount(), "Triangles");
  for (SIZE_T i=0; i<PartitionFunction.Buckets.Length(); i++)
  {
    UINT Length = (UINT)PartitionFunction.Buckets[i]->Length();
    UINT Count = Length / TriangleSize;

    LPVOID pBlock = AllocateMemory(Length);
    PartitionFunction.Buckets[i]->Seek(0);
    PartitionFunction.Buckets[i]->ReadToMemory(pBlock, Length);

    Sort(pBlock, Count, TriangleSize, C);
    TriangleData.Load(pBlock, Offset, Count);

    Offset += Count;
    FreeMemory(pBlock);

    Progress::Update(Offset);
  }
  Progress::End();

  Progress::End();

  m_TriangleBlockInfoResolution = 0;
  if (m_pTriangleBlockInfo)
    delete[] m_pTriangleBlockInfo;
  m_pTriangleBlockInfo = 0;

}

//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
class SetVertexIndexFunction
{
public:
  DWORD GetTriangleAccess()
  { return PageAccess::NOACCESS; }
  DWORD GetVertexAccess()
  { return PageAccess::READWRITE; }
  LPCTSTR GetTaskName()
  { return "Setting Vertex Indices"; }

  typedef UINT ELEMENTTYPE;

  VOID ProcessVertex(UINT i, UINT& Index, LPVOID pVertexData)
  {
    Index = i;
  }
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::SetVertexIndices()
{
  if (!VertexData.HasAttribute(VertexAttribute::INDEX))
    VertexData.AddAttribute(VertexAttribute::INDEX);

  SetVertexIndexFunction F;
  IterateVertices(F, VertexAttribute::INDEX);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
class SetVertexZIndexFunction
{
public:
  SetVertexZIndexFunction(UINT Resolution, CONST AxisAlignedBox3F& BoundingBox)
  {
    m_Resolution = Resolution;
    m_BoundingBox = BoundingBox;
  }

  DWORD GetTriangleAccess()
  { return PageAccess::NOACCESS; }
  DWORD GetVertexAccess()
  { return PageAccess::READWRITE; }
  LPCTSTR GetTaskName()
  { return "Setting Vertex Z-Curve Indices"; }

  typedef ULONGLONG ELEMENTTYPE0;
  typedef Vector3F ELEMENTTYPE1;

  VOID ProcessVertex(UINT i, ULONGLONG& Index, Vector3F Position, LPVOID pVertexData)
  {
    Position[0] = (Position[0] - m_BoundingBox.Min(0)) / (m_BoundingBox.Max(0)-m_BoundingBox.Min(0));
    Position[1] = (Position[1] - m_BoundingBox.Min(1)) / (m_BoundingBox.Max(1)-m_BoundingBox.Min(1));
    Position[2] = (Position[2] - m_BoundingBox.Min(2)) / (m_BoundingBox.Max(2)-m_BoundingBox.Min(2));

    UINT16 X = Min((UINT)(Position[0] * m_Resolution), m_Resolution-1);
    UINT16 Y = Min((UINT)(Position[1] * m_Resolution), m_Resolution-1);
    UINT16 Z = Min((UINT)(Position[2] * m_Resolution), m_Resolution-1);

    Index = Interleave(X,Y,Z);
  }

private:
  AxisAlignedBox3F m_BoundingBox;
  UINT m_Resolution;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::SetVertexZCurveIndex(UINT Resolution)
{
  if (!VertexData.HasAttribute(VertexAttribute::ZINDEX))
    VertexData.AddAttribute(VertexAttribute::ZINDEX);

  SetVertexZIndexFunction F(Resolution, GetBoundingBox());
  IterateVertices(F, VertexAttribute::ZINDEX, VertexAttribute::POSITION3);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
class SetVertexSliceZIndexFunction
{
public:
  SetVertexSliceZIndexFunction(UINT Resolution, CONST AxisAlignedBox3F& BoundingBox)
  {
    m_Resolution = Resolution;
    m_BoundingBox = BoundingBox;
  }

  DWORD GetTriangleAccess()
  { return PageAccess::NOACCESS; }
  DWORD GetVertexAccess()
  { return PageAccess::READWRITE; }
  LPCTSTR GetTaskName()
  { return "Setting Vertex Slice Z-Curve Indices"; }

  typedef ULONGLONG ELEMENTTYPE0;
  typedef Vector3F ELEMENTTYPE1;

  VOID ProcessVertex(UINT i, ULONGLONG& Index, Vector3F Position, LPVOID pVertexData)
  {
    Position[0] = (Position[0] - m_BoundingBox.Min(0)) / (m_BoundingBox.Max(0)-m_BoundingBox.Min(0));
    Position[1] = (Position[1] - m_BoundingBox.Min(1)) / (m_BoundingBox.Max(1)-m_BoundingBox.Min(1));
    Position[2] = (Position[2] - m_BoundingBox.Min(2)) / (m_BoundingBox.Max(2)-m_BoundingBox.Min(2));

    UINT16 X = Min((UINT)(Position[0] * m_Resolution), m_Resolution-1);
    UINT16 Y = Min((UINT)(Position[1] * m_Resolution), m_Resolution-1);
    UINT16 Z = Min((UINT)(Position[2] * m_Resolution), m_Resolution-1);

    Index = Interleave(Y,Z) | ((ULONGLONG)X << 32);
  }

private:
  AxisAlignedBox3F m_BoundingBox;
  UINT m_Resolution;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::SetVertexSliceZCurveIndex(UINT Resolution)
{
  if (!VertexData.HasAttribute(VertexAttribute::SLICEZINDEX))
    VertexData.AddAttribute(VertexAttribute::SLICEZINDEX);

  SetVertexSliceZIndexFunction F(Resolution, GetBoundingBox());
  IterateVertices(F, VertexAttribute::SLICEZINDEX, VertexAttribute::POSITION3);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
class ComputeStatisticsFunction
{
public:
  ComputeStatisticsFunction()
  {
    BoundingBox.Min() = Vector3F( RealTraits<FLOAT>::MaxValue,  RealTraits<FLOAT>::MaxValue,  RealTraits<FLOAT>::MaxValue);
    BoundingBox.Max() = Vector3F(-RealTraits<FLOAT>::MaxValue, -RealTraits<FLOAT>::MaxValue, -RealTraits<FLOAT>::MaxValue);
  }

  DWORD GetTriangleAccess()
  { return PageAccess::NOACCESS; }
  DWORD GetVertexAccess()
  { return PageAccess::READ; }
  LPCTSTR GetTaskName()
  { return "Computing Statistics"; }

  typedef Vector3F ELEMENTTYPE;

  VOID ProcessVertex(UINT i, Vector3F& P, LPVOID pVertexData)
  {
    BoundingBox.Combine(P);

    CenterOfMass += Vector3<DOUBLE>(P(0), P(1), P(2));

    Covariance(0,0) += P(0)*P(0);
    Covariance(0,1) += P(0)*P(1);
    Covariance(0,2) += P(0)*P(2);
    Covariance(1,0) += P(1)*P(0);
    Covariance(1,1) += P(1)*P(1);
    Covariance(1,2) += P(1)*P(2);
    Covariance(2,0) += P(2)*P(0);
    Covariance(2,1) += P(2)*P(1);
    Covariance(2,2) += P(2)*P(2);
  }

  Matrix3<DOUBLE> Covariance;
  Vector3<DOUBLE> CenterOfMass;
  AxisAlignedBox3F BoundingBox;

};
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::ComputeStatistics()
{
  ComputeStatisticsFunction StatisticsFunction;
  IterateVertices(StatisticsFunction, VertexAttribute::POSITION3);

  StatisticsFunction.Covariance /= (DOUBLE)VertexData.GetCount();
  StatisticsFunction.CenterOfMass /= (DOUBLE)VertexData.GetCount();

  StatisticsFunction.Covariance(0,0) -= StatisticsFunction.CenterOfMass(0)*StatisticsFunction.CenterOfMass(0);
  StatisticsFunction.Covariance(0,1) -= StatisticsFunction.CenterOfMass(0)*StatisticsFunction.CenterOfMass(1);
  StatisticsFunction.Covariance(0,2) -= StatisticsFunction.CenterOfMass(0)*StatisticsFunction.CenterOfMass(2);
  StatisticsFunction.Covariance(1,0) -= StatisticsFunction.CenterOfMass(1)*StatisticsFunction.CenterOfMass(0);
  StatisticsFunction.Covariance(1,1) -= StatisticsFunction.CenterOfMass(1)*StatisticsFunction.CenterOfMass(1);
  StatisticsFunction.Covariance(1,2) -= StatisticsFunction.CenterOfMass(1)*StatisticsFunction.CenterOfMass(2);
  StatisticsFunction.Covariance(2,0) -= StatisticsFunction.CenterOfMass(2)*StatisticsFunction.CenterOfMass(0);
  StatisticsFunction.Covariance(2,1) -= StatisticsFunction.CenterOfMass(2)*StatisticsFunction.CenterOfMass(1);
  StatisticsFunction.Covariance(2,2) -= StatisticsFunction.CenterOfMass(2)*StatisticsFunction.CenterOfMass(2);

  m_pHeader->Statistics.BoundingBox = StatisticsFunction.BoundingBox;
  m_pHeader->Statistics.BoundingSphere.Center() = StatisticsFunction.BoundingBox.Center();
  m_pHeader->Statistics.BoundingSphere.Radius() = 0.5f * StatisticsFunction.BoundingBox.Diagonal().Length();

  m_pHeader->Statistics.CenterOfMass = StatisticsFunction.CenterOfMass;
  m_pHeader->Statistics.Covariance = StatisticsFunction.Covariance;

  m_pHeader->Statistics.FeatureSize = ComputeApproximateFeatureSize();

  m_pHeader->StatisticsFlags = 1;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Sphere3F BlockMesh::GetBoundingSphere()
{
  if (m_pHeader->StatisticsFlags == 0)
    ComputeStatistics();
  return m_pHeader->Statistics.BoundingSphere;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
AxisAlignedBox3F BlockMesh::GetBoundingBox()
{
  if (m_pHeader->StatisticsFlags == 0)
    ComputeStatistics();
  return m_pHeader->Statistics.BoundingBox;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
class ZeroVertexNormalFunction
{
public:
  DWORD GetTriangleAccess()
  { return PageAccess::NOACCESS; }
  DWORD GetVertexAccess()
  { return PageAccess::READWRITE; }
  LPCTSTR GetTaskName()
  { return "Clearing Normals"; }

  typedef Vector3F ELEMENTTYPE;

  VOID ProcessVertex(UINT i, Vector3F& Normal, LPVOID pVertexData)
  { Normal.SetZero(); }
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
class AccumulateNormalFunction
{
public:
  DWORD GetTriangleAccess()
  { return PageAccess::READ; }
  DWORD GetVertexAccess()
  { return PageAccess::READWRITE; }
  LPCTSTR GetTaskName()
  { return "Accumulating Triangle Face Normals"; }

  typedef Vector3F VERTEXELEMENTTYPE0;
  typedef Vector3F VERTEXELEMENTTYPE1;

  VOID ProcessTriangle(UINT i, INDEXEDTRIANGLE& T, Vector3F& V0, Vector3F& V1, Vector3F& V2, Vector3F& N0, Vector3F& N1, Vector3F& N2)
  {
    Vector3F A = V0-V1, B = V1-V2, C = V2-V0;

    Vector3F Normal0 = A.Cross(B);
    Vector3F Normal1 = B.Cross(C);
    Vector3F Normal2 = C.Cross(A);

    if (Normal0.Length() < 1E-10 || Normal1.Length() < 1E-10 || Normal2.Length() < 1E-10)
      return;

    FLOAT Area = (Normal0.Length() + Normal1.Length() + Normal2.Length()) / 3.0f;

    Vector3<FLOAT> Normal = Normal0 + Normal1 + Normal2;
    Normal.Normalize();

    N0 += Normal * Area;
    N1 += Normal * Area;
    N2 += Normal * Area;
  }

};
//----------------------------------------------------------------------------------------------------------------------------------------------------
class NormalizeVertexNormalFunction
{
public:
  DWORD GetTriangleAccess()
  { return PageAccess::NOACCESS; }
  DWORD GetVertexAccess()
  { return PageAccess::READWRITE; }
  LPCTSTR GetTaskName()
  { return "Normalizing Vertex Normals"; }

  typedef Vector3F ELEMENTTYPE;

  VOID ProcessVertex(UINT i, Vector3F& Normal, LPVOID pVertexData)
  { Normal.Normalize(); }
  
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::ComputeNormals()
{
  if (!TriangleData.HasAttribute(TriangleAttribute::VERTICES))
    return;

  Progress::Begin("Computing Normals");
  if (!VertexData.HasAttribute(VertexAttribute::NORMAL3))
  {
    Progress::Begin("Resizing Vertex data table");
    Progress::Initialize();
    VertexData.AddAttribute(VertexAttribute::NORMAL3);
    Progress::End();
  }
  else
  {
    ZeroVertexNormalFunction ZeroFunction;
    IterateVertices(ZeroFunction, VertexAttribute::NORMAL3);
  }

  AccumulateNormalFunction AccumulateFunction;
  IterateTriangleVertices(AccumulateFunction, VertexAttribute::POSITION3, VertexAttribute::NORMAL3);

  NormalizeVertexNormalFunction NormalizeFunction;
  IterateVertices(NormalizeFunction, VertexAttribute::NORMAL3);

  Progress::End();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT BlockMesh::ComputeApproximateFeatureSize()
{
  if (TriangleData.GetCount() == 0)
    return 0.0f;

  UINT NumberOfFaces = TriangleData.GetCount();
  UINT NumberOfSamples = Min(NumberOfFaces >> 1, 333u);

  Array<FLOAT> Samples;
  Samples.Reserve(NumberOfSamples * 3);

  UINT VertexPositionOffset = VertexData.GetAttributeOffset(VertexAttribute::POSITION3);
  UINT VertexNormalOffset = VertexData.GetAttributeOffset(VertexAttribute::NORMAL3);
  UINT TriangleVerticesOffset = TriangleData.GetAttributeOffset(TriangleAttribute::VERTICES);

  for (UINT i = 0; i < NumberOfSamples; i++) 
  {
    static unsigned randq = 0;
    randq = unsigned(1664525) * randq + unsigned(1013904223);

    UINT j = randq % NumberOfFaces;

    INDEXEDTRIANGLE& T = TriangleData.Attribute<INDEXEDTRIANGLE>(j, TriangleVerticesOffset);

    Vector3F& V0 = VertexData.Attribute<Vector3F>(T.Vertex[0], VertexPositionOffset);
    Vector3F& V1 = VertexData.Attribute<Vector3F>(T.Vertex[1], VertexPositionOffset);
    Vector3F& V2 = VertexData.Attribute<Vector3F>(T.Vertex[2], VertexPositionOffset);
    Vector3F A = V0-V1, B = V1-V2, C = V2-V0;

    Samples.Add(A.SquareLength());
    Samples.Add(B.SquareLength());
    Samples.Add(C.SquareLength());
  }
  Samples.Sort();

  return Sqrt(Samples[Samples.Length() >> 1]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
class TriangleSpanFunction
{
public:
  TriangleSpanFunction(UINT BlockSize, BlockMesh::TRIANGLEBLOCK* pBlock)
  {
    m_BlockSize = BlockSize;
    m_pBlock = pBlock;
  }

  DWORD GetTriangleAccess()
  { return PageAccess::READ; }
  DWORD GetVertexAccess()
  { return PageAccess::NOACCESS; }
  LPCTSTR GetTaskName()
  { return "Computing Triangle-Vertex Span"; }

  typedef INDEXEDTRIANGLE ELEMENTTYPE;

  VOID ProcessTriangle(UINT i, INDEXEDTRIANGLE& T, LPVOID pTriangleData)
  {
    UINT BlockIndex = i / m_BlockSize;

    m_pBlock[BlockIndex].MinimumVertexIndex = Min(m_pBlock[BlockIndex].MinimumVertexIndex, T.Vertex[0]);
    m_pBlock[BlockIndex].MinimumVertexIndex = Min(m_pBlock[BlockIndex].MinimumVertexIndex, T.Vertex[1]);
    m_pBlock[BlockIndex].MinimumVertexIndex = Min(m_pBlock[BlockIndex].MinimumVertexIndex, T.Vertex[2]);
    m_pBlock[BlockIndex].MaximumVertexIndex = Max(m_pBlock[BlockIndex].MaximumVertexIndex, T.Vertex[0]);
    m_pBlock[BlockIndex].MaximumVertexIndex = Max(m_pBlock[BlockIndex].MaximumVertexIndex, T.Vertex[1]);
    m_pBlock[BlockIndex].MaximumVertexIndex = Max(m_pBlock[BlockIndex].MaximumVertexIndex, T.Vertex[2]);
  }

  BlockMesh::TRIANGLEBLOCK* m_pBlock;
  UINT m_BlockSize;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::ComputeTriangleBlockInfo(UINT BlockSize)
{
  UINT TriangleVerticesOffset = TriangleData.GetAttributeOffset(TriangleAttribute::VERTICES);
  UINT BlockCount = TriangleData.GetCount() / BlockSize + 1;

  if (m_pTriangleBlockInfo)
  {
    delete[] m_pTriangleBlockInfo;
    m_pTriangleBlockInfo = 0;
  }

  TRIANGLEBLOCK* pTriangleBlockInfo = new TRIANGLEBLOCK[BlockCount];

  for (UINT i=0; i<BlockCount; i++)
  {
    pTriangleBlockInfo[i].MaximumVertexIndex = 0;
    pTriangleBlockInfo[i].MinimumVertexIndex = MAXUINT;
  }
  
  TriangleSpanFunction SpanFunction(BlockSize, pTriangleBlockInfo);
  IterateTriangles(SpanFunction, TriangleAttribute::VERTICES);

  m_pTriangleBlockInfo = pTriangleBlockInfo;
  m_TriangleBlockInfoResolution = BlockSize;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
