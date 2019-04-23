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

#pragma once

#include "Utility/ElementDecl.hpp"
#include "Data/Xml/Reader.hpp"
#include "Data/Xml/Writer.hpp"

#include "Math/Matrix3.hpp"
using Bolitho::Math::Matrix3D;
using Bolitho::Math::Vector3D;
using Bolitho::Math::Matrix3F;
using Bolitho::Math::Vector3F;

#include "Math/AxisAlignedBox.hpp"
using Bolitho::Math::AxisAlignedBox3F;

#include "Math/Sphere.hpp"
using Bolitho::Math::Sphere3F;

#include "Math/Geometry/Types.hpp"
using Bolitho::Math::Geometry::INDEXEDTRIANGLE;

#include "Utility/Sort.hpp"
#include "Memory/VirtualMemory.hpp"

namespace Bolitho
{
  namespace Graphics
  {
    struct MeshDataUsage
    { enum { NONE = 0, READ = 1, WRITE = 2, READWRITE = 3, STREAMDATA = 8, MAPDATA = 16, RESIZEPARTITION = 32, CREATEPARTITION = 64, DELETEPARTITION = 128 }; };
    
    struct MeshFlags
    { enum { CREATE = 1, OPEN = 2, TEMPORARY = 4 }; };

    struct MeshPartitionFlags
    { enum { TEMPORARY = 1 }; };

    class LIBRARY_API Mesh : public Object
    {
      DeclareRuntimeType();

      class Attribute
      {
        friend class Mesh;
      public:
        Attribute();
        Attribute(LPCTSTR Name, LPCTSTR Type, UINT Offset);
        Attribute(LPCTSTR Name, DWORD Type, UINT Dimensions);
        Attribute(CONST Attribute& A);

        /* Returns the total number size (in bytes) of the  attribute */
        UINT Size() CONST;

        /* Returns the dimensionality fo the attribute */
        UINT Dimensions() CONST
        { return m_Dimensions; }
        /* Returns the fundamental datatype of the attribute */
        DWORD Type() CONST
        { return m_DataType; }
        /* Returns a string representation of the type */
        String TypeString() CONST;
        /* Returns the name of the attribute */
        String Name() CONST
        { return m_Name; }
        /* Returns the partition-based record offset */
        UINT Offset() CONST
        { return m_Offset; }

        STATIC CONST UINT INVALIDID = (UINT)~0;
      private:
        /* The human-readable name of the attribute */
        String m_Name;
        /* The basic data type */
        DWORD m_DataType;
        /* The dimensionality of the data */
        UINT m_Dimensions;
        /* The offset within a data parition row */
        UINT m_Offset;
      };

      class TableDataStorage : public Object
      {
      public:
        /* Maps the data store into memory */
        virtual LPVOID Map(DWORD PageAccess) = 0;
        /* Unmaps the data store */
        virtual VOID Unmap() = 0;

        /* Returns a stream to the data */
        virtual Ptr<IO::Stream> GetStream() = 0;

        /* Resizes the data store */
        virtual VOID Resize(ULONGLONG Length) = 0;

        /* Closes the data store */
        virtual VOID Close() = 0;

        /* Permenantly deletes the data store */
        virtual VOID Delete() = 0;

        /* Replaces the data with the contents of a new file */
        virtual VOID ReplaceData(LPCTSTR Filename) = 0;
      };

      class FileDataStorage : public TableDataStorage
      {
      public:
        /* Constructs a persistent file data store */
        FileDataStorage(LPCTSTR Filename);

        /* Maps the data store into memory */
        virtual LPVOID Map(DWORD PageAccess);
        /* Unmaps the data store */
        virtual VOID Unmap();

        /* Returns a stream to the data */
        virtual Ptr<IO::Stream> GetStream();

        /* Resizes the data store */
        virtual VOID Resize(ULONGLONG Length);

        /* Closes the data store */
        virtual VOID Close();

        /* Permenantly deletes the data store */
        virtual VOID Delete();

        /* Replaces the data with the contents of a new file */
        virtual VOID ReplaceData(LPCTSTR Filename);
      private:
        Ptr<IO::File> m_pFile;
        Ptr<IO::FileView> m_pView;
        BOOL m_IsPersistent;
        String m_Filename;
      };
      
      class Table;
      class Partition
      {
        friend class Mesh;
      public:
        Partition();
        ~Partition();

        /* Opens the partition data file */
        VOID Open(DWORD DataUsage, TableDataStorage* pStore);
        /* Closes the partition data file */
        VOID Close();
        /* Deletes the partition data file */
        VOID Delete();

        /* Maps the partition into memory and returns the base pointer */
        virtual LPVOID Map(DWORD PageAccess);
        /* Unmaps the partition from memory */
        virtual VOID Unmap();

        /* Returns the row size */
        UINT GetRowSize() CONST
        { return m_RowSize; }
        /* Returns the allocated row size */
        UINT GetAllocatedRowSize() CONST
        { return m_AttributeSize; }
        /* Returns the size of the partition */
        ULONGLONG GetSize() CONST
        { return m_RowSize * m_pTable->m_Count; }

        /* Changes the shape of the table */
        virtual VOID Resize(UINT RowSize, UINT RowCount);

        /* Returns a stream to the table data */
        virtual Ptr<IO::Stream> GetStream();

        /* Returns the collection of attributes */
        CONST Array<Attribute>& Attributes()
        { return m_Attributes; }

        /* Returns a reference to an attribute */
        CONST Attribute& GetAttribute(UINT i) CONST
        { return m_Attributes[i]; }

        /* Adds an attribute to the partition */
        VOID AddAttribute(CONST Attribute& A);

        /* Returns TRUE if the table contains a given attribute */
        BOOL HasAttribute(CONST Attribute& A) CONST;

        /* Returns the offset of an attribute */
        UINT GetAttributeOffset(UINT i)
        { return m_Attributes[i].Offset(); }

        /* Returns the index of an attribute */
        UINT GetAttributeIndex(CONST Attribute& A) CONST;

        /* Removes an attribute from the table */
        VOID RemoveAttribute(CONST Attribute& A);

        /* Removes all attributes */
        VOID RemoveAllAttributes();

        /* Returns the storage name of the partition */
        String GetName()
        { return m_Name; }

        /* Returns TRUE iff the partition should persist across sessions */
        BOOL IsPersistent();
      private:
        /* The attributes in this table partition */
        Array<Attribute> m_Attributes;

        /* The numebr of times the partition data has been mapped into memory */
        UINT m_MappingCount;

        Table* m_pTable;
        Ptr<TableDataStorage> m_pStore;

        /* The name of the partition */
        String m_Name;

        DWORD m_Flags;

        DWORD m_DataUsage;

        /* The stride of data in the data store */
        UINT m_RowSize;

        /* The size of the allocated protion of a row */
        UINT m_AttributeSize;
      };

      struct AttributeType
      { 
        enum 
        { 
          NONE      = 0x0001, 
          INT8      = 0x1001, 
          UINT8     = 0x2001, 
          INT16     = 0x1002, 
          UINT16    = 0x2002, 
          INT32     = 0x1004, 
          UINT32    = 0x2004, 
          INT64     = 0x1008, 
          UINT64    = 0x2008, 
          REAL32    = 0x3004, 
          REAL64    = 0x3008 
        }; 
      };

      class Table
      {
        friend class Mesh;
      public:
        Table(Mesh& M);

        /* Gets an attribute Id */
        UINT GetAttributeId(CONST Attribute& A) CONST;

        /* Initializes the table for a certain usage pattern, returns TRUE if the mesh is in the correct storage mode */
        BOOL Open(DWORD DataUsage);
        VOID Close();

        CONST Attribute& GetAttribute(UINT A) CONST
        { return m_Partitions[A >> 16].GetAttribute(A & 0xffff); }

        /* Returns the partition index of an attribute */
        UINT GetAttributePartition(UINT A) CONST
        { return A >> 16; }

        /* Returns the offset of an attribute in a partition */
        UINT GetAttributePartitionOffset(UINT A)
        { return m_Partitions[A >> 16].GetAttributeOffset(A & 0xffff); }

        /* Returns the row size of the partition an attribute exists in */
        UINT GetAttributePartitionRowSize(UINT A)
        { return m_Partitions[A >> 16].GetRowSize(); }

        /* Returns TRUE if the table contains a given attribute */
        BOOL HasAttribute(CONST Attribute& A) CONST;

        /* Adds a new attribute to the table */
        VOID AddAttribute(CONST Attribute& A);
        VOID AddAttribute(CONST Attribute& A, UINT PartitionId);

        /* Removes an attribute from the table */
        VOID RemoveAttribute(CONST Attribute& A);

        /* Removes all attributes from the table */
        VOID RemoveAllAttributes();
        /* Removes all attributes from a partition */
        VOID RemoveAllAttributes(UINT PartitionId);

        /* Resizes the table */
        VOID Resize(UINT Count);

        /* Creates a new data partition */
        UINT CreatePartition(UINT Size, DWORD Flags = 0);
        UINT CreatePartition(LPCTSTR Filename, UINT Size, DWORD Flags = 0);

        /* Peforms a gather read from the table data to a memory location */
        VOID Read(LPVOID pData, UINT Offset, UINT Count);
        /* Performs a scatter write to the table from a memory location */
        VOID Write(LPCVOID pData, UINT Offset, UINT Count);

        /* Peforms a read from a table partition to a memory location */
        VOID Read(LPVOID pData, UINT Partition, UINT Offset, UINT Count);
        /* Performs a write to the table partition from a memory location */
        VOID Write(LPCVOID pData, UINT Partition, UINT Offset, UINT Count);

        /* Deletes a partition and all attributes assocated with it */
        VOID DeletePartition(UINT PartitionId);

        /* Deletes all partitions */
        VOID DeleteAllPartitions();

        /* Partition access */
        CONST Partition& GetPartition(UINT i) CONST
        { return m_Partitions[i]; }
        Partition& GetPartition(UINT i)
        { return m_Partitions[i]; }

        /* Returns the collection of table partitions */
        CONST Array<Partition>& Partitions()
        { return m_Partitions; }

        /* Returns the number of table partitions */
        UINT GetPartitionCount() CONST
        { return (UINT)m_Partitions.Length(); }

        /* Returns the number of rows in the table */
        UINT GetCount() CONST
        { return m_Count; }

        UINT GetTotalRowSize() CONST
        { UINT RowSize = 0; for (SIZE_T i=0; i<m_Partitions.Length(); i++) RowSize += m_Partitions[i].GetRowSize(); return RowSize; }

        /* Maps all the table partitions into memory */
        VOID Map();
        /* Unmaps the table partitions from memory */
        VOID Unmap();

        /* Returns a reference to a given attribute in a given row of the table.  Assumes that the table has already been mapped into memory */
        template<class T>
        T& GetAttribute(UINT i, UINT Id);

        /* Iterates across a table in blocks of size 'BlockSize' */
        template<class FUNCTION>
        VOID IterateBlocks(FUNCTION& F, UINT BlockSize, UINT Offset, UINT Length);
        /* Iterates across a table */
        template<class FUNCTION>
        VOID Iterate(FUNCTION& F, UINT Offset, UINT Length);
        /* Iterates across a table, dereferencing a single attribute 'AttributeId0' */
        template<class FUNCTION>
        VOID Iterate(FUNCTION& F, UINT AttributeId0, UINT Offset, UINT Length);
        /* Iterates across a table, dereferencing two attributes 'AttributeId0' and 'AttributeId1' */
        template<class FUNCTION>
        VOID Iterate(FUNCTION& F, UINT AttributeId0, UINT AttributeId1, UINT Offset, UINT Length);

        /* Sorts a table by a given attribute */
        template<class PARTITIONFUNCTION>
        VOID Sort(UINT AttributeId, PARTITIONFUNCTION& ParitionFunction);

        CONST STATIC UINT MAXPARTITIONS = 8;
      protected:
        Array<Partition> m_Partitions;
        LPBYTE m_pPartitionData[MAXPARTITIONS];
        UINT m_PartitionStride[MAXPARTITIONS];

        UINT m_Count;
        DWORD m_DataUsage;
        Mesh& m_Mesh;
      };

      struct MetadataType
      {
        enum
        {
          STRING,
          INTEGER,
          FLOAT,
          DOUBLE,
          DATETIME,
          BINARY_BASE64,
          BINARY_HEX,
          BINARY_BLOB,
        };
      };

      class MetadataTable;
      class MetadataValue
      {
        friend class MetadataTable;
      public:
        MetadataValue() {}
        /* Constructs a new name-value pair */
        MetadataValue(LPCTSTR Name, LPCTSTR Value, DWORD DataType)
        { m_Name = Name; m_Value = Value; m_DataType = DataType; }

        MetadataValue(CONST MetadataValue& M) : m_Name(M.m_Name), m_Value(M.m_Value), m_DataType(M.m_DataType) {}

        /* Returns the name of the item */
        String Name() CONST
        { return m_Name; }
        /* Returns the raw value of the item */
        String Value() CONST
        { return m_Value; }
        /* Returns the type of the item */
        DWORD Type() CONST
        { return m_DataType; }
        /* Returns the type of the item */
        String TypeString() CONST;
      protected:
        String m_Name;
        String m_Value;
        DWORD m_DataType;
      };

      class MetadataTable
      {
        friend class Mesh;
      public:
        /* Returns TRUE iff there is a MetadataValue associated witha given name */
        BOOL Contains(LPCTSTR Name);

        /* Returns the value associated with a name */
        String GetValue(LPCTSTR Name);
        /* Returns the type associated with a name */
        DWORD GetType(LPCTSTR Name);

        /* Sets the value associated with a name */
        VOID SetValue(LPCTSTR Name, LPCTSTR Value);

        /* Sets/Gets the value associated with a name */
        String& operator[] (LPCTSTR Name);

      protected:
        Map<String, MetadataValue> m_Values;
      };

    public:
      /* Creates an in memory Mesh */
      Mesh(DWORD MeshFlags);
      /* Creates or opens a Mesh */
      Mesh(LPCTSTR Filename, DWORD MeshFlags);

      virtual ~Mesh();

      /* Initializes the mesh for a certain usage pattern, returns TRUE if the mesh is in the correct storage mode */
      BOOL Begin(DWORD VertexDataUsage, DWORD TriangleDataUsage, DWORD EdgeDataUsage, BOOL ForceStorageFormat = FALSE);
      /* Finishes use of the mesh */
      VOID End();


      /* Optimizes the mesh for a certain usage pattern */
      VOID Optimize(DWORD VertexDataUsage, DWORD TriangleDataUsage, DWORD EdgeDataUsage);

      /* Imports a mesh from a foreign data format */
      VOID Import(LPCTSTR Filename);
      /* Exports the mesh to a foreign data format */
      VOID Export(LPCTSTR Filename);

      /* Flush metadata to disk */
      VOID Flush();
      /* Closes the mesh */
      VOID Close();

      Table VertexData;
      Table TriangleData;
      Table EdgeData;

      MetadataTable MetaData;

      /* The table information passed into an IterateTable callback */
      struct TABLEROW
      {
        TABLEROW(Table& T) : DataTable(T) 
        {
          PartitionCount = (UINT)T.Partitions().Length();
          TotalRowSize = 0;
          Index = 0;
          ZeroMemory(PartitionRowSize, sizeof(UINT)*Table::MAXPARTITIONS);
          ZeroMemory(pPartitionRowData, sizeof(LPVOID)*Table::MAXPARTITIONS);
        }

        /* Coalease row data from all paritions into a single row buffer */
        VOID CopyTo(LPVOID pData);
        /* Scatter a single row buffer into each partition */
        VOID CopyFrom(LPCVOID pData);

        /* The table the data belongs to */
        Table& DataTable;
        /* The number of data partitions */
        UINT PartitionCount;
        /* The total size of a row */
        UINT TotalRowSize;
        
        /* The ordinal number of the table row */
        UINT Index;
        /* An array of partition row sizes */
        UINT PartitionRowSize[Table::MAXPARTITIONS];
        /* An array of partition data pointers */
        LPBYTE pPartitionRowData[Table::MAXPARTITIONS];
      };

      template<class FUNCTION>
      VOID IterateVertexBlocks(FUNCTION& F, UINT BlockSize)
      { VertexData.IterateBlocks(F, BlockSize, 0, VertexData.GetCount()); }
      template<class FUNCTION>
      VOID IterateVertices(FUNCTION& F)
      { IterateVertices(F, 0, VertexData.GetCount()); }
      template<class FUNCTION>
      VOID IterateVertices(FUNCTION& F, UINT Offset, UINT Length)
      { VertexData.Iterate(F, Offset, Length); }

      template<class FUNCTION>
      VOID IterateVertices(FUNCTION& F, CONST Attribute& Attribute0)
      { IterateVertices(F, VertexData.GetAttributeId(Attribute0), 0, VertexData.GetCount()); }
      template<class FUNCTION>
      VOID IterateVertices(FUNCTION& F, CONST Attribute& Attribute0, UINT Offset, UINT Length)
      { IterateVertices(F, VertexData.GetAttributeId(Attribute0), Offset, Length); }
      template<class FUNCTION>
      VOID IterateVertices(FUNCTION& F, UINT AttributeId0, UINT Offset, UINT Length)
      { VertexData.Iterate(F, AttributeId0, Offset, Length); }

      template<class FUNCTION>
      VOID IterateVertices(FUNCTION& F, CONST Attribute& Attribute0, CONST Attribute& Attribute1)
      { IterateVertices(F, VertexData.GetAttributeId(Attribute0), VertexData.GetAttributeId(Attribute1), 0, VertexData.GetCount()); }
      template<class FUNCTION>
      VOID IterateVertices(FUNCTION& F, CONST Attribute& Attribute0, CONST Attribute& Attribute1, UINT Offset, UINT Length)
      { IterateVertices(F, VertexData.GetAttributeId(Attribute0), VertexData.GetAttributeId(Attribute1), Offset, Length); }
      template<class FUNCTION>
      VOID IterateVertices(FUNCTION& F, UINT AttributeId0, UINT AttributeId1, UINT Offset, UINT Length)
      { VertexData.Iterate(F, AttributeId0, AttributeId1, Offset, Length); }

      template<class FUNCTION>
      VOID IterateTriangles(FUNCTION& F, CONST Attribute& Attribute0)
      { IterateTriangles(F, TriangleData.GetAttributeId(Attribute0), 0, TriangleData.GetCount()); }
      template<class FUNCTION>
      VOID IterateTriangles(FUNCTION& F, CONST Attribute& Attribute0, UINT Offset, UINT Length)
      { IterateTriangles(F, TriangleData.GetAttributeId(Attribute0), Offset, Length); }
      template<class FUNCTION>
      VOID IterateTriangles(FUNCTION& F, UINT AttributeId0, UINT Offset, UINT Length)
      { TriangleData.Iterate(F, AttributeId0, Offset, Length); }

      template<class FUNCTION>
      VOID IterateTriangles(FUNCTION& F, CONST Attribute& Attribute0, CONST Attribute& Attribute1)
      { IterateTriangles(F, TriangleData.GetAttributeId(Attribute0), TriangleData.GetAttributeId(Attribute1), 0, TriangleData.GetCount()); }
      template<class FUNCTION>
      VOID IterateTriangles(FUNCTION& F, CONST Attribute& Attribute0, CONST Attribute& Attribute1, UINT Offset, UINT Length)
      { IterateTriangles(F, TriangleData.GetAttributeId(Attribute0), TriangleData.GetAttributeId(Attribute1), Offset, Length); }
      template<class FUNCTION>
      VOID IterateTriangles(FUNCTION& F, UINT AttributeId0, UINT AttributeId1, UINT Offset, UINT Length)
      { TriangleData.Iterate(F, AttributeId0, AttributeId1, Offset, Length); }

      template<class FUNCTION>
      VOID IterateEdges(FUNCTION& F, CONST Attribute& Attribute0)
      { IterateEdges(F, EdgeData.GetAttributeId(Attribute0), 0, EdgeData.GetCount()); }
      template<class FUNCTION>
      VOID IterateEdges(FUNCTION& F, CONST Attribute& Attribute0, UINT Offset, UINT Length)
      { IterateEdges(F, EdgeData.GetAttributeId(Attribute0), Offset, Length); }
      template<class FUNCTION>
      VOID IterateEdges(FUNCTION& F, UINT AttributeId0, UINT Offset, UINT Length)
      { EdgeData.Iterate(F, AttributeId0, Offset, Length); }

      template<class FUNCTION>
      VOID IterateEdges(FUNCTION& F, CONST Attribute& Attribute0, CONST Attribute& Attribute1)
      { IterateEdges(F, EdgeData.GetAttributeId(Attribute0), EdgeData.GetAttributeId(Attribute1), 0, EdgeData.GetCount()); }
      template<class FUNCTION>
      VOID IterateEdges(FUNCTION& F, CONST Attribute& Attribute0, CONST Attribute& Attribute1, UINT Offset, UINT Length)
      { IterateEdges(F, EdgeData.GetAttributeId(Attribute0), EdgeData.GetAttributeId(Attribute1), Offset, Length); }
      template<class FUNCTION>
      VOID IterateEdges(FUNCTION& F, UINT AttributeId0, UINT AttributeId1, UINT Offset, UINT Length)
      { EdgeData.Iterate(F, AttributeId0, AttributeId1, Offset, Length); }

      template<class FUNCTION>
      VOID IterateTriangleVertices(FUNCTION& F, CONST Attribute& Attribute0)
      { IterateTriangleVertices(F, Attribute0, 0, TriangleData.GetCount()); }
      template<class FUNCTION>
      VOID IterateTriangleVertices(FUNCTION& F, CONST Attribute& Attribute0, UINT Offset, UINT Length)
      { IterateTriangleVertices(F, VertexData.GetAttributeId(Attribute0), Offset, Length); }
      template<class FUNCTION>
      VOID IterateTriangleVertices(FUNCTION& F, UINT AttributeId0, UINT Offset, UINT Length);

      template<class FUNCTION>
      VOID IterateTriangleVertices(FUNCTION& F, CONST Attribute& Attribute0, CONST Attribute& Attribute1)
      { IterateTriangleVertices(F, Attribute0, Attribute1, 0, TriangleData.GetCount()); }
      template<class FUNCTION>
      VOID IterateTriangleVertices(FUNCTION& F, CONST Attribute& Attribute0, CONST Attribute& Attribute1, UINT Offset, UINT Length)
      { IterateTriangleVertices(F, VertexData.GetAttributeId(Attribute0), VertexData.GetAttributeId(Attribute1), Offset, Length); }
      template<class FUNCTION>
      VOID IterateTriangleVertices(FUNCTION& F, UINT AttributeId0, UINT AttributeId1, UINT Offset, UINT Length);

      /* Sorts triangle data by a given comparison operator */
      template<class PARTITIONFUNCTION>
      VOID SortTriangleData(CONST Attribute& Attribute, PARTITIONFUNCTION& PartitionFunction)
      { SortTriangleData(TriangleData.GetAttributeId(Attribute), PartitionFunction); }
      template<class PARTITIONFUNCTION>
      VOID SortTriangleData(UINT AttributeId, PARTITIONFUNCTION& PartitionFunction)
      { TriangleData.Sort(AttributeId, PartitionFunction); }

      /* Sorts vertex data by a given comparison operator */
      template<class PARTITIONFUNCTION>
      VOID SortVertexData(CONST Attribute& Attribute, PARTITIONFUNCTION& PartitionFunction)
      { SortVertexData(VertexData.GetAttributeId(Attribute), PartitionFunction); }
      template<class PARTITIONFUNCTION>
      VOID SortVertexData(UINT AttributeId, PARTITIONFUNCTION& PartitionFunction);

      /* Computes normals for a Mesh */
      VOID ComputeNormals();
      /* Computes Mesh statistics */
      VOID ComputeStatistics();

      class OctreeEnumerator
      {

      };

      class OctreeNode
      {
      public:
        OctreeNode();

        /* Returns a pointer to the parent node */
        CONST OctreeNode* GetParent() CONST
        { return m_pParent; }
        OctreeNode* GetParent()
        { return m_pParent; }

        /* Returns TRUE iff a node has children */
        BOOL HasChildren() CONST
        { return m_pChildren != 0; }

        /* Returns a pointer to a child node */
        CONST OctreeNode* GetChild(UINT i) CONST
        { return &m_pChildren[i]; }
        OctreeNode* GetChild(UINT i)
        { return &m_pChildren[i]; }

        /* Creates children if they do not exist */
        OctreeNode* CreateChildren();

        /* Returns the spatial extent of the nodes vertex data */
        CONST AxisAlignedBox3F& GetVertexBounds() CONST
        { return m_VertexBounds; }
        /* Returns the spatial extent of the nodes triangle data */
        CONST AxisAlignedBox3F& GetTriangleBounds() CONST
        { return m_TriangleBounds; }
        /* Returns the number of vertices contained in the node */
        UINT GetVertexCount() CONST
        { return m_VertexCount; }
        /* Returns the offset of vertex data contained in the node */
        UINT GetVertexOffset() CONST
        { return m_VertexOffset; }
        /* Returns the number of triangles contained in the node */
        UINT GetTriangleCount() CONST
        { return m_TriangleCount; }
        /* Returns the offset of triangle data contained in the node */
        UINT GetTriangleOffset() CONST
        { return m_TriangleOffset; }

        /* Returns the spatial extent of the nodes vertex data */
        AxisAlignedBox3F& VolumeBounds()
        { return m_VolumeBounds; }
        /* Returns the spatial extent of the nodes vertex data */
        AxisAlignedBox3F& VertexBounds()
        { return m_VertexBounds; }
        /* Returns the spatial extent of the nodes triangle data */
        AxisAlignedBox3F& TriangleBounds()
        { return m_TriangleBounds; }
        /* Returns the number of vertices contained in the node */
        UINT& VertexCount()
        { return m_VertexCount; }
        /* Returns the offset of vertex data contained in the node */
        UINT& VertexOffset()
        { return m_VertexOffset; }
        /* Returns the number of triangles contained in the node */
        UINT& TriangleCount()
        { return m_TriangleCount; }
        /* Returns the offset of triangle data contained in the node */
        UINT& TriangleOffset()
        { return m_TriangleOffset; }

        /* Sets the vertex span and extent of the node */
        VOID SetVertexBounds(UINT Offset, UINT Count, CONST AxisAlignedBox3F& SpatialBound)
        { m_VertexOffset = Offset; m_VertexCount = Count; m_VertexBounds = SpatialBound; }

        /* Aggregates information from leaf nodes into internal nodes */
        VOID AggregateTree();


      private:
        OctreeNode* m_pChildren;
        OctreeNode* m_pParent;

        AxisAlignedBox3F m_VertexBounds;
        AxisAlignedBox3F m_TriangleBounds;
        AxisAlignedBox3F m_VolumeBounds;
        UINT m_VertexCount;
        UINT m_VertexOffset;
        UINT m_TriangleCount;
        UINT m_TriangleOffset;
      };

      class SliceQuadtreeNode
      {
      };

      /* Constructs a octree index structure on the vertex data */
      VOID ConstructOctreeIndex(UINT Resolution)
      { ConstructOctreeIndex(Resolution, GetBoundingBox()); }
      VOID ConstructOctreeIndex(UINT Resolution, CONST AxisAlignedBox3F& BoundingBox);
      
      /* Returns a pointer to the root of the octree index */
      CONST OctreeNode* GetOctreeIndexRoot() CONST
      { return m_pOctreeIndex; }

      /* Constructs a per slice quadtree index structure on the vertex data */
      VOID ConstructSliceQuadtreeIndex(UINT XResolution, UINT YZResolution)
      { ConstructSliceQuadtreeIndex(XResolution, YZResolution, GetBoundingBox()); }
      VOID ConstructSliceQuadtreeIndex(UINT XResolution, UINT YZResolution, CONST AxisAlignedBox3F& BoundingBox);

      /* Sets the ZINDEX attribute for each vertex based upon an octree index*/
      VOID SetVertexZCurveIndex(UINT Resolution)
      { SetVertexZCurveIndex(Resolution, GetBoundingBox()); }
      VOID SetVertexZCurveIndex(UINT Resolution, CONST AxisAlignedBox3F& BoundingBox);

      /* Sets the ZINDEX attribute for each vertex based upon a per slice quadtree index */
      VOID SetVertexSliceZCurveIndex(UINT XResolution, UINT YZResolution)
      { SetVertexSliceZCurveIndex(XResolution, YZResolution, GetBoundingBox()); }
      VOID SetVertexSliceZCurveIndex(UINT Resolution, UINT YZResolution, CONST AxisAlignedBox3F& BoundingBox);

      /* Sets the INDEX attribute for each vertex */
      VOID SetVertexIndices();

      /* Remaps triangle indeices after sorting vertex data */
      VOID RemapTriangleIndices();

      /* Computes and returns the axis-aligned bounding box of the mesh */
      CONST AxisAlignedBox3F& GetBoundingBox();

      /* Computes and returns the bounding sphere of the mesh */
      CONST Sphere3F& GetBoundingSphere();
    private:
      /* The flags used to open/create the Mesh object */
      DWORD m_Flags;
      /* The base filename of the mesh */
      String m_Filename;

      /* Mesh statistics */
      Matrix3F m_Covariance;
      Vector3F m_CenterOfMass;
      AxisAlignedBox3F m_BoundingBox;
      Sphere3F m_BoundingSphere;
      FLOAT m_FeatureSize;
      DWORD m_StatisticsFlags;
      FLOAT ComputeApproximateFeatureSize();

      DWORD m_VertexDataUsage;
      DWORD m_TriangleDataUsage;
      DWORD m_EdgeDataUsage;

      /* The meta-data */
      Map<String, MetadataValue> m_MetaData;

      /* Creates a new Mesh structure on disk */
      VOID Create();

      /* The octree index */
      OctreeNode* m_pOctreeIndex;

      /* Read XML headers */
      VOID ReadHeader();
      VOID ReadMetadata(Xml::XmlReader& Reader);
      VOID ReadVertexHeader(Xml::XmlReader& Reader)
      { ReadTableHeader(Reader, &VertexData); }
      VOID ReadTriangleHeader(Xml::XmlReader& Reader)
      { ReadTableHeader(Reader, &TriangleData); }
      VOID ReadEdgeHeader(Xml::XmlReader& Reader)
      { ReadTableHeader(Reader, &EdgeData); }
      VOID ReadTableHeader(Xml::XmlReader& Reader, Table* pTable);
      VOID ReadTableAttributeHeader(Xml::XmlReader& Reader, Partition& Partition);
      VOID ReadTableStorageHeader(Xml::XmlReader& Reader, Table* pTable);
      VOID ReadTableIndexHeader(Xml::XmlReader& Reader, Table* pTable);

      /* Write XML header */
      VOID WriteHeader();

      /* Import foreign data formats */
      VOID ImportBNPTS(LPCTSTR Filename);
      VOID ImportPLY(LPCTSTR Filename);
      VOID ReadPlyVerticesBinary(Ptr<IO::Stream> pStream);
      VOID ReadPlyTrianglesBinary(Ptr<IO::Stream> pStream);
      VOID ReadPlyFacesBinary(Ptr<IO::Stream> pStream);

      VOID ExportBNPTS(LPCTSTR Filename);
      VOID ExportPLY(LPCTSTR Filename);
      VOID ExportLXS(LPCTSTR Filename);

      DWORD m_StorageFormat;
      BOOL EvaluateUsage(DWORD VertexDataUsage, DWORD TriangleDataUsage, DWORD EdgeDataUsage, DWORD& IdealStorageFormat);

      /* Returns a reference to a data storage container */
      TableDataStorage* GetStore(LPCTSTR Name);
      String m_DataPath;

    };


    struct VertexAttribute
    {
      /* Vertex Attributes */
      static const Mesh::Attribute POSITION3;
      static const Mesh::Attribute POSITION2;
      static const Mesh::Attribute NORMAL2;
      static const Mesh::Attribute NORMAL3;
      static const Mesh::Attribute DIFFUSE;
      static const Mesh::Attribute EDGE;
      static const Mesh::Attribute MEANCURVATURE;
      static const Mesh::Attribute LOCALDEPTH;
      static const Mesh::Attribute ACCESSIBILITY;
      static const Mesh::Attribute FREE1;
      static const Mesh::Attribute FREE4;
      static const Mesh::Attribute FREE8;
      static const Mesh::Attribute FREE12;
      static const Mesh::Attribute FREE16;
      static const Mesh::Attribute FREE32;
      static const Mesh::Attribute INDEX;
      static const Mesh::Attribute ZINDEX;
      static const Mesh::Attribute SLICEZINDEX;
      static const Mesh::Attribute FLAGS;
    };

    struct TriangleAttribute
    {
      /* Triangle Attributes */
      static const Mesh::Attribute VERTICES;
      static const Mesh::Attribute EDGES;
    };

    struct EdgeAttribute
    {
      /* Edge Attributes */
      static const Mesh::Attribute VERTICES;
      static const Mesh::Attribute TRIANGLES;
      static const Mesh::Attribute ADJACENTEDGES;
    };

    #include "MeshImpl.hpp"
  }
}
