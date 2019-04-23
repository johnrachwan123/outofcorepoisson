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

#include "CodeLibrary.hpp"

#include "Mesh.hpp"
#include "IO/Path.hpp"
#include "Utility/Flags.hpp"
#include "Debug/Trace.hpp"
#include "Math/Math.hpp"
#include "Data/Xml/Reader.hpp"
#include "Data/Xml/Writer.hpp"
#include "System/Process.hpp"

using namespace Bolitho;
using namespace Bolitho::Math;
using namespace Bolitho::Xml;
using namespace Bolitho::Graphics;
using namespace Bolitho::IO;
using namespace Bolitho::System;

//----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Graphics,Mesh,Object);
//ImplementRuntimeType(Bolitho::Graphics,MeshException,UserException);
//----------------------------------------------------------------------------------------------------------------------------------------------------
CONST Mesh::Attribute VertexAttribute::POSITION3 = Mesh::Attribute("Position", Mesh::AttributeType::REAL32, 3);
CONST Mesh::Attribute VertexAttribute::POSITION2 = Mesh::Attribute("Position", Mesh::AttributeType::REAL32, 2);
CONST Mesh::Attribute VertexAttribute::NORMAL2 = Mesh::Attribute("Normal", Mesh::AttributeType::REAL32, 2);
CONST Mesh::Attribute VertexAttribute::NORMAL3 = Mesh::Attribute("Normal", Mesh::AttributeType::REAL32, 3);
CONST Mesh::Attribute VertexAttribute::DIFFUSE = Mesh::Attribute("Diffuse", Mesh::AttributeType::UINT8, 3);
CONST Mesh::Attribute VertexAttribute::MEANCURVATURE = Mesh::Attribute("MeanCurvature", Mesh::AttributeType::REAL32, 1);
CONST Mesh::Attribute VertexAttribute::LOCALDEPTH = Mesh::Attribute("LocalDepth", Mesh::AttributeType::REAL32, 1);
CONST Mesh::Attribute VertexAttribute::ACCESSIBILITY = Mesh::Attribute("Accessibility", Mesh::AttributeType::REAL32, 1);

CONST Mesh::Attribute VertexAttribute::FREE1 = Mesh::Attribute("FreeSpace", Mesh::AttributeType::NONE, 1);
CONST Mesh::Attribute VertexAttribute::FREE4 = Mesh::Attribute("FreeSpace", Mesh::AttributeType::NONE, 4);
CONST Mesh::Attribute VertexAttribute::FREE8 = Mesh::Attribute("FreeSpace", Mesh::AttributeType::NONE, 8);
CONST Mesh::Attribute VertexAttribute::FREE12 = Mesh::Attribute("FreeSpace", Mesh::AttributeType::NONE, 12);
CONST Mesh::Attribute VertexAttribute::FREE16 = Mesh::Attribute("FreeSpace", Mesh::AttributeType::NONE, 16);
CONST Mesh::Attribute VertexAttribute::FREE32 = Mesh::Attribute("FreeSpace", Mesh::AttributeType::NONE, 32);
CONST Mesh::Attribute VertexAttribute::INDEX = Mesh::Attribute("Index", Mesh::AttributeType::UINT32, 1);
CONST Mesh::Attribute VertexAttribute::ZINDEX = Mesh::Attribute("ZIndex", Mesh::AttributeType::UINT64, 1);
CONST Mesh::Attribute VertexAttribute::SLICEZINDEX = Mesh::Attribute("SliceZIndex", Mesh::AttributeType::UINT64, 1);
CONST Mesh::Attribute VertexAttribute::FLAGS = Mesh::Attribute("Flags", Mesh::AttributeType::UINT32, 1);
//----------------------------------------------------------------------------------------------------------------------------------------------------
CONST Mesh::Attribute TriangleAttribute::VERTICES = Mesh::Attribute("Vertices", Mesh::AttributeType::UINT32, 3);
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
Mesh::Mesh(LPCTSTR Filename, DWORD MeshFlags) : VertexData(*this), EdgeData(*this), TriangleData(*this)
{
  m_StatisticsFlags = 0;
  m_Filename = Filename;
  m_Flags = MeshFlags;

  if (m_Flags & MeshFlags::CREATE)
  {
    Create();
    ReadHeader();
  }
  else if (m_Flags & MeshFlags::OPEN)
  {
    ReadHeader();
  }
  m_DataPath = Path::Directory(Path::FullPath(Filename));

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Mesh::Mesh(DWORD MeshFlags) : VertexData(*this), EdgeData(*this), TriangleData(*this)
{
  m_StatisticsFlags = 0;
  m_Flags = MeshFlags | MeshFlags::TEMPORARY;

  Partition& VertexPartition = VertexData.m_Partitions.Add();
  VertexPartition.m_pTable = &VertexData;
  VertexPartition.m_RowSize = 0;
  VertexPartition.m_Flags = MeshPartitionFlags::TEMPORARY;
  VertexPartition.m_Name = String::Format("%d.%p.VertexData", Process::GetCurrentProcessId(), this);

  Partition& TrianglePartition = TriangleData.m_Partitions.Add();
  TrianglePartition.m_pTable = &TriangleData;
  TrianglePartition.m_RowSize = 0;
  TrianglePartition.m_Flags = MeshPartitionFlags::TEMPORARY;
  TrianglePartition.m_Name = String::Format("%d.%p.TriangleData", Process::GetCurrentProcessId(), this);

  m_DataPath = Path::Directory(Path::FullPath("."));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Mesh::~Mesh()
{
  Close();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Mesh::Begin(DWORD VertexDataUsage, DWORD TriangleDataUsage, DWORD EdgeDataUsage, BOOL ForceStorageFormat)
{
  VertexData.Open(VertexDataUsage);
  TriangleData.Open(TriangleDataUsage);
  EdgeData.Open(EdgeDataUsage);

  return TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::End()
{
  VertexData.Close();
  TriangleData.Close();
  EdgeData.Close();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Optimize(DWORD VertexDataUsage, DWORD TriangleDataUsage, DWORD EdgeDataUsage)
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Import(LPCTSTR Filename)
{
  CONST DWORD ImportUsage = MeshDataUsage::WRITE | MeshDataUsage::STREAMDATA | MeshDataUsage::CREATEPARTITION | MeshDataUsage::RESIZEPARTITION;
  
  Begin(ImportUsage, ImportUsage, ImportUsage);
  if (Path::ExtensionMatches(Filename, "Ply"))
    ImportPLY(Filename);
  else if (Path::ExtensionMatches(Filename, "Bnpts"))
    ImportBNPTS(Filename);
  End();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Export(LPCTSTR Filename)
{
  CONST DWORD ImportUsage = MeshDataUsage::READ | MeshDataUsage::STREAMDATA;
  
  Begin(ImportUsage, ImportUsage, ImportUsage);
  if (Path::ExtensionMatches(Filename, "Ply"))
    ExportPLY(Filename);
  else if (Path::ExtensionMatches(Filename, "Bnpts"))
    ExportBNPTS(Filename);
  else if (Path::ExtensionMatches(Filename, "Lxs"))
    ExportLXS(Filename);
  End();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Create()
{
  if (File::Exists(m_Filename))
    File::Delete(m_Filename);
  
  XmlWriter W(m_Filename, 0);

  W.StartDocument();
  W.StartElement("Mesh");

  W.StartElement("Vertices");
  W.WriteAttribute("Count", "0");
  W.StartElement("Storage");
  W.StartElement("Partition");
  W.WriteAttribute("Filename", Path::Relative(m_Filename, m_Filename + ".VertexData") );
  W.WriteAttribute("RowSize", "0");
  W.EndElement();
  W.EndElement();
  W.EndElement();

  W.StartElement("Triangles");
  W.WriteAttribute("Count", "0");
  W.StartElement("Storage");
  W.StartElement("Partition");
  W.WriteAttribute("Filename", Path::Relative(m_Filename, m_Filename + ".TriangleData"));
  W.WriteAttribute("RowSize", "0");
  W.EndElement();
  W.EndElement();
  W.EndElement();

  W.StartElement("Edges");
  W.WriteAttribute("Count", "0");
  W.StartElement("Storage");
  W.StartElement("Partition");
  W.WriteAttribute("Filename", Path::Relative(m_Filename, m_Filename + ".EdgeData"));
  W.WriteAttribute("RowSize", "0");
  W.EndElement();
  W.EndElement();
  W.EndElement();

  W.StartElement("Metadata");
  W.EndElement();

  W.EndElement();
  W.EndDocument();

  Ptr<File> VertexData = File::Open(m_Filename + ".VertexData", FileMode::CREATE);
  VertexData->Close();
  Ptr<File> TriangleData = File::Open(m_Filename + ".TriangleData", FileMode::CREATE);
  TriangleData->Close();
  Ptr<File> EdgeData = File::Open(m_Filename + ".EdgeData", FileMode::CREATE);
  EdgeData->Close();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::ReadHeader()
{
  XmlReader Reader(m_Filename, 0);
  
  while (Reader.Read())
  {
    Trace("%u %u %s", Reader.Depth(), Reader.Type(), Reader.Name().ConstStr());

    if (Reader.Type() == NodeType::ELEMENT)
    {
      if (Reader.Name() == "Vertices")
        ReadVertexHeader(Reader);
      if (Reader.Name() == "Triangles")
        ReadTriangleHeader(Reader);
      if (Reader.Name() == "Edges")
        ReadEdgeHeader(Reader);

      if (Reader.Name() == "Metadata")
        ReadMetadata(Reader);
    }
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::ReadMetadata(XmlReader& Reader)
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::ReadTableHeader(XmlReader& Reader, Table* pTable)
{
  UINT Depth = Reader.Depth();

  if (!Reader.MoveToAttribute("Count"))
    throw "Expected Count attribute";
  pTable->m_Count = Parse<UINT>(Reader.Value());

  while (Reader.Read())
  {
    if (Reader.Depth() <= Depth)
      break;

    if (Reader.Type() == NodeType::ELEMENT)
    {
      if (Reader.Name() == "Storage")
        ReadTableStorageHeader(Reader, pTable);
      if (Reader.Name() == "Index")
        ReadTableIndexHeader(Reader, pTable);
      if (Reader.Name() == "Metadata")
        ReadMetadata(Reader);
    }
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::ReadTableAttributeHeader(XmlReader& Reader, Partition& P)
{
  UINT Depth = Reader.Depth();
  P.m_AttributeSize = 0;
  
  while (Reader.Read())
  {
    if (Reader.Depth() < Depth)
      break;
    
    if (Reader.Type() == NodeType::ELEMENT && Reader.Name() == "Attribute")
    {
      String Name;
      String Type;
      UINT Partition = 0;

      while (Reader.MoveToNextAttribute())
      {
        if (Reader.Name() == "Name")
          Name = Reader.Value();
        if (Reader.Name() == "Type")
          Type = Reader.Value();
      }

      Attribute A(Name, Type, P.m_AttributeSize);
      P.m_Attributes.Add(A);
      P.m_AttributeSize += A.Size();
    }

  }

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::ReadTableStorageHeader(Xml::XmlReader& Reader, Table* pTable)
{
  UINT Depth = Reader.Depth();
  
  while (Reader.Read())
  {
    if (Reader.Depth() <= Depth)
      break;

    if (Reader.Type() == NodeType::ELEMENT && Reader.Name() == "Partition")
    {
      Partition& P = pTable->m_Partitions.Add();
      P.m_pTable = pTable;

      while (Reader.MoveToNextAttribute())
      {
        if (Reader.Name() == "Name")
          P.m_Name = Reader.Value();
        if (Reader.Name() == "RowSize")
          P.m_RowSize = Parse<UINT>(Reader.Value());
      }

      ReadTableAttributeHeader(Reader, P);

    }
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::ReadTableIndexHeader(Xml::XmlReader& Reader, Table* pTable)
{
  UINT Depth = Reader.Depth();
  
  while (Reader.Read())
  {
    Trace("%u %u %s %s", Reader.Type(), Reader.Depth(), Reader.Name().ConstStr(), Reader.Value().ConstStr());

    if (Reader.Depth() == Depth)
      break;
    Trace("%s", Reader.Name().ConstStr());

  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::WriteHeader()
{
  XmlWriter W(m_Filename, 0);

  W.StartDocument();
  W.StartElement("Mesh");

  W.StartElement("Vertices");
  W.WriteAttribute("Count", String::Format("%u", VertexData.GetCount()));
  W.StartElement("Storage");
  
  for (SIZE_T i=0; i<VertexData.m_Partitions.Length(); i++)
  {
    W.StartElement("Partition");
    W.WriteAttribute("Name", VertexData.m_Partitions[i].GetName());
    W.WriteAttribute("RowSize", String::Format("%d", VertexData.m_Partitions[i].GetRowSize()));

    for (SIZE_T j=0; j<VertexData.m_Partitions[i].m_Attributes.Length(); j++)
    {
      W.StartElement("Attribute");
      W.WriteAttribute("Name", VertexData.m_Partitions[i].m_Attributes[j].Name());
      W.WriteAttribute("Type", VertexData.m_Partitions[i].m_Attributes[j].TypeString());
      W.EndElement();
    }

    W.EndElement();
  }

  W.EndElement();
  W.EndElement();

  W.StartElement("Triangles");
  W.WriteAttribute("Count", String::Format("%u", TriangleData.GetCount()));
  W.StartElement("Storage");

  for (SIZE_T i=0; i<TriangleData.m_Partitions.Length(); i++)
  {
    W.StartElement("Partition");
    W.WriteAttribute("Name", TriangleData.m_Partitions[i].GetName());
    W.WriteAttribute("RowSize", String::Format("%d", TriangleData.m_Partitions[i].GetRowSize()));

    for (SIZE_T j=0; j<TriangleData.m_Partitions[i].m_Attributes.Length(); j++)
    {
      W.StartElement("Attribute");
      W.WriteAttribute("Name", TriangleData.m_Partitions[i].m_Attributes[j].Name());
      W.WriteAttribute("Type", TriangleData.m_Partitions[i].m_Attributes[j].TypeString());
      W.EndElement();
    }

    W.EndElement();
  }

  W.EndElement();
  W.EndElement();

  W.StartElement("Edges");
  W.WriteAttribute("Count", String::Format("%u", EdgeData.GetCount()));
  W.StartElement("Storage");

  for (SIZE_T i=0; i<EdgeData.m_Partitions.Length(); i++)
  {
    W.StartElement("Partition");
    W.WriteAttribute("Name", EdgeData.m_Partitions[i].GetName());
    W.WriteAttribute("RowSize", String::Format("%d", EdgeData.m_Partitions[i].GetRowSize()));

    for (SIZE_T j=0; j<EdgeData.m_Partitions[i].m_Attributes.Length(); j++)
    {
      W.StartElement("Attribute");
      W.WriteAttribute("Name", EdgeData.m_Partitions[i].m_Attributes[j].Name());
      W.WriteAttribute("Type", EdgeData.m_Partitions[i].m_Attributes[j].TypeString());
      W.EndElement();
    }

    W.EndElement();
  }

  W.EndElement();
  W.EndElement();

  W.StartElement("Metadata");
  for (Map<String,MetadataValue>::Enumerator E = MetaData.m_Values.Elements(); E.IsValid(); E++)
  {
    W.StartElement("Meta");
    W.WriteAttribute("Name", E.Key());
    W.WriteAttribute("Type", E.Value().TypeString());
    W.WriteAttribute("Value", E.Value().Value());
    W.EndElement();
  }
  W.EndElement();

  W.EndElement();
  W.EndDocument();

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Flush()
{
  WriteHeader();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Close()
{
  if (!IsFlagSet(m_Flags, MeshFlags::TEMPORARY))
    WriteHeader();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Mesh::TableDataStorage* Mesh::GetStore(LPCTSTR Name)
{
  return new FileDataStorage(Path::Append(m_DataPath, Name));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::TABLEROW::CopyTo(LPVOID pData)
{
  LPBYTE pData0 = (LPBYTE)pData;
  for (UINT i=0; i<PartitionCount; i++)
  {
    CopyMemory(pData0, pPartitionRowData[i], PartitionRowSize[i]);
    pData0 += PartitionRowSize[i];
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::TABLEROW::CopyFrom(LPCVOID pData)
{
  LPCBYTE pData0 = (LPCBYTE)pData;
  for (UINT i=0; i<PartitionCount; i++)
  {
    CopyMemory(pPartitionRowData[i], pData0, PartitionRowSize[i]);
    pData0 += PartitionRowSize[i];
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Mesh::Attribute::Attribute()
{
  m_DataType = AttributeType::NONE;
  m_Dimensions = 0;
  m_Offset = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Mesh::Attribute::Attribute(LPCTSTR Name, LPCTSTR Type, UINT Offset)
{
  m_Name = Name;
  m_Offset = Offset;
  
  String TypeString(Type);
  TypeString.ToUpper();

  m_Dimensions = 1;
  if (TypeString.BeginsWith("BYTE") ||TypeString.BeginsWith("INT8"))
    m_DataType = AttributeType::INT8;
  else if (TypeString.BeginsWith("UINT8"))
    m_DataType = AttributeType::UINT8;
  else if (TypeString.BeginsWith("SHORT") || TypeString.BeginsWith("INT16"))
    m_DataType = AttributeType::INT16;
  else if (TypeString.BeginsWith("USHORT") || TypeString.BeginsWith("UINT16"))
    m_DataType = AttributeType::UINT16;
  else if (TypeString.BeginsWith("INT") || TypeString.BeginsWith("INT32"))
    m_DataType = AttributeType::INT32;
  else if (TypeString.BeginsWith("UINT") || TypeString.BeginsWith("UINT32"))
    m_DataType = AttributeType::UINT32;
  else if (TypeString.BeginsWith("LONG") || TypeString.BeginsWith("INT64") || TypeString.BeginsWith("LONGLONG"))
    m_DataType = AttributeType::INT64;
  else if (TypeString.BeginsWith("ULONG") || TypeString.BeginsWith("UINT64") || TypeString.BeginsWith("ULONGLONG"))
    m_DataType = AttributeType::UINT64;
  else if (TypeString.BeginsWith("FLOAT") || TypeString.BeginsWith("REAL32"))
    m_DataType = AttributeType::REAL32;
  else if (TypeString.BeginsWith("DOUBLE") || TypeString.BeginsWith("REAL64"))
    m_DataType = AttributeType::REAL64;
  else if (TypeString.BeginsWith("NONE"))
    m_DataType = AttributeType::NONE;

  String::Iterator i = TypeString.End();
  String::Iterator j = TypeString.End();
  i = TypeString.Find("[");
  if (i != TypeString.End())
    j = TypeString.Find("]");
  else
  {
    i = TypeString.Find("(");
    if (i != TypeString.End())
      j = TypeString.Find(")");
  }

  if (i != TypeString.End() && j != TypeString.End())
  {
    String Dimensions = TypeString.Substr(i+1, j);
    m_Dimensions = Parse<UINT>(Dimensions);
  }

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Mesh::Attribute::Attribute(LPCTSTR Name, DWORD Type, UINT Dimensions)
{
  m_Name = Name;
  m_DataType = Type;
  m_Dimensions = Dimensions;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Mesh::Attribute::Attribute(CONST Attribute& A) : m_Name(A.m_Name)
{
  m_DataType = A.m_DataType;
  m_Dimensions = A.m_Dimensions;
  m_Offset = A.m_Offset;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
UINT Mesh::Attribute::Size() CONST
{
  return (m_DataType & 0xf) * m_Dimensions;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
String Mesh::Attribute::TypeString() CONST
{
  switch (m_DataType)
  {
  case AttributeType::UINT8:
    return String::Format("UINT8(%u)", m_Dimensions);
  case AttributeType::INT8:
    return String::Format("INT8(%u)", m_Dimensions);
  case AttributeType::UINT16:
    return String::Format("UINT16(%u)", m_Dimensions);
  case AttributeType::INT16:
    return String::Format("INT16(%u)", m_Dimensions);
  case AttributeType::UINT32:
    return String::Format("UINT32(%u)", m_Dimensions);
  case AttributeType::INT32:
    return String::Format("INT32(%u)", m_Dimensions);
  case AttributeType::REAL32:
    return String::Format("REAL32(%u)", m_Dimensions);
  case AttributeType::REAL64:
    return String::Format("REAL64(%u)", m_Dimensions);
  default:
    return String::Format("NONE(%u)", m_Dimensions);
  };
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Mesh::MetadataTable::Contains(LPCTSTR Name)
{
  return m_Values.Contains(Name);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
String Mesh::MetadataTable::GetValue(LPCTSTR Name)
{
  return m_Values.Get(Name).m_Value;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD Mesh::MetadataTable::GetType(LPCTSTR Name)
{
  return m_Values.Get(Name).m_DataType;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::MetadataTable::SetValue(LPCTSTR Name, LPCTSTR Value)
{
  m_Values.Add(Name, MetadataValue(Name, Value, MetadataType::STRING));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
String& Mesh::MetadataTable::operator[] (LPCTSTR Name)
{
  if (!m_Values.Contains(Name))
    m_Values.Add(Name, MetadataValue(Name, "", MetadataType::STRING));

  return m_Values[Name].m_Value;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
String Mesh::MetadataValue::TypeString() CONST
{
  switch (m_DataType)
  {
  default:
    return "STRING";
  };
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
Mesh::Partition::Partition()
{
  m_RowSize = 0;
  m_AttributeSize = 0;
  m_DataUsage = MeshDataUsage::NONE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Mesh::Partition::~Partition()
{
  if (m_pStore)
  {
    m_pStore->Close();
    if (IsFlagSet(m_Flags, MeshPartitionFlags::TEMPORARY))
      m_pStore->Delete();
    m_pStore = 0;
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Partition::Open(DWORD DataUsage, TableDataStorage* pStore)
{
  m_DataUsage = DataUsage;
  m_pStore = pStore;
  m_MappingCount = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Partition::Close()
{
  m_DataUsage = 0;
  m_pStore->Close();
  m_pStore = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Partition::Delete()
{
  m_pStore->Delete();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LPVOID Mesh::Partition::Map(DWORD PageAccess)
{
  return m_pStore->Map(PageAccess);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Partition::Unmap()
{
  m_pStore->Unmap();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<IO::Stream> Mesh::Partition::GetStream()
{
  return m_pStore->GetStream();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Partition::Resize(UINT RowSize, UINT Count)
{
  /* Cheap */
  if (RowSize == m_RowSize)
  {
    m_pStore->Resize(m_RowSize * Count);
    return;
  }

  /* Expensive */
  Ptr<File> pFile = File::Open(GetName() + ".ResizeTemp", FileMode::CREATE, FileAccess::WRITE);
  Ptr<Stream> pSource = m_pStore->GetStream();

  BYTE* pBuffer = new BYTE[Max(RowSize, m_RowSize)];
  ZeroMemory(pBuffer, Max(RowSize, m_RowSize));
  for (UINT i=0; i<Min(Count, m_pTable->m_Count); i++)
  {
    pSource->Read(pBuffer, m_RowSize);
    pFile->Write(pBuffer, RowSize);
  }

  pFile = 0;
  pSource = 0;
  m_pStore->ReplaceData(GetName() + ".ResizeTemp");
  
  m_RowSize = RowSize;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Partition::RemoveAllAttributes()
{
  m_Attributes.Resize(0);
  m_AttributeSize = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Partition::AddAttribute(CONST Attribute& A)
{
  UINT RequiredSize = A.Size();

  /* First, search the existing attributes for free space */
  for (SIZE_T i=0; i<m_Attributes.Length(); i++)
  {
    if (m_Attributes[i].Type() == AttributeType::NONE && m_Attributes[i].Size() >= RequiredSize)
    {
      m_Attributes[i] = A; 
      return;
    }
  }
  
  /* Second, look for free row space */
  if (m_RowSize - m_AttributeSize >= RequiredSize)
  {
    Attribute& NewAttribute = m_Attributes.Add(A);
    NewAttribute.m_Offset = m_AttributeSize;
    m_AttributeSize += RequiredSize;
    return;
  }

  /* Lastly, increase row space */
  Resize(m_RowSize + RequiredSize, m_pTable->m_Count);
  Attribute& NewAttribute = m_Attributes.Add(A);
  NewAttribute.m_Offset = m_AttributeSize;
  m_AttributeSize += RequiredSize;

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Partition::RemoveAttribute(CONST Attribute& A)
{
  UINT AttributeId = GetAttributeIndex(A);
  m_Attributes[AttributeId].m_Name = "";
  m_Attributes[AttributeId].m_Dimensions = m_Attributes[AttributeId].Size();
  m_Attributes[AttributeId].m_DataType = AttributeType::NONE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
Mesh::Table::Table(Mesh& Mesh) : m_Mesh(Mesh)
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Mesh::Table::Open(DWORD DataUsage)
{
  m_DataUsage = DataUsage;
  for (SIZE_T i=0; i<m_Partitions.Length(); i++)
    m_Partitions[i].Open(DataUsage, m_Mesh.GetStore(m_Partitions[i].GetName()));
  return TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Table::Close()
{
  m_DataUsage = 0;
  for (SIZE_T i=0; i<m_Partitions.Length(); i++)
    m_Partitions[i].Close();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Mesh::Table::HasAttribute(CONST Attribute& A) CONST
{
  return GetAttributeId(A) != Attribute::INVALIDID;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Mesh::Partition::HasAttribute(CONST Attribute& A) CONST
{
  for (SIZE_T i=0; i<m_Attributes.Length(); i++)
  {
    if (m_Attributes[i].m_Name == A.m_Name && m_Attributes[i].m_DataType == A.m_DataType && m_Attributes[i].m_Dimensions == A.m_Dimensions)
      return TRUE;
  }
  return FALSE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
UINT Mesh::Partition::GetAttributeIndex(CONST Attribute& A) CONST
{
  for (SIZE_T i=0; i<m_Attributes.Length(); i++)
  {
    if (m_Attributes[i].m_Name == A.m_Name && m_Attributes[i].m_DataType == A.m_DataType && m_Attributes[i].m_Dimensions == A.m_Dimensions)
      return (UINT)i;
  }
  return Attribute::INVALIDID;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
UINT Mesh::Table::GetAttributeId(CONST Attribute& A) CONST
{
  for (SIZE_T i=0; i<m_Partitions.Length(); i++)
    for (SIZE_T j=0; j<m_Partitions[i].m_Attributes.Length(); j++)
      if (m_Partitions[i].m_Attributes[j].m_Name == A.m_Name && m_Partitions[i].m_Attributes[j].m_DataType == A.m_DataType && m_Partitions[i].m_Attributes[j].m_Dimensions == A.m_Dimensions)
        return (UINT)(i << 16) | (UINT)j;
  return Attribute::INVALIDID;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Table::AddAttribute(CONST Attribute& A)
{
  AddAttribute(A, 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Table::AddAttribute(CONST Attribute& A, UINT PartitionId)
{
  if (!HasAttribute(A))
    m_Partitions[PartitionId].AddAttribute(A);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Table::RemoveAttribute(CONST Attribute& A)
{
  UINT i = GetAttributeId(A);
  if (i == Attribute::INVALIDID)
    throw "Attribute not found";
  
  UINT PartitionId = i >> 16;
  UINT AttributeId = i & 0xffff;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Table::RemoveAllAttributes()
{
  for (SIZE_T i=0; i<m_Partitions.Length(); i++)
    RemoveAllAttributes((UINT)i);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Table::RemoveAllAttributes(UINT PartitionId)
{
  m_Partitions[PartitionId].RemoveAllAttributes();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Table::Resize(UINT Count)
{
  for (SIZE_T i=0; i<m_Partitions.Length(); i++)
    m_Partitions[i].Resize(m_Partitions[i].GetRowSize(), Count);
  m_Count = Count;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Table::DeletePartition(UINT PartitionId)
{
  m_Partitions[PartitionId].Close();
  m_Partitions[PartitionId].Delete();
  m_Partitions.Erase(PartitionId);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Table::DeleteAllPartitions()
{
  for (SIZE_T i=0; i<m_Partitions.Length(); i++)
  {
    m_Partitions[i].Close();
    m_Partitions[i].Delete();
  }
  m_Partitions.Resize(0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
UINT Mesh::Table::CreatePartition(UINT Size, DWORD Flags)
{
  return CreatePartition("Temp", Size, Flags | MeshPartitionFlags::TEMPORARY);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
UINT Mesh::Table::CreatePartition(LPCTSTR Filename, UINT Size, DWORD Flags)
{
  Partition& P = m_Partitions.Add();
  P.m_pTable = this;
  P.m_Name = Path::Append(m_Mesh.m_Filename, Filename);
  P.m_RowSize = Size;
  P.m_Flags = Flags;

  P.Open(m_DataUsage, m_Mesh.GetStore(P.GetName()));
  P.Resize(Size, m_Count);

  return (UINT)m_Partitions.Length()-1;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Table::Map()
{
  for (SIZE_T i=0; i<m_Partitions.Length(); i++)
  {
    m_pPartitionData[i] = (LPBYTE)m_Partitions[i].Map(PageAccess::READWRITE);
    m_PartitionStride[i] = m_Partitions[i].GetRowSize();
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Table::Unmap()
{
  for (SIZE_T i=0; i<m_Partitions.Length(); i++)
  {
    m_Partitions[i].Unmap();
    m_pPartitionData[i] = 0;
    m_PartitionStride[i] = 0;
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
class ReadTableDataFunction
{
public:
  ReadTableDataFunction(LPVOID pData)
  {
    m_pData = (LPBYTE)pData;
  }

  VOID operator()(Mesh::TABLEROW& Row)
  {
    Row.CopyTo(m_pData);
    m_pData += Row.TotalRowSize;
  }

private:
  LPBYTE m_pData;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
class WriteTableDataFunction
{
public:
  WriteTableDataFunction(LPCVOID pData)
  {
    m_pData = (LPCBYTE)pData;
  }

  VOID operator()(Mesh::TABLEROW& Row)
  {
    Row.CopyFrom(m_pData);
    m_pData += Row.TotalRowSize;
  }

private:
  LPCBYTE m_pData;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Table::Read(LPVOID pData, UINT Offset, UINT Count)
{
  ReadTableDataFunction ReadData(pData);
  Iterate(ReadData, Offset, Count);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Table::Write(LPCVOID pData, UINT Offset, UINT Count)
{
  WriteTableDataFunction WriteData(pData);
  Iterate(WriteData, Offset, Count);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Table::Read(LPVOID pData, UINT Partition, UINT Offset, UINT Count)
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Table::Write(LPCVOID pData, UINT Partition, UINT Offset, UINT Count)
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------
Mesh::FileDataStorage::FileDataStorage(LPCTSTR Filename)
{
  m_pFile = File::Open(Filename);
  m_Filename = Filename;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LPVOID Mesh::FileDataStorage::Map(DWORD PageAccess)
{
  m_pView = m_pFile->MapView(PageAccess);
  return m_pView->GetBaseAddress();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::FileDataStorage::Unmap()
{
  m_pView = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<IO::Stream> Mesh::FileDataStorage::GetStream()
{
  Ptr<IO::Stream> pStream = m_pFile->Duplicate();
  pStream->Seek(0);
  return pStream;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::FileDataStorage::Resize(ULONGLONG Length)
{
  m_pFile->Resize(Length);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::FileDataStorage::Close()
{
  m_pFile->Close();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::FileDataStorage::Delete()
{
  m_pFile->Delete();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::FileDataStorage::ReplaceData(LPCTSTR Filename)
{
  m_pView = 0;
  m_pFile = 0;

  File::Delete(m_Filename);
  File::Rename(Filename, m_Filename);
  m_pFile = File::Open(m_Filename, FileMode::OPEN);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
Mesh::OctreeNode::OctreeNode()
{
  m_pChildren = 0;
  m_pParent = 0;
  m_VertexOffset = 0;
  m_VertexCount = 0;
  m_TriangleOffset = 0;
  m_TriangleCount = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Mesh::OctreeNode* Mesh::OctreeNode::CreateChildren()
{
  if (m_pChildren)
    return m_pChildren;

  Vector3F Center = m_VolumeBounds.Center();
  m_pChildren = new OctreeNode[8];

  for (UINT i=0; i<8;i++)
  {
    m_pChildren[i].m_VolumeBounds = m_VolumeBounds;
    m_pChildren[i].m_pParent = this;

    if (i & 1)
      m_pChildren[i].m_VolumeBounds.Min(0) = Center(0);
    else
      m_pChildren[i].m_VolumeBounds.Max(0) = Center(0);

    if (i & 2)
      m_pChildren[i].m_VolumeBounds.Min(1) = Center(1);
    else
      m_pChildren[i].m_VolumeBounds.Max(1) = Center(1);

    if (i & 4)
      m_pChildren[i].m_VolumeBounds.Min(2) = Center(2);
    else
      m_pChildren[i].m_VolumeBounds.Max(2) = Center(2);
  }

  return m_pChildren;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::OctreeNode::AggregateTree()
{
  STATIC UINT Offset = 0;

  if (m_pChildren)
  {
    for (UINT i=0; i<8; i++)
      m_pChildren[i].AggregateTree();
  
    m_VertexOffset = m_pChildren[0].m_VertexOffset;
    m_TriangleOffset = m_pChildren[0].m_TriangleOffset;

    m_VertexBounds = AxisAlignedBox3F::Empty;
    m_TriangleBounds = AxisAlignedBox3F::Empty;
    
    for (UINT i=0; i<8; i++)
    {
      m_VertexCount += m_pChildren[i].m_VertexCount;
      m_TriangleCount += m_pChildren[i].m_TriangleCount;

      if (m_pChildren[i].m_VertexCount > 0)
        m_VertexBounds.Union(m_pChildren[i].m_VertexBounds);
      if (m_pChildren[i].m_TriangleCount > 0)
        m_TriangleBounds.Union(m_pChildren[i].m_TriangleBounds);
    }
  }
  else
  {
    if (m_VertexOffset == 0)
      m_VertexOffset = Offset;
    else
      Offset = m_VertexOffset + m_VertexCount;
  }

}
