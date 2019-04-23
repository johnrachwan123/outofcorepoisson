/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/Grid.cpp $
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
#include "IO/Path.hpp"
#include "Graphics/Grid.hpp"
#include "Graphics/Image.hpp"

using namespace Bolitho;
using namespace Bolitho::IO;
using namespace Bolitho::Graphics;

//---------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Graphics,GridException,UserException);
ImplementRuntimeType(Bolitho::Graphics,Grid,Object);
//---------------------------------------------------------------------------------------------------
Grid::Grid()
{
  m_BoundaryConditions = GridBoundary::NONE;
  ZeroMemory(m_pGridData, sizeof(m_pGridData));
  
  InitializeInMemory();
}
//---------------------------------------------------------------------------------------------------
Grid::Grid(LPCTSTR Filename, DWORD GridMode, DWORD GridAccess)
{
  m_BoundaryConditions = GridBoundary::NONE;
  ZeroMemory(m_pGridData, sizeof(m_pGridData));

  if (GridMode == GridMode::CREATE)
    InitializeCreate(Filename, 0);
  else if (GridMode == GridMode::OPEN)
    InitializeOpen(Filename, GridAccess);
}
//---------------------------------------------------------------------------------------------------
Grid::Grid(CONST Grid& G)
{
  Copy(G, FALSE);
}
//---------------------------------------------------------------------------------------------------
Grid& Grid::operator= (CONST Grid& G)
{
  Copy(G, FALSE);
  return *this;
}
//---------------------------------------------------------------------------------------------------
Grid::~Grid()
{
  Free();    
}
//---------------------------------------------------------------------------------------------------
VOID Grid::Copy(CONST Grid& G, BOOL DeepCopy)
{
  if (DeepCopy)
  {
    Resize(G.GetWidth(), G.GetHeight());
    Assert(FALSE);
  }
  else
  {
    Free();
    m_pHeader = G.m_pHeader;
    if (G.m_pHeader->RefCount > 0)
    {
      m_pHeader->RefCount++;
      for (UINT i=0; i<32; i++)
        m_pGridData[i] = G.m_pGridData[i];
    }
    else
    {
      m_pHeader = G.m_pHeader;
      m_pFile = G.m_pFile;
      m_pMappedView = G.m_pMappedView;
      for (UINT i=0; i<32; i++)
        m_pGridData[i] = G.m_pGridData[i];
    }
  }
}
//---------------------------------------------------------------------------------------------------
VOID Grid::Free()
{
  if (m_pHeader == 0)
    return;

  if (m_pHeader->RefCount > 0)
  {
    m_pHeader->RefCount--;
    if (m_pHeader->RefCount == 0)
    {
      for (UINT i=0; i<m_pHeader->LevelCount; i++)
        FreeMemory(m_pGridData[i]);
      FreeMemory(m_pHeader);
      m_pHeader = 0;
      ZeroMemory(m_pGridData, sizeof(m_pGridData));
    }
  }
  else
  {
    m_pMappedView = 0;
    m_pFile = 0;
    m_pHeader = 0;
    ZeroMemory(m_pGridData, sizeof(m_pGridData));
  }
}
//---------------------------------------------------------------------------------------------------
VOID Grid::InitializeInMemory()
{
  m_pHeader = new GRIDHEADER();
  ZeroMemory(m_pHeader, sizeof(GRIDHEADER));
  m_pHeader->GridVersion = GRIDVERSION;
  m_pHeader->Signature = GRIDSIGNATURE;
  m_pHeader->DataLayout = GridLayout::ROWMAJOR;
}
//---------------------------------------------------------------------------------------------------
VOID Grid::InitializeOpen(LPCTSTR Filename, DWORD Flags)
{
  using Math::Log2;

  m_pFile = File::Open(Filename, FileMode::OPEN, Flags);
  m_pMappedView = m_pFile->MapView(PageAccess::READWRITE);

  InitializeMapping();
}
//---------------------------------------------------------------------------------------------------
VOID Grid::InitializeCreate(LPCTSTR Filename, DWORD Flags)
{
  using Math::Log2;

  m_pFile = File::Open(Filename, FileMode::CREATE, PageAccess::READWRITE);
  m_pFile->Resize(sizeof(GRIDHEADER));
  m_pMappedView = m_pFile->MapView(PageAccess::READWRITE);

  m_pHeader = (GRIDHEADER*)m_pMappedView->GetBaseAddress();
  ZeroMemory(m_pHeader, sizeof(GRIDHEADER));
  m_pHeader->GridVersion = GRIDVERSION;
  m_pHeader->Signature = GRIDSIGNATURE;

  InitializeMapping();
}
//---------------------------------------------------------------------------------------------------
VOID Grid::InitializeMapping()
{
  m_pHeader = (GRIDHEADER*)m_pMappedView->GetBaseAddress();
  
  if (m_pHeader->Signature != GRIDSIGNATURE)
    throw GridException(this, "The GridSignature is incorrect");

  if (m_pHeader->GridVersion != GRIDVERSION)
    throw GridException(this, "The GridVersion is for this grid is not supported");

  for (UINT i=0; i<m_pHeader->LevelCount; i++)
  {
    if (m_pHeader->GridDataOffset[i] == 0)
      throw GridException(this, String::Format("The LevelCount is %d, but level %d has no data segment", m_pHeader->LevelCount, i));

    m_pGridData[i] = (LPBYTE)m_pMappedView->GetBaseAddress() + m_pHeader->GridDataOffset[i];
    m_GridRowStride[i] = m_pHeader->Width[i] * m_pHeader->ElementSize;
  }
}
//---------------------------------------------------------------------------------------------------
VOID Grid::Import(LPCTSTR Filename, DWORD Flags)
{
  /* Fail if the import file doesn't exist */
  if (!File::Exists(Filename))
    throw GridException(this, String::Format("The file \"%s\" cannot be found", Filename));

  /* Import from an image */
  if (Path::ExtensionMatches(Filename, "Png") || 
      Path::ExtensionMatches(Filename, "Bmp") || 
      Path::ExtensionMatches(Filename, "Jpeg") || Path::ExtensionMatches(Filename, "Jpg") ||
      Path::ExtensionMatches(Filename, "Wdp") ||
      Path::ExtensionMatches(Filename, "Tiff"))
  {
    DWORD PixelFormat;
    
    if (IsFlagSet(Flags, GridFlags::FORCEELEMENTTYPE))
    {
      if (m_pHeader->ElementSize == 4)
        PixelFormat = PixelFormat::R32F;
      if (m_pHeader->ElementSize == 12)
        PixelFormat = PixelFormat::R32FG32FB32F;
      if (m_pHeader->ElementSize == 16)
        PixelFormat = PixelFormat::R32FG32FB32FA32F;
    }
    else
    {
      Array<ElementDecl> Decl;
      PixelFormat = Image::GetImagePixelFormat(Filename);
      switch(Image::GetPixelChannels(PixelFormat))
      {
      case 1:
        PixelFormat = PixelFormat::R32F; Decl.Add(ElementDecl("Value", ElementDeclType::FLOAT, 1)); break;
      case 3:
        PixelFormat = PixelFormat::R32FG32FB32F; Decl.Add(ElementDecl("Value", ElementDeclType::FLOAT, 3)); break;
      case 4:
        PixelFormat = PixelFormat::R32FG32FB32FA32F; Decl.Add(ElementDecl("Value", ElementDeclType::FLOAT, 4)); break;
      };
      SetElementType(Decl);
    }

    Image I(Filename, PixelFormat);

    UINT Width = I.Width();
    UINT Height = I.Height();

    Resize(Width, Height);

    if (m_pHeader->DataLayout == GridLayout::ROWMAJOR)
      CopyMemory(m_pGridData[0], &I.Pixel<BYTE>(0,0), Width*Height*m_pHeader->ElementSize);

    switch(Image::GetPixelChannels(PixelFormat))
    {
    case 1:
      GenerateMipMaps<ColorR32F>(); break;
    case 3:
      GenerateMipMaps<ColorRGB96F>(); break;
    case 4:
      GenerateMipMaps<ColorRGBA128F>(); break;
    };

  }

}
//---------------------------------------------------------------------------------------------------
VOID Grid::Export(LPCTSTR Filename, DWORD Flags)
{
  /* Import from an image */
  if (Path::ExtensionMatches(Filename, "Png") || 
      Path::ExtensionMatches(Filename, "Bmp") || 
      Path::ExtensionMatches(Filename, "Jpeg") || Path::ExtensionMatches(Filename, "Jpg") ||
      Path::ExtensionMatches(Filename, "Wdp") ||
      Path::ExtensionMatches(Filename, "Tiff"))
  {
    DWORD PixelFormat = 0;
    if (m_pHeader->ElementSize == 4)
      PixelFormat = PixelFormat::R32F;
    if (m_pHeader->ElementSize == 12)
      PixelFormat = PixelFormat::R32FG32FB32F;
    if (m_pHeader->ElementSize == 16)
      PixelFormat = PixelFormat::R32FG32FB32FA32F;

    Image I(GetWidth(), GetHeight(), PixelFormat);

    if (m_pHeader->DataLayout == GridLayout::ROWMAJOR)
      CopyMemory(&I.Pixel<BYTE>(0,0), m_pGridData[0], GetWidth()*GetHeight()*m_pHeader->ElementSize);



    I.Save(Filename);
  }
  
}
//---------------------------------------------------------------------------------------------------
VOID Grid::Flush()
{
  //if (m_pMappedView)
  //  m_pMappedView->Flush();
}
//---------------------------------------------------------------------------------------------------
VOID Grid::SetMetaData(LPCTSTR Key, LPCTSTR Value)
{
  m_MetaData.Add(Key, Value);
}
//---------------------------------------------------------------------------------------------------
String Grid::GetMetaData(LPCTSTR Key)
{
  if (m_MetaData.Contains(Key))
    return m_MetaData[Key];
  else
    return String::Empty;
}
//---------------------------------------------------------------------------------------------------
VOID Grid::RemoveMetaData(LPCTSTR Key)
{
  m_MetaData.Remove(Key);
}
//---------------------------------------------------------------------------------------------------
VOID Grid::RemoveAllMetaData()
{
  m_MetaData.RemoveAll();
}
//---------------------------------------------------------------------------------------------------
VOID Grid::Resize(UINT Width, UINT Height)
{
  if (Width == GetWidth() && Height == GetHeight())
    return;

  using Math::Log2;

  UINT BlockWidth, BlockHeight;
  ULONGLONG GridDataLength;

  /* Determine the amount of storage space required */
  if (m_pHeader->DataLayout == GridLayout::ROWMAJOR)
  {
    BlockWidth = 1;
    BlockHeight = 1;
    GridDataLength = (ULONGLONG)Width * Height * m_pHeader->ElementSize;
  }

  if (m_pMappedView)
  {
    /* Unmap the grid data, resize and remap */
    m_pMappedView = 0;
    m_pFile->Resize(sizeof(GRIDHEADER) + GridDataLength);
    m_pMappedView = m_pFile->MapView(PageAccess::READWRITE);

    m_pHeader = (GRIDHEADER*)m_pMappedView->GetBaseAddress();
    m_pGridData[0] = m_pHeader + 1;
    
    m_pHeader->GridDataOffset[0] = sizeof(GRIDHEADER);
  }
  else
  {
    if (m_pGridData[0])
      FreeMemory(m_pGridData[0]);
    m_pGridData[0] = AllocateMemory((SIZE_T)GridDataLength);
    m_pHeader->GridDataOffset[0] = 0;
  }

  m_pHeader->GridDataLength[0] = GridDataLength;
  m_pHeader->LevelCount = 1;
  m_pHeader->Width[0] = Width;
  m_pHeader->Height[0] = Height;
  //m_pHeader->BlockWidth = BlockWidth;
  //m_pHeader->BlockHeight = BlockHeight;

  //m_BlockSizeBytes = m_pHeader->BlockSize * m_pHeader->ElementSize;
  //m_GridBlockStride = m_pHeader->BlockSize * m_pHeader->BlockSize * m_pHeader->ElementSize * m_pHeader->Width;
  for (UINT i=0; i<m_pHeader->LevelCount; i++)
    m_GridRowStride[i] = m_pHeader->Width[i] * m_pHeader->ElementSize;
  //m_GridBlockRowStride = m_pHeader->BlockWidth * m_pHeader->ElementSize;

  //m_BlockAddressShift = Log2(m_pHeader->BlockSize);
  //m_BlockOffsetMask = m_pHeader->BlockSize - 1;

}
//---------------------------------------------------------------------------------------------------
VOID Grid::SetGridLayout(DWORD LayoutType, UINT Param)
{
  throw NotImplementedException(this);
}
//---------------------------------------------------------------------------------------------------
VOID Grid::LockBlock(UINT i, UINT j)
{
  throw NotImplementedException(this);
}
//---------------------------------------------------------------------------------------------------
VOID Grid::UnlockBlock(UINT i, UINT j)
{
  throw NotImplementedException(this);
}
//---------------------------------------------------------------------------------------------------
VOID Grid::SetElementType(Array<ElementDecl> Decl)
{
  m_pHeader->ElementCount = (UINT)Decl.Length();
  ZeroMemory(&m_pHeader->ElementDecl[0], 32*sizeof(ELEMENTDECL));

  UINT ElementSize = 0;
  for (SIZE_T i=0; i<Decl.Length(); i++)
  {
    StringA Name = Decl[i].Name;
    CopyMemory(m_pHeader->ElementDecl[i].Name, Name, Min<SIZE_T>(32, Name.Length()));

    m_pHeader->ElementDecl[i].Type = Decl[i].Type;
    m_pHeader->ElementDecl[i].Length = Decl[i].Length;

    ElementSize += Decl[i].GetSize();
  }

  m_pHeader->ElementSize = ElementSize;
  
}
//---------------------------------------------------------------------------------------------------
BOOL Grid::IsElementTypeCompatible(Array<ElementDecl> Decl)
{
  for (SIZE_T i=0; i<Decl.Length(); i++)
  {
    if (m_pHeader->ElementDecl[i].Type != Decl[i].Type ||
        m_pHeader->ElementDecl[i].Length != Decl[i].Length)
      return FALSE;
  }
  return TRUE;
}
//---------------------------------------------------------------------------------------------------
Array<ElementDecl> Grid::GetElementType()
{
  Array<ElementDecl> Decl;
  for (SIZE_T i=0; i<m_pHeader->ElementCount; i++)
    Decl.Add(ElementDecl(String(m_pHeader->ElementDecl[i].Name, 32), m_pHeader->ElementDecl[i].Type, m_pHeader->ElementDecl[i].Length));

  return Decl;
}
//---------------------------------------------------------------------------------------------------

/*
template<class T>
template<class TRANSFORM>
VOID Grid<T>::Transform(TRANSFORM& XF)
{
  for (UINT i=0; i<BlockHeight(); i++)
  {
    for (UINT j=0; j<BlockWidth(); j++)
    {
      LockBlock(i,j);
      FLOAT* B = Block(i,j);
      for (UINT y=0; y<BLOCKDIMENSION; y++)
        for (UINT x=0; x<BLOCKDIMENSION; x++)
          B[y * BlockSize() + x] = XF(B[y * BlockSize() + x]);
      UnlockBlock(i,j);
    }
  }
}
//---------------------------------------------------------------------------------------------------
template<class T>
template<class TRANSFORM, class U>
VOID Grid<T>::Transform(TRANSFORM& XF, Grid<U>& OutputGrid) CONST
{
  Assert(OutputGrid.Width() == Width());
  Assert(OutputGrid.Height() == Height());

  for (UINT i=0; i<BlockHeight(); i++)
  {
    for (UINT j=0; j<BlockWidth(); j++)
    {
      LockBlock(i,j);
      O.LockBlock(i,j);

      CONST T* B = Block(i,j);
      U* O = OutputGrid.Block(i,j);

      for (UINT y=0; y<BLOCKDIMENSION; y++)
        for (UINT x=0; x<BLOCKDIMENSION; x++)
          O[y * BlockSize() + x] = XF(B[y * BlockSize() + x]);
      
      UnlockBlock(i,j);
      O.UnlockBlock(i,j);
    }
  }
}
//---------------------------------------------------------------------------------------------------
template<class T>
template<class OPERATOR>
VOID Grid<T>::Combine(OPERATOR& O, CONST Grid<T>& Other)
{
  Assert(Other.Width() == Width());
  Assert(Other.Height() == Height());

  for (UINT i=0; i<BlockHeight(); i++)
  {
    for (UINT j=0; j<BlockWidth(); j++)
    {
      LockBlock(i,j);
      Other.LockBlock(i,j);

      FLOAT* A = Block(i,j);
      FLOAT* B = Other.Block(i,j);

      for (UINT y=0; y<BLOCKDIMENSION; y++)
        for (UINT x=0; x<BLOCKDIMENSION; x++)
          A[y * BlockSize() + x] = O(A[y * BlockSize() + x], B[y * BlockSize() + x]);
      
      UnlockBlock(i,j);
      Other.UnlockBlock(i,j);
    }
  }
}
//---------------------------------------------------------------------------------------------------
template<class T>
template<class OPERATOR, class U>
VOID Grid<T>::Combine(OPERATOR& O, CONST Grid<T>& Other, Grid<U>& Output) CONST
{
  Assert(Other.Width() == Width());
  Assert(Other.Height() == Height());

  Assert(OutputGrid.Width() == Width());
  Assert(OutputGrid.Height() == Height());

  for (UINT i=0; i<BlockHeight(); i++)
  {
    for (UINT j=0; j<BlockWidth(); j++)
    {
      LockBlock(i,j);
      Other.LockBlock(i,j);
      Output.LockBlock(i,j);

      CONST FLOAT* A = Block(i,j);
      CONST FLOAT* B = Other.Block(i,j);
      FLOAT* C = Output.Block(i,j); 

      for (UINT y=0; y<BLOCKDIMENSION; y++)
        for (UINT x=0; x<BLOCKDIMENSION; x++)
          C[y * BlockSize() + x] = O(A[y * BlockSize() + x], B[y * BlockSize() + x]);
      
      UnlockBlock(i,j);
      Other.UnlockBlock(i,j);
      Output.UnlockBlock(i,j);
    }
  }
}
//---------------------------------------------------------------------------------------------------
#if 0
template<class T>
template<class FILTER>
VOID Grid<T>::Filter(FILTER& F)
{
  for (UINT i=0; i<BlockHeight(); i++)
  {
    for (UINT j=0; j<BlockWidth(); j++)
    {
      T* B0 = Block(i-1, j-1);
      T* B1 = Block(i-1, j );
      T* B2 = Block(i-1, j+1);
      T* B3 = Block(i  , j-1);
      T* B4 = Block(i  , j );
      T* B5 = Block(i  , j+1);
      T* B6 = Block(i+1, j-1);
      T* B7 = Block(i+1, j );
      T* B8 = Block(i+1, j+1);

      for (UINT y=1; y<BLOCKDIMENSION-1; y++)
        for (UINT x=1; x<BLOCKDIMENSION-1; x++)
          B4[y * BlockSize() + x] = pFilter->Accumulate(9, &B4[(y-1) * BLOCKDIMENSION + x-1], &B4[(y-1) * BLOCKDIMENSION + x], &B4[(y-1) * BLOCKDIMENSION + x+1],
                                      &B4[ y    * BLOCKDIMENSION + x-1], &B4[ y    * BLOCKDIMENSION + x], &B4[ y    * BLOCKDIMENSION + x+1],
                                      &B4[(y+1) * BLOCKDIMENSION + x-1], &B4[(y+1) * BLOCKDIMENSION + x], &B4[(y+1) * BLOCKDIMENSION + x+1]);

      continue;

      if (B3 != 0)
      {
        UINT x = 0;
        for (UINT y=1; y<BLOCKDIMENSION-1; y++)
        {
          B4[y * BlockSize() + x] = pFilter->Accumulate(9, &B3[(y-1) * BLOCKDIMENSION + BLOCKDIMENSION-1], &B4[(y-1) * BLOCKDIMENSION + x], &B4[(y-1) * BLOCKDIMENSION + x+1],
            &B3[ y    * BLOCKDIMENSION + BLOCKDIMENSION-1], &B4[ y    * BLOCKDIMENSION + x], &B4[ y    * BLOCKDIMENSION + x+1],
            &B3[(y+1) * BLOCKDIMENSION + BLOCKDIMENSION-1], &B4[(y+1) * BLOCKDIMENSION + x], &B4[(y+1) * BLOCKDIMENSION + x+1]);
        }
      }
      else
      {
        UINT x = 0;
        for (UINT y=1; y<BLOCKDIMENSION-1; y++)
        {
          B4[y * BlockSize() + x] = pFilter->Accumulate(9, 0, &B4[(y-1) * BLOCKDIMENSION + x], &B4[(y-1) * BLOCKDIMENSION + x+1],
            0, &B4[ y    * BLOCKDIMENSION + x], &B4[ y    * BLOCKDIMENSION + x+1],
            0, &B4[(y+1) * BLOCKDIMENSION + x], &B4[(y+1) * BLOCKDIMENSION + x+1]);
        }
      }

      if (B5 != 0)
      {
        UINT x = BLOCKDIMENSION - 1;
        for (UINT y=1; y<BLOCKDIMENSION-1; y++)
        {
          B4[y * BlockSize() + x] = pFilter->Accumulate(9, &B4[(y-1) * BLOCKDIMENSION + x-1], &B4[(y-1) * BLOCKDIMENSION + x], &B5[(y-1) * BLOCKDIMENSION + 0],
            &B4[ y    * BLOCKDIMENSION + x-1], &B4[ y    * BLOCKDIMENSION + x], &B5[ y    * BLOCKDIMENSION + 0],
            &B4[(y+1) * BLOCKDIMENSION + x-1], &B4[(y+1) * BLOCKDIMENSION + x], &B5[(y+1) * BLOCKDIMENSION + 0]);
        }
      }
      else
      {
        UINT x = BLOCKDIMENSION - 1;
        for (UINT y=1; y<BLOCKDIMENSION-1; y++)
        {
          B4[y * BlockSize() + x] = pFilter->Accumulate(9, &B4[(y-1) * BLOCKDIMENSION + x-1], &B4[(y-1) * BLOCKDIMENSION + x], 0,
            &B4[ y    * BLOCKDIMENSION + x-1], &B4[ y    * BLOCKDIMENSION + x], 0,
            &B4[(y+1) * BLOCKDIMENSION + x-1], &B4[(y+1) * BLOCKDIMENSION + x], 0);
        }
      }

      if (B1 != 0)
      {
        UINT y = 0;
        for (UINT x=1; x<BLOCKDIMENSION-1; x++)
        {
          B4[y * BlockSize() + x] = pFilter->Accumulate(9, &B1[(BLOCKDIMENSION-1) * BLOCKDIMENSION + x-1], &B1[(BLOCKDIMENSION-1) * BLOCKDIMENSION + x], &B1[(BLOCKDIMENSION-1) * BLOCKDIMENSION + x+1],
            &B4[ y    * BLOCKDIMENSION + x-1], &B4[ y    * BLOCKDIMENSION + x], &B4[ y    * BLOCKDIMENSION + x+1],
            &B4[(y+1) * BLOCKDIMENSION + x-1], &B4[(y+1) * BLOCKDIMENSION + x], &B4[(y+1) * BLOCKDIMENSION + x+1]);
        }
      }
      else
      {
        UINT y = 0;
        for (UINT x=1; x<BLOCKDIMENSION-1; x++)
        {
          B4[y * BlockSize() + x] = pFilter->Accumulate(9, 0                             , 0                           , 0                             ,
            &B4[ y    * BLOCKDIMENSION + x-1], &B4[ y    * BLOCKDIMENSION + x], &B4[ y    * BLOCKDIMENSION + x+1],
            &B4[(y+1) * BLOCKDIMENSION + x-1], &B4[(y+1) * BLOCKDIMENSION + x], &B4[(y+1) * BLOCKDIMENSION + x+1]);
        }
      }

      if (B7 != 0)
      {
        UINT y = BLOCKDIMENSION - 1;
        for (UINT x=1; x<BLOCKDIMENSION-1; x++)
        {
          B4[y * BlockSize() + x] = pFilter->Accumulate(9, &B4[(y-1) * BLOCKDIMENSION + x-1], &B4[(y-1) * BLOCKDIMENSION + x], &B4[(y-1) * BLOCKDIMENSION + x+1],
            &B4[ y    * BLOCKDIMENSION + x-1], &B4[ y    * BLOCKDIMENSION + x], &B4[ y    * BLOCKDIMENSION + x+1],
            &B7[                      x-1], &B7[                      x], &B7[                      x+1]);
        }
      }
      else
      {
        UINT y = BLOCKDIMENSION - 1;
        for (UINT x=1; x<BLOCKDIMENSION-1; x++)
        {
          B4[y * BlockSize() + x] = pFilter->Accumulate(9, &B4[(y-1) * BLOCKDIMENSION + x-1], &B4[(y-1) * BLOCKDIMENSION + x], &B4[(y-1) * BLOCKDIMENSION + x+1],
            &B4[ y    * BLOCKDIMENSION + x-1], &B4[ y    * BLOCKDIMENSION + x], &B4[ y    * BLOCKDIMENSION + x+1],
            0                             , 0                           , 0                            );
        }
      }

    }
  }

}
#endif
//---------------------------------------------------------------------------------------------------
template<class T>
VOID Grid<T>::FilterAccumulateKn(IAccumulationFilter<T,T>* pFilter)
{
  for (UINT i=0; i<BlockHeight(); i++)
    for (UINT j=0; j<BlockWidth(); j++)
    {
      FLOAT* B = Block(i,j);
      for (UINT x=0; x<BLOCKDIMENSION; x++)
        for (UINT y=0; y<BLOCKDIMENSION; y++)
          pFilter->Accumulate(1, &B[x + BLOCKDIMENSION + y]);
    }

}
*/
//---------------------------------------------------------------------------------------------------


