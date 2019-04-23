/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Parallel/Cuda/CudaContext.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 1729 $
 * Last Updated: $Date: 2008-12-15 08:58:43 -0800 (Mon, 15 Dec 2008) $
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

#if defined(USE_CUDA)

#include "Parallel/Cuda/Context.hpp"
#include "Parallel/Cuda/Device.hpp"
#include "Parallel/Cuda/CudaException.hpp"

#include "System/Threading/Thread.hpp"

using namespace Bolitho;
using namespace Bolitho::Cuda;
using namespace Bolitho::System;

//---------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Cuda,Context,Object);
//---------------------------------------------------------------------------------------------------
Context::Context(CUdevice D)
{
  VerifyCudaResult(cuCtxCreate(&m_Context, 0, D));
  m_ThreadId = Thread::GetCurrentThreadId();
}
//---------------------------------------------------------------------------------------------------
Context::~Context()
{
  VerifyCudaResult(cuCtxDetach(m_Context));
}
//---------------------------------------------------------------------------------------------------
VOID Context::Synchronize()
{
  Assert(m_ThreadId == Thread::GetCurrentThreadId());

  VerifyCudaResultStatic(cuCtxSynchronize());
}
//---------------------------------------------------------------------------------------------------
Context::MEMORYINFO Context::GetMemoryInfo() CONST
{
  Assert(m_ThreadId == Thread::GetCurrentThreadId());

  MEMORYINFO Info;
  VerifyCudaResult(cuMemGetInfo(&Info.FreeMemory, &Info.TotalMemory));
  return Info;
}
//---------------------------------------------------------------------------------------------------
LPDVOID Context::AllocateRawDeviceMemory(UINT N)
{
  Assert(m_ThreadId == Thread::GetCurrentThreadId());

  LPDVOID pdData;
  VerifyCudaResult(cuMemAlloc(&pdData, N));
  return pdData;
}
//---------------------------------------------------------------------------------------------------
VOID Context::FreeRawDeviceMemory(LPDVOID pdData)
{
  Assert(m_ThreadId == Thread::GetCurrentThreadId());

  VerifyCudaResult(cuMemFree((LPDVOID)pdData));
}
//---------------------------------------------------------------------------------------------------
LPVOID Context::AllocateHostMemory(UINT N)
{
  Assert(m_ThreadId == Thread::GetCurrentThreadId());

  LPVOID pData;
  VerifyCudaResult(cuMemAllocHost(&pData, N));
  return pData;
}
//---------------------------------------------------------------------------------------------------
VOID Context::FreeHostMemory(LPVOID pData)
{
  Assert(m_ThreadId == Thread::GetCurrentThreadId());

  VerifyCudaResult(cuMemFreeHost(pData));
}
//---------------------------------------------------------------------------------------------------
VOID Context::CopyMemoryHD(LPDVOID pdData, LPCVOID pData, UINT N)
{
  Assert(m_ThreadId == Thread::GetCurrentThreadId());
  
  VerifyCudaResult(cuMemcpyHtoD((LPDVOID)pdData, pData, N));
}
//---------------------------------------------------------------------------------------------------
VOID Context::CopyMemoryDH(LPVOID pData, LPCDVOID pdData, UINT N)
{
  Assert(m_ThreadId == Thread::GetCurrentThreadId());
  
  VerifyCudaResult(cuMemcpyDtoH(pData, (LPDVOID)pdData, N));
}
//---------------------------------------------------------------------------------------------------
Ptr<Context> Context::CreateContext()
{
  return Device::ChooseBestDevice()->CreateContext();
}
//---------------------------------------------------------------------------------------------------

#endif
