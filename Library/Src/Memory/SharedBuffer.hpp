/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Memory/SharedBuffer.hpp $
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

#pragma once

#include "Debug/Assert.hpp"
#include "IO/Stream.hpp"
#include "Memory/Memory.hpp"
#include "Memory/Md5.hpp"
#include "Memory/Crc32.hpp"
#include "Memory/Allocator.hpp"

namespace Bolitho
{
  template <class T, class Allocator=DefaultAllocator<T> >
  class SharedBuffer : public Object
  {
    DeclareRuntimeType();
  public:
    /* Constructs an empty, 0-byte SharedBuffer */
    SharedBuffer();
    /* Constructs a N-byte SharedBuffer filled with the data pointed to by pData */
    SharedBuffer(CONST T* pData, SIZE_T N);
    /* Constructs a SharedBuffer as a copy of another */
    SharedBuffer(CONST SharedBuffer& B);
    /* Constructs an empty N-byte SharedBuffer */
    explicit SharedBuffer(SIZE_T N);

    SharedBuffer(Allocator& A);
    SharedBuffer(CONST T* pData, SIZE_T N, Allocator& A);
    SharedBuffer(CONST SharedBuffer& B, Allocator& A);
    SharedBuffer(SIZE_T N, Allocator& A);

    virtual ~SharedBuffer();

    /* Add a reference to the SharedBuffer data */
    VOID AddRef() CONST;
    /* Release a reference to the SharedBuffer data */
    VOID ReleaseRef();
    /* Returns the current reference count of the SharedBuffer data */
    DWORD RefCount() CONST;

    /* Returns the length of the SharedBuffer */
    SIZE_T Length() CONST;

    /* Creates a new N-byte SharedBuffer */
    VOID Allocate(SIZE_T N);
    /* Resizes the current SharedBuffer */
    VOID Resize(SIZE_T N);
    /* Frees the SharedBuffer */
    VOID Free();

    /* Implicit pointer conversions */
    operator CONST T* () CONST;
    operator T* ();

    /* Explicit pointer conversions */
    CONST T* GetSharedBuffer() CONST;
    T* GetSharedBuffer();

    // array access
    CONST T& operator () (SIZE_T i) CONST;
    T& operator () (SIZE_T i);

    // assignment
    SharedBuffer& operator= (CONST SharedBuffer& B);

    // comparisons
    BOOL operator== (T* pData) CONST;
    BOOL operator!= (T* pData) CONST;
    BOOL operator== (CONST SharedBuffer& B) CONST;
    BOOL operator!= (CONST SharedBuffer& B) CONST;

    /* Serializes the SharedBuffer to a Stream */
    virtual VOID Write(Ptr<IO::Stream> pStream) CONST;
    /* Deserializes a SharedBuffer from a Stream */
    virtual VOID Read(Ptr<IO::Stream> pStream);

    SharedBuffer<BYTE> Compress(DWORD Type) CONST;
    VOID Decompress(DWORD Type, CONST SharedBuffer<BYTE>& Source);

    UINT32 ComputeCrc32() CONST;
    Md5Hash ComputeMd5() CONST;

  private:
    inline VOID CopyOnWrite();

    inline SIZE_T AllocateSize(SIZE_T N);

    struct SharedBufferInfo
    {
      DWORD RefCount;
      SIZE_T DataSize;
      T* pData;
    };

    SharedBufferInfo* m_pInfo;
    Allocator m_Allocator;
  };

  #include "SharedBufferImpl.hpp"
}


