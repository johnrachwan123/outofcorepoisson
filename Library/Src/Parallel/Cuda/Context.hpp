/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Parallel/Cuda/Context.hpp $
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

#pragma once

#if defined(USE_CUDA)

#include "Parallel/Cuda/Cuda.hpp"

namespace Bolitho
{
  namespace Cuda 
  {
    template<class T>
    class DevicePtr;

    class LIBRARY_API Context : public Object
    {
      friend class Device;
      DeclareRuntimeType();
    public:
      virtual ~Context();

      /* Waits until all asynchronous operations on the GPU have completed */ 
      VOID Synchronize();

      struct MEMORYINFO
      {
        UINT FreeMemory;
        UINT TotalMemory;
      }; 

      /* Returns the total and available memory on the device */
      MEMORYINFO GetMemoryInfo() CONST;

      /* Allocates linear device memory */
      LPDVOID AllocateRawDeviceMemory(UINT N);
      /* Frees linear device memory */
      VOID FreeRawDeviceMemory(LPDVOID pdData);

      /* Allocates linear device memory */
      template<class T>
      DevicePtr<T> AllocateDeviceMemory(UINT N)
      { return DevicePtr<T>(this, AllocateRawDeviceMemory(sizeof(T)*N), N); }

      /* Allocates page locked host memory */
      LPVOID AllocateHostMemory(UINT N);
      /* Frees page locked host memory */
      VOID FreeHostMemory(LPVOID pData);

      /* Copies data from host to device memory */
      VOID CopyMemoryHD(LPDVOID pdData, LPCVOID pData, UINT N);
      /* Copies data from device to host memory */
      VOID CopyMemoryDH(LPVOID pData, LPCDVOID pdData, UINT N);
      /* Copies data from device to host memory */
      VOID CopyMemoryDD(LPDVOID pdData0, LPCDVOID pdData1, UINT N);

      /* Creates a context from the best available device */
      STATIC Ptr<Context> CreateContext();
    protected:
      Context() {}
      Context(CUdevice D);

      CUcontext m_Context;
      DWORD m_ThreadId;
    };
  }
}

#endif
