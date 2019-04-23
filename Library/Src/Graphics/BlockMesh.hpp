/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/BlockMesh.hpp $
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

#pragma once

#include "IO/BlockStore.hpp"
#include "IO/MemoryStream.hpp"
#include "Collections/Map.hpp"

#include "Utility/FuncPtr.hpp"
#include "Utility/Progress.hpp"
#include "Utility/ElementDecl.hpp"

#include "Math/AxisAlignedBox.hpp"
#include "Math/Sphere.hpp"
#include "Math/Line.hpp"
#include "Math/Matrix3.hpp"
#include "Math/Matrix4.hpp"
#include "Math/Histogram.hpp"
#include "Math/Geometry/Types.hpp"
#include "Lua/Binding.hpp"
#include "Graphics/Image.hpp"
#include "Collections/Stack.hpp"

namespace Bolitho
{
  namespace Graphics
  {
    using Math::Sphere3F;
    using Math::AxisAlignedBox3F;

    using Math::Vector3F;
    using Math::Matrix3F;
    using Math::Matrix4F;

    using Math::Geometry::INDEXEDTRIANGLE;

    class LIBRARY_API MeshException : public UserException
    {
      DeclareRuntimeType();
    public:
      MeshException(CONST Object* Sender, LPCTSTR Description) : UserException(Sender)
      { m_Description = Description; }

      MeshException(CONST MeshException& E) : UserException(E)
      { m_Description = E.m_Description; }
     
      virtual String Description() CONST
      { return m_Description; }

    private:
      String m_Description;
    };

    struct VertexAttribute
    {
      /* Vertex Attributes */
      static const ElementDecl POSITION3;
      static const ElementDecl POSITION2;
      static const ElementDecl NORMAL2;
      static const ElementDecl NORMAL3;
      static const ElementDecl DIFFUSE;
      static const ElementDecl EDGE;
      static const ElementDecl MEANCURVATURE;
      static const ElementDecl LOCALDEPTH;
      static const ElementDecl ACCESSIBILITY;
      static const ElementDecl FREE1;
      static const ElementDecl FREE4;
      static const ElementDecl FREE8;
      static const ElementDecl FREE12;
      static const ElementDecl FREE16;
      static const ElementDecl FREE32;
      static const ElementDecl INDEX;
      static const ElementDecl ZINDEX;
      static const ElementDecl SLICEZINDEX;
      static const ElementDecl FLAGS;
    };

    struct TriangleAttribute
    {
      /* Triangle Attributes */
      static const ElementDecl VERTICES;
      static const ElementDecl EDGES;
    };

    struct EdgeAttribute
    {
      /* Edge Attributes */
      static const ElementDecl VERTICES;
      static const ElementDecl TRIANGLES;
      static const ElementDecl ADJACENTEDGES;
    };

    struct IndexType
    { enum { NONE, ZINDEX, SLICEZINDEX, AXIALINDEX }; };

    /* A simple geometric BlockMesh */
    class LIBRARY_API BlockMesh : public Object
    {
      DeclareRuntimeType();
    protected:
      static CONST UINT32 MESHVERSION = 0x0224;
      static CONST UINT64 MESHSIGNATURE = 0x202020204853454d;

      struct ELEMENTDECL
      {
        CHAR Name[32];
        DWORD Type;
        UINT Length;
      };

      struct BLOCKTABLE
      {
        UINT Directory;
        UINT BlockCount;
      };

      struct TABLEHEADER
      {
        BLOCKTABLE Storage;
        UINT CountPerBlock;
        UINT Count;
        UINT ElementSize;
        UINT AttributeCount;
        ELEMENTDECL ElementDecl[16];
      };

      struct BLOB
      {
        CHAR Name[32];
        CHAR Type[32];
        ULONGLONG Length;
        BLOCKTABLE Storage;
      };

      struct MESHSTATISTICS
      {
        Sphere3F BoundingSphere;
        AxisAlignedBox3F BoundingBox;
        Vector3F CenterOfMass;
        Matrix3F Covariance;
        FLOAT FeatureSize;
      };

      struct MESHHEADER
      {
        UINT64 Signature;
        UINT32 Version;

        TABLEHEADER VertexDataDecl;
        TABLEHEADER TriangleDataDecl;
        TABLEHEADER EdgeDataDecl;

        BLOCKTABLE MetaDataStorage;

        BLOB Blobs[8];

        BLOCKTABLE IndexStorage;
        DWORD IndexType;
        UINT IndexResolution;

        DWORD StatisticsFlags;
        MESHSTATISTICS Statistics;
      };

    public:
      struct INDEXNODE
      {
        INDEXNODE();

        INDEXNODE* pChildren;
        AxisAlignedBox3F VolumeBounds;

        UINT VertexDataOffset;
        UINT VertexDataCount;
        AxisAlignedBox3F VertexDataBounds;

        UINT TriangleDataOffset;
        UINT TriangleDataCount;
        AxisAlignedBox3F TriangleDataBounds;
      };

      struct TRIANGLEBLOCK
      {
        UINT MinimumVertexIndex;
        UINT MaximumVertexIndex;
      };

      struct VERTEXBLOCK
      {
        UINT MinimumTriangleIndex;
        UINT MaximumTriangleIndex;
      };

    protected:

    #include "BlockMeshTable.hpp"

    public:
      virtual ~BlockMesh();

      /* Create an in-memory BlockMesh */
      BlockMesh();
      /* Create/Open a persistant BlockMesh */
      BlockMesh(LPCTSTR Filename, DWORD FileMode);

      /* Import a BlockMesh from another format.  
       * The format is determined by filename extension 
       */
      VOID Import(LPCTSTR Filename);

      /* Export the BlockMesh to another format.  
       * The format is determined by filename extension 
       */
      VOID Export(LPCTSTR Filename);

      /* Flushes all dirty data to disk */
      VOID Flush();

      /* Frees all memory used to store the BlockMesh */
      VOID Close();

      /* The data tables */
      DataTable VertexData;
      DataTable TriangleData;
      DataTable EdgeData;

      class OctreeIndexEnumerator
      {
        friend class BlockMesh;
      public:
        OctreeIndexEnumerator(CONST OctreeIndexEnumerator& E)
        { m_pNode = E.m_pNode; m_Parent = E.m_Parent; }
        OctreeIndexEnumerator& operator= (CONST OctreeIndexEnumerator& E)
        { m_pNode = E.m_pNode; m_Parent = E.m_Parent; return *this; }

        CONST INDEXNODE* operator-> ()
        { return m_pNode; }
        CONST INDEXNODE* GetCurrentNode()
        { return m_pNode; }
        CONST INDEXNODE* Next();

        VOID Advance(UINT i);
      protected:
        OctreeIndexEnumerator(CONST INDEXNODE* pRoot);
        CONST INDEXNODE* m_pNode;
        Stack<CONST INDEXNODE*> m_Parent;
      };

      /* Returns an octree enumerator */
      OctreeIndexEnumerator GetIndexEnumerator();

      /* The Spatial Index Root */
      CONST INDEXNODE* GetIndexRoot() CONST
      { return (CONST INDEXNODE*)m_pIndex; }

      /* Gets the Spatial Index Type */
      DWORD GetIndexType() CONST
      { return m_pHeader->IndexType; }

      /* Gets the Spatial Index Resolution */
      UINT GetIndexResolution() CONST
      { return m_pHeader->IndexResolution; }

      /* Adds a metadata value to the BlockMesh */
      VOID SetMetaData(LPCTSTR Key, LPCTSTR Value);

      /* Gets a metadata value */
      String GetMetaData(LPCTSTR Key);

      /* Removes a metadata value */
      VOID RemoveMetaData(LPCTSTR Key);

      /* Removes all metadata */
      VOID RemoveAllMetaData();

      /* Adds a blob */
      VOID CreateBlob(LPCTSTR Name, SIZE_T Length, LPCVOID pData);
      VOID CreateBlob(LPCTSTR Name, LPCTSTR Filename);
      VOID CreateBlob(LPCTSTR Name, Ptr<IO::File> pFile);
      VOID CreateBlob(LPCTSTR Name, ULONGLONG Length, Ptr<IO::Stream> pStream);

      /* Returns TRUE off a blob exists */
      BOOL BlobExists(LPCTSTR Name);

      /* Returns the length of a blob */
      ULONGLONG GetBlobLength(LPCTSTR Name);

      /* Maps a blob into memory */
      Ptr<IO::Stream> GetBlobDataStream(LPCTSTR Name);

      /* Loads a blob's data into a user supplied buffer */
      VOID GetBlobData(LPCTSTR Name, SIZE_T Length, LPVOID pData);

      /* Sets a blobs data from a user supplied buffer */
      VOID SetBlobData(LPCTSTR Name, SIZE_T Length, LPCVOID);

      /* Deletes a blob */
      VOID DeleteBlob(LPCTSTR Name);

      /* Computes topological information */
      VOID ComputeTopology();

      /* Computes BlockMesh statistics */
      VOID ComputeStatistics();

      /* Computes normals for a BlockMesh */
      VOID ComputeNormals();

      /* Sorts the vertex and triangle data according to a Z-curve */
      VOID SortZCurve(UINT Resolution);

      /* Sorts the vertes and triangle data according to a x-slice and then z-curve within slices */
      VOID SortSliceZCurve(UINT Resolution);

      /* Sorts the vertex and triangle data by x-axis */
      VOID SortXAxis();

      /* Constructs an octree-based spatial index */
      VOID ConstructOctreeIndex(UINT Resolution);

      /* Constructs an axial sort based spatial index */
      VOID ConstructAxialSortIndex(UINT Resolution);

      /* Constructs vertex span information */
      VOID ComputeTriangleBlockInfo(UINT BlockSize);
      VOID ComputeVertexBlockInfo(UINT BlockSize);

      /* Calls ProcessTriangle on a given function for each triangle in the BlockMesh */
      template<class FUNCTION>
      VOID IterateTriangles(FUNCTION& F);
      template<class FUNCTION>
      VOID IterateTriangles(FUNCTION& F, CONST ElementDecl& Attribute);
      template<class FUNCTION>
      VOID IterateTriangles(FUNCTION& F, UINT Offset);
      template<class FUNCTION>
      VOID IterateTriangles(FUNCTION& F, CONST ElementDecl& Attribute0, CONST ElementDecl& Attribute1);
      template<class FUNCTION>
      VOID IterateTriangles(FUNCTION& F, UINT Offset0, UINT Offset1);
      template<class FUNCTION>
      VOID IterateTriangleBlocks(FUNCTION& F);

      /* Calls ProcessVertex on a given function for each vertex in the BlockMesh */
      template<class FUNCTION>
      VOID IterateVertices(FUNCTION& F);
      template<class FUNCTION>
      VOID IterateVertices(FUNCTION& F, CONST ElementDecl& Attribute);
      template<class FUNCTION>
      VOID IterateVertices(FUNCTION& F, UINT Offset);
      template<class FUNCTION>
      VOID IterateVertices(FUNCTION& F, CONST ElementDecl& Attribute0, CONST ElementDecl& Attribute1);
      template<class FUNCTION>
      VOID IterateVertices(FUNCTION& F, UINT Offset0, UINT Offset1);
      template<class FUNCTION>
      VOID IterateVertexBlocks(FUNCTION& F);

      /* Calls ProcessTriangle on a given function for each triangle in the BlockMesh, dereferencing vertex data */
      template<class FUNCTION>
      VOID IterateTriangleVertices(FUNCTION& F, CONST ElementDecl& Attribute0);
      template<class FUNCTION>
      VOID IterateTriangleVertices(FUNCTION& F, UINT Offset0);
      template<class FUNCTION>
      VOID IterateTriangleVertices(FUNCTION& F, CONST ElementDecl& Attribute0, CONST ElementDecl& Attribute1);
      template<class FUNCTION>
      VOID IterateTriangleVertices(FUNCTION& F, UINT Offset0, UINT Offset1);

      /* Sorts vertex data by a given comparison operator */
      template<class PARTITIONFUNCTION, class SORTFUNCTION>
      VOID SortVertexData(CONST ElementDecl& Attribute, PARTITIONFUNCTION& ParitionFunction, SORTFUNCTION& SortFunction);
      template<class PARTITIONFUNCTION, class SORTFUNCTION>
      VOID SortVertexData(UINT Offset, PARTITIONFUNCTION& ParitionFunction, SORTFUNCTION& SortFunction);

      /* Sorts triangle data by a given comparison operator */
      template<class PARTITIONFUNCTION, class SORTFUNCTION>
      VOID SortTriangleData(CONST ElementDecl& Attribute, PARTITIONFUNCTION& ParitionFunction, SORTFUNCTION& SortFunction);
      template<class PARTITIONFUNCTION, class SORTFUNCTION>
      VOID SortTriangleData(UINT Offset, PARTITIONFUNCTION& ParitionFunction, SORTFUNCTION& SortFunction);

      /* Generate Lua bindings for the BlockMesh class */
      #if defined(USE_LUA)
      static Lua::Bind::Scope GenerateBindings(LPCTSTR Name);
      #endif

      Sphere3F GetBoundingSphere();
      AxisAlignedBox3F GetBoundingBox();

      UINT GetTriangleInfoBlockSize()
      { return m_TriangleBlockInfoResolution; }
      TRIANGLEBLOCK GetTriangleBlockInfo(UINT i)
      { return m_pTriangleBlockInfo[i]; }
    protected:
      /* The spatial index */
      INDEXNODE* m_pIndex;

      /* Triangle block info */
      TRIANGLEBLOCK* m_pTriangleBlockInfo;
      UINT m_TriangleBlockInfoResolution;

      /* The BlockMesh file */
      IO::BlockStore m_BlockStore;

      STATIC CONST UINT BLOCKSIZE = 256*1024;

      /* The BlockMesh header */
      MESHHEADER* m_pHeader;

      /* Initialize Storage */
      VOID InitializeInMemory();
      VOID InitializeTables();
      VOID InitializeOpen(LPCTSTR Filename, DWORD Flags);
      VOID InitializeCreate(LPCTSTR Filename);

      /* A collection of key/value pairs to store custom metadata */
      Map<String,String> m_MetaData;

      /* Called to load the metadata from the file */
      VOID LoadMetaData();
      /* Saves all metadata into the file */
      VOID SaveMetaData();

      /* Blob helpers */
      UINT GetBlobIndex(LPCTSTR Name);
      UINT AllocateBlob(LPCTSTR Name);

      /* Loads a PLY File */
      VOID ImportPLY(LPCTSTR Filename);
      VOID ReadPlyVerticesBinary(Ptr<IO::Stream> pStream);
      VOID ReadPlyTrianglesBinary(Ptr<IO::Stream> pStream);
      VOID ReadPlyFacesBinary(Ptr<IO::Stream> pStream);

      /* Imports a BNPTS file */
      VOID ImportBNPTS(LPCTSTR Filename);

      /* Compute the approximate feature size */
      FLOAT ComputeApproximateFeatureSize();

      /* Creates the INDEX VertexAttribute and initializes the values */
      VOID SetVertexIndices();

      /* Creates the ZINDEX VertexAttribute and initialized the value */
      VOID SetVertexZCurveIndex(UINT Resolution);

      /* Creates the SLICEZINDEX VertexAttribute and initialized the value */
      VOID SetVertexSliceZCurveIndex(UINT Resolution);

      /* Sorts triangle data */
      VOID SortTriangleData();

    };


    #include "BlockMeshImpl.hpp"

  }
}
