/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/DirectX/Device.cpp $
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




#include "CodeLibrary.hpp"


#if defined(USE_DIRECTX9)

#include "Debug/Trace.hpp"

#include "Graphics/DirectX.hpp"
#include "Graphics/DirectX/Device.hpp"
#include "Graphics/DirectX/DirectXException.hpp"
#include "Graphics/DirectX/RenderControl.hpp"

using namespace Bolitho;
using namespace Bolitho::Math;
using namespace Bolitho::Graphics;
using namespace Bolitho::Graphics::DirectX;
using namespace Bolitho::UI;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Graphics::DirectX,Device,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID DeviceTransform::operator = (CONST Matrix4<FLOAT>& M)
{
  m_pDevice->SetTransform(m_TransformType, (CONST D3DMATRIX*)&M.Transpose());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID DeviceTransform::operator = (CONST D3DMATRIX* pMatrix)
{
  m_pDevice->SetTransform(m_TransformType, pMatrix);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
#pragma warning(suppress:4355)
Device::Device(Size S) : Material(this), Light0(0, this), Light1(1, this), Light2(2, this), Light3(3, this), Light4(4, this), Light5(5, this), Light6(6, this), Light7(7, this), View(D3DTS_VIEW, this), World(D3DTS_WORLD, this), Projection(D3DTS_PROJECTION, this)
{
  m_pWindow = new Window();
  m_pWindow->Create( Window::m_pDefaultWindowClass, TEXT(""), 0, 0, Ptr<Window>::Null, 0, Point(0,0), Point(S.cx, S.cy) );
  m_pWindow->Show();

  Create(m_pWindow);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#pragma warning(suppress:4355)
Device::Device(Ptr<Window> pWindow) : Material(this), Light0(0, this), Light1(1, this), Light2(2, this), Light3(3, this), Light4(4, this), Light5(5, this), Light6(6, this), Light7(7, this), View(D3DTS_VIEW, this), World(D3DTS_WORLD, this), Projection(D3DTS_PROJECTION, this)
{
  Create(pWindow);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::Create(Ptr<Window> pWindow)
{
  HRESULT Result;
  LPDIRECT3D9 pD3D = Direct3DCreate9(D3D_SDK_VERSION);

  if (pD3D == 0)
    throw RenderException(this, TEXT("Unable to create IDirect3D9*"));

  D3DDISPLAYMODE d3ddm;
  Result = pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
  if (FAILED(Result))
    throw RenderException(this, Result);

  ZeroMemory(&m_D3DPresentParam, sizeof(D3DPRESENT_PARAMETERS));

  m_D3DPresentParam.Windowed = TRUE;
  m_D3DPresentParam.hDeviceWindow = pWindow->Handle();
  m_D3DPresentParam.BackBufferFormat = d3ddm.Format;
  m_D3DPresentParam.SwapEffect = D3DSWAPEFFECT_COPY;

  Result = pD3D->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3ddm.Format, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16 );

  if( Result == S_OK )
  {
    m_D3DPresentParam.EnableAutoDepthStencil = TRUE;
    m_D3DPresentParam.AutoDepthStencilFormat = D3DFMT_D16;
  }
  else
  {
    m_D3DPresentParam.EnableAutoDepthStencil = FALSE;
  }

  Result = pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, pWindow->Handle(), D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_D3DPresentParam, &m_pDevice);
  if (FAILED(Result))
  {
    Result = pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, pWindow->Handle(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_D3DPresentParam, &m_pDevice);
    if (FAILED(Result))
    {
      Result = pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, pWindow->Handle(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_D3DPresentParam, &m_pDevice);
      if (FAILED(Result))
        throw RenderException(this, Result);
    }
  }

  pD3D->Release();

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Device::~Device()
{
  if (m_pDevice)
    m_pDevice->Release();
  m_pDevice = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Device::TestCooperativeLevel()
{
  HRESULT Result = m_pDevice->TestCooperativeLevel();

  if (Result == D3DERR_DEVICELOST)
  {
    m_DeviceLost = TRUE;
    return FALSE;
  }

  if (Result == D3DERR_DEVICENOTRESET)
  {
    m_pDevice->Reset( &m_D3DPresentParam );
    m_DeviceLost = FALSE;
  }
  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::Clear( DWORD Flags, D3DCOLOR Color, FLOAT Z, DWORD Stencil )
{
  HRESULT Result = m_pDevice->Clear( 0, NULL, Flags, Color, Z, Stencil );
  if (FAILED(Result))
    throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::Clear(ColorF Color)
{
  Clear(D3DCLEAR_TARGET, Color, 0.0f, 0);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::Clear(ColorF Color, FLOAT Depth)
{
  Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, Color, Depth, 0);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::BeginScene()
{
  HRESULT Result = m_pDevice->BeginScene();
  if (FAILED(Result))
    throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::EndScene()
{
  HRESULT Result = m_pDevice->EndScene();
  if (FAILED(Result))
    throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::Present()
{
  HRESULT Result = m_pDevice->Present( 0, 0, 0, 0 );
  if (FAILED(Result))
    throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Surface> Device::GetDepthStencil()
{
  LPDIRECT3DSURFACE9 pSurface = 0;
  HRESULT Result = m_pDevice->GetDepthStencilSurface( &pSurface );
  if (FAILED(Result))
    throw RenderException( this, Result );

  return new Surface(pSurface);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<VertexBuffer> Device::CreateVertexBuffer( UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool )
{
  LPDIRECT3DVERTEXBUFFER9 pVertexBuffer = 0;
  HRESULT Result = m_pDevice->CreateVertexBuffer( Length, Usage, FVF, Pool, &pVertexBuffer, 0 );
  if (FAILED(Result))
    throw RenderException( this, Result );

  return new VertexBuffer(pVertexBuffer);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<IndexBuffer> Device::CreateIndexBuffer( UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool )
{
  LPDIRECT3DINDEXBUFFER9 pIndexBuffer = 0;
  HRESULT Result = m_pDevice->CreateIndexBuffer( Length, Usage, Format, Pool, &pIndexBuffer, 0 );
  if (FAILED(Result))
    throw RenderException( this, Result );

  return new IndexBuffer(pIndexBuffer);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Texture> Device::CreateTexture( UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool )
{
  LPDIRECT3DTEXTURE9 pTexture = 0;
  HRESULT Result = m_pDevice->CreateTexture( Width, Height, Levels, Usage, Format, Pool, &pTexture, 0 );
  if (FAILED(Result))
    throw RenderException( this, Result );

  return new Texture(pTexture);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<VertexDeclaration> Device::CreateVertexDeclaration( CONST D3DVERTEXELEMENT9* pVertexElements )
{
  LPDIRECT3DVERTEXDECLARATION9 pVertexDeclaration = 0;
  HRESULT Result = m_pDevice->CreateVertexDeclaration( pVertexElements, &pVertexDeclaration );
  if (FAILED(Result))
    throw RenderException( this, Result );
  
  return new VertexDeclaration(pVertexDeclaration);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<VertexShader> Device::CreateVertexShader( LPCSTR Source, LPCSTR FunctionName, LPCSTR Profile, DWORD Flags )
{
  return new VertexShader( m_pDevice, Source, FunctionName, Profile, Flags );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<VertexShader> Device::CreateVertexShader( CONST DWORD* pTokens, SIZE_T Length )
{
  return new VertexShader( m_pDevice, pTokens, Length );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<VertexShader> Device::CreateVertexShader( CONST Buffer<DWORD>& Tokens )
{
  return new VertexShader( m_pDevice, Tokens );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<PixelShader> Device::CreatePixelShader( LPCSTR Source, LPCSTR FunctionName, LPCSTR Profile, DWORD Flags )
{
  return new PixelShader( m_pDevice, Source, FunctionName, Profile, Flags );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<PixelShader> Device::CreatePixelShader( CONST DWORD* pTokens, SIZE_T Length )
{
  return new PixelShader( m_pDevice, pTokens, Length );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<PixelShader> Device::CreatePixelShader( CONST Buffer<DWORD>& Tokens )
{
  return new PixelShader( m_pDevice, Tokens );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::SetVertexDeclaration( Ptr<VertexDeclaration> pDeclaration )
{
  HRESULT Result = m_pDevice->SetVertexDeclaration( pDeclaration->m_pVertexDeclaration );
  if (FAILED(Result))
    throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::SetTexture( UINT StreamNumber, Ptr<Texture> pTexture )
{
  HRESULT Result = m_pDevice->SetTexture( 0, pTexture->m_pTexture );
  if (FAILED(Result))
    throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::SetIndices( Ptr<IndexBuffer> pIndices )
{
  HRESULT Result = m_pDevice->SetIndices( pIndices->m_pIndexBuffer );
  if (FAILED(Result))
    throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::SetStreamSource( UINT StreamNumber, Ptr<VertexBuffer> pStreamData, UINT OffsetInBytes, UINT Stride )
{
  HRESULT Result = m_pDevice->SetStreamSource( StreamNumber, pStreamData->m_pVertexBuffer, OffsetInBytes, Stride );
  if (FAILED(Result))
    throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::SetVertexShader( Ptr<VertexShader> pShader )
{
  HRESULT Result = m_pDevice->SetVertexShader( pShader->m_pVertexShader );
  if (FAILED(Result))
    throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::SetPixelShader( Ptr<PixelShader> pShader )
{
  HRESULT Result = m_pDevice->SetPixelShader( pShader->m_pPixelShader );
  if (FAILED(Result))
    throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::DrawIndexedPrimitive( DWORD PrimitiveType, INT BaseVertexIndex, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount )
{
  HRESULT Result = m_pDevice->DrawIndexedPrimitive( (D3DPRIMITIVETYPE)PrimitiveType, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount );
  if (FAILED(Result))
    throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::DrawPrimitive( DWORD PrimitiveType, UINT StartVertex, UINT PrimitiveCount )
{
  HRESULT Result = m_pDevice->DrawPrimitive( (D3DPRIMITIVETYPE)PrimitiveType, StartVertex, PrimitiveCount );
  if (FAILED(Result))
    throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD Device::GetRenderState( D3DRENDERSTATETYPE State )
{
  DWORD Value = 0;
  HRESULT Result = m_pDevice->GetRenderState( State, &Value );
  if (FAILED(Result))
    throw RenderException( this, Result );
  return Value;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::SetSamplerState( DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value )
{
  HRESULT Result = m_pDevice->SetSamplerState( Sampler, Type, Value );
  if (FAILED(Result))
    throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::SetRenderState( D3DRENDERSTATETYPE State, DWORD Value )
{
  HRESULT Result = m_pDevice->SetRenderState( State, Value );
  if (FAILED(Result))
    throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::EnableLighting()
{
  SetRenderState(D3DRS_LIGHTING, TRUE);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::DisableLighting()
{
  SetRenderState(D3DRS_LIGHTING, FALSE);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::EnableDepthTest()
{
  SetRenderState(D3DRS_ZENABLE, TRUE);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::DisableDepthTest()
{
  SetRenderState(D3DRS_ZENABLE, FALSE);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::SetCullMode(DWORD CullMode)
{
  SetRenderState(D3DRS_CULLMODE, CullMode);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::SetTransform( D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix )
{
  HRESULT Result = m_pDevice->SetTransform( State, pMatrix );
  if (FAILED(Result))
    throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::SetTransform( D3DTRANSFORMSTATETYPE State, CONST Matrix4<FLOAT>& Matrix )
{
  Matrix4<FLOAT> M = Matrix.Transpose();
	SetTransform( State, (CONST D3DMATRIX*)&M[0] );	
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::GetTransform( D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix )
{
  HRESULT Result = m_pDevice->GetTransform( State, pMatrix );
  if (FAILED(Result))
    throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Matrix4<FLOAT> Device::GetTransform( D3DTRANSFORMSTATETYPE State )
{
  return Matrix4<FLOAT>();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::Resize( Size S )
{
  if (m_D3DPresentParam.BackBufferWidth != S.cx || m_D3DPresentParam.BackBufferHeight != S.cy)
  {
    m_D3DPresentParam.BackBufferWidth = S.cx;
    m_D3DPresentParam.BackBufferHeight = S.cy;
		m_pDevice->Reset( &m_D3DPresentParam );
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Surface> Device::CreateRenderTarget( UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable )
{
	LPDIRECT3DSURFACE9 pSurface = 0;
	HRESULT Result = m_pDevice->CreateRenderTarget( Width, Height, Format, MultiSample, MultisampleQuality, Lockable, &pSurface, NULL );
	if (FAILED(Result))
		throw RenderException( this, Result );
	return new Surface(pSurface);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::SetRenderTarget( DWORD Index, Ptr<Surface> pSurface )
{
	HRESULT Result = m_pDevice->SetRenderTarget( Index, pSurface->m_pSurface );
	if (FAILED(Result))
		throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::GetRenderTargetData( Ptr<Surface> pFromSurface, Ptr<Surface> pToSurface )
{
	HRESULT Result = m_pDevice->GetRenderTargetData( pFromSurface->m_pSurface, pToSurface->m_pSurface );
	if (FAILED(Result))
		throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Surface> Device::GetRenderTarget( DWORD Index )
{
	LPDIRECT3DSURFACE9 pSurface = 0;
	HRESULT Result = m_pDevice->GetRenderTarget( Index, &pSurface );
	if (FAILED(Result))
		throw RenderException( this, Result );
	return new Surface(pSurface);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Surface> Device::CreateOffscreenPlainSurface( UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool )
{
	LPDIRECT3DSURFACE9 pSurface = 0;
	HRESULT Result = m_pDevice->CreateOffscreenPlainSurface( Width, Height, Format, Pool, &pSurface, NULL );
	if (FAILED(Result))
		throw RenderException( this, Result );

	return new Surface(pSurface);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Surface> Device::GetBackBuffer( UINT SwapChain, UINT Index )
{
	LPDIRECT3DSURFACE9 pSurface = 0;
	HRESULT Result = m_pDevice->GetBackBuffer( SwapChain, Index, D3DBACKBUFFER_TYPE_MONO, &pSurface );
	if (FAILED(Result))
		throw RenderException( this, Result );

	return new Surface(pSurface);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::SetDepthSencil( Ptr<Surface> pSurface )
{
  HRESULT Result = m_pDevice->SetDepthStencilSurface(pSurface->m_pSurface);
  if (FAILED(Result))
    throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::UpdateSurface( Ptr<Surface> pSourceSurface, CONST RECT* pSourceRect, Ptr<Surface> pDestinationSurface, CONST POINT* pDestinationPoint )
{
	HRESULT Result = m_pDevice->UpdateSurface( pSourceSurface->m_pSurface, pSourceRect, pDestinationSurface->m_pSurface, pDestinationPoint );
	if (FAILED(Result))
		throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
D3DVIEWPORT9 Device::GetViewport()
{
	D3DVIEWPORT9 Viewport;
	HRESULT Result = m_pDevice->GetViewport(&Viewport);
	if (FAILED(Result))
		throw RenderException( this, Result );

	return Viewport;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::SetViewport( CONST D3DVIEWPORT9* pViewport )
{
	HRESULT Result = m_pDevice->SetViewport(pViewport);
	if (FAILED(Result))
		throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::SetLight(DWORD Index, CONST Light& L)
{
  HRESULT Result = m_pDevice->SetLight(Index, (D3DLIGHT9*)&L);
	if (FAILED(Result))
		throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::LightEnable( DWORD Index, BOOL V )
{
  HRESULT Result = m_pDevice->LightEnable( Index, V );
	if (FAILED(Result))
		throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Device::SetMaterial(CONST DirectX::Material& M)
{
  HRESULT Result = m_pDevice->SetMaterial((D3DMATERIAL9*)&M);
	if (FAILED(Result))
		throw RenderException( this, Result );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Vector3F Device::UnProject( Vector2<FLOAT> ScreenSpace, Matrix4<FLOAT> Projection, Matrix4<FLOAT> View )
{
  return Vector3F(0,0,0);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Vector3F Device::UnProject( Vector3<FLOAT> V, Matrix4<FLOAT> Projection, Matrix4<FLOAT> View )
{
	D3DVIEWPORT9 Viewport;
	D3DXMATRIX World;
	D3DXVECTOR3 Out;

	m_pDevice->GetViewport( &Viewport );
	D3DXMatrixIdentity(&World);	
	D3DXVec3Unproject( &Out, (CONST D3DXVECTOR3*)&V[0], &Viewport, (CONST D3DXMATRIX*)&Projection[0], (CONST D3DXMATRIX*)&View[0], &World );
	
	return Vector3F( Out[0], Out[1], Out[2] );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Vector3<FLOAT> Device::Project( Vector3F WorldSpace, Matrix4F Projection, Matrix4F View )
{
	return Vector3F(0,0,0);
}


#endif