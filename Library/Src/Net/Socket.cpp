/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Net/Socket.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 1732 $
 * Last Updated: $Date: 2008-12-17 16:21:03 -0800 (Wed, 17 Dec 2008) $
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

#if defined(USE_SOCKETS)

#include "Net/Socket.hpp"
#include "Debug/Trace.hpp"

using namespace Bolitho;
using namespace Bolitho::Net;

//----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Net,Socket,Object);
ImplementRuntimeType(Bolitho::Net,SocketException,Exception);
//----------------------------------------------------------------------------------------------------------------------------------------------------
Socket::Socket(DWORD AddressFamily, DWORD SocketType, DWORD ProtocolType) : m_EndPoint(IPAddress::Any(), 0)
{
  m_Socket = socket(AddressFamily, SocketType, ProtocolType);
  if (m_Socket == -1)
    throw SocketException(this, SystemException::GetError(this));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Socket::Socket(SOCKETHANDLE S) : m_EndPoint(IPAddress::Any(), 0)
{
  m_Socket = S;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Socket::~Socket()
{
  Close();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Socket::Connect(IPEndPoint EndPoint)
{
  IPAddress IPAdd = EndPoint.Address();
  
  sockaddr_in Name;
  ZeroMemory(&Name, sizeof(Name));
  Name.sin_family = AF_INET;
  Name.sin_port = htons(EndPoint.Port());
  CopyMemory(&Name.sin_addr, &IPAdd.m_Address, sizeof(in_addr));
  if (connect(m_Socket, (sockaddr*)&Name, sizeof(Name)) == -1)
    throw SocketException(this, SystemException::GetError(this));
  m_EndPoint = EndPoint;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Socket::Listen()
{
  if (listen(m_Socket, SOMAXCONN) == -1)
  {
    SystemException::ThrowError(this);
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Socket::Bind(IPEndPoint EndPoint)
{
  IPAddress IPAdd = EndPoint.Address();
  
  sockaddr_in Name;
  ZeroMemory(&Name, sizeof(Name));
  Name.sin_family = AF_INET;
  Name.sin_port = htons(EndPoint.Port());
  CopyMemory(&Name.sin_addr, &IPAdd.m_Address, sizeof(in_addr));
  if (bind(m_Socket, (sockaddr*)&Name, sizeof(Name)) == -1)
    throw SocketException(this, SystemException::GetError(this));
  
  m_EndPoint = EndPoint;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Socket> Socket::Accept()
{
  #if defined(PLATFORM_WINDOWS)
  typedef int socklen_t;
  #endif

  sockaddr_in Remote;
  socklen_t Length = sizeof(Remote);
  
  SOCKETHANDLE S = accept(m_Socket, (sockaddr*)&Remote, &Length);
  Socket* pSocket = new Socket(S);
  pSocket->m_EndPoint = IPEndPoint(IPAddress(Remote.sin_addr.s_addr), Remote.sin_port);
  
  return pSocket;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Socket::Recieve(LPVOID pBuffer, SIZE_T Length, BOOL WaitForCompleteBuffer)
{
#if PLATFORM_WINDOWS < PLATFORM_WINDOWS_VISTA
  int Result = recv(m_Socket, (char*)pBuffer, (int)Length, 0);
#else
  int Result = recv(m_Socket, (char*)pBuffer, (int)Length, WaitForCompleteBuffer ? MSG_WAITALL : 0);
#endif
  if (Result < 0)
    throw SocketException(this, SystemException::GetError(this));
  return Result;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Socket::Send(LPCVOID pBuffer, SIZE_T Length)
{
  SIZE_T TotalSent = 0;
  while (TotalSent < Length)
  {
    int Result = send(m_Socket, (char*)pBuffer + TotalSent, (int)(Length - TotalSent), 0);
    
    if (Result < 0)
      throw SocketException(this, SystemException::GetError(this));
    else
      TotalSent += Result;
  }
  return TotalSent;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Socket::Close()
{
  #if defined(PLATFORM_WINDOWS)
  closesocket(m_Socket);
  #endif
  
  #if defined(PLATFORM_POSIX)
  close(m_Socket);
  #endif
  
  m_Socket = -1;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_WINDOWS)
VOID Socket::TransmitFile(Ptr<IO::File> pFile, SIZE_T FileLength, LPVOID pHead, SIZE_T HeadLength, LPVOID pTail, SIZE_T TailLength)
{
  TRANSMIT_FILE_BUFFERS HeadTail;
  HeadTail.Head = pHead;
  HeadTail.HeadLength = (DWORD)HeadLength;
  HeadTail.Tail = pTail;
  HeadTail.TailLength = (DWORD)TailLength;

  ::TransmitFile(m_Socket, pFile->Handle(), (DWORD)FileLength, 0, 0, &HeadTail, 0);
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------

#endif
