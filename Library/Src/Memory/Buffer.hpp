/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Memory/Buffer.hpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1588 $
 * Last Updated: $Date: 2008-08-23 09:36:02 -0700 (Sat, 23 Aug 2008) $
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
  class Buffer : public Object
  {
    DeclareRuntimeType();
  public:
    /* Constructs an empty, 0-byte buffer */
    Buffer();
    /* Constructs a N-byte buffer filled with the data pointed to by pData */
    Buffer(CONST T* pData, SIZE_T N);
    /* Constructs a buffer as a copy of another */
    Buffer(CONST Buffer& B);
    /* Constructs an empty N-byte buffer */
    explicit Buffer(SIZE_T N);

    Buffer(Allocator& A);
    Buffer(CONST T* pData, SIZE_T N, Allocator& A);
    Buffer(CONST Buffer& B, Allocator& A);
    Buffer(SIZE_T N, Allocator& A);

    virtual ~Buffer();

    /* Returns the length of the buffer */
    SIZE_T Length() CONST
    { return m_Length; }

    /* Creates a new N-byte buffer */
    VOID Allocate(SIZE_T N);
    /* Resizes the current buffer */
    VOID Resize(SIZE_T N);
    /* Frees the buffer */
    VOID Free();

    /* Explicit pointer conversions */
    CONST T* GetBuffer() CONST
    { return m_pData; }
    T* GetBuffer()
    { return m_pData; }

    // array access
    CONST T& operator() (SIZE_T i) CONST
    { Assert(i < m_Length); return m_pData[i]; }
    T& operator() (SIZE_T i)
    { Assert(i < m_Length); return m_pData[i]; }
    CONST T& operator[] (SIZE_T i) CONST
    { Assert(i < m_Length); return m_pData[i]; }
    T& operator[] (SIZE_T i)
    { Assert(i < m_Length); return m_pData[i]; }

    // assignment
    Buffer& operator= (CONST Buffer& B);

    // comparisons
    BOOL operator== (T* pData) CONST;
    BOOL operator!= (T* pData) CONST;
    BOOL operator== (CONST Buffer& B) CONST;
    BOOL operator!= (CONST Buffer& B) CONST;

    /* Serializes the buffer to a Stream */
    virtual VOID Write(Ptr<IO::Stream> pStream) CONST;
    /* Deserializes a buffer from a Stream */
    virtual VOID Read(Ptr<IO::Stream> pStream);

    Buffer<BYTE> Compress(DWORD Type) CONST;
    VOID Decompress(DWORD Type, CONST Buffer<BYTE>& Source);

    UINT32 ComputeCrc32() CONST;
    Md5Hash ComputeMd5() CONST;

  private:
    inline VOID CopyOnWrite();
    INLINE SIZE_T AllocateSize(SIZE_T N);

    SIZE_T m_Length;
    T* m_pData;
    Allocator m_Allocator;
  };

  #include "BufferImpl.hpp"
}
