/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Trunk/Src/Graphics/BlockMesh.cpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1526 $
 * Last Updated: $Date: 2008-05-27 09:47:11 -0400 (Tue, 27 May 2008) $
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
T& Mesh::Table::GetAttribute(UINT i, UINT Id)
{
  UINT Partition = HIWORD(Id);
  return (T&)*(m_pPartitionData[Partition] + i*m_PartitionStride[Partition] + LOWORD(Id));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class FUNCTION>
VOID Mesh::Table::Iterate(FUNCTION& F, UINT Offset, UINT Length)
{
  TABLEROW Row(*this);

  if (Offset >= m_Count)
    Offset = m_Count;
  if ((Offset+Length) > m_Count)
    Length = 0;

  Row.Index = Offset;
  for (SIZE_T i=0; i<m_Partitions.Length(); i++)
  {
    Row.pPartitionRowData[i] = (LPBYTE)m_Partitions[i].Map(Bolitho::PageAccess::READWRITE);
    Row.PartitionRowSize[i] = m_Partitions[i].GetRowSize();
    Row.TotalRowSize += Row.PartitionRowSize[i];
  }

  for (UINT i=0; i<Length; i++)
  {
    F(Row);
    
    Row.Index++;
    for (UINT i=0; i<Row.PartitionCount; i++)
      Row.pPartitionRowData[i] += Row.PartitionRowSize[i];
  }

  for (SIZE_T i=0; i<m_Partitions.Length(); i++)
    m_Partitions[i].Unmap();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class FUNCTION>
VOID Mesh::Table::IterateBlocks(FUNCTION& F, UINT BlockSize, UINT Offset, UINT Length)
{
  TABLEROW Row(*this);

  if (Offset >= m_Count)
    Offset = m_Count;
  if ((Offset+Length) > m_Count)
    Length = 0;

  Row.Index = Offset;
  for (SIZE_T i=0; i<m_Partitions.Length(); i++)
  {
    Row.pPartitionRowData[i] = (LPBYTE)m_Partitions[i].Map(PageAccess::READWRITE);
    Row.PartitionRowSize[i] = m_Partitions[i].GetRowSize();
    Row.TotalRowSize += Row.PartitionRowSize[i];
  }

  for (UINT i=0; i<Length; i+=BlockSize)
  {
    UINT ThisBlockSize = Min(BlockSize, m_Count-i);
    F(Row, ThisBlockSize);
    
    Row.Index += BlockSize;
    for (UINT i=0; i<Row.PartitionCount; i++)
      Row.pPartitionRowData[i] += Row.PartitionRowSize[i]*BlockSize;
  }

  for (SIZE_T i=0; i<m_Partitions.Length(); i++)
    m_Partitions[i].Unmap();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class FUNCTION>
VOID Mesh::Table::Iterate(FUNCTION& F, UINT AttributeId0, UINT Offset, UINT Length)
{
  TABLEROW Row(*this);

  if (Offset >= m_Count)
    Offset = m_Count;
  if ((Offset+Length) > m_Count)
    Length = 0;

  Row.Index = Offset;
  for (SIZE_T i=0; i<m_Partitions.Length(); i++)
  {
    Row.pPartitionRowData[i] = (LPBYTE)m_Partitions[i].Map(PageAccess::READWRITE);
    Row.PartitionRowSize[i] = m_Partitions[i].GetRowSize();
    Row.TotalRowSize += Row.PartitionRowSize[i];
  }

  UINT PartitionId0 = HIWORD(AttributeId0);
  Partition& Partition0 = m_Partitions[PartitionId0];
  UINT Offset0 = Partition0.GetAttributeOffset(LOWORD(AttributeId0));
  LPBYTE pAttribute0 = Row.pPartitionRowData[PartitionId0] + Offset*Row.PartitionRowSize[PartitionId0] + Offset0;

  for (UINT i=0; i<Length; i++)
  {
    F(Row, (typename FUNCTION::ATTRIBUTE0&)*pAttribute0);
    
    Row.Index++;
    for (UINT i=0; i<Row.PartitionCount; i++)
      Row.pPartitionRowData[i] += Row.PartitionRowSize[i];
    pAttribute0 += Row.PartitionRowSize[PartitionId0];
  }

  for (SIZE_T i=0; i<m_Partitions.Length(); i++)
    m_Partitions[i].Unmap();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class FUNCTION>
VOID Mesh::Table::Iterate(FUNCTION& F, UINT AttributeId0, UINT AttributeId1, UINT Offset, UINT Length)
{
  TABLEROW Row(*this);

  if (Offset >= m_Count)
    Offset = m_Count;
  if ((Offset+Length) > m_Count)
    Length = 0;

  Row.Index = Offset;
  for (SIZE_T i=0; i<m_Partitions.Length(); i++)
  {
    Row.pPartitionRowData[i] = (LPBYTE)m_Partitions[i].Map(PageAccess::READWRITE);
    Row.PartitionRowSize[i] = m_Partitions[i].GetRowSize();
    Row.TotalRowSize += Row.PartitionRowSize[i];
  }

  UINT PartitionId0 = HIWORD(AttributeId0);
  Partition& Partition0 = m_Partitions[PartitionId0];
  UINT Offset0 = Partition0.GetAttributeOffset(LOWORD(AttributeId0));
  LPBYTE pAttribute0 = Row.pPartitionRowData[PartitionId0] + Offset*Row.PartitionRowSize[PartitionId0] + Offset0;

  UINT PartitionId1 = HIWORD(AttributeId1);
  Partition& Partition1 = m_Partitions[PartitionId1];
  UINT Offset1 = Partition1.GetAttributeOffset(LOWORD(AttributeId1));
  LPBYTE pAttribute1 = Row.pPartitionRowData[PartitionId1] + Offset*Row.PartitionRowSize[PartitionId1] + Offset1;

  for (UINT i=0; i<Length; i++)
  {
    F(Row, (typename FUNCTION::ATTRIBUTE0&)*pAttribute0, (typename FUNCTION::ATTRIBUTE1&)*pAttribute1);
    
    Row.Index++;
    for (UINT i=0; i<Row.PartitionCount; i++)
      Row.pPartitionRowData[i] += Row.PartitionRowSize[i];
    pAttribute0 += Row.PartitionRowSize[PartitionId0];
    pAttribute1 += Row.PartitionRowSize[PartitionId1];
  }

  for (SIZE_T i=0; i<m_Partitions.Length(); i++)
    m_Partitions[i].Unmap();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class FUNCTION>
VOID Mesh::IterateTriangleVertices(FUNCTION& F, UINT VertexAttributeId0, UINT Offset, UINT Length)
{
  TABLEROW TriangleRow(TriangleData);
  TABLEROW VertexRowTemplate(VertexData);

  TABLEROW VertexRow0(VertexData);
  TABLEROW VertexRow1(VertexData);
  TABLEROW VertexRow2(VertexData);

  if (Offset >= TriangleData.m_Count)
    Offset = TriangleData.m_Count;
  if ((Offset+Length) > TriangleData.m_Count)
    Length = 0;

  TriangleRow.Index = Offset;
  for (SIZE_T i=0; i<TriangleData.m_Partitions.Length(); i++)
  {
    TriangleRow.pPartitionRowData[i] = (LPBYTE)TriangleData.m_Partitions[i].Map(PageAccess::READWRITE);
    TriangleRow.PartitionRowSize[i] = TriangleData.m_Partitions[i].GetRowSize();
    TriangleRow.TotalRowSize += TriangleRow.PartitionRowSize[i];
  }

  for (SIZE_T i=0; i<VertexData.m_Partitions.Length(); i++)
  {
    VertexRowTemplate.pPartitionRowData[i] = (LPBYTE)VertexData.m_Partitions[i].Map(PageAccess::READWRITE);
    VertexRowTemplate.PartitionRowSize[i] = VertexData.m_Partitions[i].GetRowSize();
    VertexRowTemplate.TotalRowSize += VertexRowTemplate.PartitionRowSize[i];

    VertexRow0.pPartitionRowData[i] = VertexRowTemplate.pPartitionRowData[i];
    VertexRow0.PartitionRowSize[i] = VertexRowTemplate.PartitionRowSize[i];
    VertexRow1.pPartitionRowData[i] = VertexRowTemplate.pPartitionRowData[i];
    VertexRow1.PartitionRowSize[i] = VertexRowTemplate.PartitionRowSize[i];
    VertexRow2.pPartitionRowData[i] = VertexRowTemplate.pPartitionRowData[i];
    VertexRow2.PartitionRowSize[i] = VertexRowTemplate.PartitionRowSize[i];

  }
  VertexRow0.TotalRowSize = VertexRow1.TotalRowSize = VertexRow2.TotalRowSize = VertexRowTemplate.TotalRowSize;

  UINT VertexPartitionId0 = HIWORD(VertexAttributeId0);
  Partition& VertexPartition0 = VertexData.m_Partitions[VertexPartitionId0];
  UINT VertexOffset0 = VertexPartition0.GetAttributeOffset(LOWORD(VertexAttributeId0));
  LPBYTE pVertexAttribute0 = VertexRowTemplate.pPartitionRowData[VertexPartitionId0] + VertexOffset0;

  UINT TriangleAttributeId0 = TriangleData.GetAttributeId(TriangleAttribute::VERTICES);
  UINT TrianglePartitionId0 = HIWORD(TriangleAttributeId0);
  Partition& TrianglePartition0 = TriangleData.m_Partitions[TrianglePartitionId0];
  UINT TriangleOffset0 = TrianglePartition0.GetAttributeOffset(LOWORD(TriangleAttributeId0));
  LPBYTE pTriangleAttribute0 = TriangleRow.pPartitionRowData[TrianglePartitionId0] + TriangleOffset0;

  for (UINT i=0; i<Length; i++)
  {
    INDEXEDTRIANGLE& Triangle = (INDEXEDTRIANGLE&)*pTriangleAttribute0;

    /* Dereference triangle */
    VertexRow0.Index = Triangle.Vertex[0];
    VertexRow1.Index = Triangle.Vertex[1];
    VertexRow2.Index = Triangle.Vertex[2];
    for(UINT i=0; i<VertexRowTemplate.PartitionCount; i++)
    {
      VertexRow0.pPartitionRowData[i] += VertexRow0.PartitionRowSize[i]*Triangle.Vertex[0];
      VertexRow1.pPartitionRowData[i] += VertexRow1.PartitionRowSize[i]*Triangle.Vertex[1];
      VertexRow2.pPartitionRowData[i] += VertexRow2.PartitionRowSize[i]*Triangle.Vertex[2];
    }

    F(TriangleRow, VertexRow0, VertexRow1, VertexRow2,
      (typename FUNCTION::ATTRIBUTE0&)*(pVertexAttribute0 + VertexRowTemplate.PartitionRowSize[VertexPartitionId0]*Triangle.Vertex[0]),
      (typename FUNCTION::ATTRIBUTE0&)*(pVertexAttribute0 + VertexRowTemplate.PartitionRowSize[VertexPartitionId0]*Triangle.Vertex[1]),
      (typename FUNCTION::ATTRIBUTE0&)*(pVertexAttribute0 + VertexRowTemplate.PartitionRowSize[VertexPartitionId0]*Triangle.Vertex[2])
     );

    TriangleRow.Index++;
    for (UINT i=0; i<TriangleRow.PartitionCount; i++)
      TriangleRow.pPartitionRowData[i] += TriangleRow.PartitionRowSize[i];
    pTriangleAttribute0 += TriangleRow.PartitionRowSize[TrianglePartitionId0];

    for(UINT i=0; i<VertexRowTemplate.PartitionCount; i++)
    {
      VertexRow0.pPartitionRowData[i] = VertexRowTemplate.pPartitionRowData[i];
      VertexRow1.pPartitionRowData[i] = VertexRowTemplate.pPartitionRowData[i];
      VertexRow2.pPartitionRowData[i] = VertexRowTemplate.pPartitionRowData[i];
    }
  }

  for (SIZE_T i=0; i<VertexData.m_Partitions.Length(); i++)
    VertexData.m_Partitions[i].Unmap();
  for (SIZE_T i=0; i<TriangleData.m_Partitions.Length(); i++)
    TriangleData.m_Partitions[i].Unmap();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class FUNCTION>
VOID Mesh::IterateTriangleVertices(FUNCTION& F, UINT VertexAttributeId0, UINT VertexAttributeId1, UINT Offset, UINT Length)
{
  TABLEROW TriangleRow(TriangleData);
  TABLEROW VertexRowTemplate(VertexData);

  TABLEROW VertexRow0(VertexData);
  TABLEROW VertexRow1(VertexData);
  TABLEROW VertexRow2(VertexData);

  if (Offset >= TriangleData.m_Count)
    Offset = TriangleData.m_Count;
  if ((Offset+Length) > TriangleData.m_Count)
    Length = 0;

  TriangleRow.Index = Offset;
  for (SIZE_T i=0; i<TriangleData.m_Partitions.Length(); i++)
  {
    TriangleRow.pPartitionRowData[i] = (LPBYTE)TriangleData.m_Partitions[i].Map(PageAccess::READWRITE);
    TriangleRow.PartitionRowSize[i] = TriangleData.m_Partitions[i].GetRowSize();
    TriangleRow.TotalRowSize += TriangleRow.PartitionRowSize[i];
  }

  for (SIZE_T i=0; i<VertexData.m_Partitions.Length(); i++)
  {
    VertexRowTemplate.pPartitionRowData[i] = (LPBYTE)VertexData.m_Partitions[i].Map(PageAccess::READWRITE);
    VertexRowTemplate.PartitionRowSize[i] = VertexData.m_Partitions[i].GetRowSize();
    VertexRowTemplate.TotalRowSize += VertexRowTemplate.PartitionRowSize[i];

    VertexRow0.pPartitionRowData[i] = VertexRowTemplate.pPartitionRowData[i];
    VertexRow0.PartitionRowSize[i] = VertexRowTemplate.PartitionRowSize[i];
    VertexRow1.pPartitionRowData[i] = VertexRowTemplate.pPartitionRowData[i];
    VertexRow1.PartitionRowSize[i] = VertexRowTemplate.PartitionRowSize[i];
    VertexRow2.pPartitionRowData[i] = VertexRowTemplate.pPartitionRowData[i];
    VertexRow2.PartitionRowSize[i] = VertexRowTemplate.PartitionRowSize[i];

  }
  VertexRow0.TotalRowSize = VertexRow1.TotalRowSize = VertexRow2.TotalRowSize = VertexRowTemplate.TotalRowSize;

  UINT VertexPartitionId0 = HIWORD(VertexAttributeId0);
  Partition& VertexPartition0 = VertexData.m_Partitions[VertexPartitionId0];
  UINT VertexOffset0 = VertexPartition0.GetAttributeOffset(LOWORD(VertexAttributeId0));
  LPBYTE pVertexAttribute0 = VertexRowTemplate.pPartitionRowData[VertexPartitionId0] + VertexOffset0;

  UINT VertexPartitionId1 = HIWORD(VertexAttributeId1);
  Partition& VertexPartition1 = VertexData.m_Partitions[VertexPartitionId1];
  UINT VertexOffset1 = VertexPartition1.GetAttributeOffset(LOWORD(VertexAttributeId1));
  LPBYTE pVertexAttribute1 = VertexRowTemplate.pPartitionRowData[VertexPartitionId1] + VertexOffset1;

  UINT TriangleAttributeId0 = TriangleData.GetAttributeId(TriangleAttribute::VERTICES);
  UINT TrianglePartitionId0 = HIWORD(TriangleAttributeId0);
  Partition& TrianglePartition0 = TriangleData.m_Partitions[TrianglePartitionId0];
  UINT TriangleOffset0 = TrianglePartition0.GetAttributeOffset(LOWORD(TriangleAttributeId0));
  LPBYTE pTriangleAttribute0 = TriangleRow.pPartitionRowData[TrianglePartitionId0] + TriangleOffset0;

  for (UINT i=0; i<Length; i++)
  {
    INDEXEDTRIANGLE& Triangle = (INDEXEDTRIANGLE&)*pTriangleAttribute0;

    /* Dereference triangle */
    VertexRow0.Index = Triangle.Vertex[0];
    VertexRow1.Index = Triangle.Vertex[1];
    VertexRow2.Index = Triangle.Vertex[2];

    for(UINT i=0; i<VertexRowTemplate.PartitionCount; i++)
    {
      VertexRow0.pPartitionRowData[i] += VertexRow0.PartitionRowSize[i]*Triangle.Vertex[0];
      VertexRow1.pPartitionRowData[i] += VertexRow1.PartitionRowSize[i]*Triangle.Vertex[1];
      VertexRow2.pPartitionRowData[i] += VertexRow2.PartitionRowSize[i]*Triangle.Vertex[2];
    }

    F(TriangleRow, VertexRow0, VertexRow1, VertexRow2,
      (typename FUNCTION::ATTRIBUTE0&)*(pVertexAttribute0 + VertexRowTemplate.PartitionRowSize[VertexPartitionId0]*Triangle.Vertex[0]),
      (typename FUNCTION::ATTRIBUTE0&)*(pVertexAttribute0 + VertexRowTemplate.PartitionRowSize[VertexPartitionId0]*Triangle.Vertex[1]),
      (typename FUNCTION::ATTRIBUTE0&)*(pVertexAttribute0 + VertexRowTemplate.PartitionRowSize[VertexPartitionId0]*Triangle.Vertex[2]),

      (typename FUNCTION::ATTRIBUTE1&)*(pVertexAttribute1 + VertexRowTemplate.PartitionRowSize[VertexPartitionId1]*Triangle.Vertex[0]),
      (typename FUNCTION::ATTRIBUTE1&)*(pVertexAttribute1 + VertexRowTemplate.PartitionRowSize[VertexPartitionId1]*Triangle.Vertex[1]),
      (typename FUNCTION::ATTRIBUTE1&)*(pVertexAttribute1 + VertexRowTemplate.PartitionRowSize[VertexPartitionId1]*Triangle.Vertex[2])
     );

    TriangleRow.Index++;
    for (UINT i=0; i<TriangleRow.PartitionCount; i++)
      TriangleRow.pPartitionRowData[i] += TriangleRow.PartitionRowSize[i];
    pTriangleAttribute0 += TriangleRow.PartitionRowSize[TrianglePartitionId0];

    for(UINT i=0; i<VertexRowTemplate.PartitionCount; i++)
    {
      VertexRow0.pPartitionRowData[i] = VertexRowTemplate.pPartitionRowData[i];
      VertexRow1.pPartitionRowData[i] = VertexRowTemplate.pPartitionRowData[i];
      VertexRow2.pPartitionRowData[i] = VertexRowTemplate.pPartitionRowData[i];
    }
  }

  for (SIZE_T i=0; i<VertexData.m_Partitions.Length(); i++)
    VertexData.m_Partitions[i].Unmap();
  for (SIZE_T i=0; i<TriangleData.m_Partitions.Length(); i++)
    TriangleData.m_Partitions[i].Unmap();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PARTITIONCOMPAREFUNCTION>
class PartitionCompareFunction
{
public:
  PartitionCompareFunction(PARTITIONCOMPAREFUNCTION& PartitionCompareFunction, UINT BucketCount, UINT Offset) : m_PartitionCompareFunction(PartitionCompareFunction)
  {
    m_BucketCount = BucketCount;
    m_Buckets.Resize(BucketCount);
    for (SIZE_T i=0; i<BucketCount; i++)
      m_Buckets[i] = File::OpenTemporary();
    m_Offset = Offset;
  }

  typedef typename PARTITIONCOMPAREFUNCTION::ATTRIBUTE0 ATTRIBUTE0;

  VOID operator()(Mesh::TABLEROW& Row, CONST ATTRIBUTE0& Attribute)
  {
    BYTE RowData[1024];

    UINT Bucket = m_PartitionCompareFunction.Partition(Row, Attribute, m_BucketCount);
    Row.CopyTo(RowData);
    m_Buckets[Bucket]->Write(RowData, Row.TotalRowSize);
  }

  BOOL CompareLessThan(LPVOID pA, LPVOID pB)
  {
    return m_PartitionCompareFunction.CompareLessThan(*(ATTRIBUTE0*)((LPBYTE)pA + m_Offset), *(ATTRIBUTE0*)((LPBYTE)pB + m_Offset));
  }

  Array< Ptr<IO::File> > m_Buckets;

private:
  PARTITIONCOMPAREFUNCTION& m_PartitionCompareFunction;
  UINT m_BucketCount;
  UINT m_Offset;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PARTITIONCOMPAREFUNCTION>
VOID Mesh::Table::Sort(UINT AttributeId0, PARTITIONCOMPAREFUNCTION& F)
{
  UINT BucketCount = 32;

  PartitionCompareFunction<PARTITIONCOMPAREFUNCTION> PartitionCompare(F, BucketCount, 0);
  
  Iterate(PartitionCompare, AttributeId0, 0, m_Count);

  UINT Offset = 0;
  UINT TotalRowSize = GetTotalRowSize();

  for (UINT i=0; i<BucketCount; i++)
  {
    UINT Length = (UINT)PartitionCompare.m_Buckets[i]->Length();
    UINT Count = Length / TotalRowSize;

    LPVOID pBlock = AllocateMemory(Length);
    PartitionCompare.m_Buckets[i]->Seek(0);
    Stream::Copy(pBlock, PartitionCompare.m_Buckets[i], Length);

    Bolitho::Sort(pBlock, Count, TotalRowSize, PartitionCompare);

    Write(pBlock, Offset, Count);

    Offset += Count;
    FreeMemory(pBlock);
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PARTITIONFUNCTION>
VOID Mesh::SortVertexData(UINT AttributeId, PARTITIONFUNCTION& PartitionFunction)
{ 
  if (TriangleData.GetCount() > 0)
    SetVertexIndices();

  VertexData.Sort(AttributeId, PartitionFunction); 

  if (TriangleData.GetCount() > 0)
    RemapTriangleIndices();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
