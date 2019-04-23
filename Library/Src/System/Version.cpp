/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/Version.cpp $
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

#include "System/Version.hpp"

using namespace Bolitho;
//----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho,Version,Object);
//----------------------------------------------------------------------------------------------------------------------------------------------------
Version::Version()
{
  m_Major = 0;
  m_Minor = 0;
  m_Build = 0;
  m_Revision = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Version::Version(USHORT Major, USHORT Minor, USHORT Build, USHORT Revision)
{
  m_Major = Major;
  m_Minor = Minor;
  m_Build = Build;
  m_Revision = Revision;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Version::Version(DWORD Upper, DWORD Lower)
{
  m_Major = (USHORT)(Upper >> 16);
  m_Minor = (USHORT)(Upper & 0xffff);
  m_Build = (USHORT)(Lower >> 16);
  m_Revision = (USHORT)(Lower & 0xffff);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Version::Version(LPCTSTR VersionString)
{
  Array<String> VersionParts = String(VersionString).Split('.');
  
  m_Major = Parse<USHORT>(VersionParts[0]);
  m_Minor = Parse<USHORT>(VersionParts[1]);
  m_Build = Parse<USHORT>(VersionParts[2]);
  m_Revision = Parse<USHORT>(VersionParts[3]);

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
String Version::ToString() CONST
{
  return String::Format(TEXT("%d.%d.%d.%d"), m_Major, m_Minor, m_Build, m_Revision);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
