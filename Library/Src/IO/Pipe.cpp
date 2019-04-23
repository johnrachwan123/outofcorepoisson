/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/IO/Pipe.cpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1627 $
 * Last Updated: $Date: 2008-09-14 05:01:15 -0700 (Sun, 14 Sep 2008) $
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

#include "IO/Pipe.hpp"

using namespace Bolitho;
using namespace Bolitho::IO;

//----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::IO,Pipe,Stream);
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_WINDOWS)
VOID Pipe::CreateAnonymous(Ptr<Pipe>& Read, Ptr<Pipe>& Write)
{
  Pipe* pRead = new Pipe(0);
  Pipe* pWrite = new Pipe(0);

  if (CreatePipe(&pRead->m_hPipe, &pWrite->m_hPipe, 0, 0) == 0)
    Win32Exception::ThrowError();

  Read = pRead;
  Write = pWrite;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Pipe> Pipe::CreateNamed(LPCTSTR Name, DWORD Mode, DWORD Type)
{
  return new Pipe(0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Pipe> Pipe::OpenNamed(LPCTSTR Name, DWORD Access)
{
  return new Pipe(0);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Pipe> Pipe::OpenStdIn()
{
  return new Pipe(GetStdHandle(STD_INPUT_HANDLE));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Pipe> Pipe::OpenStdOut()
{
  return new Pipe(GetStdHandle(STD_OUTPUT_HANDLE));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Pipe> Pipe::OpenStdError()
{
  return new Pipe(GetStdHandle(STD_ERROR_HANDLE));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Pipe::Flush()
{
  FlushFileBuffers(m_hPipe);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Pipe::Close()
{
  CloseHandle(m_hPipe);
  m_hPipe = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Pipe::Write(LPCVOID pData, SIZE_T N)
{
  DWORD NN = 0;
  if (WriteFile(m_hPipe, pData, (DWORD)N, &NN, 0) == 0)
    Win32Exception::ThrowError(this);

  return NN;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Pipe::Read(LPVOID pData, SIZE_T N)
{
  DWORD NN = 0;
  if (ReadFile(m_hPipe, pData, (DWORD)N, &NN, 0) == 0)
    Win32Exception::ThrowError(this);

  return NN;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Pipe::EndOfStream()
{
  return TRUE;
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_POSIX)
VOID Pipe::CreateAnonymous(Ptr<Pipe>& Read, Ptr<Pipe>& Write)
{
  Pipe* pRead = new Pipe(0);
  Pipe* pWrite = new Pipe(0);

	PIPEHANDLE hPipe[2];

	pipe(&hPipe[0]);

	pRead->m_hPipe = hPipe[0];
	pWrite->m_hPipe = hPipe[1];

  Read = pRead;
  Write = pWrite;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Pipe> Pipe::CreateNamed(LPCTSTR Name, DWORD Mode, DWORD Type)
{
  return new Pipe(0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Pipe> Pipe::OpenNamed(LPCTSTR Name, DWORD Access)
{
  return new Pipe(0);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Pipe> Pipe::OpenStdIn()
{
  return new Pipe(dup(STDIN_FILENO));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Pipe> Pipe::OpenStdOut()
{
  return new Pipe(dup(STDOUT_FILENO));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Pipe> Pipe::OpenStdError()
{
  return new Pipe(dup(STDERR_FILENO));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Pipe::Flush()
{
  fsync(m_hPipe);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Pipe::Close()
{
  close(m_hPipe);
  m_hPipe = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Pipe::Write(LPCVOID pData, SIZE_T N)
{
	return write(m_hPipe, pData, N);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Pipe::Read(LPVOID pData, SIZE_T N)
{
	return read(m_hPipe, pData, N);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Pipe::EndOfStream()
{
  return TRUE;
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------------------------------------
Pipe::Pipe(PIPEHANDLE hPipe)
{
  m_hPipe = hPipe;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Pipe::~Pipe()
{
	Close();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

