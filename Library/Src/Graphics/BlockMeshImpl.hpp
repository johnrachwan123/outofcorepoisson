/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/BlockMeshImpl.hpp $
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T* BlockMesh::DataTable::Row(UINT i)
{
  Assert(i < m_pHeader->Count);
  UINT j = i / m_pHeader->CountPerBlock;
  UINT k = i % m_pHeader->CountPerBlock;

  LPBYTE pData = (LPBYTE)GetBlockData(j);
  return (T*)(pData + m_pHeader->ElementSize*k);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T* BlockMesh::DataTable::Row(UINT i) CONST
{
  Assert(i < m_pHeader->Count);
  UINT j = i / m_pHeader->CountPerBlock;
  UINT k = i % m_pHeader->CountPerBlock;

  LPCBYTE pData = (LPBYTE)GetBlockData(j);
  return (CONST T*)(pData + m_pHeader->ElementSize*k);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T& BlockMesh::DataTable::Attribute(UINT i, UINT Offset)
{
  Assert(i < m_pHeader->Count);
  UINT j = i / m_pHeader->CountPerBlock;
  UINT k = i % m_pHeader->CountPerBlock;

  LPBYTE pData = (LPBYTE)GetBlockData(j);
  return *(T*)(pData + m_pHeader->ElementSize*k + Offset);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& BlockMesh::DataTable::Attribute(UINT i, UINT Offset) CONST
{
  Assert(i < m_pHeader->Count);
  UINT j = i / m_pHeader->CountPerBlock;
  UINT k = i % m_pHeader->CountPerBlock;

  LPCBYTE pData = (LPBYTE)GetBlockData(j);
  return *(CONST T*)(pData + m_pHeader->ElementSize*k + Offset);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------


/*
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class Func>
VOID BlockMesh::DataTable::Iterate(Func F)
{
  Iterate<T>(F, 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class Func, class U>
VOID BlockMesh::DataTable::Iterate(Func F, U& P0)
{
  Iterate<T>(F, 0, P0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class Func, class U, class V>
VOID BlockMesh::DataTable::Iterate(Func F, U& P0, V& P1)
{
  Iterate<T>(F, 0, P0, P1);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class Func>
VOID BlockMesh::DataTable::Iterate(Func F, CONST ElementDecl& Attribute)
{
  Iterate<T>(F, GetAttributeOffset(Attribute));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class Func, class U>
VOID BlockMesh::DataTable::Iterate(Func F, CONST ElementDecl& Attribute, U& P0)
{
  Iterate<T>(F, GetAttributeOffset(Attribute), P0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class Func, class U, class V>
VOID BlockMesh::DataTable::Iterate(Func F, CONST ElementDecl& Attribute, U& P0, V& P1)
{
  Iterate<T>(F, GetAttributeOffset(Attribute), P0, P1);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class Func>
VOID BlockMesh::DataTable::Iterate(Func F, UINT Offset)
{
  MapView(PageAccess::READWRITE);

  UINT Stride = m_pHeader->ElementSize;
  LPBYTE pData = (LPBYTE)m_pData + Offset;

  Progress::Initialize(0, m_pHeader->Count);
  UINT j = 0;
  for (UINT i=0; i<m_pHeader->Count; i++, pData+=Stride)
  {
    F(i, *(T*)pData);

    if (j - i > 131072)
    {
      Progress::Update(i);
      //Hint(i, BlockSize, TableHint::FINISHED);
      //Unlock(j, i - j - 65536);
      j = i-65536;
    }
  }
  
  Unmap();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class Func, class U>
VOID BlockMesh::DataTable::Iterate(Func F, UINT Offset, U& P0)
{
  MapView(PageAccess::READWRITE);

  UINT Stride = m_pHeader->ElementSize;
  LPBYTE pData = (LPBYTE)m_pData + Offset;

  UINT j = 0;
  Progress::Initialize(0, m_pHeader->Count, "Vertices");
  for (UINT i=0; i<m_pHeader->Count; i++, pData+=Stride)
  {
    F(i, *(T*)pData, P0);

    if (i - j > 131072)
    {
      Progress::Update(i);
      Hint(j, i - j - 65536, TableHint::FINISHED);
      j = i-65536;
    }
  }

  Unmap();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class Func, class U, class V>
VOID BlockMesh::DataTable::Iterate(Func F, UINT Offset, U& P0, V& P1)
{
  MapView(PageAccess::READWRITE);

  UINT Stride = m_pHeader->ElementSize;
  LPBYTE pData = (LPBYTE)m_pData + Offset;

  UINT j = 0;
  Progress::Initialize(0, m_pHeader->Count, "Vertices");
  for (UINT i=0; i<m_pHeader->Count; i++, pData+=Stride)
  {
    F(i, *(T*)pData, P0, P1);

    if (i - j > 131072)
    {
      Progress::Update(i);
      Hint(j, i - j - 65536, TableHint::FINISHED);
      j = i-65536;
    }
  }

  Unmap();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
*/

//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class FUNCTION>
VOID BlockMesh::IterateTriangles(FUNCTION& F)
{
  IterateTriangles(F, 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class FUNCTION>
VOID BlockMesh::IterateTriangles(FUNCTION& F, CONST ElementDecl& Attribute)
{
  UINT Offset = TriangleData.GetAttributeOffset(Attribute);
  if (Offset > TriangleData.GetElementSize())
    throw MeshException(this, String::Format("The triangle data table does not have the attribute \"%s\"", Attribute.Name.ConstStr()));
  if (TriangleData.GetCount() == 0)
    return;

  IterateTriangles(F, Offset);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class FUNCTION>
VOID BlockMesh::IterateTriangles(FUNCTION& F, CONST ElementDecl& Attribute0, CONST ElementDecl& Attribute1)
{
  UINT Offset0 = TriangleData.GetAttributeOffset(Attribute0);
  if (Offset0 > TriangleData.GetElementSize())
    throw MeshException(this, String::Format("The triangle data table does not have the attribute \"%s\"", Attribute0.Name.ConstStr()));
  UINT Offset1 = TriangleData.GetAttributeOffset(Attribute1);
  if (Offset1 > TriangleData.GetElementSize())
    throw MeshException(this, String::Format("The triangle data table does not have the attribute \"%s\"", Attribute1.Name.ConstStr()));

  if (TriangleData.GetCount() == 0)
    return;

  IterateTriangles(F, Offset0, Offset1);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class FUNCTION>
VOID BlockMesh::IterateTriangles(FUNCTION& F, UINT Offset)
{
  Progress::Begin(F.GetTaskName());
  Progress::Initialize(0, TriangleData.GetCount(), "Triangles");
  
  for (UINT i=0; i<TriangleData.GetBlockCount(); i++)
  {
    LPBYTE pData = (LPBYTE)TriangleData.GetBlockData(i) + Offset;
    
    UINT k = i * TriangleData.GetBlockRowCount();
    for (UINT j=0; j<TriangleData.GetBlockRowCount(i); j++, k++)
    {
      F.ProcessTriangle(k, (typename FUNCTION::ELEMENTTYPE&)*pData, pData);
      pData += TriangleData.GetElementSize();
    }

    Progress::Update(k);
  }

  Progress::End();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class FUNCTION>
VOID BlockMesh::IterateTriangles(FUNCTION& F, UINT Offset0, UINT Offset1)
{  
  Progress::Begin(F.GetTaskName());
  Progress::Initialize(0, TriangleData.GetCount(), "Triangles");
  
  for (UINT i=0; i<TriangleData.GetBlockCount(); i++)
  {
    LPBYTE pData0 = (LPBYTE)TriangleData.GetBlockData(i) + Offset0;
    LPBYTE pData1 = (LPBYTE)TriangleData.GetBlockData(i) + Offset1;

    UINT k = i * TriangleData.GetBlockRowCount();
    for (UINT j=0; j<TriangleData.GetBlockRowCount(i); j++, k++)
    {
      F.ProcessTriangle(k, (typename FUNCTION::ELEMENTTYPE0&)*pData0, (typename FUNCTION::ELEMENTTYPE1&)*pData1, pData0-Offset0);
      pData0 += TriangleData.GetElementSize();
      pData1 += TriangleData.GetElementSize();
    }

    Progress::Update(k);
  }

  Progress::End();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class FUNCTION>
VOID BlockMesh::IterateVertices(FUNCTION& F)
{
  IterateVertices(F, 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class FUNCTION>
VOID BlockMesh::IterateVertices(FUNCTION& F, CONST ElementDecl& Attribute)
{
  UINT Offset = VertexData.GetAttributeOffset(Attribute);
  if (Offset > VertexData.GetElementSize())
    throw MeshException(this, String::Format("The vertex data table does not have the attribute \"%s\"", Attribute.Name.ConstStr()));
  if (VertexData.GetCount() == 0)
    return;

  IterateVertices(F, Offset);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class FUNCTION>
VOID BlockMesh::IterateVertices(FUNCTION& F, CONST ElementDecl& Attribute0, CONST ElementDecl& Attribute1)
{
  UINT Offset0 = VertexData.GetAttributeOffset(Attribute0);
  if (Offset0 > VertexData.GetElementSize())
    throw MeshException(this, String::Format("The vertex data table does not have the attribute \"%s\"", Attribute0.Name.ConstStr()));
  UINT Offset1 = VertexData.GetAttributeOffset(Attribute1);
  if (Offset1 > VertexData.GetElementSize())
    throw MeshException(this, String::Format("The vertex data table does not have the attribute \"%s\"", Attribute1.Name.ConstStr()));

  if (VertexData.GetCount() == 0)
    return;

  IterateVertices(F, Offset0, Offset1);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class FUNCTION>
VOID BlockMesh::IterateVertices(FUNCTION& F, UINT Offset)
{
  Progress::Begin(F.GetTaskName());
  Progress::Initialize(0, VertexData.GetCount(), "Vertices");

  for (UINT i=0; i<VertexData.GetBlockCount(); i++)
  {
    LPBYTE pData = (LPBYTE)VertexData.GetBlockData(i) + Offset;
    UINT k = i*VertexData.GetBlockRowCount();

    for (UINT j=0; j<VertexData.GetBlockRowCount(i); j++, k++)
    {
      F.ProcessVertex(k, (typename FUNCTION::ELEMENTTYPE&)*pData, pData-Offset);
      pData += VertexData.GetElementSize();
    }
    
    Progress::Update(k);
  }

  Progress::End();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class FUNCTION>
VOID BlockMesh::IterateVertices(FUNCTION& F, UINT Offset0, UINT Offset1)
{
  Progress::Begin(F.GetTaskName());
  Progress::Initialize(0, VertexData.GetCount(), "Vertices");

  for (UINT i=0; i<VertexData.GetBlockCount(); i++)
  {
    LPBYTE pData0 = (LPBYTE)VertexData.GetBlockData(i) + Offset0;
    LPBYTE pData1 = (LPBYTE)VertexData.GetBlockData(i) + Offset1;
    UINT k = i*VertexData.GetBlockRowCount();

    for (UINT j=0; j<VertexData.GetBlockRowCount(i); j++, k++)
    {
      F.ProcessVertex(k, (typename FUNCTION::ELEMENTTYPE0&)*pData0, (typename FUNCTION::ELEMENTTYPE1&)*pData1, pData0-Offset0);
      pData0 += VertexData.GetElementSize();
      pData1 += VertexData.GetElementSize();
    }
    
    Progress::Update(k);
  }

  Progress::End();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class FUNCTION>
VOID BlockMesh::IterateTriangleVertices(FUNCTION& F, CONST ElementDecl& Attribute0)
{
  UINT Offset0 = VertexData.GetAttributeOffset(Attribute0);
  if (Offset0 > VertexData.GetElementSize())
    throw MeshException(this, String::Format("The vertex data table does not have the attribute \"%s\"", Attribute0.Name.ConstStr()));

  if (TriangleData.GetCount() == 0)
    return;

  IterateTriangleVertices(F, Offset0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class FUNCTION>
VOID BlockMesh::IterateTriangleVertices(FUNCTION& F, CONST ElementDecl& Attribute0, CONST ElementDecl& Attribute1)
{
  UINT Offset0 = VertexData.GetAttributeOffset(Attribute0);
  if (Offset0 > VertexData.GetElementSize())
    throw MeshException(this, String::Format("The vertex data table does not have the attribute \"%s\"", Attribute0.Name.ConstStr()));
  UINT Offset1 = VertexData.GetAttributeOffset(Attribute1);
  if (Offset1 > VertexData.GetElementSize())
    throw MeshException(this, String::Format("The vertex data table does not have the attribute \"%s\"", Attribute1.Name.ConstStr()));

  if (TriangleData.GetCount() == 0)
    return;

  IterateTriangleVertices(F, Offset0, Offset1);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class FUNCTION>
VOID BlockMesh::IterateTriangleVertices(FUNCTION& F, UINT Offset0)
{
  Progress::Begin(F.GetTaskName());
  Progress::Initialize(0, TriangleData.GetCount(), "Triangles");

  UINT Offset = TriangleData.GetAttributeOffset(TriangleAttribute::VERTICES);
  UINT VertexStride = VertexData.GetElementSize();
  UINT TriangleStride = TriangleData.GetElementSize();

  for (UINT i=0; i<TriangleData.GetBlockCount(); i++)
  {
    LPBYTE pData = (LPBYTE)TriangleData.GetBlockData(i) + Offset;
    UINT k = i*TriangleData.GetBlockRowCount();

    for (UINT j=0; j<TriangleData.GetBlockRowCount(i); j++, k++)
    {
      INDEXEDTRIANGLE& T = (INDEXEDTRIANGLE&)*pData;

      LPBYTE pVertex00 = VertexData.Row<BYTE>(T.Vertex[0]) + Offset0;
      LPBYTE pVertex01 = VertexData.Row<BYTE>(T.Vertex[1]) + Offset0;
      LPBYTE pVertex02 = VertexData.Row<BYTE>(T.Vertex[2]) + Offset0;
      F.ProcessTriangle(k, T, (typename FUNCTION::VERTEXELEMENTTYPE&)*pVertex00, (typename FUNCTION::VERTEXELEMENTTYPE&)*pVertex01, (typename FUNCTION::VERTEXELEMENTTYPE&)*pVertex02 );

      pData += TriangleStride;
    }
    
    Progress::Update(k);
  }
  Progress::End();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class FUNCTION>
VOID BlockMesh::IterateTriangleVertices(FUNCTION& F, UINT Offset0, UINT Offset1)
{
  Progress::Begin(F.GetTaskName());
  Progress::Initialize(0, TriangleData.GetCount(), "Triangles");

  UINT Offset = TriangleData.GetAttributeOffset(TriangleAttribute::VERTICES);
  UINT VertexStride = VertexData.GetElementSize();
  UINT TriangleStride = TriangleData.GetElementSize();

  for (UINT i=0; i<TriangleData.GetBlockCount(); i++)
  {
    LPBYTE pData = (LPBYTE)TriangleData.GetBlockData(i) + Offset;
    UINT k = i*TriangleData.GetBlockRowCount();

    for (UINT j=0; j<TriangleData.GetBlockRowCount(i); j++, k++)
    {
      INDEXEDTRIANGLE& T = (INDEXEDTRIANGLE&)*pData;

      LPBYTE pVertex00 = VertexData.Row<BYTE>(T.Vertex[0]) + Offset0;
      LPBYTE pVertex01 = VertexData.Row<BYTE>(T.Vertex[1]) + Offset0;
      LPBYTE pVertex02 = VertexData.Row<BYTE>(T.Vertex[2]) + Offset0;

      LPBYTE pVertex10 = VertexData.Row<BYTE>(T.Vertex[0]) + Offset1;
      LPBYTE pVertex11 = VertexData.Row<BYTE>(T.Vertex[1]) + Offset1;
      LPBYTE pVertex12 = VertexData.Row<BYTE>(T.Vertex[2]) + Offset1;

      F.ProcessTriangle(k, T, (typename FUNCTION::VERTEXELEMENTTYPE0&)*pVertex00, (typename FUNCTION::VERTEXELEMENTTYPE0&)*pVertex01, (typename FUNCTION::VERTEXELEMENTTYPE0&)*pVertex02, (typename FUNCTION::VERTEXELEMENTTYPE1&)*pVertex10, (typename FUNCTION::VERTEXELEMENTTYPE1&)*pVertex11, (typename FUNCTION::VERTEXELEMENTTYPE1&)*pVertex12 );

      pData += TriangleStride;
    }
    
    Progress::Update(k);
  }
  Progress::End();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class FUNCTION>
VOID BlockMesh::IterateTriangleBlocks(FUNCTION& F)
{
  Progress::Begin(F.GetTaskName());
  Progress::Initialize(0, TriangleData.GetCount(), "Triangles");
  
  for (UINT i=0; i<TriangleData.GetBlockCount(); i++)
  {
    UINT k = i * TriangleData.GetBlockRowCount();

    F.ProcessTriangleBlock(k, i, TriangleData.GetBlockRowCount(i), TriangleData.GetBlockData(i), TriangleData.GetBlock(i));

    Progress::Update(k);
  }

  Progress::End();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class FUNCTION>
VOID BlockMesh::IterateVertexBlocks(FUNCTION& F)
{
  Progress::Begin(F.GetTaskName());
  Progress::Initialize(0, VertexData.GetCount(), "Vertices");
  
  for (UINT i=0; i<VertexData.GetBlockCount(); i++)
  {
    UINT k = i * VertexData.GetBlockRowCount();

    F.ProcessVertexBlock(k, i, VertexData.GetBlockRowCount(i), VertexData.GetBlockData(i), VertexData.GetBlock(i));

    Progress::Update(k);
  }

  Progress::End();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
