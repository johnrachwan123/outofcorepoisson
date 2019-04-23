/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/Raster.hpp $
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




#pragma once

#include "Utility/SizePointRect.hpp"

namespace Bolitho
{
  namespace Graphics
  {

    struct DrawingQuality
    { enum { FASTEST = 0, NEAREST = 0, BEST = 0, DEFAULT = 0 }; };

    struct PointStyle
    { enum { SQUARE = 0, ROUND = 1, DEFAULT = 0 }; };

    template<class Color>
    class Pen
    {
    public:
      Pen(CONST Color& C, FLOAT Width = 1.0f) : m_Color(C), m_Width(Width)
      {}

      Color GetColor() CONST
      { return m_Color; }

      FLOAT GetWidth() CONST
      { return m_Width; }

    private:
      Color m_Color;
      FLOAT m_Width;
    };

    template<class Color>
    class Brush
    {
    public:
      Brush(CONST Color& C) : m_Color(C)
      {}

      Color GetColor() CONST
      { return m_Color; }

    private:
      Color m_Color;

    };

    template<class Surface, class Color>
    class Raster : public Object
    {
    public:
      Raster(Surface& S) : m_Surface(S)
      {}

      /* Draws a point */
      VOID DrawPoint(PointF Pt, Pen<Color> P, DWORD Style = PointStyle::DEFAULT);

      /* Draws a line segment */
      VOID DrawLine(PointF From, PointF To, Pen<Color> P); 
      VOID DrawLine(PointF From, PointF To, Pen<Color> FromPen, Pen<Color> ToPen); 
 
      /* Draws a rectangle */
      VOID DrawRect(RectF R, Pen<Color> P);

      /* Fills a rectangle */
      VOID FillRect(RectF R, Brush<Color> B);

      /* Draws a triangle */

      /* Fills a triangle */
      VOID FillTriangle(PointF V0, PointF V1, PointF V2, Brush<Color> B);

    private:
      Surface& m_Surface;

    };

    #include "RasterImpl.hpp"
  }
}