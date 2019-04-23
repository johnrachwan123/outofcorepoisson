/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/RasterImpl.hpp $
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




//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class Surface, class Color>
VOID Raster<Surface,Color>::DrawPoint(PointF Pt, Pen<Color> P, DWORD Style)
{
  switch(Style)
  {
  case PointStyle::SQUARE:
    {
      SIZE_T mx = (SIZE_T)(Pt.x - P.GetWidth() / 2.0f);
      SIZE_T my = (SIZE_T)(Pt.y - P.GetWidth() / 2.0f);
      SIZE_T Mx = (SIZE_T)(Pt.x + P.GetWidth() / 2.0f);
      SIZE_T My = (SIZE_T)(Pt.y + P.GetWidth() / 2.0f);

      mx = Clamp(mx, (SIZE_T)0, m_Surface.Width());
      my = Clamp(my, (SIZE_T)0, m_Surface.Height());
      Mx = Clamp(Mx, (SIZE_T)0, m_Surface.Width());
      My = Clamp(My, (SIZE_T)0, m_Surface.Height());


      for (SIZE_T i=mx; i<Mx; i++)
        for (SIZE_T j=my; j<My; j++)
          m_Surface(i,j) = P.GetColor();

      break;
    }
  case PointStyle::ROUND:
    {
      SIZE_T mx = (SIZE_T)(Pt.x - P.GetWidth() / 2.0f);
      SIZE_T my = (SIZE_T)(Pt.y - P.GetWidth() / 2.0f);
      SIZE_T Mx = (SIZE_T)(Pt.x + P.GetWidth() / 2.0f);
      SIZE_T My = (SIZE_T)(Pt.y + P.GetWidth() / 2.0f);

      mx = Clamp(mx, (SIZE_T)0, m_Surface.Width());
      my = Clamp(my, (SIZE_T)0, m_Surface.Height());
      Mx = Clamp(Mx, (SIZE_T)0, m_Surface.Width());
      My = Clamp(My, (SIZE_T)0, m_Surface.Height());

      FLOAT MaxR2 = (P.GetWidth() / 2.0f) * (P.GetWidth() / 2.0f);
      for (SIZE_T i=mx; i<Mx; i++)
        for (SIZE_T j=my; j<My; j++)
        {
          FLOAT R2 = (i-Pt.x)*(i-Pt.x) + (j-Pt.y)*(j-Pt.y);
          if (R2 < MaxR2)
            m_Surface(i,j) = P.GetColor();
        }

        break;
    }
  default:
    throw InvalidOperationException(this, "Unknown Style");
  };
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class Surface, class Color>
VOID Raster<Surface,Color>::DrawLine(PointF From, PointF To, Pen<Color> P)
{

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class Surface, class Color>
VOID Raster<Surface,Color>::DrawLine(PointF From, PointF To, Pen<Color> FromPen, Pen<Color> ToPen)
{

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class Surface, class Color>
VOID Raster<Surface,Color>::DrawRect(RectF R, Pen<Color> P)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class Surface, class Color>
VOID Raster<Surface,Color>::FillRect(RectF R, Brush<Color> B)
{

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class Surface, class Color>
VOID Raster<Surface,Color>::FillTriangle(PointF v1, PointF v2, PointF v3, Brush<Color> B)
{
#define iround (INT)

  // 28.4 fixed-point coordinates
  CONST INT Y1 = iround(16.0f * v1.y);
  CONST INT Y2 = iround(16.0f * v2.y);
  CONST INT Y3 = iround(16.0f * v3.y);

  CONST INT X1 = iround(16.0f * v1.x);
  CONST INT X2 = iround(16.0f * v2.x);
  CONST INT X3 = iround(16.0f * v3.x);

  // Deltas
  CONST INT DX12 = X1 - X2;
  CONST INT DX23 = X2 - X3;
  CONST INT DX31 = X3 - X1;

  CONST INT DY12 = Y1 - Y2;
  CONST INT DY23 = Y2 - Y3;
  CONST INT DY31 = Y3 - Y1;

  // Fixed-poINT deltas
  CONST INT FDX12 = DX12 << 4;
  CONST INT FDX23 = DX23 << 4;
  CONST INT FDX31 = DX31 << 4;

  CONST INT FDY12 = DY12 << 4;
  CONST INT FDY23 = DY23 << 4;
  CONST INT FDY31 = DY31 << 4;

  // Bounding rectangle
  INT minx = (Min(X1, X2, X3) + 0xF) >> 4;
  INT maxx = (Max(X1, X2, X3) + 0xF) >> 4;
  INT miny = (Min(Y1, Y2, Y3) + 0xF) >> 4;
  INT maxy = (Max(Y1, Y2, Y3) + 0xF) >> 4;

  // Block size, standard 8x8 (must be power of two)
  CONST INT q = 8;

  // Start in corner of 8x8 block
  minx &= ~(q - 1);
  miny &= ~(q - 1);

  Color* colorBuffer = &m_Surface(0,0);
  SIZE_T stride = m_Surface.Width();

  (Color*&)colorBuffer += miny * stride;

  // Half-edge CONSTants
  INT C1 = DY12 * X1 - DX12 * Y1;
  INT C2 = DY23 * X2 - DX23 * Y2;
  INT C3 = DY31 * X3 - DX31 * Y3;

  // Correct for fill convention
  if(DY12 < 0 || (DY12 == 0 && DX12 > 0)) C1++;
  if(DY23 < 0 || (DY23 == 0 && DX23 > 0)) C2++;
  if(DY31 < 0 || (DY31 == 0 && DX31 > 0)) C3++;

  // Loop through blocks
  for(INT y = miny; y < maxy; y += q)
  {
    for(INT x = minx; x < maxx; x += q)
    {
      // Corners of block
      INT x0 = x << 4;
      INT x1 = (x + q - 1) << 4;
      INT y0 = y << 4;
      INT y1 = (y + q - 1) << 4;

      // Evaluate half-space functions
      bool a00 = C1 + DX12 * y0 - DY12 * x0 > 0;
      bool a10 = C1 + DX12 * y0 - DY12 * x1 > 0;
      bool a01 = C1 + DX12 * y1 - DY12 * x0 > 0;
      bool a11 = C1 + DX12 * y1 - DY12 * x1 > 0;
      INT a = (a00 << 0) | (a10 << 1) | (a01 << 2) | (a11 << 3);

      bool b00 = C2 + DX23 * y0 - DY23 * x0 > 0;
      bool b10 = C2 + DX23 * y0 - DY23 * x1 > 0;
      bool b01 = C2 + DX23 * y1 - DY23 * x0 > 0;
      bool b11 = C2 + DX23 * y1 - DY23 * x1 > 0;
      INT b = (b00 << 0) | (b10 << 1) | (b01 << 2) | (b11 << 3);

      bool c00 = C3 + DX31 * y0 - DY31 * x0 > 0;
      bool c10 = C3 + DX31 * y0 - DY31 * x1 > 0;
      bool c01 = C3 + DX31 * y1 - DY31 * x0 > 0;
      bool c11 = C3 + DX31 * y1 - DY31 * x1 > 0;
      INT c = (c00 << 0) | (c10 << 1) | (c01 << 2) | (c11 << 3);

      // Skip block when outside an edge
      if(a == 0x0 || b == 0x0 || c == 0x0) continue;

      Color* buffer = colorBuffer;

      // Accept whole block when totally covered
      if(a == 0xF && b == 0xF && c == 0xF)
      {
        for(INT iy = 0; iy < q; iy++)
        {
          for(INT ix = x; ix < x + q; ix++)
          {
            buffer[ix] = Color::Green; // << // Green
          }

          (Color*&)buffer += stride;
        }
      }
      else //<< // Partially covered block
      {
        INT CY1 = C1 + DX12 * y0 - DY12 * x0;
        INT CY2 = C2 + DX23 * y0 - DY23 * x0;
        INT CY3 = C3 + DX31 * y0 - DY31 * x0;

        for(INT iy = y; iy < y + q; iy++)
        {
          INT CX1 = CY1;
          INT CX2 = CY2;
          INT CX3 = CY3;

          for(INT ix = x; ix < x + q; ix++)
          {
            if(CX1 > 0 && CX2 > 0 && CX3 > 0)
            {
              buffer[ix] = Color::Blue; // << // Blue
            }

            CX1 -= FDY12;
            CX2 -= FDY23;
            CX3 -= FDY31;
          }

          CY1 += FDX12;
          CY2 += FDX23;
          CY3 += FDX31;

          (Color*&)buffer += stride;
        }
      }
    }

    (Color*&)colorBuffer += q * stride;
  }
}