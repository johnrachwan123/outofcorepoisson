/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/OpenGL/Material.cpp $
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




#include "CodeLibrary.hpp"

#if defined(USE_OPENGL)

#include "Graphics/OpenGL/Material.hpp"
#include "Graphics/OpenGL/Context.hpp"

using namespace Bolitho;
using namespace Bolitho::Graphics;
using namespace Bolitho::Graphics::OpenGL;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Graphics::OpenGL,MaterialState,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MaterialState::SetAmbient(DWORD Face, CONST ColorF& C)
{
  glMaterialfv(Face, GL_AMBIENT, C);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MaterialState::SetDiffuse(DWORD Face, CONST ColorF& C)
{
  glMaterialfv(Face, GL_DIFFUSE, C);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MaterialState::SetSpecular(DWORD Face, CONST ColorF& C)
{
  glMaterialfv(Face, GL_SPECULAR, C);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MaterialState::SetEmissive(DWORD Face, CONST ColorF& C)
{
  glMaterialfv(Face, GL_EMISSION, C);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MaterialState::SetShininess(DWORD Face, FLOAT V)
{
  glMaterialf(Face, GL_SHININESS, V);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
ColorF MaterialState::GetAmbient(DWORD Face)
{
  ColorF C;
  glGetMaterialfv(Face, GL_AMBIENT, C);
  return C;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
ColorF MaterialState::GetDiffuse(DWORD Face)
{
  ColorF C;
  glGetMaterialfv(Face, GL_DIFFUSE, C);
  return C;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
ColorF MaterialState::GetSpecular(DWORD Face)
{
  ColorF C;
  glGetMaterialfv(Face, GL_SPECULAR, C);
  return C;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
ColorF MaterialState::GetEmissive(DWORD Face)
{
  ColorF C;
  glGetMaterialfv(Face, GL_EMISSION, C);
  return C;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT MaterialState::GetShininess(DWORD Face)
{
  FLOAT C;
  glGetMaterialfv(Face, GL_SHININESS, &C);
  return C;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MaterialState::operator= (CONST MATERIAL& M)
{
  SetAmbient(Face::FRONT, M.Ambient[0]);
  SetAmbient(Face::BACK, M.Ambient[1]);
  SetDiffuse(Face::FRONT, M.Diffuse[0]);
  SetDiffuse(Face::BACK, M.Diffuse[1]);
  SetSpecular(Face::FRONT, M.Specular[0]);
  SetSpecular(Face::BACK, M.Specular[1]);
  SetEmissive(Face::FRONT, M.Emissive[0]);
  SetEmissive(Face::BACK, M.Emissive[1]);
  SetShininess(Face::FRONT, M.Shininess[0]);
  SetShininess(Face::BACK, M.Shininess[1]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
MaterialState::operator MATERIAL()
{
  MATERIAL M;

  M.Ambient[0] = GetAmbient(Face::FRONT);
  M.Ambient[1] = GetAmbient(Face::BACK);
  M.Diffuse[0] = GetDiffuse(Face::FRONT);
  M.Diffuse[1] = GetDiffuse(Face::BACK);
  M.Specular[0] = GetSpecular(Face::FRONT);
  M.Specular[1] = GetSpecular(Face::BACK);
  M.Emissive[0] = GetEmissive(Face::FRONT);
  M.Emissive[1] = GetEmissive(Face::BACK);
  M.Shininess[0] = GetShininess(Face::FRONT);
  M.Shininess[1] = GetShininess(Face::BACK);

  return M;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#endif
