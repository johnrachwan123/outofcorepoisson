/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/Grid.cpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1631 $
 * Last Updated: $Date: 2008-09-15 07:52:23 -0700 (Mon, 15 Sep 2008) $
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

#include "Math/Grid.hpp"
#include "Math/Math.hpp"
#include "IO/File.hpp"
#include "IO/TextStream.hpp"

using namespace Bolitho;
using namespace Bolitho::IO;
using namespace Bolitho::Math;


//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Math,Grid,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Grid> Grid::Create(LPCTSTR Filename, UINT TileSize, UINT CellSize, UINT Width, UINT Height, DWORD Flags)
{
  Grid* pGrid = new Grid();
  
  pGrid->m_pFile = MemoryMappedFile::Open(Filename, FileMode::CREATE);
  
  UINT BlockSize = (TileSize * TileSize * CellSize) * 3 / 2;
  UINT BlockCount = Width * Height + 1; 

  pGrid->m_pFile->Resize((ULONGLONG)BlockCount * (ULONGLONG)BlockSize);
  pGrid->m_pView = pGrid->m_pFile->CreateView();
  pGrid->m_pHeader = (GRIDHEADER*)pGrid->m_pView->GetBasePointer();

  pGrid->m_pHeader->BlockSize = BlockSize;
  pGrid->m_pHeader->CellSize = CellSize;
  pGrid->m_pHeader->FileVersion = 0x100;
  pGrid->m_pHeader->Width = Width;
  pGrid->m_pHeader->Height = Height;
  pGrid->m_pHeader->TileSize = TileSize;
  pGrid->m_pHeader->LevelCount = Log2(TileSize);

  return pGrid;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Grid> Grid::Open(LPCTSTR Filename, DWORD Flags)
{
  Grid* pGrid = new Grid();

  pGrid->m_pFile = MemoryMappedFile::Open(Filename, FileMode::OPEN);
  pGrid->m_pView = pGrid->m_pFile->CreateView();
  pGrid->m_pHeader = (GRIDHEADER*)pGrid->m_pView->GetBasePointer();

  return pGrid;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Grid::~Grid()
{
  
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
LPCVOID Grid::GetBlockPointer(UINT x, UINT y) CONST
{
  LPCBYTE pBlock = m_pView->GetBasePointer();
  pBlock += m_pHeader->BlockSize * (x + y * m_pHeader->Width);
  return pBlock;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LPVOID Grid::GetBlockPointer(UINT x, UINT y)
{
  LPBYTE pBlock = m_pView->GetBasePointer();
  pBlock += m_pHeader->BlockSize * (x + y * m_pHeader->Width);
  return pBlock;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Grid::GetLevelOffset(UINT Level) CONST
{
  SIZE_T Offset = 0;
  UINT TileSize = m_pHeader->TileSize;
  for (UINT i=0; i<Level; i++)
  {
    Offset += TileSize*TileSize*m_pHeader->CellSize;
    TileSize >>= 1;
  }

  return Offset;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

