/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/DirectX/RenderControl.cpp $
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
#include "Graphics/DirectX/RenderControl.hpp"
#include "Graphics/DirectX/DirectXException.hpp"

using namespace Bolitho;
using namespace Bolitho::Graphics;
using namespace Bolitho::Graphics::DirectX;
using namespace Bolitho::UI;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Graphics::DirectX,RenderControl,Control);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<UI::WindowClass> RenderControl::m_pDxWndClass;
//-----------------------------------------------------------------------------------------------------------------------------------------------------
RenderControl::RenderControl()
{
  m_InteractiveSizing = FALSE;

  if (m_pDxWndClass == 0)
    m_pDxWndClass = WindowClass::Create( TEXT("RenderControl"), CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS, 0, 0, Cursor::LoadFromStock(IDC_ARROW) );
  else
    m_pDxWndClass.AddRef();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL RenderControl::Create( Ptr<UI::Window> pParent )
{
  Window::Create( m_pDxWndClass, TEXT(""), WS_CHILDWINDOW, WS_EX_CLIENTEDGE, pParent, 0, Point(0,0), Point(100,100) );
  m_pDevice = new Device(this);
  OnResetDevice();

  return m_hWnd != 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
RenderControl::~RenderControl()
{
  OnLostDevice();
  m_pDxWndClass.Release();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT RenderControl::OnPaint( DeviceContext* pDC, Rect UpdateRect )
{
  if (!m_pDevice->TestCooperativeLevel())
    return 0;

  OnUpdate(m_Timer.ElapsedSeconds());
  m_Timer.Start();

  OnRender();

  return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID RenderControl::SetSize(LONG W, LONG H)
{
  Control::SetSize(W,H);

  if (m_pDevice)
  {
    OnLostDevice();
    m_pDevice->Resize(Size(W,H));
    OnResetDevice();
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID RenderControl::SetSize(Size S)
{
  Control::SetSize(S);

  if (m_pDevice)
  {
    OnLostDevice();
    m_pDevice->Resize(S);
    OnResetDevice();
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT RenderControl::OnSize( UINT Type, Size S )
{
  /*
  if (m_pDevice && m_InteractiveSizing)
  {
    m_InteractiveSizing = FALSE;
    OnLostDevice();
    m_pDevice->Resize(GetClientSize());
    OnResetDevice();
    Invalidate();
  }
  m_InteractiveSizing = TRUE;
  */
  return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT RenderControl::OnBeginSizeOrMove()
{
  return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT RenderControl::OnEndSizeOrMove()
{
  m_InteractiveSizing = TRUE;
  if (m_pDevice && m_InteractiveSizing)
  {
    m_InteractiveSizing = FALSE;

    Size C = GetClientSize();
    if (C.cx != m_pDevice->GetBackBufferWidth() || C.cy != m_pDevice->GetBackBufferHeight())
    {
      OnLostDevice();
      m_pDevice->Resize(C);
      OnResetDevice();
      Invalidate();
    }
  }
  return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

#endif
