/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/Color.hpp $
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

namespace Bolitho
{
  namespace Graphics
  {
    struct ColorChannel
    {
      enum
      {
        RED = 0,
        R = 0,
        GREEN = 1,
        G = 1,
        BLUE = 2,
        B = 2,
        ALPHA = 3,
        A = 3,
      };
      
      #if defined(USE_LUA)
      /* Generate Lua script bindings for this class */
      static Lua::Bind::Scope GenerateBindings(LPCTSTR Name);
      #endif
    };

    /* The format of a pixel in memory 
     * Numerical value interpreted as follows:
     *
     *  0xN0000000 = Number of color channels
     *  0x0T000000 = Data Type:
     *               1: 8bit INT
     *               2: 16bit INT
     *               3: 32bit INT
     *               4: 16bit FLOAT
     *               5: 32bit FLOAT
     *  0x00SS0000 = Number of bits for all channels 
     *  0x0000RGBA = Channel arrangement:
     *               0: No Channel
     *               1: 1st channel
     *               2: 2nd channel
     *               3: 3rd channel
     *               4: 4th channel
     */

    struct PixelFormat
    { 
      enum
      {
      UNKNOWN         = 0x00000000,

      R8              = 0x11081000,
      R16             = 0x12101000,
      R32             = 0x13201000,
      R32F            = 0x15201000,

      A8R8            = 0x21102001,
      A16R16          = 0x22202001,
      A32R32          = 0x23402001,
      A32FR32F        = 0x25402001,

      R8A8            = 0x21101002,
      R16A16          = 0x22201002,
      R32A32          = 0x23401002,
      R32FA32F        = 0x25401002,

      X8R8            = 0x11102000,
      X16R16          = 0x12202000,
      X32R32          = 0x13402000,
      X32R32F         = 0x15402000,

      R8X8            = 0x11101000,
      R16X16          = 0x12201000,
      R32X32          = 0x13401000,
      R32FX32         = 0x15401000,

      R8G8B8          = 0x31181230,
      R16G16B16       = 0x32301230,
      R32G32B32       = 0x33601230,
      R32FG32FB32F    = 0x35601230,

      A8R8G8B8          = 0x41202341,
      A16R16G16B16      = 0x42402341,
      A32R32G32B32      = 0x43802341,
      A32FR32FG32FB32F  = 0x45802341,

      R8G8B8A8          = 0x41201234,
      R16G16B16A16      = 0x42401234,
      R32G32B32A32      = 0x43801234,
      R32FG32FB32FA32F  = 0x45801234,

      X8R8G8B8          = 0x31202340,
      X16R16G16B16      = 0x32402340,
      X32R32G32B32      = 0x33802340,
      X32R32FG32FB32F   = 0x35802340,

      R8G8B8X8          = 0x31201230,
      R16G16B16X16      = 0x32401230,
      R32G32B32X32      = 0x33801230,
      R32FG32FB32FX32   = 0x35801230,

      B8G8R8          = 0x31183210,
      B16G16R16       = 0x32303210,
      B32G32R32       = 0x33603210,
      B32FG32FR32F    = 0x35603210,

      A8B8G8R8          = 0x41204321,
      A16B16G16R16      = 0x42404321,
      A32B32G32R32      = 0x43804321,
      A32FB32FG32FR32F  = 0x45804321,

      B8G8R8A8          = 0x41203214,
      B16G16R16A16      = 0x42403214,
      B32G32R32A32      = 0x43803214,
      B32FG32FR32FA32F  = 0x45803214,

      X8B8G8R8          = 0x31204320,
      X16B16G16R16      = 0x32404320,
      X32B32G32R32      = 0x33804320,
      X32B32FG32FR32F   = 0x35804320,

      B8G8R8X8          = 0x31203210,
      B16G16R16X16      = 0x32403210,
      B32G32R32X32      = 0x33803210,
      B32FG32FR32FX32   = 0x35803210,


      /*
      R8G8B8A8    = 0x41201234,
      RGB32       = 0x47100020,
      RGB24       = 0x39100018,
      BGRA32      = 0x41000020,
      BGR32       = 0x41010020,
      BGR24       = 0x31000018,
      RGBA128F    = 0x40100080,
      RGB128F     = 0x40110080,
      RGB96F      = 0x30100060,
      
      RGBA128     = 0x40200080,
      RGB128      = 0x40210080,
      RGB96       = 0x30200060,
      R8          = 0x12000008,
      R32F        = 0x12100020,
      R32         = 0x12200020,
      */
      };

      #if defined(USE_LUA)
      /* Generate Lua script bindings for this class */
      static Lua::Bind::Scope GenerateBindings(LPCTSTR Name);
      #endif

    };

    class LIBRARY_API ColorRGBA32
    {
    public:
      ColorRGBA32();
      ColorRGBA32(BYTE R, BYTE G, BYTE B, BYTE A);
      ColorRGBA32(BYTE R, BYTE G, BYTE B);
      ColorRGBA32(CONST ColorRGBA32& C);

      BYTE& R()
      { return m_V[0]; }
      CONST BYTE& R() CONST
      { return m_V[0]; }
      BYTE& G()
      { return m_V[1]; }
      CONST BYTE& G() CONST
      { return m_V[1]; }
      BYTE& B()
      { return m_V[2]; }
      CONST BYTE& B() CONST
      { return m_V[2]; }
      BYTE& A()
      { return m_V[3]; }
      CONST BYTE& A() CONST
      { return m_V[3]; }

      VOID SetR(BYTE r)
      { m_V[0] = r; }
      VOID SetG(BYTE g)
      { m_V[1] = g; }
      VOID SetB(BYTE b)
      { m_V[2] = b; }
      VOID SetA(BYTE a)
      { m_V[3] = a; }

      BYTE GetR() CONST
      { return m_V[0]; }
      BYTE GetG() CONST
      { return m_V[1]; }
      BYTE GetB() CONST
      { return m_V[2]; }
      BYTE GetA() CONST
      { return m_V[3]; }

      #if defined(USE_DIRECTX9)
      operator D3DCOLOR() CONST;
      operator D3DCOLORVALUE() CONST;
      static CONST D3DFORMAT DirectXPixelFormat = D3DFMT_A8B8G8R8;
      #endif

      static CONST DWORD PixelFormat = PixelFormat::R8G8B8A8;

      typedef BYTE ColorChannelType;

      operator CONST BYTE* () CONST
      { return &m_V[0]; }
      operator BYTE* ()
      { return &m_V[0]; }

      VOID Clamp();
      VOID Clamp(BYTE Min, BYTE Max);

      ColorRGBA32  operator +  (CONST ColorRGBA32& C) CONST;
      ColorRGBA32  operator -  (CONST ColorRGBA32& C) CONST;
      ColorRGBA32& operator += (CONST ColorRGBA32& C);
      ColorRGBA32& operator -= (CONST ColorRGBA32& C);

      ColorRGBA32  operator *  (FLOAT V) CONST;
      ColorRGBA32  operator /  (FLOAT V) CONST;
      ColorRGBA32& operator *= (FLOAT V);
      ColorRGBA32& operator /= (FLOAT V);

      friend ColorRGBA32 operator* (FLOAT V, CONST ColorRGBA32& C)
      { return C*V; }

      String ToString() CONST;

      #if defined(USE_LUA)
      static Lua::Bind::Scope GenerateBindings(LPCTSTR Name);
      #endif

      FLOAT Luminance() CONST;

      static ColorRGBA32 FromName(LPCTSTR String);
      static ColorRGBA32 FromHSV(FLOAT Hue, FLOAT Saturation, FLOAT Value);

      static ColorRGBA32 RandomColor(FLOAT Saturation = 1.0, FLOAT Value = 1.0);

      static CONST BYTE MaxChannelValue;
      static CONST UINT BitsPerChannel;
      static CONST UINT Channels;

      static CONST ColorRGBA32 Transparent;
      static CONST ColorRGBA32 AliceBlue;
      static CONST ColorRGBA32 AntiqueWhite;
      static CONST ColorRGBA32 Aqua;
      static CONST ColorRGBA32 Aquamarine;
      static CONST ColorRGBA32 Azure;
      static CONST ColorRGBA32 Beige;
      static CONST ColorRGBA32 Bisque;
      static CONST ColorRGBA32 Black;
      static CONST ColorRGBA32 BlanchedAlmond;
      static CONST ColorRGBA32 Blue;
      static CONST ColorRGBA32 BlueViolet;
      static CONST ColorRGBA32 Brown;
      static CONST ColorRGBA32 BurlyWood;
      static CONST ColorRGBA32 CadetBlue;
      static CONST ColorRGBA32 Chartreuse;
      static CONST ColorRGBA32 Chocolate;
      static CONST ColorRGBA32 Coral;
      static CONST ColorRGBA32 CornflowerBlue;
      static CONST ColorRGBA32 Cornsilk;
      static CONST ColorRGBA32 Crimson;
      static CONST ColorRGBA32 Cyan;
      static CONST ColorRGBA32 DarkBlue;
      static CONST ColorRGBA32 DarkCyan;
      static CONST ColorRGBA32 DarkGoldenrod;
      static CONST ColorRGBA32 DarkGray;
      static CONST ColorRGBA32 DarkGreen;
      static CONST ColorRGBA32 DarkKhaki;
      static CONST ColorRGBA32 DarkMagenta;
      static CONST ColorRGBA32 DarkOliveGreen;
      static CONST ColorRGBA32 DarkOrange;
      static CONST ColorRGBA32 DarkOrchid;
      static CONST ColorRGBA32 DarkRed;
      static CONST ColorRGBA32 DarkSalmon;
      static CONST ColorRGBA32 DarkSeaGreen;
      static CONST ColorRGBA32 DarkSlateBlue;
      static CONST ColorRGBA32 DarkSlateGray;
      static CONST ColorRGBA32 DarkTurquoise;
      static CONST ColorRGBA32 DarkViolet;
      static CONST ColorRGBA32 DeepPink;
      static CONST ColorRGBA32 DeepSkyBlue;
      static CONST ColorRGBA32 DimGray;
      static CONST ColorRGBA32 DodgerBlue;
      static CONST ColorRGBA32 Firebrick;
      static CONST ColorRGBA32 FloralWhite;
      static CONST ColorRGBA32 ForestGreen;
      static CONST ColorRGBA32 Fuchsia;
      static CONST ColorRGBA32 Gainsboro;
      static CONST ColorRGBA32 GhostWhite;
      static CONST ColorRGBA32 Gold;
      static CONST ColorRGBA32 Goldenrod;
      static CONST ColorRGBA32 Gray;
      static CONST ColorRGBA32 Green;
      static CONST ColorRGBA32 GreenYellow;
      static CONST ColorRGBA32 Honeydew;
      static CONST ColorRGBA32 HotPink;
      static CONST ColorRGBA32 IndianRed;
      static CONST ColorRGBA32 Indigo;
      static CONST ColorRGBA32 Ivory;
      static CONST ColorRGBA32 Khaki;
      static CONST ColorRGBA32 Lavender;
      static CONST ColorRGBA32 LavenderBlush;
      static CONST ColorRGBA32 LawnGreen;
      static CONST ColorRGBA32 LemonChiffon;
      static CONST ColorRGBA32 LightBlue;
      static CONST ColorRGBA32 LightCoral;
      static CONST ColorRGBA32 LightCyan;
      static CONST ColorRGBA32 LightGoldenrodYellow;
      static CONST ColorRGBA32 LightGreen;
      static CONST ColorRGBA32 LightGray;
      static CONST ColorRGBA32 LightPink;
      static CONST ColorRGBA32 LightSalmon;
      static CONST ColorRGBA32 LightSeaGreen;
      static CONST ColorRGBA32 LightSkyBlue;
      static CONST ColorRGBA32 LightSlateGray;
      static CONST ColorRGBA32 LightSteelBlue;
      static CONST ColorRGBA32 LightYellow;
      static CONST ColorRGBA32 Lime;
      static CONST ColorRGBA32 LimeGreen;
      static CONST ColorRGBA32 Linen;
      static CONST ColorRGBA32 Magenta;
      static CONST ColorRGBA32 Maroon;
      static CONST ColorRGBA32 MediumAquamarine;
      static CONST ColorRGBA32 MediumBlue;
      static CONST ColorRGBA32 MediumOrchid;
      static CONST ColorRGBA32 MediumPurple;
      static CONST ColorRGBA32 MediumSeaGreen;
      static CONST ColorRGBA32 MediumSlateBlue;
      static CONST ColorRGBA32 MediumSpringGreen;
      static CONST ColorRGBA32 MediumTurquoise;
      static CONST ColorRGBA32 MediumVioletRed;
      static CONST ColorRGBA32 MidnightBlue;
      static CONST ColorRGBA32 MintCream;
      static CONST ColorRGBA32 MistyRose;
      static CONST ColorRGBA32 Moccasin;
      static CONST ColorRGBA32 NavajoWhite;
      static CONST ColorRGBA32 Navy;
      static CONST ColorRGBA32 OldLace;
      static CONST ColorRGBA32 Olive;
      static CONST ColorRGBA32 OliveDrab;
      static CONST ColorRGBA32 Orange;
      static CONST ColorRGBA32 OrangeRed;
      static CONST ColorRGBA32 Orchid;
      static CONST ColorRGBA32 PaleGoldenrod;
      static CONST ColorRGBA32 PaleGreen;
      static CONST ColorRGBA32 PaleTurquoise;
      static CONST ColorRGBA32 PaleVioletRed;
      static CONST ColorRGBA32 PapayaWhip;
      static CONST ColorRGBA32 PeachPuff;
      static CONST ColorRGBA32 Peru;
      static CONST ColorRGBA32 Pink;
      static CONST ColorRGBA32 Plum;
      static CONST ColorRGBA32 PowderBlue;
      static CONST ColorRGBA32 Purple;
      static CONST ColorRGBA32 Red;
      static CONST ColorRGBA32 RosyBrown;
      static CONST ColorRGBA32 RoyalBlue;
      static CONST ColorRGBA32 SaddleBrown;
      static CONST ColorRGBA32 Salmon;
      static CONST ColorRGBA32 SandyBrown;
      static CONST ColorRGBA32 SeaGreen;
      static CONST ColorRGBA32 SeaShell;
      static CONST ColorRGBA32 Sienna;
      static CONST ColorRGBA32 Silver;
      static CONST ColorRGBA32 SkyBlue;
      static CONST ColorRGBA32 SlateBlue;
      static CONST ColorRGBA32 SlateGray;
      static CONST ColorRGBA32 Snow;
      static CONST ColorRGBA32 SpringGreen;
      static CONST ColorRGBA32 SteelBlue;
      static CONST ColorRGBA32 Tan;
      static CONST ColorRGBA32 Teal;
      static CONST ColorRGBA32 Thistle;
      static CONST ColorRGBA32 Tomato;
      static CONST ColorRGBA32 Turquoise;
      static CONST ColorRGBA32 Violet;
      static CONST ColorRGBA32 Wheat;
      static CONST ColorRGBA32 White;
      static CONST ColorRGBA32 WhiteSmoke;
      static CONST ColorRGBA32 Yellow;
      static CONST ColorRGBA32 YellowGreen;

    private:
      BYTE m_V[4];
    };

    class LIBRARY_API ColorRGB32
    {
    public:
      ColorRGB32();
      ColorRGB32(BYTE R, BYTE G, BYTE B);
      ColorRGB32(CONST ColorRGB32& C);

      BYTE& R()
      { return m_V[0]; }
      CONST BYTE& R() CONST
      { return m_V[0]; }
      BYTE& G()
      { return m_V[1]; }
      CONST BYTE& G() CONST
      { return m_V[1]; }
      BYTE& B()
      { return m_V[2]; }
      CONST BYTE& B() CONST
      { return m_V[2]; }

      VOID SetR(BYTE r)
      { m_V[0] = r; }
      VOID SetG(BYTE g)
      { m_V[1] = g; }
      VOID SetB(BYTE b)
      { m_V[2] = b; }

      BYTE GetR() CONST
      { return m_V[0]; }
      BYTE GetG() CONST
      { return m_V[1]; }
      BYTE GetB() CONST
      { return m_V[2]; }

#if defined(USE_DIRECTX9)
      operator D3DCOLOR() CONST;
      operator D3DCOLORVALUE() CONST;
      static CONST D3DFORMAT DirectXPixelFormat = D3DFMT_X8B8G8R8;
#endif

      static CONST DWORD PixelFormat = PixelFormat::R8G8B8X8;

      operator CONST BYTE* () CONST
      { return &m_V[0]; }
      operator BYTE* ()
      { return &m_V[0]; }

      VOID Clamp();
      VOID Clamp(BYTE Min, BYTE Max);

      ColorRGB32  operator +  (CONST ColorRGB32& C) CONST;
      ColorRGB32  operator -  (CONST ColorRGB32& C) CONST;
      ColorRGB32& operator += (CONST ColorRGB32& C);
      ColorRGB32& operator -= (CONST ColorRGB32& C);

      ColorRGB32  operator *  (FLOAT V) CONST;
      ColorRGB32  operator /  (FLOAT V) CONST;
      ColorRGB32& operator *= (FLOAT V);
      ColorRGB32& operator /= (FLOAT V);

      String ToString() CONST;

#if defined(USE_LUA)
      static Lua::Bind::Scope GenerateBindings(LPCTSTR Name);
#endif

      FLOAT Luminance() CONST;

      static ColorRGB32 FromName(LPCTSTR String);
      static ColorRGB32 FromHSV(FLOAT Hue, FLOAT Saturation, FLOAT Value);

      static ColorRGB32 RandomColor(FLOAT Saturation = 1.0, FLOAT Value = 1.0);

      static CONST BYTE MaxChannelValue;
      static CONST UINT BitsPerChannel;
      static CONST UINT Channels;

      static CONST ColorRGB32 AliceBlue;
      static CONST ColorRGB32 AntiqueWhite;
      static CONST ColorRGB32 Aqua;
      static CONST ColorRGB32 Aquamarine;
      static CONST ColorRGB32 Azure;
      static CONST ColorRGB32 Beige;
      static CONST ColorRGB32 Bisque;
      static CONST ColorRGB32 Black;
      static CONST ColorRGB32 BlanchedAlmond;
      static CONST ColorRGB32 Blue;
      static CONST ColorRGB32 BlueViolet;
      static CONST ColorRGB32 Brown;
      static CONST ColorRGB32 BurlyWood;
      static CONST ColorRGB32 CadetBlue;
      static CONST ColorRGB32 Chartreuse;
      static CONST ColorRGB32 Chocolate;
      static CONST ColorRGB32 Coral;
      static CONST ColorRGB32 CornflowerBlue;
      static CONST ColorRGB32 Cornsilk;
      static CONST ColorRGB32 Crimson;
      static CONST ColorRGB32 Cyan;
      static CONST ColorRGB32 DarkBlue;
      static CONST ColorRGB32 DarkCyan;
      static CONST ColorRGB32 DarkGoldenrod;
      static CONST ColorRGB32 DarkGray;
      static CONST ColorRGB32 DarkGreen;
      static CONST ColorRGB32 DarkKhaki;
      static CONST ColorRGB32 DarkMagenta;
      static CONST ColorRGB32 DarkOliveGreen;
      static CONST ColorRGB32 DarkOrange;
      static CONST ColorRGB32 DarkOrchid;
      static CONST ColorRGB32 DarkRed;
      static CONST ColorRGB32 DarkSalmon;
      static CONST ColorRGB32 DarkSeaGreen;
      static CONST ColorRGB32 DarkSlateBlue;
      static CONST ColorRGB32 DarkSlateGray;
      static CONST ColorRGB32 DarkTurquoise;
      static CONST ColorRGB32 DarkViolet;
      static CONST ColorRGB32 DeepPink;
      static CONST ColorRGB32 DeepSkyBlue;
      static CONST ColorRGB32 DimGray;
      static CONST ColorRGB32 DodgerBlue;
      static CONST ColorRGB32 Firebrick;
      static CONST ColorRGB32 FloralWhite;
      static CONST ColorRGB32 ForestGreen;
      static CONST ColorRGB32 Fuchsia;
      static CONST ColorRGB32 Gainsboro;
      static CONST ColorRGB32 GhostWhite;
      static CONST ColorRGB32 Gold;
      static CONST ColorRGB32 Goldenrod;
      static CONST ColorRGB32 Gray;
      static CONST ColorRGB32 Green;
      static CONST ColorRGB32 GreenYellow;
      static CONST ColorRGB32 Honeydew;
      static CONST ColorRGB32 HotPink;
      static CONST ColorRGB32 IndianRed;
      static CONST ColorRGB32 Indigo;
      static CONST ColorRGB32 Ivory;
      static CONST ColorRGB32 Khaki;
      static CONST ColorRGB32 Lavender;
      static CONST ColorRGB32 LavenderBlush;
      static CONST ColorRGB32 LawnGreen;
      static CONST ColorRGB32 LemonChiffon;
      static CONST ColorRGB32 LightBlue;
      static CONST ColorRGB32 LightCoral;
      static CONST ColorRGB32 LightCyan;
      static CONST ColorRGB32 LightGoldenrodYellow;
      static CONST ColorRGB32 LightGreen;
      static CONST ColorRGB32 LightGray;
      static CONST ColorRGB32 LightPink;
      static CONST ColorRGB32 LightSalmon;
      static CONST ColorRGB32 LightSeaGreen;
      static CONST ColorRGB32 LightSkyBlue;
      static CONST ColorRGB32 LightSlateGray;
      static CONST ColorRGB32 LightSteelBlue;
      static CONST ColorRGB32 LightYellow;
      static CONST ColorRGB32 Lime;
      static CONST ColorRGB32 LimeGreen;
      static CONST ColorRGB32 Linen;
      static CONST ColorRGB32 Magenta;
      static CONST ColorRGB32 Maroon;
      static CONST ColorRGB32 MediumAquamarine;
      static CONST ColorRGB32 MediumBlue;
      static CONST ColorRGB32 MediumOrchid;
      static CONST ColorRGB32 MediumPurple;
      static CONST ColorRGB32 MediumSeaGreen;
      static CONST ColorRGB32 MediumSlateBlue;
      static CONST ColorRGB32 MediumSpringGreen;
      static CONST ColorRGB32 MediumTurquoise;
      static CONST ColorRGB32 MediumVioletRed;
      static CONST ColorRGB32 MidnightBlue;
      static CONST ColorRGB32 MintCream;
      static CONST ColorRGB32 MistyRose;
      static CONST ColorRGB32 Moccasin;
      static CONST ColorRGB32 NavajoWhite;
      static CONST ColorRGB32 Navy;
      static CONST ColorRGB32 OldLace;
      static CONST ColorRGB32 Olive;
      static CONST ColorRGB32 OliveDrab;
      static CONST ColorRGB32 Orange;
      static CONST ColorRGB32 OrangeRed;
      static CONST ColorRGB32 Orchid;
      static CONST ColorRGB32 PaleGoldenrod;
      static CONST ColorRGB32 PaleGreen;
      static CONST ColorRGB32 PaleTurquoise;
      static CONST ColorRGB32 PaleVioletRed;
      static CONST ColorRGB32 PapayaWhip;
      static CONST ColorRGB32 PeachPuff;
      static CONST ColorRGB32 Peru;
      static CONST ColorRGB32 Pink;
      static CONST ColorRGB32 Plum;
      static CONST ColorRGB32 PowderBlue;
      static CONST ColorRGB32 Purple;
      static CONST ColorRGB32 Red;
      static CONST ColorRGB32 RosyBrown;
      static CONST ColorRGB32 RoyalBlue;
      static CONST ColorRGB32 SaddleBrown;
      static CONST ColorRGB32 Salmon;
      static CONST ColorRGB32 SandyBrown;
      static CONST ColorRGB32 SeaGreen;
      static CONST ColorRGB32 SeaShell;
      static CONST ColorRGB32 Sienna;
      static CONST ColorRGB32 Silver;
      static CONST ColorRGB32 SkyBlue;
      static CONST ColorRGB32 SlateBlue;
      static CONST ColorRGB32 SlateGray;
      static CONST ColorRGB32 Snow;
      static CONST ColorRGB32 SpringGreen;
      static CONST ColorRGB32 SteelBlue;
      static CONST ColorRGB32 Tan;
      static CONST ColorRGB32 Teal;
      static CONST ColorRGB32 Thistle;
      static CONST ColorRGB32 Tomato;
      static CONST ColorRGB32 Turquoise;
      static CONST ColorRGB32 Violet;
      static CONST ColorRGB32 Wheat;
      static CONST ColorRGB32 White;
      static CONST ColorRGB32 WhiteSmoke;
      static CONST ColorRGB32 Yellow;
      static CONST ColorRGB32 YellowGreen;

    private:
      BYTE m_V[4];
    };

    class LIBRARY_API ColorRGB24
    {
    public:
      ColorRGB24();
      ColorRGB24(BYTE R, BYTE G, BYTE B);
      ColorRGB24(CONST ColorRGB24& C);

      BYTE& R()
      { return m_V[0]; }
      CONST BYTE& R() CONST
      { return m_V[0]; }
      BYTE& G()
      { return m_V[1]; }
      CONST BYTE& G() CONST
      { return m_V[1]; }
      BYTE& B()
      { return m_V[2]; }
      CONST BYTE& B() CONST
      { return m_V[2]; }

      VOID SetR(BYTE r)
      { m_V[0] = r; }
      VOID SetG(BYTE g)
      { m_V[1] = g; }
      VOID SetB(BYTE b)
      { m_V[2] = b; }

      BYTE GetR() CONST
      { return m_V[0]; }
      BYTE GetG() CONST
      { return m_V[1]; }
      BYTE GetB() CONST
      { return m_V[2]; }

#if defined(USE_DIRECTX9)
      operator D3DCOLOR() CONST;
      operator D3DCOLORVALUE() CONST;
#endif

      static CONST DWORD PixelFormat = PixelFormat::R8G8B8;

      operator CONST BYTE* () CONST
      { return &m_V[0]; }
      operator BYTE* ()
      { return &m_V[0]; }

      VOID Clamp();
      VOID Clamp(BYTE Min, BYTE Max);

      ColorRGB24  operator +  (CONST ColorRGB24& C) CONST;
      ColorRGB24  operator -  (CONST ColorRGB24& C) CONST;
      ColorRGB24& operator += (CONST ColorRGB24& C);
      ColorRGB24& operator -= (CONST ColorRGB24& C);

      ColorRGB24  operator *  (FLOAT V) CONST;
      ColorRGB24  operator /  (FLOAT V) CONST;
      ColorRGB24& operator *= (FLOAT V);
      ColorRGB24& operator /= (FLOAT V);

      String ToString() CONST;

#if defined(USE_LUA)
      static Lua::Bind::Scope GenerateBindings(LPCTSTR Name);
#endif

      FLOAT Luminance() CONST;

      static ColorRGB24 FromName(LPCTSTR String);
      static ColorRGB24 FromHSV(FLOAT Hue, FLOAT Saturation, FLOAT Value);

      static ColorRGB24 RandomColor(FLOAT Saturation = 1.0, FLOAT Value = 1.0);

      static CONST BYTE MaxChannelValue;
      static CONST UINT BitsPerChannel;
      static CONST UINT Channels;

      static CONST ColorRGB24 AliceBlue;
      static CONST ColorRGB24 AntiqueWhite;
      static CONST ColorRGB24 Aqua;
      static CONST ColorRGB24 Aquamarine;
      static CONST ColorRGB24 Azure;
      static CONST ColorRGB24 Beige;
      static CONST ColorRGB24 Bisque;
      static CONST ColorRGB24 Black;
      static CONST ColorRGB24 BlanchedAlmond;
      static CONST ColorRGB24 Blue;
      static CONST ColorRGB24 BlueViolet;
      static CONST ColorRGB24 Brown;
      static CONST ColorRGB24 BurlyWood;
      static CONST ColorRGB24 CadetBlue;
      static CONST ColorRGB24 Chartreuse;
      static CONST ColorRGB24 Chocolate;
      static CONST ColorRGB24 Coral;
      static CONST ColorRGB24 CornflowerBlue;
      static CONST ColorRGB24 Cornsilk;
      static CONST ColorRGB24 Crimson;
      static CONST ColorRGB24 Cyan;
      static CONST ColorRGB24 DarkBlue;
      static CONST ColorRGB24 DarkCyan;
      static CONST ColorRGB24 DarkGoldenrod;
      static CONST ColorRGB24 DarkGray;
      static CONST ColorRGB24 DarkGreen;
      static CONST ColorRGB24 DarkKhaki;
      static CONST ColorRGB24 DarkMagenta;
      static CONST ColorRGB24 DarkOliveGreen;
      static CONST ColorRGB24 DarkOrange;
      static CONST ColorRGB24 DarkOrchid;
      static CONST ColorRGB24 DarkRed;
      static CONST ColorRGB24 DarkSalmon;
      static CONST ColorRGB24 DarkSeaGreen;
      static CONST ColorRGB24 DarkSlateBlue;
      static CONST ColorRGB24 DarkSlateGray;
      static CONST ColorRGB24 DarkTurquoise;
      static CONST ColorRGB24 DarkViolet;
      static CONST ColorRGB24 DeepPink;
      static CONST ColorRGB24 DeepSkyBlue;
      static CONST ColorRGB24 DimGray;
      static CONST ColorRGB24 DodgerBlue;
      static CONST ColorRGB24 Firebrick;
      static CONST ColorRGB24 FloralWhite;
      static CONST ColorRGB24 ForestGreen;
      static CONST ColorRGB24 Fuchsia;
      static CONST ColorRGB24 Gainsboro;
      static CONST ColorRGB24 GhostWhite;
      static CONST ColorRGB24 Gold;
      static CONST ColorRGB24 Goldenrod;
      static CONST ColorRGB24 Gray;
      static CONST ColorRGB24 Green;
      static CONST ColorRGB24 GreenYellow;
      static CONST ColorRGB24 Honeydew;
      static CONST ColorRGB24 HotPink;
      static CONST ColorRGB24 IndianRed;
      static CONST ColorRGB24 Indigo;
      static CONST ColorRGB24 Ivory;
      static CONST ColorRGB24 Khaki;
      static CONST ColorRGB24 Lavender;
      static CONST ColorRGB24 LavenderBlush;
      static CONST ColorRGB24 LawnGreen;
      static CONST ColorRGB24 LemonChiffon;
      static CONST ColorRGB24 LightBlue;
      static CONST ColorRGB24 LightCoral;
      static CONST ColorRGB24 LightCyan;
      static CONST ColorRGB24 LightGoldenrodYellow;
      static CONST ColorRGB24 LightGreen;
      static CONST ColorRGB24 LightGray;
      static CONST ColorRGB24 LightPink;
      static CONST ColorRGB24 LightSalmon;
      static CONST ColorRGB24 LightSeaGreen;
      static CONST ColorRGB24 LightSkyBlue;
      static CONST ColorRGB24 LightSlateGray;
      static CONST ColorRGB24 LightSteelBlue;
      static CONST ColorRGB24 LightYellow;
      static CONST ColorRGB24 Lime;
      static CONST ColorRGB24 LimeGreen;
      static CONST ColorRGB24 Linen;
      static CONST ColorRGB24 Magenta;
      static CONST ColorRGB24 Maroon;
      static CONST ColorRGB24 MediumAquamarine;
      static CONST ColorRGB24 MediumBlue;
      static CONST ColorRGB24 MediumOrchid;
      static CONST ColorRGB24 MediumPurple;
      static CONST ColorRGB24 MediumSeaGreen;
      static CONST ColorRGB24 MediumSlateBlue;
      static CONST ColorRGB24 MediumSpringGreen;
      static CONST ColorRGB24 MediumTurquoise;
      static CONST ColorRGB24 MediumVioletRed;
      static CONST ColorRGB24 MidnightBlue;
      static CONST ColorRGB24 MintCream;
      static CONST ColorRGB24 MistyRose;
      static CONST ColorRGB24 Moccasin;
      static CONST ColorRGB24 NavajoWhite;
      static CONST ColorRGB24 Navy;
      static CONST ColorRGB24 OldLace;
      static CONST ColorRGB24 Olive;
      static CONST ColorRGB24 OliveDrab;
      static CONST ColorRGB24 Orange;
      static CONST ColorRGB24 OrangeRed;
      static CONST ColorRGB24 Orchid;
      static CONST ColorRGB24 PaleGoldenrod;
      static CONST ColorRGB24 PaleGreen;
      static CONST ColorRGB24 PaleTurquoise;
      static CONST ColorRGB24 PaleVioletRed;
      static CONST ColorRGB24 PapayaWhip;
      static CONST ColorRGB24 PeachPuff;
      static CONST ColorRGB24 Peru;
      static CONST ColorRGB24 Pink;
      static CONST ColorRGB24 Plum;
      static CONST ColorRGB24 PowderBlue;
      static CONST ColorRGB24 Purple;
      static CONST ColorRGB24 Red;
      static CONST ColorRGB24 RosyBrown;
      static CONST ColorRGB24 RoyalBlue;
      static CONST ColorRGB24 SaddleBrown;
      static CONST ColorRGB24 Salmon;
      static CONST ColorRGB24 SandyBrown;
      static CONST ColorRGB24 SeaGreen;
      static CONST ColorRGB24 SeaShell;
      static CONST ColorRGB24 Sienna;
      static CONST ColorRGB24 Silver;
      static CONST ColorRGB24 SkyBlue;
      static CONST ColorRGB24 SlateBlue;
      static CONST ColorRGB24 SlateGray;
      static CONST ColorRGB24 Snow;
      static CONST ColorRGB24 SpringGreen;
      static CONST ColorRGB24 SteelBlue;
      static CONST ColorRGB24 Tan;
      static CONST ColorRGB24 Teal;
      static CONST ColorRGB24 Thistle;
      static CONST ColorRGB24 Tomato;
      static CONST ColorRGB24 Turquoise;
      static CONST ColorRGB24 Violet;
      static CONST ColorRGB24 Wheat;
      static CONST ColorRGB24 White;
      static CONST ColorRGB24 WhiteSmoke;
      static CONST ColorRGB24 Yellow;
      static CONST ColorRGB24 YellowGreen;

    private:
      BYTE m_V[3];
    };

    class LIBRARY_API ColorBGRA32
    {
    public:
      ColorBGRA32();
      ColorBGRA32(BYTE R, BYTE G, BYTE B);
      ColorBGRA32(BYTE R, BYTE G, BYTE B, BYTE A);
      ColorBGRA32(CONST ColorBGRA32& C);

      BYTE& R()
      { return m_V[2]; }
      CONST BYTE& R() CONST
      { return m_V[2]; }
      BYTE& G()
      { return m_V[1]; }
      CONST BYTE& G() CONST
      { return m_V[1]; }
      BYTE& B()
      { return m_V[0]; }
      CONST BYTE& B() CONST
      { return m_V[0]; }
      BYTE& A()
      { return m_V[3]; }
      CONST BYTE& A() CONST
      { return m_V[3]; }

      VOID SetR(BYTE r)
      { m_V[2] = r; }
      VOID SetG(BYTE g)
      { m_V[1] = g; }
      VOID SetB(BYTE b)
      { m_V[0] = b; }
      VOID SetA(BYTE a)
      { m_V[3] = a; }

      BYTE GetR() CONST
      { return m_V[2]; }
      BYTE GetG() CONST
      { return m_V[1]; }
      BYTE GetB() CONST
      { return m_V[0]; }
      BYTE GetA() CONST
      { return m_V[3]; }

#if defined(USE_DIRECTX9)
      operator D3DCOLOR() CONST;
      operator D3DCOLORVALUE() CONST;
      static CONST D3DFORMAT DirectXPixelFormat = D3DFMT_A8R8G8B8;
#endif

      static CONST DWORD PixelFormat = PixelFormat::B8G8R8A8;

      operator CONST BYTE* () CONST
      { return &m_V[0]; }
      operator BYTE* ()
      { return &m_V[0]; }

      VOID Clamp();
      VOID Clamp(BYTE Min, BYTE Max);

      ColorBGRA32  operator +  (CONST ColorBGRA32& C) CONST;
      ColorBGRA32  operator -  (CONST ColorBGRA32& C) CONST;
      ColorBGRA32& operator += (CONST ColorBGRA32& C);
      ColorBGRA32& operator -= (CONST ColorBGRA32& C);

      ColorBGRA32  operator *  (FLOAT V) CONST;
      ColorBGRA32  operator /  (FLOAT V) CONST;
      ColorBGRA32& operator *= (FLOAT V);
      ColorBGRA32& operator /= (FLOAT V);

      String ToString() CONST;

#if defined(USE_LUA)
      static Lua::Bind::Scope GenerateBindings(LPCTSTR Name);
#endif

      FLOAT Luminance() CONST;

      static ColorBGRA32 FromName(LPCTSTR String);
      static ColorBGRA32 FromHSV(FLOAT Hue, FLOAT Saturation, FLOAT Value);

      static ColorBGRA32 RandomColor(FLOAT Saturation = 1.0, FLOAT Value = 1.0);

      static CONST BYTE MaxChannelValue;
      static CONST UINT BitsPerChannel;
      static CONST UINT Channels;

      static CONST ColorBGRA32 Transparent;
      static CONST ColorBGRA32 AliceBlue;
      static CONST ColorBGRA32 AntiqueWhite;
      static CONST ColorBGRA32 Aqua;
      static CONST ColorBGRA32 Aquamarine;
      static CONST ColorBGRA32 Azure;
      static CONST ColorBGRA32 Beige;
      static CONST ColorBGRA32 Bisque;
      static CONST ColorBGRA32 Black;
      static CONST ColorBGRA32 BlanchedAlmond;
      static CONST ColorBGRA32 Blue;
      static CONST ColorBGRA32 BlueViolet;
      static CONST ColorBGRA32 Brown;
      static CONST ColorBGRA32 BurlyWood;
      static CONST ColorBGRA32 CadetBlue;
      static CONST ColorBGRA32 Chartreuse;
      static CONST ColorBGRA32 Chocolate;
      static CONST ColorBGRA32 Coral;
      static CONST ColorBGRA32 CornflowerBlue;
      static CONST ColorBGRA32 Cornsilk;
      static CONST ColorBGRA32 Crimson;
      static CONST ColorBGRA32 Cyan;
      static CONST ColorBGRA32 DarkBlue;
      static CONST ColorBGRA32 DarkCyan;
      static CONST ColorBGRA32 DarkGoldenrod;
      static CONST ColorBGRA32 DarkGray;
      static CONST ColorBGRA32 DarkGreen;
      static CONST ColorBGRA32 DarkKhaki;
      static CONST ColorBGRA32 DarkMagenta;
      static CONST ColorBGRA32 DarkOliveGreen;
      static CONST ColorBGRA32 DarkOrange;
      static CONST ColorBGRA32 DarkOrchid;
      static CONST ColorBGRA32 DarkRed;
      static CONST ColorBGRA32 DarkSalmon;
      static CONST ColorBGRA32 DarkSeaGreen;
      static CONST ColorBGRA32 DarkSlateBlue;
      static CONST ColorBGRA32 DarkSlateGray;
      static CONST ColorBGRA32 DarkTurquoise;
      static CONST ColorBGRA32 DarkViolet;
      static CONST ColorBGRA32 DeepPink;
      static CONST ColorBGRA32 DeepSkyBlue;
      static CONST ColorBGRA32 DimGray;
      static CONST ColorBGRA32 DodgerBlue;
      static CONST ColorBGRA32 Firebrick;
      static CONST ColorBGRA32 FloralWhite;
      static CONST ColorBGRA32 ForestGreen;
      static CONST ColorBGRA32 Fuchsia;
      static CONST ColorBGRA32 Gainsboro;
      static CONST ColorBGRA32 GhostWhite;
      static CONST ColorBGRA32 Gold;
      static CONST ColorBGRA32 Goldenrod;
      static CONST ColorBGRA32 Gray;
      static CONST ColorBGRA32 Green;
      static CONST ColorBGRA32 GreenYellow;
      static CONST ColorBGRA32 Honeydew;
      static CONST ColorBGRA32 HotPink;
      static CONST ColorBGRA32 IndianRed;
      static CONST ColorBGRA32 Indigo;
      static CONST ColorBGRA32 Ivory;
      static CONST ColorBGRA32 Khaki;
      static CONST ColorBGRA32 Lavender;
      static CONST ColorBGRA32 LavenderBlush;
      static CONST ColorBGRA32 LawnGreen;
      static CONST ColorBGRA32 LemonChiffon;
      static CONST ColorBGRA32 LightBlue;
      static CONST ColorBGRA32 LightCoral;
      static CONST ColorBGRA32 LightCyan;
      static CONST ColorBGRA32 LightGoldenrodYellow;
      static CONST ColorBGRA32 LightGreen;
      static CONST ColorBGRA32 LightGray;
      static CONST ColorBGRA32 LightPink;
      static CONST ColorBGRA32 LightSalmon;
      static CONST ColorBGRA32 LightSeaGreen;
      static CONST ColorBGRA32 LightSkyBlue;
      static CONST ColorBGRA32 LightSlateGray;
      static CONST ColorBGRA32 LightSteelBlue;
      static CONST ColorBGRA32 LightYellow;
      static CONST ColorBGRA32 Lime;
      static CONST ColorBGRA32 LimeGreen;
      static CONST ColorBGRA32 Linen;
      static CONST ColorBGRA32 Magenta;
      static CONST ColorBGRA32 Maroon;
      static CONST ColorBGRA32 MediumAquamarine;
      static CONST ColorBGRA32 MediumBlue;
      static CONST ColorBGRA32 MediumOrchid;
      static CONST ColorBGRA32 MediumPurple;
      static CONST ColorBGRA32 MediumSeaGreen;
      static CONST ColorBGRA32 MediumSlateBlue;
      static CONST ColorBGRA32 MediumSpringGreen;
      static CONST ColorBGRA32 MediumTurquoise;
      static CONST ColorBGRA32 MediumVioletRed;
      static CONST ColorBGRA32 MidnightBlue;
      static CONST ColorBGRA32 MintCream;
      static CONST ColorBGRA32 MistyRose;
      static CONST ColorBGRA32 Moccasin;
      static CONST ColorBGRA32 NavajoWhite;
      static CONST ColorBGRA32 Navy;
      static CONST ColorBGRA32 OldLace;
      static CONST ColorBGRA32 Olive;
      static CONST ColorBGRA32 OliveDrab;
      static CONST ColorBGRA32 Orange;
      static CONST ColorBGRA32 OrangeRed;
      static CONST ColorBGRA32 Orchid;
      static CONST ColorBGRA32 PaleGoldenrod;
      static CONST ColorBGRA32 PaleGreen;
      static CONST ColorBGRA32 PaleTurquoise;
      static CONST ColorBGRA32 PaleVioletRed;
      static CONST ColorBGRA32 PapayaWhip;
      static CONST ColorBGRA32 PeachPuff;
      static CONST ColorBGRA32 Peru;
      static CONST ColorBGRA32 Pink;
      static CONST ColorBGRA32 Plum;
      static CONST ColorBGRA32 PowderBlue;
      static CONST ColorBGRA32 Purple;
      static CONST ColorBGRA32 Red;
      static CONST ColorBGRA32 RosyBrown;
      static CONST ColorBGRA32 RoyalBlue;
      static CONST ColorBGRA32 SaddleBrown;
      static CONST ColorBGRA32 Salmon;
      static CONST ColorBGRA32 SandyBrown;
      static CONST ColorBGRA32 SeaGreen;
      static CONST ColorBGRA32 SeaShell;
      static CONST ColorBGRA32 Sienna;
      static CONST ColorBGRA32 Silver;
      static CONST ColorBGRA32 SkyBlue;
      static CONST ColorBGRA32 SlateBlue;
      static CONST ColorBGRA32 SlateGray;
      static CONST ColorBGRA32 Snow;
      static CONST ColorBGRA32 SpringGreen;
      static CONST ColorBGRA32 SteelBlue;
      static CONST ColorBGRA32 Tan;
      static CONST ColorBGRA32 Teal;
      static CONST ColorBGRA32 Thistle;
      static CONST ColorBGRA32 Tomato;
      static CONST ColorBGRA32 Turquoise;
      static CONST ColorBGRA32 Violet;
      static CONST ColorBGRA32 Wheat;
      static CONST ColorBGRA32 White;
      static CONST ColorBGRA32 WhiteSmoke;
      static CONST ColorBGRA32 Yellow;
      static CONST ColorBGRA32 YellowGreen;

    private:
      BYTE m_V[4];
    };

    class LIBRARY_API ColorBGR32
    {
    public:
      ColorBGR32();
      ColorBGR32(BYTE R, BYTE G, BYTE B);
      ColorBGR32(CONST ColorBGR32& C);

      BYTE& R()
      { return m_V[2]; }
      CONST BYTE& R() CONST
      { return m_V[2]; }
      BYTE& G()
      { return m_V[1]; }
      CONST BYTE& G() CONST
      { return m_V[1]; }
      BYTE& B()
      { return m_V[0]; }
      CONST BYTE& B() CONST
      { return m_V[0]; }

      VOID SetR(BYTE r)
      { m_V[2] = r; }
      VOID SetG(BYTE g)
      { m_V[1] = g; }
      VOID SetB(BYTE b)
      { m_V[0] = b; }

      BYTE GetR() CONST
      { return m_V[2]; }
      BYTE GetG() CONST
      { return m_V[1]; }
      BYTE GetB() CONST
      { return m_V[0]; }

#if defined(USE_DIRECTX9)
      operator D3DCOLOR() CONST;
      operator D3DCOLORVALUE() CONST;
      static CONST D3DFORMAT DirectXPixelFormat = D3DFMT_X8R8G8B8;
#endif
      static CONST DWORD PixelFormat = PixelFormat::B8G8R8X8;


      operator CONST BYTE* () CONST
      { return &m_V[0]; }
      operator BYTE* ()
      { return &m_V[0]; }

      VOID Clamp();
      VOID Clamp(BYTE Min, BYTE Max);

      ColorBGR32  operator +  (CONST ColorBGR32& C) CONST;
      ColorBGR32  operator -  (CONST ColorBGR32& C) CONST;
      ColorBGR32& operator += (CONST ColorBGR32& C);
      ColorBGR32& operator -= (CONST ColorBGR32& C);

      ColorBGR32  operator *  (FLOAT V) CONST;
      ColorBGR32  operator /  (FLOAT V) CONST;
      ColorBGR32& operator *= (FLOAT V);
      ColorBGR32& operator /= (FLOAT V);

      String ToString() CONST;

#if defined(USE_LUA)
      static Lua::Bind::Scope GenerateBindings(LPCTSTR Name);
#endif

      FLOAT Luminance() CONST;

      static ColorBGR32 FromName(LPCTSTR String);
      static ColorBGR32 FromHSV(FLOAT Hue, FLOAT Saturation, FLOAT Value);

      static ColorBGR32 RandomColor(FLOAT Saturation = 1.0, FLOAT Value = 1.0);

      static CONST BYTE MaxChannelValue;
      static CONST UINT BitsPerChannel;
      static CONST UINT Channels;

      static CONST ColorBGR32 AliceBlue;
      static CONST ColorBGR32 AntiqueWhite;
      static CONST ColorBGR32 Aqua;
      static CONST ColorBGR32 Aquamarine;
      static CONST ColorBGR32 Azure;
      static CONST ColorBGR32 Beige;
      static CONST ColorBGR32 Bisque;
      static CONST ColorBGR32 Black;
      static CONST ColorBGR32 BlanchedAlmond;
      static CONST ColorBGR32 Blue;
      static CONST ColorBGR32 BlueViolet;
      static CONST ColorBGR32 Brown;
      static CONST ColorBGR32 BurlyWood;
      static CONST ColorBGR32 CadetBlue;
      static CONST ColorBGR32 Chartreuse;
      static CONST ColorBGR32 Chocolate;
      static CONST ColorBGR32 Coral;
      static CONST ColorBGR32 CornflowerBlue;
      static CONST ColorBGR32 Cornsilk;
      static CONST ColorBGR32 Crimson;
      static CONST ColorBGR32 Cyan;
      static CONST ColorBGR32 DarkBlue;
      static CONST ColorBGR32 DarkCyan;
      static CONST ColorBGR32 DarkGoldenrod;
      static CONST ColorBGR32 DarkGray;
      static CONST ColorBGR32 DarkGreen;
      static CONST ColorBGR32 DarkKhaki;
      static CONST ColorBGR32 DarkMagenta;
      static CONST ColorBGR32 DarkOliveGreen;
      static CONST ColorBGR32 DarkOrange;
      static CONST ColorBGR32 DarkOrchid;
      static CONST ColorBGR32 DarkRed;
      static CONST ColorBGR32 DarkSalmon;
      static CONST ColorBGR32 DarkSeaGreen;
      static CONST ColorBGR32 DarkSlateBlue;
      static CONST ColorBGR32 DarkSlateGray;
      static CONST ColorBGR32 DarkTurquoise;
      static CONST ColorBGR32 DarkViolet;
      static CONST ColorBGR32 DeepPink;
      static CONST ColorBGR32 DeepSkyBlue;
      static CONST ColorBGR32 DimGray;
      static CONST ColorBGR32 DodgerBlue;
      static CONST ColorBGR32 Firebrick;
      static CONST ColorBGR32 FloralWhite;
      static CONST ColorBGR32 ForestGreen;
      static CONST ColorBGR32 Fuchsia;
      static CONST ColorBGR32 Gainsboro;
      static CONST ColorBGR32 GhostWhite;
      static CONST ColorBGR32 Gold;
      static CONST ColorBGR32 Goldenrod;
      static CONST ColorBGR32 Gray;
      static CONST ColorBGR32 Green;
      static CONST ColorBGR32 GreenYellow;
      static CONST ColorBGR32 Honeydew;
      static CONST ColorBGR32 HotPink;
      static CONST ColorBGR32 IndianRed;
      static CONST ColorBGR32 Indigo;
      static CONST ColorBGR32 Ivory;
      static CONST ColorBGR32 Khaki;
      static CONST ColorBGR32 Lavender;
      static CONST ColorBGR32 LavenderBlush;
      static CONST ColorBGR32 LawnGreen;
      static CONST ColorBGR32 LemonChiffon;
      static CONST ColorBGR32 LightBlue;
      static CONST ColorBGR32 LightCoral;
      static CONST ColorBGR32 LightCyan;
      static CONST ColorBGR32 LightGoldenrodYellow;
      static CONST ColorBGR32 LightGreen;
      static CONST ColorBGR32 LightGray;
      static CONST ColorBGR32 LightPink;
      static CONST ColorBGR32 LightSalmon;
      static CONST ColorBGR32 LightSeaGreen;
      static CONST ColorBGR32 LightSkyBlue;
      static CONST ColorBGR32 LightSlateGray;
      static CONST ColorBGR32 LightSteelBlue;
      static CONST ColorBGR32 LightYellow;
      static CONST ColorBGR32 Lime;
      static CONST ColorBGR32 LimeGreen;
      static CONST ColorBGR32 Linen;
      static CONST ColorBGR32 Magenta;
      static CONST ColorBGR32 Maroon;
      static CONST ColorBGR32 MediumAquamarine;
      static CONST ColorBGR32 MediumBlue;
      static CONST ColorBGR32 MediumOrchid;
      static CONST ColorBGR32 MediumPurple;
      static CONST ColorBGR32 MediumSeaGreen;
      static CONST ColorBGR32 MediumSlateBlue;
      static CONST ColorBGR32 MediumSpringGreen;
      static CONST ColorBGR32 MediumTurquoise;
      static CONST ColorBGR32 MediumVioletRed;
      static CONST ColorBGR32 MidnightBlue;
      static CONST ColorBGR32 MintCream;
      static CONST ColorBGR32 MistyRose;
      static CONST ColorBGR32 Moccasin;
      static CONST ColorBGR32 NavajoWhite;
      static CONST ColorBGR32 Navy;
      static CONST ColorBGR32 OldLace;
      static CONST ColorBGR32 Olive;
      static CONST ColorBGR32 OliveDrab;
      static CONST ColorBGR32 Orange;
      static CONST ColorBGR32 OrangeRed;
      static CONST ColorBGR32 Orchid;
      static CONST ColorBGR32 PaleGoldenrod;
      static CONST ColorBGR32 PaleGreen;
      static CONST ColorBGR32 PaleTurquoise;
      static CONST ColorBGR32 PaleVioletRed;
      static CONST ColorBGR32 PapayaWhip;
      static CONST ColorBGR32 PeachPuff;
      static CONST ColorBGR32 Peru;
      static CONST ColorBGR32 Pink;
      static CONST ColorBGR32 Plum;
      static CONST ColorBGR32 PowderBlue;
      static CONST ColorBGR32 Purple;
      static CONST ColorBGR32 Red;
      static CONST ColorBGR32 RosyBrown;
      static CONST ColorBGR32 RoyalBlue;
      static CONST ColorBGR32 SaddleBrown;
      static CONST ColorBGR32 Salmon;
      static CONST ColorBGR32 SandyBrown;
      static CONST ColorBGR32 SeaGreen;
      static CONST ColorBGR32 SeaShell;
      static CONST ColorBGR32 Sienna;
      static CONST ColorBGR32 Silver;
      static CONST ColorBGR32 SkyBlue;
      static CONST ColorBGR32 SlateBlue;
      static CONST ColorBGR32 SlateGray;
      static CONST ColorBGR32 Snow;
      static CONST ColorBGR32 SpringGreen;
      static CONST ColorBGR32 SteelBlue;
      static CONST ColorBGR32 Tan;
      static CONST ColorBGR32 Teal;
      static CONST ColorBGR32 Thistle;
      static CONST ColorBGR32 Tomato;
      static CONST ColorBGR32 Turquoise;
      static CONST ColorBGR32 Violet;
      static CONST ColorBGR32 Wheat;
      static CONST ColorBGR32 White;
      static CONST ColorBGR32 WhiteSmoke;
      static CONST ColorBGR32 Yellow;
      static CONST ColorBGR32 YellowGreen;

    private:
      BYTE m_V[4];
    };

    class LIBRARY_API ColorBGR24
    {
    public:
      ColorBGR24();
      ColorBGR24(BYTE R, BYTE G, BYTE B);
      ColorBGR24(CONST ColorBGR24& C);

      BYTE& R()
      { return m_V[0]; }
      CONST BYTE& R() CONST
      { return m_V[0]; }
      BYTE& G()
      { return m_V[1]; }
      CONST BYTE& G() CONST
      { return m_V[1]; }
      BYTE& B()
      { return m_V[2]; }
      CONST BYTE& B() CONST
      { return m_V[2]; }

      VOID SetR(BYTE r)
      { m_V[0] = r; }
      VOID SetG(BYTE g)
      { m_V[1] = g; }
      VOID SetB(BYTE b)
      { m_V[2] = b; }

      BYTE GetR() CONST
      { return m_V[0]; }
      BYTE GetG() CONST
      { return m_V[1]; }
      BYTE GetB() CONST
      { return m_V[2]; }

#if defined(USE_DIRECTX9)
      operator D3DCOLOR() CONST;
      operator D3DCOLORVALUE() CONST;
      static CONST D3DFORMAT DirectXPixelFormat = D3DFMT_R8G8B8;
#endif
      static CONST DWORD PixelFormat = PixelFormat::B8G8R8;


      operator CONST BYTE* () CONST
      { return &m_V[0]; }
      operator BYTE* ()
      { return &m_V[0]; }

      VOID Clamp();
      VOID Clamp(BYTE Min, BYTE Max);

      ColorBGR24  operator +  (CONST ColorBGR24& C) CONST;
      ColorBGR24  operator -  (CONST ColorBGR24& C) CONST;
      ColorBGR24& operator += (CONST ColorBGR24& C);
      ColorBGR24& operator -= (CONST ColorBGR24& C);

      ColorBGR24  operator *  (FLOAT V) CONST;
      ColorBGR24  operator /  (FLOAT V) CONST;
      ColorBGR24& operator *= (FLOAT V);
      ColorBGR24& operator /= (FLOAT V);

      String ToString() CONST;

#if defined(USE_LUA)
      static Lua::Bind::Scope GenerateBindings(LPCTSTR Name);
#endif

      FLOAT Luminance() CONST;

      static ColorBGR24 FromName(LPCTSTR String);
      static ColorBGR24 FromHSV(FLOAT Hue, FLOAT Saturation, FLOAT Value);

      static ColorBGR24 RandomColor(FLOAT Saturation = 1.0, FLOAT Value = 1.0);

      static CONST BYTE MaxChannelValue;
      static CONST UINT BitsPerChannel;
      static CONST UINT Channels;

      static CONST ColorBGR24 AliceBlue;
      static CONST ColorBGR24 AntiqueWhite;
      static CONST ColorBGR24 Aqua;
      static CONST ColorBGR24 Aquamarine;
      static CONST ColorBGR24 Azure;
      static CONST ColorBGR24 Beige;
      static CONST ColorBGR24 Bisque;
      static CONST ColorBGR24 Black;
      static CONST ColorBGR24 BlanchedAlmond;
      static CONST ColorBGR24 Blue;
      static CONST ColorBGR24 BlueViolet;
      static CONST ColorBGR24 Brown;
      static CONST ColorBGR24 BurlyWood;
      static CONST ColorBGR24 CadetBlue;
      static CONST ColorBGR24 Chartreuse;
      static CONST ColorBGR24 Chocolate;
      static CONST ColorBGR24 Coral;
      static CONST ColorBGR24 CornflowerBlue;
      static CONST ColorBGR24 Cornsilk;
      static CONST ColorBGR24 Crimson;
      static CONST ColorBGR24 Cyan;
      static CONST ColorBGR24 DarkBlue;
      static CONST ColorBGR24 DarkCyan;
      static CONST ColorBGR24 DarkGoldenrod;
      static CONST ColorBGR24 DarkGray;
      static CONST ColorBGR24 DarkGreen;
      static CONST ColorBGR24 DarkKhaki;
      static CONST ColorBGR24 DarkMagenta;
      static CONST ColorBGR24 DarkOliveGreen;
      static CONST ColorBGR24 DarkOrange;
      static CONST ColorBGR24 DarkOrchid;
      static CONST ColorBGR24 DarkRed;
      static CONST ColorBGR24 DarkSalmon;
      static CONST ColorBGR24 DarkSeaGreen;
      static CONST ColorBGR24 DarkSlateBlue;
      static CONST ColorBGR24 DarkSlateGray;
      static CONST ColorBGR24 DarkTurquoise;
      static CONST ColorBGR24 DarkViolet;
      static CONST ColorBGR24 DeepPink;
      static CONST ColorBGR24 DeepSkyBlue;
      static CONST ColorBGR24 DimGray;
      static CONST ColorBGR24 DodgerBlue;
      static CONST ColorBGR24 Firebrick;
      static CONST ColorBGR24 FloralWhite;
      static CONST ColorBGR24 ForestGreen;
      static CONST ColorBGR24 Fuchsia;
      static CONST ColorBGR24 Gainsboro;
      static CONST ColorBGR24 GhostWhite;
      static CONST ColorBGR24 Gold;
      static CONST ColorBGR24 Goldenrod;
      static CONST ColorBGR24 Gray;
      static CONST ColorBGR24 Green;
      static CONST ColorBGR24 GreenYellow;
      static CONST ColorBGR24 Honeydew;
      static CONST ColorBGR24 HotPink;
      static CONST ColorBGR24 IndianRed;
      static CONST ColorBGR24 Indigo;
      static CONST ColorBGR24 Ivory;
      static CONST ColorBGR24 Khaki;
      static CONST ColorBGR24 Lavender;
      static CONST ColorBGR24 LavenderBlush;
      static CONST ColorBGR24 LawnGreen;
      static CONST ColorBGR24 LemonChiffon;
      static CONST ColorBGR24 LightBlue;
      static CONST ColorBGR24 LightCoral;
      static CONST ColorBGR24 LightCyan;
      static CONST ColorBGR24 LightGoldenrodYellow;
      static CONST ColorBGR24 LightGreen;
      static CONST ColorBGR24 LightGray;
      static CONST ColorBGR24 LightPink;
      static CONST ColorBGR24 LightSalmon;
      static CONST ColorBGR24 LightSeaGreen;
      static CONST ColorBGR24 LightSkyBlue;
      static CONST ColorBGR24 LightSlateGray;
      static CONST ColorBGR24 LightSteelBlue;
      static CONST ColorBGR24 LightYellow;
      static CONST ColorBGR24 Lime;
      static CONST ColorBGR24 LimeGreen;
      static CONST ColorBGR24 Linen;
      static CONST ColorBGR24 Magenta;
      static CONST ColorBGR24 Maroon;
      static CONST ColorBGR24 MediumAquamarine;
      static CONST ColorBGR24 MediumBlue;
      static CONST ColorBGR24 MediumOrchid;
      static CONST ColorBGR24 MediumPurple;
      static CONST ColorBGR24 MediumSeaGreen;
      static CONST ColorBGR24 MediumSlateBlue;
      static CONST ColorBGR24 MediumSpringGreen;
      static CONST ColorBGR24 MediumTurquoise;
      static CONST ColorBGR24 MediumVioletRed;
      static CONST ColorBGR24 MidnightBlue;
      static CONST ColorBGR24 MintCream;
      static CONST ColorBGR24 MistyRose;
      static CONST ColorBGR24 Moccasin;
      static CONST ColorBGR24 NavajoWhite;
      static CONST ColorBGR24 Navy;
      static CONST ColorBGR24 OldLace;
      static CONST ColorBGR24 Olive;
      static CONST ColorBGR24 OliveDrab;
      static CONST ColorBGR24 Orange;
      static CONST ColorBGR24 OrangeRed;
      static CONST ColorBGR24 Orchid;
      static CONST ColorBGR24 PaleGoldenrod;
      static CONST ColorBGR24 PaleGreen;
      static CONST ColorBGR24 PaleTurquoise;
      static CONST ColorBGR24 PaleVioletRed;
      static CONST ColorBGR24 PapayaWhip;
      static CONST ColorBGR24 PeachPuff;
      static CONST ColorBGR24 Peru;
      static CONST ColorBGR24 Pink;
      static CONST ColorBGR24 Plum;
      static CONST ColorBGR24 PowderBlue;
      static CONST ColorBGR24 Purple;
      static CONST ColorBGR24 Red;
      static CONST ColorBGR24 RosyBrown;
      static CONST ColorBGR24 RoyalBlue;
      static CONST ColorBGR24 SaddleBrown;
      static CONST ColorBGR24 Salmon;
      static CONST ColorBGR24 SandyBrown;
      static CONST ColorBGR24 SeaGreen;
      static CONST ColorBGR24 SeaShell;
      static CONST ColorBGR24 Sienna;
      static CONST ColorBGR24 Silver;
      static CONST ColorBGR24 SkyBlue;
      static CONST ColorBGR24 SlateBlue;
      static CONST ColorBGR24 SlateGray;
      static CONST ColorBGR24 Snow;
      static CONST ColorBGR24 SpringGreen;
      static CONST ColorBGR24 SteelBlue;
      static CONST ColorBGR24 Tan;
      static CONST ColorBGR24 Teal;
      static CONST ColorBGR24 Thistle;
      static CONST ColorBGR24 Tomato;
      static CONST ColorBGR24 Turquoise;
      static CONST ColorBGR24 Violet;
      static CONST ColorBGR24 Wheat;
      static CONST ColorBGR24 White;
      static CONST ColorBGR24 WhiteSmoke;
      static CONST ColorBGR24 Yellow;
      static CONST ColorBGR24 YellowGreen;

    private:
      BYTE m_V[3];
    };

    class LIBRARY_API ColorR8
    {
    public:
      ColorR8();
      ColorR8(BYTE R);
      ColorR8(CONST ColorR8& C);

      BYTE& R()
      { return m_V[0]; }
      CONST BYTE& R() CONST
      { return m_V[0]; }

      VOID SetR(BYTE r)
      { m_V[0] = r; }

      BYTE GetR() CONST
      { return m_V[0]; }

#if defined(USE_DIRECTX9)
      operator D3DCOLOR() CONST;
      operator D3DCOLORVALUE() CONST;
      static CONST D3DFORMAT DirectXPixelFormat = D3DFMT_L8;
#endif

      static CONST DWORD PixelFormat = PixelFormat::R8;


      operator CONST BYTE* () CONST
      { return &m_V[0]; }
      operator BYTE* ()
      { return &m_V[0]; }

      VOID Clamp();
      VOID Clamp(BYTE Min, BYTE Max);

      ColorR8  operator +  (CONST ColorR8& C) CONST;
      ColorR8  operator -  (CONST ColorR8& C) CONST;
      ColorR8& operator += (CONST ColorR8& C);
      ColorR8& operator -= (CONST ColorR8& C);

      ColorR8  operator *  (FLOAT V) CONST;
      ColorR8  operator /  (FLOAT V) CONST;
      ColorR8& operator *= (FLOAT V);
      ColorR8& operator /= (FLOAT V);

      String ToString() CONST;

#if defined(USE_LUA)
      static Lua::Bind::Scope GenerateBindings(LPCTSTR Name);
#endif

      FLOAT Luminance() CONST;

      static CONST BYTE MaxChannelValue;
      static CONST UINT BitsPerChannel;
      static CONST UINT Channels;

    private:
      BYTE m_V[1];
    };

    class LIBRARY_API ColorR32F
    {
    public:
      ColorR32F();
      ColorR32F(FLOAT R);
      ColorR32F(CONST ColorR32F& C);

      FLOAT& R()
      { return m_V[0]; }
      CONST FLOAT& R() CONST
      { return m_V[0]; }

      VOID SetR(FLOAT r)
      { m_V[0] = r; }

      FLOAT GetR() CONST
      { return m_V[0]; }

#if defined(USE_DIRECTX9)
      operator D3DCOLOR() CONST;
      operator D3DCOLORVALUE() CONST;
      static CONST D3DFORMAT DirectXPixelFormat = D3DFMT_R32F;
#endif

      static CONST DWORD PixelFormat = PixelFormat::R32F;

      operator CONST FLOAT* () CONST
      { return &m_V[0]; }
      operator FLOAT* ()
      { return &m_V[0]; }

      VOID Clamp();
      VOID Clamp(FLOAT Min, FLOAT Max);

      ColorR32F  operator +  (CONST ColorR32F& C) CONST;
      ColorR32F  operator -  (CONST ColorR32F& C) CONST;
      ColorR32F& operator += (CONST ColorR32F& C);
      ColorR32F& operator -= (CONST ColorR32F& C);

      ColorR32F  operator *  (FLOAT V) CONST;
      ColorR32F  operator /  (FLOAT V) CONST;
      ColorR32F& operator *= (FLOAT V);
      ColorR32F& operator /= (FLOAT V);

      String ToString() CONST;

#if defined(USE_LUA)
      static Lua::Bind::Scope GenerateBindings(LPCTSTR Name);
#endif

      FLOAT Luminance() CONST;

      static ColorR32F RandomColor();

      static CONST FLOAT MaxChannelValue;
      static CONST UINT BitsPerChannel;
      static CONST UINT Channels;

    private:
      FLOAT m_V[1];
    };

    class LIBRARY_API ColorRGBA128F
    {
    public:
      ColorRGBA128F();
      ColorRGBA128F(FLOAT R, FLOAT G, FLOAT B, FLOAT A);
      ColorRGBA128F(FLOAT R, FLOAT G, FLOAT B);
      ColorRGBA128F(CONST ColorRGBA128F& C);

      FLOAT& R()
      { return m_V[0]; }
      CONST FLOAT& R() CONST
      { return m_V[0]; }
      FLOAT& G()
      { return m_V[1]; }
      CONST FLOAT& G() CONST
      { return m_V[1]; }
      FLOAT& B()
      { return m_V[2]; }
      CONST FLOAT& B() CONST
      { return m_V[2]; }
      FLOAT& A()
      { return m_V[3]; }
      CONST FLOAT& A() CONST
      { return m_V[3]; }

      VOID SetR(FLOAT r)
      { m_V[0] = r; }
      VOID SetG(FLOAT g)
      { m_V[1] = g; }
      VOID SetB(FLOAT b)
      { m_V[2] = b; }
      VOID SetA(FLOAT a)
      { m_V[3] = a; }

      FLOAT GetR() CONST
      { return m_V[0]; }
      FLOAT GetG() CONST
      { return m_V[1]; }
      FLOAT GetB() CONST
      { return m_V[2]; }
      FLOAT GetA() CONST
      { return m_V[3]; }

#if defined(USE_DIRECTX9)
      operator D3DCOLOR() CONST;
      operator D3DCOLORVALUE() CONST;
      static CONST D3DFORMAT DirectXPixelFormat = D3DFMT_A32B32G32R32F;
#endif
      static CONST DWORD PixelFormat = PixelFormat::R32FG32FB32FA32F;

      typedef FLOAT ColorChannelType;

      operator CONST FLOAT* () CONST
      { return &m_V[0]; }
      operator FLOAT* ()
      { return &m_V[0]; }

      VOID Clamp();
      VOID Clamp(FLOAT Min, FLOAT Max);

      ColorRGBA128F  operator +  (CONST ColorRGBA128F& C) CONST;
      ColorRGBA128F  operator -  (CONST ColorRGBA128F& C) CONST;
      ColorRGBA128F& operator += (CONST ColorRGBA128F& C);
      ColorRGBA128F& operator -= (CONST ColorRGBA128F& C);

      ColorRGBA128F& operator *= (CONST ColorRGBA128F& C);
      ColorRGBA128F& operator /= (CONST ColorRGBA128F& C);

      ColorRGBA128F  operator *  (FLOAT V) CONST;
      ColorRGBA128F  operator *  (CONST ColorRGBA128F& C) CONST;
      ColorRGBA128F  operator /  (FLOAT V) CONST;
      ColorRGBA128F& operator *= (FLOAT V);
      ColorRGBA128F& operator /= (FLOAT V);

      friend ColorRGBA128F operator* (FLOAT V, CONST ColorRGBA128F& C)
      { return C*V; }

      String ToString() CONST;

#if defined(USE_LUA)
      static Lua::Bind::Scope GenerateBindings(LPCTSTR Name);
#endif

      FLOAT Luminance() CONST;

      static ColorRGBA128F FromName(LPCTSTR String);
      static ColorRGBA128F FromHSV(FLOAT Hue, FLOAT Saturation, FLOAT Value);

      static ColorRGBA128F RandomColor(FLOAT Saturation = 1.0, FLOAT Value = 1.0);

      static CONST FLOAT MaxChannelValue;
      static CONST UINT BitsPerChannel;
      static CONST UINT Channels;

      static CONST ColorRGBA128F Transparent;
      static CONST ColorRGBA128F AliceBlue;
      static CONST ColorRGBA128F AntiqueWhite;
      static CONST ColorRGBA128F Aqua;
      static CONST ColorRGBA128F Aquamarine;
      static CONST ColorRGBA128F Azure;
      static CONST ColorRGBA128F Beige;
      static CONST ColorRGBA128F Bisque;
      static CONST ColorRGBA128F Black;
      static CONST ColorRGBA128F BlanchedAlmond;
      static CONST ColorRGBA128F Blue;
      static CONST ColorRGBA128F BlueViolet;
      static CONST ColorRGBA128F Brown;
      static CONST ColorRGBA128F BurlyWood;
      static CONST ColorRGBA128F CadetBlue;
      static CONST ColorRGBA128F Chartreuse;
      static CONST ColorRGBA128F Chocolate;
      static CONST ColorRGBA128F Coral;
      static CONST ColorRGBA128F CornflowerBlue;
      static CONST ColorRGBA128F Cornsilk;
      static CONST ColorRGBA128F Crimson;
      static CONST ColorRGBA128F Cyan;
      static CONST ColorRGBA128F DarkBlue;
      static CONST ColorRGBA128F DarkCyan;
      static CONST ColorRGBA128F DarkGoldenrod;
      static CONST ColorRGBA128F DarkGray;
      static CONST ColorRGBA128F DarkGreen;
      static CONST ColorRGBA128F DarkKhaki;
      static CONST ColorRGBA128F DarkMagenta;
      static CONST ColorRGBA128F DarkOliveGreen;
      static CONST ColorRGBA128F DarkOrange;
      static CONST ColorRGBA128F DarkOrchid;
      static CONST ColorRGBA128F DarkRed;
      static CONST ColorRGBA128F DarkSalmon;
      static CONST ColorRGBA128F DarkSeaGreen;
      static CONST ColorRGBA128F DarkSlateBlue;
      static CONST ColorRGBA128F DarkSlateGray;
      static CONST ColorRGBA128F DarkTurquoise;
      static CONST ColorRGBA128F DarkViolet;
      static CONST ColorRGBA128F DeepPink;
      static CONST ColorRGBA128F DeepSkyBlue;
      static CONST ColorRGBA128F DimGray;
      static CONST ColorRGBA128F DodgerBlue;
      static CONST ColorRGBA128F Firebrick;
      static CONST ColorRGBA128F FloralWhite;
      static CONST ColorRGBA128F ForestGreen;
      static CONST ColorRGBA128F Fuchsia;
      static CONST ColorRGBA128F Gainsboro;
      static CONST ColorRGBA128F GhostWhite;
      static CONST ColorRGBA128F Gold;
      static CONST ColorRGBA128F Goldenrod;
      static CONST ColorRGBA128F Gray;
      static CONST ColorRGBA128F Green;
      static CONST ColorRGBA128F GreenYellow;
      static CONST ColorRGBA128F Honeydew;
      static CONST ColorRGBA128F HotPink;
      static CONST ColorRGBA128F IndianRed;
      static CONST ColorRGBA128F Indigo;
      static CONST ColorRGBA128F Ivory;
      static CONST ColorRGBA128F Khaki;
      static CONST ColorRGBA128F Lavender;
      static CONST ColorRGBA128F LavenderBlush;
      static CONST ColorRGBA128F LawnGreen;
      static CONST ColorRGBA128F LemonChiffon;
      static CONST ColorRGBA128F LightBlue;
      static CONST ColorRGBA128F LightCoral;
      static CONST ColorRGBA128F LightCyan;
      static CONST ColorRGBA128F LightGoldenrodYellow;
      static CONST ColorRGBA128F LightGreen;
      static CONST ColorRGBA128F LightGray;
      static CONST ColorRGBA128F LightPink;
      static CONST ColorRGBA128F LightSalmon;
      static CONST ColorRGBA128F LightSeaGreen;
      static CONST ColorRGBA128F LightSkyBlue;
      static CONST ColorRGBA128F LightSlateGray;
      static CONST ColorRGBA128F LightSteelBlue;
      static CONST ColorRGBA128F LightYellow;
      static CONST ColorRGBA128F Lime;
      static CONST ColorRGBA128F LimeGreen;
      static CONST ColorRGBA128F Linen;
      static CONST ColorRGBA128F Magenta;
      static CONST ColorRGBA128F Maroon;
      static CONST ColorRGBA128F MediumAquamarine;
      static CONST ColorRGBA128F MediumBlue;
      static CONST ColorRGBA128F MediumOrchid;
      static CONST ColorRGBA128F MediumPurple;
      static CONST ColorRGBA128F MediumSeaGreen;
      static CONST ColorRGBA128F MediumSlateBlue;
      static CONST ColorRGBA128F MediumSpringGreen;
      static CONST ColorRGBA128F MediumTurquoise;
      static CONST ColorRGBA128F MediumVioletRed;
      static CONST ColorRGBA128F MidnightBlue;
      static CONST ColorRGBA128F MintCream;
      static CONST ColorRGBA128F MistyRose;
      static CONST ColorRGBA128F Moccasin;
      static CONST ColorRGBA128F NavajoWhite;
      static CONST ColorRGBA128F Navy;
      static CONST ColorRGBA128F OldLace;
      static CONST ColorRGBA128F Olive;
      static CONST ColorRGBA128F OliveDrab;
      static CONST ColorRGBA128F Orange;
      static CONST ColorRGBA128F OrangeRed;
      static CONST ColorRGBA128F Orchid;
      static CONST ColorRGBA128F PaleGoldenrod;
      static CONST ColorRGBA128F PaleGreen;
      static CONST ColorRGBA128F PaleTurquoise;
      static CONST ColorRGBA128F PaleVioletRed;
      static CONST ColorRGBA128F PapayaWhip;
      static CONST ColorRGBA128F PeachPuff;
      static CONST ColorRGBA128F Peru;
      static CONST ColorRGBA128F Pink;
      static CONST ColorRGBA128F Plum;
      static CONST ColorRGBA128F PowderBlue;
      static CONST ColorRGBA128F Purple;
      static CONST ColorRGBA128F Red;
      static CONST ColorRGBA128F RosyBrown;
      static CONST ColorRGBA128F RoyalBlue;
      static CONST ColorRGBA128F SaddleBrown;
      static CONST ColorRGBA128F Salmon;
      static CONST ColorRGBA128F SandyBrown;
      static CONST ColorRGBA128F SeaGreen;
      static CONST ColorRGBA128F SeaShell;
      static CONST ColorRGBA128F Sienna;
      static CONST ColorRGBA128F Silver;
      static CONST ColorRGBA128F SkyBlue;
      static CONST ColorRGBA128F SlateBlue;
      static CONST ColorRGBA128F SlateGray;
      static CONST ColorRGBA128F Snow;
      static CONST ColorRGBA128F SpringGreen;
      static CONST ColorRGBA128F SteelBlue;
      static CONST ColorRGBA128F Tan;
      static CONST ColorRGBA128F Teal;
      static CONST ColorRGBA128F Thistle;
      static CONST ColorRGBA128F Tomato;
      static CONST ColorRGBA128F Turquoise;
      static CONST ColorRGBA128F Violet;
      static CONST ColorRGBA128F Wheat;
      static CONST ColorRGBA128F White;
      static CONST ColorRGBA128F WhiteSmoke;
      static CONST ColorRGBA128F Yellow;
      static CONST ColorRGBA128F YellowGreen;

    private:
      FLOAT m_V[4];
    };

    class LIBRARY_API ColorRGB128F
    {
    public:
      ColorRGB128F();
      ColorRGB128F(FLOAT R, FLOAT G, FLOAT B);
      ColorRGB128F(CONST ColorRGB128F& C);

      FLOAT& R()
      { return m_V[0]; }
      CONST FLOAT& R() CONST
      { return m_V[0]; }
      FLOAT& G()
      { return m_V[1]; }
      CONST FLOAT& G() CONST
      { return m_V[1]; }
      FLOAT& B()
      { return m_V[2]; }
      CONST FLOAT& B() CONST
      { return m_V[2]; }

      VOID SetR(FLOAT r)
      { m_V[0] = r; }
      VOID SetG(FLOAT g)
      { m_V[1] = g; }
      VOID SetB(FLOAT b)
      { m_V[2] = b; }

      FLOAT GetR() CONST
      { return m_V[0]; }
      FLOAT GetG() CONST
      { return m_V[1]; }
      FLOAT GetB() CONST
      { return m_V[2]; }

#if defined(USE_DIRECTX9)
      operator D3DCOLOR() CONST;
      operator D3DCOLORVALUE() CONST;
#endif
      static CONST DWORD PixelFormat = PixelFormat::R32FG32FB32FX32;

      typedef FLOAT ColorChannelType;

      operator CONST FLOAT* () CONST
      { return &m_V[0]; }
      operator FLOAT* ()
      { return &m_V[0]; }

      VOID Clamp();
      VOID Clamp(FLOAT Min, FLOAT Max);

      ColorRGB128F  operator +  (CONST ColorRGB128F& C) CONST;
      ColorRGB128F  operator -  (CONST ColorRGB128F& C) CONST;
      ColorRGB128F& operator += (CONST ColorRGB128F& C);
      ColorRGB128F& operator -= (CONST ColorRGB128F& C);

      ColorRGB128F  operator *  (FLOAT V) CONST;
      ColorRGB128F  operator /  (FLOAT V) CONST;
      ColorRGB128F& operator *= (FLOAT V);
      ColorRGB128F& operator /= (FLOAT V);

      String ToString() CONST;

#if defined(USE_LUA)
      static Lua::Bind::Scope GenerateBindings(LPCTSTR Name);
#endif

      FLOAT Luminance() CONST;

      static ColorRGB128F FromName(LPCTSTR String);
      static ColorRGB128F FromHSV(FLOAT Hue, FLOAT Saturation, FLOAT Value);

      static ColorRGB128F RandomColor(FLOAT Saturation = 1.0, FLOAT Value = 1.0);

      static CONST BYTE MaxChannelValue;
      static CONST UINT BitsPerChannel;
      static CONST UINT Channels;

      static CONST ColorRGB128F AliceBlue;
      static CONST ColorRGB128F AntiqueWhite;
      static CONST ColorRGB128F Aqua;
      static CONST ColorRGB128F Aquamarine;
      static CONST ColorRGB128F Azure;
      static CONST ColorRGB128F Beige;
      static CONST ColorRGB128F Bisque;
      static CONST ColorRGB128F Black;
      static CONST ColorRGB128F BlanchedAlmond;
      static CONST ColorRGB128F Blue;
      static CONST ColorRGB128F BlueViolet;
      static CONST ColorRGB128F Brown;
      static CONST ColorRGB128F BurlyWood;
      static CONST ColorRGB128F CadetBlue;
      static CONST ColorRGB128F Chartreuse;
      static CONST ColorRGB128F Chocolate;
      static CONST ColorRGB128F Coral;
      static CONST ColorRGB128F CornflowerBlue;
      static CONST ColorRGB128F Cornsilk;
      static CONST ColorRGB128F Crimson;
      static CONST ColorRGB128F Cyan;
      static CONST ColorRGB128F DarkBlue;
      static CONST ColorRGB128F DarkCyan;
      static CONST ColorRGB128F DarkGoldenrod;
      static CONST ColorRGB128F DarkGray;
      static CONST ColorRGB128F DarkGreen;
      static CONST ColorRGB128F DarkKhaki;
      static CONST ColorRGB128F DarkMagenta;
      static CONST ColorRGB128F DarkOliveGreen;
      static CONST ColorRGB128F DarkOrange;
      static CONST ColorRGB128F DarkOrchid;
      static CONST ColorRGB128F DarkRed;
      static CONST ColorRGB128F DarkSalmon;
      static CONST ColorRGB128F DarkSeaGreen;
      static CONST ColorRGB128F DarkSlateBlue;
      static CONST ColorRGB128F DarkSlateGray;
      static CONST ColorRGB128F DarkTurquoise;
      static CONST ColorRGB128F DarkViolet;
      static CONST ColorRGB128F DeepPink;
      static CONST ColorRGB128F DeepSkyBlue;
      static CONST ColorRGB128F DimGray;
      static CONST ColorRGB128F DodgerBlue;
      static CONST ColorRGB128F Firebrick;
      static CONST ColorRGB128F FloralWhite;
      static CONST ColorRGB128F ForestGreen;
      static CONST ColorRGB128F Fuchsia;
      static CONST ColorRGB128F Gainsboro;
      static CONST ColorRGB128F GhostWhite;
      static CONST ColorRGB128F Gold;
      static CONST ColorRGB128F Goldenrod;
      static CONST ColorRGB128F Gray;
      static CONST ColorRGB128F Green;
      static CONST ColorRGB128F GreenYellow;
      static CONST ColorRGB128F Honeydew;
      static CONST ColorRGB128F HotPink;
      static CONST ColorRGB128F IndianRed;
      static CONST ColorRGB128F Indigo;
      static CONST ColorRGB128F Ivory;
      static CONST ColorRGB128F Khaki;
      static CONST ColorRGB128F Lavender;
      static CONST ColorRGB128F LavenderBlush;
      static CONST ColorRGB128F LawnGreen;
      static CONST ColorRGB128F LemonChiffon;
      static CONST ColorRGB128F LightBlue;
      static CONST ColorRGB128F LightCoral;
      static CONST ColorRGB128F LightCyan;
      static CONST ColorRGB128F LightGoldenrodYellow;
      static CONST ColorRGB128F LightGreen;
      static CONST ColorRGB128F LightGray;
      static CONST ColorRGB128F LightPink;
      static CONST ColorRGB128F LightSalmon;
      static CONST ColorRGB128F LightSeaGreen;
      static CONST ColorRGB128F LightSkyBlue;
      static CONST ColorRGB128F LightSlateGray;
      static CONST ColorRGB128F LightSteelBlue;
      static CONST ColorRGB128F LightYellow;
      static CONST ColorRGB128F Lime;
      static CONST ColorRGB128F LimeGreen;
      static CONST ColorRGB128F Linen;
      static CONST ColorRGB128F Magenta;
      static CONST ColorRGB128F Maroon;
      static CONST ColorRGB128F MediumAquamarine;
      static CONST ColorRGB128F MediumBlue;
      static CONST ColorRGB128F MediumOrchid;
      static CONST ColorRGB128F MediumPurple;
      static CONST ColorRGB128F MediumSeaGreen;
      static CONST ColorRGB128F MediumSlateBlue;
      static CONST ColorRGB128F MediumSpringGreen;
      static CONST ColorRGB128F MediumTurquoise;
      static CONST ColorRGB128F MediumVioletRed;
      static CONST ColorRGB128F MidnightBlue;
      static CONST ColorRGB128F MintCream;
      static CONST ColorRGB128F MistyRose;
      static CONST ColorRGB128F Moccasin;
      static CONST ColorRGB128F NavajoWhite;
      static CONST ColorRGB128F Navy;
      static CONST ColorRGB128F OldLace;
      static CONST ColorRGB128F Olive;
      static CONST ColorRGB128F OliveDrab;
      static CONST ColorRGB128F Orange;
      static CONST ColorRGB128F OrangeRed;
      static CONST ColorRGB128F Orchid;
      static CONST ColorRGB128F PaleGoldenrod;
      static CONST ColorRGB128F PaleGreen;
      static CONST ColorRGB128F PaleTurquoise;
      static CONST ColorRGB128F PaleVioletRed;
      static CONST ColorRGB128F PapayaWhip;
      static CONST ColorRGB128F PeachPuff;
      static CONST ColorRGB128F Peru;
      static CONST ColorRGB128F Pink;
      static CONST ColorRGB128F Plum;
      static CONST ColorRGB128F PowderBlue;
      static CONST ColorRGB128F Purple;
      static CONST ColorRGB128F Red;
      static CONST ColorRGB128F RosyBrown;
      static CONST ColorRGB128F RoyalBlue;
      static CONST ColorRGB128F SaddleBrown;
      static CONST ColorRGB128F Salmon;
      static CONST ColorRGB128F SandyBrown;
      static CONST ColorRGB128F SeaGreen;
      static CONST ColorRGB128F SeaShell;
      static CONST ColorRGB128F Sienna;
      static CONST ColorRGB128F Silver;
      static CONST ColorRGB128F SkyBlue;
      static CONST ColorRGB128F SlateBlue;
      static CONST ColorRGB128F SlateGray;
      static CONST ColorRGB128F Snow;
      static CONST ColorRGB128F SpringGreen;
      static CONST ColorRGB128F SteelBlue;
      static CONST ColorRGB128F Tan;
      static CONST ColorRGB128F Teal;
      static CONST ColorRGB128F Thistle;
      static CONST ColorRGB128F Tomato;
      static CONST ColorRGB128F Turquoise;
      static CONST ColorRGB128F Violet;
      static CONST ColorRGB128F Wheat;
      static CONST ColorRGB128F White;
      static CONST ColorRGB128F WhiteSmoke;
      static CONST ColorRGB128F Yellow;
      static CONST ColorRGB128F YellowGreen;

    private:
      FLOAT m_V[4];
    };

    class LIBRARY_API ColorRGB96F
    {
    public:
      ColorRGB96F();
      ColorRGB96F(FLOAT R, FLOAT G, FLOAT B);
      ColorRGB96F(CONST ColorRGB96F& C);

      FLOAT& R()
      { return m_V[0]; }
      CONST FLOAT& R() CONST
      { return m_V[0]; }
      FLOAT& G()
      { return m_V[1]; }
      CONST FLOAT& G() CONST
      { return m_V[1]; }
      FLOAT& B()
      { return m_V[2]; }
      CONST FLOAT& B() CONST
      { return m_V[2]; }

      VOID SetR(FLOAT r)
      { m_V[0] = r; }
      VOID SetG(FLOAT g)
      { m_V[1] = g; }
      VOID SetB(FLOAT b)
      { m_V[2] = b; }

      FLOAT GetR() CONST
      { return m_V[0]; }
      FLOAT GetG() CONST
      { return m_V[1]; }
      FLOAT GetB() CONST
      { return m_V[2]; }

#if defined(USE_DIRECTX9)
      operator D3DCOLOR() CONST;
      operator D3DCOLORVALUE() CONST;
#endif
      static CONST DWORD PixelFormat = PixelFormat::R32FG32FB32FX32;

      typedef FLOAT ColorChannelType;

      operator CONST FLOAT* () CONST
      { return &m_V[0]; }
      operator FLOAT* ()
      { return &m_V[0]; }

      VOID Clamp();
      VOID Clamp(FLOAT Min, FLOAT Max);

      ColorRGB96F  operator +  (CONST ColorRGB96F& C) CONST;
      ColorRGB96F  operator -  (CONST ColorRGB96F& C) CONST;
      ColorRGB96F& operator += (CONST ColorRGB96F& C);
      ColorRGB96F& operator -= (CONST ColorRGB96F& C);

      ColorRGB96F  operator *  (FLOAT V) CONST;
      ColorRGB96F  operator /  (FLOAT V) CONST;
      ColorRGB96F& operator *= (FLOAT V);
      ColorRGB96F& operator /= (FLOAT V);

      String ToString() CONST;

#if defined(USE_LUA)
      static Lua::Bind::Scope GenerateBindings(LPCTSTR Name);
#endif

      FLOAT Luminance() CONST;

      static ColorRGB96F FromName(LPCTSTR String);
      static ColorRGB96F FromHSV(FLOAT Hue, FLOAT Saturation, FLOAT Value);

      static ColorRGB96F RandomColor(FLOAT Saturation = 1.0, FLOAT Value = 1.0);

      static CONST BYTE MaxChannelValue;
      static CONST UINT BitsPerChannel;
      static CONST UINT Channels;

      static CONST ColorRGB96F AliceBlue;
      static CONST ColorRGB96F AntiqueWhite;
      static CONST ColorRGB96F Aqua;
      static CONST ColorRGB96F Aquamarine;
      static CONST ColorRGB96F Azure;
      static CONST ColorRGB96F Beige;
      static CONST ColorRGB96F Bisque;
      static CONST ColorRGB96F Black;
      static CONST ColorRGB96F BlanchedAlmond;
      static CONST ColorRGB96F Blue;
      static CONST ColorRGB96F BlueViolet;
      static CONST ColorRGB96F Brown;
      static CONST ColorRGB96F BurlyWood;
      static CONST ColorRGB96F CadetBlue;
      static CONST ColorRGB96F Chartreuse;
      static CONST ColorRGB96F Chocolate;
      static CONST ColorRGB96F Coral;
      static CONST ColorRGB96F CornflowerBlue;
      static CONST ColorRGB96F Cornsilk;
      static CONST ColorRGB96F Crimson;
      static CONST ColorRGB96F Cyan;
      static CONST ColorRGB96F DarkBlue;
      static CONST ColorRGB96F DarkCyan;
      static CONST ColorRGB96F DarkGoldenrod;
      static CONST ColorRGB96F DarkGray;
      static CONST ColorRGB96F DarkGreen;
      static CONST ColorRGB96F DarkKhaki;
      static CONST ColorRGB96F DarkMagenta;
      static CONST ColorRGB96F DarkOliveGreen;
      static CONST ColorRGB96F DarkOrange;
      static CONST ColorRGB96F DarkOrchid;
      static CONST ColorRGB96F DarkRed;
      static CONST ColorRGB96F DarkSalmon;
      static CONST ColorRGB96F DarkSeaGreen;
      static CONST ColorRGB96F DarkSlateBlue;
      static CONST ColorRGB96F DarkSlateGray;
      static CONST ColorRGB96F DarkTurquoise;
      static CONST ColorRGB96F DarkViolet;
      static CONST ColorRGB96F DeepPink;
      static CONST ColorRGB96F DeepSkyBlue;
      static CONST ColorRGB96F DimGray;
      static CONST ColorRGB96F DodgerBlue;
      static CONST ColorRGB96F Firebrick;
      static CONST ColorRGB96F FloralWhite;
      static CONST ColorRGB96F ForestGreen;
      static CONST ColorRGB96F Fuchsia;
      static CONST ColorRGB96F Gainsboro;
      static CONST ColorRGB96F GhostWhite;
      static CONST ColorRGB96F Gold;
      static CONST ColorRGB96F Goldenrod;
      static CONST ColorRGB96F Gray;
      static CONST ColorRGB96F Green;
      static CONST ColorRGB96F GreenYellow;
      static CONST ColorRGB96F Honeydew;
      static CONST ColorRGB96F HotPink;
      static CONST ColorRGB96F IndianRed;
      static CONST ColorRGB96F Indigo;
      static CONST ColorRGB96F Ivory;
      static CONST ColorRGB96F Khaki;
      static CONST ColorRGB96F Lavender;
      static CONST ColorRGB96F LavenderBlush;
      static CONST ColorRGB96F LawnGreen;
      static CONST ColorRGB96F LemonChiffon;
      static CONST ColorRGB96F LightBlue;
      static CONST ColorRGB96F LightCoral;
      static CONST ColorRGB96F LightCyan;
      static CONST ColorRGB96F LightGoldenrodYellow;
      static CONST ColorRGB96F LightGreen;
      static CONST ColorRGB96F LightGray;
      static CONST ColorRGB96F LightPink;
      static CONST ColorRGB96F LightSalmon;
      static CONST ColorRGB96F LightSeaGreen;
      static CONST ColorRGB96F LightSkyBlue;
      static CONST ColorRGB96F LightSlateGray;
      static CONST ColorRGB96F LightSteelBlue;
      static CONST ColorRGB96F LightYellow;
      static CONST ColorRGB96F Lime;
      static CONST ColorRGB96F LimeGreen;
      static CONST ColorRGB96F Linen;
      static CONST ColorRGB96F Magenta;
      static CONST ColorRGB96F Maroon;
      static CONST ColorRGB96F MediumAquamarine;
      static CONST ColorRGB96F MediumBlue;
      static CONST ColorRGB96F MediumOrchid;
      static CONST ColorRGB96F MediumPurple;
      static CONST ColorRGB96F MediumSeaGreen;
      static CONST ColorRGB96F MediumSlateBlue;
      static CONST ColorRGB96F MediumSpringGreen;
      static CONST ColorRGB96F MediumTurquoise;
      static CONST ColorRGB96F MediumVioletRed;
      static CONST ColorRGB96F MidnightBlue;
      static CONST ColorRGB96F MintCream;
      static CONST ColorRGB96F MistyRose;
      static CONST ColorRGB96F Moccasin;
      static CONST ColorRGB96F NavajoWhite;
      static CONST ColorRGB96F Navy;
      static CONST ColorRGB96F OldLace;
      static CONST ColorRGB96F Olive;
      static CONST ColorRGB96F OliveDrab;
      static CONST ColorRGB96F Orange;
      static CONST ColorRGB96F OrangeRed;
      static CONST ColorRGB96F Orchid;
      static CONST ColorRGB96F PaleGoldenrod;
      static CONST ColorRGB96F PaleGreen;
      static CONST ColorRGB96F PaleTurquoise;
      static CONST ColorRGB96F PaleVioletRed;
      static CONST ColorRGB96F PapayaWhip;
      static CONST ColorRGB96F PeachPuff;
      static CONST ColorRGB96F Peru;
      static CONST ColorRGB96F Pink;
      static CONST ColorRGB96F Plum;
      static CONST ColorRGB96F PowderBlue;
      static CONST ColorRGB96F Purple;
      static CONST ColorRGB96F Red;
      static CONST ColorRGB96F RosyBrown;
      static CONST ColorRGB96F RoyalBlue;
      static CONST ColorRGB96F SaddleBrown;
      static CONST ColorRGB96F Salmon;
      static CONST ColorRGB96F SandyBrown;
      static CONST ColorRGB96F SeaGreen;
      static CONST ColorRGB96F SeaShell;
      static CONST ColorRGB96F Sienna;
      static CONST ColorRGB96F Silver;
      static CONST ColorRGB96F SkyBlue;
      static CONST ColorRGB96F SlateBlue;
      static CONST ColorRGB96F SlateGray;
      static CONST ColorRGB96F Snow;
      static CONST ColorRGB96F SpringGreen;
      static CONST ColorRGB96F SteelBlue;
      static CONST ColorRGB96F Tan;
      static CONST ColorRGB96F Teal;
      static CONST ColorRGB96F Thistle;
      static CONST ColorRGB96F Tomato;
      static CONST ColorRGB96F Turquoise;
      static CONST ColorRGB96F Violet;
      static CONST ColorRGB96F Wheat;
      static CONST ColorRGB96F White;
      static CONST ColorRGB96F WhiteSmoke;
      static CONST ColorRGB96F Yellow;
      static CONST ColorRGB96F YellowGreen;

    private:
      FLOAT m_V[3];
    };

    typedef ColorRGBA128F ColorF;
    typedef ColorBGRA32 Color;

  }

  template<>
  inline Graphics::ColorRGBA128F Min(CONST Graphics::ColorRGBA128F& A, CONST Graphics::ColorRGBA128F& B)
  { return Graphics::ColorRGBA128F(Min(A[0],B[0]), Min(A[1],B[1]), Min(A[2],B[2]), Min(A[3],B[3])); }
  template<>
  inline Graphics::ColorRGBA128F Max(CONST Graphics::ColorRGBA128F& A, CONST Graphics::ColorRGBA128F& B)
  { return Graphics::ColorRGBA128F(Max(A[0],B[0]), Max(A[1],B[1]), Max(A[2],B[2]), Max(A[3],B[3])); }

}

