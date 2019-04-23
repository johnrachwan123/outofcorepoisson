/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Parallel/Cuda/Buffer.hpp $
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
#include "Parallel/Cuda/Context.hpp"

namespace Bolitho
{
  namespace Cuda 
  {
    struct GPUBufferUsage
    { enum { NONE = 0, READONLY = 1, WRITEONLY = 2, READWRITE = 4 }; };

    class LIBRARY_API GPUBuffer : public Object
    {
      friend class Device;
      DeclareRuntimeType();
    public:
      GPUBuffer(Ptr<Context> pContext);
      GPUBuffer(Ptr<Context> pContext, UINT Length);
      virtual ~GPUBuffer();

      /* Allocates a buffer of N-bytes */
      VOID Allocate(UINT Length);

      /* Frees the memory buffer */
      VOID Free();

      operator LPDVOID ()
      { return m_pdData; }

      /* Maps the buffer into host memory, and returns a pointer to the start of the memory block */
      LPVOID MapBuffer(DWORD Access);
      /* Unmaps the buffer from host memory.  Updates the GPU copy of the data */
      BOOL UnmapBuffer();
    protected:

      Ptr<Context> m_pContext;
      UINT m_Length;
      LPDVOID m_pdData;

      LPVOID m_pData;
      DWORD m_MappingFlags;
    };

    template<class T>
    class TGPUBuffer : public GPUBuffer
    {
    public:
      TGPUBuffer(Ptr<Context> pContext) : GPUBuffer(pContext)
      {}
      TGPUBuffer(Ptr<Context> pContext, UINT Count) : GPUBuffer(pContext, sizeof(T)*Count)
      {}
      
      /* Allocates a buffer of N-objects */
      VOID Allocate(UINT Count)
      { GPUBuffer::Allocate(sizeof(T)*Count); }

      operator T* ()
      { return (T*)m_pdData; }

      /* Maps the buffer into host memory, and returns a pointer to the start of the memory block */
      T* MapBuffer(DWORD Access)
      { return (T*)GPUBuffer::MapBuffer(Access); }

    };
  }
}

#endif
