/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/DirectX/Device.hpp $
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

#include "Utility/SizePointRect.hpp"
#include "UI/Window.hpp"
#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"
#include "Math/Matrix4.hpp"

#include "Graphics/DirectX/Surface.hpp"
#include "Graphics/DirectX/Texture.hpp"
#include "Graphics/DirectX/VertexShader.hpp"
#include "Graphics/DirectX/PixelShader.hpp"
#include "Graphics/DirectX/VertexBuffer.hpp"
#include "Graphics/DirectX/IndexBuffer.hpp"
#include "Graphics/DirectX/VertexDeclaration.hpp"
#include "Graphics/DirectX/Material.hpp"
#include "Graphics/DirectX/Light.hpp"

namespace Bolitho
{
  namespace Graphics
  {
    using Math::Vector2;
    using Math::Vector3;
    using Math::Matrix3;
    using Math::Matrix4;

    namespace DirectX
    {
      class DeviceTransform
      {
      public:
        DeviceTransform(D3DTRANSFORMSTATETYPE TransformType, Device* pDevice)
        { m_pDevice = pDevice; m_TransformType = TransformType; }

        VOID operator = (CONST Matrix4<FLOAT>& M);
        VOID operator = (CONST D3DMATRIX* pMatrix);

      private:
        Device* m_pDevice;
        D3DTRANSFORMSTATETYPE m_TransformType;
      };

      struct CullMode
      { enum { NONE = D3DCULL_NONE, CLOCKWISE = D3DCULL_CW, COUNTERCLOCKWISE = D3DCULL_CCW }; };

      struct PrimitiveType
      { enum { POINTLIST = D3DPT_POINTLIST, LINELIST = D3DPT_LINELIST, LINESTRIP = D3DPT_LINESTRIP, TRIANGLELIST = D3DPT_TRIANGLELIST, TRIANGLESTRIP = D3DPT_TRIANGLESTRIP, TRIANGLEFAN = D3DPT_TRIANGLEFAN }; };


      class RenderControl;
      class LIBRARY_API Device : public Object
      {
        friend class RenderControl;

        DeclareRuntimeType();
      public:
        /* Creates a device for a user-created window */
        Device(Ptr<UI::Window> pWindow);
        
        /* Creates a off-screen device with a specified size */
        Device(Size S);

        virtual ~Device();

        /* Resizes the device */
        VOID Resize( Size S );

				Vector3<FLOAT> UnProject(Vector2<FLOAT> ScreenSpace);
				Vector3<FLOAT> UnProject(Vector2<FLOAT> ScreenSpace, Matrix4<FLOAT> Projection, Matrix4<FLOAT> View );
        Vector3<FLOAT> UnProject(Vector3<FLOAT> ScreenSpaceWithZ);
        Vector3<FLOAT> UnProject(Vector3<FLOAT> ScreenSpaceWithZ, Matrix4<FLOAT> Projection, Matrix4<FLOAT> View );
        Vector3<FLOAT> Project(Vector3<FLOAT> WorldSpace);
        Vector3<FLOAT> Project(Vector3<FLOAT> WorldSpace, Matrix4<FLOAT> Projection, Matrix4<FLOAT> View );

        /* Wrapping some Device calls ... */
        VOID Clear(ColorF Color);
        VOID Clear(ColorF Color, FLOAT Depth);
        VOID Clear( DWORD Flags, D3DCOLOR Color, FLOAT Z, DWORD Stencil );
        VOID BeginScene();
        VOID EndScene();
        VOID Present();

        /* Render state */
        DWORD GetRenderState(D3DRENDERSTATETYPE State);
        VOID SetRenderState(D3DRENDERSTATETYPE State, DWORD Value);

        VOID EnableLighting();
        VOID DisableLighting();

        VOID EnableDepthTest();
        VOID DisableDepthTest();

        VOID SetCullMode(DWORD CullMode);

        /* Transforms */
        DeviceTransform View;
        DeviceTransform World;
        DeviceTransform Projection;

        VOID SetTransform( D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix );
        VOID GetTransform( D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix );
        VOID SetTransform( D3DTRANSFORMSTATETYPE State, CONST Matrix4<FLOAT>& Matrix );
        Matrix4<FLOAT> GetTransform( D3DTRANSFORMSTATETYPE State );
        
        VOID SetSamplerState( DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value );

        Ptr<VertexBuffer> CreateVertexBuffer( UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool );
        Ptr<IndexBuffer> CreateIndexBuffer( UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool );
        Ptr<Texture> CreateTexture( UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool );
        Ptr<VertexDeclaration> CreateVertexDeclaration( CONST D3DVERTEXELEMENT9* pVertexElements );

        Ptr<VertexShader> CreateVertexShader( CONST Buffer<DWORD>& Tokens );
        Ptr<VertexShader> CreateVertexShader( CONST DWORD* pTokens, SIZE_T Length );
        Ptr<VertexShader> CreateVertexShader( LPCSTR Source, LPCSTR FunctionName, LPCSTR Profile, DWORD Flags );
        Ptr<PixelShader> CreatePixelShader( CONST Buffer<DWORD>& Tokens );
        Ptr<PixelShader> CreatePixelShader( CONST DWORD* pTokens, SIZE_T Length );
        Ptr<PixelShader> CreatePixelShader( LPCSTR Source, LPCSTR FunctionName, LPCSTR Profile, DWORD Flags );

				Ptr<Surface> CreateRenderTarget( UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable );
				Ptr<Surface> CreateOffscreenPlainSurface( UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool );

				VOID SetRenderTarget(DWORD Index, Ptr<Surface> pSurface);
				VOID GetRenderTargetData(Ptr<Surface> pFromSurface, Ptr<Surface> pToSurface);

				Ptr<Surface> GetRenderTarget(DWORD Index);
				Ptr<Surface> GetBackBuffer(UINT SwapChain, UINT Index);
        Ptr<Surface> GetDepthStencil();
				VOID UpdateSurface(Ptr<Surface> pSourceSurface, CONST RECT* pSourceRect, Ptr<Surface> pDestinationSurface, CONST POINT* pDestinationPoint);
        VOID SetDepthSencil(Ptr<Surface> pSurface);
				
				D3DVIEWPORT9 GetViewport();
				VOID SetViewport(CONST D3DVIEWPORT9* pViewport);

				BOOL TestCooperativeLevel();
       
        VOID SetVertexDeclaration(Ptr<VertexDeclaration> pDeclaration);
        VOID SetVertexShader(Ptr<VertexShader> pShader);
        VOID SetPixelShader(Ptr<PixelShader> pShader);
        VOID SetIndices(Ptr<IndexBuffer> pIndices);
        VOID SetStreamSource(UINT StreamNumber, Ptr<VertexBuffer> pStreamData, UINT OffsetInBytes, UINT Stride);
        VOID SetTexture(UINT StreamNumber, Ptr<Texture> pTexture);

        VOID DrawIndexedPrimitive( DWORD PrimitiveType, INT BaseVertexIndex, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount );
        VOID DrawPrimitive( DWORD PrimitiveType, UINT StartVertex, UINT PrimitiveCount );

        /* Sets the current light properties */
        VOID SetLight(DWORD Index, CONST Light& L);
        VOID LightEnable(DWORD Index, BOOL V = TRUE);

        /* Lights */
        DeviceLight Light0;
        DeviceLight Light1;
        DeviceLight Light2;
        DeviceLight Light3;
        DeviceLight Light4;
        DeviceLight Light5;
        DeviceLight Light6;
        DeviceLight Light7;

        /* Sets the current material properties */
        VOID SetMaterial(CONST Material& M);
        
        /* Surface Material */
        DeviceMaterial Material;

        /* Returns the underly COM interface */
				LPDIRECT3DDEVICE9 GetRawInterface()
				{ return m_pDevice; }

        /* Returns the present parameters the device is configured with */
				CONST D3DPRESENT_PARAMETERS* GetPresentParameters() CONST
				{ return &m_D3DPresentParam; }
        /* Returns the width of the back buffer */
        UINT GetBackBufferWidth()
        { return m_D3DPresentParam.BackBufferWidth; }
        /* Returns the height of the back buffer */
        UINT GetBackBufferHeight()
        { return m_D3DPresentParam.BackBufferHeight; }
        /* Returns the pixel format of the back buffer */
        D3DFORMAT GetBackBufferFormat()
        { return m_D3DPresentParam.BackBufferFormat; }

        /* Returns the version number of the driver being used */
        Version GetDriverVersion() CONST
        { return m_DriverVersion; }
        /* Returns the date of the driver being used */
        DateTime GetDriverDate() CONST
        { return m_DriverDate; }
        /* Returns a string description of the hardware being used */
        String GetHardwareDescription() CONST
        { return m_HardwareDescription; }
        /* Returns the name of the hardware manufacturer */
        String GetHardwareManufacturer() CONST
        { return m_HardwareManufacturer; }
        /* Returns the name of the hardware chipset */
        String GetHardwareChipset() CONST
        { return m_HardwareChipset; }
        /* Returns the amount of dedicated graphics memory present */
        SIZE_T GetHardwareDedicatedMemory() CONST
        { return m_HardwareDedicatedMemory; }
        /* Returns the total amount of graphics memory present */
        SIZE_T GetHardwareTotalMemory() CONST
        { return m_HardwareTotalMemory; }

			protected:
        D3DPRESENT_PARAMETERS m_D3DPresentParam;
        LPDIRECT3DDEVICE9 m_pDevice;

        BOOL m_DeviceLost;

        Ptr<UI::Window> m_pWindow;
        VOID Create(Ptr<UI::Window> pWindow);

        /* Hardware information discovered at device creation time */
        Version m_DriverVersion;
        DateTime m_DriverDate;
        String m_HardwareDescription;
        String m_HardwareManufacturer;
        String m_HardwareChipset;
        SIZE_T m_HardwareDedicatedMemory;
        SIZE_T m_HardwareTotalMemory;
      };
    }
  }
}

#endif 
