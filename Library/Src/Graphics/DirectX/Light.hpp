/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/DirectX/Light.hpp $
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

#if defined(USE_DIRECTX9)

#include "Math/Vector3.hpp"
#include "Graphics/Color.hpp"

namespace Bolitho
{
  namespace Graphics
  {
    using Math::Vector3;

    namespace DirectX
    {
      class LIBRARY_API Light
      {
      public:
        Light();

      protected:
        D3DLIGHTTYPE m_Type;
        ColorF m_Diffuse;
        ColorF m_Specular;
        ColorF m_Ambient;
        Vector3<FLOAT> m_Position;
        Vector3<FLOAT> m_Direction;
        FLOAT m_Range;
        FLOAT m_Falloff;
        FLOAT m_Attenuation0;
        FLOAT m_Attenuation1;
        FLOAT m_Attenuation2;
        FLOAT m_Theta;
        FLOAT m_Phi;
      };

      class LIBRARY_API PointLight : public Light
      {
      public:
        PointLight()
        { m_Type = D3DLIGHT_POINT; }

        ColorF& Diffuse()
        { return m_Diffuse; }
        ColorF& Specular()
        { return m_Specular; }
        ColorF& Ambient()
        { return m_Ambient; }

        Vector3<FLOAT>& Position()
        { return m_Position; }

        FLOAT& Range()
        { return m_Range; }

        FLOAT& Attenuation0()
        { return m_Attenuation0; }
        FLOAT& Attenuation1()
        { return m_Attenuation1; }
        FLOAT& Attenuation2()
        { return m_Attenuation2; }
                
      };

      class LIBRARY_API DirectionalLight : public Light
      {
      public:
        DirectionalLight()
        { m_Type = D3DLIGHT_DIRECTIONAL; }

        ColorF& Diffuse()
        { return m_Diffuse; }
        ColorF& Specular()
        { return m_Specular; }
        ColorF& Ambient()
        { return m_Ambient; }

        Vector3<FLOAT>& Direction()
        { return m_Direction; }

      };

      class LIBRARY_API SpotLight : public Light
      {
      public:
        SpotLight()
        { m_Type = D3DLIGHT_SPOT; }

        ColorF& Diffuse()
        { return m_Diffuse; }
        ColorF& Specular()
        { return m_Specular; }
        ColorF& Ambient()
        { return m_Ambient; }

        Vector3<FLOAT>& Position()
        { return m_Position; }
        Vector3<FLOAT>& Direction()
        { return m_Direction; }

        FLOAT& Range()
        { return m_Range; }

        FLOAT& Attenuation0()
        { return m_Attenuation0; }
        FLOAT& Attenuation1()
        { return m_Attenuation1; }
        FLOAT& Attenuation2()
        { return m_Attenuation2; }

        FLOAT& Falloff()
        { return m_Falloff; }
        FLOAT& Theta()
        { return m_Theta; }
        FLOAT& Phi()
        { return m_Phi; }

      };

      class Device;
      class DeviceLight
      {
      public:
        DeviceLight(DWORD LightIndex, Device* pDevice)
        { m_LightIndex = LightIndex; m_pDevice = pDevice; }

        /* Enables/Disables the light */
        VOID Enable();
        VOID Disable();

        /* Sets light properties */
        VOID operator = (CONST Light& L);

      private:
        Device* m_pDevice;
        DWORD m_LightIndex;
      };

    }
  }
}

#endif