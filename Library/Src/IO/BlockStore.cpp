/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/IO/BlockStore.cpp $
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
#include "IO/BlockStore.hpp"

using namespace Bolitho;
using namespace Bolitho::IO;

//---------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::IO, BlockStoreException, UserException);
ImplementRuntimeType(Bolitho::IO, BlockStore, Object);
//---------------------------------------------------------------------------------------------------
BlockStore::Table::Stream::Stream(BlockStore* pStore, UINT Directory, UINT BlockCount, ULONGLONG Offset)
{
  m_pTable = new Table(pStore);
  m_pTable->Initialize(Directory, BlockCount);

  while (Offset > m_pTable->GetBlockSize())
  {
    m_CurrentBlock++;
    Offset -= m_pTable->GetBlockSize();
  }
  m_CurrentOffset = (BLOCKSIZE)Offset;
}
//---------------------------------------------------------------------------------------------------
BlockStore::Table::Stream::Stream(BlockStore::Table* pTable, ULONGLONG Offset)
  : m_pTable(pTable)
{
  while (Offset > pTable->GetBlockSize())
  {
    m_CurrentBlock++;
    Offset -= pTable->GetBlockSize();
  }
  m_CurrentOffset = (BLOCKSIZE)Offset;
}
//---------------------------------------------------------------------------------------------------
VOID BlockStore::Table::Stream::Flush()
{}
//---------------------------------------------------------------------------------------------------
VOID BlockStore::Table::Stream::Close()
{}
//---------------------------------------------------------------------------------------------------
SIZE_T BlockStore::Table::Stream::Write(LPCVOID pData, SIZE_T N)
{
  SIZE_T Offset = 0;

  while (N > 0)
  {
    if (m_CurrentOffset >= m_pTable->GetBlockSize())
    {
      m_CurrentOffset = 0;
      m_CurrentBlock++;

      if (m_CurrentBlock >= m_pTable->GetBlockCount())
        m_pTable->Resize(m_pTable->GetBlockCount() + 1);
    }

    SIZE_T BlockSize = N;
    if (BlockSize > (m_pTable->GetBlockSize() - m_CurrentOffset))
      BlockSize = m_pTable->GetBlockSize() - m_CurrentOffset;

    LPVOID pTableBlock = m_pTable->GetBlock(m_CurrentBlock);
    CopyMemory((LPBYTE)pTableBlock + m_CurrentOffset, (LPCBYTE)pData + Offset, BlockSize);

    N -= BlockSize;
    Offset += BlockSize;
    m_CurrentOffset += (BLOCKSIZE)BlockSize;
  }

  return Offset;
}
//---------------------------------------------------------------------------------------------------
SIZE_T BlockStore::Table::Stream::Read(LPVOID pData, SIZE_T N)
{
  SIZE_T Offset = 0;

  while (N > 0)
  {
    if (m_CurrentOffset >= m_pTable->GetBlockSize())
    {
      m_CurrentOffset = 0;
      m_CurrentBlock++;

      if (m_CurrentBlock >= m_pTable->GetBlockCount())
        break;
    }

    SIZE_T BlockSize = N;
    if (BlockSize > (m_pTable->GetBlockSize() - m_CurrentOffset))
      BlockSize = m_pTable->GetBlockSize() - m_CurrentOffset;

    LPCVOID pTableBlock = m_pTable->GetBlock(m_CurrentBlock);
    CopyMemory((LPBYTE)pData + Offset, (LPCBYTE)pTableBlock + m_CurrentOffset, BlockSize);

    N -= BlockSize;
    Offset += BlockSize;
    m_CurrentOffset += (BLOCKSIZE)BlockSize;
  }

  return Offset;
}
//---------------------------------------------------------------------------------------------------
BOOL BlockStore::Table::Stream::EndOfStream()
{
  return m_CurrentBlock >= m_pTable->GetBlockCount();
}
//---------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------
BlockStore::Table::Table(BlockStore* pBlockStore)
{
  m_pBlockStore = pBlockStore;
  m_pRootTranslationTable = 0;
}
//---------------------------------------------------------------------------------------------------
VOID BlockStore::Table::Initialize(BLOCKADDRESS Root, BLOCKCOUNT BlockCount)
{
  m_BlockCount = BlockCount;
  m_RootTranslationTableBlock = Root;
  m_pRootTranslationTable = (BLOCKADDRESS*)AllocateMemory(m_pBlockStore->GetBlockSize());
  CopyMemory(m_pRootTranslationTable, m_pBlockStore->GetBlock(Root), m_pBlockStore->GetBlockSize());

  UINT TableEntries = m_pBlockStore->GetBlockSize() / sizeof(BLOCKADDRESS);
  m_ppTranslationTable = new BLOCKADDRESS*[TableEntries];
  for (UINT i=0; i<TableEntries; i++)
  {
    if (m_pRootTranslationTable[i] != 0)
    {
      BLOCKADDRESS* pBlock = (BLOCKADDRESS*)AllocateMemory(m_pBlockStore->GetBlockSize());
      CopyMemory(pBlock, m_pBlockStore->GetBlock(m_pRootTranslationTable[i]), m_pBlockStore->GetBlockSize());
      m_ppTranslationTable[i] = pBlock;
    }
    else
    {
      m_ppTranslationTable[i] = 0;
    }
  }
}
//---------------------------------------------------------------------------------------------------
BlockStore::BLOCKADDRESS BlockStore::Table::UpdateDirectory(BLOCKADDRESS TableAddress, BLOCKADDRESS PhysicalAddress) CONST
{
  BLOCKADDRESS i = TableAddress / (m_pBlockStore->GetBlockSize() / 4);
  BLOCKADDRESS j = TableAddress % (m_pBlockStore->GetBlockSize() / 4);
  
  if (m_pRootTranslationTable[i] == 0)
  {
    m_pRootTranslationTable[i] = m_pBlockStore->AllocateBlock();
    BLOCKADDRESS* pBlock = (BLOCKADDRESS*)AllocateMemory(m_pBlockStore->GetBlockSize());
    CopyMemory(pBlock, m_pBlockStore->GetBlock(m_pRootTranslationTable[i]), m_pBlockStore->GetBlockSize());
    m_ppTranslationTable[i] = pBlock;
  }

  BLOCKADDRESS k = m_ppTranslationTable[i][j];
  m_ppTranslationTable[i][j] = PhysicalAddress;
  return k;
}
//---------------------------------------------------------------------------------------------------
VOID BlockStore::Table::Resize(BLOCKADDRESS BlockCount)
{
  if (BlockCount < m_BlockCount)
  {
    for (BLOCKADDRESS i=BlockCount; i<m_BlockCount; i++)
    {
      m_pBlockStore->FreeBlock(Translate(i));
      UpdateDirectory(i, 0);
    }
  }
  if (BlockCount > m_BlockCount)
  {
    for (BLOCKADDRESS i=m_BlockCount; i<BlockCount; i++)
    {
      if (Translate(i) == 0)
        UpdateDirectory(i, m_pBlockStore->AllocateBlock());
    }
  }

  m_BlockCount = BlockCount;
}
//---------------------------------------------------------------------------------------------------
LPCVOID BlockStore::Table::GetBlock(BLOCKADDRESS Address) CONST
{
  BLOCKADDRESS PhysicalAddress = Translate(Address);
  if (PhysicalAddress == 0)
    return 0;
  else
    return m_pBlockStore->GetBlock(PhysicalAddress);
}
//---------------------------------------------------------------------------------------------------
LPVOID BlockStore::Table::GetBlock(BLOCKADDRESS Address)
{
  BLOCKADDRESS PhysicalAddress = Translate(Address);
  if (PhysicalAddress == 0)
    return 0;
  else
    return m_pBlockStore->GetBlock(PhysicalAddress);
}
//---------------------------------------------------------------------------------------------------
VOID BlockStore::Table::Lock(BLOCKADDRESS Address)
{
  m_pBlockStore->Lock(Translate(Address));
}
//---------------------------------------------------------------------------------------------------
VOID BlockStore::Table::Unlock(BLOCKADDRESS Address)
{
  m_pBlockStore->Unlock(Translate(Address));
}
//---------------------------------------------------------------------------------------------------
BlockStore::BLOCKADDRESS BlockStore::Table::Translate(BLOCKADDRESS TableAddress) CONST
{
  if (m_TranslationLookup[0] == TableAddress)
    return m_TranslationLookup[1];
  
  BLOCKADDRESS Address = 0;

  BLOCKADDRESS i = TableAddress / (m_pBlockStore->GetBlockSize() / 4);
  BLOCKADDRESS j = TableAddress % (m_pBlockStore->GetBlockSize() / 4);
  
  if (m_ppTranslationTable[i] == 0)
    return 0;

  Address = m_ppTranslationTable[i][j];

  m_TranslationLookup[0] = TableAddress;
  m_TranslationLookup[1] = Address;

  return Address;
}
//---------------------------------------------------------------------------------------------------
VOID BlockStore::Table::Flush()
{
  if (m_pRootTranslationTable == 0)
    return;

  UINT TableEntries = m_pBlockStore->GetBlockSize() / sizeof(BLOCKADDRESS);
  for (UINT i=0; i<TableEntries; i++)
  {
    if (m_pRootTranslationTable[i] != 0)
    {
      BOOL HasBlocks = FALSE;
      for (UINT j=0; j<TableEntries; j++)
        if (m_ppTranslationTable[i][j] != 0)
        { HasBlocks = TRUE; break; }

      if (!HasBlocks)
      {
        m_pBlockStore->FreeBlock(m_pRootTranslationTable[i]);
        m_pRootTranslationTable[i] = 0;
      }
      else
      {
        CopyMemory(m_pBlockStore->GetBlock(m_pRootTranslationTable[i]), m_ppTranslationTable[i], m_pBlockStore->GetBlockSize());
      }
    }
  }

  CopyMemory(m_pBlockStore->GetBlock(m_RootTranslationTableBlock), m_pRootTranslationTable, m_pBlockStore->GetBlockSize());
}
//---------------------------------------------------------------------------------------------------
BlockStore::Table::~Table()
{
  if (m_pRootTranslationTable == 0)
    return;

  FreeMemory(m_pRootTranslationTable);
  UINT TableEntries = m_pBlockStore->GetBlockSize() / sizeof(BLOCKADDRESS);
  for (UINT i=0; i<TableEntries; i++)
    if (m_ppTranslationTable[i] != 0)
      FreeMemory(m_ppTranslationTable[i]);
  FreeMemory(m_ppTranslationTable);
}
//---------------------------------------------------------------------------------------------------
Ptr<BlockStore::Table::Stream> BlockStore::Table::GetStream(ULONGLONG Offset)
{
  return new Stream(this, Offset);
}
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
BlockStore::~BlockStore()
{
  if (m_Header.Version == 0)
  {
    for (SIZE_T i=0; i<m_pBlocks.Length(); i++)
      if (m_pBlocks[i] != 0)
        FreeMemory(m_pBlocks[i]);
  }
  else
  {
    Flush();
    for (UINT i=0; i<32; i++)
      if (m_ppBlockBitmaps[i] != 0)
        FreeMemory(m_ppBlockBitmaps[i]);
  }

  if (m_ppBlockBitmaps)
    delete[] m_ppBlockBitmaps;
  m_ppBlockBitmaps = 0;
}
//---------------------------------------------------------------------------------------------------
VOID BlockStore::Flush()
{
  if (m_Header.Version == 0)
    return;

  m_pFile->Seek(0);
  m_pFile->Write(&m_Header, sizeof(m_Header));
  
  for (UINT i=0; i<32; i++)
    if (m_ppBlockBitmaps[i] != 0)
      CopyMemory(GetBlock(m_Header.AllocationBitmapBlock[i]), m_ppBlockBitmaps[i], m_Header.BlockSize);
}
//---------------------------------------------------------------------------------------------------
VOID BlockStore::Resize(BLOCKADDRESS BlockCount)
{
  if (BlockCount <= m_Header.BlockCount)
    return;

  if (m_Header.Version == 0)
  {
    m_pBlocks.Resize(BlockCount);
  }
  else
  {
    ULONGLONG NewSize = (ULONGLONG)BlockCount * m_Header.BlockSize;

    m_pView = 0;
    m_pFile->Resize(NewSize);
    m_pView = m_pFile->MapView(PageAccess::READWRITE);
  }

  m_Header.FreeBlockCount += (BlockCount-m_Header.BlockCount);
  m_Header.BlockCount = BlockCount;
}
//---------------------------------------------------------------------------------------------------
BlockStore::BLOCKADDRESS BlockStore::AllocateBlock(BOOL InitializeToZero)
{
  return AllocateBlock(InitializeToZero, TRUE, TRUE);
}
//---------------------------------------------------------------------------------------------------
BlockStore::BLOCKADDRESS BlockStore::AllocateBlock(BOOL InitializeToZero, BOOL IncreaseBlockSpace, BOOL IncreaseBitmapSpace)
{
  if (m_Header.FreeBlockCount == 0 && IncreaseBlockSpace)
    if (m_Header.BlockCount < 1024)
      Resize(m_Header.BlockCount << 1);
    else
      Resize(m_Header.BlockCount + (m_Header.BlockCount >> 2));

  for (SIZE_T i=0; i<m_Header.BlockSize / sizeof(BLOCKADDRESS); i++)
  {
    if (m_Header.AllocationBitmapBlock[i+1] == 0 && IncreaseBitmapSpace)
    {
      m_Header.AllocationBitmapBlock[i+1] = AllocateBlock(TRUE, FALSE, FALSE);
      DWORD* pBlockBitmap = (DWORD*)AllocateMemory(m_Header.BlockSize);
      ZeroMemory(pBlockBitmap, m_Header.BlockSize);
      m_ppBlockBitmaps[i+1] = pBlockBitmap;
      
      Flush();
    }

    DWORD Bit = 0;
    for (BLOCKADDRESS j=0; j<m_Header.BlockSize / sizeof(DWORD); j++)
    {
      if (BitScanForward(&Bit, ~m_ppBlockBitmaps[i][j]))
      {
        m_ppBlockBitmaps[i][j] = (m_ppBlockBitmaps[i][j] | 1<<Bit);
        m_Header.FreeBlockCount--;
        BLOCKADDRESS BlockNumber = (BLOCKADDRESS)(i*m_Header.BlockSize*8 + j*sizeof(DWORD)*8 + Bit);
        
        if (m_Header.Version == 0 && m_pBlocks[BlockNumber] == 0)
          m_pBlocks[BlockNumber] = AllocateMemory(m_Header.BlockSize);

        if (InitializeToZero)
          ZeroMemory(GetBlock(BlockNumber), m_Header.BlockSize);
        
        return BlockNumber;
      }
    }
  }
  throw OutOfMemoryException(this, TEXT("Unable to Allocate a new block"));
}
//---------------------------------------------------------------------------------------------------
VOID BlockStore::FreeBlock(BLOCKADDRESS Block)
{
  BLOCKADDRESS i = Block / (m_Header.BlockSize*8);
  Block -= i;
  
  BLOCKADDRESS j = Block / 32;
  Block -= j;
  
  BLOCKADDRESS k = Block;

  m_ppBlockBitmaps[i][j] &= ~(1<<k);
  m_Header.FreeBlockCount++;

  if (m_Header.Version == 0)
  {
    FreeMemory(m_pBlocks[Block]);
    m_pBlocks[Block] = 0;
  }
}
//---------------------------------------------------------------------------------------------------
VOID BlockStore::Lock(BLOCKADDRESS Block)
{
  //m_pView->Lock(Block*m_pHeader->BlockSize, m_pHeader->BlockSize);
}
//---------------------------------------------------------------------------------------------------
VOID BlockStore::Unlock(BLOCKADDRESS Block)
{
  //m_pView->Unlock(Block*m_pHeader->BlockSize, m_pHeader->BlockSize);
}
//---------------------------------------------------------------------------------------------------
VOID BlockStore::Close()
{
  //TODO: Implement
}
//---------------------------------------------------------------------------------------------------
BlockStore::BlockStore() 
{
  ZeroMemory(&m_Header, sizeof(m_Header));
  m_ppBlockBitmaps = 0;
}
//---------------------------------------------------------------------------------------------------
VOID BlockStore::InitializeInMemory(BLOCKSIZE BlockSize)
{
  m_Header.Signature = 0;
  m_Header.Version = 0xffff;
  
  ZeroMemory(&m_Header, sizeof(m_Header));
  m_Header.Signature = 0;
  m_Header.Version = 0;
  m_Header.BlockCount = 4;
  m_Header.FreeBlockCount = 1;
  m_Header.BlockSize = BlockSize;
  m_Header.AllocationBitmapBlock[0] = 1;
  m_Header.AllocationBitmapBlock[1] = 2;

  m_pBlocks.Resize(4);
  for (SIZE_T i=0; i<m_pBlocks.Length(); i++)
    m_pBlocks[i] = AllocateMemory(m_Header.BlockSize);

  m_ppBlockBitmaps = new DWORD*[32];
  ZeroMemory(m_ppBlockBitmaps, sizeof(DWORD*)*32);
  m_ppBlockBitmaps[0] = (DWORD*)m_pBlocks[1];
  m_ppBlockBitmaps[1] = (DWORD*)m_pBlocks[2];

  m_ppBlockBitmaps[0][0] = 0x7;

}
//---------------------------------------------------------------------------------------------------
VOID BlockStore::InitializeCreate(LPCTSTR Filename, BLOCKSIZE BlockSize)
{
  m_pFile = File::Open(Filename, FileMode::CREATE, FileAccess::READWRITE);
  m_pFile->Resize(4*BlockSize);
  m_pView = m_pFile->MapView(PageAccess::READWRITE);

  ZeroMemory(&m_Header, sizeof(m_Header));
  m_Header.Signature = BLOCKSTORESIGNATURE;
  m_Header.Version = BLOCKSTOREVERSION;
  m_Header.BlockCount = 4;
  m_Header.FreeBlockCount = 1;
  m_Header.BlockSize = BlockSize;
  m_Header.AllocationBitmapBlock[0] = 1;
  m_Header.AllocationBitmapBlock[1] = 2;

  m_pFile->Seek(0);
  m_pFile->Write(&m_Header, sizeof(m_Header));

  m_ppBlockBitmaps = new DWORD*[32];
  ZeroMemory(m_ppBlockBitmaps, sizeof(DWORD*)*32);

  m_ppBlockBitmaps[0] = (DWORD*)AllocateMemory(BlockSize);
  ZeroMemory(m_ppBlockBitmaps[0], BlockSize);
  m_ppBlockBitmaps[1] = (DWORD*)AllocateMemory(BlockSize);
  ZeroMemory(m_ppBlockBitmaps[1], BlockSize);

  m_ppBlockBitmaps[0][0] = 0x7;
  
  m_pFile->Seek(BlockSize);
  m_pFile->Write(m_ppBlockBitmaps[0], BlockSize);
  m_pFile->Write(m_ppBlockBitmaps[1], BlockSize);
}
//---------------------------------------------------------------------------------------------------
VOID BlockStore::InitializeOpen(LPCTSTR Filename)
{
  m_pFile = File::Open(Filename, FileMode::OPEN, FileAccess::READWRITE);
  m_pView = m_pFile->MapView(PageAccess::READWRITE);

  m_pFile->Read(&m_Header, sizeof(m_Header));

  m_ppBlockBitmaps = new DWORD*[32];
  ZeroMemory(m_ppBlockBitmaps, sizeof(DWORD*)*32);

  for (UINT i=0; i<32; i++)
  {
    if (m_Header.AllocationBitmapBlock[i] != 0)
    {
      m_ppBlockBitmaps[i] = (DWORD*)AllocateMemory(m_Header.BlockSize);
      CopyMemory(m_ppBlockBitmaps[i], GetBlock(m_Header.AllocationBitmapBlock[i]), m_Header.BlockSize);
    }
  }

}
//---------------------------------------------------------------------------------------------------

