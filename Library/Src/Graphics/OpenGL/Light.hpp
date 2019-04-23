/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/OpenGL/Light.hpp $
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

#if defined(USE_OPENGL)

#include "Math/Vector4.hpp"
#include "Math/Vector3.hpp"
#include "Graphics/Color.hpp"
using Bolitho::Math::Vector3F;
using Bolitho::Math::Vector4F;

namespace Bolitho
{
  namespace Graphics
  {
    namespace OpenGL
    {
      struct LIBRARY_API LIGHT
      {
        ColorF Ambient;
        ColorF Diffuse;
        ColorF Specular;
        Vector4F Position;
        Vector3F Direction;
        BOOL Enabled;
      };

      struct LIBRARY_API LIGHTING
      {
        BOOL Enabled;
        BOOL TwoSided;
        ColorF GlobalAmbient;
        LIGHT Light[8];
      };

      class Context;

      class LIBRARY_API LightingState
      {
      public:
        VOID SetGlobalAmbient(CONST ColorF& C);
        VOID EnableLighting(BOOL B);
        VOID EnableTwoSidedLighting(BOOL B);

        VOID Enable();
        VOID Disable();

        BOOL IsLightingEnabled();
        BOOL IsTwoSidedLightingEnabled();
        ColorF GetGlobalAmbient();

        operator LIGHTING();
        VOID operator= (CONST LIGHTING& L);
      };

      class LIBRARY_API LightState : public Object
      {
        friend class Context;
        DeclareRuntimeType();
      public:
        VOID Enable(BOOL B = TRUE);
        VOID Disable();

        BOOL IsEnabled();

        VOID SetAmbient(CONST ColorF& C);
        VOID SetDiffuse(CONST ColorF& C);
        VOID SetSpecular(CONST ColorF& C);
        VOID SetPosition(CONST Math::Vector4<FLOAT>& V);
        VOID SetPosition(CONST Math::Vector3<FLOAT>& V);
        VOID SetDirection(CONST Math::Vector3<FLOAT>& V);

        ColorF GetAmbient();
        ColorF GetDiffuse();
        ColorF GetSpecular();
        Vector4F GetPosition();
        Vector3F GetDirection();

        operator LIGHT();
        VOID operator= (CONST LIGHT& L);
      protected:
        LightState(UINT LightId);
        virtual ~LightState() {}

        UINT m_LightId;
      };

      template<UINT i>
      class TLightState : public LightState
      {
      public:
        TLightState() : LightState(i)
        {}
      };

    }
  }
}

#endif
