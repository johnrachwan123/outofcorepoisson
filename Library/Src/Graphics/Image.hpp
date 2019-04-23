/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/Image.hpp $
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

#pragma once

#include "IO/File.hpp"
#include "IO/Path.hpp"
#include "Graphics/Color.hpp"
#include "IO/Path.hpp"
#include "Math/Histogram.hpp"

namespace Bolitho
{
  namespace Graphics
  {
    class LIBRARY_API ImageException : public UserException
    {
      DeclareRuntimeType();
    public:
      ImageException(CONST Object* Sender, LPCTSTR Description) : UserException(Sender)
      {
        m_Description = CharTraits<TCHAR>::Duplicate(Description);
      }

      ImageException(CONST ImageException& E) : UserException(E)
      {
        m_Description = CharTraits<TCHAR>::Duplicate(E.m_Description);
      }

      virtual ~ImageException()
      {
        if (m_Description)
          FreeMemory(m_Description);
        m_Description = 0;
      }

      virtual String Description() CONST
      {
        return m_Description;
      }

    private:
      TCHAR* m_Description;
    };

    /* The format of an image container on disk */
    struct ImageFormat 
    { 
      enum 
      { 
        UNKNOWN = 0x00000000, 
        BMP, 
        PNG, 
        JPEG, 
        TIFF, 
        WDP,
        FLT,
        PPMBINARY,
        PPM,
	EXR,
      };

      #if defined(USE_LUA)
      /* Generate Lua script bindings for this class */
      static Lua::Bind::Scope GenerateBindings(LPCTSTR Name);
      #endif
    };

    /* The behaviour of image indexing */
    struct ImageBoundary 
    { 
      enum 
      { 
        NONE, 
        REFLECTIVE, 
        PERIODIC, 
        CLAMP, 
        ZERO
      };

      #if defined(USE_LUA)
      /* Generate Lua script bindings for this class */
      static Lua::Bind::Scope GenerateBindings(LPCTSTR Name);
      #endif
    };

    class Image : public Object
    {
      DeclareRuntimeType();
    public:
      /* Constructs an empty image */
      Image();
      /* Construct a Width*Height image, with PixelFormat */
      Image(UINT Width, UINT Height, DWORD PixelFormat);
      /* Construct a Width*Height image, with PixelFormat.  Copy data from pImageData */
      Image(UINT Width, UINT Height, DWORD PixelFormat, LPCVOID pImageData);
      /* Construct an image as a copy of another image */
      Image(CONST Image& I);
      /* Construct an image by loading Filename from disk */
      Image(LPCTSTR Filename);
      /* Construct an image by loading Filename from disk with PixelFormat */
      Image(LPCTSTR Filename, DWORD PixelFormat);

      virtual ~Image();

      Image& operator = (CONST Image& I);

      /* Resizes the image, retaining existing the pixel format */
      VOID Resize(UINT Width, UINT Height);
      /* Resizes the image, selecting a new pixel format */
      VOID Resize(UINT Width, UINT Height, DWORD PixelFormat);

      /* Returns the width of the image */
      UINT Width() CONST
      { return m_Width; }
      /* Returns the height of the image */
      UINT Height() CONST
      { return m_Height; }
      /* Returns format of a pixel */
      DWORD GetPixelFormat() CONST
      { return m_PixelFormat; }

      /* Sets the behavior of image indexing */
      VOID SetBoundary(DWORD Boundary)
      { m_Boundary = Boundary; }
      /* Gets the behavior of image indexing */
      DWORD GetBoundary() CONST
      { return m_Boundary; }

      /* Returns the size of a single pixel, in bytes */
      UINT GetPixelSize() CONST
      { return GetPixelSize(m_PixelFormat); }
      /* Returns the number of color channels */
      UINT GetPixelChannels() CONST
      { return GetPixelChannels(m_PixelFormat); }

      /* Sets a new pixel format.  Converts data if nessesary */
      VOID SetPixelFormat(DWORD PixelFormat);

      /* Pixel Data access */
      LPVOID GetPixelData()
      { return m_pPixelData; }
      LPCVOID GetPixelData() CONST
      { return m_pPixelData; }

      /* Pixel access */
      template<class P>
      CONST P& Pixel(INT i, INT j) CONST;
      template<class P>
      P& Pixel(INT i, INT j);

      /* Pixel Sampling */
      template<class P>
      P SampleNearest(FLOAT i, FLOAT j) CONST;
      template<class P>
      P SampleBilinear(FLOAT i, FLOAT j) CONST;
      template<class P>
      P SampleBicubic(FLOAT  i, FLOAT j) CONST;
      template<class P>
      P SampleGaussian(FLOAT i, FLOAT j, FLOAT Sigma) CONST;

      /* Saves the image on disk with a certain container and pixel format */
      VOID Save(LPCTSTR Filename, DWORD ContainerFormat, DWORD PixelFormat) CONST;
      VOID Save(LPCTSTR Filename, DWORD PixelFormat) CONST
      { Save(Filename, ImageFormat::UNKNOWN, PixelFormat); }
      VOID Save(LPCTSTR Filename) CONST
      { Save(Filename, ImageFormat::UNKNOWN, PixelFormat::UNKNOWN); }

      /* Loads an image from disk with a specified pixel format */
      VOID Load(LPCTSTR Filename, DWORD PixelFormat);
      VOID Load(LPCTSTR Filename)
      { Load(Filename, PixelFormat::UNKNOWN); }

      /* Returns the native pixel format of an image on disk */
      STATIC DWORD GetImagePixelFormat(LPCTSTR Filename);

      /* Returns the nearest floating point version of a pixel format */
      STATIC DWORD GetNearestFloatingPointPixelFormat(DWORD PixelFormat);

      /* Returns the nearest byte version of a pixel format */
      STATIC DWORD GetNearestBytePixelFormat(DWORD PixelFormat);

      #if defined(USE_LUA)
      /* Generate Lua script bindings for this class */
      STATIC Lua::Bind::Scope GenerateBindings(LPCTSTR Name);
      #endif

      /* Returns the size (in bytes) of a pixel in a given PixelFormat */
      STATIC UINT GetPixelSize(DWORD PixelFormat)
      { return (PixelFormat & 0xff) / 8; }
      /* Returns the number of color channels of a pixel in a given PixelFormat */
      STATIC UINT GetPixelChannels(DWORD PixelFormat)
      { return (PixelFormat & 0xf0000000) >> 28; }

      /* Extracts a sub-image */
      Image GetSubImage(UINT x, UINT y, UINT Width, UINT Height);
      
      /* Crops the image */
      VOID Crop(UINT x, UINT y, UINT Width, UINT Height);

      /* Resizes the image with a sampling operator */
      template<class P>
      VOID ScaleNearest(UINT Width, UINT Height);
      template<class P>
      VOID ScaleBilinear(UINT Width, UINT Height);
      template<class P>
      VOID ScaleBicubic(UINT Width, UINT Height);
      template<class P>
      VOID ScaleGaussian(UINT Width, UINT Height);

      /* Aritmetic operations */
      template<class PixelType>
      VOID Add(CONST Image& I);
      template<class PixelType>
      VOID Add(PixelType X);
      template<class PixelType>
      VOID Add(typename PixelType::ColorChannelType X);
      template<class PixelType>
      VOID Subtract(CONST Image& I);
      template<class PixelType>
      VOID Subtract(PixelType X);
      template<class PixelType>
      VOID Subtract(typename PixelType::ColorChannelType X);
      template<class PixelType>
      VOID Multiply(CONST Image& I);
      template<class PixelType>
      VOID Multiply(PixelType X);
      template<class PixelType>
      VOID Multiply(typename PixelType::ColorChannelType X);
      template<class PixelType>
      VOID Divide(CONST Image& I);
      template<class PixelType>
      VOID Divide(PixelType X);
      template<class PixelType>
      VOID Divide(typename PixelType::ColorChannelType X);


      /* Clamps all color values in the image */
      template<class P>
      VOID Clamp();
      template<class P>
      VOID Clamp(typename P::ColorChannelType Low, typename P::ColorChannelType High);

      /* Normalizes all color values */
      template<class P>
      VOID Normalize();

      /* Computes a color distribution histograms */
      template<class P>
      Math::Histogram1<typename P::ColorChannelType> GetColorDistribution(UINT Bins);

      /* Computes the gradient in the X and Y directions */
      template<class P>
      Image ComputeGradientX() CONST;
      template<class P>
      Image ComputeGradientY() CONST;

      /* Computes the laplacian */
      template<class P>
      Image ComputeLaplacian() CONST;

      /* Computes the Discrete Wavelet Transform of the image */
      template<class P>
      Image ComputeWaveletTransform() CONST;

      /* Captures an image from a video device */
      #if defined(PLATFORM_WINDOWS)
      STATIC Image CaptureImage(UINT DeviceOrdinal, UINT Width, UINT Height, UINT FrameCount, UINT FrameDelay);
      STATIC Image CaptureImage(LPCTSTR DeviceName, UINT Width, UINT Height, UINT FrameCount, UINT FrameDelay);
      #endif
    protected:
      /* Converts a scanline of pixels from one format to another */
      STATIC VOID ConvertScanLine(LPVOID pOutData, DWORD OutPixelFormat, LPCVOID pInData, DWORD InPixelFormat, UINT Width);

      /* Returns TRUE if the pixel is within the boundas of the image */
      INLINE BOOL IsInsideImage(INT i, INT j) CONST
      { return i >= 0 && j >= 0 && (UINT)i < m_Height && (UINT)j < m_Width; }
      
      /* The width of the image, in pixels */
      UINT m_Width;
      /* The height of the image, in pixels */
      UINT m_Height;
      /* The format of the pixel data */
      DWORD m_PixelFormat;
      /* The pixel data */
      LPVOID m_pPixelData;

      /* The image boundary conditions */
      DWORD m_Boundary;

      /* Save to other formats */
      VOID SaveFLT(LPCTSTR Filename) CONST;
      VOID SavePPMBinary(LPCTSTR Filename) CONST;
      VOID SavePPM(LPCTSTR Filename) CONST;
      VOID SaveEXR(LPCTSTR Filename) CONST;

      #if defined(PLATFORM_POSIX)
      VOID SavePNG(LPCTSTR Filename) CONST;
      VOID SaveBMP(LPCTSTR Filename) CONST;
      VOID SaveJPEG(LPCTSTR Filename) CONST;
      #endif

      /* Load other formats */
      VOID LoadFLT(LPCTSTR Filename);
      VOID LoadPPMBinary(LPCTSTR Filename);
      VOID LoadPPM(LPCTSTR Filename);
      VOID LoadEXR(LPCTSTR Filename);

      #if defined(PLATFORM_POSIX)
      VOID LoadPNG(LPCTSTR Filename);
      VOID LoadBMP(LPCTSTR Filename);
      VOID LoadJPEG(LPCTSTR Filename);
      #endif

    };

    template<class PixelType>
    class TImage : public Image
    {
      DeclareRuntimeType();
    public:
      TImage();
      TImage(UINT Width, UINT Height);
      TImage(UINT Width, UINT Height, CONST PixelType* pImageData);
      TImage(CONST Image& I);
      TImage(LPCTSTR Filename);

      TImage& operator= (CONST Image& I);

      /* Get a pointer to the first pixel */
      operator CONST PixelType* () CONST;
      operator PixelType* ();

      /* Pixel Access */
      CONST PixelType& operator() (INT i, INT j) CONST
      { return Image::Pixel<PixelType>(i,j); }
      PixelType& operator() (INT i, INT j)
      { return Image::Pixel<PixelType>(i,j); }
      CONST PixelType& Pixel(INT i, INT j) CONST
      { return Image::Pixel<PixelType>(i,j); }
      PixelType& Pixel(INT i, INT j)
      { return Image::Pixel<PixelType>(i,j); }
      PixelType GetPixel(INT i, INT j) CONST
      { return Image::Pixel<PixelType>(i,j); }
      VOID SetPixel(INT i, INT j, PixelType P)
      { Image::Pixel<PixelType>(i,j) = P; }

      /* Pixel Sampling */
      PixelType SampleNearest(FLOAT i, FLOAT j) CONST
      { return Image::SampleNearest<PixelType>(i,j); }
      PixelType SampleBilinear(FLOAT i, FLOAT j) CONST
      { return Image::SampleBilinear<PixelType>(i,j); }
      PixelType SampleBicubic(FLOAT  i, FLOAT j) CONST
      { return Image::SampleBicubic<PixelType>(i,j); }
      PixelType SampleGaussian(FLOAT i, FLOAT j, FLOAT Sigma) CONST
      { return Image::SampleGaussian<PixelType>(i,j,Sigma); }

      /* Resizes the image with a sampling operator */
      VOID ScaleNearest(UINT Width, UINT Height)
      { Image::ScaleNearest<PixelType>(Width, Height); }
      VOID ScaleBilinear(UINT Width, UINT Height)
      { Image::ScaleBilinear<PixelType>(Width, Height); }
      VOID ScaleBicubic(UINT Width, UINT Height)
      { Image::ScaleBicubic<PixelType>(Width, Height); }
      VOID ScaleGaussian(UINT Width, UINT Height)
      { Image::ScaleGaussian<PixelType>(Width, Height); }

      /* Loads an image from disk with a specified pixel format */
      VOID Load(LPCTSTR Filename);

      VOID Normalize()
      { Image::Normalize<PixelType>(); }
      
      VOID Clamp()
      { Image::Clamp<PixelType>(); }
      VOID Clamp(typename PixelType::ColorChannelType Low, typename PixelType::ColorChannelType High)
      { Image::Clamp<PixelType>(Low, High); }

      /* Computes the gradient in the X and Y directions */
      Image ComputeGradientX() CONST
      { return Image::ComputeGradientX<PixelType>(); }
      Image ComputeGradientY() CONST
      { return Image::ComputeGradientY<PixelType>(); }

      /* Computes the laplacian */
      Image ComputeLaplacian() CONST
      { return Image::ComputeLaplacian<PixelType>(); }

      TImage& operator += (CONST TImage& I)
      { Image::Add(I); return *this; }
      TImage& operator += (PixelType X)
      { Image::Add(X); return *this; }
      TImage& operator += (typename PixelType::ColorChannelType X)
      { Image::Add(X); return *this; }

      TImage& operator -= (CONST TImage& I)
      { Image::Subtract(I); return *this; }
      TImage& operator -= (PixelType X)
      { Image::Subtract(X); return *this; }
      TImage& operator -= (typename PixelType::ColorChannelType X)
      { Image::Subtract(X); return *this; }

      TImage& operator *= (CONST TImage& I)
      { Image::Multiply(I); return *this; }
      TImage& operator *= (PixelType X)
      { Image::Multiply(X); return *this; }
      TImage& operator *= (typename PixelType::ColorChannelType X)
      { Image::Multiply(X); return *this; }

      TImage& operator /= (CONST TImage& I)
      { Image::Divide(I); return *this; }
      TImage& operator /= (PixelType X)
      { Image::Divide(X); return *this; }
      TImage& operator /= (typename PixelType::ColorChannelType X)
      { Image::Divide(X); return *this; }

      #if defined(USE_LUA)
      /* Generate Lua script bindings for this class */
      static Lua::Bind::Scope GenerateBindings(LPCTSTR Name);
      #endif

    private:
      VOID SetPixelFormat(DWORD PixelFormat)
      { Image::SetPixelFormat(PixelFormat); }

    };

    typedef TImage<ColorRGBA32> ImageRGBA32;
    typedef TImage<ColorRGB32> ImageRGB32;
    typedef TImage<ColorRGB24> ImageRGB24;
    typedef TImage<ColorBGRA32> ImageBGRA32;
    typedef TImage<ColorBGR32> ImageBGR32;
    typedef TImage<ColorBGR24> ImageBGR24;
    typedef TImage<ColorRGBA128F> ImageRGBA128F;
    typedef TImage<ColorRGB128F> ImageRGB128F;
    typedef TImage<ColorR8> ImageR8;
    typedef TImage<ColorR32F> ImageR32F;

    typedef ImageBGRA32 ImageB;
    typedef ImageRGBA128F ImageF;

    typedef ImageR8 ImageLumB;
    typedef ImageR32F ImageLumF;

    #include "ImageImpl.hpp"
  }
}

