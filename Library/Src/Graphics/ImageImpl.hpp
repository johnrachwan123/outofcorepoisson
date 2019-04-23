/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/ImageImpl.hpp $
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


//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_LUA)
template<class PixelType>
Lua::Bind::Scope TImage<PixelType>::GenerateBindings(LPCTSTR Name)
{
  using namespace Lua::Bind;

  return 
    Class<TImage<PixelType>>(Name)
      .Def(Constructor<>())
      .Def(Constructor<UINT,UINT>())
      .Def(Constructor<LPCTSTR>())
      .Def(Constructor<Image&>())

      .Property("Width", &TImage<PixelType>::Width)
      .Property("Height", &TImage<PixelType>::Height)
      .Property("PixelFormat", &TImage<PixelType>::GetPixelFormat)
      .Property("PixelSize", (UINT(TImage<PixelType>::*)() CONST)&TImage<PixelType>::GetPixelSize) 
      .Property("PixelChannelCount", (UINT(TImage<PixelType>::*)() CONST)&TImage<PixelType>::GetPixelChannels)

      .Def("SetPixelFormat", &TImage<PixelType>::SetPixelFormat)
      .Def("SetBoundary", &TImage<PixelType>::SetBoundary)

      .Def("Resize", (VOID(TImage<PixelType>::*)(UINT, UINT))&TImage<PixelType>::Resize)
      .Def("Resize", (VOID(TImage<PixelType>::*)(UINT, UINT, DWORD))&TImage<PixelType>::Resize)

      .Def("Load", (VOID(TImage<PixelType>::*)(LPCTSTR))&TImage<PixelType>::Load)
      
      .Def("Save", (VOID(TImage<PixelType>::*)(LPCTSTR) CONST)&TImage<PixelType>::Save)
      .Def("Save", (VOID(TImage<PixelType>::*)(LPCTSTR, DWORD) CONST)&TImage<PixelType>::Save)

      .Def("GetPixel", &TImage<PixelType>::GetPixel)
      .Def("GetPixel", &TImage<PixelType>::SetPixel)

      .Def("SampleNearest", &TImage<PixelType>::SampleNearest)
      .Def("SampleBilinear", &TImage<PixelType>::SampleBilinear)
      .Def("SampleBicubic", &TImage<PixelType>::SampleBicubic)
      .Def("SampleGaussian", &TImage<PixelType>::SampleGaussian)
     
      .Def("Crop", &TImage<PixelType>::Crop)
      .Def("GetSubImage", &TImage<PixelType>::GetSubImage)

      .Def("Clamp", (VOID(TImage<PixelType>::*)())&TImage<PixelType>::Clamp)
      .Def("Clamp", (VOID(TImage<PixelType>::*)(typename PixelType::ColorChannelType, typename PixelType::ColorChannelType))&TImage<PixelType>::Clamp)

      .Def("Normalize", &TImage<PixelType>::Normalize)

      .Def("ComputeGradientX", &TImage<PixelType>::ComputeGradientX)
      .Def("ComputeGradientY", &TImage<PixelType>::ComputeGradientY)
      .Def("ComputeLaplacian", &TImage<PixelType>::ComputeLaplacian)

      ;
  
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
Image Image::ComputeGradientX() CONST
{
  Image Dx(m_Width, m_Height, Image::GetNearestFloatingPointPixelFormat(m_PixelFormat));

  for (UINT i=0; i<m_Height; i++)
  {
    for (UINT j=0; j<m_Width; j++)
    {
      PixelType P0 = Pixel<PixelType>(i,j);
      PixelType P1 = Pixel<PixelType>(i,j+1);

      FLOAT* pDx = &Dx.Pixel<FLOAT>(i,j);
      for (UINT k=0; k<PixelType::Channels; k++)
        pDx[k] = P0[k] - P1[k];
    }
  }

  return Dx;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
Image Image::ComputeGradientY() CONST
{
  Image Dx(m_Width, m_Height, Image::GetNearestFloatingPointPixelFormat(m_PixelFormat));

  for (UINT i=0; i<m_Height; i++)
  {
    for (UINT j=0; j<m_Width; j++)
    {
      PixelType P0 = Pixel<PixelType>(i,j);
      PixelType P1 = Pixel<PixelType>(i+1,j);

      FLOAT* pDx = &Dx.Pixel<FLOAT>(i,j);
      for (UINT k=0; k<PixelType::Channels; k++)
        pDx[k] = P0[k] - P1[k];
    }
  }

  return Dx;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
Image Image::ComputeLaplacian() CONST
{
  Image Dx(m_Width, m_Height, Image::GetNearestFloatingPointPixelFormat(m_PixelFormat));

  for (UINT i=0; i<m_Height; i++)
  {
    for (UINT j=0; j<m_Width; j++)
    {
      PixelType P0 = Pixel<PixelType>(i,j);

      PixelType P1 = Pixel<PixelType>(i-1,j);
      PixelType P2 = Pixel<PixelType>(i,j+1);
      PixelType P3 = Pixel<PixelType>(i+1,j);
      PixelType P4 = Pixel<PixelType>(i,j-1);
      
      FLOAT* pDx = &Dx.Pixel<FLOAT>(i,j);
      for (UINT k=0; k<PixelType::Channels; k++)
        pDx[k] = 4.0f * P0[k] - P1[k] - P2[k] - P3[k] - P4[k];
    }
  }

  return Dx;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
Image Image::ComputeWaveletTransform() CONST
{
  Image DWT(*this);
  DWT.SetPixelFormat(Image::GetNearestFloatingPointPixelFormat(m_PixelFormat));

  UINT Channels = PixelType::Channels;
  

  
  return DWT;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
CONST PixelType& Image::Pixel(INT i, INT j) CONST
{
  static PixelType Zero = PixelType::Black;
  LPCBYTE p = 0;

  switch (m_Boundary)
  {
  case ImageBoundary::NONE:
    if (i < 0 || (UINT)i >= m_Height)
      throw InvalidIndexException(this, "The i coordinate is outside the Image domain");
    if (j < 0 || (UINT)j >= m_Width)
      throw InvalidIndexException(this, "The j coordinate is outside the Image domain");

    p = (LPCBYTE)m_pPixelData + i*m_Width*GetPixelSize() + j*GetPixelSize();
    return *(CONST PixelType*)p;

  case ImageBoundary::ZERO:
    if (i < 0 || (UINT)i >= m_Height)
      return Zero;
    if (j < 0 || (UINT)j >= m_Width)
      return Zero;

    p = (LPCBYTE)m_pPixelData + i*m_Width*GetPixelSize() + j*GetPixelSize();
    return *(CONST PixelType*)p;
    
  case ImageBoundary::CLAMP:
    if (i < 0) i = 0;
    if ((UINT)i >= m_Height) i = m_Height-1;
    if (j < 0) j = 0;
    if ((UINT)j >= m_Width) j = m_Width-1;

    p = (LPCBYTE)m_pPixelData + i*m_Width*GetPixelSize() + j*GetPixelSize();
    return *(CONST PixelType*)p;

  case ImageBoundary::REFLECTIVE:
    i = (UINT)i % m_Width;
    j = (UINT)j % m_Height;

    p = (LPCBYTE)m_pPixelData + i*m_Width*GetPixelSize() + j*GetPixelSize();
    return *(CONST PixelType*)p;

  case ImageBoundary::PERIODIC:
    if (i < 0) i = -i;
    if ((UINT)i >= m_Height) i = 2*m_Height - 2 - i;
    if (j < 0) j = -j;
    if ((UINT)j >= m_Width) j = 2*m_Width - 2 - j;
    
    if (i < 0 || (UINT)i >= m_Height)
      throw InvalidIndexException(this, "The i coordinate is outside the Image domain");
    if (j < 0 || (UINT)j >= m_Width)
      throw InvalidIndexException(this, "The j coordinate is outside the Image domain");

    p = (LPCBYTE)m_pPixelData + i*m_Width*GetPixelSize() + j*GetPixelSize();
    return *(CONST PixelType*)p;

  }
  
  Assert(FALSE);
  return *(CONST PixelType*)p;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
PixelType& Image::Pixel(INT i, INT j)
{
  LPBYTE p = (LPBYTE)m_pPixelData + i*m_Width*GetPixelSize() + j*GetPixelSize();
  return *(PixelType*)p;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
VOID Image::Clamp()
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j).Clamp();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
VOID Image::Clamp(typename PixelType::ColorChannelType Low, typename PixelType::ColorChannelType High)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j).Clamp(Low, High);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
VOID Image::Normalize()
{
  if (Width() == 0 || Height() == 0)
    return;

  typename PixelType::ColorChannelType MinimumColor = Pixel<PixelType>(0,0)[0];
  typename PixelType::ColorChannelType MaximumColor = Pixel<PixelType>(0,0)[0];
 
  for (UINT i=0; i<Height(); i++)
  {
    for (UINT j=0; j<Width(); j++)
    {
      for (UINT k=0; k<PixelType::Channels; k++)
      {
        MinimumColor = Min(MinimumColor, Pixel<PixelType>(i,j)[k]);
        MaximumColor = Max(MaximumColor, Pixel<PixelType>(i,j)[k]);
      }
    }
  }

  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      for (UINT k=0; k<PixelType::Channels; k++)
        Pixel<PixelType>(i,j)[k] = (PixelType::ColorChannelType)((Pixel<PixelType>(i,j)[k] - MinimumColor) / (FLOAT)(MaximumColor - MinimumColor));
  
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
Math::Histogram1<typename PixelType::ColorChannelType> Image::GetColorDistribution(UINT Bins)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
PixelType Image::SampleNearest(FLOAT i, FLOAT j) CONST
{
  return Pixel<PixelType>((INT)(i + 0.5f),(INT)(j + 0.5f));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
PixelType Image::SampleBilinear(FLOAT i, FLOAT j) CONST
{
  if (m_Boundary == ImageBoundary::NONE)
  {
    PixelType P;

    INT i0 = (INT)i;
    INT j0 = (INT)j;
    
    if (i < 0) i0--;
    if (j < 0) j0--;
    
    FLOAT di = i - i0;
    FLOAT dj = j - j0;
    FLOAT WeightSum = 0.0f;

    if (IsInsideImage(i0, j0))
    { P += (1.0f-di)*(1.0f-dj)*Pixel<PixelType>(i0, j0); WeightSum += (1.0f-di)*(1.0f-dj); }
    if (IsInsideImage(i0+1, j0))
    { P += (di)*(1.0f-dj)*Pixel<PixelType>(i0+1, j0); WeightSum += (di)*(1.0f-dj); }
    if (IsInsideImage(i0, j0+1))
    { P += (1.0f-di)*(dj)*Pixel<PixelType>(i0, j0+1); WeightSum += (1.0f-di)*(dj); }
    if (IsInsideImage(i0+1, j0+1))
    { P += (di)*(dj)*Pixel<PixelType>(i0+1, j0+1); WeightSum += (di)*(dj); }

    return P * (1.0f / WeightSum);
  }
  else
  {
    INT i0 = (INT)i;
    INT j0 = (INT)j;
    FLOAT di = i - i0;
    FLOAT dj = j - j0;

    return (1.0f-di)*(1.0f-dj)*Pixel<PixelType>(i0, j0) 
      + (di)*(1.0f-dj)*Pixel<PixelType>(i0+1, j0) 
      + (1.0f-di)*(dj)*Pixel<PixelType>(i0, j0+1) 
      + (di)*(dj)*Pixel<PixelType>(i0+1, j0+1);
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
INLINE FLOAT P0(FLOAT t, FLOAT a)
{
  return -a*t*t*t + a*t*t;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
INLINE FLOAT P1(FLOAT t, FLOAT a)
{
  return -(a+2)*t*t*t + (2*a+3)*t*t - a*t;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
INLINE FLOAT P2(FLOAT t, FLOAT a)
{
  return (a+2)*t*t*t - (a+3)*t*t + 1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
INLINE FLOAT P3(FLOAT t, FLOAT a)
{
  return a*t*t*t - 2*a*t*t + a*t;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
PixelType Image::SampleBicubic(FLOAT i, FLOAT j) CONST
{
  CONST FLOAT Alpha = 1.0f;

  if (m_Boundary == ImageBoundary::NONE)
  {
    PixelType P;

    INT i0 = (INT)i;
    INT j0 = (INT)j;
    
    if (i < 0) i0--;
    if (j < 0) j0--;
    
    FLOAT di = i - i0;
    FLOAT dj = j - j0;
    FLOAT WeightSum = 0.0f;

    if (IsInsideImage(i0, j0))
    { P += (1.0f-di)*(1.0f-dj)*Pixel<PixelType>(i0, j0); WeightSum += (1.0f-di)*(1.0f-dj); }
    if (IsInsideImage(i0+1, j0))
    { P += (di)*(1.0f-dj)*Pixel<PixelType>(i0+1, j0); WeightSum += (di)*(1.0f-dj); }
    if (IsInsideImage(i0, j0+1))
    { P += (1.0f-di)*(dj)*Pixel<PixelType>(i0, j0+1); WeightSum += (1.0f-di)*(dj); }
    if (IsInsideImage(i0+1, j0+1))
    { P += (di)*(dj)*Pixel<PixelType>(i0+1, j0+1); WeightSum += (di)*(dj); }

    return P * (1.0f / WeightSum);
  }
  else
  {
    INT i0 = (INT)i;
    INT j0 = (INT)j;
    FLOAT di = i - i0;
    FLOAT dj = j - j0;

    return (1.0f-di)*(1.0f-dj)*Pixel<PixelType>(i0, j0) 
      + (di)*(1.0f-dj)*Pixel<PixelType>(i0+1, j0) 
      + (1.0f-di)*(dj)*Pixel<PixelType>(i0, j0+1) 
      + (di)*(dj)*Pixel<PixelType>(i0+1, j0+1);
  }}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
PixelType Image::SampleGaussian(FLOAT i, FLOAT j, FLOAT Sigma) CONST
{
  return SampleNearest<PixelType>(i, j);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
VOID Image::Add(CONST Image& I)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j) += I.Pixel<PixelType>(i,j);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
VOID Image::Add(PixelType X)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j) += X;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
VOID Image::Add(typename PixelType::ColorChannelType X)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j) += X;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
VOID Image::Subtract(CONST Image& I)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j) -= I.Pixel<PixelType>(i,j);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
VOID Image::Subtract(PixelType X)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j) -= X;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
VOID Image::Subtract(typename PixelType::ColorChannelType X)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j) -= X;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
VOID Image::Multiply(CONST Image& I)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j) *= I.Pixel<PixelType>(i,j);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
VOID Image::Multiply(PixelType X)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j) *= X;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
VOID Image::Multiply(typename PixelType::ColorChannelType X)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j) *= X;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
VOID Image::Divide(CONST Image& I)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j) /= I.Pixel<PixelType>(i,j);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
VOID Image::Divide(PixelType X)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j) /= X;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
VOID Image::Divide(typename PixelType::ColorChannelType X)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j) /= X;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementTemplateRuntimeType(Bolitho::Graphics,TImage,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>::TImage() : Image()
{
  SetPixelFormat(PixelType::PixelFormat);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>::TImage(LPCTSTR Filename) : Image(Filename, PixelType::PixelFormat)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>::TImage(UINT Width, UINT Height) : Image(Width, Height, PixelType::PixelFormat)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>::TImage(UINT Width, UINT Height, CONST PixelType* pImageData) : Image(Width, Height, PixelType::PixelFormat, pImageData)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>::TImage(CONST Image& I) : Image(I)
{
  SetPixelFormat(PixelType::PixelFormat);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>& TImage<PixelType>::operator = (CONST Image& I)
{
  Resize(I.m_Width, I.m_Height);
  if (m_pPixelData)
    CopyMemory(m_pPixelData, I.GetPixelData(), sizeof(PixelType)*m_Width*m_Height);
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>::operator CONST PixelType* () CONST
{
  return (CONST PixelType*)m_pPixelData;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>::operator PixelType* ()
{
  return (PixelType*)m_pPixelData;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
VOID TImage<PixelType>::Load(LPCTSTR Filename)
{
  Image::Load(Filename, PixelType::PixelFormat);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
