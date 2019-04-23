/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/IO/MemoryStream.hpp $
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

#pragma once

namespace Bolitho
{
  namespace IO
  {
    class LIBRARY_API MemoryStream : public Stream
    {
      DeclareRuntimeType();
    public:
      MemoryStream();
      MemoryStream(Buffer<BYTE>& Buffer);
      virtual ~MemoryStream();

      virtual VOID Flush();
      virtual VOID Close();

      virtual SIZE_T Write(LPCVOID pData, SIZE_T N);
      virtual SIZE_T Read(LPVOID pData, SIZE_T N);

      virtual ULONGLONG Position();
      virtual ULONGLONG Seek(ULONGLONG NewPosition);
      virtual ULONGLONG Length();
      
      virtual BOOL EndOfStream();

      virtual BOOL CanRead()
      { return TRUE; }
      virtual BOOL CanWrite()
      { return TRUE; }
      virtual BOOL CanSeek()
      { return TRUE; }

      VOID Clear();

      CONST Buffer<BYTE>& GetBuffer()
      { return m_Buffer; }

    private:
      Buffer<BYTE>& m_Buffer;
      Buffer<BYTE> m_InternalBuffer;
      SIZE_T m_BufferOffset;
      SIZE_T m_BufferLength;

    };

  }
}

