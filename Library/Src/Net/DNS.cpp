/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Net/DNS.cpp $
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

#include "Net/DNS.hpp"

#if defined(USE_SOCKETS)

using namespace Bolitho;
using namespace Bolitho::Net;

//----------------------------------------------------------------------------------------------------------------------------------------------------
String Dns::GetHostName()
{
  char HostName[128];
  ZeroMemory(HostName, 128);
  int Result = gethostname(HostName, 128);

  return String(HostName);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
String Dns::GetHostByAddress(LPCTSTR Address)
{
  return GetHostByAddress(IPAddress(Address));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
String Dns::GetHostByAddress(IPAddress Address)
{
  hostent* pRemoteHost;
  pRemoteHost = gethostbyaddr((char*)&Address.m_Address, 4, AF_INET);
  if (pRemoteHost)
    return String(pRemoteHost->h_name);
  else
    return Address.ToString();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
IPAddress Dns::Resolve(LPCTSTR HostName)
{
  addrinfo* pAddressInfo;
  addrinfo Hints;

  ZeroMemory(&Hints, sizeof(addrinfo));
  Hints.ai_family = AF_INET;
  Hints.ai_socktype = SOCK_STREAM;
  Hints.ai_protocol = IPPROTO_TCP;

#if defined(UNICODE)
  StringA AnsiHostName(HostName);
  int Result = getaddrinfo(AnsiHostName.ConstStr(), 0, &Hints, &pAddressInfo);
#else
  int Result = getaddrinfo(HostName, 0, &Hints, &pAddressInfo);
#endif

  if (Result != 0)
    return IPAddress(INADDR_NONE);

  sockaddr_in* pAddress = (sockaddr_in*)pAddressInfo->ai_addr;
#if defined(PLATFORM_WINDOWS)
  ULONG Address = pAddress->sin_addr.S_un.S_addr;
#else
  ULONG Address = pAddress->sin_addr.s_addr;
#endif
  freeaddrinfo(pAddressInfo);

  return IPAddress(Address);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

#endif
