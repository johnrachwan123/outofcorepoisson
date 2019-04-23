/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/Color.cpp $
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

#include "Graphics/Color.hpp"
#include "Math/Math.hpp"

using namespace Bolitho;
using namespace Bolitho::Graphics;
using namespace Bolitho::Math;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_LUA)
Lua::Bind::Scope ColorChannel::GenerateBindings(LPCTSTR Name)
{
  using namespace Lua::Bind;

  return Class<Bolitho::Graphics::ColorChannel>(Name)
    .Enum("ColorChannel")
    [
      Value("R", ColorChannel::R),
      Value("RED", ColorChannel::RED),
      Value("G", ColorChannel::G),
      Value("GREEN", ColorChannel::GREEN),
      Value("B", ColorChannel::B),
      Value("BLUE", ColorChannel::BLUE),
      Value("A", ColorChannel::A),
      Value("ALPHA", ColorChannel::ALPHA)
    ];
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_LUA)
Lua::Bind::Scope PixelFormat::GenerateBindings(LPCTSTR Name)
{
  using namespace Lua::Bind;

  return Class<Bolitho::Graphics::PixelFormat>(Name)
    .Enum("PixelFormat")
    [
      Value("UNKNOWN", PixelFormat::UNKNOWN),
      Value("RGBA32", PixelFormat::R8G8B8A8),
      Value("RGB32", PixelFormat::R8G8B8X8),
      Value("RGB24", PixelFormat::R8G8B8),
      Value("BGRA32", PixelFormat::B8G8R8A8),
      Value("BGR32", PixelFormat::B8G8R8X8),
      Value("BGR24", PixelFormat::B8G8R8),
      Value("RGBA128F", PixelFormat::R32FG32FB32FA32F),
      Value("RGB128F", PixelFormat::R32FG32FB32FX32),
      Value("RGB96F", PixelFormat::R32FG32FB32F),
      Value("RGBA128", PixelFormat::R32G32B32A32),
      Value("RGB128", PixelFormat::R32G32B32X32),
      Value("RGB96", PixelFormat::R32G32B32),
      Value("R8", PixelFormat::R8),
      Value("R32F", PixelFormat::R32F),
      Value("R32", PixelFormat::R32)
    ];
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA32::ColorRGBA32()
{
  m_V[0] = 0;
  m_V[1] = 0;
  m_V[2] = 0;
  m_V[3] = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA32::ColorRGBA32(BYTE R, BYTE G, BYTE B, BYTE A)
{
  m_V[0] = R;
  m_V[1] = G;
  m_V[2] = B;
  m_V[3] = A;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA32::ColorRGBA32(BYTE R, BYTE G, BYTE B)
{
  m_V[0] = R;
  m_V[1] = G;
  m_V[2] = B;
  m_V[3] = 255;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA32::ColorRGBA32(CONST ColorRGBA32& C)
{
  m_V[0] = C.m_V[0];
  m_V[1] = C.m_V[1];
  m_V[2] = C.m_V[2];
  m_V[3] = C.m_V[3];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_DIRECTX9)
ColorRGBA32::operator D3DCOLOR() CONST
{
  return D3DCOLOR_RGBA(m_V[0], m_V[1], m_V[2], m_V[3]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA32::operator D3DCOLORVALUE() CONST
{
  D3DCOLORVALUE V;
  V.r = m_V[0] / 255.0f;
  V.g = m_V[1] / 255.0f;
  V.b = m_V[2] / 255.0f;
  V.a = m_V[3] / 255.0f;

  return V;
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ColorRGBA32::Clamp()
{
  Clamp(0, 255);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ColorRGBA32::Clamp(BYTE Min, BYTE Max)
{
  m_V[0] = (m_V[0] > Max) ? Max : m_V[0];
  m_V[1] = (m_V[1] > Max) ? Max : m_V[1];
  m_V[2] = (m_V[2] > Max) ? Max : m_V[2];
  m_V[3] = (m_V[3] > Max) ? Max : m_V[3];

  m_V[0] = (m_V[0] < Min) ? Min : m_V[0];
  m_V[1] = (m_V[1] < Min) ? Min : m_V[1];
  m_V[2] = (m_V[2] < Min) ? Min : m_V[2];
  m_V[3] = (m_V[3] < Min) ? Min : m_V[3];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA32 ColorRGBA32::operator + (CONST ColorRGBA32& C) CONST
{
  return ColorRGBA32(m_V[0] + C.m_V[0], m_V[1] + C.m_V[1], m_V[2] + C.m_V[2], m_V[3] + C.m_V[3]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA32 ColorRGBA32::operator - (CONST ColorRGBA32& C) CONST
{
  return ColorRGBA32(m_V[0] - C.m_V[0], m_V[1] - C.m_V[1], m_V[2] - C.m_V[2], m_V[3] - C.m_V[3]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA32& ColorRGBA32::operator += (CONST ColorRGBA32& C)
{
  m_V[0] += C.m_V[0];
  m_V[1] += C.m_V[1];
  m_V[2] += C.m_V[2];
  m_V[3] += C.m_V[3];
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA32& ColorRGBA32::operator -= (CONST ColorRGBA32& C)
{
  m_V[0] += C.m_V[0];
  m_V[1] += C.m_V[1];
  m_V[2] += C.m_V[2];
  m_V[3] += C.m_V[3];
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA32 ColorRGBA32::operator * (FLOAT V) CONST
{
  return ColorRGBA32((BYTE)(m_V[0]*V), (BYTE)(m_V[1]*V), (BYTE)(m_V[2]*V), (BYTE)(m_V[3]*V));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA32 ColorRGBA32::operator / (FLOAT V) CONST
{
  return ColorRGBA32((BYTE)(m_V[0]/V), (BYTE)(m_V[1]/V), (BYTE)(m_V[2]/V), (BYTE)(m_V[3]/V));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA32& ColorRGBA32::operator *= (FLOAT V)
{
  m_V[0] = (BYTE)(m_V[0]*V);
  m_V[1] = (BYTE)(m_V[1]*V);
  m_V[2] = (BYTE)(m_V[2]*V);
  m_V[3] = (BYTE)(m_V[3]*V);
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA32& ColorRGBA32::operator /= (FLOAT V)
{
  m_V[0] = (BYTE)(m_V[0]/V);
  m_V[1] = (BYTE)(m_V[1]/V);
  m_V[2] = (BYTE)(m_V[2]/V);
  m_V[3] = (BYTE)(m_V[3]/V);
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT ColorRGBA32::Luminance() CONST
{
  return 0.299f*m_V[0] + 0.587f*m_V[1] + 0.114f*m_V[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA32 ColorRGBA32::FromHSV(FLOAT Hue, FLOAT Saturation, FLOAT Value)
{
	if (Saturation <= 0.0f)
		return ColorRGBA32((BYTE)(Value*255),(BYTE)(Value*255),(BYTE)(Value*255));
  Hue = Math::Mod(Hue, 2.0f*Math::PI);
	if (Hue < 0.0)
    Hue += 2.0f*Math::PI;
	
	Hue /= Math::PI / 3.0f;
	
  int i = int(Math::Floor(Hue));
	
	FLOAT f = Hue - i;
	FLOAT p = Value * (1.0f - Saturation);
	FLOAT q = Value * (1.0f - (Saturation*f));
	FLOAT t = Value * (1.0f - (Saturation*(1.0f-f)));
	
	switch(i) 
	{
	case 0: 
		return ColorRGBA32((BYTE)(Value*255), (BYTE)(t*255), (BYTE)(p*255));
	case 1: 
		return ColorRGBA32((BYTE)(q*255), (BYTE)(Value*255), (BYTE)(p*255));
	case 2: 
		return ColorRGBA32((BYTE)(p*255), (BYTE)(Value*255), (BYTE)(t*255));
	case 3:
		return ColorRGBA32((BYTE)(p*255), (BYTE)(q*255), (BYTE)(Value*255));
	case 4:
		return ColorRGBA32((BYTE)(t*255), (BYTE)(p*255), (BYTE)(Value*255));
	default: 
		return ColorRGBA32((BYTE)(Value*255), (BYTE)(p*255), (BYTE)(q*255));
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA32 ColorRGBA32::RandomColor(FLOAT Saturation, FLOAT Value)
{
  FLOAT Hue = UniformRandom<FLOAT>() * 2.0f * PI;
	return FromHSV(Hue, Saturation, Value);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA32 ColorRGBA32::FromName(LPCTSTR String)
{
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Transparent")) == 0)
		return ColorRGBA32::Transparent;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("AliceBlue")) == 0)
		return ColorRGBA32::AliceBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("AntiqueWhite")) == 0)
		return ColorRGBA32::AntiqueWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Aqua")) == 0)
		return ColorRGBA32::Aqua;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Aquamarine")) == 0)
		return ColorRGBA32::Aquamarine;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Azure")) == 0)
		return ColorRGBA32::Azure;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Beige")) == 0)
		return ColorRGBA32::Beige;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Bisque")) == 0)
		return ColorRGBA32::Bisque;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Black")) == 0)
		return ColorRGBA32::Black;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BlanchedAlmond")) == 0)
		return ColorRGBA32::BlanchedAlmond;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Blue")) == 0)
		return ColorRGBA32::Blue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BlueViolet")) == 0)
		return ColorRGBA32::BlueViolet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Brown")) == 0)
		return ColorRGBA32::Brown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BurlyWood")) == 0)
		return ColorRGBA32::BurlyWood;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("CadetBlue")) == 0)
		return ColorRGBA32::CadetBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Chartreuse")) == 0)
		return ColorRGBA32::Chartreuse;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Chocolate")) == 0)
		return ColorRGBA32::Chocolate;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Coral")) == 0)
		return ColorRGBA32::Coral;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("CornflowerBlue")) == 0)
		return ColorRGBA32::CornflowerBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Cornsilk")) == 0)
		return ColorRGBA32::Cornsilk;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Crimson")) == 0)
		return ColorRGBA32::Crimson;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Cyan")) == 0)
		return ColorRGBA32::Cyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkBlue")) == 0)
		return ColorRGBA32::DarkBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkCyan")) == 0)
		return ColorRGBA32::DarkCyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGoldenrod")) == 0)
		return ColorRGBA32::DarkGoldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGray")) == 0)
		return ColorRGBA32::DarkGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGreen")) == 0)
		return ColorRGBA32::DarkGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkKhaki")) == 0)
		return ColorRGBA32::DarkKhaki;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkMagenta")) == 0)
		return ColorRGBA32::DarkMagenta;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOliveGreen")) == 0)
		return ColorRGBA32::DarkOliveGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOrange")) == 0)
		return ColorRGBA32::DarkOrange;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOrchid")) == 0)
		return ColorRGBA32::DarkOrchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkRed")) == 0)
		return ColorRGBA32::DarkRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSalmon")) == 0)
		return ColorRGBA32::DarkSalmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSeaGreen")) == 0)
		return ColorRGBA32::DarkSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSlateBlue")) == 0)
		return ColorRGBA32::DarkSlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSlateGray")) == 0)
		return ColorRGBA32::DarkSlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkTurquoise")) == 0)
		return ColorRGBA32::DarkTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkViolet")) == 0)
		return ColorRGBA32::DarkViolet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DeepPink")) == 0)
		return ColorRGBA32::DeepPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DeepSkyBlue")) == 0)
		return ColorRGBA32::DeepSkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DimGray")) == 0)
		return ColorRGBA32::DimGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DodgerBlue")) == 0)
		return ColorRGBA32::DodgerBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Firebrick")) == 0)
		return ColorRGBA32::Firebrick;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("FloralWhite")) == 0)
		return ColorRGBA32::FloralWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("ForestGreen")) == 0)
		return ColorRGBA32::ForestGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Fuchsia")) == 0)
		return ColorRGBA32::Fuchsia;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gainsboro")) == 0)
		return ColorRGBA32::Gainsboro;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("GhostWhite")) == 0)
		return ColorRGBA32::GhostWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gold")) == 0)
		return ColorRGBA32::Gold;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Goldenrod")) == 0)
		return ColorRGBA32::Goldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gray")) == 0)
		return ColorRGBA32::Gray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Green")) == 0)
		return ColorRGBA32::Green;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("GreenYellow")) == 0)
		return ColorRGBA32::GreenYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Honeydew")) == 0)
		return ColorRGBA32::Honeydew;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("HotPink")) == 0)
		return ColorRGBA32::HotPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("IndianRed")) == 0)
		return ColorRGBA32::IndianRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Indigo")) == 0)
		return ColorRGBA32::Indigo;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Ivory")) == 0)
		return ColorRGBA32::Ivory;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Khaki")) == 0)
		return ColorRGBA32::Khaki;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Lavender")) == 0)
		return ColorRGBA32::Lavender;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LavenderBlush")) == 0)
		return ColorRGBA32::LavenderBlush;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LawnGreen")) == 0)
		return ColorRGBA32::LawnGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LemonChiffon")) == 0)
		return ColorRGBA32::LemonChiffon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightBlue")) == 0)
		return ColorRGBA32::LightBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightCoral")) == 0)
		return ColorRGBA32::LightCoral;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightCyan")) == 0)
		return ColorRGBA32::LightCyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGoldenrodYellow")) == 0)
		return ColorRGBA32::LightGoldenrodYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGreen")) == 0)
		return ColorRGBA32::LightGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGray")) == 0)
		return ColorRGBA32::LightGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightPink")) == 0)
		return ColorRGBA32::LightPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSalmon")) == 0)
		return ColorRGBA32::LightSalmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSeaGreen")) == 0)
		return ColorRGBA32::LightSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSkyBlue")) == 0)
		return ColorRGBA32::LightSkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSlateGray")) == 0)
		return ColorRGBA32::LightSlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSteelBlue")) == 0)
		return ColorRGBA32::LightSteelBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightYellow")) == 0)
		return ColorRGBA32::LightYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Lime")) == 0)
		return ColorRGBA32::Lime;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LimeGreen")) == 0)
		return ColorRGBA32::LimeGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Linen")) == 0)
		return ColorRGBA32::Linen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Magenta")) == 0)
		return ColorRGBA32::Magenta;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Maroon")) == 0)
		return ColorRGBA32::Maroon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumAquamarine")) == 0)
		return ColorRGBA32::MediumAquamarine;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumBlue")) == 0)
		return ColorRGBA32::MediumBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumOrchid")) == 0)
		return ColorRGBA32::MediumOrchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumPurple")) == 0)
		return ColorRGBA32::MediumPurple;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSeaGreen")) == 0)
		return ColorRGBA32::MediumSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSlateBlue")) == 0)
		return ColorRGBA32::MediumSlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSpringGreen")) == 0)
		return ColorRGBA32::MediumSpringGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumTurquoise")) == 0)
		return ColorRGBA32::MediumTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumVioletRed")) == 0)
		return ColorRGBA32::MediumVioletRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MidnightBlue")) == 0)
		return ColorRGBA32::MidnightBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MintCream")) == 0)
		return ColorRGBA32::MintCream;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MistyRose")) == 0)
		return ColorRGBA32::MistyRose;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Moccasin")) == 0)
		return ColorRGBA32::Moccasin;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("NavajoWhite")) == 0)
		return ColorRGBA32::NavajoWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Navy")) == 0)
		return ColorRGBA32::Navy;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OldLace")) == 0)
		return ColorRGBA32::OldLace;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Olive")) == 0)
		return ColorRGBA32::Olive;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OliveDrab")) == 0)
		return ColorRGBA32::OliveDrab;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Orange")) == 0)
		return ColorRGBA32::Orange;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OrangeRed")) == 0)
		return ColorRGBA32::OrangeRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Orchid")) == 0)
		return ColorRGBA32::Orchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleGoldenrod")) == 0)
		return ColorRGBA32::PaleGoldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleGreen")) == 0)
		return ColorRGBA32::PaleGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleTurquoise")) == 0)
		return ColorRGBA32::PaleTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleVioletRed")) == 0)
		return ColorRGBA32::PaleVioletRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PapayaWhip")) == 0)
		return ColorRGBA32::PapayaWhip;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PeachPuff")) == 0)
		return ColorRGBA32::PeachPuff;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Peru")) == 0)
		return ColorRGBA32::Peru;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Pink")) == 0)
		return ColorRGBA32::Pink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Plum")) == 0)
		return ColorRGBA32::Plum;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PowderBlue")) == 0)
		return ColorRGBA32::PowderBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Purple")) == 0)
		return ColorRGBA32::Purple;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Red")) == 0)
		return ColorRGBA32::Red;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("RosyBrown")) == 0)
		return ColorRGBA32::RosyBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("RoyalBlue")) == 0)
		return ColorRGBA32::RoyalBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SaddleBrown")) == 0)
		return ColorRGBA32::SaddleBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Salmon")) == 0)
		return ColorRGBA32::Salmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SandyBrown")) == 0)
		return ColorRGBA32::SandyBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SeaGreen")) == 0)
		return ColorRGBA32::SeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SeaShell")) == 0)
		return ColorRGBA32::SeaShell;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Sienna")) == 0)
		return ColorRGBA32::Sienna;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Silver")) == 0)
		return ColorRGBA32::Silver;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SkyBlue")) == 0)
		return ColorRGBA32::SkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SlateBlue")) == 0)
		return ColorRGBA32::SlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SlateGray")) == 0)
		return ColorRGBA32::SlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Snow")) == 0)
		return ColorRGBA32::Snow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SpringGreen")) == 0)
		return ColorRGBA32::SpringGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SteelBlue")) == 0)
		return ColorRGBA32::SteelBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Tan")) == 0)
		return ColorRGBA32::Tan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Teal")) == 0)
		return ColorRGBA32::Teal;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Thistle")) == 0)
		return ColorRGBA32::Thistle;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Tomato")) == 0)
		return ColorRGBA32::Tomato;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Turquoise")) == 0)
		return ColorRGBA32::Turquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Violet")) == 0)
		return ColorRGBA32::Violet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Wheat")) == 0)
		return ColorRGBA32::Wheat;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("White")) == 0)
		return ColorRGBA32::White;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("WhiteSmoke")) == 0)
		return ColorRGBA32::WhiteSmoke;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Yellow")) == 0)
		return ColorRGBA32::Yellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("YellowGreen")) == 0)
		return ColorRGBA32::YellowGreen;

  return ColorRGBA32::Black;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String ColorRGBA32::ToString() CONST
{
  return String::Format(TEXT("(%d,%d,%d,%d)"), m_V[0], m_V[1], m_V[2], m_V[3]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_LUA)
Lua::Bind::Scope ColorRGBA32::GenerateBindings(LPCTSTR Name)
{
  using namespace Lua::Bind;

  return 
    Class<Graphics::ColorRGBA32>(Name)
      .Def(Constructor<>())
      .Def(Constructor<CONST Graphics::ColorRGBA32&>())
      .Def(Constructor<BYTE,BYTE,BYTE,BYTE>())
      .Def(Constructor<BYTE,BYTE,BYTE>())

      .Property("R", &ColorRGBA32::GetR, &ColorRGBA32::SetR)
      .Property("G", &ColorRGBA32::GetG, &ColorRGBA32::SetG)
      .Property("B", &ColorRGBA32::GetB, &ColorRGBA32::SetB)
      .Property("A", &ColorRGBA32::GetA, &ColorRGBA32::SetA)

      .Property("Luminance", &ColorRGBA32::Luminance)

      .Def("Clamp", (VOID(ColorRGBA32::*)())&ColorRGBA32::Clamp)
      .Def("Clamp", (VOID(ColorRGBA32::*)(BYTE,BYTE))&ColorRGBA32::Clamp)

      .Def(tostring(Self))
      .Def(Self + ColorRGBA32())
      .Def(Self - ColorRGBA32())
      .Def(Self * FLOAT())
      .Def(Self / FLOAT())
      
      .Scope[ Def("FromHSV", &ColorRGBA32::FromHSV) ]
      .Scope[ Def("FromName", &ColorRGBA32::FromName) ];

}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------
CONST ColorRGBA32 ColorRGBA32::Transparent = ColorRGBA32(255, 255, 255, 0);
CONST ColorRGBA32 ColorRGBA32::AliceBlue = ColorRGBA32(240, 248, 255, 255);
CONST ColorRGBA32 ColorRGBA32::AntiqueWhite = ColorRGBA32(250, 235, 215, 255);
CONST ColorRGBA32 ColorRGBA32::Aqua = ColorRGBA32(0, 255, 255, 255);
CONST ColorRGBA32 ColorRGBA32::Aquamarine = ColorRGBA32(127, 255, 212, 255);
CONST ColorRGBA32 ColorRGBA32::Azure = ColorRGBA32(240, 255, 255, 255);
CONST ColorRGBA32 ColorRGBA32::Beige = ColorRGBA32(245, 245, 220, 255);
CONST ColorRGBA32 ColorRGBA32::Bisque = ColorRGBA32(255, 228, 196, 255);
CONST ColorRGBA32 ColorRGBA32::Black = ColorRGBA32(0, 0, 0, 255);
CONST ColorRGBA32 ColorRGBA32::BlanchedAlmond = ColorRGBA32(255, 235, 205, 255);
CONST ColorRGBA32 ColorRGBA32::Blue = ColorRGBA32(0, 0, 255, 255);
CONST ColorRGBA32 ColorRGBA32::BlueViolet = ColorRGBA32(138, 43, 226, 255);
CONST ColorRGBA32 ColorRGBA32::Brown = ColorRGBA32(165, 42, 42, 255);
CONST ColorRGBA32 ColorRGBA32::BurlyWood = ColorRGBA32(222, 184, 135, 255);
CONST ColorRGBA32 ColorRGBA32::CadetBlue = ColorRGBA32(95, 158, 160, 255);
CONST ColorRGBA32 ColorRGBA32::Chartreuse = ColorRGBA32(127, 255, 0, 255);
CONST ColorRGBA32 ColorRGBA32::Chocolate = ColorRGBA32(210, 105, 30, 255);
CONST ColorRGBA32 ColorRGBA32::Coral = ColorRGBA32(255, 127, 80, 255);
CONST ColorRGBA32 ColorRGBA32::CornflowerBlue = ColorRGBA32(100, 149, 237, 255);
CONST ColorRGBA32 ColorRGBA32::Cornsilk = ColorRGBA32(255, 248, 220, 255);
CONST ColorRGBA32 ColorRGBA32::Crimson = ColorRGBA32(220, 20, 60, 255);
CONST ColorRGBA32 ColorRGBA32::Cyan = ColorRGBA32(0, 255, 255, 255);
CONST ColorRGBA32 ColorRGBA32::DarkBlue = ColorRGBA32(0, 0, 139, 255);
CONST ColorRGBA32 ColorRGBA32::DarkCyan = ColorRGBA32(0, 139, 139, 255);
CONST ColorRGBA32 ColorRGBA32::DarkGoldenrod = ColorRGBA32(184, 134, 11, 255);
CONST ColorRGBA32 ColorRGBA32::DarkGray = ColorRGBA32(169, 169, 169, 255);
CONST ColorRGBA32 ColorRGBA32::DarkGreen = ColorRGBA32(0, 100, 0, 255);
CONST ColorRGBA32 ColorRGBA32::DarkKhaki = ColorRGBA32(189, 183, 107, 255);
CONST ColorRGBA32 ColorRGBA32::DarkMagenta = ColorRGBA32(139, 0, 139, 255);
CONST ColorRGBA32 ColorRGBA32::DarkOliveGreen = ColorRGBA32(85, 107, 47, 255);
CONST ColorRGBA32 ColorRGBA32::DarkOrange = ColorRGBA32(255, 140, 0, 255);
CONST ColorRGBA32 ColorRGBA32::DarkOrchid = ColorRGBA32(153, 50, 204, 255);
CONST ColorRGBA32 ColorRGBA32::DarkRed = ColorRGBA32(139, 0, 0, 255);
CONST ColorRGBA32 ColorRGBA32::DarkSalmon = ColorRGBA32(233, 150, 122, 255);
CONST ColorRGBA32 ColorRGBA32::DarkSeaGreen = ColorRGBA32(143, 188, 139, 255);
CONST ColorRGBA32 ColorRGBA32::DarkSlateBlue = ColorRGBA32(72, 61, 139, 255);
CONST ColorRGBA32 ColorRGBA32::DarkSlateGray = ColorRGBA32(47, 79, 79, 255);
CONST ColorRGBA32 ColorRGBA32::DarkTurquoise = ColorRGBA32(0, 206, 209, 255);
CONST ColorRGBA32 ColorRGBA32::DarkViolet = ColorRGBA32(148, 0, 211, 255);
CONST ColorRGBA32 ColorRGBA32::DeepPink = ColorRGBA32(255, 20, 147, 255);
CONST ColorRGBA32 ColorRGBA32::DeepSkyBlue = ColorRGBA32(0, 191, 255, 255);
CONST ColorRGBA32 ColorRGBA32::DimGray = ColorRGBA32(105, 105, 105, 255);
CONST ColorRGBA32 ColorRGBA32::DodgerBlue = ColorRGBA32(30, 144, 255, 255);
CONST ColorRGBA32 ColorRGBA32::Firebrick = ColorRGBA32(178, 34, 34, 255);
CONST ColorRGBA32 ColorRGBA32::FloralWhite = ColorRGBA32(255, 250, 240, 255);
CONST ColorRGBA32 ColorRGBA32::ForestGreen = ColorRGBA32(34, 139, 34, 255);
CONST ColorRGBA32 ColorRGBA32::Fuchsia = ColorRGBA32(255, 0, 255, 255);
CONST ColorRGBA32 ColorRGBA32::Gainsboro = ColorRGBA32(220, 220, 220, 255);
CONST ColorRGBA32 ColorRGBA32::GhostWhite = ColorRGBA32(248, 248, 255, 255);
CONST ColorRGBA32 ColorRGBA32::Gold = ColorRGBA32(255, 215, 0, 255);
CONST ColorRGBA32 ColorRGBA32::Goldenrod = ColorRGBA32(218, 165, 32, 255);
CONST ColorRGBA32 ColorRGBA32::Gray = ColorRGBA32(128, 128, 128, 255);
CONST ColorRGBA32 ColorRGBA32::Green = ColorRGBA32(0, 128, 0, 255);
CONST ColorRGBA32 ColorRGBA32::GreenYellow = ColorRGBA32(173, 255, 47, 255);
CONST ColorRGBA32 ColorRGBA32::Honeydew = ColorRGBA32(240, 255, 240, 255);
CONST ColorRGBA32 ColorRGBA32::HotPink = ColorRGBA32(255, 105, 180, 255);
CONST ColorRGBA32 ColorRGBA32::IndianRed = ColorRGBA32(205, 92, 92, 255);
CONST ColorRGBA32 ColorRGBA32::Indigo = ColorRGBA32(75, 0, 130, 255);
CONST ColorRGBA32 ColorRGBA32::Ivory = ColorRGBA32(255, 255, 240, 255);
CONST ColorRGBA32 ColorRGBA32::Khaki = ColorRGBA32(240, 230, 140, 255);
CONST ColorRGBA32 ColorRGBA32::Lavender = ColorRGBA32(230, 230, 250, 255);
CONST ColorRGBA32 ColorRGBA32::LavenderBlush = ColorRGBA32(255, 240, 245, 255);
CONST ColorRGBA32 ColorRGBA32::LawnGreen = ColorRGBA32(124, 252, 0, 255);
CONST ColorRGBA32 ColorRGBA32::LemonChiffon = ColorRGBA32(255, 250, 205, 255);
CONST ColorRGBA32 ColorRGBA32::LightBlue = ColorRGBA32(173, 216, 230, 255);
CONST ColorRGBA32 ColorRGBA32::LightCoral = ColorRGBA32(240, 128, 128, 255);
CONST ColorRGBA32 ColorRGBA32::LightCyan = ColorRGBA32(224, 255, 255, 255);
CONST ColorRGBA32 ColorRGBA32::LightGoldenrodYellow = ColorRGBA32(250, 250, 210, 255);
CONST ColorRGBA32 ColorRGBA32::LightGreen = ColorRGBA32(144, 238, 144, 255);
CONST ColorRGBA32 ColorRGBA32::LightGray = ColorRGBA32(211, 211, 211, 255);
CONST ColorRGBA32 ColorRGBA32::LightPink = ColorRGBA32(255, 182, 193, 255);
CONST ColorRGBA32 ColorRGBA32::LightSalmon = ColorRGBA32(255, 160, 122, 255);
CONST ColorRGBA32 ColorRGBA32::LightSeaGreen = ColorRGBA32(32, 178, 170, 255);
CONST ColorRGBA32 ColorRGBA32::LightSkyBlue = ColorRGBA32(135, 206, 250, 255);
CONST ColorRGBA32 ColorRGBA32::LightSlateGray = ColorRGBA32(119, 136, 153, 255);
CONST ColorRGBA32 ColorRGBA32::LightSteelBlue = ColorRGBA32(176, 196, 222, 255);
CONST ColorRGBA32 ColorRGBA32::LightYellow = ColorRGBA32(255, 255, 224, 255);
CONST ColorRGBA32 ColorRGBA32::Lime = ColorRGBA32(0, 255, 0, 255);
CONST ColorRGBA32 ColorRGBA32::LimeGreen = ColorRGBA32(50, 205, 50, 255);
CONST ColorRGBA32 ColorRGBA32::Linen = ColorRGBA32(250, 240, 230, 255);
CONST ColorRGBA32 ColorRGBA32::Magenta = ColorRGBA32(255, 0, 255, 255);
CONST ColorRGBA32 ColorRGBA32::Maroon = ColorRGBA32(128, 0, 0, 255);
CONST ColorRGBA32 ColorRGBA32::MediumAquamarine = ColorRGBA32(102, 205, 170, 255);
CONST ColorRGBA32 ColorRGBA32::MediumBlue = ColorRGBA32(0, 0, 205, 255);
CONST ColorRGBA32 ColorRGBA32::MediumOrchid = ColorRGBA32(186, 85, 211, 255);
CONST ColorRGBA32 ColorRGBA32::MediumPurple = ColorRGBA32(147, 112, 219, 255);
CONST ColorRGBA32 ColorRGBA32::MediumSeaGreen = ColorRGBA32(60, 179, 113, 255);
CONST ColorRGBA32 ColorRGBA32::MediumSlateBlue = ColorRGBA32(123, 104, 238, 255);
CONST ColorRGBA32 ColorRGBA32::MediumSpringGreen = ColorRGBA32(0, 250, 154, 255);
CONST ColorRGBA32 ColorRGBA32::MediumTurquoise = ColorRGBA32(72, 209, 204, 255);
CONST ColorRGBA32 ColorRGBA32::MediumVioletRed = ColorRGBA32(199, 21, 133, 255);
CONST ColorRGBA32 ColorRGBA32::MidnightBlue = ColorRGBA32(25, 25, 112, 255);
CONST ColorRGBA32 ColorRGBA32::MintCream = ColorRGBA32(245, 255, 250, 255);
CONST ColorRGBA32 ColorRGBA32::MistyRose = ColorRGBA32(255, 228, 225, 255);
CONST ColorRGBA32 ColorRGBA32::Moccasin = ColorRGBA32(255, 228, 181, 255);
CONST ColorRGBA32 ColorRGBA32::NavajoWhite = ColorRGBA32(255, 222, 173, 255);
CONST ColorRGBA32 ColorRGBA32::Navy = ColorRGBA32(0, 0, 128, 255);
CONST ColorRGBA32 ColorRGBA32::OldLace = ColorRGBA32(253, 245, 230, 255);
CONST ColorRGBA32 ColorRGBA32::Olive = ColorRGBA32(128, 128, 0, 255);
CONST ColorRGBA32 ColorRGBA32::OliveDrab = ColorRGBA32(107, 142, 35, 255);
CONST ColorRGBA32 ColorRGBA32::Orange = ColorRGBA32(255, 165, 0, 255);
CONST ColorRGBA32 ColorRGBA32::OrangeRed = ColorRGBA32(255, 69, 0, 255);
CONST ColorRGBA32 ColorRGBA32::Orchid = ColorRGBA32(218, 112, 214, 255);
CONST ColorRGBA32 ColorRGBA32::PaleGoldenrod = ColorRGBA32(238, 232, 170, 255);
CONST ColorRGBA32 ColorRGBA32::PaleGreen = ColorRGBA32(152, 251, 152, 255);
CONST ColorRGBA32 ColorRGBA32::PaleTurquoise = ColorRGBA32(175, 238, 238, 255);
CONST ColorRGBA32 ColorRGBA32::PaleVioletRed = ColorRGBA32(219, 112, 147, 255);
CONST ColorRGBA32 ColorRGBA32::PapayaWhip = ColorRGBA32(255, 239, 213, 255);
CONST ColorRGBA32 ColorRGBA32::PeachPuff = ColorRGBA32(255, 218, 185, 255);
CONST ColorRGBA32 ColorRGBA32::Peru = ColorRGBA32(205, 133, 63, 255);
CONST ColorRGBA32 ColorRGBA32::Pink = ColorRGBA32(255, 192, 203, 255);
CONST ColorRGBA32 ColorRGBA32::Plum = ColorRGBA32(221, 160, 221, 255);
CONST ColorRGBA32 ColorRGBA32::PowderBlue = ColorRGBA32(176, 224, 230, 255);
CONST ColorRGBA32 ColorRGBA32::Purple = ColorRGBA32(128, 0, 128, 255);
CONST ColorRGBA32 ColorRGBA32::Red = ColorRGBA32(255, 0, 0, 255);
CONST ColorRGBA32 ColorRGBA32::RosyBrown = ColorRGBA32(188, 143, 143, 255);
CONST ColorRGBA32 ColorRGBA32::RoyalBlue = ColorRGBA32(65, 105, 225, 255);
CONST ColorRGBA32 ColorRGBA32::SaddleBrown = ColorRGBA32(139, 69, 19, 255);
CONST ColorRGBA32 ColorRGBA32::Salmon = ColorRGBA32(250, 128, 114, 255);
CONST ColorRGBA32 ColorRGBA32::SandyBrown = ColorRGBA32(244, 164, 96, 255);
CONST ColorRGBA32 ColorRGBA32::SeaGreen = ColorRGBA32(46, 139, 87, 255);
CONST ColorRGBA32 ColorRGBA32::SeaShell = ColorRGBA32(255, 245, 238, 255);
CONST ColorRGBA32 ColorRGBA32::Sienna = ColorRGBA32(160, 82, 45, 255);
CONST ColorRGBA32 ColorRGBA32::Silver = ColorRGBA32(192, 192, 192, 255);
CONST ColorRGBA32 ColorRGBA32::SkyBlue = ColorRGBA32(135, 206, 235, 255);
CONST ColorRGBA32 ColorRGBA32::SlateBlue = ColorRGBA32(106, 90, 205, 255);
CONST ColorRGBA32 ColorRGBA32::SlateGray = ColorRGBA32(112, 128, 144, 255);
CONST ColorRGBA32 ColorRGBA32::Snow = ColorRGBA32(255, 250, 250, 255);
CONST ColorRGBA32 ColorRGBA32::SpringGreen = ColorRGBA32(0, 255, 127, 255);
CONST ColorRGBA32 ColorRGBA32::SteelBlue = ColorRGBA32(70, 130, 180, 255);
CONST ColorRGBA32 ColorRGBA32::Tan = ColorRGBA32(210, 180, 140, 255);
CONST ColorRGBA32 ColorRGBA32::Teal = ColorRGBA32(0, 128, 128, 255);
CONST ColorRGBA32 ColorRGBA32::Thistle = ColorRGBA32(216, 191, 216, 255);
CONST ColorRGBA32 ColorRGBA32::Tomato = ColorRGBA32(255, 99, 71, 255);
CONST ColorRGBA32 ColorRGBA32::Turquoise = ColorRGBA32(64, 224, 208, 255);
CONST ColorRGBA32 ColorRGBA32::Violet = ColorRGBA32(238, 130, 238, 255);
CONST ColorRGBA32 ColorRGBA32::Wheat = ColorRGBA32(245, 222, 179, 255);
CONST ColorRGBA32 ColorRGBA32::White = ColorRGBA32(255, 255, 255, 255);
CONST ColorRGBA32 ColorRGBA32::WhiteSmoke = ColorRGBA32(245, 245, 245, 255);
CONST ColorRGBA32 ColorRGBA32::Yellow = ColorRGBA32(255, 255, 0, 255);
CONST ColorRGBA32 ColorRGBA32::YellowGreen = ColorRGBA32(154, 205, 50, 255);
//-----------------------------------------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB32::ColorRGB32()
{
  m_V[0] = 0;
  m_V[1] = 0;
  m_V[2] = 0;
  m_V[3] = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB32::ColorRGB32(BYTE R, BYTE G, BYTE B)
{
  m_V[0] = R;
  m_V[1] = G;
  m_V[2] = B;
  m_V[3] = 255;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB32::ColorRGB32(CONST ColorRGB32& C)
{
  m_V[0] = C.m_V[0];
  m_V[1] = C.m_V[1];
  m_V[2] = C.m_V[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_DIRECTX9)
ColorRGB32::operator D3DCOLOR() CONST
{
  return D3DCOLOR_XRGB(m_V[0], m_V[1], m_V[2]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB32::operator D3DCOLORVALUE() CONST
{
  D3DCOLORVALUE V;
  V.r = m_V[0] / 255.0f;
  V.g = m_V[1] / 255.0f;
  V.b = m_V[2] / 255.0f;
  V.a = 1.0f;

  return V;
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ColorRGB32::Clamp()
{
  Clamp(0, 255);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ColorRGB32::Clamp(BYTE Min, BYTE Max)
{
  m_V[0] = (m_V[0] > Max) ? Max : m_V[0];
  m_V[1] = (m_V[1] > Max) ? Max : m_V[1];
  m_V[2] = (m_V[2] > Max) ? Max : m_V[2];

  m_V[0] = (m_V[0] < Min) ? Min : m_V[0];
  m_V[1] = (m_V[1] < Min) ? Min : m_V[1];
  m_V[2] = (m_V[2] < Min) ? Min : m_V[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB32 ColorRGB32::operator + (CONST ColorRGB32& C) CONST
{
  return ColorRGB32(m_V[0] + C.m_V[0], m_V[1] + C.m_V[1], m_V[2] + C.m_V[2]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB32 ColorRGB32::operator - (CONST ColorRGB32& C) CONST
{
  return ColorRGB32(m_V[0] - C.m_V[0], m_V[1] - C.m_V[1], m_V[2] - C.m_V[2]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB32& ColorRGB32::operator += (CONST ColorRGB32& C)
{
  m_V[0] += C.m_V[0];
  m_V[1] += C.m_V[1];
  m_V[2] += C.m_V[2];
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB32& ColorRGB32::operator -= (CONST ColorRGB32& C)
{
  m_V[0] += C.m_V[0];
  m_V[1] += C.m_V[1];
  m_V[2] += C.m_V[2];
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB32 ColorRGB32::operator * (FLOAT V) CONST
{
  return ColorRGB32((BYTE)(m_V[0]*V), (BYTE)(m_V[1]*V), (BYTE)(m_V[2]*V));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB32 ColorRGB32::operator / (FLOAT V) CONST
{
  return ColorRGB32((BYTE)(m_V[0]/V), (BYTE)(m_V[1]/V), (BYTE)(m_V[2]/V));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB32& ColorRGB32::operator *= (FLOAT V)
{
  m_V[0] = (BYTE)(m_V[0]*V);
  m_V[1] = (BYTE)(m_V[1]*V);
  m_V[2] = (BYTE)(m_V[2]*V);
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB32& ColorRGB32::operator /= (FLOAT V)
{
  m_V[0] = (BYTE)(m_V[0]/V);
  m_V[1] = (BYTE)(m_V[1]/V);
  m_V[2] = (BYTE)(m_V[2]/V);
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT ColorRGB32::Luminance() CONST
{
  return 0.299f*m_V[0] + 0.587f*m_V[1] + 0.114f*m_V[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB32 ColorRGB32::FromHSV(FLOAT Hue, FLOAT Saturation, FLOAT Value)
{
	if (Saturation <= 0.0f)
		return ColorRGB32((BYTE)(Value*255),(BYTE)(Value*255),(BYTE)(Value*255));
  Hue = Math::Mod(Hue, 2.0f*Math::PI);
	if (Hue < 0.0)
    Hue += 2.0f*Math::PI;
	
	Hue /= Math::PI / 3.0f;
	
  int i = int(Math::Floor(Hue));
	
	FLOAT f = Hue - i;
	FLOAT p = Value * (1.0f - Saturation);
	FLOAT q = Value * (1.0f - (Saturation*f));
	FLOAT t = Value * (1.0f - (Saturation*(1.0f-f)));
	
	switch(i) 
	{
	case 0: 
		return ColorRGB32((BYTE)(Value*255), (BYTE)(t*255), (BYTE)(p*255));
	case 1: 
		return ColorRGB32((BYTE)(q*255), (BYTE)(Value*255), (BYTE)(p*255));
	case 2: 
		return ColorRGB32((BYTE)(p*255), (BYTE)(Value*255), (BYTE)(t*255));
	case 3:
		return ColorRGB32((BYTE)(p*255), (BYTE)(q*255), (BYTE)(Value*255));
	case 4:
		return ColorRGB32((BYTE)(t*255), (BYTE)(p*255), (BYTE)(Value*255));
	default: 
		return ColorRGB32((BYTE)(Value*255), (BYTE)(p*255), (BYTE)(q*255));
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB32 ColorRGB32::RandomColor(FLOAT Saturation, FLOAT Value)
{
  FLOAT Hue = UniformRandom<FLOAT>() * 2.0f * PI;
	return FromHSV(Hue, Saturation, Value);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB32 ColorRGB32::FromName(LPCTSTR String)
{
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("AliceBlue")) == 0)
		return ColorRGB32::AliceBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("AntiqueWhite")) == 0)
		return ColorRGB32::AntiqueWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Aqua")) == 0)
		return ColorRGB32::Aqua;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Aquamarine")) == 0)
		return ColorRGB32::Aquamarine;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Azure")) == 0)
		return ColorRGB32::Azure;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Beige")) == 0)
		return ColorRGB32::Beige;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Bisque")) == 0)
		return ColorRGB32::Bisque;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Black")) == 0)
		return ColorRGB32::Black;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BlanchedAlmond")) == 0)
		return ColorRGB32::BlanchedAlmond;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Blue")) == 0)
		return ColorRGB32::Blue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BlueViolet")) == 0)
		return ColorRGB32::BlueViolet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Brown")) == 0)
		return ColorRGB32::Brown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BurlyWood")) == 0)
		return ColorRGB32::BurlyWood;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("CadetBlue")) == 0)
		return ColorRGB32::CadetBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Chartreuse")) == 0)
		return ColorRGB32::Chartreuse;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Chocolate")) == 0)
		return ColorRGB32::Chocolate;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Coral")) == 0)
		return ColorRGB32::Coral;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("CornflowerBlue")) == 0)
		return ColorRGB32::CornflowerBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Cornsilk")) == 0)
		return ColorRGB32::Cornsilk;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Crimson")) == 0)
		return ColorRGB32::Crimson;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Cyan")) == 0)
		return ColorRGB32::Cyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkBlue")) == 0)
		return ColorRGB32::DarkBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkCyan")) == 0)
		return ColorRGB32::DarkCyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGoldenrod")) == 0)
		return ColorRGB32::DarkGoldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGray")) == 0)
		return ColorRGB32::DarkGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGreen")) == 0)
		return ColorRGB32::DarkGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkKhaki")) == 0)
		return ColorRGB32::DarkKhaki;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkMagenta")) == 0)
		return ColorRGB32::DarkMagenta;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOliveGreen")) == 0)
		return ColorRGB32::DarkOliveGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOrange")) == 0)
		return ColorRGB32::DarkOrange;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOrchid")) == 0)
		return ColorRGB32::DarkOrchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkRed")) == 0)
		return ColorRGB32::DarkRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSalmon")) == 0)
		return ColorRGB32::DarkSalmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSeaGreen")) == 0)
		return ColorRGB32::DarkSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSlateBlue")) == 0)
		return ColorRGB32::DarkSlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSlateGray")) == 0)
		return ColorRGB32::DarkSlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkTurquoise")) == 0)
		return ColorRGB32::DarkTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkViolet")) == 0)
		return ColorRGB32::DarkViolet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DeepPink")) == 0)
		return ColorRGB32::DeepPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DeepSkyBlue")) == 0)
		return ColorRGB32::DeepSkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DimGray")) == 0)
		return ColorRGB32::DimGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DodgerBlue")) == 0)
		return ColorRGB32::DodgerBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Firebrick")) == 0)
		return ColorRGB32::Firebrick;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("FloralWhite")) == 0)
		return ColorRGB32::FloralWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("ForestGreen")) == 0)
		return ColorRGB32::ForestGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Fuchsia")) == 0)
		return ColorRGB32::Fuchsia;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gainsboro")) == 0)
		return ColorRGB32::Gainsboro;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("GhostWhite")) == 0)
		return ColorRGB32::GhostWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gold")) == 0)
		return ColorRGB32::Gold;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Goldenrod")) == 0)
		return ColorRGB32::Goldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gray")) == 0)
		return ColorRGB32::Gray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Green")) == 0)
		return ColorRGB32::Green;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("GreenYellow")) == 0)
		return ColorRGB32::GreenYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Honeydew")) == 0)
		return ColorRGB32::Honeydew;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("HotPink")) == 0)
		return ColorRGB32::HotPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("IndianRed")) == 0)
		return ColorRGB32::IndianRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Indigo")) == 0)
		return ColorRGB32::Indigo;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Ivory")) == 0)
		return ColorRGB32::Ivory;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Khaki")) == 0)
		return ColorRGB32::Khaki;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Lavender")) == 0)
		return ColorRGB32::Lavender;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LavenderBlush")) == 0)
		return ColorRGB32::LavenderBlush;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LawnGreen")) == 0)
		return ColorRGB32::LawnGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LemonChiffon")) == 0)
		return ColorRGB32::LemonChiffon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightBlue")) == 0)
		return ColorRGB32::LightBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightCoral")) == 0)
		return ColorRGB32::LightCoral;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightCyan")) == 0)
		return ColorRGB32::LightCyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGoldenrodYellow")) == 0)
		return ColorRGB32::LightGoldenrodYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGreen")) == 0)
		return ColorRGB32::LightGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGray")) == 0)
		return ColorRGB32::LightGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightPink")) == 0)
		return ColorRGB32::LightPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSalmon")) == 0)
		return ColorRGB32::LightSalmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSeaGreen")) == 0)
		return ColorRGB32::LightSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSkyBlue")) == 0)
		return ColorRGB32::LightSkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSlateGray")) == 0)
		return ColorRGB32::LightSlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSteelBlue")) == 0)
		return ColorRGB32::LightSteelBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightYellow")) == 0)
		return ColorRGB32::LightYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Lime")) == 0)
		return ColorRGB32::Lime;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LimeGreen")) == 0)
		return ColorRGB32::LimeGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Linen")) == 0)
		return ColorRGB32::Linen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Magenta")) == 0)
		return ColorRGB32::Magenta;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Maroon")) == 0)
		return ColorRGB32::Maroon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumAquamarine")) == 0)
		return ColorRGB32::MediumAquamarine;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumBlue")) == 0)
		return ColorRGB32::MediumBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumOrchid")) == 0)
		return ColorRGB32::MediumOrchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumPurple")) == 0)
		return ColorRGB32::MediumPurple;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSeaGreen")) == 0)
		return ColorRGB32::MediumSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSlateBlue")) == 0)
		return ColorRGB32::MediumSlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSpringGreen")) == 0)
		return ColorRGB32::MediumSpringGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumTurquoise")) == 0)
		return ColorRGB32::MediumTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumVioletRed")) == 0)
		return ColorRGB32::MediumVioletRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MidnightBlue")) == 0)
		return ColorRGB32::MidnightBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MintCream")) == 0)
		return ColorRGB32::MintCream;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MistyRose")) == 0)
		return ColorRGB32::MistyRose;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Moccasin")) == 0)
		return ColorRGB32::Moccasin;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("NavajoWhite")) == 0)
		return ColorRGB32::NavajoWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Navy")) == 0)
		return ColorRGB32::Navy;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OldLace")) == 0)
		return ColorRGB32::OldLace;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Olive")) == 0)
		return ColorRGB32::Olive;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OliveDrab")) == 0)
		return ColorRGB32::OliveDrab;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Orange")) == 0)
		return ColorRGB32::Orange;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OrangeRed")) == 0)
		return ColorRGB32::OrangeRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Orchid")) == 0)
		return ColorRGB32::Orchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleGoldenrod")) == 0)
		return ColorRGB32::PaleGoldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleGreen")) == 0)
		return ColorRGB32::PaleGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleTurquoise")) == 0)
		return ColorRGB32::PaleTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleVioletRed")) == 0)
		return ColorRGB32::PaleVioletRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PapayaWhip")) == 0)
		return ColorRGB32::PapayaWhip;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PeachPuff")) == 0)
		return ColorRGB32::PeachPuff;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Peru")) == 0)
		return ColorRGB32::Peru;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Pink")) == 0)
		return ColorRGB32::Pink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Plum")) == 0)
		return ColorRGB32::Plum;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PowderBlue")) == 0)
		return ColorRGB32::PowderBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Purple")) == 0)
		return ColorRGB32::Purple;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Red")) == 0)
		return ColorRGB32::Red;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("RosyBrown")) == 0)
		return ColorRGB32::RosyBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("RoyalBlue")) == 0)
		return ColorRGB32::RoyalBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SaddleBrown")) == 0)
		return ColorRGB32::SaddleBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Salmon")) == 0)
		return ColorRGB32::Salmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SandyBrown")) == 0)
		return ColorRGB32::SandyBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SeaGreen")) == 0)
		return ColorRGB32::SeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SeaShell")) == 0)
		return ColorRGB32::SeaShell;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Sienna")) == 0)
		return ColorRGB32::Sienna;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Silver")) == 0)
		return ColorRGB32::Silver;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SkyBlue")) == 0)
		return ColorRGB32::SkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SlateBlue")) == 0)
		return ColorRGB32::SlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SlateGray")) == 0)
		return ColorRGB32::SlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Snow")) == 0)
		return ColorRGB32::Snow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SpringGreen")) == 0)
		return ColorRGB32::SpringGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SteelBlue")) == 0)
		return ColorRGB32::SteelBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Tan")) == 0)
		return ColorRGB32::Tan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Teal")) == 0)
		return ColorRGB32::Teal;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Thistle")) == 0)
		return ColorRGB32::Thistle;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Tomato")) == 0)
		return ColorRGB32::Tomato;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Turquoise")) == 0)
		return ColorRGB32::Turquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Violet")) == 0)
		return ColorRGB32::Violet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Wheat")) == 0)
		return ColorRGB32::Wheat;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("White")) == 0)
		return ColorRGB32::White;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("WhiteSmoke")) == 0)
		return ColorRGB32::WhiteSmoke;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Yellow")) == 0)
		return ColorRGB32::Yellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("YellowGreen")) == 0)
		return ColorRGB32::YellowGreen;

  return ColorRGB32::Black;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String ColorRGB32::ToString() CONST
{
  return String::Format(TEXT("(%d,%d,%d)"), m_V[0], m_V[1], m_V[2]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_LUA)
Lua::Bind::Scope ColorRGB32::GenerateBindings(LPCTSTR Name)
{
  using namespace Lua::Bind;

  return 
    Class<Graphics::ColorRGB32>(Name)
      .Def(Constructor<>())
      .Def(Constructor<CONST Graphics::ColorRGB32&>())
      .Def(Constructor<BYTE,BYTE,BYTE>())

      .Property("R", &ColorRGB32::GetR, &ColorRGB32::SetR)
      .Property("G", &ColorRGB32::GetG, &ColorRGB32::SetG)
      .Property("B", &ColorRGB32::GetB, &ColorRGB32::SetB)

      .Property("Luminance", &ColorRGB32::Luminance)

      .Def("Clamp", (VOID(ColorRGB32::*)())&ColorRGB32::Clamp)
      .Def("Clamp", (VOID(ColorRGB32::*)(BYTE,BYTE))&ColorRGB32::Clamp)

      .Def(tostring(Self))
      .Def(Self + ColorRGB32())
      .Def(Self - ColorRGB32())
      .Def(Self * FLOAT())
      .Def(Self / FLOAT())
      
      .Scope[ Def("FromHSV", &ColorRGB32::FromHSV) ]
      .Scope[ Def("FromName", &ColorRGB32::FromName) ];

}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------
CONST ColorRGB32 ColorRGB32::AliceBlue = ColorRGB32(240, 248, 255);
CONST ColorRGB32 ColorRGB32::AntiqueWhite = ColorRGB32(250, 235, 215);
CONST ColorRGB32 ColorRGB32::Aqua = ColorRGB32(0, 255, 255);
CONST ColorRGB32 ColorRGB32::Aquamarine = ColorRGB32(127, 255, 212);
CONST ColorRGB32 ColorRGB32::Azure = ColorRGB32(240, 255, 255);
CONST ColorRGB32 ColorRGB32::Beige = ColorRGB32(245, 245, 220);
CONST ColorRGB32 ColorRGB32::Bisque = ColorRGB32(255, 228, 196);
CONST ColorRGB32 ColorRGB32::Black = ColorRGB32(0, 0, 0);
CONST ColorRGB32 ColorRGB32::BlanchedAlmond = ColorRGB32(255, 235, 205);
CONST ColorRGB32 ColorRGB32::Blue = ColorRGB32(0, 0, 255);
CONST ColorRGB32 ColorRGB32::BlueViolet = ColorRGB32(138, 43, 226);
CONST ColorRGB32 ColorRGB32::Brown = ColorRGB32(165, 42, 42);
CONST ColorRGB32 ColorRGB32::BurlyWood = ColorRGB32(222, 184, 135);
CONST ColorRGB32 ColorRGB32::CadetBlue = ColorRGB32(95, 158, 160);
CONST ColorRGB32 ColorRGB32::Chartreuse = ColorRGB32(127, 255, 0);
CONST ColorRGB32 ColorRGB32::Chocolate = ColorRGB32(210, 105, 30);
CONST ColorRGB32 ColorRGB32::Coral = ColorRGB32(255, 127, 80);
CONST ColorRGB32 ColorRGB32::CornflowerBlue = ColorRGB32(100, 149, 237);
CONST ColorRGB32 ColorRGB32::Cornsilk = ColorRGB32(255, 248, 220);
CONST ColorRGB32 ColorRGB32::Crimson = ColorRGB32(220, 20, 60);
CONST ColorRGB32 ColorRGB32::Cyan = ColorRGB32(0, 255, 255);
CONST ColorRGB32 ColorRGB32::DarkBlue = ColorRGB32(0, 0, 139);
CONST ColorRGB32 ColorRGB32::DarkCyan = ColorRGB32(0, 139, 139);
CONST ColorRGB32 ColorRGB32::DarkGoldenrod = ColorRGB32(184, 134, 11);
CONST ColorRGB32 ColorRGB32::DarkGray = ColorRGB32(169, 169, 169);
CONST ColorRGB32 ColorRGB32::DarkGreen = ColorRGB32(0, 100, 0);
CONST ColorRGB32 ColorRGB32::DarkKhaki = ColorRGB32(189, 183, 107);
CONST ColorRGB32 ColorRGB32::DarkMagenta = ColorRGB32(139, 0, 139);
CONST ColorRGB32 ColorRGB32::DarkOliveGreen = ColorRGB32(85, 107, 47);
CONST ColorRGB32 ColorRGB32::DarkOrange = ColorRGB32(255, 140, 0);
CONST ColorRGB32 ColorRGB32::DarkOrchid = ColorRGB32(153, 50, 204);
CONST ColorRGB32 ColorRGB32::DarkRed = ColorRGB32(139, 0, 0);
CONST ColorRGB32 ColorRGB32::DarkSalmon = ColorRGB32(233, 150, 122);
CONST ColorRGB32 ColorRGB32::DarkSeaGreen = ColorRGB32(143, 188, 139);
CONST ColorRGB32 ColorRGB32::DarkSlateBlue = ColorRGB32(72, 61, 139);
CONST ColorRGB32 ColorRGB32::DarkSlateGray = ColorRGB32(47, 79, 79);
CONST ColorRGB32 ColorRGB32::DarkTurquoise = ColorRGB32(0, 206, 209);
CONST ColorRGB32 ColorRGB32::DarkViolet = ColorRGB32(148, 0, 211);
CONST ColorRGB32 ColorRGB32::DeepPink = ColorRGB32(255, 20, 147);
CONST ColorRGB32 ColorRGB32::DeepSkyBlue = ColorRGB32(0, 191, 255);
CONST ColorRGB32 ColorRGB32::DimGray = ColorRGB32(105, 105, 105);
CONST ColorRGB32 ColorRGB32::DodgerBlue = ColorRGB32(30, 144, 255);
CONST ColorRGB32 ColorRGB32::Firebrick = ColorRGB32(178, 34, 34);
CONST ColorRGB32 ColorRGB32::FloralWhite = ColorRGB32(255, 250, 240);
CONST ColorRGB32 ColorRGB32::ForestGreen = ColorRGB32(34, 139, 34);
CONST ColorRGB32 ColorRGB32::Fuchsia = ColorRGB32(255, 0, 255);
CONST ColorRGB32 ColorRGB32::Gainsboro = ColorRGB32(220, 220, 220);
CONST ColorRGB32 ColorRGB32::GhostWhite = ColorRGB32(248, 248, 255);
CONST ColorRGB32 ColorRGB32::Gold = ColorRGB32(255, 215, 0);
CONST ColorRGB32 ColorRGB32::Goldenrod = ColorRGB32(218, 165, 32);
CONST ColorRGB32 ColorRGB32::Gray = ColorRGB32(128, 128, 128);
CONST ColorRGB32 ColorRGB32::Green = ColorRGB32(0, 128, 0);
CONST ColorRGB32 ColorRGB32::GreenYellow = ColorRGB32(173, 255, 47);
CONST ColorRGB32 ColorRGB32::Honeydew = ColorRGB32(240, 255, 240);
CONST ColorRGB32 ColorRGB32::HotPink = ColorRGB32(255, 105, 180);
CONST ColorRGB32 ColorRGB32::IndianRed = ColorRGB32(205, 92, 92);
CONST ColorRGB32 ColorRGB32::Indigo = ColorRGB32(75, 0, 130);
CONST ColorRGB32 ColorRGB32::Ivory = ColorRGB32(255, 255, 240);
CONST ColorRGB32 ColorRGB32::Khaki = ColorRGB32(240, 230, 140);
CONST ColorRGB32 ColorRGB32::Lavender = ColorRGB32(230, 230, 250);
CONST ColorRGB32 ColorRGB32::LavenderBlush = ColorRGB32(255, 240, 245);
CONST ColorRGB32 ColorRGB32::LawnGreen = ColorRGB32(124, 252, 0);
CONST ColorRGB32 ColorRGB32::LemonChiffon = ColorRGB32(255, 250, 205);
CONST ColorRGB32 ColorRGB32::LightBlue = ColorRGB32(173, 216, 230);
CONST ColorRGB32 ColorRGB32::LightCoral = ColorRGB32(240, 128, 128);
CONST ColorRGB32 ColorRGB32::LightCyan = ColorRGB32(224, 255, 255);
CONST ColorRGB32 ColorRGB32::LightGoldenrodYellow = ColorRGB32(250, 250, 210);
CONST ColorRGB32 ColorRGB32::LightGreen = ColorRGB32(144, 238, 144);
CONST ColorRGB32 ColorRGB32::LightGray = ColorRGB32(211, 211, 211);
CONST ColorRGB32 ColorRGB32::LightPink = ColorRGB32(255, 182, 193);
CONST ColorRGB32 ColorRGB32::LightSalmon = ColorRGB32(255, 160, 122);
CONST ColorRGB32 ColorRGB32::LightSeaGreen = ColorRGB32(32, 178, 170);
CONST ColorRGB32 ColorRGB32::LightSkyBlue = ColorRGB32(135, 206, 250);
CONST ColorRGB32 ColorRGB32::LightSlateGray = ColorRGB32(119, 136, 153);
CONST ColorRGB32 ColorRGB32::LightSteelBlue = ColorRGB32(176, 196, 222);
CONST ColorRGB32 ColorRGB32::LightYellow = ColorRGB32(255, 255, 224);
CONST ColorRGB32 ColorRGB32::Lime = ColorRGB32(0, 255, 0);
CONST ColorRGB32 ColorRGB32::LimeGreen = ColorRGB32(50, 205, 50);
CONST ColorRGB32 ColorRGB32::Linen = ColorRGB32(250, 240, 230);
CONST ColorRGB32 ColorRGB32::Magenta = ColorRGB32(255, 0, 255);
CONST ColorRGB32 ColorRGB32::Maroon = ColorRGB32(128, 0, 0);
CONST ColorRGB32 ColorRGB32::MediumAquamarine = ColorRGB32(102, 205, 170);
CONST ColorRGB32 ColorRGB32::MediumBlue = ColorRGB32(0, 0, 205);
CONST ColorRGB32 ColorRGB32::MediumOrchid = ColorRGB32(186, 85, 211);
CONST ColorRGB32 ColorRGB32::MediumPurple = ColorRGB32(147, 112, 219);
CONST ColorRGB32 ColorRGB32::MediumSeaGreen = ColorRGB32(60, 179, 113);
CONST ColorRGB32 ColorRGB32::MediumSlateBlue = ColorRGB32(123, 104, 238);
CONST ColorRGB32 ColorRGB32::MediumSpringGreen = ColorRGB32(0, 250, 154);
CONST ColorRGB32 ColorRGB32::MediumTurquoise = ColorRGB32(72, 209, 204);
CONST ColorRGB32 ColorRGB32::MediumVioletRed = ColorRGB32(199, 21, 133);
CONST ColorRGB32 ColorRGB32::MidnightBlue = ColorRGB32(25, 25, 112);
CONST ColorRGB32 ColorRGB32::MintCream = ColorRGB32(245, 255, 250);
CONST ColorRGB32 ColorRGB32::MistyRose = ColorRGB32(255, 228, 225);
CONST ColorRGB32 ColorRGB32::Moccasin = ColorRGB32(255, 228, 181);
CONST ColorRGB32 ColorRGB32::NavajoWhite = ColorRGB32(255, 222, 173);
CONST ColorRGB32 ColorRGB32::Navy = ColorRGB32(0, 0, 128);
CONST ColorRGB32 ColorRGB32::OldLace = ColorRGB32(253, 245, 230);
CONST ColorRGB32 ColorRGB32::Olive = ColorRGB32(128, 128, 0);
CONST ColorRGB32 ColorRGB32::OliveDrab = ColorRGB32(107, 142, 35);
CONST ColorRGB32 ColorRGB32::Orange = ColorRGB32(255, 165, 0);
CONST ColorRGB32 ColorRGB32::OrangeRed = ColorRGB32(255, 69, 0);
CONST ColorRGB32 ColorRGB32::Orchid = ColorRGB32(218, 112, 214);
CONST ColorRGB32 ColorRGB32::PaleGoldenrod = ColorRGB32(238, 232, 170);
CONST ColorRGB32 ColorRGB32::PaleGreen = ColorRGB32(152, 251, 152);
CONST ColorRGB32 ColorRGB32::PaleTurquoise = ColorRGB32(175, 238, 238);
CONST ColorRGB32 ColorRGB32::PaleVioletRed = ColorRGB32(219, 112, 147);
CONST ColorRGB32 ColorRGB32::PapayaWhip = ColorRGB32(255, 239, 213);
CONST ColorRGB32 ColorRGB32::PeachPuff = ColorRGB32(255, 218, 185);
CONST ColorRGB32 ColorRGB32::Peru = ColorRGB32(205, 133, 63);
CONST ColorRGB32 ColorRGB32::Pink = ColorRGB32(255, 192, 203);
CONST ColorRGB32 ColorRGB32::Plum = ColorRGB32(221, 160, 221);
CONST ColorRGB32 ColorRGB32::PowderBlue = ColorRGB32(176, 224, 230);
CONST ColorRGB32 ColorRGB32::Purple = ColorRGB32(128, 0, 128);
CONST ColorRGB32 ColorRGB32::Red = ColorRGB32(255, 0, 0);
CONST ColorRGB32 ColorRGB32::RosyBrown = ColorRGB32(188, 143, 143);
CONST ColorRGB32 ColorRGB32::RoyalBlue = ColorRGB32(65, 105, 225);
CONST ColorRGB32 ColorRGB32::SaddleBrown = ColorRGB32(139, 69, 19);
CONST ColorRGB32 ColorRGB32::Salmon = ColorRGB32(250, 128, 114);
CONST ColorRGB32 ColorRGB32::SandyBrown = ColorRGB32(244, 164, 96);
CONST ColorRGB32 ColorRGB32::SeaGreen = ColorRGB32(46, 139, 87);
CONST ColorRGB32 ColorRGB32::SeaShell = ColorRGB32(255, 245, 238);
CONST ColorRGB32 ColorRGB32::Sienna = ColorRGB32(160, 82, 45);
CONST ColorRGB32 ColorRGB32::Silver = ColorRGB32(192, 192, 192);
CONST ColorRGB32 ColorRGB32::SkyBlue = ColorRGB32(135, 206, 235);
CONST ColorRGB32 ColorRGB32::SlateBlue = ColorRGB32(106, 90, 205);
CONST ColorRGB32 ColorRGB32::SlateGray = ColorRGB32(112, 128, 144);
CONST ColorRGB32 ColorRGB32::Snow = ColorRGB32(255, 250, 250);
CONST ColorRGB32 ColorRGB32::SpringGreen = ColorRGB32(0, 255, 127);
CONST ColorRGB32 ColorRGB32::SteelBlue = ColorRGB32(70, 130, 180);
CONST ColorRGB32 ColorRGB32::Tan = ColorRGB32(210, 180, 140);
CONST ColorRGB32 ColorRGB32::Teal = ColorRGB32(0, 128, 128);
CONST ColorRGB32 ColorRGB32::Thistle = ColorRGB32(216, 191, 216);
CONST ColorRGB32 ColorRGB32::Tomato = ColorRGB32(255, 99, 71);
CONST ColorRGB32 ColorRGB32::Turquoise = ColorRGB32(64, 224, 208);
CONST ColorRGB32 ColorRGB32::Violet = ColorRGB32(238, 130, 238);
CONST ColorRGB32 ColorRGB32::Wheat = ColorRGB32(245, 222, 179);
CONST ColorRGB32 ColorRGB32::White = ColorRGB32(255, 255, 255);
CONST ColorRGB32 ColorRGB32::WhiteSmoke = ColorRGB32(245, 245, 245);
CONST ColorRGB32 ColorRGB32::Yellow = ColorRGB32(255, 255, 0);
CONST ColorRGB32 ColorRGB32::YellowGreen = ColorRGB32(154, 205, 50);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB24::ColorRGB24()
{
  m_V[0] = 0;
  m_V[1] = 0;
  m_V[2] = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB24::ColorRGB24(BYTE R, BYTE G, BYTE B)
{
  m_V[0] = R;
  m_V[1] = G;
  m_V[2] = B;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB24::ColorRGB24(CONST ColorRGB24& C)
{
  m_V[0] = C.m_V[0];
  m_V[1] = C.m_V[1];
  m_V[2] = C.m_V[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_DIRECTX9)
ColorRGB24::operator D3DCOLOR() CONST
{
  return D3DCOLOR_XRGB(m_V[0], m_V[1], m_V[2]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB24::operator D3DCOLORVALUE() CONST
{
  D3DCOLORVALUE V;
  V.r = m_V[0] / 255.0f;
  V.g = m_V[1] / 255.0f;
  V.b = m_V[2] / 255.0f;
  V.a = 1.0f;

  return V;
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ColorRGB24::Clamp()
{
  Clamp(0, 255);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ColorRGB24::Clamp(BYTE Min, BYTE Max)
{
  m_V[0] = (m_V[0] > Max) ? Max : m_V[0];
  m_V[1] = (m_V[1] > Max) ? Max : m_V[1];
  m_V[2] = (m_V[2] > Max) ? Max : m_V[2];

  m_V[0] = (m_V[0] < Min) ? Min : m_V[0];
  m_V[1] = (m_V[1] < Min) ? Min : m_V[1];
  m_V[2] = (m_V[2] < Min) ? Min : m_V[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB24 ColorRGB24::operator + (CONST ColorRGB24& C) CONST
{
  return ColorRGB24(m_V[0] + C.m_V[0], m_V[1] + C.m_V[1], m_V[2] + C.m_V[2]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB24 ColorRGB24::operator - (CONST ColorRGB24& C) CONST
{
  return ColorRGB24(m_V[0] - C.m_V[0], m_V[1] - C.m_V[1], m_V[2] - C.m_V[2]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB24& ColorRGB24::operator += (CONST ColorRGB24& C)
{
  m_V[0] += C.m_V[0];
  m_V[1] += C.m_V[1];
  m_V[2] += C.m_V[2];
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB24& ColorRGB24::operator -= (CONST ColorRGB24& C)
{
  m_V[0] += C.m_V[0];
  m_V[1] += C.m_V[1];
  m_V[2] += C.m_V[2];
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB24 ColorRGB24::operator * (FLOAT V) CONST
{
  return ColorRGB24((BYTE)(m_V[0]*V), (BYTE)(m_V[1]*V), (BYTE)(m_V[2]*V));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB24 ColorRGB24::operator / (FLOAT V) CONST
{
  return ColorRGB24((BYTE)(m_V[0]/V), (BYTE)(m_V[1]/V), (BYTE)(m_V[2]/V));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB24& ColorRGB24::operator *= (FLOAT V)
{
  m_V[0] = (BYTE)(m_V[0]*V);
  m_V[1] = (BYTE)(m_V[1]*V);
  m_V[2] = (BYTE)(m_V[2]*V);
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB24& ColorRGB24::operator /= (FLOAT V)
{
  m_V[0] = (BYTE)(m_V[0]/V);
  m_V[1] = (BYTE)(m_V[1]/V);
  m_V[2] = (BYTE)(m_V[2]/V);
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT ColorRGB24::Luminance() CONST
{
  return 0.299f*m_V[0] + 0.587f*m_V[1] + 0.114f*m_V[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB24 ColorRGB24::FromHSV(FLOAT Hue, FLOAT Saturation, FLOAT Value)
{
	if (Saturation <= 0.0f)
		return ColorRGB24((BYTE)(Value*255),(BYTE)(Value*255),(BYTE)(Value*255));
  Hue = Math::Mod(Hue, 2.0f*Math::PI);
	if (Hue < 0.0)
    Hue += 2.0f*Math::PI;
	
	Hue /= Math::PI / 3.0f;
	
  int i = int(Math::Floor(Hue));
	
	FLOAT f = Hue - i;
	FLOAT p = Value * (1.0f - Saturation);
	FLOAT q = Value * (1.0f - (Saturation*f));
	FLOAT t = Value * (1.0f - (Saturation*(1.0f-f)));
	
	switch(i) 
	{
	case 0: 
		return ColorRGB24((BYTE)(Value*255), (BYTE)(t*255), (BYTE)(p*255));
	case 1: 
		return ColorRGB24((BYTE)(q*255), (BYTE)(Value*255), (BYTE)(p*255));
	case 2: 
		return ColorRGB24((BYTE)(p*255), (BYTE)(Value*255), (BYTE)(t*255));
	case 3:
		return ColorRGB24((BYTE)(p*255), (BYTE)(q*255), (BYTE)(Value*255));
	case 4:
		return ColorRGB24((BYTE)(t*255), (BYTE)(p*255), (BYTE)(Value*255));
	default: 
		return ColorRGB24((BYTE)(Value*255), (BYTE)(p*255), (BYTE)(q*255));
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB24 ColorRGB24::RandomColor(FLOAT Saturation, FLOAT Value)
{
  FLOAT Hue = UniformRandom<FLOAT>() * 2.0f * PI;
	return FromHSV(Hue, Saturation, Value);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB24 ColorRGB24::FromName(LPCTSTR String)
{
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("AliceBlue")) == 0)
		return ColorRGB24::AliceBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("AntiqueWhite")) == 0)
		return ColorRGB24::AntiqueWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Aqua")) == 0)
		return ColorRGB24::Aqua;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Aquamarine")) == 0)
		return ColorRGB24::Aquamarine;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Azure")) == 0)
		return ColorRGB24::Azure;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Beige")) == 0)
		return ColorRGB24::Beige;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Bisque")) == 0)
		return ColorRGB24::Bisque;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Black")) == 0)
		return ColorRGB24::Black;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BlanchedAlmond")) == 0)
		return ColorRGB24::BlanchedAlmond;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Blue")) == 0)
		return ColorRGB24::Blue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BlueViolet")) == 0)
		return ColorRGB24::BlueViolet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Brown")) == 0)
		return ColorRGB24::Brown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BurlyWood")) == 0)
		return ColorRGB24::BurlyWood;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("CadetBlue")) == 0)
		return ColorRGB24::CadetBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Chartreuse")) == 0)
		return ColorRGB24::Chartreuse;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Chocolate")) == 0)
		return ColorRGB24::Chocolate;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Coral")) == 0)
		return ColorRGB24::Coral;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("CornflowerBlue")) == 0)
		return ColorRGB24::CornflowerBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Cornsilk")) == 0)
		return ColorRGB24::Cornsilk;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Crimson")) == 0)
		return ColorRGB24::Crimson;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Cyan")) == 0)
		return ColorRGB24::Cyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkBlue")) == 0)
		return ColorRGB24::DarkBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkCyan")) == 0)
		return ColorRGB24::DarkCyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGoldenrod")) == 0)
		return ColorRGB24::DarkGoldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGray")) == 0)
		return ColorRGB24::DarkGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGreen")) == 0)
		return ColorRGB24::DarkGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkKhaki")) == 0)
		return ColorRGB24::DarkKhaki;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkMagenta")) == 0)
		return ColorRGB24::DarkMagenta;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOliveGreen")) == 0)
		return ColorRGB24::DarkOliveGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOrange")) == 0)
		return ColorRGB24::DarkOrange;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOrchid")) == 0)
		return ColorRGB24::DarkOrchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkRed")) == 0)
		return ColorRGB24::DarkRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSalmon")) == 0)
		return ColorRGB24::DarkSalmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSeaGreen")) == 0)
		return ColorRGB24::DarkSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSlateBlue")) == 0)
		return ColorRGB24::DarkSlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSlateGray")) == 0)
		return ColorRGB24::DarkSlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkTurquoise")) == 0)
		return ColorRGB24::DarkTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkViolet")) == 0)
		return ColorRGB24::DarkViolet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DeepPink")) == 0)
		return ColorRGB24::DeepPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DeepSkyBlue")) == 0)
		return ColorRGB24::DeepSkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DimGray")) == 0)
		return ColorRGB24::DimGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DodgerBlue")) == 0)
		return ColorRGB24::DodgerBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Firebrick")) == 0)
		return ColorRGB24::Firebrick;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("FloralWhite")) == 0)
		return ColorRGB24::FloralWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("ForestGreen")) == 0)
		return ColorRGB24::ForestGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Fuchsia")) == 0)
		return ColorRGB24::Fuchsia;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gainsboro")) == 0)
		return ColorRGB24::Gainsboro;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("GhostWhite")) == 0)
		return ColorRGB24::GhostWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gold")) == 0)
		return ColorRGB24::Gold;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Goldenrod")) == 0)
		return ColorRGB24::Goldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gray")) == 0)
		return ColorRGB24::Gray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Green")) == 0)
		return ColorRGB24::Green;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("GreenYellow")) == 0)
		return ColorRGB24::GreenYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Honeydew")) == 0)
		return ColorRGB24::Honeydew;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("HotPink")) == 0)
		return ColorRGB24::HotPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("IndianRed")) == 0)
		return ColorRGB24::IndianRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Indigo")) == 0)
		return ColorRGB24::Indigo;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Ivory")) == 0)
		return ColorRGB24::Ivory;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Khaki")) == 0)
		return ColorRGB24::Khaki;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Lavender")) == 0)
		return ColorRGB24::Lavender;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LavenderBlush")) == 0)
		return ColorRGB24::LavenderBlush;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LawnGreen")) == 0)
		return ColorRGB24::LawnGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LemonChiffon")) == 0)
		return ColorRGB24::LemonChiffon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightBlue")) == 0)
		return ColorRGB24::LightBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightCoral")) == 0)
		return ColorRGB24::LightCoral;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightCyan")) == 0)
		return ColorRGB24::LightCyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGoldenrodYellow")) == 0)
		return ColorRGB24::LightGoldenrodYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGreen")) == 0)
		return ColorRGB24::LightGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGray")) == 0)
		return ColorRGB24::LightGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightPink")) == 0)
		return ColorRGB24::LightPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSalmon")) == 0)
		return ColorRGB24::LightSalmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSeaGreen")) == 0)
		return ColorRGB24::LightSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSkyBlue")) == 0)
		return ColorRGB24::LightSkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSlateGray")) == 0)
		return ColorRGB24::LightSlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSteelBlue")) == 0)
		return ColorRGB24::LightSteelBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightYellow")) == 0)
		return ColorRGB24::LightYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Lime")) == 0)
		return ColorRGB24::Lime;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LimeGreen")) == 0)
		return ColorRGB24::LimeGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Linen")) == 0)
		return ColorRGB24::Linen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Magenta")) == 0)
		return ColorRGB24::Magenta;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Maroon")) == 0)
		return ColorRGB24::Maroon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumAquamarine")) == 0)
		return ColorRGB24::MediumAquamarine;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumBlue")) == 0)
		return ColorRGB24::MediumBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumOrchid")) == 0)
		return ColorRGB24::MediumOrchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumPurple")) == 0)
		return ColorRGB24::MediumPurple;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSeaGreen")) == 0)
		return ColorRGB24::MediumSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSlateBlue")) == 0)
		return ColorRGB24::MediumSlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSpringGreen")) == 0)
		return ColorRGB24::MediumSpringGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumTurquoise")) == 0)
		return ColorRGB24::MediumTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumVioletRed")) == 0)
		return ColorRGB24::MediumVioletRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MidnightBlue")) == 0)
		return ColorRGB24::MidnightBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MintCream")) == 0)
		return ColorRGB24::MintCream;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MistyRose")) == 0)
		return ColorRGB24::MistyRose;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Moccasin")) == 0)
		return ColorRGB24::Moccasin;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("NavajoWhite")) == 0)
		return ColorRGB24::NavajoWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Navy")) == 0)
		return ColorRGB24::Navy;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OldLace")) == 0)
		return ColorRGB24::OldLace;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Olive")) == 0)
		return ColorRGB24::Olive;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OliveDrab")) == 0)
		return ColorRGB24::OliveDrab;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Orange")) == 0)
		return ColorRGB24::Orange;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OrangeRed")) == 0)
		return ColorRGB24::OrangeRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Orchid")) == 0)
		return ColorRGB24::Orchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleGoldenrod")) == 0)
		return ColorRGB24::PaleGoldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleGreen")) == 0)
		return ColorRGB24::PaleGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleTurquoise")) == 0)
		return ColorRGB24::PaleTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleVioletRed")) == 0)
		return ColorRGB24::PaleVioletRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PapayaWhip")) == 0)
		return ColorRGB24::PapayaWhip;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PeachPuff")) == 0)
		return ColorRGB24::PeachPuff;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Peru")) == 0)
		return ColorRGB24::Peru;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Pink")) == 0)
		return ColorRGB24::Pink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Plum")) == 0)
		return ColorRGB24::Plum;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PowderBlue")) == 0)
		return ColorRGB24::PowderBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Purple")) == 0)
		return ColorRGB24::Purple;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Red")) == 0)
		return ColorRGB24::Red;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("RosyBrown")) == 0)
		return ColorRGB24::RosyBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("RoyalBlue")) == 0)
		return ColorRGB24::RoyalBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SaddleBrown")) == 0)
		return ColorRGB24::SaddleBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Salmon")) == 0)
		return ColorRGB24::Salmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SandyBrown")) == 0)
		return ColorRGB24::SandyBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SeaGreen")) == 0)
		return ColorRGB24::SeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SeaShell")) == 0)
		return ColorRGB24::SeaShell;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Sienna")) == 0)
		return ColorRGB24::Sienna;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Silver")) == 0)
		return ColorRGB24::Silver;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SkyBlue")) == 0)
		return ColorRGB24::SkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SlateBlue")) == 0)
		return ColorRGB24::SlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SlateGray")) == 0)
		return ColorRGB24::SlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Snow")) == 0)
		return ColorRGB24::Snow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SpringGreen")) == 0)
		return ColorRGB24::SpringGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SteelBlue")) == 0)
		return ColorRGB24::SteelBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Tan")) == 0)
		return ColorRGB24::Tan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Teal")) == 0)
		return ColorRGB24::Teal;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Thistle")) == 0)
		return ColorRGB24::Thistle;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Tomato")) == 0)
		return ColorRGB24::Tomato;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Turquoise")) == 0)
		return ColorRGB24::Turquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Violet")) == 0)
		return ColorRGB24::Violet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Wheat")) == 0)
		return ColorRGB24::Wheat;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("White")) == 0)
		return ColorRGB24::White;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("WhiteSmoke")) == 0)
		return ColorRGB24::WhiteSmoke;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Yellow")) == 0)
		return ColorRGB24::Yellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("YellowGreen")) == 0)
		return ColorRGB24::YellowGreen;

  return ColorRGB24::Black;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String ColorRGB24::ToString() CONST
{
  return String::Format(TEXT("(%d,%d,%d)"), m_V[0], m_V[1], m_V[2]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_LUA)
Lua::Bind::Scope ColorRGB24::GenerateBindings(LPCTSTR Name)
{
  using namespace Lua::Bind;

  return 
    Class<Graphics::ColorRGB24>(Name)
      .Def(Constructor<>())
      .Def(Constructor<CONST Graphics::ColorRGB24&>())
      .Def(Constructor<BYTE,BYTE,BYTE>())

      .Property("R", &ColorRGB24::GetR, &ColorRGB24::SetR)
      .Property("G", &ColorRGB24::GetG, &ColorRGB24::SetG)
      .Property("B", &ColorRGB24::GetB, &ColorRGB24::SetB)

      .Property("Luminance", &ColorRGB24::Luminance)

      .Def("Clamp", (VOID(ColorRGB24::*)())&ColorRGB24::Clamp)
      .Def("Clamp", (VOID(ColorRGB24::*)(BYTE,BYTE))&ColorRGB24::Clamp)

      .Def(tostring(Self))
      .Def(Self + ColorRGB24())
      .Def(Self - ColorRGB24())
      .Def(Self * FLOAT())
      .Def(Self / FLOAT())
      
      .Scope[ Def("FromHSV", &ColorRGB24::FromHSV) ]
      .Scope[ Def("FromName", &ColorRGB24::FromName) ];

}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------
CONST ColorRGB24 ColorRGB24::AliceBlue = ColorRGB24(240, 248, 255);
CONST ColorRGB24 ColorRGB24::AntiqueWhite = ColorRGB24(250, 235, 215);
CONST ColorRGB24 ColorRGB24::Aqua = ColorRGB24(0, 255, 255);
CONST ColorRGB24 ColorRGB24::Aquamarine = ColorRGB24(127, 255, 212);
CONST ColorRGB24 ColorRGB24::Azure = ColorRGB24(240, 255, 255);
CONST ColorRGB24 ColorRGB24::Beige = ColorRGB24(245, 245, 220);
CONST ColorRGB24 ColorRGB24::Bisque = ColorRGB24(255, 228, 196);
CONST ColorRGB24 ColorRGB24::Black = ColorRGB24(0, 0, 0);
CONST ColorRGB24 ColorRGB24::BlanchedAlmond = ColorRGB24(255, 235, 205);
CONST ColorRGB24 ColorRGB24::Blue = ColorRGB24(0, 0, 255);
CONST ColorRGB24 ColorRGB24::BlueViolet = ColorRGB24(138, 43, 226);
CONST ColorRGB24 ColorRGB24::Brown = ColorRGB24(165, 42, 42);
CONST ColorRGB24 ColorRGB24::BurlyWood = ColorRGB24(222, 184, 135);
CONST ColorRGB24 ColorRGB24::CadetBlue = ColorRGB24(95, 158, 160);
CONST ColorRGB24 ColorRGB24::Chartreuse = ColorRGB24(127, 255, 0);
CONST ColorRGB24 ColorRGB24::Chocolate = ColorRGB24(210, 105, 30);
CONST ColorRGB24 ColorRGB24::Coral = ColorRGB24(255, 127, 80);
CONST ColorRGB24 ColorRGB24::CornflowerBlue = ColorRGB24(100, 149, 237);
CONST ColorRGB24 ColorRGB24::Cornsilk = ColorRGB24(255, 248, 220);
CONST ColorRGB24 ColorRGB24::Crimson = ColorRGB24(220, 20, 60);
CONST ColorRGB24 ColorRGB24::Cyan = ColorRGB24(0, 255, 255);
CONST ColorRGB24 ColorRGB24::DarkBlue = ColorRGB24(0, 0, 139);
CONST ColorRGB24 ColorRGB24::DarkCyan = ColorRGB24(0, 139, 139);
CONST ColorRGB24 ColorRGB24::DarkGoldenrod = ColorRGB24(184, 134, 11);
CONST ColorRGB24 ColorRGB24::DarkGray = ColorRGB24(169, 169, 169);
CONST ColorRGB24 ColorRGB24::DarkGreen = ColorRGB24(0, 100, 0);
CONST ColorRGB24 ColorRGB24::DarkKhaki = ColorRGB24(189, 183, 107);
CONST ColorRGB24 ColorRGB24::DarkMagenta = ColorRGB24(139, 0, 139);
CONST ColorRGB24 ColorRGB24::DarkOliveGreen = ColorRGB24(85, 107, 47);
CONST ColorRGB24 ColorRGB24::DarkOrange = ColorRGB24(255, 140, 0);
CONST ColorRGB24 ColorRGB24::DarkOrchid = ColorRGB24(153, 50, 204);
CONST ColorRGB24 ColorRGB24::DarkRed = ColorRGB24(139, 0, 0);
CONST ColorRGB24 ColorRGB24::DarkSalmon = ColorRGB24(233, 150, 122);
CONST ColorRGB24 ColorRGB24::DarkSeaGreen = ColorRGB24(143, 188, 139);
CONST ColorRGB24 ColorRGB24::DarkSlateBlue = ColorRGB24(72, 61, 139);
CONST ColorRGB24 ColorRGB24::DarkSlateGray = ColorRGB24(47, 79, 79);
CONST ColorRGB24 ColorRGB24::DarkTurquoise = ColorRGB24(0, 206, 209);
CONST ColorRGB24 ColorRGB24::DarkViolet = ColorRGB24(148, 0, 211);
CONST ColorRGB24 ColorRGB24::DeepPink = ColorRGB24(255, 20, 147);
CONST ColorRGB24 ColorRGB24::DeepSkyBlue = ColorRGB24(0, 191, 255);
CONST ColorRGB24 ColorRGB24::DimGray = ColorRGB24(105, 105, 105);
CONST ColorRGB24 ColorRGB24::DodgerBlue = ColorRGB24(30, 144, 255);
CONST ColorRGB24 ColorRGB24::Firebrick = ColorRGB24(178, 34, 34);
CONST ColorRGB24 ColorRGB24::FloralWhite = ColorRGB24(255, 250, 240);
CONST ColorRGB24 ColorRGB24::ForestGreen = ColorRGB24(34, 139, 34);
CONST ColorRGB24 ColorRGB24::Fuchsia = ColorRGB24(255, 0, 255);
CONST ColorRGB24 ColorRGB24::Gainsboro = ColorRGB24(220, 220, 220);
CONST ColorRGB24 ColorRGB24::GhostWhite = ColorRGB24(248, 248, 255);
CONST ColorRGB24 ColorRGB24::Gold = ColorRGB24(255, 215, 0);
CONST ColorRGB24 ColorRGB24::Goldenrod = ColorRGB24(218, 165, 32);
CONST ColorRGB24 ColorRGB24::Gray = ColorRGB24(128, 128, 128);
CONST ColorRGB24 ColorRGB24::Green = ColorRGB24(0, 128, 0);
CONST ColorRGB24 ColorRGB24::GreenYellow = ColorRGB24(173, 255, 47);
CONST ColorRGB24 ColorRGB24::Honeydew = ColorRGB24(240, 255, 240);
CONST ColorRGB24 ColorRGB24::HotPink = ColorRGB24(255, 105, 180);
CONST ColorRGB24 ColorRGB24::IndianRed = ColorRGB24(205, 92, 92);
CONST ColorRGB24 ColorRGB24::Indigo = ColorRGB24(75, 0, 130);
CONST ColorRGB24 ColorRGB24::Ivory = ColorRGB24(255, 255, 240);
CONST ColorRGB24 ColorRGB24::Khaki = ColorRGB24(240, 230, 140);
CONST ColorRGB24 ColorRGB24::Lavender = ColorRGB24(230, 230, 250);
CONST ColorRGB24 ColorRGB24::LavenderBlush = ColorRGB24(255, 240, 245);
CONST ColorRGB24 ColorRGB24::LawnGreen = ColorRGB24(124, 252, 0);
CONST ColorRGB24 ColorRGB24::LemonChiffon = ColorRGB24(255, 250, 205);
CONST ColorRGB24 ColorRGB24::LightBlue = ColorRGB24(173, 216, 230);
CONST ColorRGB24 ColorRGB24::LightCoral = ColorRGB24(240, 128, 128);
CONST ColorRGB24 ColorRGB24::LightCyan = ColorRGB24(224, 255, 255);
CONST ColorRGB24 ColorRGB24::LightGoldenrodYellow = ColorRGB24(250, 250, 210);
CONST ColorRGB24 ColorRGB24::LightGreen = ColorRGB24(144, 238, 144);
CONST ColorRGB24 ColorRGB24::LightGray = ColorRGB24(211, 211, 211);
CONST ColorRGB24 ColorRGB24::LightPink = ColorRGB24(255, 182, 193);
CONST ColorRGB24 ColorRGB24::LightSalmon = ColorRGB24(255, 160, 122);
CONST ColorRGB24 ColorRGB24::LightSeaGreen = ColorRGB24(32, 178, 170);
CONST ColorRGB24 ColorRGB24::LightSkyBlue = ColorRGB24(135, 206, 250);
CONST ColorRGB24 ColorRGB24::LightSlateGray = ColorRGB24(119, 136, 153);
CONST ColorRGB24 ColorRGB24::LightSteelBlue = ColorRGB24(176, 196, 222);
CONST ColorRGB24 ColorRGB24::LightYellow = ColorRGB24(255, 255, 224);
CONST ColorRGB24 ColorRGB24::Lime = ColorRGB24(0, 255, 0);
CONST ColorRGB24 ColorRGB24::LimeGreen = ColorRGB24(50, 205, 50);
CONST ColorRGB24 ColorRGB24::Linen = ColorRGB24(250, 240, 230);
CONST ColorRGB24 ColorRGB24::Magenta = ColorRGB24(255, 0, 255);
CONST ColorRGB24 ColorRGB24::Maroon = ColorRGB24(128, 0, 0);
CONST ColorRGB24 ColorRGB24::MediumAquamarine = ColorRGB24(102, 205, 170);
CONST ColorRGB24 ColorRGB24::MediumBlue = ColorRGB24(0, 0, 205);
CONST ColorRGB24 ColorRGB24::MediumOrchid = ColorRGB24(186, 85, 211);
CONST ColorRGB24 ColorRGB24::MediumPurple = ColorRGB24(147, 112, 219);
CONST ColorRGB24 ColorRGB24::MediumSeaGreen = ColorRGB24(60, 179, 113);
CONST ColorRGB24 ColorRGB24::MediumSlateBlue = ColorRGB24(123, 104, 238);
CONST ColorRGB24 ColorRGB24::MediumSpringGreen = ColorRGB24(0, 250, 154);
CONST ColorRGB24 ColorRGB24::MediumTurquoise = ColorRGB24(72, 209, 204);
CONST ColorRGB24 ColorRGB24::MediumVioletRed = ColorRGB24(199, 21, 133);
CONST ColorRGB24 ColorRGB24::MidnightBlue = ColorRGB24(25, 25, 112);
CONST ColorRGB24 ColorRGB24::MintCream = ColorRGB24(245, 255, 250);
CONST ColorRGB24 ColorRGB24::MistyRose = ColorRGB24(255, 228, 225);
CONST ColorRGB24 ColorRGB24::Moccasin = ColorRGB24(255, 228, 181);
CONST ColorRGB24 ColorRGB24::NavajoWhite = ColorRGB24(255, 222, 173);
CONST ColorRGB24 ColorRGB24::Navy = ColorRGB24(0, 0, 128);
CONST ColorRGB24 ColorRGB24::OldLace = ColorRGB24(253, 245, 230);
CONST ColorRGB24 ColorRGB24::Olive = ColorRGB24(128, 128, 0);
CONST ColorRGB24 ColorRGB24::OliveDrab = ColorRGB24(107, 142, 35);
CONST ColorRGB24 ColorRGB24::Orange = ColorRGB24(255, 165, 0);
CONST ColorRGB24 ColorRGB24::OrangeRed = ColorRGB24(255, 69, 0);
CONST ColorRGB24 ColorRGB24::Orchid = ColorRGB24(218, 112, 214);
CONST ColorRGB24 ColorRGB24::PaleGoldenrod = ColorRGB24(238, 232, 170);
CONST ColorRGB24 ColorRGB24::PaleGreen = ColorRGB24(152, 251, 152);
CONST ColorRGB24 ColorRGB24::PaleTurquoise = ColorRGB24(175, 238, 238);
CONST ColorRGB24 ColorRGB24::PaleVioletRed = ColorRGB24(219, 112, 147);
CONST ColorRGB24 ColorRGB24::PapayaWhip = ColorRGB24(255, 239, 213);
CONST ColorRGB24 ColorRGB24::PeachPuff = ColorRGB24(255, 218, 185);
CONST ColorRGB24 ColorRGB24::Peru = ColorRGB24(205, 133, 63);
CONST ColorRGB24 ColorRGB24::Pink = ColorRGB24(255, 192, 203);
CONST ColorRGB24 ColorRGB24::Plum = ColorRGB24(221, 160, 221);
CONST ColorRGB24 ColorRGB24::PowderBlue = ColorRGB24(176, 224, 230);
CONST ColorRGB24 ColorRGB24::Purple = ColorRGB24(128, 0, 128);
CONST ColorRGB24 ColorRGB24::Red = ColorRGB24(255, 0, 0);
CONST ColorRGB24 ColorRGB24::RosyBrown = ColorRGB24(188, 143, 143);
CONST ColorRGB24 ColorRGB24::RoyalBlue = ColorRGB24(65, 105, 225);
CONST ColorRGB24 ColorRGB24::SaddleBrown = ColorRGB24(139, 69, 19);
CONST ColorRGB24 ColorRGB24::Salmon = ColorRGB24(250, 128, 114);
CONST ColorRGB24 ColorRGB24::SandyBrown = ColorRGB24(244, 164, 96);
CONST ColorRGB24 ColorRGB24::SeaGreen = ColorRGB24(46, 139, 87);
CONST ColorRGB24 ColorRGB24::SeaShell = ColorRGB24(255, 245, 238);
CONST ColorRGB24 ColorRGB24::Sienna = ColorRGB24(160, 82, 45);
CONST ColorRGB24 ColorRGB24::Silver = ColorRGB24(192, 192, 192);
CONST ColorRGB24 ColorRGB24::SkyBlue = ColorRGB24(135, 206, 235);
CONST ColorRGB24 ColorRGB24::SlateBlue = ColorRGB24(106, 90, 205);
CONST ColorRGB24 ColorRGB24::SlateGray = ColorRGB24(112, 128, 144);
CONST ColorRGB24 ColorRGB24::Snow = ColorRGB24(255, 250, 250);
CONST ColorRGB24 ColorRGB24::SpringGreen = ColorRGB24(0, 255, 127);
CONST ColorRGB24 ColorRGB24::SteelBlue = ColorRGB24(70, 130, 180);
CONST ColorRGB24 ColorRGB24::Tan = ColorRGB24(210, 180, 140);
CONST ColorRGB24 ColorRGB24::Teal = ColorRGB24(0, 128, 128);
CONST ColorRGB24 ColorRGB24::Thistle = ColorRGB24(216, 191, 216);
CONST ColorRGB24 ColorRGB24::Tomato = ColorRGB24(255, 99, 71);
CONST ColorRGB24 ColorRGB24::Turquoise = ColorRGB24(64, 224, 208);
CONST ColorRGB24 ColorRGB24::Violet = ColorRGB24(238, 130, 238);
CONST ColorRGB24 ColorRGB24::Wheat = ColorRGB24(245, 222, 179);
CONST ColorRGB24 ColorRGB24::White = ColorRGB24(255, 255, 255);
CONST ColorRGB24 ColorRGB24::WhiteSmoke = ColorRGB24(245, 245, 245);
CONST ColorRGB24 ColorRGB24::Yellow = ColorRGB24(255, 255, 0);
CONST ColorRGB24 ColorRGB24::YellowGreen = ColorRGB24(154, 205, 50);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGRA32::ColorBGRA32()
{
  m_V[0] = 0;
  m_V[1] = 0;
  m_V[2] = 0;
  m_V[3] = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGRA32::ColorBGRA32(BYTE R, BYTE G, BYTE B, BYTE A)
{
  m_V[0] = B;
  m_V[1] = G;
  m_V[2] = R;
  m_V[3] = A;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGRA32::ColorBGRA32(BYTE R, BYTE G, BYTE B)
{
  m_V[0] = B;
  m_V[1] = G;
  m_V[2] = R;
  m_V[3] = 255;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGRA32::ColorBGRA32(CONST ColorBGRA32& C)
{
  m_V[0] = C.m_V[0];
  m_V[1] = C.m_V[1];
  m_V[2] = C.m_V[2];
  m_V[3] = C.m_V[3];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_DIRECTX9)
ColorBGRA32::operator D3DCOLOR() CONST
{
  return D3DCOLOR_RGBA(m_V[0], m_V[1], m_V[2], m_V[3]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGRA32::operator D3DCOLORVALUE() CONST
{
  D3DCOLORVALUE V;
  V.r = m_V[0] / 255.0f;
  V.g = m_V[1] / 255.0f;
  V.b = m_V[2] / 255.0f;
  V.a = m_V[3] / 255.0f;

  return V;
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ColorBGRA32::Clamp()
{
  Clamp(0, 255);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ColorBGRA32::Clamp(BYTE Min, BYTE Max)
{
  m_V[0] = (m_V[0] > Max) ? Max : m_V[0];
  m_V[1] = (m_V[1] > Max) ? Max : m_V[1];
  m_V[2] = (m_V[2] > Max) ? Max : m_V[2];
  m_V[3] = (m_V[3] > Max) ? Max : m_V[3];

  m_V[0] = (m_V[0] < Min) ? Min : m_V[0];
  m_V[1] = (m_V[1] < Min) ? Min : m_V[1];
  m_V[2] = (m_V[2] < Min) ? Min : m_V[2];
  m_V[3] = (m_V[3] < Min) ? Min : m_V[3];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGRA32 ColorBGRA32::operator + (CONST ColorBGRA32& C) CONST
{
  return ColorBGRA32(m_V[0] + C.m_V[0], m_V[1] + C.m_V[1], m_V[2] + C.m_V[2], m_V[3] + C.m_V[3]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGRA32 ColorBGRA32::operator - (CONST ColorBGRA32& C) CONST
{
  return ColorBGRA32(m_V[0] - C.m_V[0], m_V[1] - C.m_V[1], m_V[2] - C.m_V[2], m_V[3] - C.m_V[3]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGRA32& ColorBGRA32::operator += (CONST ColorBGRA32& C)
{
  m_V[0] += C.m_V[0];
  m_V[1] += C.m_V[1];
  m_V[2] += C.m_V[2];
  m_V[3] += C.m_V[3];
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGRA32& ColorBGRA32::operator -= (CONST ColorBGRA32& C)
{
  m_V[0] += C.m_V[0];
  m_V[1] += C.m_V[1];
  m_V[2] += C.m_V[2];
  m_V[3] += C.m_V[3];
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGRA32 ColorBGRA32::operator * (FLOAT V) CONST
{
  return ColorBGRA32((BYTE)(m_V[0]*V), (BYTE)(m_V[1]*V), (BYTE)(m_V[2]*V), (BYTE)(m_V[3]*V));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGRA32 ColorBGRA32::operator / (FLOAT V) CONST
{
  return ColorBGRA32((BYTE)(m_V[0]/V), (BYTE)(m_V[1]/V), (BYTE)(m_V[2]/V), (BYTE)(m_V[3]/V));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGRA32& ColorBGRA32::operator *= (FLOAT V)
{
  m_V[0] = (BYTE)(m_V[0]*V);
  m_V[1] = (BYTE)(m_V[1]*V);
  m_V[2] = (BYTE)(m_V[2]*V);
  m_V[3] = (BYTE)(m_V[3]*V);
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGRA32& ColorBGRA32::operator /= (FLOAT V)
{
  m_V[0] = (BYTE)(m_V[0]/V);
  m_V[1] = (BYTE)(m_V[1]/V);
  m_V[2] = (BYTE)(m_V[2]/V);
  m_V[3] = (BYTE)(m_V[3]/V);
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT ColorBGRA32::Luminance() CONST
{
  return 0.299f*m_V[0] + 0.587f*m_V[1] + 0.114f*m_V[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGRA32 ColorBGRA32::FromHSV(FLOAT Hue, FLOAT Saturation, FLOAT Value)
{
	if (Saturation <= 0.0f)
		return ColorBGRA32((BYTE)(Value*255),(BYTE)(Value*255),(BYTE)(Value*255));
  Hue = Math::Mod(Hue, 2.0f*Math::PI);
	if (Hue < 0.0)
    Hue += 2.0f*Math::PI;
	
	Hue /= Math::PI / 3.0f;
	
  int i = int(Math::Floor(Hue));
	
	FLOAT f = Hue - i;
	FLOAT p = Value * (1.0f - Saturation);
	FLOAT q = Value * (1.0f - (Saturation*f));
	FLOAT t = Value * (1.0f - (Saturation*(1.0f-f)));
	
	switch(i) 
	{
	case 0: 
		return ColorBGRA32((BYTE)(Value*255), (BYTE)(t*255), (BYTE)(p*255));
	case 1: 
		return ColorBGRA32((BYTE)(q*255), (BYTE)(Value*255), (BYTE)(p*255));
	case 2: 
		return ColorBGRA32((BYTE)(p*255), (BYTE)(Value*255), (BYTE)(t*255));
	case 3:
		return ColorBGRA32((BYTE)(p*255), (BYTE)(q*255), (BYTE)(Value*255));
	case 4:
		return ColorBGRA32((BYTE)(t*255), (BYTE)(p*255), (BYTE)(Value*255));
	default: 
		return ColorBGRA32((BYTE)(Value*255), (BYTE)(p*255), (BYTE)(q*255));
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGRA32 ColorBGRA32::RandomColor(FLOAT Saturation, FLOAT Value)
{
  FLOAT Hue = UniformRandom<FLOAT>() * 2.0f * PI;
	return FromHSV(Hue, Saturation, Value);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGRA32 ColorBGRA32::FromName(LPCTSTR String)
{
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Transparent")) == 0)
		return ColorBGRA32::Transparent;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("AliceBlue")) == 0)
		return ColorBGRA32::AliceBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("AntiqueWhite")) == 0)
		return ColorBGRA32::AntiqueWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Aqua")) == 0)
		return ColorBGRA32::Aqua;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Aquamarine")) == 0)
		return ColorBGRA32::Aquamarine;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Azure")) == 0)
		return ColorBGRA32::Azure;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Beige")) == 0)
		return ColorBGRA32::Beige;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Bisque")) == 0)
		return ColorBGRA32::Bisque;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Black")) == 0)
		return ColorBGRA32::Black;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BlanchedAlmond")) == 0)
		return ColorBGRA32::BlanchedAlmond;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Blue")) == 0)
		return ColorBGRA32::Blue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BlueViolet")) == 0)
		return ColorBGRA32::BlueViolet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Brown")) == 0)
		return ColorBGRA32::Brown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BurlyWood")) == 0)
		return ColorBGRA32::BurlyWood;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("CadetBlue")) == 0)
		return ColorBGRA32::CadetBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Chartreuse")) == 0)
		return ColorBGRA32::Chartreuse;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Chocolate")) == 0)
		return ColorBGRA32::Chocolate;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Coral")) == 0)
		return ColorBGRA32::Coral;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("CornflowerBlue")) == 0)
		return ColorBGRA32::CornflowerBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Cornsilk")) == 0)
		return ColorBGRA32::Cornsilk;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Crimson")) == 0)
		return ColorBGRA32::Crimson;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Cyan")) == 0)
		return ColorBGRA32::Cyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkBlue")) == 0)
		return ColorBGRA32::DarkBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkCyan")) == 0)
		return ColorBGRA32::DarkCyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGoldenrod")) == 0)
		return ColorBGRA32::DarkGoldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGray")) == 0)
		return ColorBGRA32::DarkGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGreen")) == 0)
		return ColorBGRA32::DarkGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkKhaki")) == 0)
		return ColorBGRA32::DarkKhaki;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkMagenta")) == 0)
		return ColorBGRA32::DarkMagenta;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOliveGreen")) == 0)
		return ColorBGRA32::DarkOliveGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOrange")) == 0)
		return ColorBGRA32::DarkOrange;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOrchid")) == 0)
		return ColorBGRA32::DarkOrchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkRed")) == 0)
		return ColorBGRA32::DarkRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSalmon")) == 0)
		return ColorBGRA32::DarkSalmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSeaGreen")) == 0)
		return ColorBGRA32::DarkSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSlateBlue")) == 0)
		return ColorBGRA32::DarkSlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSlateGray")) == 0)
		return ColorBGRA32::DarkSlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkTurquoise")) == 0)
		return ColorBGRA32::DarkTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkViolet")) == 0)
		return ColorBGRA32::DarkViolet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DeepPink")) == 0)
		return ColorBGRA32::DeepPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DeepSkyBlue")) == 0)
		return ColorBGRA32::DeepSkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DimGray")) == 0)
		return ColorBGRA32::DimGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DodgerBlue")) == 0)
		return ColorBGRA32::DodgerBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Firebrick")) == 0)
		return ColorBGRA32::Firebrick;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("FloralWhite")) == 0)
		return ColorBGRA32::FloralWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("ForestGreen")) == 0)
		return ColorBGRA32::ForestGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Fuchsia")) == 0)
		return ColorBGRA32::Fuchsia;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gainsboro")) == 0)
		return ColorBGRA32::Gainsboro;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("GhostWhite")) == 0)
		return ColorBGRA32::GhostWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gold")) == 0)
		return ColorBGRA32::Gold;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Goldenrod")) == 0)
		return ColorBGRA32::Goldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gray")) == 0)
		return ColorBGRA32::Gray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Green")) == 0)
		return ColorBGRA32::Green;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("GreenYellow")) == 0)
		return ColorBGRA32::GreenYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Honeydew")) == 0)
		return ColorBGRA32::Honeydew;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("HotPink")) == 0)
		return ColorBGRA32::HotPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("IndianRed")) == 0)
		return ColorBGRA32::IndianRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Indigo")) == 0)
		return ColorBGRA32::Indigo;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Ivory")) == 0)
		return ColorBGRA32::Ivory;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Khaki")) == 0)
		return ColorBGRA32::Khaki;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Lavender")) == 0)
		return ColorBGRA32::Lavender;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LavenderBlush")) == 0)
		return ColorBGRA32::LavenderBlush;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LawnGreen")) == 0)
		return ColorBGRA32::LawnGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LemonChiffon")) == 0)
		return ColorBGRA32::LemonChiffon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightBlue")) == 0)
		return ColorBGRA32::LightBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightCoral")) == 0)
		return ColorBGRA32::LightCoral;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightCyan")) == 0)
		return ColorBGRA32::LightCyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGoldenrodYellow")) == 0)
		return ColorBGRA32::LightGoldenrodYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGreen")) == 0)
		return ColorBGRA32::LightGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGray")) == 0)
		return ColorBGRA32::LightGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightPink")) == 0)
		return ColorBGRA32::LightPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSalmon")) == 0)
		return ColorBGRA32::LightSalmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSeaGreen")) == 0)
		return ColorBGRA32::LightSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSkyBlue")) == 0)
		return ColorBGRA32::LightSkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSlateGray")) == 0)
		return ColorBGRA32::LightSlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSteelBlue")) == 0)
		return ColorBGRA32::LightSteelBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightYellow")) == 0)
		return ColorBGRA32::LightYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Lime")) == 0)
		return ColorBGRA32::Lime;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LimeGreen")) == 0)
		return ColorBGRA32::LimeGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Linen")) == 0)
		return ColorBGRA32::Linen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Magenta")) == 0)
		return ColorBGRA32::Magenta;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Maroon")) == 0)
		return ColorBGRA32::Maroon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumAquamarine")) == 0)
		return ColorBGRA32::MediumAquamarine;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumBlue")) == 0)
		return ColorBGRA32::MediumBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumOrchid")) == 0)
		return ColorBGRA32::MediumOrchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumPurple")) == 0)
		return ColorBGRA32::MediumPurple;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSeaGreen")) == 0)
		return ColorBGRA32::MediumSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSlateBlue")) == 0)
		return ColorBGRA32::MediumSlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSpringGreen")) == 0)
		return ColorBGRA32::MediumSpringGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumTurquoise")) == 0)
		return ColorBGRA32::MediumTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumVioletRed")) == 0)
		return ColorBGRA32::MediumVioletRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MidnightBlue")) == 0)
		return ColorBGRA32::MidnightBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MintCream")) == 0)
		return ColorBGRA32::MintCream;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MistyRose")) == 0)
		return ColorBGRA32::MistyRose;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Moccasin")) == 0)
		return ColorBGRA32::Moccasin;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("NavajoWhite")) == 0)
		return ColorBGRA32::NavajoWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Navy")) == 0)
		return ColorBGRA32::Navy;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OldLace")) == 0)
		return ColorBGRA32::OldLace;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Olive")) == 0)
		return ColorBGRA32::Olive;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OliveDrab")) == 0)
		return ColorBGRA32::OliveDrab;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Orange")) == 0)
		return ColorBGRA32::Orange;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OrangeRed")) == 0)
		return ColorBGRA32::OrangeRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Orchid")) == 0)
		return ColorBGRA32::Orchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleGoldenrod")) == 0)
		return ColorBGRA32::PaleGoldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleGreen")) == 0)
		return ColorBGRA32::PaleGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleTurquoise")) == 0)
		return ColorBGRA32::PaleTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleVioletRed")) == 0)
		return ColorBGRA32::PaleVioletRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PapayaWhip")) == 0)
		return ColorBGRA32::PapayaWhip;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PeachPuff")) == 0)
		return ColorBGRA32::PeachPuff;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Peru")) == 0)
		return ColorBGRA32::Peru;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Pink")) == 0)
		return ColorBGRA32::Pink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Plum")) == 0)
		return ColorBGRA32::Plum;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PowderBlue")) == 0)
		return ColorBGRA32::PowderBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Purple")) == 0)
		return ColorBGRA32::Purple;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Red")) == 0)
		return ColorBGRA32::Red;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("RosyBrown")) == 0)
		return ColorBGRA32::RosyBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("RoyalBlue")) == 0)
		return ColorBGRA32::RoyalBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SaddleBrown")) == 0)
		return ColorBGRA32::SaddleBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Salmon")) == 0)
		return ColorBGRA32::Salmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SandyBrown")) == 0)
		return ColorBGRA32::SandyBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SeaGreen")) == 0)
		return ColorBGRA32::SeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SeaShell")) == 0)
		return ColorBGRA32::SeaShell;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Sienna")) == 0)
		return ColorBGRA32::Sienna;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Silver")) == 0)
		return ColorBGRA32::Silver;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SkyBlue")) == 0)
		return ColorBGRA32::SkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SlateBlue")) == 0)
		return ColorBGRA32::SlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SlateGray")) == 0)
		return ColorBGRA32::SlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Snow")) == 0)
		return ColorBGRA32::Snow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SpringGreen")) == 0)
		return ColorBGRA32::SpringGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SteelBlue")) == 0)
		return ColorBGRA32::SteelBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Tan")) == 0)
		return ColorBGRA32::Tan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Teal")) == 0)
		return ColorBGRA32::Teal;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Thistle")) == 0)
		return ColorBGRA32::Thistle;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Tomato")) == 0)
		return ColorBGRA32::Tomato;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Turquoise")) == 0)
		return ColorBGRA32::Turquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Violet")) == 0)
		return ColorBGRA32::Violet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Wheat")) == 0)
		return ColorBGRA32::Wheat;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("White")) == 0)
		return ColorBGRA32::White;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("WhiteSmoke")) == 0)
		return ColorBGRA32::WhiteSmoke;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Yellow")) == 0)
		return ColorBGRA32::Yellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("YellowGreen")) == 0)
		return ColorBGRA32::YellowGreen;

  return ColorBGRA32::Black;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String ColorBGRA32::ToString() CONST
{
  return String::Format(TEXT("(%d,%d,%d,%d)"), m_V[0], m_V[1], m_V[2], m_V[3]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_LUA)
Lua::Bind::Scope ColorBGRA32::GenerateBindings(LPCTSTR Name)
{
  using namespace Lua::Bind;

  return 
    Class<Graphics::ColorBGRA32>(Name)
      .Def(Constructor<>())
      .Def(Constructor<CONST Graphics::ColorBGRA32&>())
      .Def(Constructor<BYTE,BYTE,BYTE,BYTE>())
      .Def(Constructor<BYTE,BYTE,BYTE>())

      .Property("R", &ColorBGRA32::GetR, &ColorBGRA32::SetR)
      .Property("G", &ColorBGRA32::GetG, &ColorBGRA32::SetG)
      .Property("B", &ColorBGRA32::GetB, &ColorBGRA32::SetB)
      .Property("A", &ColorBGRA32::GetA, &ColorBGRA32::SetA)

      .Property("Luminance", &ColorBGRA32::Luminance)

      .Def("Clamp", (VOID(ColorBGRA32::*)())&ColorBGRA32::Clamp)
      .Def("Clamp", (VOID(ColorBGRA32::*)(BYTE,BYTE))&ColorBGRA32::Clamp)

      .Def(tostring(Self))
      .Def(Self + ColorBGRA32())
      .Def(Self - ColorBGRA32())
      .Def(Self * FLOAT())
      .Def(Self / FLOAT())
      
      .Scope[ Def("FromHSV", &ColorBGRA32::FromHSV) ]
      .Scope[ Def("FromName", &ColorBGRA32::FromName) ];

}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------
CONST ColorBGRA32 ColorBGRA32::Transparent = ColorBGRA32(255, 255, 255, 0);
CONST ColorBGRA32 ColorBGRA32::AliceBlue = ColorBGRA32(240, 248, 255, 255);
CONST ColorBGRA32 ColorBGRA32::AntiqueWhite = ColorBGRA32(250, 235, 215, 255);
CONST ColorBGRA32 ColorBGRA32::Aqua = ColorBGRA32(0, 255, 255, 255);
CONST ColorBGRA32 ColorBGRA32::Aquamarine = ColorBGRA32(127, 255, 212, 255);
CONST ColorBGRA32 ColorBGRA32::Azure = ColorBGRA32(240, 255, 255, 255);
CONST ColorBGRA32 ColorBGRA32::Beige = ColorBGRA32(245, 245, 220, 255);
CONST ColorBGRA32 ColorBGRA32::Bisque = ColorBGRA32(255, 228, 196, 255);
CONST ColorBGRA32 ColorBGRA32::Black = ColorBGRA32(0, 0, 0, 255);
CONST ColorBGRA32 ColorBGRA32::BlanchedAlmond = ColorBGRA32(255, 235, 205, 255);
CONST ColorBGRA32 ColorBGRA32::Blue = ColorBGRA32(0, 0, 255, 255);
CONST ColorBGRA32 ColorBGRA32::BlueViolet = ColorBGRA32(138, 43, 226, 255);
CONST ColorBGRA32 ColorBGRA32::Brown = ColorBGRA32(165, 42, 42, 255);
CONST ColorBGRA32 ColorBGRA32::BurlyWood = ColorBGRA32(222, 184, 135, 255);
CONST ColorBGRA32 ColorBGRA32::CadetBlue = ColorBGRA32(95, 158, 160, 255);
CONST ColorBGRA32 ColorBGRA32::Chartreuse = ColorBGRA32(127, 255, 0, 255);
CONST ColorBGRA32 ColorBGRA32::Chocolate = ColorBGRA32(210, 105, 30, 255);
CONST ColorBGRA32 ColorBGRA32::Coral = ColorBGRA32(255, 127, 80, 255);
CONST ColorBGRA32 ColorBGRA32::CornflowerBlue = ColorBGRA32(100, 149, 237, 255);
CONST ColorBGRA32 ColorBGRA32::Cornsilk = ColorBGRA32(255, 248, 220, 255);
CONST ColorBGRA32 ColorBGRA32::Crimson = ColorBGRA32(220, 20, 60, 255);
CONST ColorBGRA32 ColorBGRA32::Cyan = ColorBGRA32(0, 255, 255, 255);
CONST ColorBGRA32 ColorBGRA32::DarkBlue = ColorBGRA32(0, 0, 139, 255);
CONST ColorBGRA32 ColorBGRA32::DarkCyan = ColorBGRA32(0, 139, 139, 255);
CONST ColorBGRA32 ColorBGRA32::DarkGoldenrod = ColorBGRA32(184, 134, 11, 255);
CONST ColorBGRA32 ColorBGRA32::DarkGray = ColorBGRA32(169, 169, 169, 255);
CONST ColorBGRA32 ColorBGRA32::DarkGreen = ColorBGRA32(0, 100, 0, 255);
CONST ColorBGRA32 ColorBGRA32::DarkKhaki = ColorBGRA32(189, 183, 107, 255);
CONST ColorBGRA32 ColorBGRA32::DarkMagenta = ColorBGRA32(139, 0, 139, 255);
CONST ColorBGRA32 ColorBGRA32::DarkOliveGreen = ColorBGRA32(85, 107, 47, 255);
CONST ColorBGRA32 ColorBGRA32::DarkOrange = ColorBGRA32(255, 140, 0, 255);
CONST ColorBGRA32 ColorBGRA32::DarkOrchid = ColorBGRA32(153, 50, 204, 255);
CONST ColorBGRA32 ColorBGRA32::DarkRed = ColorBGRA32(139, 0, 0, 255);
CONST ColorBGRA32 ColorBGRA32::DarkSalmon = ColorBGRA32(233, 150, 122, 255);
CONST ColorBGRA32 ColorBGRA32::DarkSeaGreen = ColorBGRA32(143, 188, 139, 255);
CONST ColorBGRA32 ColorBGRA32::DarkSlateBlue = ColorBGRA32(72, 61, 139, 255);
CONST ColorBGRA32 ColorBGRA32::DarkSlateGray = ColorBGRA32(47, 79, 79, 255);
CONST ColorBGRA32 ColorBGRA32::DarkTurquoise = ColorBGRA32(0, 206, 209, 255);
CONST ColorBGRA32 ColorBGRA32::DarkViolet = ColorBGRA32(148, 0, 211, 255);
CONST ColorBGRA32 ColorBGRA32::DeepPink = ColorBGRA32(255, 20, 147, 255);
CONST ColorBGRA32 ColorBGRA32::DeepSkyBlue = ColorBGRA32(0, 191, 255, 255);
CONST ColorBGRA32 ColorBGRA32::DimGray = ColorBGRA32(105, 105, 105, 255);
CONST ColorBGRA32 ColorBGRA32::DodgerBlue = ColorBGRA32(30, 144, 255, 255);
CONST ColorBGRA32 ColorBGRA32::Firebrick = ColorBGRA32(178, 34, 34, 255);
CONST ColorBGRA32 ColorBGRA32::FloralWhite = ColorBGRA32(255, 250, 240, 255);
CONST ColorBGRA32 ColorBGRA32::ForestGreen = ColorBGRA32(34, 139, 34, 255);
CONST ColorBGRA32 ColorBGRA32::Fuchsia = ColorBGRA32(255, 0, 255, 255);
CONST ColorBGRA32 ColorBGRA32::Gainsboro = ColorBGRA32(220, 220, 220, 255);
CONST ColorBGRA32 ColorBGRA32::GhostWhite = ColorBGRA32(248, 248, 255, 255);
CONST ColorBGRA32 ColorBGRA32::Gold = ColorBGRA32(255, 215, 0, 255);
CONST ColorBGRA32 ColorBGRA32::Goldenrod = ColorBGRA32(218, 165, 32, 255);
CONST ColorBGRA32 ColorBGRA32::Gray = ColorBGRA32(128, 128, 128, 255);
CONST ColorBGRA32 ColorBGRA32::Green = ColorBGRA32(0, 128, 0, 255);
CONST ColorBGRA32 ColorBGRA32::GreenYellow = ColorBGRA32(173, 255, 47, 255);
CONST ColorBGRA32 ColorBGRA32::Honeydew = ColorBGRA32(240, 255, 240, 255);
CONST ColorBGRA32 ColorBGRA32::HotPink = ColorBGRA32(255, 105, 180, 255);
CONST ColorBGRA32 ColorBGRA32::IndianRed = ColorBGRA32(205, 92, 92, 255);
CONST ColorBGRA32 ColorBGRA32::Indigo = ColorBGRA32(75, 0, 130, 255);
CONST ColorBGRA32 ColorBGRA32::Ivory = ColorBGRA32(255, 255, 240, 255);
CONST ColorBGRA32 ColorBGRA32::Khaki = ColorBGRA32(240, 230, 140, 255);
CONST ColorBGRA32 ColorBGRA32::Lavender = ColorBGRA32(230, 230, 250, 255);
CONST ColorBGRA32 ColorBGRA32::LavenderBlush = ColorBGRA32(255, 240, 245, 255);
CONST ColorBGRA32 ColorBGRA32::LawnGreen = ColorBGRA32(124, 252, 0, 255);
CONST ColorBGRA32 ColorBGRA32::LemonChiffon = ColorBGRA32(255, 250, 205, 255);
CONST ColorBGRA32 ColorBGRA32::LightBlue = ColorBGRA32(173, 216, 230, 255);
CONST ColorBGRA32 ColorBGRA32::LightCoral = ColorBGRA32(240, 128, 128, 255);
CONST ColorBGRA32 ColorBGRA32::LightCyan = ColorBGRA32(224, 255, 255, 255);
CONST ColorBGRA32 ColorBGRA32::LightGoldenrodYellow = ColorBGRA32(250, 250, 210, 255);
CONST ColorBGRA32 ColorBGRA32::LightGreen = ColorBGRA32(144, 238, 144, 255);
CONST ColorBGRA32 ColorBGRA32::LightGray = ColorBGRA32(211, 211, 211, 255);
CONST ColorBGRA32 ColorBGRA32::LightPink = ColorBGRA32(255, 182, 193, 255);
CONST ColorBGRA32 ColorBGRA32::LightSalmon = ColorBGRA32(255, 160, 122, 255);
CONST ColorBGRA32 ColorBGRA32::LightSeaGreen = ColorBGRA32(32, 178, 170, 255);
CONST ColorBGRA32 ColorBGRA32::LightSkyBlue = ColorBGRA32(135, 206, 250, 255);
CONST ColorBGRA32 ColorBGRA32::LightSlateGray = ColorBGRA32(119, 136, 153, 255);
CONST ColorBGRA32 ColorBGRA32::LightSteelBlue = ColorBGRA32(176, 196, 222, 255);
CONST ColorBGRA32 ColorBGRA32::LightYellow = ColorBGRA32(255, 255, 224, 255);
CONST ColorBGRA32 ColorBGRA32::Lime = ColorBGRA32(0, 255, 0, 255);
CONST ColorBGRA32 ColorBGRA32::LimeGreen = ColorBGRA32(50, 205, 50, 255);
CONST ColorBGRA32 ColorBGRA32::Linen = ColorBGRA32(250, 240, 230, 255);
CONST ColorBGRA32 ColorBGRA32::Magenta = ColorBGRA32(255, 0, 255, 255);
CONST ColorBGRA32 ColorBGRA32::Maroon = ColorBGRA32(128, 0, 0, 255);
CONST ColorBGRA32 ColorBGRA32::MediumAquamarine = ColorBGRA32(102, 205, 170, 255);
CONST ColorBGRA32 ColorBGRA32::MediumBlue = ColorBGRA32(0, 0, 205, 255);
CONST ColorBGRA32 ColorBGRA32::MediumOrchid = ColorBGRA32(186, 85, 211, 255);
CONST ColorBGRA32 ColorBGRA32::MediumPurple = ColorBGRA32(147, 112, 219, 255);
CONST ColorBGRA32 ColorBGRA32::MediumSeaGreen = ColorBGRA32(60, 179, 113, 255);
CONST ColorBGRA32 ColorBGRA32::MediumSlateBlue = ColorBGRA32(123, 104, 238, 255);
CONST ColorBGRA32 ColorBGRA32::MediumSpringGreen = ColorBGRA32(0, 250, 154, 255);
CONST ColorBGRA32 ColorBGRA32::MediumTurquoise = ColorBGRA32(72, 209, 204, 255);
CONST ColorBGRA32 ColorBGRA32::MediumVioletRed = ColorBGRA32(199, 21, 133, 255);
CONST ColorBGRA32 ColorBGRA32::MidnightBlue = ColorBGRA32(25, 25, 112, 255);
CONST ColorBGRA32 ColorBGRA32::MintCream = ColorBGRA32(245, 255, 250, 255);
CONST ColorBGRA32 ColorBGRA32::MistyRose = ColorBGRA32(255, 228, 225, 255);
CONST ColorBGRA32 ColorBGRA32::Moccasin = ColorBGRA32(255, 228, 181, 255);
CONST ColorBGRA32 ColorBGRA32::NavajoWhite = ColorBGRA32(255, 222, 173, 255);
CONST ColorBGRA32 ColorBGRA32::Navy = ColorBGRA32(0, 0, 128, 255);
CONST ColorBGRA32 ColorBGRA32::OldLace = ColorBGRA32(253, 245, 230, 255);
CONST ColorBGRA32 ColorBGRA32::Olive = ColorBGRA32(128, 128, 0, 255);
CONST ColorBGRA32 ColorBGRA32::OliveDrab = ColorBGRA32(107, 142, 35, 255);
CONST ColorBGRA32 ColorBGRA32::Orange = ColorBGRA32(255, 165, 0, 255);
CONST ColorBGRA32 ColorBGRA32::OrangeRed = ColorBGRA32(255, 69, 0, 255);
CONST ColorBGRA32 ColorBGRA32::Orchid = ColorBGRA32(218, 112, 214, 255);
CONST ColorBGRA32 ColorBGRA32::PaleGoldenrod = ColorBGRA32(238, 232, 170, 255);
CONST ColorBGRA32 ColorBGRA32::PaleGreen = ColorBGRA32(152, 251, 152, 255);
CONST ColorBGRA32 ColorBGRA32::PaleTurquoise = ColorBGRA32(175, 238, 238, 255);
CONST ColorBGRA32 ColorBGRA32::PaleVioletRed = ColorBGRA32(219, 112, 147, 255);
CONST ColorBGRA32 ColorBGRA32::PapayaWhip = ColorBGRA32(255, 239, 213, 255);
CONST ColorBGRA32 ColorBGRA32::PeachPuff = ColorBGRA32(255, 218, 185, 255);
CONST ColorBGRA32 ColorBGRA32::Peru = ColorBGRA32(205, 133, 63, 255);
CONST ColorBGRA32 ColorBGRA32::Pink = ColorBGRA32(255, 192, 203, 255);
CONST ColorBGRA32 ColorBGRA32::Plum = ColorBGRA32(221, 160, 221, 255);
CONST ColorBGRA32 ColorBGRA32::PowderBlue = ColorBGRA32(176, 224, 230, 255);
CONST ColorBGRA32 ColorBGRA32::Purple = ColorBGRA32(128, 0, 128, 255);
CONST ColorBGRA32 ColorBGRA32::Red = ColorBGRA32(255, 0, 0, 255);
CONST ColorBGRA32 ColorBGRA32::RosyBrown = ColorBGRA32(188, 143, 143, 255);
CONST ColorBGRA32 ColorBGRA32::RoyalBlue = ColorBGRA32(65, 105, 225, 255);
CONST ColorBGRA32 ColorBGRA32::SaddleBrown = ColorBGRA32(139, 69, 19, 255);
CONST ColorBGRA32 ColorBGRA32::Salmon = ColorBGRA32(250, 128, 114, 255);
CONST ColorBGRA32 ColorBGRA32::SandyBrown = ColorBGRA32(244, 164, 96, 255);
CONST ColorBGRA32 ColorBGRA32::SeaGreen = ColorBGRA32(46, 139, 87, 255);
CONST ColorBGRA32 ColorBGRA32::SeaShell = ColorBGRA32(255, 245, 238, 255);
CONST ColorBGRA32 ColorBGRA32::Sienna = ColorBGRA32(160, 82, 45, 255);
CONST ColorBGRA32 ColorBGRA32::Silver = ColorBGRA32(192, 192, 192, 255);
CONST ColorBGRA32 ColorBGRA32::SkyBlue = ColorBGRA32(135, 206, 235, 255);
CONST ColorBGRA32 ColorBGRA32::SlateBlue = ColorBGRA32(106, 90, 205, 255);
CONST ColorBGRA32 ColorBGRA32::SlateGray = ColorBGRA32(112, 128, 144, 255);
CONST ColorBGRA32 ColorBGRA32::Snow = ColorBGRA32(255, 250, 250, 255);
CONST ColorBGRA32 ColorBGRA32::SpringGreen = ColorBGRA32(0, 255, 127, 255);
CONST ColorBGRA32 ColorBGRA32::SteelBlue = ColorBGRA32(70, 130, 180, 255);
CONST ColorBGRA32 ColorBGRA32::Tan = ColorBGRA32(210, 180, 140, 255);
CONST ColorBGRA32 ColorBGRA32::Teal = ColorBGRA32(0, 128, 128, 255);
CONST ColorBGRA32 ColorBGRA32::Thistle = ColorBGRA32(216, 191, 216, 255);
CONST ColorBGRA32 ColorBGRA32::Tomato = ColorBGRA32(255, 99, 71, 255);
CONST ColorBGRA32 ColorBGRA32::Turquoise = ColorBGRA32(64, 224, 208, 255);
CONST ColorBGRA32 ColorBGRA32::Violet = ColorBGRA32(238, 130, 238, 255);
CONST ColorBGRA32 ColorBGRA32::Wheat = ColorBGRA32(245, 222, 179, 255);
CONST ColorBGRA32 ColorBGRA32::White = ColorBGRA32(255, 255, 255, 255);
CONST ColorBGRA32 ColorBGRA32::WhiteSmoke = ColorBGRA32(245, 245, 245, 255);
CONST ColorBGRA32 ColorBGRA32::Yellow = ColorBGRA32(255, 255, 0, 255);
CONST ColorBGRA32 ColorBGRA32::YellowGreen = ColorBGRA32(154, 205, 50, 255);
//-----------------------------------------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR32::ColorBGR32()
{
  m_V[0] = 0;
  m_V[1] = 0;
  m_V[2] = 0;
  m_V[3] = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR32::ColorBGR32(BYTE R, BYTE G, BYTE B)
{
  m_V[0] = R;
  m_V[1] = G;
  m_V[2] = B;
  m_V[3] = 255;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR32::ColorBGR32(CONST ColorBGR32& C)
{
  m_V[0] = C.m_V[0];
  m_V[1] = C.m_V[1];
  m_V[2] = C.m_V[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_DIRECTX9)
ColorBGR32::operator D3DCOLOR() CONST
{
  return D3DCOLOR_XRGB(m_V[0], m_V[1], m_V[2]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR32::operator D3DCOLORVALUE() CONST
{
  D3DCOLORVALUE V;
  V.r = m_V[0] / 255.0f;
  V.g = m_V[1] / 255.0f;
  V.b = m_V[2] / 255.0f;
  V.a = 1.0f;

  return V;
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ColorBGR32::Clamp()
{
  Clamp(0, 255);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ColorBGR32::Clamp(BYTE Min, BYTE Max)
{
  m_V[0] = (m_V[0] > Max) ? Max : m_V[0];
  m_V[1] = (m_V[1] > Max) ? Max : m_V[1];
  m_V[2] = (m_V[2] > Max) ? Max : m_V[2];

  m_V[0] = (m_V[0] < Min) ? Min : m_V[0];
  m_V[1] = (m_V[1] < Min) ? Min : m_V[1];
  m_V[2] = (m_V[2] < Min) ? Min : m_V[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR32 ColorBGR32::operator + (CONST ColorBGR32& C) CONST
{
  return ColorBGR32(m_V[0] + C.m_V[0], m_V[1] + C.m_V[1], m_V[2] + C.m_V[2]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR32 ColorBGR32::operator - (CONST ColorBGR32& C) CONST
{
  return ColorBGR32(m_V[0] - C.m_V[0], m_V[1] - C.m_V[1], m_V[2] - C.m_V[2]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR32& ColorBGR32::operator += (CONST ColorBGR32& C)
{
  m_V[0] += C.m_V[0];
  m_V[1] += C.m_V[1];
  m_V[2] += C.m_V[2];
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR32& ColorBGR32::operator -= (CONST ColorBGR32& C)
{
  m_V[0] += C.m_V[0];
  m_V[1] += C.m_V[1];
  m_V[2] += C.m_V[2];
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR32 ColorBGR32::operator * (FLOAT V) CONST
{
  return ColorBGR32((BYTE)(m_V[0]*V), (BYTE)(m_V[1]*V), (BYTE)(m_V[2]*V));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR32 ColorBGR32::operator / (FLOAT V) CONST
{
  return ColorBGR32((BYTE)(m_V[0]/V), (BYTE)(m_V[1]/V), (BYTE)(m_V[2]/V));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR32& ColorBGR32::operator *= (FLOAT V)
{
  m_V[0] = (BYTE)(m_V[0]*V);
  m_V[1] = (BYTE)(m_V[1]*V);
  m_V[2] = (BYTE)(m_V[2]*V);
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR32& ColorBGR32::operator /= (FLOAT V)
{
  m_V[0] = (BYTE)(m_V[0]/V);
  m_V[1] = (BYTE)(m_V[1]/V);
  m_V[2] = (BYTE)(m_V[2]/V);
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT ColorBGR32::Luminance() CONST
{
  return 0.299f*m_V[0] + 0.587f*m_V[1] + 0.114f*m_V[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR32 ColorBGR32::FromHSV(FLOAT Hue, FLOAT Saturation, FLOAT Value)
{
	if (Saturation <= 0.0f)
		return ColorBGR32((BYTE)(Value*255),(BYTE)(Value*255),(BYTE)(Value*255));
  Hue = Math::Mod(Hue, 2.0f*Math::PI);
	if (Hue < 0.0)
    Hue += 2.0f*Math::PI;
	
	Hue /= Math::PI / 3.0f;
	
  int i = int(Math::Floor(Hue));
	
	FLOAT f = Hue - i;
	FLOAT p = Value * (1.0f - Saturation);
	FLOAT q = Value * (1.0f - (Saturation*f));
	FLOAT t = Value * (1.0f - (Saturation*(1.0f-f)));
	
	switch(i) 
	{
	case 0: 
		return ColorBGR32((BYTE)(Value*255), (BYTE)(t*255), (BYTE)(p*255));
	case 1: 
		return ColorBGR32((BYTE)(q*255), (BYTE)(Value*255), (BYTE)(p*255));
	case 2: 
		return ColorBGR32((BYTE)(p*255), (BYTE)(Value*255), (BYTE)(t*255));
	case 3:
		return ColorBGR32((BYTE)(p*255), (BYTE)(q*255), (BYTE)(Value*255));
	case 4:
		return ColorBGR32((BYTE)(t*255), (BYTE)(p*255), (BYTE)(Value*255));
	default: 
		return ColorBGR32((BYTE)(Value*255), (BYTE)(p*255), (BYTE)(q*255));
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR32 ColorBGR32::RandomColor(FLOAT Saturation, FLOAT Value)
{
  FLOAT Hue = UniformRandom<FLOAT>() * 2.0f * PI;
	return FromHSV(Hue, Saturation, Value);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR32 ColorBGR32::FromName(LPCTSTR String)
{
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("AliceBlue")) == 0)
		return ColorBGR32::AliceBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("AntiqueWhite")) == 0)
		return ColorBGR32::AntiqueWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Aqua")) == 0)
		return ColorBGR32::Aqua;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Aquamarine")) == 0)
		return ColorBGR32::Aquamarine;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Azure")) == 0)
		return ColorBGR32::Azure;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Beige")) == 0)
		return ColorBGR32::Beige;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Bisque")) == 0)
		return ColorBGR32::Bisque;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Black")) == 0)
		return ColorBGR32::Black;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BlanchedAlmond")) == 0)
		return ColorBGR32::BlanchedAlmond;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Blue")) == 0)
		return ColorBGR32::Blue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BlueViolet")) == 0)
		return ColorBGR32::BlueViolet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Brown")) == 0)
		return ColorBGR32::Brown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BurlyWood")) == 0)
		return ColorBGR32::BurlyWood;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("CadetBlue")) == 0)
		return ColorBGR32::CadetBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Chartreuse")) == 0)
		return ColorBGR32::Chartreuse;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Chocolate")) == 0)
		return ColorBGR32::Chocolate;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Coral")) == 0)
		return ColorBGR32::Coral;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("CornflowerBlue")) == 0)
		return ColorBGR32::CornflowerBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Cornsilk")) == 0)
		return ColorBGR32::Cornsilk;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Crimson")) == 0)
		return ColorBGR32::Crimson;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Cyan")) == 0)
		return ColorBGR32::Cyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkBlue")) == 0)
		return ColorBGR32::DarkBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkCyan")) == 0)
		return ColorBGR32::DarkCyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGoldenrod")) == 0)
		return ColorBGR32::DarkGoldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGray")) == 0)
		return ColorBGR32::DarkGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGreen")) == 0)
		return ColorBGR32::DarkGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkKhaki")) == 0)
		return ColorBGR32::DarkKhaki;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkMagenta")) == 0)
		return ColorBGR32::DarkMagenta;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOliveGreen")) == 0)
		return ColorBGR32::DarkOliveGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOrange")) == 0)
		return ColorBGR32::DarkOrange;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOrchid")) == 0)
		return ColorBGR32::DarkOrchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkRed")) == 0)
		return ColorBGR32::DarkRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSalmon")) == 0)
		return ColorBGR32::DarkSalmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSeaGreen")) == 0)
		return ColorBGR32::DarkSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSlateBlue")) == 0)
		return ColorBGR32::DarkSlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSlateGray")) == 0)
		return ColorBGR32::DarkSlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkTurquoise")) == 0)
		return ColorBGR32::DarkTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkViolet")) == 0)
		return ColorBGR32::DarkViolet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DeepPink")) == 0)
		return ColorBGR32::DeepPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DeepSkyBlue")) == 0)
		return ColorBGR32::DeepSkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DimGray")) == 0)
		return ColorBGR32::DimGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DodgerBlue")) == 0)
		return ColorBGR32::DodgerBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Firebrick")) == 0)
		return ColorBGR32::Firebrick;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("FloralWhite")) == 0)
		return ColorBGR32::FloralWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("ForestGreen")) == 0)
		return ColorBGR32::ForestGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Fuchsia")) == 0)
		return ColorBGR32::Fuchsia;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gainsboro")) == 0)
		return ColorBGR32::Gainsboro;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("GhostWhite")) == 0)
		return ColorBGR32::GhostWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gold")) == 0)
		return ColorBGR32::Gold;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Goldenrod")) == 0)
		return ColorBGR32::Goldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gray")) == 0)
		return ColorBGR32::Gray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Green")) == 0)
		return ColorBGR32::Green;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("GreenYellow")) == 0)
		return ColorBGR32::GreenYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Honeydew")) == 0)
		return ColorBGR32::Honeydew;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("HotPink")) == 0)
		return ColorBGR32::HotPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("IndianRed")) == 0)
		return ColorBGR32::IndianRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Indigo")) == 0)
		return ColorBGR32::Indigo;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Ivory")) == 0)
		return ColorBGR32::Ivory;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Khaki")) == 0)
		return ColorBGR32::Khaki;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Lavender")) == 0)
		return ColorBGR32::Lavender;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LavenderBlush")) == 0)
		return ColorBGR32::LavenderBlush;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LawnGreen")) == 0)
		return ColorBGR32::LawnGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LemonChiffon")) == 0)
		return ColorBGR32::LemonChiffon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightBlue")) == 0)
		return ColorBGR32::LightBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightCoral")) == 0)
		return ColorBGR32::LightCoral;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightCyan")) == 0)
		return ColorBGR32::LightCyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGoldenrodYellow")) == 0)
		return ColorBGR32::LightGoldenrodYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGreen")) == 0)
		return ColorBGR32::LightGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGray")) == 0)
		return ColorBGR32::LightGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightPink")) == 0)
		return ColorBGR32::LightPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSalmon")) == 0)
		return ColorBGR32::LightSalmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSeaGreen")) == 0)
		return ColorBGR32::LightSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSkyBlue")) == 0)
		return ColorBGR32::LightSkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSlateGray")) == 0)
		return ColorBGR32::LightSlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSteelBlue")) == 0)
		return ColorBGR32::LightSteelBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightYellow")) == 0)
		return ColorBGR32::LightYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Lime")) == 0)
		return ColorBGR32::Lime;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LimeGreen")) == 0)
		return ColorBGR32::LimeGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Linen")) == 0)
		return ColorBGR32::Linen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Magenta")) == 0)
		return ColorBGR32::Magenta;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Maroon")) == 0)
		return ColorBGR32::Maroon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumAquamarine")) == 0)
		return ColorBGR32::MediumAquamarine;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumBlue")) == 0)
		return ColorBGR32::MediumBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumOrchid")) == 0)
		return ColorBGR32::MediumOrchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumPurple")) == 0)
		return ColorBGR32::MediumPurple;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSeaGreen")) == 0)
		return ColorBGR32::MediumSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSlateBlue")) == 0)
		return ColorBGR32::MediumSlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSpringGreen")) == 0)
		return ColorBGR32::MediumSpringGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumTurquoise")) == 0)
		return ColorBGR32::MediumTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumVioletRed")) == 0)
		return ColorBGR32::MediumVioletRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MidnightBlue")) == 0)
		return ColorBGR32::MidnightBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MintCream")) == 0)
		return ColorBGR32::MintCream;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MistyRose")) == 0)
		return ColorBGR32::MistyRose;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Moccasin")) == 0)
		return ColorBGR32::Moccasin;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("NavajoWhite")) == 0)
		return ColorBGR32::NavajoWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Navy")) == 0)
		return ColorBGR32::Navy;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OldLace")) == 0)
		return ColorBGR32::OldLace;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Olive")) == 0)
		return ColorBGR32::Olive;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OliveDrab")) == 0)
		return ColorBGR32::OliveDrab;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Orange")) == 0)
		return ColorBGR32::Orange;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OrangeRed")) == 0)
		return ColorBGR32::OrangeRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Orchid")) == 0)
		return ColorBGR32::Orchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleGoldenrod")) == 0)
		return ColorBGR32::PaleGoldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleGreen")) == 0)
		return ColorBGR32::PaleGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleTurquoise")) == 0)
		return ColorBGR32::PaleTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleVioletRed")) == 0)
		return ColorBGR32::PaleVioletRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PapayaWhip")) == 0)
		return ColorBGR32::PapayaWhip;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PeachPuff")) == 0)
		return ColorBGR32::PeachPuff;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Peru")) == 0)
		return ColorBGR32::Peru;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Pink")) == 0)
		return ColorBGR32::Pink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Plum")) == 0)
		return ColorBGR32::Plum;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PowderBlue")) == 0)
		return ColorBGR32::PowderBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Purple")) == 0)
		return ColorBGR32::Purple;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Red")) == 0)
		return ColorBGR32::Red;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("RosyBrown")) == 0)
		return ColorBGR32::RosyBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("RoyalBlue")) == 0)
		return ColorBGR32::RoyalBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SaddleBrown")) == 0)
		return ColorBGR32::SaddleBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Salmon")) == 0)
		return ColorBGR32::Salmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SandyBrown")) == 0)
		return ColorBGR32::SandyBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SeaGreen")) == 0)
		return ColorBGR32::SeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SeaShell")) == 0)
		return ColorBGR32::SeaShell;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Sienna")) == 0)
		return ColorBGR32::Sienna;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Silver")) == 0)
		return ColorBGR32::Silver;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SkyBlue")) == 0)
		return ColorBGR32::SkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SlateBlue")) == 0)
		return ColorBGR32::SlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SlateGray")) == 0)
		return ColorBGR32::SlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Snow")) == 0)
		return ColorBGR32::Snow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SpringGreen")) == 0)
		return ColorBGR32::SpringGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SteelBlue")) == 0)
		return ColorBGR32::SteelBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Tan")) == 0)
		return ColorBGR32::Tan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Teal")) == 0)
		return ColorBGR32::Teal;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Thistle")) == 0)
		return ColorBGR32::Thistle;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Tomato")) == 0)
		return ColorBGR32::Tomato;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Turquoise")) == 0)
		return ColorBGR32::Turquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Violet")) == 0)
		return ColorBGR32::Violet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Wheat")) == 0)
		return ColorBGR32::Wheat;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("White")) == 0)
		return ColorBGR32::White;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("WhiteSmoke")) == 0)
		return ColorBGR32::WhiteSmoke;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Yellow")) == 0)
		return ColorBGR32::Yellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("YellowGreen")) == 0)
		return ColorBGR32::YellowGreen;

  return ColorBGR32::Black;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String ColorBGR32::ToString() CONST
{
  return String::Format(TEXT("(%d,%d,%d)"), m_V[0], m_V[1], m_V[2]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_LUA)
Lua::Bind::Scope ColorBGR32::GenerateBindings(LPCTSTR Name)
{
  using namespace Lua::Bind;

  return 
    Class<Graphics::ColorBGR32>(Name)
      .Def(Constructor<>())
      .Def(Constructor<CONST Graphics::ColorBGR32&>())
      .Def(Constructor<BYTE,BYTE,BYTE>())

      .Property("R", &ColorBGR32::GetR, &ColorBGR32::SetR)
      .Property("G", &ColorBGR32::GetG, &ColorBGR32::SetG)
      .Property("B", &ColorBGR32::GetB, &ColorBGR32::SetB)

      .Property("Luminance", &ColorBGR32::Luminance)

      .Def("Clamp", (VOID(ColorBGR32::*)())&ColorBGR32::Clamp)
      .Def("Clamp", (VOID(ColorBGR32::*)(BYTE,BYTE))&ColorBGR32::Clamp)

      .Def(tostring(Self))
      .Def(Self + ColorBGR32())
      .Def(Self - ColorBGR32())
      .Def(Self * FLOAT())
      .Def(Self / FLOAT())
      
      .Scope[ Def("FromHSV", &ColorBGR32::FromHSV) ]
      .Scope[ Def("FromName", &ColorBGR32::FromName) ];

}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------
CONST ColorBGR32 ColorBGR32::AliceBlue = ColorBGR32(240, 248, 255);
CONST ColorBGR32 ColorBGR32::AntiqueWhite = ColorBGR32(250, 235, 215);
CONST ColorBGR32 ColorBGR32::Aqua = ColorBGR32(0, 255, 255);
CONST ColorBGR32 ColorBGR32::Aquamarine = ColorBGR32(127, 255, 212);
CONST ColorBGR32 ColorBGR32::Azure = ColorBGR32(240, 255, 255);
CONST ColorBGR32 ColorBGR32::Beige = ColorBGR32(245, 245, 220);
CONST ColorBGR32 ColorBGR32::Bisque = ColorBGR32(255, 228, 196);
CONST ColorBGR32 ColorBGR32::Black = ColorBGR32(0, 0, 0);
CONST ColorBGR32 ColorBGR32::BlanchedAlmond = ColorBGR32(255, 235, 205);
CONST ColorBGR32 ColorBGR32::Blue = ColorBGR32(0, 0, 255);
CONST ColorBGR32 ColorBGR32::BlueViolet = ColorBGR32(138, 43, 226);
CONST ColorBGR32 ColorBGR32::Brown = ColorBGR32(165, 42, 42);
CONST ColorBGR32 ColorBGR32::BurlyWood = ColorBGR32(222, 184, 135);
CONST ColorBGR32 ColorBGR32::CadetBlue = ColorBGR32(95, 158, 160);
CONST ColorBGR32 ColorBGR32::Chartreuse = ColorBGR32(127, 255, 0);
CONST ColorBGR32 ColorBGR32::Chocolate = ColorBGR32(210, 105, 30);
CONST ColorBGR32 ColorBGR32::Coral = ColorBGR32(255, 127, 80);
CONST ColorBGR32 ColorBGR32::CornflowerBlue = ColorBGR32(100, 149, 237);
CONST ColorBGR32 ColorBGR32::Cornsilk = ColorBGR32(255, 248, 220);
CONST ColorBGR32 ColorBGR32::Crimson = ColorBGR32(220, 20, 60);
CONST ColorBGR32 ColorBGR32::Cyan = ColorBGR32(0, 255, 255);
CONST ColorBGR32 ColorBGR32::DarkBlue = ColorBGR32(0, 0, 139);
CONST ColorBGR32 ColorBGR32::DarkCyan = ColorBGR32(0, 139, 139);
CONST ColorBGR32 ColorBGR32::DarkGoldenrod = ColorBGR32(184, 134, 11);
CONST ColorBGR32 ColorBGR32::DarkGray = ColorBGR32(169, 169, 169);
CONST ColorBGR32 ColorBGR32::DarkGreen = ColorBGR32(0, 100, 0);
CONST ColorBGR32 ColorBGR32::DarkKhaki = ColorBGR32(189, 183, 107);
CONST ColorBGR32 ColorBGR32::DarkMagenta = ColorBGR32(139, 0, 139);
CONST ColorBGR32 ColorBGR32::DarkOliveGreen = ColorBGR32(85, 107, 47);
CONST ColorBGR32 ColorBGR32::DarkOrange = ColorBGR32(255, 140, 0);
CONST ColorBGR32 ColorBGR32::DarkOrchid = ColorBGR32(153, 50, 204);
CONST ColorBGR32 ColorBGR32::DarkRed = ColorBGR32(139, 0, 0);
CONST ColorBGR32 ColorBGR32::DarkSalmon = ColorBGR32(233, 150, 122);
CONST ColorBGR32 ColorBGR32::DarkSeaGreen = ColorBGR32(143, 188, 139);
CONST ColorBGR32 ColorBGR32::DarkSlateBlue = ColorBGR32(72, 61, 139);
CONST ColorBGR32 ColorBGR32::DarkSlateGray = ColorBGR32(47, 79, 79);
CONST ColorBGR32 ColorBGR32::DarkTurquoise = ColorBGR32(0, 206, 209);
CONST ColorBGR32 ColorBGR32::DarkViolet = ColorBGR32(148, 0, 211);
CONST ColorBGR32 ColorBGR32::DeepPink = ColorBGR32(255, 20, 147);
CONST ColorBGR32 ColorBGR32::DeepSkyBlue = ColorBGR32(0, 191, 255);
CONST ColorBGR32 ColorBGR32::DimGray = ColorBGR32(105, 105, 105);
CONST ColorBGR32 ColorBGR32::DodgerBlue = ColorBGR32(30, 144, 255);
CONST ColorBGR32 ColorBGR32::Firebrick = ColorBGR32(178, 34, 34);
CONST ColorBGR32 ColorBGR32::FloralWhite = ColorBGR32(255, 250, 240);
CONST ColorBGR32 ColorBGR32::ForestGreen = ColorBGR32(34, 139, 34);
CONST ColorBGR32 ColorBGR32::Fuchsia = ColorBGR32(255, 0, 255);
CONST ColorBGR32 ColorBGR32::Gainsboro = ColorBGR32(220, 220, 220);
CONST ColorBGR32 ColorBGR32::GhostWhite = ColorBGR32(248, 248, 255);
CONST ColorBGR32 ColorBGR32::Gold = ColorBGR32(255, 215, 0);
CONST ColorBGR32 ColorBGR32::Goldenrod = ColorBGR32(218, 165, 32);
CONST ColorBGR32 ColorBGR32::Gray = ColorBGR32(128, 128, 128);
CONST ColorBGR32 ColorBGR32::Green = ColorBGR32(0, 128, 0);
CONST ColorBGR32 ColorBGR32::GreenYellow = ColorBGR32(173, 255, 47);
CONST ColorBGR32 ColorBGR32::Honeydew = ColorBGR32(240, 255, 240);
CONST ColorBGR32 ColorBGR32::HotPink = ColorBGR32(255, 105, 180);
CONST ColorBGR32 ColorBGR32::IndianRed = ColorBGR32(205, 92, 92);
CONST ColorBGR32 ColorBGR32::Indigo = ColorBGR32(75, 0, 130);
CONST ColorBGR32 ColorBGR32::Ivory = ColorBGR32(255, 255, 240);
CONST ColorBGR32 ColorBGR32::Khaki = ColorBGR32(240, 230, 140);
CONST ColorBGR32 ColorBGR32::Lavender = ColorBGR32(230, 230, 250);
CONST ColorBGR32 ColorBGR32::LavenderBlush = ColorBGR32(255, 240, 245);
CONST ColorBGR32 ColorBGR32::LawnGreen = ColorBGR32(124, 252, 0);
CONST ColorBGR32 ColorBGR32::LemonChiffon = ColorBGR32(255, 250, 205);
CONST ColorBGR32 ColorBGR32::LightBlue = ColorBGR32(173, 216, 230);
CONST ColorBGR32 ColorBGR32::LightCoral = ColorBGR32(240, 128, 128);
CONST ColorBGR32 ColorBGR32::LightCyan = ColorBGR32(224, 255, 255);
CONST ColorBGR32 ColorBGR32::LightGoldenrodYellow = ColorBGR32(250, 250, 210);
CONST ColorBGR32 ColorBGR32::LightGreen = ColorBGR32(144, 238, 144);
CONST ColorBGR32 ColorBGR32::LightGray = ColorBGR32(211, 211, 211);
CONST ColorBGR32 ColorBGR32::LightPink = ColorBGR32(255, 182, 193);
CONST ColorBGR32 ColorBGR32::LightSalmon = ColorBGR32(255, 160, 122);
CONST ColorBGR32 ColorBGR32::LightSeaGreen = ColorBGR32(32, 178, 170);
CONST ColorBGR32 ColorBGR32::LightSkyBlue = ColorBGR32(135, 206, 250);
CONST ColorBGR32 ColorBGR32::LightSlateGray = ColorBGR32(119, 136, 153);
CONST ColorBGR32 ColorBGR32::LightSteelBlue = ColorBGR32(176, 196, 222);
CONST ColorBGR32 ColorBGR32::LightYellow = ColorBGR32(255, 255, 224);
CONST ColorBGR32 ColorBGR32::Lime = ColorBGR32(0, 255, 0);
CONST ColorBGR32 ColorBGR32::LimeGreen = ColorBGR32(50, 205, 50);
CONST ColorBGR32 ColorBGR32::Linen = ColorBGR32(250, 240, 230);
CONST ColorBGR32 ColorBGR32::Magenta = ColorBGR32(255, 0, 255);
CONST ColorBGR32 ColorBGR32::Maroon = ColorBGR32(128, 0, 0);
CONST ColorBGR32 ColorBGR32::MediumAquamarine = ColorBGR32(102, 205, 170);
CONST ColorBGR32 ColorBGR32::MediumBlue = ColorBGR32(0, 0, 205);
CONST ColorBGR32 ColorBGR32::MediumOrchid = ColorBGR32(186, 85, 211);
CONST ColorBGR32 ColorBGR32::MediumPurple = ColorBGR32(147, 112, 219);
CONST ColorBGR32 ColorBGR32::MediumSeaGreen = ColorBGR32(60, 179, 113);
CONST ColorBGR32 ColorBGR32::MediumSlateBlue = ColorBGR32(123, 104, 238);
CONST ColorBGR32 ColorBGR32::MediumSpringGreen = ColorBGR32(0, 250, 154);
CONST ColorBGR32 ColorBGR32::MediumTurquoise = ColorBGR32(72, 209, 204);
CONST ColorBGR32 ColorBGR32::MediumVioletRed = ColorBGR32(199, 21, 133);
CONST ColorBGR32 ColorBGR32::MidnightBlue = ColorBGR32(25, 25, 112);
CONST ColorBGR32 ColorBGR32::MintCream = ColorBGR32(245, 255, 250);
CONST ColorBGR32 ColorBGR32::MistyRose = ColorBGR32(255, 228, 225);
CONST ColorBGR32 ColorBGR32::Moccasin = ColorBGR32(255, 228, 181);
CONST ColorBGR32 ColorBGR32::NavajoWhite = ColorBGR32(255, 222, 173);
CONST ColorBGR32 ColorBGR32::Navy = ColorBGR32(0, 0, 128);
CONST ColorBGR32 ColorBGR32::OldLace = ColorBGR32(253, 245, 230);
CONST ColorBGR32 ColorBGR32::Olive = ColorBGR32(128, 128, 0);
CONST ColorBGR32 ColorBGR32::OliveDrab = ColorBGR32(107, 142, 35);
CONST ColorBGR32 ColorBGR32::Orange = ColorBGR32(255, 165, 0);
CONST ColorBGR32 ColorBGR32::OrangeRed = ColorBGR32(255, 69, 0);
CONST ColorBGR32 ColorBGR32::Orchid = ColorBGR32(218, 112, 214);
CONST ColorBGR32 ColorBGR32::PaleGoldenrod = ColorBGR32(238, 232, 170);
CONST ColorBGR32 ColorBGR32::PaleGreen = ColorBGR32(152, 251, 152);
CONST ColorBGR32 ColorBGR32::PaleTurquoise = ColorBGR32(175, 238, 238);
CONST ColorBGR32 ColorBGR32::PaleVioletRed = ColorBGR32(219, 112, 147);
CONST ColorBGR32 ColorBGR32::PapayaWhip = ColorBGR32(255, 239, 213);
CONST ColorBGR32 ColorBGR32::PeachPuff = ColorBGR32(255, 218, 185);
CONST ColorBGR32 ColorBGR32::Peru = ColorBGR32(205, 133, 63);
CONST ColorBGR32 ColorBGR32::Pink = ColorBGR32(255, 192, 203);
CONST ColorBGR32 ColorBGR32::Plum = ColorBGR32(221, 160, 221);
CONST ColorBGR32 ColorBGR32::PowderBlue = ColorBGR32(176, 224, 230);
CONST ColorBGR32 ColorBGR32::Purple = ColorBGR32(128, 0, 128);
CONST ColorBGR32 ColorBGR32::Red = ColorBGR32(255, 0, 0);
CONST ColorBGR32 ColorBGR32::RosyBrown = ColorBGR32(188, 143, 143);
CONST ColorBGR32 ColorBGR32::RoyalBlue = ColorBGR32(65, 105, 225);
CONST ColorBGR32 ColorBGR32::SaddleBrown = ColorBGR32(139, 69, 19);
CONST ColorBGR32 ColorBGR32::Salmon = ColorBGR32(250, 128, 114);
CONST ColorBGR32 ColorBGR32::SandyBrown = ColorBGR32(244, 164, 96);
CONST ColorBGR32 ColorBGR32::SeaGreen = ColorBGR32(46, 139, 87);
CONST ColorBGR32 ColorBGR32::SeaShell = ColorBGR32(255, 245, 238);
CONST ColorBGR32 ColorBGR32::Sienna = ColorBGR32(160, 82, 45);
CONST ColorBGR32 ColorBGR32::Silver = ColorBGR32(192, 192, 192);
CONST ColorBGR32 ColorBGR32::SkyBlue = ColorBGR32(135, 206, 235);
CONST ColorBGR32 ColorBGR32::SlateBlue = ColorBGR32(106, 90, 205);
CONST ColorBGR32 ColorBGR32::SlateGray = ColorBGR32(112, 128, 144);
CONST ColorBGR32 ColorBGR32::Snow = ColorBGR32(255, 250, 250);
CONST ColorBGR32 ColorBGR32::SpringGreen = ColorBGR32(0, 255, 127);
CONST ColorBGR32 ColorBGR32::SteelBlue = ColorBGR32(70, 130, 180);
CONST ColorBGR32 ColorBGR32::Tan = ColorBGR32(210, 180, 140);
CONST ColorBGR32 ColorBGR32::Teal = ColorBGR32(0, 128, 128);
CONST ColorBGR32 ColorBGR32::Thistle = ColorBGR32(216, 191, 216);
CONST ColorBGR32 ColorBGR32::Tomato = ColorBGR32(255, 99, 71);
CONST ColorBGR32 ColorBGR32::Turquoise = ColorBGR32(64, 224, 208);
CONST ColorBGR32 ColorBGR32::Violet = ColorBGR32(238, 130, 238);
CONST ColorBGR32 ColorBGR32::Wheat = ColorBGR32(245, 222, 179);
CONST ColorBGR32 ColorBGR32::White = ColorBGR32(255, 255, 255);
CONST ColorBGR32 ColorBGR32::WhiteSmoke = ColorBGR32(245, 245, 245);
CONST ColorBGR32 ColorBGR32::Yellow = ColorBGR32(255, 255, 0);
CONST ColorBGR32 ColorBGR32::YellowGreen = ColorBGR32(154, 205, 50);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR24::ColorBGR24()
{
  m_V[0] = 0;
  m_V[1] = 0;
  m_V[2] = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR24::ColorBGR24(BYTE R, BYTE G, BYTE B)
{
  m_V[0] = R;
  m_V[1] = G;
  m_V[2] = B;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR24::ColorBGR24(CONST ColorBGR24& C)
{
  m_V[0] = C.m_V[0];
  m_V[1] = C.m_V[1];
  m_V[2] = C.m_V[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_DIRECTX9)
ColorBGR24::operator D3DCOLOR() CONST
{
  return D3DCOLOR_XRGB(m_V[0], m_V[1], m_V[2]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR24::operator D3DCOLORVALUE() CONST
{
  D3DCOLORVALUE V;
  V.r = m_V[0] / 255.0f;
  V.g = m_V[1] / 255.0f;
  V.b = m_V[2] / 255.0f;
  V.a = 1.0f;

  return V;
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ColorBGR24::Clamp()
{
  Clamp(0, 255);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ColorBGR24::Clamp(BYTE Min, BYTE Max)
{
  m_V[0] = (m_V[0] > Max) ? Max : m_V[0];
  m_V[1] = (m_V[1] > Max) ? Max : m_V[1];
  m_V[2] = (m_V[2] > Max) ? Max : m_V[2];

  m_V[0] = (m_V[0] < Min) ? Min : m_V[0];
  m_V[1] = (m_V[1] < Min) ? Min : m_V[1];
  m_V[2] = (m_V[2] < Min) ? Min : m_V[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR24 ColorBGR24::operator + (CONST ColorBGR24& C) CONST
{
  return ColorBGR24(m_V[0] + C.m_V[0], m_V[1] + C.m_V[1], m_V[2] + C.m_V[2]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR24 ColorBGR24::operator - (CONST ColorBGR24& C) CONST
{
  return ColorBGR24(m_V[0] - C.m_V[0], m_V[1] - C.m_V[1], m_V[2] - C.m_V[2]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR24& ColorBGR24::operator += (CONST ColorBGR24& C)
{
  m_V[0] += C.m_V[0];
  m_V[1] += C.m_V[1];
  m_V[2] += C.m_V[2];
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR24& ColorBGR24::operator -= (CONST ColorBGR24& C)
{
  m_V[0] += C.m_V[0];
  m_V[1] += C.m_V[1];
  m_V[2] += C.m_V[2];
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR24 ColorBGR24::operator * (FLOAT V) CONST
{
  return ColorBGR24((BYTE)(m_V[0]*V), (BYTE)(m_V[1]*V), (BYTE)(m_V[2]*V));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR24 ColorBGR24::operator / (FLOAT V) CONST
{
  return ColorBGR24((BYTE)(m_V[0]/V), (BYTE)(m_V[1]/V), (BYTE)(m_V[2]/V));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR24& ColorBGR24::operator *= (FLOAT V)
{
  m_V[0] = (BYTE)(m_V[0]*V);
  m_V[1] = (BYTE)(m_V[1]*V);
  m_V[2] = (BYTE)(m_V[2]*V);
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR24& ColorBGR24::operator /= (FLOAT V)
{
  m_V[0] = (BYTE)(m_V[0]/V);
  m_V[1] = (BYTE)(m_V[1]/V);
  m_V[2] = (BYTE)(m_V[2]/V);
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT ColorBGR24::Luminance() CONST
{
  return 0.299f*m_V[0] + 0.587f*m_V[1] + 0.114f*m_V[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR24 ColorBGR24::FromHSV(FLOAT Hue, FLOAT Saturation, FLOAT Value)
{
	if (Saturation <= 0.0f)
		return ColorBGR24((BYTE)(Value*255),(BYTE)(Value*255),(BYTE)(Value*255));
  Hue = Math::Mod(Hue, 2.0f*Math::PI);
	if (Hue < 0.0)
    Hue += 2.0f*Math::PI;
	
	Hue /= Math::PI / 3.0f;
	
  int i = int(Math::Floor(Hue));
	
	FLOAT f = Hue - i;
	FLOAT p = Value * (1.0f - Saturation);
	FLOAT q = Value * (1.0f - (Saturation*f));
	FLOAT t = Value * (1.0f - (Saturation*(1.0f-f)));
	
	switch(i) 
	{
	case 0: 
		return ColorBGR24((BYTE)(Value*255), (BYTE)(t*255), (BYTE)(p*255));
	case 1: 
		return ColorBGR24((BYTE)(q*255), (BYTE)(Value*255), (BYTE)(p*255));
	case 2: 
		return ColorBGR24((BYTE)(p*255), (BYTE)(Value*255), (BYTE)(t*255));
	case 3:
		return ColorBGR24((BYTE)(p*255), (BYTE)(q*255), (BYTE)(Value*255));
	case 4:
		return ColorBGR24((BYTE)(t*255), (BYTE)(p*255), (BYTE)(Value*255));
	default: 
		return ColorBGR24((BYTE)(Value*255), (BYTE)(p*255), (BYTE)(q*255));
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR24 ColorBGR24::RandomColor(FLOAT Saturation, FLOAT Value)
{
  FLOAT Hue = UniformRandom<FLOAT>() * 2.0f * PI;
	return FromHSV(Hue, Saturation, Value);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorBGR24 ColorBGR24::FromName(LPCTSTR String)
{
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("AliceBlue")) == 0)
		return ColorBGR24::AliceBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("AntiqueWhite")) == 0)
		return ColorBGR24::AntiqueWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Aqua")) == 0)
		return ColorBGR24::Aqua;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Aquamarine")) == 0)
		return ColorBGR24::Aquamarine;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Azure")) == 0)
		return ColorBGR24::Azure;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Beige")) == 0)
		return ColorBGR24::Beige;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Bisque")) == 0)
		return ColorBGR24::Bisque;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Black")) == 0)
		return ColorBGR24::Black;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BlanchedAlmond")) == 0)
		return ColorBGR24::BlanchedAlmond;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Blue")) == 0)
		return ColorBGR24::Blue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BlueViolet")) == 0)
		return ColorBGR24::BlueViolet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Brown")) == 0)
		return ColorBGR24::Brown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BurlyWood")) == 0)
		return ColorBGR24::BurlyWood;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("CadetBlue")) == 0)
		return ColorBGR24::CadetBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Chartreuse")) == 0)
		return ColorBGR24::Chartreuse;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Chocolate")) == 0)
		return ColorBGR24::Chocolate;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Coral")) == 0)
		return ColorBGR24::Coral;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("CornflowerBlue")) == 0)
		return ColorBGR24::CornflowerBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Cornsilk")) == 0)
		return ColorBGR24::Cornsilk;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Crimson")) == 0)
		return ColorBGR24::Crimson;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Cyan")) == 0)
		return ColorBGR24::Cyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkBlue")) == 0)
		return ColorBGR24::DarkBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkCyan")) == 0)
		return ColorBGR24::DarkCyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGoldenrod")) == 0)
		return ColorBGR24::DarkGoldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGray")) == 0)
		return ColorBGR24::DarkGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGreen")) == 0)
		return ColorBGR24::DarkGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkKhaki")) == 0)
		return ColorBGR24::DarkKhaki;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkMagenta")) == 0)
		return ColorBGR24::DarkMagenta;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOliveGreen")) == 0)
		return ColorBGR24::DarkOliveGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOrange")) == 0)
		return ColorBGR24::DarkOrange;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOrchid")) == 0)
		return ColorBGR24::DarkOrchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkRed")) == 0)
		return ColorBGR24::DarkRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSalmon")) == 0)
		return ColorBGR24::DarkSalmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSeaGreen")) == 0)
		return ColorBGR24::DarkSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSlateBlue")) == 0)
		return ColorBGR24::DarkSlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSlateGray")) == 0)
		return ColorBGR24::DarkSlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkTurquoise")) == 0)
		return ColorBGR24::DarkTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkViolet")) == 0)
		return ColorBGR24::DarkViolet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DeepPink")) == 0)
		return ColorBGR24::DeepPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DeepSkyBlue")) == 0)
		return ColorBGR24::DeepSkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DimGray")) == 0)
		return ColorBGR24::DimGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DodgerBlue")) == 0)
		return ColorBGR24::DodgerBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Firebrick")) == 0)
		return ColorBGR24::Firebrick;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("FloralWhite")) == 0)
		return ColorBGR24::FloralWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("ForestGreen")) == 0)
		return ColorBGR24::ForestGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Fuchsia")) == 0)
		return ColorBGR24::Fuchsia;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gainsboro")) == 0)
		return ColorBGR24::Gainsboro;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("GhostWhite")) == 0)
		return ColorBGR24::GhostWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gold")) == 0)
		return ColorBGR24::Gold;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Goldenrod")) == 0)
		return ColorBGR24::Goldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gray")) == 0)
		return ColorBGR24::Gray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Green")) == 0)
		return ColorBGR24::Green;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("GreenYellow")) == 0)
		return ColorBGR24::GreenYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Honeydew")) == 0)
		return ColorBGR24::Honeydew;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("HotPink")) == 0)
		return ColorBGR24::HotPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("IndianRed")) == 0)
		return ColorBGR24::IndianRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Indigo")) == 0)
		return ColorBGR24::Indigo;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Ivory")) == 0)
		return ColorBGR24::Ivory;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Khaki")) == 0)
		return ColorBGR24::Khaki;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Lavender")) == 0)
		return ColorBGR24::Lavender;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LavenderBlush")) == 0)
		return ColorBGR24::LavenderBlush;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LawnGreen")) == 0)
		return ColorBGR24::LawnGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LemonChiffon")) == 0)
		return ColorBGR24::LemonChiffon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightBlue")) == 0)
		return ColorBGR24::LightBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightCoral")) == 0)
		return ColorBGR24::LightCoral;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightCyan")) == 0)
		return ColorBGR24::LightCyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGoldenrodYellow")) == 0)
		return ColorBGR24::LightGoldenrodYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGreen")) == 0)
		return ColorBGR24::LightGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGray")) == 0)
		return ColorBGR24::LightGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightPink")) == 0)
		return ColorBGR24::LightPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSalmon")) == 0)
		return ColorBGR24::LightSalmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSeaGreen")) == 0)
		return ColorBGR24::LightSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSkyBlue")) == 0)
		return ColorBGR24::LightSkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSlateGray")) == 0)
		return ColorBGR24::LightSlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSteelBlue")) == 0)
		return ColorBGR24::LightSteelBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightYellow")) == 0)
		return ColorBGR24::LightYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Lime")) == 0)
		return ColorBGR24::Lime;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LimeGreen")) == 0)
		return ColorBGR24::LimeGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Linen")) == 0)
		return ColorBGR24::Linen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Magenta")) == 0)
		return ColorBGR24::Magenta;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Maroon")) == 0)
		return ColorBGR24::Maroon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumAquamarine")) == 0)
		return ColorBGR24::MediumAquamarine;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumBlue")) == 0)
		return ColorBGR24::MediumBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumOrchid")) == 0)
		return ColorBGR24::MediumOrchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumPurple")) == 0)
		return ColorBGR24::MediumPurple;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSeaGreen")) == 0)
		return ColorBGR24::MediumSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSlateBlue")) == 0)
		return ColorBGR24::MediumSlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSpringGreen")) == 0)
		return ColorBGR24::MediumSpringGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumTurquoise")) == 0)
		return ColorBGR24::MediumTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumVioletRed")) == 0)
		return ColorBGR24::MediumVioletRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MidnightBlue")) == 0)
		return ColorBGR24::MidnightBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MintCream")) == 0)
		return ColorBGR24::MintCream;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MistyRose")) == 0)
		return ColorBGR24::MistyRose;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Moccasin")) == 0)
		return ColorBGR24::Moccasin;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("NavajoWhite")) == 0)
		return ColorBGR24::NavajoWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Navy")) == 0)
		return ColorBGR24::Navy;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OldLace")) == 0)
		return ColorBGR24::OldLace;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Olive")) == 0)
		return ColorBGR24::Olive;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OliveDrab")) == 0)
		return ColorBGR24::OliveDrab;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Orange")) == 0)
		return ColorBGR24::Orange;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OrangeRed")) == 0)
		return ColorBGR24::OrangeRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Orchid")) == 0)
		return ColorBGR24::Orchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleGoldenrod")) == 0)
		return ColorBGR24::PaleGoldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleGreen")) == 0)
		return ColorBGR24::PaleGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleTurquoise")) == 0)
		return ColorBGR24::PaleTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleVioletRed")) == 0)
		return ColorBGR24::PaleVioletRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PapayaWhip")) == 0)
		return ColorBGR24::PapayaWhip;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PeachPuff")) == 0)
		return ColorBGR24::PeachPuff;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Peru")) == 0)
		return ColorBGR24::Peru;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Pink")) == 0)
		return ColorBGR24::Pink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Plum")) == 0)
		return ColorBGR24::Plum;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PowderBlue")) == 0)
		return ColorBGR24::PowderBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Purple")) == 0)
		return ColorBGR24::Purple;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Red")) == 0)
		return ColorBGR24::Red;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("RosyBrown")) == 0)
		return ColorBGR24::RosyBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("RoyalBlue")) == 0)
		return ColorBGR24::RoyalBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SaddleBrown")) == 0)
		return ColorBGR24::SaddleBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Salmon")) == 0)
		return ColorBGR24::Salmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SandyBrown")) == 0)
		return ColorBGR24::SandyBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SeaGreen")) == 0)
		return ColorBGR24::SeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SeaShell")) == 0)
		return ColorBGR24::SeaShell;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Sienna")) == 0)
		return ColorBGR24::Sienna;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Silver")) == 0)
		return ColorBGR24::Silver;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SkyBlue")) == 0)
		return ColorBGR24::SkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SlateBlue")) == 0)
		return ColorBGR24::SlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SlateGray")) == 0)
		return ColorBGR24::SlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Snow")) == 0)
		return ColorBGR24::Snow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SpringGreen")) == 0)
		return ColorBGR24::SpringGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SteelBlue")) == 0)
		return ColorBGR24::SteelBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Tan")) == 0)
		return ColorBGR24::Tan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Teal")) == 0)
		return ColorBGR24::Teal;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Thistle")) == 0)
		return ColorBGR24::Thistle;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Tomato")) == 0)
		return ColorBGR24::Tomato;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Turquoise")) == 0)
		return ColorBGR24::Turquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Violet")) == 0)
		return ColorBGR24::Violet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Wheat")) == 0)
		return ColorBGR24::Wheat;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("White")) == 0)
		return ColorBGR24::White;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("WhiteSmoke")) == 0)
		return ColorBGR24::WhiteSmoke;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Yellow")) == 0)
		return ColorBGR24::Yellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("YellowGreen")) == 0)
		return ColorBGR24::YellowGreen;

  return ColorBGR24::Black;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String ColorBGR24::ToString() CONST
{
  return String::Format(TEXT("(%d,%d,%d)"), m_V[0], m_V[1], m_V[2]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_LUA)
Lua::Bind::Scope ColorBGR24::GenerateBindings(LPCTSTR Name)
{
  using namespace Lua::Bind;

  return 
    Class<Graphics::ColorBGR24>(Name)
      .Def(Constructor<>())
      .Def(Constructor<CONST Graphics::ColorBGR24&>())
      .Def(Constructor<BYTE,BYTE,BYTE>())

      .Property("R", &ColorBGR24::GetR, &ColorBGR24::SetR)
      .Property("G", &ColorBGR24::GetG, &ColorBGR24::SetG)
      .Property("B", &ColorBGR24::GetB, &ColorBGR24::SetB)

      .Property("Luminance", &ColorBGR24::Luminance)

      .Def("Clamp", (VOID(ColorBGR24::*)())&ColorBGR24::Clamp)
      .Def("Clamp", (VOID(ColorBGR24::*)(BYTE,BYTE))&ColorBGR24::Clamp)

      .Def(tostring(Self))
      .Def(Self + ColorBGR24())
      .Def(Self - ColorBGR24())
      .Def(Self * FLOAT())
      .Def(Self / FLOAT())
      
      .Scope[ Def("FromHSV", &ColorBGR24::FromHSV) ]
      .Scope[ Def("FromName", &ColorBGR24::FromName) ];

}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------
CONST ColorBGR24 ColorBGR24::AliceBlue = ColorBGR24(240, 248, 255);
CONST ColorBGR24 ColorBGR24::AntiqueWhite = ColorBGR24(250, 235, 215);
CONST ColorBGR24 ColorBGR24::Aqua = ColorBGR24(0, 255, 255);
CONST ColorBGR24 ColorBGR24::Aquamarine = ColorBGR24(127, 255, 212);
CONST ColorBGR24 ColorBGR24::Azure = ColorBGR24(240, 255, 255);
CONST ColorBGR24 ColorBGR24::Beige = ColorBGR24(245, 245, 220);
CONST ColorBGR24 ColorBGR24::Bisque = ColorBGR24(255, 228, 196);
CONST ColorBGR24 ColorBGR24::Black = ColorBGR24(0, 0, 0);
CONST ColorBGR24 ColorBGR24::BlanchedAlmond = ColorBGR24(255, 235, 205);
CONST ColorBGR24 ColorBGR24::Blue = ColorBGR24(0, 0, 255);
CONST ColorBGR24 ColorBGR24::BlueViolet = ColorBGR24(138, 43, 226);
CONST ColorBGR24 ColorBGR24::Brown = ColorBGR24(165, 42, 42);
CONST ColorBGR24 ColorBGR24::BurlyWood = ColorBGR24(222, 184, 135);
CONST ColorBGR24 ColorBGR24::CadetBlue = ColorBGR24(95, 158, 160);
CONST ColorBGR24 ColorBGR24::Chartreuse = ColorBGR24(127, 255, 0);
CONST ColorBGR24 ColorBGR24::Chocolate = ColorBGR24(210, 105, 30);
CONST ColorBGR24 ColorBGR24::Coral = ColorBGR24(255, 127, 80);
CONST ColorBGR24 ColorBGR24::CornflowerBlue = ColorBGR24(100, 149, 237);
CONST ColorBGR24 ColorBGR24::Cornsilk = ColorBGR24(255, 248, 220);
CONST ColorBGR24 ColorBGR24::Crimson = ColorBGR24(220, 20, 60);
CONST ColorBGR24 ColorBGR24::Cyan = ColorBGR24(0, 255, 255);
CONST ColorBGR24 ColorBGR24::DarkBlue = ColorBGR24(0, 0, 139);
CONST ColorBGR24 ColorBGR24::DarkCyan = ColorBGR24(0, 139, 139);
CONST ColorBGR24 ColorBGR24::DarkGoldenrod = ColorBGR24(184, 134, 11);
CONST ColorBGR24 ColorBGR24::DarkGray = ColorBGR24(169, 169, 169);
CONST ColorBGR24 ColorBGR24::DarkGreen = ColorBGR24(0, 100, 0);
CONST ColorBGR24 ColorBGR24::DarkKhaki = ColorBGR24(189, 183, 107);
CONST ColorBGR24 ColorBGR24::DarkMagenta = ColorBGR24(139, 0, 139);
CONST ColorBGR24 ColorBGR24::DarkOliveGreen = ColorBGR24(85, 107, 47);
CONST ColorBGR24 ColorBGR24::DarkOrange = ColorBGR24(255, 140, 0);
CONST ColorBGR24 ColorBGR24::DarkOrchid = ColorBGR24(153, 50, 204);
CONST ColorBGR24 ColorBGR24::DarkRed = ColorBGR24(139, 0, 0);
CONST ColorBGR24 ColorBGR24::DarkSalmon = ColorBGR24(233, 150, 122);
CONST ColorBGR24 ColorBGR24::DarkSeaGreen = ColorBGR24(143, 188, 139);
CONST ColorBGR24 ColorBGR24::DarkSlateBlue = ColorBGR24(72, 61, 139);
CONST ColorBGR24 ColorBGR24::DarkSlateGray = ColorBGR24(47, 79, 79);
CONST ColorBGR24 ColorBGR24::DarkTurquoise = ColorBGR24(0, 206, 209);
CONST ColorBGR24 ColorBGR24::DarkViolet = ColorBGR24(148, 0, 211);
CONST ColorBGR24 ColorBGR24::DeepPink = ColorBGR24(255, 20, 147);
CONST ColorBGR24 ColorBGR24::DeepSkyBlue = ColorBGR24(0, 191, 255);
CONST ColorBGR24 ColorBGR24::DimGray = ColorBGR24(105, 105, 105);
CONST ColorBGR24 ColorBGR24::DodgerBlue = ColorBGR24(30, 144, 255);
CONST ColorBGR24 ColorBGR24::Firebrick = ColorBGR24(178, 34, 34);
CONST ColorBGR24 ColorBGR24::FloralWhite = ColorBGR24(255, 250, 240);
CONST ColorBGR24 ColorBGR24::ForestGreen = ColorBGR24(34, 139, 34);
CONST ColorBGR24 ColorBGR24::Fuchsia = ColorBGR24(255, 0, 255);
CONST ColorBGR24 ColorBGR24::Gainsboro = ColorBGR24(220, 220, 220);
CONST ColorBGR24 ColorBGR24::GhostWhite = ColorBGR24(248, 248, 255);
CONST ColorBGR24 ColorBGR24::Gold = ColorBGR24(255, 215, 0);
CONST ColorBGR24 ColorBGR24::Goldenrod = ColorBGR24(218, 165, 32);
CONST ColorBGR24 ColorBGR24::Gray = ColorBGR24(128, 128, 128);
CONST ColorBGR24 ColorBGR24::Green = ColorBGR24(0, 128, 0);
CONST ColorBGR24 ColorBGR24::GreenYellow = ColorBGR24(173, 255, 47);
CONST ColorBGR24 ColorBGR24::Honeydew = ColorBGR24(240, 255, 240);
CONST ColorBGR24 ColorBGR24::HotPink = ColorBGR24(255, 105, 180);
CONST ColorBGR24 ColorBGR24::IndianRed = ColorBGR24(205, 92, 92);
CONST ColorBGR24 ColorBGR24::Indigo = ColorBGR24(75, 0, 130);
CONST ColorBGR24 ColorBGR24::Ivory = ColorBGR24(255, 255, 240);
CONST ColorBGR24 ColorBGR24::Khaki = ColorBGR24(240, 230, 140);
CONST ColorBGR24 ColorBGR24::Lavender = ColorBGR24(230, 230, 250);
CONST ColorBGR24 ColorBGR24::LavenderBlush = ColorBGR24(255, 240, 245);
CONST ColorBGR24 ColorBGR24::LawnGreen = ColorBGR24(124, 252, 0);
CONST ColorBGR24 ColorBGR24::LemonChiffon = ColorBGR24(255, 250, 205);
CONST ColorBGR24 ColorBGR24::LightBlue = ColorBGR24(173, 216, 230);
CONST ColorBGR24 ColorBGR24::LightCoral = ColorBGR24(240, 128, 128);
CONST ColorBGR24 ColorBGR24::LightCyan = ColorBGR24(224, 255, 255);
CONST ColorBGR24 ColorBGR24::LightGoldenrodYellow = ColorBGR24(250, 250, 210);
CONST ColorBGR24 ColorBGR24::LightGreen = ColorBGR24(144, 238, 144);
CONST ColorBGR24 ColorBGR24::LightGray = ColorBGR24(211, 211, 211);
CONST ColorBGR24 ColorBGR24::LightPink = ColorBGR24(255, 182, 193);
CONST ColorBGR24 ColorBGR24::LightSalmon = ColorBGR24(255, 160, 122);
CONST ColorBGR24 ColorBGR24::LightSeaGreen = ColorBGR24(32, 178, 170);
CONST ColorBGR24 ColorBGR24::LightSkyBlue = ColorBGR24(135, 206, 250);
CONST ColorBGR24 ColorBGR24::LightSlateGray = ColorBGR24(119, 136, 153);
CONST ColorBGR24 ColorBGR24::LightSteelBlue = ColorBGR24(176, 196, 222);
CONST ColorBGR24 ColorBGR24::LightYellow = ColorBGR24(255, 255, 224);
CONST ColorBGR24 ColorBGR24::Lime = ColorBGR24(0, 255, 0);
CONST ColorBGR24 ColorBGR24::LimeGreen = ColorBGR24(50, 205, 50);
CONST ColorBGR24 ColorBGR24::Linen = ColorBGR24(250, 240, 230);
CONST ColorBGR24 ColorBGR24::Magenta = ColorBGR24(255, 0, 255);
CONST ColorBGR24 ColorBGR24::Maroon = ColorBGR24(128, 0, 0);
CONST ColorBGR24 ColorBGR24::MediumAquamarine = ColorBGR24(102, 205, 170);
CONST ColorBGR24 ColorBGR24::MediumBlue = ColorBGR24(0, 0, 205);
CONST ColorBGR24 ColorBGR24::MediumOrchid = ColorBGR24(186, 85, 211);
CONST ColorBGR24 ColorBGR24::MediumPurple = ColorBGR24(147, 112, 219);
CONST ColorBGR24 ColorBGR24::MediumSeaGreen = ColorBGR24(60, 179, 113);
CONST ColorBGR24 ColorBGR24::MediumSlateBlue = ColorBGR24(123, 104, 238);
CONST ColorBGR24 ColorBGR24::MediumSpringGreen = ColorBGR24(0, 250, 154);
CONST ColorBGR24 ColorBGR24::MediumTurquoise = ColorBGR24(72, 209, 204);
CONST ColorBGR24 ColorBGR24::MediumVioletRed = ColorBGR24(199, 21, 133);
CONST ColorBGR24 ColorBGR24::MidnightBlue = ColorBGR24(25, 25, 112);
CONST ColorBGR24 ColorBGR24::MintCream = ColorBGR24(245, 255, 250);
CONST ColorBGR24 ColorBGR24::MistyRose = ColorBGR24(255, 228, 225);
CONST ColorBGR24 ColorBGR24::Moccasin = ColorBGR24(255, 228, 181);
CONST ColorBGR24 ColorBGR24::NavajoWhite = ColorBGR24(255, 222, 173);
CONST ColorBGR24 ColorBGR24::Navy = ColorBGR24(0, 0, 128);
CONST ColorBGR24 ColorBGR24::OldLace = ColorBGR24(253, 245, 230);
CONST ColorBGR24 ColorBGR24::Olive = ColorBGR24(128, 128, 0);
CONST ColorBGR24 ColorBGR24::OliveDrab = ColorBGR24(107, 142, 35);
CONST ColorBGR24 ColorBGR24::Orange = ColorBGR24(255, 165, 0);
CONST ColorBGR24 ColorBGR24::OrangeRed = ColorBGR24(255, 69, 0);
CONST ColorBGR24 ColorBGR24::Orchid = ColorBGR24(218, 112, 214);
CONST ColorBGR24 ColorBGR24::PaleGoldenrod = ColorBGR24(238, 232, 170);
CONST ColorBGR24 ColorBGR24::PaleGreen = ColorBGR24(152, 251, 152);
CONST ColorBGR24 ColorBGR24::PaleTurquoise = ColorBGR24(175, 238, 238);
CONST ColorBGR24 ColorBGR24::PaleVioletRed = ColorBGR24(219, 112, 147);
CONST ColorBGR24 ColorBGR24::PapayaWhip = ColorBGR24(255, 239, 213);
CONST ColorBGR24 ColorBGR24::PeachPuff = ColorBGR24(255, 218, 185);
CONST ColorBGR24 ColorBGR24::Peru = ColorBGR24(205, 133, 63);
CONST ColorBGR24 ColorBGR24::Pink = ColorBGR24(255, 192, 203);
CONST ColorBGR24 ColorBGR24::Plum = ColorBGR24(221, 160, 221);
CONST ColorBGR24 ColorBGR24::PowderBlue = ColorBGR24(176, 224, 230);
CONST ColorBGR24 ColorBGR24::Purple = ColorBGR24(128, 0, 128);
CONST ColorBGR24 ColorBGR24::Red = ColorBGR24(255, 0, 0);
CONST ColorBGR24 ColorBGR24::RosyBrown = ColorBGR24(188, 143, 143);
CONST ColorBGR24 ColorBGR24::RoyalBlue = ColorBGR24(65, 105, 225);
CONST ColorBGR24 ColorBGR24::SaddleBrown = ColorBGR24(139, 69, 19);
CONST ColorBGR24 ColorBGR24::Salmon = ColorBGR24(250, 128, 114);
CONST ColorBGR24 ColorBGR24::SandyBrown = ColorBGR24(244, 164, 96);
CONST ColorBGR24 ColorBGR24::SeaGreen = ColorBGR24(46, 139, 87);
CONST ColorBGR24 ColorBGR24::SeaShell = ColorBGR24(255, 245, 238);
CONST ColorBGR24 ColorBGR24::Sienna = ColorBGR24(160, 82, 45);
CONST ColorBGR24 ColorBGR24::Silver = ColorBGR24(192, 192, 192);
CONST ColorBGR24 ColorBGR24::SkyBlue = ColorBGR24(135, 206, 235);
CONST ColorBGR24 ColorBGR24::SlateBlue = ColorBGR24(106, 90, 205);
CONST ColorBGR24 ColorBGR24::SlateGray = ColorBGR24(112, 128, 144);
CONST ColorBGR24 ColorBGR24::Snow = ColorBGR24(255, 250, 250);
CONST ColorBGR24 ColorBGR24::SpringGreen = ColorBGR24(0, 255, 127);
CONST ColorBGR24 ColorBGR24::SteelBlue = ColorBGR24(70, 130, 180);
CONST ColorBGR24 ColorBGR24::Tan = ColorBGR24(210, 180, 140);
CONST ColorBGR24 ColorBGR24::Teal = ColorBGR24(0, 128, 128);
CONST ColorBGR24 ColorBGR24::Thistle = ColorBGR24(216, 191, 216);
CONST ColorBGR24 ColorBGR24::Tomato = ColorBGR24(255, 99, 71);
CONST ColorBGR24 ColorBGR24::Turquoise = ColorBGR24(64, 224, 208);
CONST ColorBGR24 ColorBGR24::Violet = ColorBGR24(238, 130, 238);
CONST ColorBGR24 ColorBGR24::Wheat = ColorBGR24(245, 222, 179);
CONST ColorBGR24 ColorBGR24::White = ColorBGR24(255, 255, 255);
CONST ColorBGR24 ColorBGR24::WhiteSmoke = ColorBGR24(245, 245, 245);
CONST ColorBGR24 ColorBGR24::Yellow = ColorBGR24(255, 255, 0);
CONST ColorBGR24 ColorBGR24::YellowGreen = ColorBGR24(154, 205, 50);
//-----------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA128F::ColorRGBA128F()
{
  m_V[0] = 0;
  m_V[1] = 0;
  m_V[2] = 0;
  m_V[3] = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA128F::ColorRGBA128F(FLOAT R, FLOAT G, FLOAT B, FLOAT A)
{
  m_V[0] = R;
  m_V[1] = G;
  m_V[2] = B;
  m_V[3] = A;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA128F::ColorRGBA128F(FLOAT R, FLOAT G, FLOAT B)
{
  m_V[0] = R;
  m_V[1] = G;
  m_V[2] = B;
  m_V[3] = 1.0f;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA128F::ColorRGBA128F(CONST ColorRGBA128F& C)
{
  m_V[0] = C.m_V[0];
  m_V[1] = C.m_V[1];
  m_V[2] = C.m_V[2];
  m_V[3] = C.m_V[3];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_DIRECTX9)
ColorRGBA128F::operator D3DCOLOR() CONST
{
  return D3DCOLOR_RGBA((BYTE)(m_V[0]*255), (BYTE)(m_V[1]*255), (BYTE)(m_V[2]*255), (BYTE)(m_V[3]*255));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA128F::operator D3DCOLORVALUE() CONST
{
  D3DCOLORVALUE V;
  V.r = m_V[0];
  V.g = m_V[1];
  V.b = m_V[2];
  V.a = m_V[3];

  return V;
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ColorRGBA128F::Clamp()
{
  Clamp(0.0f, 1.0f);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ColorRGBA128F::Clamp(FLOAT Min, FLOAT Max)
{
  m_V[0] = (m_V[0] > Max) ? Max : m_V[0];
  m_V[1] = (m_V[1] > Max) ? Max : m_V[1];
  m_V[2] = (m_V[2] > Max) ? Max : m_V[2];
  m_V[3] = (m_V[3] > Max) ? Max : m_V[3];

  m_V[0] = (m_V[0] < Min) ? Min : m_V[0];
  m_V[1] = (m_V[1] < Min) ? Min : m_V[1];
  m_V[2] = (m_V[2] < Min) ? Min : m_V[2];
  m_V[3] = (m_V[3] < Min) ? Min : m_V[3];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA128F ColorRGBA128F::operator + (CONST ColorRGBA128F& C) CONST
{
  return ColorRGBA128F(m_V[0] + C.m_V[0], m_V[1] + C.m_V[1], m_V[2] + C.m_V[2], m_V[3] + C.m_V[3]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA128F ColorRGBA128F::operator - (CONST ColorRGBA128F& C) CONST
{
  return ColorRGBA128F(m_V[0] - C.m_V[0], m_V[1] - C.m_V[1], m_V[2] - C.m_V[2], m_V[3] - C.m_V[3]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA128F& ColorRGBA128F::operator += (CONST ColorRGBA128F& C)
{
  m_V[0] += C.m_V[0];
  m_V[1] += C.m_V[1];
  m_V[2] += C.m_V[2];
  m_V[3] += C.m_V[3];
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA128F& ColorRGBA128F::operator -= (CONST ColorRGBA128F& C)
{
  m_V[0] += C.m_V[0];
  m_V[1] += C.m_V[1];
  m_V[2] += C.m_V[2];
  m_V[3] += C.m_V[3];
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA128F& ColorRGBA128F::operator *= (CONST ColorRGBA128F& C)
{
  m_V[0] *= C.m_V[0];
  m_V[1] *= C.m_V[1];
  m_V[2] *= C.m_V[2];
  m_V[3] *= C.m_V[3];
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA128F& ColorRGBA128F::operator /= (CONST ColorRGBA128F& C)
{
  m_V[0] /= C.m_V[0];
  m_V[1] /= C.m_V[1];
  m_V[2] /= C.m_V[2];
  m_V[3] /= C.m_V[3];
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA128F ColorRGBA128F::operator *  (CONST ColorRGBA128F& C) CONST
{
  return ColorRGBA128F(m_V[0]*C.m_V[0], m_V[1]*C.m_V[1], m_V[2]*C.m_V[2], m_V[3]*C.m_V[3]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA128F ColorRGBA128F::operator * (FLOAT V) CONST
{
  return ColorRGBA128F(m_V[0]*V, m_V[1]*V, m_V[2]*V, m_V[3]*V);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA128F ColorRGBA128F::operator / (FLOAT V) CONST
{
  return ColorRGBA128F(m_V[0]/V, m_V[1]/V, m_V[2]/V, m_V[3]/V);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA128F& ColorRGBA128F::operator *= (FLOAT V)
{
  m_V[0] = m_V[0] * V;
  m_V[1] = m_V[1] * V;
  m_V[2] = m_V[2] * V;
  m_V[3] = m_V[3] * V;
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA128F& ColorRGBA128F::operator /= (FLOAT V)
{
  m_V[0] = m_V[0] / V;
  m_V[1] = m_V[1] / V;
  m_V[2] = m_V[2] / V;
  m_V[3] = m_V[3] / V;
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT ColorRGBA128F::Luminance() CONST
{
  return 0.299f*m_V[0] + 0.587f*m_V[1] + 0.114f*m_V[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA128F ColorRGBA128F::FromHSV(FLOAT Hue, FLOAT Saturation, FLOAT Value)
{
	if (Saturation <= 0.0f)
		return ColorRGBA128F(Value, Value, Value);
  Hue = Math::Mod(Hue, 2.0f*Math::PI);
	if (Hue < 0.0)
    Hue += 2.0f*Math::PI;
	
	Hue /= Math::PI / 3.0f;
	
  int i = int(Math::Floor(Hue));
	
	FLOAT f = Hue - i;
	FLOAT p = Value * (1.0f - Saturation);
	FLOAT q = Value * (1.0f - (Saturation*f));
	FLOAT t = Value * (1.0f - (Saturation*(1.0f-f)));
	
	switch(i) 
	{
	case 0: 
		return ColorRGBA128F(Value, t, p);
	case 1: 
		return ColorRGBA128F(q, Value, p);
	case 2: 
		return ColorRGBA128F(p, Value, t);
	case 3:
		return ColorRGBA128F(p, q, Value);
	case 4:
		return ColorRGBA128F(t, p, Value);
	default: 
		return ColorRGBA128F(Value, p, q);
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA128F ColorRGBA128F::RandomColor(FLOAT Saturation, FLOAT Value)
{
  FLOAT Hue = UniformRandom<FLOAT>() * 2.0f * PI;
	return FromHSV(Hue, Saturation, Value);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGBA128F ColorRGBA128F::FromName(LPCTSTR String)
{
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Transparent")) == 0)
		return ColorRGBA128F::Transparent;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("AliceBlue")) == 0)
		return ColorRGBA128F::AliceBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("AntiqueWhite")) == 0)
		return ColorRGBA128F::AntiqueWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Aqua")) == 0)
		return ColorRGBA128F::Aqua;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Aquamarine")) == 0)
		return ColorRGBA128F::Aquamarine;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Azure")) == 0)
		return ColorRGBA128F::Azure;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Beige")) == 0)
		return ColorRGBA128F::Beige;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Bisque")) == 0)
		return ColorRGBA128F::Bisque;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Black")) == 0)
		return ColorRGBA128F::Black;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BlanchedAlmond")) == 0)
		return ColorRGBA128F::BlanchedAlmond;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Blue")) == 0)
		return ColorRGBA128F::Blue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BlueViolet")) == 0)
		return ColorRGBA128F::BlueViolet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Brown")) == 0)
		return ColorRGBA128F::Brown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BurlyWood")) == 0)
		return ColorRGBA128F::BurlyWood;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("CadetBlue")) == 0)
		return ColorRGBA128F::CadetBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Chartreuse")) == 0)
		return ColorRGBA128F::Chartreuse;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Chocolate")) == 0)
		return ColorRGBA128F::Chocolate;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Coral")) == 0)
		return ColorRGBA128F::Coral;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("CornflowerBlue")) == 0)
		return ColorRGBA128F::CornflowerBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Cornsilk")) == 0)
		return ColorRGBA128F::Cornsilk;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Crimson")) == 0)
		return ColorRGBA128F::Crimson;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Cyan")) == 0)
		return ColorRGBA128F::Cyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkBlue")) == 0)
		return ColorRGBA128F::DarkBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkCyan")) == 0)
		return ColorRGBA128F::DarkCyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGoldenrod")) == 0)
		return ColorRGBA128F::DarkGoldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGray")) == 0)
		return ColorRGBA128F::DarkGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGreen")) == 0)
		return ColorRGBA128F::DarkGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkKhaki")) == 0)
		return ColorRGBA128F::DarkKhaki;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkMagenta")) == 0)
		return ColorRGBA128F::DarkMagenta;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOliveGreen")) == 0)
		return ColorRGBA128F::DarkOliveGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOrange")) == 0)
		return ColorRGBA128F::DarkOrange;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOrchid")) == 0)
		return ColorRGBA128F::DarkOrchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkRed")) == 0)
		return ColorRGBA128F::DarkRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSalmon")) == 0)
		return ColorRGBA128F::DarkSalmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSeaGreen")) == 0)
		return ColorRGBA128F::DarkSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSlateBlue")) == 0)
		return ColorRGBA128F::DarkSlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSlateGray")) == 0)
		return ColorRGBA128F::DarkSlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkTurquoise")) == 0)
		return ColorRGBA128F::DarkTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkViolet")) == 0)
		return ColorRGBA128F::DarkViolet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DeepPink")) == 0)
		return ColorRGBA128F::DeepPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DeepSkyBlue")) == 0)
		return ColorRGBA128F::DeepSkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DimGray")) == 0)
		return ColorRGBA128F::DimGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DodgerBlue")) == 0)
		return ColorRGBA128F::DodgerBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Firebrick")) == 0)
		return ColorRGBA128F::Firebrick;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("FloralWhite")) == 0)
		return ColorRGBA128F::FloralWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("ForestGreen")) == 0)
		return ColorRGBA128F::ForestGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Fuchsia")) == 0)
		return ColorRGBA128F::Fuchsia;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gainsboro")) == 0)
		return ColorRGBA128F::Gainsboro;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("GhostWhite")) == 0)
		return ColorRGBA128F::GhostWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gold")) == 0)
		return ColorRGBA128F::Gold;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Goldenrod")) == 0)
		return ColorRGBA128F::Goldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gray")) == 0)
		return ColorRGBA128F::Gray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Green")) == 0)
		return ColorRGBA128F::Green;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("GreenYellow")) == 0)
		return ColorRGBA128F::GreenYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Honeydew")) == 0)
		return ColorRGBA128F::Honeydew;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("HotPink")) == 0)
		return ColorRGBA128F::HotPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("IndianRed")) == 0)
		return ColorRGBA128F::IndianRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Indigo")) == 0)
		return ColorRGBA128F::Indigo;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Ivory")) == 0)
		return ColorRGBA128F::Ivory;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Khaki")) == 0)
		return ColorRGBA128F::Khaki;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Lavender")) == 0)
		return ColorRGBA128F::Lavender;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LavenderBlush")) == 0)
		return ColorRGBA128F::LavenderBlush;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LawnGreen")) == 0)
		return ColorRGBA128F::LawnGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LemonChiffon")) == 0)
		return ColorRGBA128F::LemonChiffon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightBlue")) == 0)
		return ColorRGBA128F::LightBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightCoral")) == 0)
		return ColorRGBA128F::LightCoral;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightCyan")) == 0)
		return ColorRGBA128F::LightCyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGoldenrodYellow")) == 0)
		return ColorRGBA128F::LightGoldenrodYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGreen")) == 0)
		return ColorRGBA128F::LightGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGray")) == 0)
		return ColorRGBA128F::LightGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightPink")) == 0)
		return ColorRGBA128F::LightPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSalmon")) == 0)
		return ColorRGBA128F::LightSalmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSeaGreen")) == 0)
		return ColorRGBA128F::LightSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSkyBlue")) == 0)
		return ColorRGBA128F::LightSkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSlateGray")) == 0)
		return ColorRGBA128F::LightSlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSteelBlue")) == 0)
		return ColorRGBA128F::LightSteelBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightYellow")) == 0)
		return ColorRGBA128F::LightYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Lime")) == 0)
		return ColorRGBA128F::Lime;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LimeGreen")) == 0)
		return ColorRGBA128F::LimeGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Linen")) == 0)
		return ColorRGBA128F::Linen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Magenta")) == 0)
		return ColorRGBA128F::Magenta;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Maroon")) == 0)
		return ColorRGBA128F::Maroon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumAquamarine")) == 0)
		return ColorRGBA128F::MediumAquamarine;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumBlue")) == 0)
		return ColorRGBA128F::MediumBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumOrchid")) == 0)
		return ColorRGBA128F::MediumOrchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumPurple")) == 0)
		return ColorRGBA128F::MediumPurple;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSeaGreen")) == 0)
		return ColorRGBA128F::MediumSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSlateBlue")) == 0)
		return ColorRGBA128F::MediumSlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSpringGreen")) == 0)
		return ColorRGBA128F::MediumSpringGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumTurquoise")) == 0)
		return ColorRGBA128F::MediumTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumVioletRed")) == 0)
		return ColorRGBA128F::MediumVioletRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MidnightBlue")) == 0)
		return ColorRGBA128F::MidnightBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MintCream")) == 0)
		return ColorRGBA128F::MintCream;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MistyRose")) == 0)
		return ColorRGBA128F::MistyRose;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Moccasin")) == 0)
		return ColorRGBA128F::Moccasin;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("NavajoWhite")) == 0)
		return ColorRGBA128F::NavajoWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Navy")) == 0)
		return ColorRGBA128F::Navy;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OldLace")) == 0)
		return ColorRGBA128F::OldLace;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Olive")) == 0)
		return ColorRGBA128F::Olive;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OliveDrab")) == 0)
		return ColorRGBA128F::OliveDrab;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Orange")) == 0)
		return ColorRGBA128F::Orange;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OrangeRed")) == 0)
		return ColorRGBA128F::OrangeRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Orchid")) == 0)
		return ColorRGBA128F::Orchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleGoldenrod")) == 0)
		return ColorRGBA128F::PaleGoldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleGreen")) == 0)
		return ColorRGBA128F::PaleGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleTurquoise")) == 0)
		return ColorRGBA128F::PaleTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleVioletRed")) == 0)
		return ColorRGBA128F::PaleVioletRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PapayaWhip")) == 0)
		return ColorRGBA128F::PapayaWhip;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PeachPuff")) == 0)
		return ColorRGBA128F::PeachPuff;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Peru")) == 0)
		return ColorRGBA128F::Peru;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Pink")) == 0)
		return ColorRGBA128F::Pink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Plum")) == 0)
		return ColorRGBA128F::Plum;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PowderBlue")) == 0)
		return ColorRGBA128F::PowderBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Purple")) == 0)
		return ColorRGBA128F::Purple;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Red")) == 0)
		return ColorRGBA128F::Red;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("RosyBrown")) == 0)
		return ColorRGBA128F::RosyBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("RoyalBlue")) == 0)
		return ColorRGBA128F::RoyalBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SaddleBrown")) == 0)
		return ColorRGBA128F::SaddleBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Salmon")) == 0)
		return ColorRGBA128F::Salmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SandyBrown")) == 0)
		return ColorRGBA128F::SandyBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SeaGreen")) == 0)
		return ColorRGBA128F::SeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SeaShell")) == 0)
		return ColorRGBA128F::SeaShell;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Sienna")) == 0)
		return ColorRGBA128F::Sienna;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Silver")) == 0)
		return ColorRGBA128F::Silver;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SkyBlue")) == 0)
		return ColorRGBA128F::SkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SlateBlue")) == 0)
		return ColorRGBA128F::SlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SlateGray")) == 0)
		return ColorRGBA128F::SlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Snow")) == 0)
		return ColorRGBA128F::Snow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SpringGreen")) == 0)
		return ColorRGBA128F::SpringGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SteelBlue")) == 0)
		return ColorRGBA128F::SteelBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Tan")) == 0)
		return ColorRGBA128F::Tan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Teal")) == 0)
		return ColorRGBA128F::Teal;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Thistle")) == 0)
		return ColorRGBA128F::Thistle;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Tomato")) == 0)
		return ColorRGBA128F::Tomato;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Turquoise")) == 0)
		return ColorRGBA128F::Turquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Violet")) == 0)
		return ColorRGBA128F::Violet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Wheat")) == 0)
		return ColorRGBA128F::Wheat;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("White")) == 0)
		return ColorRGBA128F::White;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("WhiteSmoke")) == 0)
		return ColorRGBA128F::WhiteSmoke;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Yellow")) == 0)
		return ColorRGBA128F::Yellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("YellowGreen")) == 0)
		return ColorRGBA128F::YellowGreen;

  return ColorRGBA128F::Black;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String ColorRGBA128F::ToString() CONST
{
  return String::Format(TEXT("(%f,%f,%f,%f)"), m_V[0], m_V[1], m_V[2], m_V[3]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_LUA)
Lua::Bind::Scope ColorRGBA128F::GenerateBindings(LPCTSTR Name)
{
  using namespace Lua::Bind;

  return 
    Class<Graphics::ColorRGBA128F>(Name)
      .Def(Constructor<>())
      .Def(Constructor<CONST Graphics::ColorRGBA128F&>())
      .Def(Constructor<FLOAT,FLOAT,FLOAT,FLOAT>())
      .Def(Constructor<FLOAT,FLOAT,FLOAT>())

      .Property("R", &ColorRGBA128F::GetR, &ColorRGBA128F::SetR)
      .Property("G", &ColorRGBA128F::GetG, &ColorRGBA128F::SetG)
      .Property("B", &ColorRGBA128F::GetB, &ColorRGBA128F::SetB)
      .Property("A", &ColorRGBA128F::GetA, &ColorRGBA128F::SetA)

      .Property("Luminance", &ColorRGBA128F::Luminance)

      .Def("Clamp", (VOID(ColorRGBA128F::*)())&ColorRGBA128F::Clamp)
      .Def("Clamp", (VOID(ColorRGBA128F::*)(FLOAT,FLOAT))&ColorRGBA128F::Clamp)

      .Def(tostring(Self))
      .Def(Self + ColorRGBA128F())
      .Def(Self - ColorRGBA128F())
      .Def(Self * FLOAT())
      .Def(Self / FLOAT())
      
      .Scope[ Def("FromHSV", &ColorRGBA128F::FromHSV) ]
      .Scope[ Def("FromName", &ColorRGBA128F::FromName) ];

}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------
CONST UINT ColorRGBA128F::Channels = 4;
CONST ColorRGBA128F ColorRGBA128F::Transparent = ColorRGBA128F(1.0f, 1.0f, 1.0f, 0.0f);
CONST ColorRGBA128F ColorRGBA128F::AliceBlue = ColorRGBA128F(0.9411765f, 0.972549f, 1.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::AntiqueWhite = ColorRGBA128F(0.9803922f, 0.9215686f, 0.8431373f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Aqua = ColorRGBA128F(0.0f, 1.0f, 1.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Aquamarine = ColorRGBA128F(0.4980392f, 1.0f, 0.8313726f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Azure = ColorRGBA128F(0.9411765f, 1.0f, 1.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Beige = ColorRGBA128F(0.9607843f, 0.9607843f, 0.8627451f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Bisque = ColorRGBA128F(1.0f, 0.8941177f, 0.7686275f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Black = ColorRGBA128F(0.0f, 0.0f, 0.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::BlanchedAlmond = ColorRGBA128F(1.0f, 0.9215686f, 0.8039216f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Blue = ColorRGBA128F(0.0f, 0.0f, 1.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::BlueViolet = ColorRGBA128F(0.5411765f, 0.1686275f, 0.8862745f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Brown = ColorRGBA128F(0.6470588f, 0.1647059f, 0.1647059f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::BurlyWood = ColorRGBA128F(0.8705882f, 0.7215686f, 0.5294118f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::CadetBlue = ColorRGBA128F(0.372549f, 0.6196079f, 0.627451f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Chartreuse = ColorRGBA128F(0.4980392f, 1.0f, 0.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Chocolate = ColorRGBA128F(0.8235294f, 0.4117647f, 0.1176471f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Coral = ColorRGBA128F(1.0f, 0.4980392f, 0.3137255f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::CornflowerBlue = ColorRGBA128F(0.3921569f, 0.5843138f, 0.9294118f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Cornsilk = ColorRGBA128F(1.0f, 0.972549f, 0.8627451f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Crimson = ColorRGBA128F(0.8627451f, 0.07843138f, 0.2352941f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Cyan = ColorRGBA128F(0.0f, 1.0f, 1.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::DarkBlue = ColorRGBA128F(0.0f, 0.0f, 0.5450981f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::DarkCyan = ColorRGBA128F(0.0f, 0.5450981f, 0.5450981f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::DarkGoldenrod = ColorRGBA128F(0.7215686f, 0.5254902f, 0.04313726f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::DarkGray = ColorRGBA128F(0.6627451f, 0.6627451f, 0.6627451f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::DarkGreen = ColorRGBA128F(0.0f, 0.3921569f, 0.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::DarkKhaki = ColorRGBA128F(0.7411765f, 0.7176471f, 0.4196078f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::DarkMagenta = ColorRGBA128F(0.5450981f, 0.0f, 0.5450981f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::DarkOliveGreen = ColorRGBA128F(0.3333333f, 0.4196078f, 0.1843137f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::DarkOrange = ColorRGBA128F(1.0f, 0.5490196f, 0.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::DarkOrchid = ColorRGBA128F(0.6f, 0.1960784f, 0.8f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::DarkRed = ColorRGBA128F(0.5450981f, 0.0f, 0.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::DarkSalmon = ColorRGBA128F(0.9137255f, 0.5882353f, 0.4784314f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::DarkSeaGreen = ColorRGBA128F(0.5607843f, 0.7372549f, 0.5450981f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::DarkSlateBlue = ColorRGBA128F(0.282353f, 0.2392157f, 0.5450981f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::DarkSlateGray = ColorRGBA128F(0.1843137f, 0.3098039f, 0.3098039f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::DarkTurquoise = ColorRGBA128F(0.0f, 0.8078431f, 0.8196079f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::DarkViolet = ColorRGBA128F(0.5803922f, 0.0f, 0.827451f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::DeepPink = ColorRGBA128F(1.0f, 0.07843138f, 0.5764706f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::DeepSkyBlue = ColorRGBA128F(0.0f, 0.7490196f, 1.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::DimGray = ColorRGBA128F(0.4117647f, 0.4117647f, 0.4117647f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::DodgerBlue = ColorRGBA128F(0.1176471f, 0.5647059f, 1.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Firebrick = ColorRGBA128F(0.6980392f, 0.1333333f, 0.1333333f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::FloralWhite = ColorRGBA128F(1.0f, 0.9803922f, 0.9411765f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::ForestGreen = ColorRGBA128F(0.1333333f, 0.5450981f, 0.1333333f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Fuchsia = ColorRGBA128F(1.0f, 0.0f, 1.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Gainsboro = ColorRGBA128F(0.8627451f, 0.8627451f, 0.8627451f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::GhostWhite = ColorRGBA128F(0.972549f, 0.972549f, 1.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Gold = ColorRGBA128F(1.0f, 0.8431373f, 0.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Goldenrod = ColorRGBA128F(0.854902f, 0.6470588f, 0.1254902f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Gray = ColorRGBA128F(0.5019608f, 0.5019608f, 0.5019608f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Green = ColorRGBA128F(0.0f, 0.5019608f, 0.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::GreenYellow = ColorRGBA128F(0.6784314f, 1.0f, 0.1843137f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Honeydew = ColorRGBA128F(0.9411765f, 1.0f, 0.9411765f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::HotPink = ColorRGBA128F(1.0f, 0.4117647f, 0.7058824f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::IndianRed = ColorRGBA128F(0.8039216f, 0.3607843f, 0.3607843f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Indigo = ColorRGBA128F(0.2941177f, 0.0f, 0.509804f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Ivory = ColorRGBA128F(1.0f, 1.0f, 0.9411765f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Khaki = ColorRGBA128F(0.9411765f, 0.9019608f, 0.5490196f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Lavender = ColorRGBA128F(0.9019608f, 0.9019608f, 0.9803922f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::LavenderBlush = ColorRGBA128F(1.0f, 0.9411765f, 0.9607843f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::LawnGreen = ColorRGBA128F(0.4862745f, 0.9882353f, 0.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::LemonChiffon = ColorRGBA128F(1.0f, 0.9803922f, 0.8039216f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::LightBlue = ColorRGBA128F(0.6784314f, 0.8470588f, 0.9019608f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::LightCoral = ColorRGBA128F(0.9411765f, 0.5019608f, 0.5019608f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::LightCyan = ColorRGBA128F(0.8784314f, 1.0f, 1.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::LightGoldenrodYellow = ColorRGBA128F(0.9803922f, 0.9803922f, 0.8235294f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::LightGreen = ColorRGBA128F(0.5647059f, 0.9333333f, 0.5647059f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::LightGray = ColorRGBA128F(0.827451f, 0.827451f, 0.827451f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::LightPink = ColorRGBA128F(1.0f, 0.7137255f, 0.7568628f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::LightSalmon = ColorRGBA128F(1.0f, 0.627451f, 0.4784314f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::LightSeaGreen = ColorRGBA128F(0.1254902f, 0.6980392f, 0.6666667f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::LightSkyBlue = ColorRGBA128F(0.5294118f, 0.8078431f, 0.9803922f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::LightSlateGray = ColorRGBA128F(0.4666667f, 0.5333334f, 0.6f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::LightSteelBlue = ColorRGBA128F(0.6901961f, 0.7686275f, 0.8705882f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::LightYellow = ColorRGBA128F(1.0f, 1.0f, 0.8784314f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Lime = ColorRGBA128F(0.0f, 1.0f, 0.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::LimeGreen = ColorRGBA128F(0.1960784f, 0.8039216f, 0.1960784f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Linen = ColorRGBA128F(0.9803922f, 0.9411765f, 0.9019608f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Magenta = ColorRGBA128F(1.0f, 0.0f, 1.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Maroon = ColorRGBA128F(0.5019608f, 0.0f, 0.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::MediumAquamarine = ColorRGBA128F(0.4f, 0.8039216f, 0.6666667f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::MediumBlue = ColorRGBA128F(0.0f, 0.0f, 0.8039216f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::MediumOrchid = ColorRGBA128F(0.7294118f, 0.3333333f, 0.827451f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::MediumPurple = ColorRGBA128F(0.5764706f, 0.4392157f, 0.8588235f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::MediumSeaGreen = ColorRGBA128F(0.2352941f, 0.7019608f, 0.4431373f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::MediumSlateBlue = ColorRGBA128F(0.4823529f, 0.4078431f, 0.9333333f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::MediumSpringGreen = ColorRGBA128F(0.0f, 0.9803922f, 0.6039216f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::MediumTurquoise = ColorRGBA128F(0.282353f, 0.8196079f, 0.8f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::MediumVioletRed = ColorRGBA128F(0.7803922f, 0.08235294f, 0.5215687f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::MidnightBlue = ColorRGBA128F(0.09803922f, 0.09803922f, 0.4392157f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::MintCream = ColorRGBA128F(0.9607843f, 1.0f, 0.9803922f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::MistyRose = ColorRGBA128F(1.0f, 0.8941177f, 0.8823529f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Moccasin = ColorRGBA128F(1.0f, 0.8941177f, 0.7098039f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::NavajoWhite = ColorRGBA128F(1.0f, 0.8705882f, 0.6784314f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Navy = ColorRGBA128F(0.0f, 0.0f, 0.5019608f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::OldLace = ColorRGBA128F(0.9921569f, 0.9607843f, 0.9019608f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Olive = ColorRGBA128F(0.5019608f, 0.5019608f, 0.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::OliveDrab = ColorRGBA128F(0.4196078f, 0.5568628f, 0.1372549f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Orange = ColorRGBA128F(1.0f, 0.6470588f, 0.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::OrangeRed = ColorRGBA128F(1.0f, 0.2705882f, 0.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Orchid = ColorRGBA128F(0.854902f, 0.4392157f, 0.8392157f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::PaleGoldenrod = ColorRGBA128F(0.9333333f, 0.9098039f, 0.6666667f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::PaleGreen = ColorRGBA128F(0.5960785f, 0.9843137f, 0.5960785f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::PaleTurquoise = ColorRGBA128F(0.6862745f, 0.9333333f, 0.9333333f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::PaleVioletRed = ColorRGBA128F(0.8588235f, 0.4392157f, 0.5764706f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::PapayaWhip = ColorRGBA128F(1.0f, 0.9372549f, 0.8352941f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::PeachPuff = ColorRGBA128F(1.0f, 0.854902f, 0.7254902f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Peru = ColorRGBA128F(0.8039216f, 0.5215687f, 0.2470588f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Pink = ColorRGBA128F(1.0f, 0.7529412f, 0.7960784f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Plum = ColorRGBA128F(0.8666667f, 0.627451f, 0.8666667f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::PowderBlue = ColorRGBA128F(0.6901961f, 0.8784314f, 0.9019608f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Purple = ColorRGBA128F(0.5019608f, 0.0f, 0.5019608f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Red = ColorRGBA128F(1.0f, 0.0f, 0.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::RosyBrown = ColorRGBA128F(0.7372549f, 0.5607843f, 0.5607843f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::RoyalBlue = ColorRGBA128F(0.254902f, 0.4117647f, 0.8823529f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::SaddleBrown = ColorRGBA128F(0.5450981f, 0.2705882f, 0.07450981f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Salmon = ColorRGBA128F(0.9803922f, 0.5019608f, 0.4470588f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::SandyBrown = ColorRGBA128F(0.9568627f, 0.6431373f, 0.3764706f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::SeaGreen = ColorRGBA128F(0.1803922f, 0.5450981f, 0.3411765f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::SeaShell = ColorRGBA128F(1.0f, 0.9607843f, 0.9333333f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Sienna = ColorRGBA128F(0.627451f, 0.3215686f, 0.1764706f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Silver = ColorRGBA128F(0.7529412f, 0.7529412f, 0.7529412f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::SkyBlue = ColorRGBA128F(0.5294118f, 0.8078431f, 0.9215686f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::SlateBlue = ColorRGBA128F(0.4156863f, 0.3529412f, 0.8039216f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::SlateGray = ColorRGBA128F(0.4392157f, 0.5019608f, 0.5647059f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Snow = ColorRGBA128F(1.0f, 0.9803922f, 0.9803922f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::SpringGreen = ColorRGBA128F(0.0f, 1.0f, 0.4980392f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::SteelBlue = ColorRGBA128F(0.2745098f, 0.509804f, 0.7058824f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Tan = ColorRGBA128F(0.8235294f, 0.7058824f, 0.5490196f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Teal = ColorRGBA128F(0.0f, 0.5019608f, 0.5019608f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Thistle = ColorRGBA128F(0.8470588f, 0.7490196f, 0.8470588f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Tomato = ColorRGBA128F(1.0f, 0.3882353f, 0.2784314f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Turquoise = ColorRGBA128F(0.2509804f, 0.8784314f, 0.8156863f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Violet = ColorRGBA128F(0.9333333f, 0.509804f, 0.9333333f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Wheat = ColorRGBA128F(0.9607843f, 0.8705882f, 0.7019608f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::White = ColorRGBA128F(1.0f, 1.0f, 1.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::WhiteSmoke = ColorRGBA128F(0.9607843f, 0.9607843f, 0.9607843f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::Yellow = ColorRGBA128F(1.0f, 1.0f, 0.0f, 1.0f);
CONST ColorRGBA128F ColorRGBA128F::YellowGreen = ColorRGBA128F(0.6039216f, 0.8039216f, 0.1960784f, 1.0f);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB128F::ColorRGB128F()
{
  m_V[0] = 0;
  m_V[1] = 0;
  m_V[2] = 0;
  m_V[3] = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB128F::ColorRGB128F(FLOAT R, FLOAT G, FLOAT B)
{
  m_V[0] = R;
  m_V[1] = G;
  m_V[2] = B;
  m_V[3] = 1.0f;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB128F::ColorRGB128F(CONST ColorRGB128F& C)
{
  m_V[0] = C.m_V[0];
  m_V[1] = C.m_V[1];
  m_V[2] = C.m_V[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_DIRECTX9)
ColorRGB128F::operator D3DCOLOR() CONST
{
  return D3DCOLOR_XRGB((BYTE)(m_V[0]*255), (BYTE)(m_V[1]*255), (BYTE)(m_V[2]*255));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB128F::operator D3DCOLORVALUE() CONST
{
  D3DCOLORVALUE V;
  V.r = m_V[0];
  V.g = m_V[1];
  V.b = m_V[2];
  V.a = m_V[3];

  return V;
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ColorRGB128F::Clamp()
{
  Clamp(0.0f, 1.0f);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ColorRGB128F::Clamp(FLOAT Min, FLOAT Max)
{
  m_V[0] = (m_V[0] > Max) ? Max : m_V[0];
  m_V[1] = (m_V[1] > Max) ? Max : m_V[1];
  m_V[2] = (m_V[2] > Max) ? Max : m_V[2];

  m_V[0] = (m_V[0] < Min) ? Min : m_V[0];
  m_V[1] = (m_V[1] < Min) ? Min : m_V[1];
  m_V[2] = (m_V[2] < Min) ? Min : m_V[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB128F ColorRGB128F::operator + (CONST ColorRGB128F& C) CONST
{
  return ColorRGB128F(m_V[0] + C.m_V[0], m_V[1] + C.m_V[1], m_V[2] + C.m_V[2]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB128F ColorRGB128F::operator - (CONST ColorRGB128F& C) CONST
{
  return ColorRGB128F(m_V[0] - C.m_V[0], m_V[1] - C.m_V[1], m_V[2] - C.m_V[2]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB128F& ColorRGB128F::operator += (CONST ColorRGB128F& C)
{
  m_V[0] += C.m_V[0];
  m_V[1] += C.m_V[1];
  m_V[2] += C.m_V[2];
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB128F& ColorRGB128F::operator -= (CONST ColorRGB128F& C)
{
  m_V[0] += C.m_V[0];
  m_V[1] += C.m_V[1];
  m_V[2] += C.m_V[2];
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB128F ColorRGB128F::operator * (FLOAT V) CONST
{
  return ColorRGB128F(m_V[0]*V, m_V[1]*V, m_V[2]*V);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB128F ColorRGB128F::operator / (FLOAT V) CONST
{
  return ColorRGB128F(m_V[0]/V, m_V[1]/V, m_V[2]/V);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB128F& ColorRGB128F::operator *= (FLOAT V)
{
  m_V[0] = m_V[0] * V;
  m_V[1] = m_V[1] * V;
  m_V[2] = m_V[2] * V;
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB128F& ColorRGB128F::operator /= (FLOAT V)
{
  m_V[0] = m_V[0] / V;
  m_V[1] = m_V[1] / V;
  m_V[2] = m_V[2] / V;
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT ColorRGB128F::Luminance() CONST
{
  return 0.299f*m_V[0] + 0.587f*m_V[1] + 0.114f*m_V[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB128F ColorRGB128F::FromHSV(FLOAT Hue, FLOAT Saturation, FLOAT Value)
{
	if (Saturation <= 0.0f)
		return ColorRGB128F(Value, Value, Value);
  Hue = Math::Mod(Hue, 2.0f*Math::PI);
	if (Hue < 0.0)
    Hue += 2.0f*Math::PI;
	
	Hue /= Math::PI / 3.0f;
	
  int i = int(Math::Floor(Hue));
	
	FLOAT f = Hue - i;
	FLOAT p = Value * (1.0f - Saturation);
	FLOAT q = Value * (1.0f - (Saturation*f));
	FLOAT t = Value * (1.0f - (Saturation*(1.0f-f)));
	
	switch(i) 
	{
	case 0: 
		return ColorRGB128F(Value, t, p);
	case 1: 
		return ColorRGB128F(q, Value, p);
	case 2: 
		return ColorRGB128F(p, Value, t);
	case 3:
		return ColorRGB128F(p, q, Value);
	case 4:
		return ColorRGB128F(t, p, Value);
	default: 
		return ColorRGB128F(Value, p, q);
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB128F ColorRGB128F::RandomColor(FLOAT Saturation, FLOAT Value)
{
  FLOAT Hue = UniformRandom<FLOAT>() * 2.0f * PI;
	return FromHSV(Hue, Saturation, Value);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB128F ColorRGB128F::FromName(LPCTSTR String)
{
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("AliceBlue")) == 0)
		return ColorRGB128F::AliceBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("AntiqueWhite")) == 0)
		return ColorRGB128F::AntiqueWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Aqua")) == 0)
		return ColorRGB128F::Aqua;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Aquamarine")) == 0)
		return ColorRGB128F::Aquamarine;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Azure")) == 0)
		return ColorRGB128F::Azure;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Beige")) == 0)
		return ColorRGB128F::Beige;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Bisque")) == 0)
		return ColorRGB128F::Bisque;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Black")) == 0)
		return ColorRGB128F::Black;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BlanchedAlmond")) == 0)
		return ColorRGB128F::BlanchedAlmond;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Blue")) == 0)
		return ColorRGB128F::Blue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BlueViolet")) == 0)
		return ColorRGB128F::BlueViolet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Brown")) == 0)
		return ColorRGB128F::Brown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BurlyWood")) == 0)
		return ColorRGB128F::BurlyWood;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("CadetBlue")) == 0)
		return ColorRGB128F::CadetBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Chartreuse")) == 0)
		return ColorRGB128F::Chartreuse;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Chocolate")) == 0)
		return ColorRGB128F::Chocolate;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Coral")) == 0)
		return ColorRGB128F::Coral;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("CornflowerBlue")) == 0)
		return ColorRGB128F::CornflowerBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Cornsilk")) == 0)
		return ColorRGB128F::Cornsilk;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Crimson")) == 0)
		return ColorRGB128F::Crimson;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Cyan")) == 0)
		return ColorRGB128F::Cyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkBlue")) == 0)
		return ColorRGB128F::DarkBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkCyan")) == 0)
		return ColorRGB128F::DarkCyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGoldenrod")) == 0)
		return ColorRGB128F::DarkGoldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGray")) == 0)
		return ColorRGB128F::DarkGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGreen")) == 0)
		return ColorRGB128F::DarkGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkKhaki")) == 0)
		return ColorRGB128F::DarkKhaki;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkMagenta")) == 0)
		return ColorRGB128F::DarkMagenta;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOliveGreen")) == 0)
		return ColorRGB128F::DarkOliveGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOrange")) == 0)
		return ColorRGB128F::DarkOrange;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOrchid")) == 0)
		return ColorRGB128F::DarkOrchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkRed")) == 0)
		return ColorRGB128F::DarkRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSalmon")) == 0)
		return ColorRGB128F::DarkSalmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSeaGreen")) == 0)
		return ColorRGB128F::DarkSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSlateBlue")) == 0)
		return ColorRGB128F::DarkSlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSlateGray")) == 0)
		return ColorRGB128F::DarkSlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkTurquoise")) == 0)
		return ColorRGB128F::DarkTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkViolet")) == 0)
		return ColorRGB128F::DarkViolet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DeepPink")) == 0)
		return ColorRGB128F::DeepPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DeepSkyBlue")) == 0)
		return ColorRGB128F::DeepSkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DimGray")) == 0)
		return ColorRGB128F::DimGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DodgerBlue")) == 0)
		return ColorRGB128F::DodgerBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Firebrick")) == 0)
		return ColorRGB128F::Firebrick;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("FloralWhite")) == 0)
		return ColorRGB128F::FloralWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("ForestGreen")) == 0)
		return ColorRGB128F::ForestGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Fuchsia")) == 0)
		return ColorRGB128F::Fuchsia;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gainsboro")) == 0)
		return ColorRGB128F::Gainsboro;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("GhostWhite")) == 0)
		return ColorRGB128F::GhostWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gold")) == 0)
		return ColorRGB128F::Gold;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Goldenrod")) == 0)
		return ColorRGB128F::Goldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gray")) == 0)
		return ColorRGB128F::Gray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Green")) == 0)
		return ColorRGB128F::Green;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("GreenYellow")) == 0)
		return ColorRGB128F::GreenYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Honeydew")) == 0)
		return ColorRGB128F::Honeydew;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("HotPink")) == 0)
		return ColorRGB128F::HotPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("IndianRed")) == 0)
		return ColorRGB128F::IndianRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Indigo")) == 0)
		return ColorRGB128F::Indigo;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Ivory")) == 0)
		return ColorRGB128F::Ivory;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Khaki")) == 0)
		return ColorRGB128F::Khaki;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Lavender")) == 0)
		return ColorRGB128F::Lavender;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LavenderBlush")) == 0)
		return ColorRGB128F::LavenderBlush;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LawnGreen")) == 0)
		return ColorRGB128F::LawnGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LemonChiffon")) == 0)
		return ColorRGB128F::LemonChiffon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightBlue")) == 0)
		return ColorRGB128F::LightBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightCoral")) == 0)
		return ColorRGB128F::LightCoral;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightCyan")) == 0)
		return ColorRGB128F::LightCyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGoldenrodYellow")) == 0)
		return ColorRGB128F::LightGoldenrodYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGreen")) == 0)
		return ColorRGB128F::LightGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGray")) == 0)
		return ColorRGB128F::LightGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightPink")) == 0)
		return ColorRGB128F::LightPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSalmon")) == 0)
		return ColorRGB128F::LightSalmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSeaGreen")) == 0)
		return ColorRGB128F::LightSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSkyBlue")) == 0)
		return ColorRGB128F::LightSkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSlateGray")) == 0)
		return ColorRGB128F::LightSlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSteelBlue")) == 0)
		return ColorRGB128F::LightSteelBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightYellow")) == 0)
		return ColorRGB128F::LightYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Lime")) == 0)
		return ColorRGB128F::Lime;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LimeGreen")) == 0)
		return ColorRGB128F::LimeGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Linen")) == 0)
		return ColorRGB128F::Linen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Magenta")) == 0)
		return ColorRGB128F::Magenta;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Maroon")) == 0)
		return ColorRGB128F::Maroon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumAquamarine")) == 0)
		return ColorRGB128F::MediumAquamarine;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumBlue")) == 0)
		return ColorRGB128F::MediumBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumOrchid")) == 0)
		return ColorRGB128F::MediumOrchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumPurple")) == 0)
		return ColorRGB128F::MediumPurple;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSeaGreen")) == 0)
		return ColorRGB128F::MediumSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSlateBlue")) == 0)
		return ColorRGB128F::MediumSlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSpringGreen")) == 0)
		return ColorRGB128F::MediumSpringGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumTurquoise")) == 0)
		return ColorRGB128F::MediumTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumVioletRed")) == 0)
		return ColorRGB128F::MediumVioletRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MidnightBlue")) == 0)
		return ColorRGB128F::MidnightBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MintCream")) == 0)
		return ColorRGB128F::MintCream;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MistyRose")) == 0)
		return ColorRGB128F::MistyRose;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Moccasin")) == 0)
		return ColorRGB128F::Moccasin;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("NavajoWhite")) == 0)
		return ColorRGB128F::NavajoWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Navy")) == 0)
		return ColorRGB128F::Navy;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OldLace")) == 0)
		return ColorRGB128F::OldLace;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Olive")) == 0)
		return ColorRGB128F::Olive;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OliveDrab")) == 0)
		return ColorRGB128F::OliveDrab;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Orange")) == 0)
		return ColorRGB128F::Orange;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OrangeRed")) == 0)
		return ColorRGB128F::OrangeRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Orchid")) == 0)
		return ColorRGB128F::Orchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleGoldenrod")) == 0)
		return ColorRGB128F::PaleGoldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleGreen")) == 0)
		return ColorRGB128F::PaleGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleTurquoise")) == 0)
		return ColorRGB128F::PaleTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleVioletRed")) == 0)
		return ColorRGB128F::PaleVioletRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PapayaWhip")) == 0)
		return ColorRGB128F::PapayaWhip;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PeachPuff")) == 0)
		return ColorRGB128F::PeachPuff;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Peru")) == 0)
		return ColorRGB128F::Peru;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Pink")) == 0)
		return ColorRGB128F::Pink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Plum")) == 0)
		return ColorRGB128F::Plum;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PowderBlue")) == 0)
		return ColorRGB128F::PowderBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Purple")) == 0)
		return ColorRGB128F::Purple;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Red")) == 0)
		return ColorRGB128F::Red;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("RosyBrown")) == 0)
		return ColorRGB128F::RosyBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("RoyalBlue")) == 0)
		return ColorRGB128F::RoyalBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SaddleBrown")) == 0)
		return ColorRGB128F::SaddleBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Salmon")) == 0)
		return ColorRGB128F::Salmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SandyBrown")) == 0)
		return ColorRGB128F::SandyBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SeaGreen")) == 0)
		return ColorRGB128F::SeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SeaShell")) == 0)
		return ColorRGB128F::SeaShell;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Sienna")) == 0)
		return ColorRGB128F::Sienna;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Silver")) == 0)
		return ColorRGB128F::Silver;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SkyBlue")) == 0)
		return ColorRGB128F::SkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SlateBlue")) == 0)
		return ColorRGB128F::SlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SlateGray")) == 0)
		return ColorRGB128F::SlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Snow")) == 0)
		return ColorRGB128F::Snow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SpringGreen")) == 0)
		return ColorRGB128F::SpringGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SteelBlue")) == 0)
		return ColorRGB128F::SteelBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Tan")) == 0)
		return ColorRGB128F::Tan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Teal")) == 0)
		return ColorRGB128F::Teal;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Thistle")) == 0)
		return ColorRGB128F::Thistle;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Tomato")) == 0)
		return ColorRGB128F::Tomato;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Turquoise")) == 0)
		return ColorRGB128F::Turquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Violet")) == 0)
		return ColorRGB128F::Violet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Wheat")) == 0)
		return ColorRGB128F::Wheat;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("White")) == 0)
		return ColorRGB128F::White;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("WhiteSmoke")) == 0)
		return ColorRGB128F::WhiteSmoke;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Yellow")) == 0)
		return ColorRGB128F::Yellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("YellowGreen")) == 0)
		return ColorRGB128F::YellowGreen;

  return ColorRGB128F::Black;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String ColorRGB128F::ToString() CONST
{
  return String::Format(TEXT("(%f,%f,%f)"), m_V[0], m_V[1], m_V[2]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_LUA)
Lua::Bind::Scope ColorRGB128F::GenerateBindings(LPCTSTR Name)
{
  using namespace Lua::Bind;

  return 
    Class<Graphics::ColorRGB128F>(Name)
      .Def(Constructor<>())
      .Def(Constructor<CONST Graphics::ColorRGB128F&>())
      .Def(Constructor<FLOAT,FLOAT,FLOAT>())

      .Property("R", &ColorRGB128F::GetR, &ColorRGB128F::SetR)
      .Property("G", &ColorRGB128F::GetG, &ColorRGB128F::SetG)
      .Property("B", &ColorRGB128F::GetB, &ColorRGB128F::SetB)

      .Property("Luminance", &ColorRGB128F::Luminance)

      .Def("Clamp", (VOID(ColorRGB128F::*)())&ColorRGB128F::Clamp)
      .Def("Clamp", (VOID(ColorRGB128F::*)(FLOAT,FLOAT))&ColorRGB128F::Clamp)

      .Def(tostring(Self))
      .Def(Self + ColorRGB128F())
      .Def(Self - ColorRGB128F())
      .Def(Self * FLOAT())
      .Def(Self / FLOAT())
      
      .Scope[ Def("FromHSV", &ColorRGB128F::FromHSV) ]
      .Scope[ Def("FromName", &ColorRGB128F::FromName) ];

}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------
CONST ColorRGB128F ColorRGB128F::AliceBlue = ColorRGB128F(0.9411765f, 0.972549f, 1.0f);
CONST ColorRGB128F ColorRGB128F::AntiqueWhite = ColorRGB128F(0.9803922f, 0.9215686f, 0.8431373f);
CONST ColorRGB128F ColorRGB128F::Aqua = ColorRGB128F(0.0f, 1.0f, 1.0f);
CONST ColorRGB128F ColorRGB128F::Aquamarine = ColorRGB128F(0.4980392f, 1.0f, 0.8313726f);
CONST ColorRGB128F ColorRGB128F::Azure = ColorRGB128F(0.9411765f, 1.0f, 1.0f);
CONST ColorRGB128F ColorRGB128F::Beige = ColorRGB128F(0.9607843f, 0.9607843f, 0.8627451f);
CONST ColorRGB128F ColorRGB128F::Bisque = ColorRGB128F(1.0f, 0.8941177f, 0.7686275f);
CONST ColorRGB128F ColorRGB128F::Black = ColorRGB128F(0.0f, 0.0f, 0.0f);
CONST ColorRGB128F ColorRGB128F::BlanchedAlmond = ColorRGB128F(1.0f, 0.9215686f, 0.8039216f);
CONST ColorRGB128F ColorRGB128F::Blue = ColorRGB128F(0.0f, 0.0f, 1.0f);
CONST ColorRGB128F ColorRGB128F::BlueViolet = ColorRGB128F(0.5411765f, 0.1686275f, 0.8862745f);
CONST ColorRGB128F ColorRGB128F::Brown = ColorRGB128F(0.6470588f, 0.1647059f, 0.1647059f);
CONST ColorRGB128F ColorRGB128F::BurlyWood = ColorRGB128F(0.8705882f, 0.7215686f, 0.5294118f);
CONST ColorRGB128F ColorRGB128F::CadetBlue = ColorRGB128F(0.372549f, 0.6196079f, 0.627451f);
CONST ColorRGB128F ColorRGB128F::Chartreuse = ColorRGB128F(0.4980392f, 1.0f, 0.0f);
CONST ColorRGB128F ColorRGB128F::Chocolate = ColorRGB128F(0.8235294f, 0.4117647f, 0.1176471f);
CONST ColorRGB128F ColorRGB128F::Coral = ColorRGB128F(1.0f, 0.4980392f, 0.3137255f);
CONST ColorRGB128F ColorRGB128F::CornflowerBlue = ColorRGB128F(0.3921569f, 0.5843138f, 0.9294118f);
CONST ColorRGB128F ColorRGB128F::Cornsilk = ColorRGB128F(1.0f, 0.972549f, 0.8627451f);
CONST ColorRGB128F ColorRGB128F::Crimson = ColorRGB128F(0.8627451f, 0.07843138f, 0.2352941f);
CONST ColorRGB128F ColorRGB128F::Cyan = ColorRGB128F(0.0f, 1.0f, 1.0f);
CONST ColorRGB128F ColorRGB128F::DarkBlue = ColorRGB128F(0.0f, 0.0f, 0.5450981f);
CONST ColorRGB128F ColorRGB128F::DarkCyan = ColorRGB128F(0.0f, 0.5450981f, 0.5450981f);
CONST ColorRGB128F ColorRGB128F::DarkGoldenrod = ColorRGB128F(0.7215686f, 0.5254902f, 0.04313726f);
CONST ColorRGB128F ColorRGB128F::DarkGray = ColorRGB128F(0.6627451f, 0.6627451f, 0.6627451f);
CONST ColorRGB128F ColorRGB128F::DarkGreen = ColorRGB128F(0.0f, 0.3921569f, 0.0f);
CONST ColorRGB128F ColorRGB128F::DarkKhaki = ColorRGB128F(0.7411765f, 0.7176471f, 0.4196078f);
CONST ColorRGB128F ColorRGB128F::DarkMagenta = ColorRGB128F(0.5450981f, 0.0f, 0.5450981f);
CONST ColorRGB128F ColorRGB128F::DarkOliveGreen = ColorRGB128F(0.3333333f, 0.4196078f, 0.1843137f);
CONST ColorRGB128F ColorRGB128F::DarkOrange = ColorRGB128F(1.0f, 0.5490196f, 0.0f);
CONST ColorRGB128F ColorRGB128F::DarkOrchid = ColorRGB128F(0.6f, 0.1960784f, 0.8f);
CONST ColorRGB128F ColorRGB128F::DarkRed = ColorRGB128F(0.5450981f, 0.0f, 0.0f);
CONST ColorRGB128F ColorRGB128F::DarkSalmon = ColorRGB128F(0.9137255f, 0.5882353f, 0.4784314f);
CONST ColorRGB128F ColorRGB128F::DarkSeaGreen = ColorRGB128F(0.5607843f, 0.7372549f, 0.5450981f);
CONST ColorRGB128F ColorRGB128F::DarkSlateBlue = ColorRGB128F(0.282353f, 0.2392157f, 0.5450981f);
CONST ColorRGB128F ColorRGB128F::DarkSlateGray = ColorRGB128F(0.1843137f, 0.3098039f, 0.3098039f);
CONST ColorRGB128F ColorRGB128F::DarkTurquoise = ColorRGB128F(0.0f, 0.8078431f, 0.8196079f);
CONST ColorRGB128F ColorRGB128F::DarkViolet = ColorRGB128F(0.5803922f, 0.0f, 0.827451f);
CONST ColorRGB128F ColorRGB128F::DeepPink = ColorRGB128F(1.0f, 0.07843138f, 0.5764706f);
CONST ColorRGB128F ColorRGB128F::DeepSkyBlue = ColorRGB128F(0.0f, 0.7490196f, 1.0f);
CONST ColorRGB128F ColorRGB128F::DimGray = ColorRGB128F(0.4117647f, 0.4117647f, 0.4117647f);
CONST ColorRGB128F ColorRGB128F::DodgerBlue = ColorRGB128F(0.1176471f, 0.5647059f, 1.0f);
CONST ColorRGB128F ColorRGB128F::Firebrick = ColorRGB128F(0.6980392f, 0.1333333f, 0.1333333f);
CONST ColorRGB128F ColorRGB128F::FloralWhite = ColorRGB128F(1.0f, 0.9803922f, 0.9411765f);
CONST ColorRGB128F ColorRGB128F::ForestGreen = ColorRGB128F(0.1333333f, 0.5450981f, 0.1333333f);
CONST ColorRGB128F ColorRGB128F::Fuchsia = ColorRGB128F(1.0f, 0.0f, 1.0f);
CONST ColorRGB128F ColorRGB128F::Gainsboro = ColorRGB128F(0.8627451f, 0.8627451f, 0.8627451f);
CONST ColorRGB128F ColorRGB128F::GhostWhite = ColorRGB128F(0.972549f, 0.972549f, 1.0f);
CONST ColorRGB128F ColorRGB128F::Gold = ColorRGB128F(1.0f, 0.8431373f, 0.0f);
CONST ColorRGB128F ColorRGB128F::Goldenrod = ColorRGB128F(0.854902f, 0.6470588f, 0.1254902f);
CONST ColorRGB128F ColorRGB128F::Gray = ColorRGB128F(0.5019608f, 0.5019608f, 0.5019608f);
CONST ColorRGB128F ColorRGB128F::Green = ColorRGB128F(0.0f, 0.5019608f, 0.0f);
CONST ColorRGB128F ColorRGB128F::GreenYellow = ColorRGB128F(0.6784314f, 1.0f, 0.1843137f);
CONST ColorRGB128F ColorRGB128F::Honeydew = ColorRGB128F(0.9411765f, 1.0f, 0.9411765f);
CONST ColorRGB128F ColorRGB128F::HotPink = ColorRGB128F(1.0f, 0.4117647f, 0.7058824f);
CONST ColorRGB128F ColorRGB128F::IndianRed = ColorRGB128F(0.8039216f, 0.3607843f, 0.3607843f);
CONST ColorRGB128F ColorRGB128F::Indigo = ColorRGB128F(0.2941177f, 0.0f, 0.509804f);
CONST ColorRGB128F ColorRGB128F::Ivory = ColorRGB128F(1.0f, 1.0f, 0.9411765f);
CONST ColorRGB128F ColorRGB128F::Khaki = ColorRGB128F(0.9411765f, 0.9019608f, 0.5490196f);
CONST ColorRGB128F ColorRGB128F::Lavender = ColorRGB128F(0.9019608f, 0.9019608f, 0.9803922f);
CONST ColorRGB128F ColorRGB128F::LavenderBlush = ColorRGB128F(1.0f, 0.9411765f, 0.9607843f);
CONST ColorRGB128F ColorRGB128F::LawnGreen = ColorRGB128F(0.4862745f, 0.9882353f, 0.0f);
CONST ColorRGB128F ColorRGB128F::LemonChiffon = ColorRGB128F(1.0f, 0.9803922f, 0.8039216f);
CONST ColorRGB128F ColorRGB128F::LightBlue = ColorRGB128F(0.6784314f, 0.8470588f, 0.9019608f);
CONST ColorRGB128F ColorRGB128F::LightCoral = ColorRGB128F(0.9411765f, 0.5019608f, 0.5019608f);
CONST ColorRGB128F ColorRGB128F::LightCyan = ColorRGB128F(0.8784314f, 1.0f, 1.0f);
CONST ColorRGB128F ColorRGB128F::LightGoldenrodYellow = ColorRGB128F(0.9803922f, 0.9803922f, 0.8235294f);
CONST ColorRGB128F ColorRGB128F::LightGreen = ColorRGB128F(0.5647059f, 0.9333333f, 0.5647059f);
CONST ColorRGB128F ColorRGB128F::LightGray = ColorRGB128F(0.827451f, 0.827451f, 0.827451f);
CONST ColorRGB128F ColorRGB128F::LightPink = ColorRGB128F(1.0f, 0.7137255f, 0.7568628f);
CONST ColorRGB128F ColorRGB128F::LightSalmon = ColorRGB128F(1.0f, 0.627451f, 0.4784314f);
CONST ColorRGB128F ColorRGB128F::LightSeaGreen = ColorRGB128F(0.1254902f, 0.6980392f, 0.6666667f);
CONST ColorRGB128F ColorRGB128F::LightSkyBlue = ColorRGB128F(0.5294118f, 0.8078431f, 0.9803922f);
CONST ColorRGB128F ColorRGB128F::LightSlateGray = ColorRGB128F(0.4666667f, 0.5333334f, 0.6f);
CONST ColorRGB128F ColorRGB128F::LightSteelBlue = ColorRGB128F(0.6901961f, 0.7686275f, 0.8705882f);
CONST ColorRGB128F ColorRGB128F::LightYellow = ColorRGB128F(1.0f, 1.0f, 0.8784314f);
CONST ColorRGB128F ColorRGB128F::Lime = ColorRGB128F(0.0f, 1.0f, 0.0f);
CONST ColorRGB128F ColorRGB128F::LimeGreen = ColorRGB128F(0.1960784f, 0.8039216f, 0.1960784f);
CONST ColorRGB128F ColorRGB128F::Linen = ColorRGB128F(0.9803922f, 0.9411765f, 0.9019608f);
CONST ColorRGB128F ColorRGB128F::Magenta = ColorRGB128F(1.0f, 0.0f, 1.0f);
CONST ColorRGB128F ColorRGB128F::Maroon = ColorRGB128F(0.5019608f, 0.0f, 0.0f);
CONST ColorRGB128F ColorRGB128F::MediumAquamarine = ColorRGB128F(0.4f, 0.8039216f, 0.6666667f);
CONST ColorRGB128F ColorRGB128F::MediumBlue = ColorRGB128F(0.0f, 0.0f, 0.8039216f);
CONST ColorRGB128F ColorRGB128F::MediumOrchid = ColorRGB128F(0.7294118f, 0.3333333f, 0.827451f);
CONST ColorRGB128F ColorRGB128F::MediumPurple = ColorRGB128F(0.5764706f, 0.4392157f, 0.8588235f);
CONST ColorRGB128F ColorRGB128F::MediumSeaGreen = ColorRGB128F(0.2352941f, 0.7019608f, 0.4431373f);
CONST ColorRGB128F ColorRGB128F::MediumSlateBlue = ColorRGB128F(0.4823529f, 0.4078431f, 0.9333333f);
CONST ColorRGB128F ColorRGB128F::MediumSpringGreen = ColorRGB128F(0.0f, 0.9803922f, 0.6039216f);
CONST ColorRGB128F ColorRGB128F::MediumTurquoise = ColorRGB128F(0.282353f, 0.8196079f, 0.8f);
CONST ColorRGB128F ColorRGB128F::MediumVioletRed = ColorRGB128F(0.7803922f, 0.08235294f, 0.5215687f);
CONST ColorRGB128F ColorRGB128F::MidnightBlue = ColorRGB128F(0.09803922f, 0.09803922f, 0.4392157f);
CONST ColorRGB128F ColorRGB128F::MintCream = ColorRGB128F(0.9607843f, 1.0f, 0.9803922f);
CONST ColorRGB128F ColorRGB128F::MistyRose = ColorRGB128F(1.0f, 0.8941177f, 0.8823529f);
CONST ColorRGB128F ColorRGB128F::Moccasin = ColorRGB128F(1.0f, 0.8941177f, 0.7098039f);
CONST ColorRGB128F ColorRGB128F::NavajoWhite = ColorRGB128F(1.0f, 0.8705882f, 0.6784314f);
CONST ColorRGB128F ColorRGB128F::Navy = ColorRGB128F(0.0f, 0.0f, 0.5019608f);
CONST ColorRGB128F ColorRGB128F::OldLace = ColorRGB128F(0.9921569f, 0.9607843f, 0.9019608f);
CONST ColorRGB128F ColorRGB128F::Olive = ColorRGB128F(0.5019608f, 0.5019608f, 0.0f);
CONST ColorRGB128F ColorRGB128F::OliveDrab = ColorRGB128F(0.4196078f, 0.5568628f, 0.1372549f);
CONST ColorRGB128F ColorRGB128F::Orange = ColorRGB128F(1.0f, 0.6470588f, 0.0f);
CONST ColorRGB128F ColorRGB128F::OrangeRed = ColorRGB128F(1.0f, 0.2705882f, 0.0f);
CONST ColorRGB128F ColorRGB128F::Orchid = ColorRGB128F(0.854902f, 0.4392157f, 0.8392157f);
CONST ColorRGB128F ColorRGB128F::PaleGoldenrod = ColorRGB128F(0.9333333f, 0.9098039f, 0.6666667f);
CONST ColorRGB128F ColorRGB128F::PaleGreen = ColorRGB128F(0.5960785f, 0.9843137f, 0.5960785f);
CONST ColorRGB128F ColorRGB128F::PaleTurquoise = ColorRGB128F(0.6862745f, 0.9333333f, 0.9333333f);
CONST ColorRGB128F ColorRGB128F::PaleVioletRed = ColorRGB128F(0.8588235f, 0.4392157f, 0.5764706f);
CONST ColorRGB128F ColorRGB128F::PapayaWhip = ColorRGB128F(1.0f, 0.9372549f, 0.8352941f);
CONST ColorRGB128F ColorRGB128F::PeachPuff = ColorRGB128F(1.0f, 0.854902f, 0.7254902f);
CONST ColorRGB128F ColorRGB128F::Peru = ColorRGB128F(0.8039216f, 0.5215687f, 0.2470588f);
CONST ColorRGB128F ColorRGB128F::Pink = ColorRGB128F(1.0f, 0.7529412f, 0.7960784f);
CONST ColorRGB128F ColorRGB128F::Plum = ColorRGB128F(0.8666667f, 0.627451f, 0.8666667f);
CONST ColorRGB128F ColorRGB128F::PowderBlue = ColorRGB128F(0.6901961f, 0.8784314f, 0.9019608f);
CONST ColorRGB128F ColorRGB128F::Purple = ColorRGB128F(0.5019608f, 0.0f, 0.5019608f);
CONST ColorRGB128F ColorRGB128F::Red = ColorRGB128F(1.0f, 0.0f, 0.0f);
CONST ColorRGB128F ColorRGB128F::RosyBrown = ColorRGB128F(0.7372549f, 0.5607843f, 0.5607843f);
CONST ColorRGB128F ColorRGB128F::RoyalBlue = ColorRGB128F(0.254902f, 0.4117647f, 0.8823529f);
CONST ColorRGB128F ColorRGB128F::SaddleBrown = ColorRGB128F(0.5450981f, 0.2705882f, 0.07450981f);
CONST ColorRGB128F ColorRGB128F::Salmon = ColorRGB128F(0.9803922f, 0.5019608f, 0.4470588f);
CONST ColorRGB128F ColorRGB128F::SandyBrown = ColorRGB128F(0.9568627f, 0.6431373f, 0.3764706f);
CONST ColorRGB128F ColorRGB128F::SeaGreen = ColorRGB128F(0.1803922f, 0.5450981f, 0.3411765f);
CONST ColorRGB128F ColorRGB128F::SeaShell = ColorRGB128F(1.0f, 0.9607843f, 0.9333333f);
CONST ColorRGB128F ColorRGB128F::Sienna = ColorRGB128F(0.627451f, 0.3215686f, 0.1764706f);
CONST ColorRGB128F ColorRGB128F::Silver = ColorRGB128F(0.7529412f, 0.7529412f, 0.7529412f);
CONST ColorRGB128F ColorRGB128F::SkyBlue = ColorRGB128F(0.5294118f, 0.8078431f, 0.9215686f);
CONST ColorRGB128F ColorRGB128F::SlateBlue = ColorRGB128F(0.4156863f, 0.3529412f, 0.8039216f);
CONST ColorRGB128F ColorRGB128F::SlateGray = ColorRGB128F(0.4392157f, 0.5019608f, 0.5647059f);
CONST ColorRGB128F ColorRGB128F::Snow = ColorRGB128F(1.0f, 0.9803922f, 0.9803922f);
CONST ColorRGB128F ColorRGB128F::SpringGreen = ColorRGB128F(0.0f, 1.0f, 0.4980392f);
CONST ColorRGB128F ColorRGB128F::SteelBlue = ColorRGB128F(0.2745098f, 0.509804f, 0.7058824f);
CONST ColorRGB128F ColorRGB128F::Tan = ColorRGB128F(0.8235294f, 0.7058824f, 0.5490196f);
CONST ColorRGB128F ColorRGB128F::Teal = ColorRGB128F(0.0f, 0.5019608f, 0.5019608f);
CONST ColorRGB128F ColorRGB128F::Thistle = ColorRGB128F(0.8470588f, 0.7490196f, 0.8470588f);
CONST ColorRGB128F ColorRGB128F::Tomato = ColorRGB128F(1.0f, 0.3882353f, 0.2784314f);
CONST ColorRGB128F ColorRGB128F::Turquoise = ColorRGB128F(0.2509804f, 0.8784314f, 0.8156863f);
CONST ColorRGB128F ColorRGB128F::Violet = ColorRGB128F(0.9333333f, 0.509804f, 0.9333333f);
CONST ColorRGB128F ColorRGB128F::Wheat = ColorRGB128F(0.9607843f, 0.8705882f, 0.7019608f);
CONST ColorRGB128F ColorRGB128F::White = ColorRGB128F(1.0f, 1.0f, 1.0f);
CONST ColorRGB128F ColorRGB128F::WhiteSmoke = ColorRGB128F(0.9607843f, 0.9607843f, 0.9607843f);
CONST ColorRGB128F ColorRGB128F::Yellow = ColorRGB128F(1.0f, 1.0f, 0.0f);
CONST ColorRGB128F ColorRGB128F::YellowGreen = ColorRGB128F(0.6039216f, 0.8039216f, 0.1960784f);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB96F::ColorRGB96F()
{
  m_V[0] = 0;
  m_V[1] = 0;
  m_V[2] = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB96F::ColorRGB96F(FLOAT R, FLOAT G, FLOAT B)
{
  m_V[0] = R;
  m_V[1] = G;
  m_V[2] = B;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB96F::ColorRGB96F(CONST ColorRGB96F& C)
{
  m_V[0] = C.m_V[0];
  m_V[1] = C.m_V[1];
  m_V[2] = C.m_V[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_DIRECTX9)
ColorRGB96F::operator D3DCOLOR() CONST
{
  return D3DCOLOR_XRGB((BYTE)(m_V[0]*255), (BYTE)(m_V[1]*255), (BYTE)(m_V[2]*255));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB96F::operator D3DCOLORVALUE() CONST
{
  D3DCOLORVALUE V;
  V.r = m_V[0];
  V.g = m_V[1];
  V.b = m_V[2];
  V.a = 1.0f;

  return V;
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ColorRGB96F::Clamp()
{
  Clamp(0.0f, 1.0f);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ColorRGB96F::Clamp(FLOAT Min, FLOAT Max)
{
  m_V[0] = (m_V[0] > Max) ? Max : m_V[0];
  m_V[1] = (m_V[1] > Max) ? Max : m_V[1];
  m_V[2] = (m_V[2] > Max) ? Max : m_V[2];

  m_V[0] = (m_V[0] < Min) ? Min : m_V[0];
  m_V[1] = (m_V[1] < Min) ? Min : m_V[1];
  m_V[2] = (m_V[2] < Min) ? Min : m_V[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB96F ColorRGB96F::operator + (CONST ColorRGB96F& C) CONST
{
  return ColorRGB96F(m_V[0] + C.m_V[0], m_V[1] + C.m_V[1], m_V[2] + C.m_V[2]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB96F ColorRGB96F::operator - (CONST ColorRGB96F& C) CONST
{
  return ColorRGB96F(m_V[0] - C.m_V[0], m_V[1] - C.m_V[1], m_V[2] - C.m_V[2]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB96F& ColorRGB96F::operator += (CONST ColorRGB96F& C)
{
  m_V[0] += C.m_V[0];
  m_V[1] += C.m_V[1];
  m_V[2] += C.m_V[2];
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB96F& ColorRGB96F::operator -= (CONST ColorRGB96F& C)
{
  m_V[0] += C.m_V[0];
  m_V[1] += C.m_V[1];
  m_V[2] += C.m_V[2];
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB96F ColorRGB96F::operator * (FLOAT V) CONST
{
  return ColorRGB96F(m_V[0]*V, m_V[1]*V, m_V[2]*V);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB96F ColorRGB96F::operator / (FLOAT V) CONST
{
  return ColorRGB96F(m_V[0]/V, m_V[1]/V, m_V[2]/V);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB96F& ColorRGB96F::operator *= (FLOAT V)
{
  m_V[0] = m_V[0] * V;
  m_V[1] = m_V[1] * V;
  m_V[2] = m_V[2] * V;
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB96F& ColorRGB96F::operator /= (FLOAT V)
{
  m_V[0] = m_V[0] / V;
  m_V[1] = m_V[1] / V;
  m_V[2] = m_V[2] / V;
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT ColorRGB96F::Luminance() CONST
{
  return 0.299f*m_V[0] + 0.587f*m_V[1] + 0.114f*m_V[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB96F ColorRGB96F::FromHSV(FLOAT Hue, FLOAT Saturation, FLOAT Value)
{
	if (Saturation <= 0.0f)
		return ColorRGB96F(Value, Value, Value);
  Hue = Math::Mod(Hue, 2.0f*Math::PI);
	if (Hue < 0.0)
    Hue += 2.0f*Math::PI;
	
	Hue /= Math::PI / 3.0f;
	
  int i = int(Math::Floor(Hue));
	
	FLOAT f = Hue - i;
	FLOAT p = Value * (1.0f - Saturation);
	FLOAT q = Value * (1.0f - (Saturation*f));
	FLOAT t = Value * (1.0f - (Saturation*(1.0f-f)));
	
	switch(i) 
	{
	case 0: 
		return ColorRGB96F(Value, t, p);
	case 1: 
		return ColorRGB96F(q, Value, p);
	case 2: 
		return ColorRGB96F(p, Value, t);
	case 3:
		return ColorRGB96F(p, q, Value);
	case 4:
		return ColorRGB96F(t, p, Value);
	default: 
		return ColorRGB96F(Value, p, q);
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB96F ColorRGB96F::RandomColor(FLOAT Saturation, FLOAT Value)
{
  FLOAT Hue = UniformRandom<FLOAT>() * 2.0f * PI;
	return FromHSV(Hue, Saturation, Value);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorRGB96F ColorRGB96F::FromName(LPCTSTR String)
{
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("AliceBlue")) == 0)
		return ColorRGB96F::AliceBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("AntiqueWhite")) == 0)
		return ColorRGB96F::AntiqueWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Aqua")) == 0)
		return ColorRGB96F::Aqua;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Aquamarine")) == 0)
		return ColorRGB96F::Aquamarine;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Azure")) == 0)
		return ColorRGB96F::Azure;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Beige")) == 0)
		return ColorRGB96F::Beige;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Bisque")) == 0)
		return ColorRGB96F::Bisque;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Black")) == 0)
		return ColorRGB96F::Black;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BlanchedAlmond")) == 0)
		return ColorRGB96F::BlanchedAlmond;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Blue")) == 0)
		return ColorRGB96F::Blue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BlueViolet")) == 0)
		return ColorRGB96F::BlueViolet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Brown")) == 0)
		return ColorRGB96F::Brown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("BurlyWood")) == 0)
		return ColorRGB96F::BurlyWood;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("CadetBlue")) == 0)
		return ColorRGB96F::CadetBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Chartreuse")) == 0)
		return ColorRGB96F::Chartreuse;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Chocolate")) == 0)
		return ColorRGB96F::Chocolate;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Coral")) == 0)
		return ColorRGB96F::Coral;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("CornflowerBlue")) == 0)
		return ColorRGB96F::CornflowerBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Cornsilk")) == 0)
		return ColorRGB96F::Cornsilk;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Crimson")) == 0)
		return ColorRGB96F::Crimson;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Cyan")) == 0)
		return ColorRGB96F::Cyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkBlue")) == 0)
		return ColorRGB96F::DarkBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkCyan")) == 0)
		return ColorRGB96F::DarkCyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGoldenrod")) == 0)
		return ColorRGB96F::DarkGoldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGray")) == 0)
		return ColorRGB96F::DarkGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkGreen")) == 0)
		return ColorRGB96F::DarkGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkKhaki")) == 0)
		return ColorRGB96F::DarkKhaki;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkMagenta")) == 0)
		return ColorRGB96F::DarkMagenta;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOliveGreen")) == 0)
		return ColorRGB96F::DarkOliveGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOrange")) == 0)
		return ColorRGB96F::DarkOrange;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkOrchid")) == 0)
		return ColorRGB96F::DarkOrchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkRed")) == 0)
		return ColorRGB96F::DarkRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSalmon")) == 0)
		return ColorRGB96F::DarkSalmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSeaGreen")) == 0)
		return ColorRGB96F::DarkSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSlateBlue")) == 0)
		return ColorRGB96F::DarkSlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkSlateGray")) == 0)
		return ColorRGB96F::DarkSlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkTurquoise")) == 0)
		return ColorRGB96F::DarkTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DarkViolet")) == 0)
		return ColorRGB96F::DarkViolet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DeepPink")) == 0)
		return ColorRGB96F::DeepPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DeepSkyBlue")) == 0)
		return ColorRGB96F::DeepSkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DimGray")) == 0)
		return ColorRGB96F::DimGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("DodgerBlue")) == 0)
		return ColorRGB96F::DodgerBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Firebrick")) == 0)
		return ColorRGB96F::Firebrick;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("FloralWhite")) == 0)
		return ColorRGB96F::FloralWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("ForestGreen")) == 0)
		return ColorRGB96F::ForestGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Fuchsia")) == 0)
		return ColorRGB96F::Fuchsia;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gainsboro")) == 0)
		return ColorRGB96F::Gainsboro;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("GhostWhite")) == 0)
		return ColorRGB96F::GhostWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gold")) == 0)
		return ColorRGB96F::Gold;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Goldenrod")) == 0)
		return ColorRGB96F::Goldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Gray")) == 0)
		return ColorRGB96F::Gray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Green")) == 0)
		return ColorRGB96F::Green;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("GreenYellow")) == 0)
		return ColorRGB96F::GreenYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Honeydew")) == 0)
		return ColorRGB96F::Honeydew;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("HotPink")) == 0)
		return ColorRGB96F::HotPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("IndianRed")) == 0)
		return ColorRGB96F::IndianRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Indigo")) == 0)
		return ColorRGB96F::Indigo;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Ivory")) == 0)
		return ColorRGB96F::Ivory;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Khaki")) == 0)
		return ColorRGB96F::Khaki;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Lavender")) == 0)
		return ColorRGB96F::Lavender;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LavenderBlush")) == 0)
		return ColorRGB96F::LavenderBlush;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LawnGreen")) == 0)
		return ColorRGB96F::LawnGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LemonChiffon")) == 0)
		return ColorRGB96F::LemonChiffon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightBlue")) == 0)
		return ColorRGB96F::LightBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightCoral")) == 0)
		return ColorRGB96F::LightCoral;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightCyan")) == 0)
		return ColorRGB96F::LightCyan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGoldenrodYellow")) == 0)
		return ColorRGB96F::LightGoldenrodYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGreen")) == 0)
		return ColorRGB96F::LightGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightGray")) == 0)
		return ColorRGB96F::LightGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightPink")) == 0)
		return ColorRGB96F::LightPink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSalmon")) == 0)
		return ColorRGB96F::LightSalmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSeaGreen")) == 0)
		return ColorRGB96F::LightSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSkyBlue")) == 0)
		return ColorRGB96F::LightSkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSlateGray")) == 0)
		return ColorRGB96F::LightSlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightSteelBlue")) == 0)
		return ColorRGB96F::LightSteelBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LightYellow")) == 0)
		return ColorRGB96F::LightYellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Lime")) == 0)
		return ColorRGB96F::Lime;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("LimeGreen")) == 0)
		return ColorRGB96F::LimeGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Linen")) == 0)
		return ColorRGB96F::Linen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Magenta")) == 0)
		return ColorRGB96F::Magenta;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Maroon")) == 0)
		return ColorRGB96F::Maroon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumAquamarine")) == 0)
		return ColorRGB96F::MediumAquamarine;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumBlue")) == 0)
		return ColorRGB96F::MediumBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumOrchid")) == 0)
		return ColorRGB96F::MediumOrchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumPurple")) == 0)
		return ColorRGB96F::MediumPurple;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSeaGreen")) == 0)
		return ColorRGB96F::MediumSeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSlateBlue")) == 0)
		return ColorRGB96F::MediumSlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumSpringGreen")) == 0)
		return ColorRGB96F::MediumSpringGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumTurquoise")) == 0)
		return ColorRGB96F::MediumTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MediumVioletRed")) == 0)
		return ColorRGB96F::MediumVioletRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MidnightBlue")) == 0)
		return ColorRGB96F::MidnightBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MintCream")) == 0)
		return ColorRGB96F::MintCream;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("MistyRose")) == 0)
		return ColorRGB96F::MistyRose;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Moccasin")) == 0)
		return ColorRGB96F::Moccasin;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("NavajoWhite")) == 0)
		return ColorRGB96F::NavajoWhite;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Navy")) == 0)
		return ColorRGB96F::Navy;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OldLace")) == 0)
		return ColorRGB96F::OldLace;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Olive")) == 0)
		return ColorRGB96F::Olive;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OliveDrab")) == 0)
		return ColorRGB96F::OliveDrab;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Orange")) == 0)
		return ColorRGB96F::Orange;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("OrangeRed")) == 0)
		return ColorRGB96F::OrangeRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Orchid")) == 0)
		return ColorRGB96F::Orchid;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleGoldenrod")) == 0)
		return ColorRGB96F::PaleGoldenrod;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleGreen")) == 0)
		return ColorRGB96F::PaleGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleTurquoise")) == 0)
		return ColorRGB96F::PaleTurquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PaleVioletRed")) == 0)
		return ColorRGB96F::PaleVioletRed;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PapayaWhip")) == 0)
		return ColorRGB96F::PapayaWhip;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PeachPuff")) == 0)
		return ColorRGB96F::PeachPuff;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Peru")) == 0)
		return ColorRGB96F::Peru;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Pink")) == 0)
		return ColorRGB96F::Pink;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Plum")) == 0)
		return ColorRGB96F::Plum;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("PowderBlue")) == 0)
		return ColorRGB96F::PowderBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Purple")) == 0)
		return ColorRGB96F::Purple;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Red")) == 0)
		return ColorRGB96F::Red;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("RosyBrown")) == 0)
		return ColorRGB96F::RosyBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("RoyalBlue")) == 0)
		return ColorRGB96F::RoyalBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SaddleBrown")) == 0)
		return ColorRGB96F::SaddleBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Salmon")) == 0)
		return ColorRGB96F::Salmon;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SandyBrown")) == 0)
		return ColorRGB96F::SandyBrown;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SeaGreen")) == 0)
		return ColorRGB96F::SeaGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SeaShell")) == 0)
		return ColorRGB96F::SeaShell;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Sienna")) == 0)
		return ColorRGB96F::Sienna;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Silver")) == 0)
		return ColorRGB96F::Silver;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SkyBlue")) == 0)
		return ColorRGB96F::SkyBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SlateBlue")) == 0)
		return ColorRGB96F::SlateBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SlateGray")) == 0)
		return ColorRGB96F::SlateGray;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Snow")) == 0)
		return ColorRGB96F::Snow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SpringGreen")) == 0)
		return ColorRGB96F::SpringGreen;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("SteelBlue")) == 0)
		return ColorRGB96F::SteelBlue;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Tan")) == 0)
		return ColorRGB96F::Tan;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Teal")) == 0)
		return ColorRGB96F::Teal;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Thistle")) == 0)
		return ColorRGB96F::Thistle;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Tomato")) == 0)
		return ColorRGB96F::Tomato;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Turquoise")) == 0)
		return ColorRGB96F::Turquoise;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Violet")) == 0)
		return ColorRGB96F::Violet;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Wheat")) == 0)
		return ColorRGB96F::Wheat;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("White")) == 0)
		return ColorRGB96F::White;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("WhiteSmoke")) == 0)
		return ColorRGB96F::WhiteSmoke;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("Yellow")) == 0)
		return ColorRGB96F::Yellow;
	if (CharTraits<TCHAR>::StrICmp(String, TEXT("YellowGreen")) == 0)
		return ColorRGB96F::YellowGreen;

  return ColorRGB96F::Black;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String ColorRGB96F::ToString() CONST
{
  return String::Format(TEXT("(%f,%f,%f)"), m_V[0], m_V[1], m_V[2]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_LUA)
Lua::Bind::Scope ColorRGB96F::GenerateBindings(LPCTSTR Name)
{
  using namespace Lua::Bind;

  return 
    Class<Graphics::ColorRGB96F>(Name)
      .Def(Constructor<>())
      .Def(Constructor<CONST Graphics::ColorRGB96F&>())
      .Def(Constructor<FLOAT,FLOAT,FLOAT>())

      .Property("R", &ColorRGB96F::GetR, &ColorRGB96F::SetR)
      .Property("G", &ColorRGB96F::GetG, &ColorRGB96F::SetG)
      .Property("B", &ColorRGB96F::GetB, &ColorRGB96F::SetB)

      .Property("Luminance", &ColorRGB96F::Luminance)

      .Def("Clamp", (VOID(ColorRGB96F::*)())&ColorRGB96F::Clamp)
      .Def("Clamp", (VOID(ColorRGB96F::*)(FLOAT,FLOAT))&ColorRGB96F::Clamp)

      .Def(tostring(Self))
      .Def(Self + ColorRGB96F())
      .Def(Self - ColorRGB96F())
      .Def(Self * FLOAT())
      .Def(Self / FLOAT())
      
      .Scope[ Def("FromHSV", &ColorRGB96F::FromHSV) ]
      .Scope[ Def("FromName", &ColorRGB96F::FromName) ];

}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------
CONST ColorRGB96F ColorRGB96F::AliceBlue = ColorRGB96F(0.9411765f, 0.972549f, 1.0f);
CONST ColorRGB96F ColorRGB96F::AntiqueWhite = ColorRGB96F(0.9803922f, 0.9215686f, 0.8431373f);
CONST ColorRGB96F ColorRGB96F::Aqua = ColorRGB96F(0.0f, 1.0f, 1.0f);
CONST ColorRGB96F ColorRGB96F::Aquamarine = ColorRGB96F(0.4980392f, 1.0f, 0.8313726f);
CONST ColorRGB96F ColorRGB96F::Azure = ColorRGB96F(0.9411765f, 1.0f, 1.0f);
CONST ColorRGB96F ColorRGB96F::Beige = ColorRGB96F(0.9607843f, 0.9607843f, 0.8627451f);
CONST ColorRGB96F ColorRGB96F::Bisque = ColorRGB96F(1.0f, 0.8941177f, 0.7686275f);
CONST ColorRGB96F ColorRGB96F::Black = ColorRGB96F(0.0f, 0.0f, 0.0f);
CONST ColorRGB96F ColorRGB96F::BlanchedAlmond = ColorRGB96F(1.0f, 0.9215686f, 0.8039216f);
CONST ColorRGB96F ColorRGB96F::Blue = ColorRGB96F(0.0f, 0.0f, 1.0f);
CONST ColorRGB96F ColorRGB96F::BlueViolet = ColorRGB96F(0.5411765f, 0.1686275f, 0.8862745f);
CONST ColorRGB96F ColorRGB96F::Brown = ColorRGB96F(0.6470588f, 0.1647059f, 0.1647059f);
CONST ColorRGB96F ColorRGB96F::BurlyWood = ColorRGB96F(0.8705882f, 0.7215686f, 0.5294118f);
CONST ColorRGB96F ColorRGB96F::CadetBlue = ColorRGB96F(0.372549f, 0.6196079f, 0.627451f);
CONST ColorRGB96F ColorRGB96F::Chartreuse = ColorRGB96F(0.4980392f, 1.0f, 0.0f);
CONST ColorRGB96F ColorRGB96F::Chocolate = ColorRGB96F(0.8235294f, 0.4117647f, 0.1176471f);
CONST ColorRGB96F ColorRGB96F::Coral = ColorRGB96F(1.0f, 0.4980392f, 0.3137255f);
CONST ColorRGB96F ColorRGB96F::CornflowerBlue = ColorRGB96F(0.3921569f, 0.5843138f, 0.9294118f);
CONST ColorRGB96F ColorRGB96F::Cornsilk = ColorRGB96F(1.0f, 0.972549f, 0.8627451f);
CONST ColorRGB96F ColorRGB96F::Crimson = ColorRGB96F(0.8627451f, 0.07843138f, 0.2352941f);
CONST ColorRGB96F ColorRGB96F::Cyan = ColorRGB96F(0.0f, 1.0f, 1.0f);
CONST ColorRGB96F ColorRGB96F::DarkBlue = ColorRGB96F(0.0f, 0.0f, 0.5450981f);
CONST ColorRGB96F ColorRGB96F::DarkCyan = ColorRGB96F(0.0f, 0.5450981f, 0.5450981f);
CONST ColorRGB96F ColorRGB96F::DarkGoldenrod = ColorRGB96F(0.7215686f, 0.5254902f, 0.04313726f);
CONST ColorRGB96F ColorRGB96F::DarkGray = ColorRGB96F(0.6627451f, 0.6627451f, 0.6627451f);
CONST ColorRGB96F ColorRGB96F::DarkGreen = ColorRGB96F(0.0f, 0.3921569f, 0.0f);
CONST ColorRGB96F ColorRGB96F::DarkKhaki = ColorRGB96F(0.7411765f, 0.7176471f, 0.4196078f);
CONST ColorRGB96F ColorRGB96F::DarkMagenta = ColorRGB96F(0.5450981f, 0.0f, 0.5450981f);
CONST ColorRGB96F ColorRGB96F::DarkOliveGreen = ColorRGB96F(0.3333333f, 0.4196078f, 0.1843137f);
CONST ColorRGB96F ColorRGB96F::DarkOrange = ColorRGB96F(1.0f, 0.5490196f, 0.0f);
CONST ColorRGB96F ColorRGB96F::DarkOrchid = ColorRGB96F(0.6f, 0.1960784f, 0.8f);
CONST ColorRGB96F ColorRGB96F::DarkRed = ColorRGB96F(0.5450981f, 0.0f, 0.0f);
CONST ColorRGB96F ColorRGB96F::DarkSalmon = ColorRGB96F(0.9137255f, 0.5882353f, 0.4784314f);
CONST ColorRGB96F ColorRGB96F::DarkSeaGreen = ColorRGB96F(0.5607843f, 0.7372549f, 0.5450981f);
CONST ColorRGB96F ColorRGB96F::DarkSlateBlue = ColorRGB96F(0.282353f, 0.2392157f, 0.5450981f);
CONST ColorRGB96F ColorRGB96F::DarkSlateGray = ColorRGB96F(0.1843137f, 0.3098039f, 0.3098039f);
CONST ColorRGB96F ColorRGB96F::DarkTurquoise = ColorRGB96F(0.0f, 0.8078431f, 0.8196079f);
CONST ColorRGB96F ColorRGB96F::DarkViolet = ColorRGB96F(0.5803922f, 0.0f, 0.827451f);
CONST ColorRGB96F ColorRGB96F::DeepPink = ColorRGB96F(1.0f, 0.07843138f, 0.5764706f);
CONST ColorRGB96F ColorRGB96F::DeepSkyBlue = ColorRGB96F(0.0f, 0.7490196f, 1.0f);
CONST ColorRGB96F ColorRGB96F::DimGray = ColorRGB96F(0.4117647f, 0.4117647f, 0.4117647f);
CONST ColorRGB96F ColorRGB96F::DodgerBlue = ColorRGB96F(0.1176471f, 0.5647059f, 1.0f);
CONST ColorRGB96F ColorRGB96F::Firebrick = ColorRGB96F(0.6980392f, 0.1333333f, 0.1333333f);
CONST ColorRGB96F ColorRGB96F::FloralWhite = ColorRGB96F(1.0f, 0.9803922f, 0.9411765f);
CONST ColorRGB96F ColorRGB96F::ForestGreen = ColorRGB96F(0.1333333f, 0.5450981f, 0.1333333f);
CONST ColorRGB96F ColorRGB96F::Fuchsia = ColorRGB96F(1.0f, 0.0f, 1.0f);
CONST ColorRGB96F ColorRGB96F::Gainsboro = ColorRGB96F(0.8627451f, 0.8627451f, 0.8627451f);
CONST ColorRGB96F ColorRGB96F::GhostWhite = ColorRGB96F(0.972549f, 0.972549f, 1.0f);
CONST ColorRGB96F ColorRGB96F::Gold = ColorRGB96F(1.0f, 0.8431373f, 0.0f);
CONST ColorRGB96F ColorRGB96F::Goldenrod = ColorRGB96F(0.854902f, 0.6470588f, 0.1254902f);
CONST ColorRGB96F ColorRGB96F::Gray = ColorRGB96F(0.5019608f, 0.5019608f, 0.5019608f);
CONST ColorRGB96F ColorRGB96F::Green = ColorRGB96F(0.0f, 0.5019608f, 0.0f);
CONST ColorRGB96F ColorRGB96F::GreenYellow = ColorRGB96F(0.6784314f, 1.0f, 0.1843137f);
CONST ColorRGB96F ColorRGB96F::Honeydew = ColorRGB96F(0.9411765f, 1.0f, 0.9411765f);
CONST ColorRGB96F ColorRGB96F::HotPink = ColorRGB96F(1.0f, 0.4117647f, 0.7058824f);
CONST ColorRGB96F ColorRGB96F::IndianRed = ColorRGB96F(0.8039216f, 0.3607843f, 0.3607843f);
CONST ColorRGB96F ColorRGB96F::Indigo = ColorRGB96F(0.2941177f, 0.0f, 0.509804f);
CONST ColorRGB96F ColorRGB96F::Ivory = ColorRGB96F(1.0f, 1.0f, 0.9411765f);
CONST ColorRGB96F ColorRGB96F::Khaki = ColorRGB96F(0.9411765f, 0.9019608f, 0.5490196f);
CONST ColorRGB96F ColorRGB96F::Lavender = ColorRGB96F(0.9019608f, 0.9019608f, 0.9803922f);
CONST ColorRGB96F ColorRGB96F::LavenderBlush = ColorRGB96F(1.0f, 0.9411765f, 0.9607843f);
CONST ColorRGB96F ColorRGB96F::LawnGreen = ColorRGB96F(0.4862745f, 0.9882353f, 0.0f);
CONST ColorRGB96F ColorRGB96F::LemonChiffon = ColorRGB96F(1.0f, 0.9803922f, 0.8039216f);
CONST ColorRGB96F ColorRGB96F::LightBlue = ColorRGB96F(0.6784314f, 0.8470588f, 0.9019608f);
CONST ColorRGB96F ColorRGB96F::LightCoral = ColorRGB96F(0.9411765f, 0.5019608f, 0.5019608f);
CONST ColorRGB96F ColorRGB96F::LightCyan = ColorRGB96F(0.8784314f, 1.0f, 1.0f);
CONST ColorRGB96F ColorRGB96F::LightGoldenrodYellow = ColorRGB96F(0.9803922f, 0.9803922f, 0.8235294f);
CONST ColorRGB96F ColorRGB96F::LightGreen = ColorRGB96F(0.5647059f, 0.9333333f, 0.5647059f);
CONST ColorRGB96F ColorRGB96F::LightGray = ColorRGB96F(0.827451f, 0.827451f, 0.827451f);
CONST ColorRGB96F ColorRGB96F::LightPink = ColorRGB96F(1.0f, 0.7137255f, 0.7568628f);
CONST ColorRGB96F ColorRGB96F::LightSalmon = ColorRGB96F(1.0f, 0.627451f, 0.4784314f);
CONST ColorRGB96F ColorRGB96F::LightSeaGreen = ColorRGB96F(0.1254902f, 0.6980392f, 0.6666667f);
CONST ColorRGB96F ColorRGB96F::LightSkyBlue = ColorRGB96F(0.5294118f, 0.8078431f, 0.9803922f);
CONST ColorRGB96F ColorRGB96F::LightSlateGray = ColorRGB96F(0.4666667f, 0.5333334f, 0.6f);
CONST ColorRGB96F ColorRGB96F::LightSteelBlue = ColorRGB96F(0.6901961f, 0.7686275f, 0.8705882f);
CONST ColorRGB96F ColorRGB96F::LightYellow = ColorRGB96F(1.0f, 1.0f, 0.8784314f);
CONST ColorRGB96F ColorRGB96F::Lime = ColorRGB96F(0.0f, 1.0f, 0.0f);
CONST ColorRGB96F ColorRGB96F::LimeGreen = ColorRGB96F(0.1960784f, 0.8039216f, 0.1960784f);
CONST ColorRGB96F ColorRGB96F::Linen = ColorRGB96F(0.9803922f, 0.9411765f, 0.9019608f);
CONST ColorRGB96F ColorRGB96F::Magenta = ColorRGB96F(1.0f, 0.0f, 1.0f);
CONST ColorRGB96F ColorRGB96F::Maroon = ColorRGB96F(0.5019608f, 0.0f, 0.0f);
CONST ColorRGB96F ColorRGB96F::MediumAquamarine = ColorRGB96F(0.4f, 0.8039216f, 0.6666667f);
CONST ColorRGB96F ColorRGB96F::MediumBlue = ColorRGB96F(0.0f, 0.0f, 0.8039216f);
CONST ColorRGB96F ColorRGB96F::MediumOrchid = ColorRGB96F(0.7294118f, 0.3333333f, 0.827451f);
CONST ColorRGB96F ColorRGB96F::MediumPurple = ColorRGB96F(0.5764706f, 0.4392157f, 0.8588235f);
CONST ColorRGB96F ColorRGB96F::MediumSeaGreen = ColorRGB96F(0.2352941f, 0.7019608f, 0.4431373f);
CONST ColorRGB96F ColorRGB96F::MediumSlateBlue = ColorRGB96F(0.4823529f, 0.4078431f, 0.9333333f);
CONST ColorRGB96F ColorRGB96F::MediumSpringGreen = ColorRGB96F(0.0f, 0.9803922f, 0.6039216f);
CONST ColorRGB96F ColorRGB96F::MediumTurquoise = ColorRGB96F(0.282353f, 0.8196079f, 0.8f);
CONST ColorRGB96F ColorRGB96F::MediumVioletRed = ColorRGB96F(0.7803922f, 0.08235294f, 0.5215687f);
CONST ColorRGB96F ColorRGB96F::MidnightBlue = ColorRGB96F(0.09803922f, 0.09803922f, 0.4392157f);
CONST ColorRGB96F ColorRGB96F::MintCream = ColorRGB96F(0.9607843f, 1.0f, 0.9803922f);
CONST ColorRGB96F ColorRGB96F::MistyRose = ColorRGB96F(1.0f, 0.8941177f, 0.8823529f);
CONST ColorRGB96F ColorRGB96F::Moccasin = ColorRGB96F(1.0f, 0.8941177f, 0.7098039f);
CONST ColorRGB96F ColorRGB96F::NavajoWhite = ColorRGB96F(1.0f, 0.8705882f, 0.6784314f);
CONST ColorRGB96F ColorRGB96F::Navy = ColorRGB96F(0.0f, 0.0f, 0.5019608f);
CONST ColorRGB96F ColorRGB96F::OldLace = ColorRGB96F(0.9921569f, 0.9607843f, 0.9019608f);
CONST ColorRGB96F ColorRGB96F::Olive = ColorRGB96F(0.5019608f, 0.5019608f, 0.0f);
CONST ColorRGB96F ColorRGB96F::OliveDrab = ColorRGB96F(0.4196078f, 0.5568628f, 0.1372549f);
CONST ColorRGB96F ColorRGB96F::Orange = ColorRGB96F(1.0f, 0.6470588f, 0.0f);
CONST ColorRGB96F ColorRGB96F::OrangeRed = ColorRGB96F(1.0f, 0.2705882f, 0.0f);
CONST ColorRGB96F ColorRGB96F::Orchid = ColorRGB96F(0.854902f, 0.4392157f, 0.8392157f);
CONST ColorRGB96F ColorRGB96F::PaleGoldenrod = ColorRGB96F(0.9333333f, 0.9098039f, 0.6666667f);
CONST ColorRGB96F ColorRGB96F::PaleGreen = ColorRGB96F(0.5960785f, 0.9843137f, 0.5960785f);
CONST ColorRGB96F ColorRGB96F::PaleTurquoise = ColorRGB96F(0.6862745f, 0.9333333f, 0.9333333f);
CONST ColorRGB96F ColorRGB96F::PaleVioletRed = ColorRGB96F(0.8588235f, 0.4392157f, 0.5764706f);
CONST ColorRGB96F ColorRGB96F::PapayaWhip = ColorRGB96F(1.0f, 0.9372549f, 0.8352941f);
CONST ColorRGB96F ColorRGB96F::PeachPuff = ColorRGB96F(1.0f, 0.854902f, 0.7254902f);
CONST ColorRGB96F ColorRGB96F::Peru = ColorRGB96F(0.8039216f, 0.5215687f, 0.2470588f);
CONST ColorRGB96F ColorRGB96F::Pink = ColorRGB96F(1.0f, 0.7529412f, 0.7960784f);
CONST ColorRGB96F ColorRGB96F::Plum = ColorRGB96F(0.8666667f, 0.627451f, 0.8666667f);
CONST ColorRGB96F ColorRGB96F::PowderBlue = ColorRGB96F(0.6901961f, 0.8784314f, 0.9019608f);
CONST ColorRGB96F ColorRGB96F::Purple = ColorRGB96F(0.5019608f, 0.0f, 0.5019608f);
CONST ColorRGB96F ColorRGB96F::Red = ColorRGB96F(1.0f, 0.0f, 0.0f);
CONST ColorRGB96F ColorRGB96F::RosyBrown = ColorRGB96F(0.7372549f, 0.5607843f, 0.5607843f);
CONST ColorRGB96F ColorRGB96F::RoyalBlue = ColorRGB96F(0.254902f, 0.4117647f, 0.8823529f);
CONST ColorRGB96F ColorRGB96F::SaddleBrown = ColorRGB96F(0.5450981f, 0.2705882f, 0.07450981f);
CONST ColorRGB96F ColorRGB96F::Salmon = ColorRGB96F(0.9803922f, 0.5019608f, 0.4470588f);
CONST ColorRGB96F ColorRGB96F::SandyBrown = ColorRGB96F(0.9568627f, 0.6431373f, 0.3764706f);
CONST ColorRGB96F ColorRGB96F::SeaGreen = ColorRGB96F(0.1803922f, 0.5450981f, 0.3411765f);
CONST ColorRGB96F ColorRGB96F::SeaShell = ColorRGB96F(1.0f, 0.9607843f, 0.9333333f);
CONST ColorRGB96F ColorRGB96F::Sienna = ColorRGB96F(0.627451f, 0.3215686f, 0.1764706f);
CONST ColorRGB96F ColorRGB96F::Silver = ColorRGB96F(0.7529412f, 0.7529412f, 0.7529412f);
CONST ColorRGB96F ColorRGB96F::SkyBlue = ColorRGB96F(0.5294118f, 0.8078431f, 0.9215686f);
CONST ColorRGB96F ColorRGB96F::SlateBlue = ColorRGB96F(0.4156863f, 0.3529412f, 0.8039216f);
CONST ColorRGB96F ColorRGB96F::SlateGray = ColorRGB96F(0.4392157f, 0.5019608f, 0.5647059f);
CONST ColorRGB96F ColorRGB96F::Snow = ColorRGB96F(1.0f, 0.9803922f, 0.9803922f);
CONST ColorRGB96F ColorRGB96F::SpringGreen = ColorRGB96F(0.0f, 1.0f, 0.4980392f);
CONST ColorRGB96F ColorRGB96F::SteelBlue = ColorRGB96F(0.2745098f, 0.509804f, 0.7058824f);
CONST ColorRGB96F ColorRGB96F::Tan = ColorRGB96F(0.8235294f, 0.7058824f, 0.5490196f);
CONST ColorRGB96F ColorRGB96F::Teal = ColorRGB96F(0.0f, 0.5019608f, 0.5019608f);
CONST ColorRGB96F ColorRGB96F::Thistle = ColorRGB96F(0.8470588f, 0.7490196f, 0.8470588f);
CONST ColorRGB96F ColorRGB96F::Tomato = ColorRGB96F(1.0f, 0.3882353f, 0.2784314f);
CONST ColorRGB96F ColorRGB96F::Turquoise = ColorRGB96F(0.2509804f, 0.8784314f, 0.8156863f);
CONST ColorRGB96F ColorRGB96F::Violet = ColorRGB96F(0.9333333f, 0.509804f, 0.9333333f);
CONST ColorRGB96F ColorRGB96F::Wheat = ColorRGB96F(0.9607843f, 0.8705882f, 0.7019608f);
CONST ColorRGB96F ColorRGB96F::White = ColorRGB96F(1.0f, 1.0f, 1.0f);
CONST ColorRGB96F ColorRGB96F::WhiteSmoke = ColorRGB96F(0.9607843f, 0.9607843f, 0.9607843f);
CONST ColorRGB96F ColorRGB96F::Yellow = ColorRGB96F(1.0f, 1.0f, 0.0f);
CONST ColorRGB96F ColorRGB96F::YellowGreen = ColorRGB96F(0.6039216f, 0.8039216f, 0.1960784f);
//-----------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorR32F::ColorR32F()
{
  m_V[0] = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorR32F::ColorR32F(FLOAT R)
{
  m_V[0] = R;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorR32F::ColorR32F(CONST ColorR32F& C)
{
  m_V[0] = C.m_V[0];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_DIRECTX9)
ColorR32F::operator D3DCOLOR() CONST
{
  return D3DCOLOR_XRGB((BYTE)(m_V[0]*255), (BYTE)(m_V[0]*255), (BYTE)(m_V[0]*255));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorR32F::operator D3DCOLORVALUE() CONST
{
  D3DCOLORVALUE V;
  V.r = m_V[0];
  V.g = m_V[0];
  V.b = m_V[0];
  V.a = 1.0f;

  return V;
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ColorR32F::Clamp()
{
  Clamp(0.0f, 1.0f);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ColorR32F::Clamp(FLOAT Min, FLOAT Max)
{
  m_V[0] = (m_V[0] > Max) ? Max : m_V[0];
  m_V[0] = (m_V[0] < Min) ? Min : m_V[0];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorR32F ColorR32F::operator + (CONST ColorR32F& C) CONST
{
  return ColorR32F(m_V[0] + C.m_V[0]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorR32F ColorR32F::operator - (CONST ColorR32F& C) CONST
{
  return ColorR32F(m_V[0] - C.m_V[0]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorR32F& ColorR32F::operator += (CONST ColorR32F& C)
{
  m_V[0] += C.m_V[0];
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorR32F& ColorR32F::operator -= (CONST ColorR32F& C)
{
  m_V[0] += C.m_V[0];
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorR32F ColorR32F::operator * (FLOAT V) CONST
{
  return ColorR32F(m_V[0] * V);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorR32F ColorR32F::operator / (FLOAT V) CONST
{
  return ColorR32F(m_V[0] / V);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorR32F& ColorR32F::operator *= (FLOAT V)
{
  m_V[0] = m_V[0] * V;
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorR32F& ColorR32F::operator /= (FLOAT V)
{
  m_V[0] = m_V[0] / V;
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT ColorR32F::Luminance() CONST
{
  return m_V[0];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorR32F ColorR32F::RandomColor()
{
	return ColorR32F(UniformRandom<FLOAT>());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String ColorR32F::ToString() CONST
{
  return String::Format(TEXT("(%f)"), m_V[0]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_LUA)
Lua::Bind::Scope ColorR32F::GenerateBindings(LPCTSTR Name)
{
  using namespace Lua::Bind;

  return 
    Class<Graphics::ColorR32F>(Name)
      .Def(Constructor<>())
      .Def(Constructor<CONST Graphics::ColorR32F&>())
      .Def(Constructor<FLOAT>())

      .Property("R", &ColorR32F::GetR, &ColorR32F::SetR)

      .Property("Luminance", &ColorR32F::Luminance)

      .Def("Clamp", (VOID(ColorR32F::*)())&ColorR32F::Clamp)
      .Def("Clamp", (VOID(ColorR32F::*)(FLOAT,FLOAT))&ColorR32F::Clamp)

      .Def(tostring(Self))
      .Def(Self + ColorR32F())
      .Def(Self - ColorR32F())
      .Def(Self * FLOAT())
      .Def(Self / FLOAT())
      ;
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------


/*




//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<> LIBRARY_API CONST FLOAT TColor<FLOAT>::MinValue = 0.0f;
template<> LIBRARY_API CONST FLOAT TColor<FLOAT>::MaxValue = 1.0f;
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<> LIBRARY_API CONST DOUBLE TColor<DOUBLE>::MinValue = 0.0;
template<> LIBRARY_API CONST DOUBLE TColor<DOUBLE>::MaxValue = 1.0;
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<> LIBRARY_API CONST BYTE TColor<BYTE>::MinValue = 0;
template<> LIBRARY_API CONST BYTE TColor<BYTE>::MaxValue = 255;
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Transparent = TColor<FLOAT>(1.0f, 1.0f, 1.0f, 0.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::AliceBlue = TColor<FLOAT>(0.9411765f, 0.972549f, 1.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::AntiqueWhite = TColor<FLOAT>(0.9803922f, 0.9215686f, 0.8431373f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Aqua = TColor<FLOAT>(0.0f, 1.0f, 1.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Aquamarine = TColor<FLOAT>(0.4980392f, 1.0f, 0.8313726f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Azure = TColor<FLOAT>(0.9411765f, 1.0f, 1.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Beige = TColor<FLOAT>(0.9607843f, 0.9607843f, 0.8627451f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Bisque = TColor<FLOAT>(1.0f, 0.8941177f, 0.7686275f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Black = TColor<FLOAT>(0.0f, 0.0f, 0.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::BlanchedAlmond = TColor<FLOAT>(1.0f, 0.9215686f, 0.8039216f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Blue = TColor<FLOAT>(0.0f, 0.0f, 1.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::BlueViolet = TColor<FLOAT>(0.5411765f, 0.1686275f, 0.8862745f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Brown = TColor<FLOAT>(0.6470588f, 0.1647059f, 0.1647059f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::BurlyWood = TColor<FLOAT>(0.8705882f, 0.7215686f, 0.5294118f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::CadetBlue = TColor<FLOAT>(0.372549f, 0.6196079f, 0.627451f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Chartreuse = TColor<FLOAT>(0.4980392f, 1.0f, 0.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Chocolate = TColor<FLOAT>(0.8235294f, 0.4117647f, 0.1176471f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Coral = TColor<FLOAT>(1.0f, 0.4980392f, 0.3137255f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::CornflowerBlue = TColor<FLOAT>(0.3921569f, 0.5843138f, 0.9294118f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Cornsilk = TColor<FLOAT>(1.0f, 0.972549f, 0.8627451f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Crimson = TColor<FLOAT>(0.8627451f, 0.07843138f, 0.2352941f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Cyan = TColor<FLOAT>(0.0f, 1.0f, 1.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::DarkBlue = TColor<FLOAT>(0.0f, 0.0f, 0.5450981f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::DarkCyan = TColor<FLOAT>(0.0f, 0.5450981f, 0.5450981f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::DarkGoldenrod = TColor<FLOAT>(0.7215686f, 0.5254902f, 0.04313726f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::DarkGray = TColor<FLOAT>(0.6627451f, 0.6627451f, 0.6627451f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::DarkGreen = TColor<FLOAT>(0.0f, 0.3921569f, 0.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::DarkKhaki = TColor<FLOAT>(0.7411765f, 0.7176471f, 0.4196078f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::DarkMagenta = TColor<FLOAT>(0.5450981f, 0.0f, 0.5450981f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::DarkOliveGreen = TColor<FLOAT>(0.3333333f, 0.4196078f, 0.1843137f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::DarkOrange = TColor<FLOAT>(1.0f, 0.5490196f, 0.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::DarkOrchid = TColor<FLOAT>(0.6f, 0.1960784f, 0.8f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::DarkRed = TColor<FLOAT>(0.5450981f, 0.0f, 0.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::DarkSalmon = TColor<FLOAT>(0.9137255f, 0.5882353f, 0.4784314f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::DarkSeaGreen = TColor<FLOAT>(0.5607843f, 0.7372549f, 0.5450981f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::DarkSlateBlue = TColor<FLOAT>(0.282353f, 0.2392157f, 0.5450981f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::DarkSlateGray = TColor<FLOAT>(0.1843137f, 0.3098039f, 0.3098039f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::DarkTurquoise = TColor<FLOAT>(0.0f, 0.8078431f, 0.8196079f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::DarkViolet = TColor<FLOAT>(0.5803922f, 0.0f, 0.827451f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::DeepPink = TColor<FLOAT>(1.0f, 0.07843138f, 0.5764706f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::DeepSkyBlue = TColor<FLOAT>(0.0f, 0.7490196f, 1.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::DimGray = TColor<FLOAT>(0.4117647f, 0.4117647f, 0.4117647f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::DodgerBlue = TColor<FLOAT>(0.1176471f, 0.5647059f, 1.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Firebrick = TColor<FLOAT>(0.6980392f, 0.1333333f, 0.1333333f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::FloralWhite = TColor<FLOAT>(1.0f, 0.9803922f, 0.9411765f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::ForestGreen = TColor<FLOAT>(0.1333333f, 0.5450981f, 0.1333333f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Fuchsia = TColor<FLOAT>(1.0f, 0.0f, 1.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Gainsboro = TColor<FLOAT>(0.8627451f, 0.8627451f, 0.8627451f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::GhostWhite = TColor<FLOAT>(0.972549f, 0.972549f, 1.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Gold = TColor<FLOAT>(1.0f, 0.8431373f, 0.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Goldenrod = TColor<FLOAT>(0.854902f, 0.6470588f, 0.1254902f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Gray = TColor<FLOAT>(0.5019608f, 0.5019608f, 0.5019608f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Green = TColor<FLOAT>(0.0f, 0.5019608f, 0.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::GreenYellow = TColor<FLOAT>(0.6784314f, 1.0f, 0.1843137f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Honeydew = TColor<FLOAT>(0.9411765f, 1.0f, 0.9411765f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::HotPink = TColor<FLOAT>(1.0f, 0.4117647f, 0.7058824f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::IndianRed = TColor<FLOAT>(0.8039216f, 0.3607843f, 0.3607843f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Indigo = TColor<FLOAT>(0.2941177f, 0.0f, 0.509804f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Ivory = TColor<FLOAT>(1.0f, 1.0f, 0.9411765f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Khaki = TColor<FLOAT>(0.9411765f, 0.9019608f, 0.5490196f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Lavender = TColor<FLOAT>(0.9019608f, 0.9019608f, 0.9803922f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::LavenderBlush = TColor<FLOAT>(1.0f, 0.9411765f, 0.9607843f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::LawnGreen = TColor<FLOAT>(0.4862745f, 0.9882353f, 0.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::LemonChiffon = TColor<FLOAT>(1.0f, 0.9803922f, 0.8039216f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::LightBlue = TColor<FLOAT>(0.6784314f, 0.8470588f, 0.9019608f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::LightCoral = TColor<FLOAT>(0.9411765f, 0.5019608f, 0.5019608f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::LightCyan = TColor<FLOAT>(0.8784314f, 1.0f, 1.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::LightGoldenrodYellow = TColor<FLOAT>(0.9803922f, 0.9803922f, 0.8235294f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::LightGreen = TColor<FLOAT>(0.5647059f, 0.9333333f, 0.5647059f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::LightGray = TColor<FLOAT>(0.827451f, 0.827451f, 0.827451f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::LightPink = TColor<FLOAT>(1.0f, 0.7137255f, 0.7568628f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::LightSalmon = TColor<FLOAT>(1.0f, 0.627451f, 0.4784314f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::LightSeaGreen = TColor<FLOAT>(0.1254902f, 0.6980392f, 0.6666667f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::LightSkyBlue = TColor<FLOAT>(0.5294118f, 0.8078431f, 0.9803922f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::LightSlateGray = TColor<FLOAT>(0.4666667f, 0.5333334f, 0.6f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::LightSteelBlue = TColor<FLOAT>(0.6901961f, 0.7686275f, 0.8705882f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::LightYellow = TColor<FLOAT>(1.0f, 1.0f, 0.8784314f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Lime = TColor<FLOAT>(0.0f, 1.0f, 0.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::LimeGreen = TColor<FLOAT>(0.1960784f, 0.8039216f, 0.1960784f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Linen = TColor<FLOAT>(0.9803922f, 0.9411765f, 0.9019608f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Magenta = TColor<FLOAT>(1.0f, 0.0f, 1.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Maroon = TColor<FLOAT>(0.5019608f, 0.0f, 0.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::MediumAquamarine = TColor<FLOAT>(0.4f, 0.8039216f, 0.6666667f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::MediumBlue = TColor<FLOAT>(0.0f, 0.0f, 0.8039216f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::MediumOrchid = TColor<FLOAT>(0.7294118f, 0.3333333f, 0.827451f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::MediumPurple = TColor<FLOAT>(0.5764706f, 0.4392157f, 0.8588235f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::MediumSeaGreen = TColor<FLOAT>(0.2352941f, 0.7019608f, 0.4431373f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::MediumSlateBlue = TColor<FLOAT>(0.4823529f, 0.4078431f, 0.9333333f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::MediumSpringGreen = TColor<FLOAT>(0.0f, 0.9803922f, 0.6039216f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::MediumTurquoise = TColor<FLOAT>(0.282353f, 0.8196079f, 0.8f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::MediumVioletRed = TColor<FLOAT>(0.7803922f, 0.08235294f, 0.5215687f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::MidnightBlue = TColor<FLOAT>(0.09803922f, 0.09803922f, 0.4392157f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::MintCream = TColor<FLOAT>(0.9607843f, 1.0f, 0.9803922f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::MistyRose = TColor<FLOAT>(1.0f, 0.8941177f, 0.8823529f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Moccasin = TColor<FLOAT>(1.0f, 0.8941177f, 0.7098039f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::NavajoWhite = TColor<FLOAT>(1.0f, 0.8705882f, 0.6784314f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Navy = TColor<FLOAT>(0.0f, 0.0f, 0.5019608f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::OldLace = TColor<FLOAT>(0.9921569f, 0.9607843f, 0.9019608f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Olive = TColor<FLOAT>(0.5019608f, 0.5019608f, 0.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::OliveDrab = TColor<FLOAT>(0.4196078f, 0.5568628f, 0.1372549f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Orange = TColor<FLOAT>(1.0f, 0.6470588f, 0.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::OrangeRed = TColor<FLOAT>(1.0f, 0.2705882f, 0.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Orchid = TColor<FLOAT>(0.854902f, 0.4392157f, 0.8392157f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::PaleGoldenrod = TColor<FLOAT>(0.9333333f, 0.9098039f, 0.6666667f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::PaleGreen = TColor<FLOAT>(0.5960785f, 0.9843137f, 0.5960785f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::PaleTurquoise = TColor<FLOAT>(0.6862745f, 0.9333333f, 0.9333333f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::PaleVioletRed = TColor<FLOAT>(0.8588235f, 0.4392157f, 0.5764706f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::PapayaWhip = TColor<FLOAT>(1.0f, 0.9372549f, 0.8352941f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::PeachPuff = TColor<FLOAT>(1.0f, 0.854902f, 0.7254902f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Peru = TColor<FLOAT>(0.8039216f, 0.5215687f, 0.2470588f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Pink = TColor<FLOAT>(1.0f, 0.7529412f, 0.7960784f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Plum = TColor<FLOAT>(0.8666667f, 0.627451f, 0.8666667f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::PowderBlue = TColor<FLOAT>(0.6901961f, 0.8784314f, 0.9019608f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Purple = TColor<FLOAT>(0.5019608f, 0.0f, 0.5019608f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Red = TColor<FLOAT>(1.0f, 0.0f, 0.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::RosyBrown = TColor<FLOAT>(0.7372549f, 0.5607843f, 0.5607843f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::RoyalBlue = TColor<FLOAT>(0.254902f, 0.4117647f, 0.8823529f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::SaddleBrown = TColor<FLOAT>(0.5450981f, 0.2705882f, 0.07450981f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Salmon = TColor<FLOAT>(0.9803922f, 0.5019608f, 0.4470588f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::SandyBrown = TColor<FLOAT>(0.9568627f, 0.6431373f, 0.3764706f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::SeaGreen = TColor<FLOAT>(0.1803922f, 0.5450981f, 0.3411765f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::SeaShell = TColor<FLOAT>(1.0f, 0.9607843f, 0.9333333f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Sienna = TColor<FLOAT>(0.627451f, 0.3215686f, 0.1764706f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Silver = TColor<FLOAT>(0.7529412f, 0.7529412f, 0.7529412f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::SkyBlue = TColor<FLOAT>(0.5294118f, 0.8078431f, 0.9215686f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::SlateBlue = TColor<FLOAT>(0.4156863f, 0.3529412f, 0.8039216f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::SlateGray = TColor<FLOAT>(0.4392157f, 0.5019608f, 0.5647059f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Snow = TColor<FLOAT>(1.0f, 0.9803922f, 0.9803922f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::SpringGreen = TColor<FLOAT>(0.0f, 1.0f, 0.4980392f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::SteelBlue = TColor<FLOAT>(0.2745098f, 0.509804f, 0.7058824f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Tan = TColor<FLOAT>(0.8235294f, 0.7058824f, 0.5490196f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Teal = TColor<FLOAT>(0.0f, 0.5019608f, 0.5019608f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Thistle = TColor<FLOAT>(0.8470588f, 0.7490196f, 0.8470588f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Tomato = TColor<FLOAT>(1.0f, 0.3882353f, 0.2784314f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Turquoise = TColor<FLOAT>(0.2509804f, 0.8784314f, 0.8156863f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Violet = TColor<FLOAT>(0.9333333f, 0.509804f, 0.9333333f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Wheat = TColor<FLOAT>(0.9607843f, 0.8705882f, 0.7019608f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::White = TColor<FLOAT>(1.0f, 1.0f, 1.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::WhiteSmoke = TColor<FLOAT>(0.9607843f, 0.9607843f, 0.9607843f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::Yellow = TColor<FLOAT>(1.0f, 1.0f, 0.0f, 1.0f);
template<> LIBRARY_API CONST TColor<FLOAT> TColor<FLOAT>::YellowGreen = TColor<FLOAT>(0.6039216f, 0.8039216f, 0.1960784f, 1.0f);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------
*/

