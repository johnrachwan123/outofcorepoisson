/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/DirectX/Texture.cpp $
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
#include "Graphics/DirectX/Texture.hpp"
#include "Graphics/DirectX/DirectXException.hpp"

using namespace Bolitho;
using namespace Bolitho::Graphics;
using namespace Bolitho::Graphics::DirectX;

//------------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Graphics::DirectX,Texture,Object);
//------------------------------------------------------------------------------------------------------------------------------------------------------
Texture::Texture( LPDIRECT3DTEXTURE9 pTexture )
{
  m_pTexture = pTexture;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Texture::~Texture()
{
  if (m_pTexture)
  {
    m_pTexture->Release();
    m_pTexture = 0;
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
D3DSURFACE_DESC Texture::GetLevelDesc( UINT Level )
{
  D3DSURFACE_DESC Desc;
  HRESULT Result = m_pTexture->GetLevelDesc( Level, &Desc );
  if (FAILED(Result))
    throw RenderException(this, Result);
  return Desc;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
D3DLOCKED_RECT Texture::LockRect( UINT Level, CONST RECT * pRect, DWORD Flags )
{
  D3DLOCKED_RECT Rect;
  HRESULT Result = m_pTexture->LockRect( Level, &Rect, pRect, Flags );
  if (FAILED(Result))
    throw RenderException( this, Result );
  
  return Rect;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID Texture::UnlockRect( UINT Level )
{
  HRESULT Result = m_pTexture->UnlockRect( Level );
  if (FAILED(Result))
    throw RenderException( this, Result );
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Surface> Texture::GetSurfaceLevel( UINT Level )
{
  LPDIRECT3DSURFACE9 pSurface = 0;
  HRESULT Result = m_pTexture->GetSurfaceLevel( Level, &pSurface );
  if (FAILED(Result))
    throw RenderException( this, Result );

  return new Surface(pSurface);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------

#endif
