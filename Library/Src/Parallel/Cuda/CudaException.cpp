/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Parallel/Cuda/CudaException.cpp $
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

#include "Parallel/Cuda/CudaException.hpp"

using namespace Bolitho;
using namespace Bolitho::Cuda;

//---------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Cuda,CudaException,Exception);
//---------------------------------------------------------------------------------------------------
CudaException::CudaException(CONST Object* Sender, LPCTSTR Description) : Exception(Sender)
{
  m_Description = Description;
}
//---------------------------------------------------------------------------------------------------
CudaException::CudaException(CONST Object* Sender, CUresult Result) : Exception(Sender)
{
  switch (Result)
  {
  case CUDA_ERROR_INVALID_VALUE:
    m_Description = "CUDA_ERROR_INVALID_VALUE"; break;
  case CUDA_ERROR_OUT_OF_MEMORY:
    m_Description = "CUDA_ERROR_OUT_OF_MEMORY"; break;
  case CUDA_ERROR_NOT_INITIALIZED:
    m_Description = "CUDA_ERROR_NOT_INITIALIZED"; break;
  case CUDA_ERROR_NO_DEVICE:
    m_Description = "CUDA_ERROR_NO_DEVICE"; break;
  case CUDA_ERROR_INVALID_DEVICE:
    m_Description = "CUDA_ERROR_INVALID_DEVICE"; break;
  case CUDA_ERROR_INVALID_IMAGE:
    m_Description = "CUDA_ERROR_INVALID_IMAGE"; break;
  case CUDA_ERROR_INVALID_CONTEXT:
    m_Description = "CUDA_ERROR_INVALID_CONTEXT"; break;
  case CUDA_ERROR_CONTEXT_ALREADY_CURRENT:
    m_Description = "CUDA_ERROR_CONTEXT_ALREADY_CURRENT"; break;
  case CUDA_ERROR_MAP_FAILED:
    m_Description = "CUDA_ERROR_MAP_FAILED"; break;
  case CUDA_ERROR_UNMAP_FAILED:
    m_Description = "CUDA_ERROR_UNMAP_FAILED"; break;
  case CUDA_ERROR_ARRAY_IS_MAPPED:
    m_Description = "CUDA_ERROR_ARRAY_IS_MAPPED"; break;
  case CUDA_ERROR_ALREADY_MAPPED:
    m_Description = "CUDA_ERROR_ALREADY_MAPPED"; break;
  case CUDA_ERROR_NO_BINARY_FOR_GPU:
    m_Description = "CUDA_ERROR_NO_BINARY_FOR_GPU"; break;
  case CUDA_ERROR_ALREADY_ACQUIRED:
    m_Description = "CUDA_ERROR_ALREADY_ACQUIRED"; break;
  case CUDA_ERROR_NOT_MAPPED: 
    m_Description = "CUDA_ERROR_NOT_MAPPED"; break;

  case CUDA_ERROR_INVALID_SOURCE:
    m_Description = "CUDA_ERROR_INVALID_SOURCE"; break;
  case CUDA_ERROR_FILE_NOT_FOUND:
    m_Description = "CUDA_ERROR_FILE_NOT_FOUND"; break;

  case CUDA_ERROR_INVALID_HANDLE:
    m_Description = "CUDA_ERROR_INVALID_HANDLE"; break;

  case CUDA_ERROR_NOT_FOUND: 
    m_Description = "CUDA_ERROR_NOT_FOUND"; break;

  case CUDA_ERROR_NOT_READY:
    m_Description = "CUDA_ERROR_NOT_READY"; break;

  case CUDA_ERROR_LAUNCH_FAILED:
    m_Description = "CUDA_ERROR_LAUNCH_FAILED"; break;

  case CUDA_ERROR_LAUNCH_OUT_OF_RESOURCES:
    m_Description = "CUDA_ERROR_LAUNCH_OUT_OF_RESOURCES"; break;
  case CUDA_ERROR_LAUNCH_TIMEOUT:
    m_Description = "CUDA_ERROR_LAUNCH_TIMEOUT"; break;
  case CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING:
    m_Description = "CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING"; break;

  case CUDA_ERROR_UNKNOWN:
    m_Description = "CUDA_ERROR_UNKNOWN"; break;
  };
}
//---------------------------------------------------------------------------------------------------


#endif
