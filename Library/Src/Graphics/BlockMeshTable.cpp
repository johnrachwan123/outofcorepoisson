/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/BlockMeshTable.cpp $
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
BlockMesh::DataTable::DataTable(BlockMesh& M) : m_Mesh(M), m_Table(&M.m_BlockStore)
{
  m_pHeader = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BlockMesh::DataTable::~DataTable()
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::DataTable::Initialize(TABLEHEADER *pHeader)
{
  m_pHeader = pHeader;

  if (m_pHeader == 0)
    return;

  m_Table.Initialize(pHeader->Storage.Directory, pHeader->Storage.BlockCount);

  m_Declaration.Resize(m_pHeader->AttributeCount);
  for (SIZE_T i=0; i<m_Declaration.Length(); i++)
    m_Declaration[i] = ElementDecl(m_pHeader->ElementDecl[i].Name, m_pHeader->ElementDecl[i].Type, m_pHeader->ElementDecl[i].Length);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
UINT BlockMesh::DataTable::GetBlockRowCount(UINT i) CONST
{
  if (i == m_pHeader->Storage.BlockCount-1)
    return m_pHeader->Count % m_pHeader->CountPerBlock;
  else
    return m_pHeader->CountPerBlock;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL BlockMesh::DataTable::HasAttribute(CONST ElementDecl& A) CONST
{
  for (SIZE_T i=0; i<m_Declaration.Length(); i++)
    if (m_Declaration[i] == A)
      return TRUE;
  return FALSE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
UINT BlockMesh::DataTable::GetAttributeOffset(CONST ElementDecl& A) CONST
{
  UINT Offset = 0;
  for (SIZE_T i=0; i<m_Declaration.Length(); i++)
    if (m_Declaration[i] == A)
      return Offset;
    else
      Offset += m_Declaration[i].GetSize();
  return -1;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
UINT BlockMesh::DataTable::GetAttributeOffset(UINT j) CONST
{
  UINT Offset = 0;
  for (UINT i=0; i<j; i++)
    Offset += m_Declaration[i].GetSize();

  return Offset;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
UINT BlockMesh::DataTable::GetAttributeIndex(CONST ElementDecl& A) CONST
{
  for (SIZE_T i=0; i<m_Declaration.Length(); i++)
    if (m_Declaration[i] == A)
      return (UINT)i;
  return -1;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::DataTable::AddAttribute(CONST ElementDecl& A)
{
  /* First, try to find free space */
  for (SIZE_T i=0; i<m_Declaration.Length(); i++)
  {
    if (m_pHeader->ElementDecl[i].Type == ElementDeclType::NONE)
    {
      if (m_pHeader->ElementDecl[i].Length < A.GetSize())
      {
        continue;
      }
      else if (m_pHeader->ElementDecl[i].Length == A.GetSize())
      {
        m_Declaration[i] = A;
      }
      else if (m_pHeader->ElementDecl[i].Length > A.GetSize())
      {
        m_Declaration[i].Length -= A.GetSize();
        m_Declaration.Insert(A, i);
        m_pHeader->AttributeCount++;
      }

      for (SIZE_T i=0; i<m_Declaration.Length(); i++)
      {
        m_pHeader->ElementDecl[i].Type = m_Declaration[i].Type;
        m_pHeader->ElementDecl[i].Length = m_Declaration[i].Length;
        ZeroMemory(m_pHeader->ElementDecl[i].Name, 32);
        CopyMemory(m_pHeader->ElementDecl[i].Name, m_Declaration[i].Name, Min<SIZE_T>(m_Declaration[i].Name.Length(), 32));
      }
      return;

    }
  }

  /* Otherwise, make space */
  UINT OldElementSize = m_pHeader->ElementSize;
  UINT NewElementSize = m_pHeader->ElementSize + A.GetSize();
  UINT OldCountPerBlock = m_pHeader->CountPerBlock;
  UINT OldBlockCount = m_pHeader->Storage.BlockCount;
  UINT OldLastCountPerBlock = OldCountPerBlock == 0 ? 0 : m_pHeader->Count % OldCountPerBlock;

  m_Declaration.Add(A);
  m_pHeader->ElementSize += A.GetSize();
  m_pHeader->CountPerBlock = BlockMesh::BLOCKSIZE / m_pHeader->ElementSize;
  for (SIZE_T i=0; i<m_Declaration.Length(); i++)
  {
    m_pHeader->ElementDecl[i].Type = m_Declaration[i].Type;
    m_pHeader->ElementDecl[i].Length = m_Declaration[i].Length;
    ZeroMemory(m_pHeader->ElementDecl[i].Name, 32);
    CopyMemory(m_pHeader->ElementDecl[i].Name, m_Declaration[i].Name, Min<SIZE_T>(m_Declaration[i].Name.Length(), 32));
  }
  m_pHeader->AttributeCount = (UINT)m_Declaration.Length();
  Resize(m_pHeader->Count);

  for (UINT i=OldBlockCount-1; i<OldBlockCount; i--)
  {
    UINT k=i*OldCountPerBlock;
    LPBYTE pData = (LPBYTE)GetBlockData(i) + OldElementSize * OldCountPerBlock;
    for (UINT j=OldCountPerBlock-1; j<OldCountPerBlock; j--)
    {
      pData -= OldElementSize;
      if (k+j >= m_pHeader->Count)
        continue;

      LPBYTE pNewRow = Row<BYTE>(k+j);
      CopyMemory(pNewRow, pData, OldElementSize);
      ZeroMemory(pNewRow + OldElementSize, NewElementSize-OldElementSize);
    }
  }

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::DataTable::SetElementDecl(CONST Array<ElementDecl>& Decl)
{
  throw NotImplementedException(&m_Mesh);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::DataTable::RemoveAttribute(CONST ElementDecl& A)
{
  for (SIZE_T i=0; i<m_Declaration.Length(); i++)
  {
    if (m_Declaration[i] == A)
    {
      m_Declaration[i] = VertexAttribute::FREE1;
      m_Declaration[i].Length = A.GetSize();
      break;
    }
  } 

  for (SIZE_T i=0; i<m_Declaration.Length(); i++)
  {
    m_pHeader->ElementDecl[i].Type = m_Declaration[i].Type;
    m_pHeader->ElementDecl[i].Length = m_Declaration[i].Length;
    ZeroMemory(m_pHeader->ElementDecl[i].Name, 32);
    CopyMemory(m_pHeader->ElementDecl[i].Name, m_Declaration[i].Name, Min<SIZE_T>(m_Declaration[i].Name.Length(), 32));
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::DataTable::RemoveAllAttributes()
{
  m_Declaration.Resize(0);
  ZeroMemory(m_pHeader->ElementDecl, sizeof(ELEMENTDECL)*32);
  m_pHeader->AttributeCount = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
CONST ElementDecl& BlockMesh::DataTable::GetAttribute(UINT i)
{
  return m_Declaration[i];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
CONST Array<ElementDecl>& BlockMesh::DataTable::GetAttributes() CONST
{
  return m_Declaration;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::DataTable::Resize(UINT Count)
{
  UINT BlockCount = (m_pHeader->CountPerBlock == 0) ? 0 : RoundUp(Count, m_pHeader->CountPerBlock) / m_pHeader->CountPerBlock;
  m_Table.Resize(BlockCount);
  m_pHeader->Storage.BlockCount = m_Table.GetBlockCount();
  m_pHeader->Count = Count;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::DataTable::Load(Ptr<IO::Stream> pSource, UINT TableOffset, UINT Length)
{
  TableOffset *= m_pHeader->ElementSize;
  Length *= m_pHeader->ElementSize;

  UINT BlockSize = m_pHeader->CountPerBlock * m_pHeader->ElementSize;
  
  UINT Block = TableOffset / BlockSize;
  UINT Offset = TableOffset % BlockSize;

  while(Length > 0) 
  {
    UINT ReadBlockSize = Min(BlockSize-Offset, Length);
    pSource->Read((LPBYTE)GetBlockData(Block) + Offset, ReadBlockSize);
    Length -= ReadBlockSize;
    Offset = 0;
    Block++;
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::DataTable::Dump(Ptr<IO::Stream> pDestination, UINT TableOffset, UINT Length)
{
  TableOffset *= m_pHeader->ElementSize;
  Length *= m_pHeader->ElementSize;

  UINT BlockSize = m_pHeader->CountPerBlock * m_pHeader->ElementSize;
  
  UINT Block = TableOffset / BlockSize;
  UINT Offset = TableOffset % BlockSize;

  while(Length > 0) 
  {
    UINT ReadBlockSize = Min(BlockSize-Offset, Length);
    pDestination->Write((LPBYTE)GetBlockData(Block) + Offset, ReadBlockSize);
    Length -= ReadBlockSize;
    Offset = 0;
    Block++;
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::DataTable::Load(LPCVOID pSource, UINT TableOffset, UINT Length)
{
  TableOffset *= m_pHeader->ElementSize;
  Length *= m_pHeader->ElementSize;

  UINT BlockSize = m_pHeader->CountPerBlock * m_pHeader->ElementSize;
  
  UINT Block = TableOffset / BlockSize;
  UINT Offset = TableOffset % BlockSize;

  LPCBYTE pData = (LPCBYTE)pSource;
  while(Length > 0) 
  {
    UINT ReadBlockSize = Min(BlockSize-Offset, Length);
    CopyMemory((LPBYTE)GetBlockData(Block) + Offset, pData, ReadBlockSize);
    Length -= ReadBlockSize;
    pData += ReadBlockSize;
    Offset = 0;
    Block++;
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::DataTable::Dump(LPVOID pDestination, UINT TableOffset, UINT Length)
{
  TableOffset *= m_pHeader->ElementSize;
  Length *= m_pHeader->ElementSize;

  UINT BlockSize = m_pHeader->CountPerBlock * m_pHeader->ElementSize;
  
  UINT Block = TableOffset / BlockSize;
  UINT Offset = TableOffset % BlockSize;

  LPBYTE pData = (LPBYTE)pDestination;
  while(Length > 0) 
  {
    UINT ReadBlockSize = Min(BlockSize-Offset, Length);
    CopyMemory(pData, (LPBYTE)GetBlockData(Block) + Offset, ReadBlockSize);
    Length -= ReadBlockSize;
    pData += ReadBlockSize;
    Offset = 0;
    Block++;
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BlockMesh::DataTable::Hint(UINT Offset, UINT Length, DWORD Hint)
{
  for (UINT i=Offset; i<Offset+Length; i++)
  {
    if (Hint == TableHint::FINISHED)
      VirtualUnlock(m_Table.GetBlock(i), m_Table.GetBlockSize());
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
UINT BlockMesh::DataTable::Append(LPVOID pRecordData)
{
  UINT i = m_pHeader->Count++;

  UINT j = i / m_pHeader->CountPerBlock;
  UINT k = i % m_pHeader->CountPerBlock;

  LPBYTE pData = (LPBYTE)GetBlockData(j);
  if (pData == 0)
  {
    m_Table.UpdateDirectory(j, m_Table.AllocateBlock(TRUE));
    pData = (LPBYTE)GetBlockData(j);
  }

  pData += m_pHeader->ElementSize * k;
  CopyMemory(pData, pRecordData, m_pHeader->ElementSize);

  return i;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

