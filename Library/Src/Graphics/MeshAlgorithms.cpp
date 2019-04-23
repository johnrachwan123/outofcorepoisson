/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Trunk/Src/Graphics/MeshIO.cpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1528 $
 * Last Updated: $Date: 2008-05-27 16:17:19 -0400 (Tue, 27 May 2008) $
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

#include "Mesh.hpp"
#include "Graphics/MeshPly.hpp"

#include "Math/Math.hpp"
#include "Math/Geometry/Types.hpp"

#include "IO/File.hpp"
#include "IO/Path.hpp"
#include "IO/TextStream.hpp"
#include "Collections/Stack.hpp"

using namespace Bolitho;
using namespace Bolitho::Math;
using namespace Bolitho::Math::Geometry;
using namespace Bolitho::Graphics;
using namespace Bolitho::IO;

//----------------------------------------------------------------------------------------------------------------------------------------------------
class ZeroVertexNormalFunction
{
public:
  typedef Vector3F ATTRIBUTE0;

  VOID operator() (CONST Mesh::TABLEROW& Row, Vector3F& Normal)
  { Normal.SetZero(); }
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
class AccumulateNormalFunction
{
public:
  typedef Vector3F ATTRIBUTE0;
  typedef Vector3F ATTRIBUTE1;

  VOID operator()(CONST Mesh::TABLEROW& TriangleRow, CONST Mesh::TABLEROW& VertexRow0, CONST Mesh::TABLEROW& VertexRow1, CONST Mesh::TABLEROW& VertexRow2, Vector3F& V0, Vector3F& V1, Vector3F& V2, Vector3F& N0, Vector3F& N1, Vector3F& N2)
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

  typedef Vector3F ATTRIBUTE0;

  VOID operator()(CONST Mesh::TABLEROW& Row, Vector3F& Normal)
  { Normal.Normalize(); }
  
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::ComputeNormals()
{
  if (!TriangleData.HasAttribute(TriangleAttribute::VERTICES))
    return;

  if (!VertexData.HasAttribute(VertexAttribute::NORMAL3))
  {
    VertexData.AddAttribute(VertexAttribute::NORMAL3);
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

}
//----------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------
class ComputeStatisticsFunction
{
public:
  ComputeStatisticsFunction()
  {
    BoundingBox.Min() = Vector3F(RealTraits<FLOAT>::MaxValue,  RealTraits<FLOAT>::MaxValue,  RealTraits<FLOAT>::MaxValue);
    BoundingBox.Max() = Vector3F(-RealTraits<FLOAT>::MaxValue, -RealTraits<FLOAT>::MaxValue, -RealTraits<FLOAT>::MaxValue);
  }

  typedef Vector3F ATTRIBUTE0;

  VOID operator()(CONST Mesh::TABLEROW& Row, Vector3F& P)
  {
    BoundingBox.Union(P);

    CenterOfMass += Vector3D(P(0), P(1), P(2));

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

  Matrix3D Covariance;
  Vector3D CenterOfMass;
  AxisAlignedBox3F BoundingBox;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::ComputeStatistics()
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

  m_BoundingBox = StatisticsFunction.BoundingBox;
  m_BoundingSphere.Center() = StatisticsFunction.BoundingBox.Center();
  m_BoundingSphere.Radius() = 0.5f * StatisticsFunction.BoundingBox.Diagonal().Length();
  m_CenterOfMass = StatisticsFunction.CenterOfMass;
  m_Covariance = StatisticsFunction.Covariance;
  m_FeatureSize = ComputeApproximateFeatureSize();
  m_StatisticsFlags = 1;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT Mesh::ComputeApproximateFeatureSize()
{
  if (TriangleData.GetCount() == 0)
    return 0.0f;

  UINT NumberOfFaces = TriangleData.GetCount();
  UINT NumberOfSamples = Min(NumberOfFaces >> 1, 333u);

  Array<FLOAT> Samples;
  Samples.Reserve(NumberOfSamples * 3);

  UINT VertexPositionId = VertexData.GetAttributeId(VertexAttribute::POSITION3);
  UINT TriangleVerticesId = TriangleData.GetAttributeId(TriangleAttribute::VERTICES);

  TriangleData.Map();
  VertexData.Map();
  for (UINT i = 0; i<NumberOfSamples; i++) 
  {
    static unsigned randq = 0;
    randq = unsigned(1664525) * randq + unsigned(1013904223);

    UINT j = randq % NumberOfFaces;

    INDEXEDTRIANGLE& T = TriangleData.GetAttribute<INDEXEDTRIANGLE>(j, TriangleVerticesId);

    Vector3F& V0 = VertexData.GetAttribute<Vector3F>(T.Vertex[0], VertexPositionId);
    Vector3F& V1 = VertexData.GetAttribute<Vector3F>(T.Vertex[1], VertexPositionId);
    Vector3F& V2 = VertexData.GetAttribute<Vector3F>(T.Vertex[2], VertexPositionId);
    Vector3F A = V0-V1, B = V1-V2, C = V2-V0;

    Samples.Add(A.SquareLength());
    Samples.Add(B.SquareLength());
    Samples.Add(C.SquareLength());
  }
  Samples.Sort();

  TriangleData.Unmap();
  VertexData.Unmap();

  return Sqrt(Samples[Samples.Length() >> 1]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
CONST AxisAlignedBox3F& Mesh::GetBoundingBox()
{
  if (m_StatisticsFlags == 0)
    ComputeStatistics();
  return m_BoundingBox;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
CONST Sphere3F& Mesh::GetBoundingSphere()
{
  if (m_StatisticsFlags == 0)
    ComputeStatistics();
  return m_BoundingSphere;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------


#include "Utility/Bits.hpp"
using Bolitho::Interleave;
//----------------------------------------------------------------------------------------------------------------------------------------------------
class SetVertexZIndexFunction
{
public:
  SetVertexZIndexFunction(UINT Resolution, CONST AxisAlignedBox3F& BoundingBox)
  {
    m_Resolution = Resolution;
    m_BoundingBox = BoundingBox;
  }

  typedef UINT64 ATTRIBUTE0;
  typedef Vector3F ATTRIBUTE1;

  VOID operator()(CONST Mesh::TABLEROW& Row, UINT64& Index, Vector3F Position)
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
VOID Mesh::SetVertexZCurveIndex(UINT Resolution, CONST AxisAlignedBox3F& BoundingBox)
{
  if (!VertexData.HasAttribute(VertexAttribute::ZINDEX))
    VertexData.AddAttribute(VertexAttribute::ZINDEX);

  SetVertexZIndexFunction F(Resolution, BoundingBox);
  IterateVertices(F, VertexAttribute::ZINDEX, VertexAttribute::POSITION3);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
class SetVertexSliceZIndexFunction
{
public:
  SetVertexSliceZIndexFunction(UINT XResolution, UINT YZResolution, CONST AxisAlignedBox3F& BoundingBox)
  {
    m_XResolution = XResolution;
    m_YZResolution = YZResolution;
    m_BoundingBox = BoundingBox;
  }

  typedef UINT64 ATTRIBUTE0;
  typedef Vector3F ATTRIBUTE1;

  VOID operator()(CONST Mesh::TABLEROW& Row, UINT64& Index, Vector3F Position)
  {
    Position[0] = (Position[0] - m_BoundingBox.Min(0)) / (m_BoundingBox.Max(0)-m_BoundingBox.Min(0));
    Position[1] = (Position[1] - m_BoundingBox.Min(1)) / (m_BoundingBox.Max(1)-m_BoundingBox.Min(1));
    Position[2] = (Position[2] - m_BoundingBox.Min(2)) / (m_BoundingBox.Max(2)-m_BoundingBox.Min(2));

    UINT64 X = Min((UINT)(Position[0] * m_XResolution), m_XResolution-1);
    UINT16 Y = Min((UINT)(Position[1] * m_YZResolution), m_YZResolution-1);
    UINT16 Z = Min((UINT)(Position[2] * m_YZResolution), m_YZResolution-1);

    Index = Interleave(Y,Z) | (X << 32);
    
  }

private:
  AxisAlignedBox3F m_BoundingBox;
  UINT m_XResolution;
  UINT m_YZResolution;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::SetVertexSliceZCurveIndex(UINT XResolution, UINT YZResolution, CONST AxisAlignedBox3F& BoundingBox)
{
  if (!VertexData.HasAttribute(VertexAttribute::ZINDEX))
    VertexData.AddAttribute(VertexAttribute::ZINDEX);

  SetVertexSliceZIndexFunction F(XResolution, YZResolution, BoundingBox);
  IterateVertices(F, VertexAttribute::ZINDEX, VertexAttribute::POSITION3);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------
class MinimumVertexIndexSortFunction
{
public:
  typedef INDEXEDTRIANGLE ATTRIBUTE0;

  UINT Partition(Mesh::TABLEROW& Row, CONST ATTRIBUTE0& Attribute, UINT BucketCount)
  {
    return 0;
  }

  BOOL CompareLessThan(INDEXEDTRIANGLE A, INDEXEDTRIANGLE B)
  {
    UINT AMin = Min(A.Vertex[0], A.Vertex[1], A.Vertex[2]);
    UINT BMin = Min(B.Vertex[0], B.Vertex[1], B.Vertex[2]);

    return AMin < BMin; 
  }

  UINT m_Resolution;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
class ZIndexSortFunction
{
public:
  ZIndexSortFunction(UINT Resolution) : m_Resolution(Resolution)
  {}

  typedef UINT64 ATTRIBUTE0;

  UINT Partition(Mesh::TABLEROW& Row, CONST ATTRIBUTE0& Attribute, UINT BucketCount)
  {
    return 0;
  }

  BOOL CompareLessThan(UINT64 A, UINT64 B)
  { return A < B; }

  UINT m_Resolution;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
class BuildOctreeFunction
{
public:
  BuildOctreeFunction(UINT OctreeResolution, UINT ZIndexResolution);

  typedef UINT ATTRIBUTE0;
  typedef Vector3F ATTRIBUTE1;

  VOID operator() (Mesh::TABLEROW& Row, UINT Index, Vector3F& Position);
  VOID FinalizeVertexProcessing();

  Mesh::OctreeNode* m_pRoot;
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
  m_pRoot = new Mesh::OctreeNode();

  m_ReduceShift = (Log2(ZIndexResolution) - Log2(OctreeResolution)) * 3;
  m_MaxZIndex = m_Resolution * m_Resolution * m_Resolution;
  m_Mask = (m_MaxZIndex >> 1) | (m_MaxZIndex >> 2) | (m_MaxZIndex >> 3);
  m_Shift = Log2(m_MaxZIndex) - 3;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BuildOctreeFunction::operator() (Mesh::TABLEROW& Row, UINT Index, Vector3F& Position)
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

  m_BoundingBox.Union(Position);
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

  Mesh::OctreeNode* pNode = m_pRoot;
  while (Mask != 0)
  {
    UINT i = (ZIndex & Mask) >> Shift;

    if (!pNode->HasChildren())
      pNode->CreateChildren();

    pNode = pNode->GetChild(i);

    Shift -= 3;
    Mask = Mask >> 3;
  }

  pNode->SetVertexBounds(Offset, Length, BoundingBox);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
class BuildTriangleBoundingVolumeFunction
{
public:
  BuildTriangleBoundingVolumeFunction(Mesh::OctreeNode* pRoot) 
  {
    m_pRoot = pRoot;
    m_pNode = pRoot;
    while (m_pNode->HasChildren())
    {
      m_Parent.Push(m_pNode);
      m_pNode = m_pNode->GetChild(0);
    }

    while (m_pNode->GetVertexCount() == 0)
      m_pNode = GetNextLeafNode();
    m_pNode->TriangleBounds() = AxisAlignedBox3F::Empty;
  }

  typedef Vector3F ATTRIBUTE0;

  VOID operator() (Mesh::TABLEROW& TriangleRow, Mesh::TABLEROW& VertexRow0, Mesh::TABLEROW& VertexRow1, Mesh::TABLEROW& VertexRow2, Vector3F& V0, Vector3F& V1, Vector3F& V2);

private:
  Mesh::OctreeNode* m_pNode;
  Mesh::OctreeNode* GetNextLeafNode();
  Stack<Mesh::OctreeNode*> m_Parent;
  Mesh::OctreeNode* m_pRoot;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BuildTriangleBoundingVolumeFunction::operator() (Mesh::TABLEROW& TriangleRow, Mesh::TABLEROW& VertexRow0, Mesh::TABLEROW& VertexRow1, Mesh::TABLEROW& VertexRow2, Vector3F& V0, Vector3F& V1, Vector3F& V2)
{
  UINT MinVertex = Min(VertexRow0.Index, VertexRow1.Index, VertexRow2.Index);

  if (MinVertex >= (m_pNode->GetVertexOffset() + m_pNode->GetVertexCount()))
  {
    do
    {
      m_pNode = GetNextLeafNode();
      if (m_pNode == NULL)
        break;
      m_pNode->TriangleOffset() = TriangleRow.Index;
      m_pNode->TriangleBounds() = AxisAlignedBox3F::Empty;
    }
    while (m_pNode->VertexCount() == 0);
  }

  m_pNode->TriangleBounds().Union(V0);
  m_pNode->TriangleBounds().Union(V1);
  m_pNode->TriangleBounds().Union(V2);
  m_pNode->TriangleCount()++;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Mesh::OctreeNode* BuildTriangleBoundingVolumeFunction::GetNextLeafNode()
{
  Mesh::OctreeNode* pNode = m_pNode;

  while(TRUE)
  {
    if (m_Parent.IsEmpty())
      return NULL;
    Mesh::OctreeNode* pParent = m_Parent.Peek();
    PTRDIFF_T i = pNode - pParent->GetChild(0);
    if (i < 7)
    {
      pNode = pParent->GetChild(i+1);
      break;
    }
    pNode = m_Parent.Pop();
  }

  while (pNode->HasChildren())
  {
    m_Parent.Push(pNode);
    pNode = pNode->GetChild(0);
  }
  
  return pNode;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::ConstructOctreeIndex(UINT Resolution, CONST AxisAlignedBox3F& BoundingBox)
{
  SetVertexZCurveIndex(Resolution, BoundingBox);
  ZIndexSortFunction VertexSortFunction(Resolution);
  SortVertexData(VertexAttribute::ZINDEX, VertexSortFunction);
  MinimumVertexIndexSortFunction TriangleSortFunction;
  SortTriangleData(TriangleAttribute::VERTICES, TriangleSortFunction);

  /* Build octree */
  BuildOctreeFunction OctreeFunction(Resolution, Resolution);
  m_pOctreeIndex = OctreeFunction.m_pRoot;
  m_pOctreeIndex->VolumeBounds() = GetBoundingBox();
  IterateVertices(OctreeFunction, VertexAttribute::ZINDEX, VertexAttribute::POSITION3);
  OctreeFunction.FinalizeVertexProcessing();

  BuildTriangleBoundingVolumeFunction TriangleBoundingVolumeFunction(m_pOctreeIndex); 
  IterateTriangleVertices(TriangleBoundingVolumeFunction, VertexAttribute::POSITION3);
  
  m_pOctreeIndex->AggregateTree();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::ConstructSliceQuadtreeIndex(UINT XResolution, UINT YZResolution, CONST AxisAlignedBox3F& BoundingBox)
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------
class SetVertexIndexFunction
{
public:
  typedef UINT ATTRIBUTE0;

  VOID operator() (Mesh::TABLEROW& Row, UINT& Index)
  {
    Index = Row.Index;
  }
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::SetVertexIndices()
{
  if (!VertexData.HasAttribute(VertexAttribute::INDEX))
    VertexData.AddAttribute(VertexAttribute::INDEX);

  SetVertexIndexFunction F;
  IterateVertices(F, VertexAttribute::INDEX);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
class CreateVertexIndexMappingFunction
{
public:
  CreateVertexIndexMappingFunction(UINT* pMapping)
  { m_pMapping = pMapping; }

  typedef UINT ATTRIBUTE0;

  VOID operator() (Mesh::TABLEROW& Row, UINT& Index)
  {
    m_pMapping[Index] = Row.Index;
  }

  UINT* m_pMapping;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
class RemapVertexIndexFunction
{
public:
  RemapVertexIndexFunction(UINT* pMapping)
  { m_pMapping = pMapping; }

  typedef INDEXEDTRIANGLE ATTRIBUTE0;

  VOID operator() (Mesh::TABLEROW& Row, INDEXEDTRIANGLE& T)
  {
    T.Vertex[0] = m_pMapping[T.Vertex[0]];
    T.Vertex[1] = m_pMapping[T.Vertex[1]];
    T.Vertex[2] = m_pMapping[T.Vertex[2]];
  }

  UINT* m_pMapping;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::RemapTriangleIndices()
{
  UINT* IndexMapping = new UINT[VertexData.GetCount()];
  ZeroMemory(IndexMapping, VertexData.GetCount()*sizeof(UINT));

  CreateVertexIndexMappingFunction  CreateMappingFunction(IndexMapping);
  IterateVertices(CreateMappingFunction, VertexAttribute::INDEX);

  RemapVertexIndexFunction RemappingFunction(IndexMapping);
  IterateTriangles(RemappingFunction, TriangleAttribute::VERTICES);

  delete[] IndexMapping;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
