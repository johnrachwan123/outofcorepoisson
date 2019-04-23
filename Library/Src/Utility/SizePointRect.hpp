/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Utility/SizePointRect.hpp $
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

#if defined(PLATFORM_POSIX)
  struct tagRECT
  {
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
  };

  struct tagPOINT
  {
    LONG x;
    LONG y;
  };

  struct tagSIZE
  {
    LONG cx;
    LONG cy;
  };
#endif

  struct LIBRARY_API Point : public tagPOINT
  {
    Point()
    { x = 0; y = 0; }
    
    Point(LONG X, LONG Y)
    { x = X; y = Y; }
    
    Point(LPARAM L)
    { x = (SHORT)LOWORD(L); y = (SHORT)HIWORD(L); }
    
    Point operator - (CONST Point& P)
    { return Point(x-P.x, y-P.y); }
    
    Point operator + (CONST Point& P)
    { return Point(x+P.x, y+P.y); }
    
    LONG& operator[] (SIZE_T i)
    { if (i == 0) return x; if (i == 1) return y; Assert(false); return y; }
    
    CONST LONG& operator[] (SIZE_T i) CONST
    { if (i == 0) return x; if (i == 1) return y; Assert(false); return y; }
  };
  
  struct LIBRARY_API PointF 
  {
    PointF()
    { x = 0; y = 0; }
    
    PointF(FLOAT X, FLOAT Y)
    { x = X; y = Y; }
    
    PointF operator - (CONST PointF& P)
    { return PointF(x-P.x, y-P.y); }
    
    PointF operator + (CONST PointF& P)
    { return PointF(x+P.x, y+P.y); }
    
    PointF operator - (CONST Point& P)
    { return PointF(x-P.x, y-P.y); }
    
    PointF operator + (CONST Point& P)
    { return PointF(x+P.x, y+P.y); }
    
    PointF operator * (FLOAT f)
    { return PointF(x*f, y*f); }
    
    PointF operator / (FLOAT f)
    { return PointF(x/f, y/f); }
    
    FLOAT& operator[] (SIZE_T i)
    { if (i == 0) return x; if (i == 1) return y; Assert(false); return y; }
    
    CONST FLOAT& operator[] (SIZE_T i) CONST
    { if (i == 0) return x; if (i == 1) return y; Assert(false); return y;}
    
    FLOAT x,y;
  };
  
  inline PointF operator- (CONST Point& A, CONST PointF& B)
  { return PointF(A.x-B.x, A.y-B.y); }
  
  struct LIBRARY_API Size : public tagSIZE
  {
    Size()
    { cx = 0; cy = 0; }
    Size(LONG Cx, LONG Cy)
    { cx = Cx; cy = Cy; }
    
    LONG& operator[] (SIZE_T i)
    { if (i == 0) return cx; if (i == 1) return cy; Assert(false); return cy; }
    
    CONST LONG& operator[] (SIZE_T i) CONST
    { if (i == 0) return cx; if (i == 1) return cy; Assert(false); return cy; }
  };
  
  struct LIBRARY_API SizeF
  {
    SizeF()
    { cx = 0; cy = 0; }
    SizeF(FLOAT Cx, FLOAT Cy)
    { cx = Cx; cy = Cy; }
    
    FLOAT& operator[] (SIZE_T i)
    { if (i == 0) return cx; if (i == 1) return cy; Assert(false); return cy; }
    
    CONST FLOAT& operator[] (SIZE_T i) CONST
    { if (i == 0) return cx; if (i == 1) return cy; Assert(false); return cy; }
  private:
    FLOAT cx, cy;
    
  };

  struct LIBRARY_API Rect : public tagRECT
  {
  public:
    Rect()
    { left = 0; top = 0; right = 0; bottom = 0; }
    Rect(LONG L, LONG T, LONG R, LONG B)
    { left = L; top = T; right = R; bottom = B; }
    Rect(CONST tagRECT& R)
    { left = R.left; top = R.top; right = R.right; bottom = R.bottom; }
    
    LONG Width() CONST
    { return right-left; }
    LONG Height() CONST
    { return bottom-top; }
    
    Point TopLeft() CONST
    { return Point(left, top); }
    Point TopRight() CONST
    { return Point(right, top); }
    Point BottomLeft() CONST
    { return Point(left, bottom); }
    Point BottomRight() CONST
    { return Point(right, bottom); }
  };
  
  struct LIBRARY_API RectF
  {
  public:
    RectF()
    { left = 0; top = 0; right = 0; bottom = 0; }
    RectF(FLOAT L, FLOAT T, FLOAT R, FLOAT B)
    { left = L; top = T; right = R; bottom = B; }
    RectF(CONST Rect& R)
    { left = (FLOAT)R.left; top = (FLOAT)R.top; right = (FLOAT)R.right; bottom = (FLOAT)R.bottom; }
    RectF(CONST RectF& R)
    { left = R.left; top = R.top; right = R.right; bottom = R.bottom; }
    
    FLOAT Width() CONST
    { return right-left; }
    FLOAT Height() CONST
    { return bottom-top; }
   
    PointF TopLeft() CONST
    { return PointF(left, top); }
    PointF TopRight() CONST
    { return PointF(right, top); }
    PointF BottomLeft() CONST
    { return PointF(left, bottom); }
    PointF BottomRight() CONST
    { return PointF(right, bottom); }

  private:
    FLOAT left, top, right, bottom;
  };

}

