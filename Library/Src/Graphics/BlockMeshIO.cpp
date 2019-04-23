/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/BlockMeshIO.cpp $
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

#include "Graphics/BlockMesh.hpp"
#include "Graphics/MeshPly.hpp"

#include "IO/File.hpp"
#include "IO/Path.hpp"
#include "IO/TextStream.hpp"

using namespace Bolitho;
using namespace Bolitho::Math;
using namespace Bolitho::Math::Geometry;
using namespace Bolitho::Graphics;
using namespace Bolitho::IO;

//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::ImportBNPTS(LPCTSTR Filename)
{
  Ptr<File> pStream = File::Open(Filename, FileMode::OPEN, FileAccess::READ);
  UINT VertexCount = (UINT)(pStream->Length() / 24);

  VertexData.Resize(0);
  VertexData.RemoveAllAttributes();
  TriangleData.Resize(0);
  TriangleData.RemoveAllAttributes();

  VertexData.AddAttribute(VertexAttribute::POSITION3);
  VertexData.AddAttribute(VertexAttribute::NORMAL3);

  VertexData.Resize(VertexCount);

  ReadPlyVerticesBinary(pStream);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::ImportPLY(LPCTSTR Filename)
{
  using namespace Internal;
  
  Ptr<File> pStream = File::Open(Filename, FileMode::OPEN, FileAccess::READ);
  pStream->SetBufferSize(1048576);

  Array<PlyElement*> Elements;
  Array<INT> RangeImage;

  SIZE_T RangeImageWidth = 0;
  SIZE_T RangeImageHeight = 0;

  PlyElement* pCurrentElement = 0;
  enum { Ascii, BinaryBigEndian, BinaryLittleEndian } Format = Ascii;

  TextStream S = TextStream(pStream);
  if (S.ReadLine() != "ply")
      throw InvalidFormatException(this, TEXT("Ply header not found"));
  
  while(!S.EndOfStream())
  {
    String L = S.ReadLine();
    if (L.Length() == 0)
      continue;

    Array<String> A = L.Split(&String::IsWhitespace);
    
    if (A[0] == "format")
    {
      if (A[1] == "ascii")
        Format = Ascii;
      if (A[1] == "binary_big_endian")
        Format = BinaryBigEndian;
      if (A[1] == "binary_little_endian")
        Format = BinaryLittleEndian;
      if (A[2] != "1.0")
        throw InvalidFormatException(this, String::Format(TEXT("Ply version is %s.  Only version 1.0 is supported"), A[2].ConstStr()));
    }

    if (A[0] == "property")
      if (A[1] == "list")
        pCurrentElement->AddPropertyList( A[4], A[2], A[3]);
      else
        pCurrentElement->AddProperty(A[2], A[1]);

    if (A[0] == "element")
      pCurrentElement = Elements.Add(new PlyElement(A[1], Parse<LONG>(A[2])));

    if (A[0] == "obj_info")
    {
      if (A[1] == "num_rows")
        RangeImageHeight = Parse<SIZE_T>(A[2]);
      if (A[1] == "num_cols")
        RangeImageWidth  = Parse<SIZE_T>(A[2]);
    }

    if (A[0] == "end_header")
      break;
  }
  
  // We need at least "vertex" elements
  pCurrentElement = 0;
  for(SIZE_T i=0; i<Elements.Length(); i++)
    if (Elements[i]->Name() == "vertex")
      pCurrentElement = Elements[i];

  if (pCurrentElement == 0)
    throw InvalidFormatException(this, TEXT("No vertex element was found"));

  VertexData.Resize(0);
  VertexData.RemoveAllAttributes();
  TriangleData.Resize(0);
  TriangleData.RemoveAllAttributes();
    
  // Now define schema
  for(SIZE_T i=0; i<Elements.Length(); i++)
  {
    if (Elements[i]->Name() == "vertex")
    {
      INT VertexOffset = -1;
      INT NormalOffset = -1;
      
      CONST Array<PlyProperty*>& P = Elements[i]->Properties();
      
      for (SIZE_T j=0; j<P.Length(); j++)
      {
        if (P[j]->Name() == "x")
        {
          if (j-P.Length() > 1 && P[j+1]->Name() == "y")
            if (j-P.Length() > 2 && P[j+2]->Name() == "z")
              VertexData.AddAttribute(VertexAttribute::POSITION3);
            else
              VertexData.AddAttribute(VertexAttribute::POSITION2);
        }
        else if (P[j]->Name() == "nx")
        {
          if (j-P.Length() > 1 && P[j+1]->Name() == "ny")
          {
            if (j-P.Length() > 2 && P[j+2]->Name() == "nz")
              VertexData.AddAttribute(VertexAttribute::NORMAL3);
            else
              VertexData.AddAttribute(VertexAttribute::NORMAL2);
          }
        }
        else if (P[j]->Name() == "diffuse_red" || P[j]->Name() == "red")
        {
          if (j-P.Length() > 1 && (P[j+1]->Name() == "diffuse_green" || P[j+1]->Name() == "green"))
            if (j-P.Length() > 2 && (P[j+2]->Name() == "diffuse_blue" || P[j+2]->Name() == "blue"))
              VertexData.AddAttribute(VertexAttribute::DIFFUSE);
        }
        else if (P[j]->Name() == "curvature")
          VertexData.AddAttribute(VertexAttribute::MEANCURVATURE);
        else if (P[j]->Name() == "accessibility")
          VertexData.AddAttribute(VertexAttribute::ACCESSIBILITY);
        else if (P[j]->Name() == "depth")
          VertexData.AddAttribute(VertexAttribute::LOCALDEPTH);

      }

      if (Elements[i]->Size() == 0)
        throw InvalidFormatException(this, TEXT("Don't support elements with lists"));

      VertexData.Resize(Elements[i]->Count());
      ReadPlyVerticesBinary(pStream);
    }

    // Initialisation for face elements
    if (Elements[i]->Name() == "face")
    {
      if (Elements[i]->Property("vertex_indices") == 0)
        throw InvalidFormatException(this, TEXT("The \'face\' element must define the 'vertex_indices' property"));

      TriangleData.AddAttribute(TriangleAttribute::VERTICES);

      TriangleData.Resize(Elements[i]->Count());
      ReadPlyFacesBinary(pStream);
    }

    if (Elements[i]->Name() == "triangle")
    {
      TriangleData.AddAttribute(TriangleAttribute::VERTICES);
      TriangleData.Resize(Elements[i]->Count());
      ReadPlyTrianglesBinary(pStream);
    }

    if (Elements[i]->Name() == "range_grid")
    {
      if (RangeImageWidth == 0 || RangeImageHeight == 0)
        throw InvalidOperationException(this, TEXT("A \'range_grid\' element was found but the dimensions are invalid"));
      RangeImage.Resize(RangeImageWidth * RangeImageHeight);
      
      if (Elements[i]->Count() != RangeImage.Length())
        throw InvalidOperationException(this, TEXT("The \'range_grid\' element was found but the dimensions are inconsistant"));
      if (Elements[i]->Property("vertex_indices") == 0)
        throw InvalidFormatException(this, TEXT("The \'range_grid\' element must define the 'vertex_indices' property"));
    }


  }

  /*
  if (RangeImage.Length() > 0)
  {
    Index2 I(RangeImageHeight, RangeImageWidth);
    
    for(SIZE_T i=0; i<RangeImageHeight-1; i++)
    {
      for(SIZE_T j=0; j<RangeImageWidth-1; j++)
      {
        SIZE_T N = 
          (RangeImage[I(i  ,j )] >= 0) + 
          (RangeImage[I(i+1,j )] >= 0) + 
          (RangeImage[I(i  ,j+1)] >= 0) + 
          (RangeImage[I(i+1,j+1)] >= 0);
        
        if (N == 3)
        {
          // 1 Triangle
          if      (RangeImage[I(i  ,j )] < 0)
            m_Faces.Add(Face( RangeImage[I(i  ,j+1)],
                                RangeImage[I(i+1,j+1)],
                                RangeImage[I(i+1,j )]));
          else if (RangeImage[I(i+1,j )] < 0)
            m_Faces.Add(Face( RangeImage[I(i  ,j )],
                                RangeImage[I(i  ,j+1)],
                                RangeImage[I(i+1,j+1)]));
          else if (RangeImage[I(i  ,j+1)] < 0)
            m_Faces.Add(Face( RangeImage[I(i+1,j+1)],
                                RangeImage[I(i+1,j )],
                                RangeImage[I(i  ,j )]));
          else if (RangeImage[I(i+1,j+1)] < 0)
            m_Faces.Add(Face( RangeImage[I(i+1,j )],
                                RangeImage[I(i  ,j )],
                                RangeImage[I(i  ,j+1)]));
        }
        if (N == 4)
        {
          // 2 Triangles, 2 possible triangulations
          
          Vector3f A = m_Vertices[RangeImage[I(i  ,j )]] - m_Vertices[RangeImage[I(i+1,j+1)]];
          Vector3f B = m_Vertices[RangeImage[I(i+1,j )]] - m_Vertices[RangeImage[I(i  ,j+1)]];

          if (A.Norm(2) < B.Norm(2))
          {
            m_Faces.Add(Face( RangeImage[I(i  ,j )],
                                RangeImage[I(i  ,j+1)],
                                RangeImage[I(i+1,j+1)]));

            m_Faces.Add(Face( RangeImage[I(i+1,j+1)],
                                RangeImage[I(i+1,j )],
                                RangeImage[I(i  ,j )]));
            
          }
          else
          {
            m_Faces.Add(Face( RangeImage[I(i+1,j )],
                                RangeImage[I(i  ,j )],
                                RangeImage[I(i  ,j+1)]));

            m_Faces.Add(Face( RangeImage[I(i+1,j )],
                                RangeImage[I(i  ,j+1)],
                                RangeImage[I(i+1,j+1)]));
          }
        }
      }
    }
  }
  */

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
class LoadVertexDataFunction
{
public:
  LoadVertexDataFunction(UINT VertexSize, Ptr<Stream> pStream) : m_VertexSize(VertexSize), m_pStream(pStream)
  {}

  LPCTSTR GetTaskName()
  { return "Loading Vertices"; }

  VOID ProcessVertexBlock(UINT VertexIndex, UINT BlockIndex, UINT BlockRowCount, LPVOID pBlockData, LPVOID pBlock)
  {
    m_pStream->Read(pBlockData, BlockRowCount*m_VertexSize);
  }

private:
  Ptr<Stream> m_pStream;
  UINT m_VertexSize;
};
VOID BlockMesh::ReadPlyVerticesBinary(Ptr<Stream> pStream)
{
  LoadVertexDataFunction LoadVertexData(VertexData.GetElementSize(), pStream);

  IterateVertexBlocks(LoadVertexData);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::ReadPlyTrianglesBinary(Ptr<Stream> pStream)
{
  throw NotImplementedException(0);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
class ReadTrianglesFunction
{
public:
  ReadTrianglesFunction(Ptr<Stream> pStream) : m_pStream(pStream)
  {}

  DWORD GetTriangleAccess()
  { return PageAccess::READWRITE; }
  DWORD GetVertexAccess()
  { return PageAccess::NOACCESS; }
  LPCTSTR GetTaskName()
  { return "Reading Faces"; }

  typedef INDEXEDTRIANGLE ELEMENTTYPE;

  VOID ProcessTriangle(UINT i, INDEXEDTRIANGLE& T, LPVOID pTriangleData)
  {
    BYTE N;
    m_pStream->Read(&N, 1);
    m_pStream->Read(&T, sizeof(INDEXEDTRIANGLE));
  }

private:
  Ptr<Stream> m_pStream;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::ReadPlyFacesBinary(Ptr<Stream> pStream)
{
  ReadTrianglesFunction ReadFunction(pStream);
  IterateTriangles(ReadFunction, TriangleAttribute::VERTICES);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------


#if 0
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::WritePly(Ptr<Stream> pStream)
{
  TextStream S = TextStream(pStream);

/*
  S.WriteLine("ply");
  S.WriteLine("format ascii 1.0");
  S.WriteLine("comment Produced by BlockMesh::WritePly");
  S.WriteLine("element vertex %d", m_Vertices.Length());
  S.WriteLine("property float x");
  S.WriteLine("property float y");
  S.WriteLine("property float z");
  S.WriteLine("element face %d", m_Faces.Length());
  S.WriteLine("property list uTCHAR int vertex_indices");
  S.WriteLine("end_header");

  // Write out vertex elements

  for(SIZE_T i=0; i<m_Vertices.Length(); i++)
    S.WriteLine("%f %f %f", m_Vertices[i][0], m_Vertices[i][1], m_Vertices[i][2]);

  // Write out face elements

//  for(SIZE_T i=0; i<m_Faces.Length(); i++)
//    S.WriteLine("3 %d %d %d", m_Faces[i][0], m_Faces[i][1], m_Faces[i][2]);
*/

}
#endif
