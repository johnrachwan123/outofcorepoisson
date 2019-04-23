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

using namespace Bolitho;
using namespace Bolitho::Math;
using namespace Bolitho::Math::Geometry;
using namespace Bolitho::Graphics;
using namespace Bolitho::IO;

//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::ImportBNPTS(LPCTSTR Filename)
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
VOID Mesh::ImportPLY(LPCTSTR Filename)
{
  using namespace Internal;
  
  Ptr<File> pStream = File::Open(Filename, FileMode::OPEN, FileAccess::READ);
  pStream->SetBufferSize(0);

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
        pCurrentElement->AddPropertyList(A[4], A[2], A[3]);
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
          (RangeImage[I(i  ,j)] >= 0) + 
          (RangeImage[I(i+1,j)] >= 0) + 
          (RangeImage[I(i  ,j+1)] >= 0) + 
          (RangeImage[I(i+1,j+1)] >= 0);
        
        if (N == 3)
        {
          // 1 Triangle
          if      (RangeImage[I(i  ,j)] < 0)
            m_Faces.Add(Face(RangeImage[I(i  ,j+1)],
                                RangeImage[I(i+1,j+1)],
                                RangeImage[I(i+1,j)]));
          else if (RangeImage[I(i+1,j)] < 0)
            m_Faces.Add(Face(RangeImage[I(i  ,j)],
                                RangeImage[I(i  ,j+1)],
                                RangeImage[I(i+1,j+1)]));
          else if (RangeImage[I(i  ,j+1)] < 0)
            m_Faces.Add(Face(RangeImage[I(i+1,j+1)],
                                RangeImage[I(i+1,j)],
                                RangeImage[I(i  ,j)]));
          else if (RangeImage[I(i+1,j+1)] < 0)
            m_Faces.Add(Face(RangeImage[I(i+1,j)],
                                RangeImage[I(i  ,j)],
                                RangeImage[I(i  ,j+1)]));
        }
        if (N == 4)
        {
          // 2 Triangles, 2 possible triangulations
          
          Vector3f A = m_Vertices[RangeImage[I(i  ,j)]] - m_Vertices[RangeImage[I(i+1,j+1)]];
          Vector3f B = m_Vertices[RangeImage[I(i+1,j)]] - m_Vertices[RangeImage[I(i  ,j+1)]];

          if (A.Norm(2) < B.Norm(2))
          {
            m_Faces.Add(Face(RangeImage[I(i  ,j)],
                                RangeImage[I(i  ,j+1)],
                                RangeImage[I(i+1,j+1)]));

            m_Faces.Add(Face(RangeImage[I(i+1,j+1)],
                                RangeImage[I(i+1,j)],
                                RangeImage[I(i  ,j)]));
            
          }
          else
          {
            m_Faces.Add(Face(RangeImage[I(i+1,j)],
                                RangeImage[I(i  ,j)],
                                RangeImage[I(i  ,j+1)]));

            m_Faces.Add(Face(RangeImage[I(i+1,j)],
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
  LoadVertexDataFunction(Ptr<Stream> pStream) : m_pStream(pStream)
  {}

  VOID operator()(CONST Mesh::TABLEROW& Row, UINT BlockSize)
  {
    Assert(Row.PartitionCount == 1);
    m_pStream->Read(Row.pPartitionRowData[0], Row.PartitionRowSize[0]*BlockSize);
  }

private:
  Ptr<Stream> m_pStream;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::ReadPlyVerticesBinary(Ptr<Stream> pStream)
{
  LoadVertexDataFunction LoadVertexData(pStream);
  IterateVertexBlocks(LoadVertexData, 65536);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::ReadPlyTrianglesBinary(Ptr<Stream> pStream)
{
  throw NotImplementedException(0);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
class ReadTrianglesFunction
{
public:
  ReadTrianglesFunction(Ptr<Stream> pStream) : m_pStream(pStream)
  {}

  typedef INDEXEDTRIANGLE ATTRIBUTE0;

  VOID operator() (CONST Mesh::TABLEROW& Row, INDEXEDTRIANGLE& T)
  {
    BYTE N;
    m_pStream->Read(&N, 1);
    m_pStream->Read(&T, sizeof(INDEXEDTRIANGLE));
  }

private:
  Ptr<Stream> m_pStream;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::ReadPlyFacesBinary(Ptr<Stream> pStream)
{
  ReadTrianglesFunction ReadFunction(pStream);
  IterateTriangles(ReadFunction, TriangleAttribute::VERTICES);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::ExportBNPTS(LPCTSTR Filename)
{
  
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::ExportPLY(LPCTSTR Filename)
{
  
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
class WriteLXSFacesFunction
{
public:
  WriteLXSFacesFunction(TextStream& OutputStream) : m_OutputStream(OutputStream)
  {}

  typedef INDEXEDTRIANGLE ATTRIBUTE0;

  VOID operator() (Mesh::TABLEROW& Row, INDEXEDTRIANGLE& Triangle)
  {
    m_OutputStream.WriteLine(String::Format("%u %u %u", Triangle.Vertex[0], Triangle.Vertex[1], Triangle.Vertex[2]));
  }

private:
  TextStream& m_OutputStream;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
class WriteLXSVertexPositionFunction
{
public:
  WriteLXSVertexPositionFunction(TextStream& OutputStream) : m_OutputStream(OutputStream)
  {}

  typedef Vector3F ATTRIBUTE0;

  VOID operator() (Mesh::TABLEROW& Row, Vector3F& Position)
  {
    m_OutputStream.WriteLine(String::Format("%f %f %f", Position[0], Position[1], Position[2]));
  }

private:
  TextStream& m_OutputStream;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::ExportLXS(LPCTSTR Filename)
{
  TextStream OutputStream(File::Open(Filename, FileMode::CREATE));

  Sphere3F BoundingSphere = GetBoundingSphere();
  OutputStream.WriteLine(String::Format("LookAt %f 0.1 %f %f %f %f 0 1 0", BoundingSphere.Center()[0], -BoundingSphere.Center()[2] + 4.0 * BoundingSphere.Radius(), BoundingSphere.Center()[0], BoundingSphere.Center()[1], BoundingSphere.Center()[2]));
  OutputStream.WriteLine("Camera \"perspective\" \"float fov\" [45]");
  OutputStream.WriteLine("Film \"fleximage\" \"integer xresolution\" [512] \"integer yresolution\" [512]");
  OutputStream.WriteLine("PixelFilter \"mitchell\" \"float xwidth\" [2] \"float ywidth\" [2]");
  OutputStream.WriteLine("Sampler \"lowdiscrepancy\" \"string pixelsampler\" [\"lowdiscrepancy\"]");
  OutputStream.WriteLine("WorldBegin");
  OutputStream.WriteLine("AttributeBegin");
  OutputStream.WriteLine("CoordSysTransform \"camera\" LightSource \"distant\" \"point from\" [0 40 -10] \"point to\" [0 0 0]	\"color L\" [0.5 0.5 0.5] \"integer nsamples\" 10");
  OutputStream.WriteLine("AttributeEnd");
  OutputStream.WriteLine("AttributeBegin");
  OutputStream.WriteLine("Texture \"object\" \"color\" \"constant\" \"color value\" [0.4 0.4 0.4]");
  OutputStream.WriteLine("Texture \"floor\" \"color\" \"constant\" \"color value\" [0.0 0.0 0.2]");
  OutputStream.WriteLine("Material \"matte\" \"texture Kd\" \"floor\"");
  OutputStream.WriteLine("Shape \"trianglemesh\" \"integer indices\" [0 2 1 2 0 3] \"point P\" [-10.0 0 -10.0 -10.0 0 10.0 10.0 0 10.0 10.0 0 -10.0]");
  OutputStream.WriteLine("Material \"matte\" \"texture Kd\" \"object\"");
  OutputStream.WriteLine("Shape \"trianglemesh\" \"integer indices\" [");

  WriteLXSFacesFunction TriangleFunction(OutputStream);
  IterateTriangles(TriangleFunction, TriangleAttribute::VERTICES);

  OutputStream.WriteLine("] \"point P\" [");
  
  WriteLXSVertexPositionFunction VertexPositionFunction(OutputStream);
  IterateVertices(VertexPositionFunction, VertexAttribute::POSITION3);

  OutputStream.WriteLine("]");
  OutputStream.WriteLine("AttributeEnd");
  OutputStream.WriteLine("WorldEnd");

  OutputStream.Close();
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
  S.WriteLine("property float x");s
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
