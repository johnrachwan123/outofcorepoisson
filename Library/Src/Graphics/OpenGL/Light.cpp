/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/OpenGL/Light.cpp $
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

#if defined(USE_OPENGL)

#include "Graphics/OpenGL/Light.hpp"
#include "Graphics/OpenGL/Context.hpp"

using namespace Bolitho;
using namespace Bolitho::Math;
using namespace Bolitho::Graphics;
using namespace Bolitho::Graphics::OpenGL;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Graphics::OpenGL,LightState,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LightState::LightState(UINT LightId)
{
  m_LightId = GL_LIGHT0 + LightId;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID LightState::SetAmbient(CONST ColorF& C)
{
  glLightfv(m_LightId, GL_AMBIENT, C);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID LightState::SetDiffuse(CONST ColorF& C)
{
  glLightfv(m_LightId, GL_DIFFUSE, C);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID LightState::SetSpecular(CONST ColorF& C)
{
  glLightfv(m_LightId, GL_SPECULAR, C);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID LightState::SetPosition(CONST Vector4<FLOAT>& V)
{
  glLightfv(m_LightId, GL_POSITION, V);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID LightState::SetPosition(CONST Vector3<FLOAT>& V)
{
  SetPosition(Vector4<FLOAT>(V[0], V[1], V[2], 1));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID LightState::SetDirection(CONST Math::Vector3<FLOAT>& V)
{
  glLightfv(m_LightId, GL_SPOT_DIRECTION, V);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID LightState::Enable(BOOL B)
{
  if (B)
    glEnable(m_LightId);
  else
    glDisable(m_LightId);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID LightState::Disable()
{
  Enable(FALSE);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
ColorF LightState::GetAmbient()
{
  ColorF C;
  glGetLightfv(m_LightId, GL_AMBIENT, C);
  return C;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
ColorF LightState::GetDiffuse()
{
  ColorF C;
  glGetLightfv(m_LightId, GL_DIFFUSE, C);
  return C;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
ColorF LightState::GetSpecular()
{
  ColorF C;
  glGetLightfv(m_LightId, GL_SPECULAR, C);
  return C;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Vector4F LightState::GetPosition()
{
  Vector4F V;
  glGetLightfv(m_LightId, GL_POSITION, V);
  return V;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Vector3F LightState::GetDirection()
{
  Vector3F V;
  glGetLightfv(m_LightId, GL_SPOT_DIRECTION, V);
  return V;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL LightState::IsEnabled()
{
  return glIsEnabled(m_LightId);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LightState::operator LIGHT()
{
  LIGHT L;
  L.Enabled = IsEnabled();
  L.Ambient = GetAmbient();
  L.Diffuse = GetDiffuse();
  L.Specular = GetSpecular();
  L.Position = GetPosition();
  L.Direction = GetDirection();

  return L;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID LightState::operator= (CONST LIGHT& L)
{
  Enable(L.Enabled);
  SetAmbient(L.Ambient);
  SetDiffuse(L.Diffuse);
  SetSpecular(L.Specular);
  SetPosition(L.Position);
  SetDirection(L.Direction);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID LightingState::SetGlobalAmbient(CONST ColorF& C)
{
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, C);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID LightingState::EnableLighting(BOOL B)
{
  if (B)
    glEnable(GL_LIGHTING);
  else
    glDisable(GL_LIGHTING);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID LightingState::EnableTwoSidedLighting(BOOL B)
{
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, B);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID LightingState::Enable()
{
  EnableLighting(TRUE);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID LightingState::Disable()
{
  EnableLighting(FALSE);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL LightingState::IsLightingEnabled()
{
  return glIsEnabled(GL_LIGHTING);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL LightingState::IsTwoSidedLightingEnabled()
{
  BOOL B;
  glGetIntegerv(GL_LIGHT_MODEL_TWO_SIDE, &B);
  return B;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
ColorF LightingState::GetGlobalAmbient()
{
  ColorF C;
  glGetFloatv(GL_LIGHT_MODEL_AMBIENT, C);
  return C;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LightingState::operator LIGHTING()
{
  LIGHTING L;

  L.Enabled = IsLightingEnabled();
  L.TwoSided = IsTwoSidedLightingEnabled();
  L.GlobalAmbient = GetGlobalAmbient();

  return L;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID LightingState::operator= (CONST LIGHTING& L)
{
  EnableLighting(L.Enabled);
  EnableTwoSidedLighting(L.TwoSided);
  SetGlobalAmbient(L.GlobalAmbient);


}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

#endif