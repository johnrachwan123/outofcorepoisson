/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Net/Uri.cpp $
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
#include "Net/Uri.hpp"

using namespace Bolitho;
using namespace Bolitho::Net;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
Uri::Uri(LPCTSTR AbsoluteUri)
{
  /* <protocol>://<server>/<path>?<query>#<fragment> */
  
  String U(AbsoluteUri);
  
  String::Iterator i = U.Find(TEXT("://"));
  
  if (i == U.End())
    throw InvalidUriException();

  m_Protocol = U.Substr(U.Begin(), i);
  U = U.Substr(i+3, U.End());

  i = U.Find('/');
  if (i == U.End())
  {
    m_Server = U;
    return;
  }
  
  m_Server = U.Substr(U.Begin(), i);
  U = U.Substr(i+1, U.End());

  i = U.Find('?');
  if (i == U.End())
  {
    m_Path = U;
    return;
  }

  m_Path = U.Substr(U.Begin(), i);
  U = U.Substr(i+1, U.End());

  i = U.Find('#');
  if (i == U.End())
  {
    m_Query = U;
    return;
  }

  m_Query = U.Substr(U.Begin(), i);
  U = U.Substr(i+1, U.End());

  if (!U.IsEmpty())
    m_Fragment = U;

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Uri::Uri(CONST Uri& Context, LPCTSTR RelativeUri)
{
  /* <relativepath>?<query>#<fragment> */


}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Uri::Uri(LPCTSTR Protocol, LPCTSTR Server, LPCTSTR Path, LPCTSTR Query, LPCTSTR Fragment)
{
  m_Protocol = Protocol;
  m_Server = Server;
  m_Port = 0;
  m_Path = Path;
  m_Query = Query;
  m_Fragment = Fragment;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Uri::Uri(LPCTSTR Protocol, LPCTSTR Server, USHORT Port, LPCTSTR Path, LPCTSTR Query, LPCTSTR Fragment)
{
  m_Protocol = Protocol;
  m_Server = Server;
  m_Port = Port;
  m_Path = Path;
  m_Query = Query;
  m_Fragment = Fragment;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Uri::ToString()
{
  String U = m_Protocol + "://" + m_Server + "/";

  if (!m_Path.IsEmpty())
    U += m_Path;
  if (!m_Query.IsEmpty())
  {
    U += '?';
    U += m_Query;
  }
  if (!m_Fragment.IsEmpty())
  {
    U += '#';
    U += m_Fragment;
  }

  return U;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------
