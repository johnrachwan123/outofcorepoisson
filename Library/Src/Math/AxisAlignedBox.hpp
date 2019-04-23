/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/AxisAlignedBox.hpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 1738 $
 * Last Updated: $Date: 2008-12-23 12:31:52 -0800 (Tue, 23 Dec 2008) $
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

#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"

#include "Math/Line.hpp"

namespace Bolitho
{
  namespace Math
  {
    template<class T>
    class AxisAlignedBox2
    {
    public:
      AxisAlignedBox2();
      AxisAlignedBox2(CONST Vector2<T>& Min, CONST Vector2<T>& Max);

      CONST Vector2<T>& Min() CONST
      { return m_Minimum; }
      Vector2<T>& Min()
      { return m_Minimum; }

      CONST Vector2<T>& Max() CONST
      { return m_Maximum; }
      Vector2<T>& Max()
      { return m_Maximum; }

    private:
      Vector2<T> m_Minimum;
      Vector2<T> m_Maximum;
    };

    template<class T>
    class AxisAlignedBox3
    {
    public:
      AxisAlignedBox3();
      AxisAlignedBox3(CONST Vector3<T>& Min, CONST Vector3<T>& Max);

      CONST Vector3<T>& Min() CONST
      { return m_Minimum; }
      Vector3<T>& Min()
      { return m_Minimum; }

      CONST T& Min(UINT i) CONST
      { return m_Minimum[i]; }
      T& Min(UINT i)
      { return m_Minimum[i]; }

      CONST Vector3<T>& Max() CONST
      { return m_Maximum; }
      Vector3<T>& Max()
      { return m_Maximum; }

      CONST T& Max(UINT i) CONST
      { return m_Maximum[i]; }
      T& Max(UINT i)
      { return m_Maximum[i]; }

      Vector3<T> Center() CONST
      { return m_Minimum + 0.5f*(m_Maximum-m_Minimum); }
      Vector3<T> Diagonal() CONST
      { return m_Maximum-m_Minimum; }

      VOID Union(CONST Vector3<T>& V);
      VOID Union(CONST AxisAlignedBox3<T>& B);

      STATIC CONST AxisAlignedBox3<T> Empty;

      BOOL Intersect(Ray3<T>& R, Vector3<T>& IntersectionPosition) CONST;
    private:
      Vector3<T> m_Minimum;
      Vector3<T> m_Maximum;
    };

    typedef AxisAlignedBox2<FLOAT> AxisAlignedBox2F;
    typedef AxisAlignedBox2<DOUBLE> AxisAlignedBox2D;
    typedef AxisAlignedBox3<FLOAT> AxisAlignedBox3F;
    typedef AxisAlignedBox3<DOUBLE> AxisAlignedBox3D;

    #include "AxisAlignedBoxImpl.hpp"
  }
}
