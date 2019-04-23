/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/GridImpl.hpp $
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


//---------------------------------------------------------------------------------------------------
template<class T>
CONST T& Grid::Element(PTRDIFF_T i, PTRDIFF_T j, UINT Level) CONST
{
  PTRDIFF_T Height = GetHeight(Level);
  PTRDIFF_T Width = GetWidth(Level);

  Assert(Level < m_pHeader->LevelCount);

  PTRDIFF_T Xi = i;
  PTRDIFF_T Xj = j;

  if (m_BoundaryConditions == GridBoundary::CLAMP)
  {
    if (i < 0)
      i = 0;
    if (j < 0)
      j = 0;
    if (i >= Height)
      i = Height-1;
    if (j >= Width)
      j = Width-1;
  }
  else if (m_BoundaryConditions == GridBoundary::REFLECTIVE)
  {
    Assert(i < 2*Height-1);
    Assert(j < 2*Width-1);
    Assert(i >= -Height);
    Assert(j >= -Width);

    if (i >= Height)
      i = Height - (i % Height) - 2;
    else if (i < 0)
      i = Math::Abs(i % Height);
    else
      i = i % Height;
  
    if (j >= Width)
      j = Width - (j % Width) - 2;
    else if (j < 0)
      j = Math::Abs(j % Width);
    else
      j = j % Width;

  }
  else if (m_BoundaryConditions == GridBoundary::PERIODIC)
  {
    i = i % GetHeight(Level);
    j = j % GetWidth(Level);
  }

  Assert(i < Height);
  Assert(j < Width);

  Assert(i >= 0);
  Assert(j >= 0);

  //Trace("(%d,%d) -> (%d,%d)", Xi, Xj, i, j);

  CONST BYTE* pGrid = (CONST BYTE*)m_pGridData[Level];

  if (m_pHeader->DataLayout == GridLayout::ROWMAJOR)
    return *(CONST T*)(pGrid + i * m_GridRowStride[Level] + j * m_pHeader->ElementSize);
  else if (m_pHeader->DataLayout == GridLayout::COLUMNMAJOR)
    return *(CONST T*)(pGrid + j * m_GridRowStride[Level] + i * m_pHeader->ElementSize);

  else if (FALSE)
  {
    /* Find Block */
    pGrid += (i >> m_BlockAddressShift[Level]) * m_GridBlockStride[Level] + (j >> m_BlockAddressShift[Level]) * m_BlockSizeBytes[Level];
    /* Find Element */
    pGrid += (i & m_BlockOffsetMask[Level]) * m_GridBlockRowStride[Level] + (j & m_BlockOffsetMask[Level]) * m_pHeader->ElementSize;
    
    return *(CONST T*)pGrid;
  }
}
//---------------------------------------------------------------------------------------------------
template<class T>
T& Grid::Element(PTRDIFF_T i, PTRDIFF_T j, UINT Level)
{
  PTRDIFF_T Height = GetHeight(Level);
  PTRDIFF_T Width = GetWidth(Level);

  Assert(Level < m_pHeader->LevelCount);

  if (m_BoundaryConditions == GridBoundary::CLAMP)
  {
    if (i < 0)
      i = 0;
    if (j < 0)
      j = 0;
    if (i >= Height)
      i = Height-1;
    if (j >= Width)
      j = Width-1;
  }
  else if (m_BoundaryConditions == GridBoundary::REFLECTIVE)
  {
    Assert(i <= 2*Height-1);
    Assert(j <= 2*Width-1);
    Assert(i >= -Height);
    Assert(j >= -Width);

    if (i >= Height)
      i = Height - (i % Height) - 2;
    else if (i < 0)
      i = Math::Abs(i % Height);
    else
      i = i % Height;
  
    if (j >= Width)
      j = Width - (j % Width) - 2;
    else if (j < 0)
      j = Math::Abs(j % Width);
    else
      j = j % Width;

  }
  else if (m_BoundaryConditions == GridBoundary::PERIODIC)
  {
    i = i % GetHeight(Level);
    j = j % GetWidth(Level);
  }

  Assert(i < Height);
  Assert(j < Width);

  Assert(i >= 0);
  Assert(j >= 0);

  BYTE* pGrid = (BYTE*)m_pGridData[Level];
  if (m_pHeader->DataLayout == GridLayout::COLUMNMAJOR)
    return *(T*)(pGrid + j * m_GridRowStride[Level] + i * m_pHeader->ElementSize);
  else
    return *(T*)(pGrid + i * m_GridRowStride[Level] + j * m_pHeader->ElementSize);

  if (FALSE)
  {
    /* Find Block */
    pGrid += (i >> m_BlockAddressShift[Level]) * m_GridBlockStride[Level] + (j >> m_BlockAddressShift[Level]) * m_BlockSizeBytes[Level];
    /* Find Element */
    pGrid += (i & m_BlockOffsetMask[Level]) * m_GridBlockRowStride[Level] + (j & m_BlockOffsetMask[Level]) * m_pHeader->ElementSize;
    
    return *(T*)pGrid;
  }
}
//---------------------------------------------------------------------------------------------------
template<class T>
CONST T* Grid::Block(UINT i, UINT j) CONST
{
  /*
  if (j >= m_pHeader->BlockWidth || i > m_pHeader->BlockHeight)
    return 0;
  
  CONST BYTE* pGrid = (CONST BYTE*)m_pGridData;
  if (m_pHeader->DataLayout == GridLayout::ROWMAJOR)
    return pGrid;
  else
    return (CONST T*)(pGrid + i * m_GridBlockStride + j * m_BlockSizeBytes);
  */
}
//---------------------------------------------------------------------------------------------------
template<class T>
T* Grid::Block(UINT i, UINT j)
{
  /*
  if (j >= m_pHeader->BlockWidth || i > m_pHeader->BlockHeight)
    return 0;
  
  BYTE* pGrid = (BYTE*)m_pGridData;
  if (m_pHeader->DataLayout == GridLayout::ROWMAJOR)
    return pGrid;
  else
    return (T*)(pGrid + i * m_GridBlockStride + j * m_BlockSizeBytes);
  */
}
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
template<class T>
TGrid<T>::TGrid(LPCTSTR Filename, DWORD FileMode, DWORD PageAccess)
{
  /*
  InitializeOpen(Filename, Flags);
  if (!pGrid->IsElementTypeCompatible(GetTemplateElementType()))
    throw GridException(this, String::Format("The element type in \"%s\" is not compatible with the template type", Filename));

  return pGrid;
  */
}
//---------------------------------------------------------------------------------------------------
template<class T>
T TGrid<T>::Norm(UINT N)
{
  T V = T();
  for (UINT i=0; i<GetHeight(); i++)
    for (UINT j=0; j<GetWidth(); j++)
    {
      T E = Element(i,j);
      for (UINT k=0; k<N-1; k++)
        E *= E;
      V += E;
    }

  return Math::Pow<T>(V, (T)1.0f / (T)N);
}
//---------------------------------------------------------------------------------------------------
template<class T>
TGrid<T>::TGrid()
{
  InitializeInMemory();
  SetElementType(GetTemplateElementType());
}
//---------------------------------------------------------------------------------------------------
template<class T>
VOID TGrid<T>::Import(LPCTSTR Filename, DWORD Flags)
{
  Grid::Import(Filename, Flags | GridFlags::FORCEELEMENTTYPE);
}
//---------------------------------------------------------------------------------------------------
template<>
INLINE Array<ElementDecl> TGrid<FLOAT>::GetTemplateElementType()
{
  Array<ElementDecl> Decl;
  Decl.Add(ElementDecl("Value", ElementDeclType::FLOAT, 1));
  return Decl;
}
//---------------------------------------------------------------------------------------------------
template<>
INLINE Array<ElementDecl> TGrid< Math::Vector3<FLOAT> >::GetTemplateElementType()
{
  Array<ElementDecl> Decl;
  Decl.Add(ElementDecl("Vector", ElementDeclType::FLOAT, 3));
  return Decl;
}
//---------------------------------------------------------------------------------------------------
template<>
INLINE Array<ElementDecl> TGrid<DOUBLE>::GetTemplateElementType()
{
  Array<ElementDecl> Decl;
  Decl.Add(ElementDecl("Value", ElementDeclType::DOUBLE, 1));
  return Decl;
}
//---------------------------------------------------------------------------------------------------
template<>
INLINE Array<ElementDecl> TGrid<UINT>::GetTemplateElementType()
{
  Array<ElementDecl> Decl;
  Decl.Add(ElementDecl("Value", ElementDeclType::INT, 1));
  return Decl;
}
//---------------------------------------------------------------------------------------------------
template<class T>
template<class U>
VOID TGrid<T>::Convert(CONST TGrid<U>& G)
{
  Resize(G.GetWidth(), G.GetHeight());
  
  for (UINT i=0; i<GetHeight(); i++)
    for (UINT j=0; j<GetWidth(); j++)
      Element(i,j) = (T)G.Element(i,j);
}
//---------------------------------------------------------------------------------------------------
template<class T>
TGrid<T>& TGrid<T>::operator -= (CONST TGrid<T>& G)
{
  for (UINT i=0; i<GetHeight(); i++)
    for (UINT j=0; j<GetWidth(); j++)
      Element(i,j) -= G.Element(i,j);

  return *this;
}
//---------------------------------------------------------------------------------------------------
template<class T>
TGrid<T>& TGrid<T>::operator -= (T x)
{
  for (UINT i=0; i<GetHeight(); i++)
    for (UINT j=0; j<GetWidth(); j++)
      Element(i,j) -= x;

  return *this;
}
//---------------------------------------------------------------------------------------------------
template<class T>
TGrid<T>& TGrid<T>::operator += (CONST TGrid<T>& G)
{
  for (UINT i=0; i<GetHeight(); i++)
    for (UINT j=0; j<GetWidth(); j++)
      Element(i,j) += G.Element(i,j);

  return *this;
}
//---------------------------------------------------------------------------------------------------
template<class T>
TGrid<T>& TGrid<T>::operator += (T x)
{
  for (UINT i=0; i<GetHeight(); i++)
    for (UINT j=0; j<GetWidth(); j++)
      Element(i,j) += x;

  return *this;
}
//---------------------------------------------------------------------------------------------------
template<class T>
TGrid<T> TGrid<T>::operator - (CONST TGrid<T>& G)
{
  TGrid R(G);
  for (UINT i=0; i<GetHeight(); i++)
    for (UINT j=0; j<GetWidth(); j++)
      R(i,j) = Element(i,j) - G(i,j);

  return R;
}
//---------------------------------------------------------------------------------------------------
template<class T>
TGrid<T> TGrid<T>::operator - (T x)
{
  TGrid R(*this);
  for (UINT i=0; i<GetHeight(); i++)
    for (UINT j=0; j<GetWidth(); j++)
      R(i,j) = Element(i,j) - x;

  return R;
}
//---------------------------------------------------------------------------------------------------
template<class T>
TGrid<T> TGrid<T>::operator- ()
{
  TGrid R(*this);
  for (UINT i=0; i<GetHeight(); i++)
    for (UINT j=0; j<GetWidth(); j++)
      R(i,j) = -Element(i,j);

  return R;
}
//---------------------------------------------------------------------------------------------------
template<class T>
TGrid<T> TGrid<T>::operator + (CONST TGrid<T>& G)
{
  TGrid R(G);
  for (UINT i=0; i<GetHeight(); i++)
    for (UINT j=0; j<GetWidth(); j++)
      R(i,j) = Element(i,j) + G(i,j);

  return R;
}
//---------------------------------------------------------------------------------------------------
template<class T>
TGrid<T> TGrid<T>::operator + (T x)
{
  TGrid R(*this);
  for (UINT i=0; i<GetHeight(); i++)
    for (UINT j=0; j<GetWidth(); j++)
      R(i,j) = Element(i,j) + x;

  return R;
}
//---------------------------------------------------------------------------------------------------
template<class T>
TGrid<T> TGrid<T>::operator * (CONST TGrid<T>& G)
{
  TGrid R(G);
  for (UINT i=0; i<GetHeight(); i++)
    for (UINT j=0; j<GetWidth(); j++)
      R(i,j) = Element(i,j) * G(i,j);

  return R;
}
//---------------------------------------------------------------------------------------------------
template<class T>
TGrid<T> TGrid<T>::operator * (T x)
{
  TGrid R(*this);
  for (UINT i=0; i<GetHeight(); i++)
    for (UINT j=0; j<GetWidth(); j++)
      R(i,j) = Element(i,j) * x;

  return R;
}
//---------------------------------------------------------------------------------------------------
template<class T>
TGrid<T>& TGrid<T>::operator *= (CONST TGrid<T>& G)
{
  for (UINT i=0; i<GetHeight(); i++)
    for (UINT j=0; j<GetWidth(); j++)
      Element(i,j) *= G(i,j);

  return *this;
}
//---------------------------------------------------------------------------------------------------
template<class T>
TGrid<T>& TGrid<T>::operator *= (T x)
{
  for (UINT i=0; i<GetHeight(); i++)
    for (UINT j=0; j<GetWidth(); j++)
      Element(i,j) *= x;

  return *this;
}
//---------------------------------------------------------------------------------------------------
template<class T>
TGrid<T> TGrid<T>::operator / (CONST TGrid<T>& G)
{
  TGrid R(G);
  for (UINT i=0; i<GetHeight(); i++)
    for (UINT j=0; j<GetWidth(); j++)
      R(i,j) = Element(i,j) / G(i,j);

  return R;
}
//---------------------------------------------------------------------------------------------------
template<class T>
TGrid<T> TGrid<T>::operator / (T x)
{
  TGrid R(*this);
  for (UINT i=0; i<GetHeight(); i++)
    for (UINT j=0; j<GetWidth(); j++)
      R(i,j) = Element(i,j) / x;

  return R;
}
//---------------------------------------------------------------------------------------------------
template<class T>
TGrid<T>& TGrid<T>::operator /= (CONST TGrid<T>& G)
{
  for (UINT i=0; i<GetHeight(); i++)
    for (UINT j=0; j<GetWidth(); j++)
      Element(i,j) /= G(i,j);

  return *this;
}
//---------------------------------------------------------------------------------------------------
template<class T>
TGrid<T>& TGrid<T>::operator /= (T x)
{
  for (UINT i=0; i<GetHeight(); i++)
    for (UINT j=0; j<GetWidth(); j++)
      Element(i,j) /= x;

  return *this;
}
//---------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------
template<class T>
VOID Grid::RemapMinMax(FLOAT Power)
{
  T MinValue = Element<T>(0,0);
  T MaxValue = MinValue;

  for (UINT i=0; i<GetHeight(); i++)
  {
    for (UINT j=0; j<GetWidth(); j++)
    {
      T Value = Element<T>(i,j);
      if (Value > MaxValue)
        MaxValue = Value;
      if (Value < MinValue)
        MinValue = Value;
    }
  }

  for (UINT i=0; i<GetHeight(); i++)
  {
    for (UINT j=0; j<GetWidth(); j++)
    {
      T Value = Element<T>(i,j);
      Element<T>(i,j) = (Value - MinValue) / (MaxValue - MinValue);
    }
  }

}
//---------------------------------------------------------------------------------------------------
template<class T>
Pair<T,T> Grid::GetMinMax()
{
  T Min = Element<T>(0,0);
  T Max = Element<T>(0,0);

  for (UINT i=0; i<GetHeight(); i++)
  {
    for (UINT j=0; j<GetWidth(); j++)
    {
      T Value = Element<T>(i,j);
      if (Value < Min)
        Min = Value;
      if (Value > Max)
        Max = Value;
    }
  }

  return Pair<T,T>(Min,Max);
}
//---------------------------------------------------------------------------------------------------
template<class T>
Pair<T,T> Grid::GetMeanStdDev()
{
  T Sum;
  T Sum2;

  for (UINT i=0; i<GetHeight(); i++)
  {
    for (UINT j=0; j<GetWidth(); j++)
    {
      Sum += Element<T>(i,j);
      Sum2 += Element<T>(i,j) * Element<T>(i,j);
    }
  }
  
  UINT SampleCount = GetWidth() * GetHeight();
  T Mean = Sum / SampleCount;
  T StdDev = Sqrt(((T)SampleCount * Sum2 - Sum * Sum) / ((T)SampleCount * (T)(SampleCount - 1)));
  
  return Pair<T,T>(Mean, StdDev);
}
//---------------------------------------------------------------------------------------------------
template<class T>
T Grid::Sample(FLOAT Fi, FLOAT Fj, UINT Level) CONST
{
  UINT i = (UINT)Fi;
  UINT j = (UINT)Fj;
  
  FLOAT Di = Fi - i;
  FLOAT Dj = Fj - j;

  T Sample0 = (1-Di) * Element<T>(i, j, Level) + Di * Element<T>(i+1, j, Level);
  T Sample1 = (1-Di) * Element<T>(i, j+1, Level) + Di * Element<T>(i+1, j+1, Level);

  return (1-Dj) * Sample0 + Dj * Sample1;
}
//---------------------------------------------------------------------------------------------------
template<class T>
VOID Grid::GenerateMipMaps()
{
  UINT Width = m_pHeader->Width[0];
  UINT Height = m_pHeader->Height[0];

  Width >>= 1;
  Height >>= 1;
  
  ULONGLONG GridDataLength = 0;
  ULONGLONG GridDataOffset = m_pHeader->GridDataOffset[0] + m_pHeader->GridDataLength[0];
  UINT Level = 1;
  while (Width > 0 && Height > 0)
  {
    GridDataLength = (ULONGLONG)Width * Height * m_pHeader->ElementSize;
    
    m_pHeader->GridDataLength[Level] = GridDataLength;
    m_pHeader->GridDataOffset[Level] = GridDataOffset;
    GridDataOffset += GridDataLength;
    
    m_pHeader->Width[Level] = Width;
    m_pHeader->Height[Level] = Height;
    
    Width >>= 1;
    Height >>= 1;
    Level++;
  }
  m_pHeader->LevelCount = Level;

  if (m_pMappedView)
  {
    m_pMappedView = 0;
    m_pFile->Resize(GridDataOffset);
    m_pMappedView = m_pFile->MapView(PageAccess::READWRITE);

    InitializeMapping();
  }
  else
  {
    for (UINT i=1; i<Level; i++)
    {
      m_pGridData[i] = AllocateMemory((SIZE_T)m_pHeader->GridDataLength[i]);
      m_pHeader->GridDataOffset[i] = 0;
      m_GridRowStride[i] = m_pHeader->Width[i] * m_pHeader->ElementSize;
    }
  }
  

  for (UINT k=1; k<m_pHeader->LevelCount; k++)
    for (UINT i=0; i<m_pHeader->Height[k]; i++)
      for (UINT j=0; j<m_pHeader->Width[k]; j++)
        Element<T>(i,j,k) = (Element<T>(2*i, 2*j, k-1) + Element<T>(2*i+1, 2*j, k-1) + Element<T>(2*i, 2*j+1, k-1) + Element<T>(2*i+1, 2*j+1, k-1)) * 0.25f;

}
//---------------------------------------------------------------------------------------------------
