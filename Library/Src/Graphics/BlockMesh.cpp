/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/BlockMesh.cpp $
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
#include "IO/Path.hpp"
#include "Utility/Flags.hpp"
#include "Debug/Trace.hpp"

using namespace Bolitho;
using namespace Bolitho::Math;
using namespace Bolitho::Math::Geometry;
using namespace Bolitho::Graphics;
using namespace Bolitho::IO;

//----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Graphics,BlockMesh,Object);
ImplementRuntimeType(Bolitho::Graphics,MeshException,UserException);
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
#pragma warning(suppress:4355)
BlockMesh::BlockMesh() : VertexData(*this), TriangleData(*this), EdgeData(*this)
{
  m_pTriangleBlockInfo = 0;
  
  InitializeInMemory();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
#pragma warning(suppress:4355)
BlockMesh::BlockMesh(LPCTSTR Filename, DWORD FileMode) : VertexData(*this), TriangleData(*this), EdgeData(*this)
{
  m_pTriangleBlockInfo = 0;

  if (FileMode == FileMode::OPEN)
    InitializeOpen(Filename, PageAccess::READWRITE);
  if (FileMode == FileMode::CREATE)
    InitializeCreate(Filename);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BlockMesh::~BlockMesh()
{
  Close();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::Close()
{
  Flush();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::Flush()
{
  VertexData.Flush();
  TriangleData.Flush();
  EdgeData.Flush();

  SaveMetaData();

  m_BlockStore.Flush();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::InitializeInMemory()
{
  m_BlockStore.InitializeInMemory(BLOCKSIZE);
  m_pHeader = m_BlockStore.GetHeaderStruct<MESHHEADER>();
  ZeroMemory(m_pHeader, sizeof(MESHHEADER));

  m_pHeader->Signature = MESHSIGNATURE;
  m_pHeader->Version = MESHVERSION;
  m_pHeader->VertexDataDecl.Storage.Directory = m_BlockStore.AllocateBlock();
  m_pHeader->TriangleDataDecl.Storage.Directory = m_BlockStore.AllocateBlock();

  InitializeTables();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::InitializeTables()
{
  VertexData.Initialize(&m_pHeader->VertexDataDecl);
  TriangleData.Initialize(&m_pHeader->TriangleDataDecl);


/*
for (UINT i=0; i<8; i++)
  {
    if (m_pHeader->Blobs[i].Storage.Directory != 0)
      m_BlobTables[i].Initialize(m_pHeader->Blobs[i].Storage.Directory, m_pHeader->Blobs[i].Storage.BlockCount);
  }
*/

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::InitializeOpen(LPCTSTR Filename, DWORD Flags)
{
  m_BlockStore.InitializeOpen(Filename);
  m_pHeader = m_BlockStore.GetHeaderStruct<MESHHEADER>();

  if (m_pHeader->Signature != MESHSIGNATURE)
    throw MeshException(this, String::Format("The file \'%s\' is not a valid BlockMesh", Filename));

  if (m_pHeader->Version != MESHVERSION)
    throw MeshException(this, String::Format("The file \'%s\' has an recognised version number %x", Filename, m_pHeader->Version));

  InitializeTables();
  LoadMetaData();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::InitializeCreate(LPCTSTR Filename)
{
  m_BlockStore.InitializeCreate(Filename, BLOCKSIZE);
  m_pHeader = m_BlockStore.GetHeaderStruct<MESHHEADER>();

  ZeroMemory(m_pHeader, sizeof(MESHHEADER));
  m_pHeader->Signature = MESHSIGNATURE;
  m_pHeader->Version = MESHVERSION;
  m_pHeader->VertexDataDecl.Storage.Directory = m_BlockStore.AllocateBlock();
  m_pHeader->TriangleDataDecl.Storage.Directory = m_BlockStore.AllocateBlock();

  m_BlockStore.Flush();

  InitializeTables();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::Import(LPCTSTR Filename)
{
  Progress::Begin(String::Format("Importing %s", Path::Filename(Filename).ConstStr() ));

  m_TriangleBlockInfoResolution = 0;
  if (m_pTriangleBlockInfo)
    delete[] m_pTriangleBlockInfo;
  m_pTriangleBlockInfo = 0;
  
  if (Path::ExtensionMatches(Filename, "Ply"))
    ImportPLY(Filename);
  else if (Path::ExtensionMatches(Filename, "Bnpts"))
    ImportBNPTS(Filename);

  /* Pre-allocate some extra space in the VertexData table */
  VertexData.AddAttribute(VertexAttribute::FREE32);

  Progress::End();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::Export(LPCTSTR Filename)
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::CreateBlob(LPCTSTR Name, SIZE_T Length, LPCVOID pData)
{
  UINT i = GetBlobIndex(Name);
  if (i == MAXUINT)
    i = AllocateBlob(Name);
  
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::CreateBlob(LPCTSTR Name, LPCTSTR Filename)
{
  CreateBlob(Name, File::Open(Filename, FileMode::OPEN, FileAccess::READ));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::CreateBlob(LPCTSTR Name, Ptr<IO::File> pFile)
{
  CreateBlob(Name, pFile->Length(), pFile);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::CreateBlob(LPCTSTR Name, ULONGLONG Length, Ptr<IO::Stream> pStream)
{
  UINT i = GetBlobIndex(Name);
  if (i == MAXUINT)
    i = AllocateBlob(Name);


}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL BlockMesh::BlobExists(LPCTSTR Name)
{
  UINT i = GetBlobIndex(Name);
  if (i == MAXUINT)
    return FALSE;
  return TRUE;  
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG BlockMesh::GetBlobLength(LPCTSTR Name)
{
  UINT i = GetBlobIndex(Name);
  if (i == MAXUINT)
    throw InvalidOperationException(this, String::Format("Unable to find blob \'%s\'", Name));
  return m_pHeader->Blobs[i].Length;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::GetBlobData(LPCTSTR Name, SIZE_T Length, LPVOID pData)
{
  UINT i = GetBlobIndex(Name);
  if (i == MAXUINT)
    throw InvalidOperationException(this, String::Format("Unable to find blob \'%s\'", Name));
  //Ptr<File> pFile = OpenExtent(m_pHeader->Blobs[i].Extent);
  //pFile->ReadToMemory(pData, Length);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Stream> BlockMesh::GetBlobDataStream(LPCTSTR Name)
{
  UINT i = GetBlobIndex(Name);
  if (i == MAXUINT)
    throw InvalidOperationException(this, String::Format("Unable to find blob \'%s\'", Name));

  return new BlockStore::Table::Stream(&m_BlockStore, m_pHeader->Blobs[i].Storage.Directory, m_pHeader->Blobs[i].Storage.BlockCount, 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::SetBlobData(LPCTSTR Name, SIZE_T Length, LPCVOID pData)
{
  UINT i = GetBlobIndex(Name);
  if (i == MAXUINT)
    throw InvalidOperationException(this, String::Format("Unable to find blob \'%s\'", Name));

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::DeleteBlob(LPCTSTR Name)
{
  UINT i = GetBlobIndex(Name);
  if (i == MAXUINT)
    throw InvalidOperationException(this, String::Format("Unable to find blob \'%s\'", Name));
  /*
  if (m_pHeader->Blobs[i].Extent != 0)
    FreeExtent(m_pHeader->Blobs[i].Extent);
  ZeroMemory(&m_pHeader->Blobs[i], sizeof(BLOB));
  */
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
UINT BlockMesh::GetBlobIndex(LPCTSTR Name)
{
  StringA NameA(Name);
  for (UINT i=0; i<64; i++)
    if (CharTraits<CHAR>::StrCmp(NameA.ConstStr(), m_pHeader->Blobs[i].Name) == 0)
      return i;
  return MAXUINT;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
UINT BlockMesh::AllocateBlob(LPCTSTR Name)
{
  /*
  StringA NameA(Name);
  for (UINT i=0; i<64; i++)
    if (m_pHeader->Blobs[i].Extent == 0)
    {
      ZeroMemory(m_pHeader->Blobs[i].Name, 64);
      CopyMemory(m_pHeader->Blobs[i].Name, NameA.ConstStr(), Min<SIZE_T>(NameA.Length(), 63));
      return i;
    }
  */
  return MAXUINT;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::LoadMetaData()
{
  //if (m_pHeader->MetaDataExtent == 0)
  //  return;

  /*
  EXTENT& E = GetExtent(m_pHeader->MetaDataExtent);
  Ptr<File> pFile = OpenExtent(m_pHeader->MetaDataExtent);
  BinaryStream S(pFile);
  S >> m_MetaData;
  */
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::SaveMetaData()
{
  /*
  if (m_MetaData.Count() == 0)
  {
    if (m_pHeader->MetaDataExtent != 0)
      FreeExtent(m_pHeader->MetaDataExtent);
    m_pHeader->MetaDataExtent = 0;
    return;
  }
    
  Ptr<MemoryStream> pMemory = new MemoryStream();
  BinaryStream S(pMemory);
  S << m_MetaData;

  ULONGLONG Length = pMemory->Length();
  if (m_pHeader->MetaDataExtent == 0)
    m_pHeader->MetaDataExtent = AllocateExtent(Length);
  else
    ResizeExtent(m_pHeader->MetaDataExtent, Length);

  Ptr<File> pFile = OpenExtent(m_pHeader->MetaDataExtent);
  pMemory->Seek(0);
  pFile->WriteFromStream(pMemory, Length);
  */
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::SetMetaData(LPCTSTR Key, LPCTSTR Value)
{
  m_MetaData.Add(Key, Value);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
CONST ElementDecl VertexAttribute::POSITION3 = ElementDecl("Position", ElementDeclType::FLOAT, 3);
CONST ElementDecl VertexAttribute::POSITION2 = ElementDecl("Position", ElementDeclType::FLOAT, 2);
CONST ElementDecl VertexAttribute::NORMAL2 = ElementDecl("Normal", ElementDeclType::FLOAT, 2);
CONST ElementDecl VertexAttribute::NORMAL3 = ElementDecl("Normal", ElementDeclType::FLOAT, 3);
CONST ElementDecl VertexAttribute::DIFFUSE = ElementDecl("Diffuse", ElementDeclType::BYTE, 3);
CONST ElementDecl VertexAttribute::MEANCURVATURE = ElementDecl("MeanCurvature", ElementDeclType::FLOAT, 1);
CONST ElementDecl VertexAttribute::LOCALDEPTH = ElementDecl("LocalDepth", ElementDeclType::FLOAT, 1);
CONST ElementDecl VertexAttribute::ACCESSIBILITY = ElementDecl("Accessibility", ElementDeclType::FLOAT, 1);

CONST ElementDecl VertexAttribute::FREE1 = ElementDecl("FreeSpace", ElementDeclType::NONE, 1);
CONST ElementDecl VertexAttribute::FREE4 = ElementDecl("FreeSpace", ElementDeclType::NONE, 4);
CONST ElementDecl VertexAttribute::FREE8 = ElementDecl("FreeSpace", ElementDeclType::NONE, 8);
CONST ElementDecl VertexAttribute::FREE12 = ElementDecl("FreeSpace", ElementDeclType::NONE, 12);
CONST ElementDecl VertexAttribute::FREE16 = ElementDecl("FreeSpace", ElementDeclType::NONE, 16);
CONST ElementDecl VertexAttribute::FREE32 = ElementDecl("FreeSpace", ElementDeclType::NONE, 32);
CONST ElementDecl VertexAttribute::INDEX = ElementDecl("Index", ElementDeclType::UINT, 1);
CONST ElementDecl VertexAttribute::ZINDEX = ElementDecl("ZIndex", ElementDeclType::ULONG, 1);
CONST ElementDecl VertexAttribute::SLICEZINDEX = ElementDecl("SliceZIndex", ElementDeclType::ULONG, 1);
CONST ElementDecl VertexAttribute::FLAGS = ElementDecl("Flags", ElementDeclType::UINT, 1);
//----------------------------------------------------------------------------------------------------------------------------------------------------
CONST ElementDecl TriangleAttribute::VERTICES = ElementDecl("Vertices", ElementDeclType::UINT, 3);
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
BlockMesh::INDEXNODE::INDEXNODE()
{
  pChildren = 0;
  VertexDataOffset = 0;
  VertexDataCount = 0;
  TriangleDataOffset = 0;
  TriangleDataCount = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------
BlockMesh::OctreeIndexEnumerator BlockMesh::GetIndexEnumerator()
{
  return OctreeIndexEnumerator(GetIndexRoot());
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
CONST BlockMesh::INDEXNODE* BlockMesh::OctreeIndexEnumerator::Next()
{
  while(TRUE)
  {
    if (m_Parent.IsEmpty())
    {
      m_pNode = NULL;
      return m_pNode;
    }
    CONST INDEXNODE* pParent = m_Parent.Peek();
    PTRDIFF_T i = m_pNode - pParent->pChildren;
    if (i < 7)
    {
      m_pNode = &pParent->pChildren[i+1];
      break;
    }
    m_pNode = m_Parent.Pop();
  }

  while (m_pNode->pChildren)
  {
    m_Parent.Push(m_pNode);
    m_pNode = m_pNode->pChildren;
  }

  return m_pNode;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BlockMesh::OctreeIndexEnumerator::OctreeIndexEnumerator(CONST INDEXNODE* pRoot)
{
  m_pNode = pRoot;
  while (m_pNode->pChildren)
  {
    m_Parent.Push(m_pNode);
    m_pNode = m_pNode->pChildren;
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
