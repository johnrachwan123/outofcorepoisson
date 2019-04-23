/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/DirectX/Shader.cpp $
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

#include "Graphics/DirectX.hpp"
#include "Graphics/DirectX/Shader.hpp"
#include "Graphics/DirectX/DirectXException.hpp"

using namespace Bolitho;
using namespace Bolitho::Graphics;
using namespace Bolitho::Graphics::DirectX;

//------------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Graphics::DirectX,Shader,Object);
//------------------------------------------------------------------------------------------------------------------------------------------------------
Shader::Shader( LPDIRECT3DDEVICE9 pDevice, CONST Buffer<DWORD>& Tokens )
{
  m_pDevice = pDevice;
  pDevice->AddRef();
  m_Buffer = Tokens;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Shader::Shader( LPDIRECT3DDEVICE9 pDevice, CONST DWORD* pTokens, SIZE_T Length )
{
  m_pDevice = pDevice;
  pDevice->AddRef();
  m_Buffer = Buffer<DWORD>(pTokens, Length);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Shader::Shader( LPDIRECT3DDEVICE9 pDevice, LPCSTR Source, LPCSTR FunctionName, LPCSTR Profile, DWORD Flags )
{
  m_pDevice = pDevice;
  pDevice->AddRef();

  LPD3DXBUFFER pBuffer;
  LPD3DXBUFFER pMessages;
  HRESULT Result = D3DXCompileShader( Source, (UINT)CharTraits<CHAR>::Length(Source), NULL, NULL, FunctionName, Profile, Flags, &pBuffer, &pMessages, &m_pConstants );  
  if (Result == E_FAIL)
  {
    StringA Messages( (CHAR*)pMessages->GetBufferPointer() );
    pMessages->Release();
    throw RenderException( this, Messages.ToString() );
  }

  if (FAILED(Result))
    throw RenderException( this, Result );



  m_Buffer = Buffer<DWORD>((CONST DWORD*)pBuffer->GetBufferPointer(), pBuffer->GetBufferSize());
  
  pBuffer->Release();

}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Shader::~Shader()
{
  m_pDevice->Release();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID Shader::SetConstant( LPCSTR Name, FLOAT f )
{
  D3DXHANDLE hConst = m_pConstants->GetConstantByName( NULL, Name );
  HRESULT Result = m_pConstants->SetFloat( m_pDevice, hConst, f );
  if (FAILED(Result))
    throw RenderException( this, Result );
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID Shader::SetConstant( LPCSTR Name, CONST FLOAT* p, UINT N )
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID Shader::SetConstant( LPCSTR Name, INT i )
{
  D3DXHANDLE hConst = m_pConstants->GetConstantByName( NULL, Name );
  m_pConstants->SetInt( m_pDevice, hConst, i );
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID Shader::SetConstant( LPCSTR Name, CONST INT* p, UINT N )
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID Shader::SetConstant( LPCSTR Name, CONST D3DXMATRIX* M )
{
  D3DXHANDLE hConst = m_pConstants->GetConstantByName( NULL, Name );
	if (hConst != 0)
		m_pConstants->SetMatrix( m_pDevice, hConst, M );
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID Shader::SetConstant( LPCSTR Name, CONST Math::Matrix4<FLOAT>& M )
{
  D3DXHANDLE hConst = m_pConstants->GetConstantByName( NULL, Name );
	if (hConst != 0)
		m_pConstants->SetMatrix( m_pDevice, hConst, (CONST D3DXMATRIX*)&M );
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID Shader::SetConstant( LPCSTR Name, CONST D3DXMATRIX* M, UINT N )
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------

#endif