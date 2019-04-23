/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Parallel/Cuda/CudaBuffer.cpp $
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

#include "Parallel/Cuda/Buffer.hpp"

using namespace Bolitho;
using namespace Bolitho::Cuda;

//---------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Cuda,GPUBuffer,Object);
//---------------------------------------------------------------------------------------------------
GPUBuffer::GPUBuffer(Ptr<Context> pContext) : m_pContext(pContext)
{
  m_pdData = 0;
}
//---------------------------------------------------------------------------------------------------
GPUBuffer::GPUBuffer(Ptr<Context> pContext, UINT Length) : m_pContext(pContext)
{
  m_pdData = 0;
  Allocate(Length);
}
//---------------------------------------------------------------------------------------------------
GPUBuffer::~GPUBuffer()
{
  Free();
}
//---------------------------------------------------------------------------------------------------
VOID GPUBuffer::Free()
{
  if (m_pdData)
    m_pContext->FreeRawDeviceMemory(m_pdData);
  m_pdData = 0;
  m_Length = 0;
}
//---------------------------------------------------------------------------------------------------
VOID GPUBuffer::Allocate(UINT Length)
{
  m_pdData = m_pContext->AllocateRawDeviceMemory(Length);
  m_Length = Length;
  m_MappingFlags = 0x80000000;
}
//---------------------------------------------------------------------------------------------------
LPVOID GPUBuffer::MapBuffer(DWORD Access)
{
  Assert((m_MappingFlags & 0x7fffffff) == 0);
  Assert(m_pData == 0);

  m_pData = AllocateMemory(m_Length);
  m_MappingFlags |= Access;
  
  if (m_MappingFlags == GPUBufferUsage::READONLY || m_MappingFlags == GPUBufferUsage::READWRITE)
    m_pContext->CopyMemoryDH(m_pData, m_pdData, m_Length);

  return m_pData;
}
//---------------------------------------------------------------------------------------------------
BOOL GPUBuffer::UnmapBuffer()
{
  Assert(m_MappingFlags != 0);
  Assert(m_pData != 0);

  m_MappingFlags &= 0x7fffffff;

  if (m_MappingFlags == GPUBufferUsage::WRITEONLY || m_MappingFlags == GPUBufferUsage::READWRITE)
    m_pContext->CopyMemoryHD(m_pdData, m_pData, m_Length);

  FreeMemory(m_pData);
  m_MappingFlags = 0;
  m_pData = 0;

  return TRUE;
}
//---------------------------------------------------------------------------------------------------

#endif
