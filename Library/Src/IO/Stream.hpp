/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/IO/Stream.hpp $
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

#include "Memory/Pointer.hpp"

namespace Bolitho
{
  class Progress;
  namespace IO
  {
    /* Represents an abstract stream of bytes */
    class LIBRARY_API Stream : public Object
    {
      DeclareRuntimeType();
    public:
      /* Flushes all data in the stream */
      virtual VOID Flush() = 0;
      /* Closes the stream.  After the stream is closed, no other operations are valid */
      virtual VOID Close() = 0;

      /* Writes data to the stream.  Returns the number of bytes written */
      virtual SIZE_T Write(LPCVOID pData, SIZE_T N) = 0;
      /* Reads data from the stream.  Returns the number of bytes read */
      virtual SIZE_T Read(LPVOID pData, SIZE_T N) = 0;

      /* Returns TRUE if the stream has reached the end (i.e. no more data can be read) */
      virtual BOOL EndOfStream() = 0;

      /* Returns TRUE iff the stream can be read from */
      virtual BOOL CanRead() = 0;
      /* Returns TRUE iff the stream can be written to */
      virtual BOOL CanWrite() = 0;

      /* Returns TRUE iff the stream can change position */
      virtual BOOL CanSeek() = 0;

      /* Returns the current stream position */
      virtual ULONGLONG Position() = 0;
      /* Sets the current stream position */
      virtual ULONGLONG Seek(ULONGLONG NewPosition) = 0;
      /* Returns the length of the stream */
      virtual ULONGLONG Length() = 0;
      
      STATIC ULONGLONG Copy(Ptr<Stream> pTo, Ptr<Stream> pFrom, ULONGLONG N)
      { return Copy(pTo, pFrom, N, 0); }
      STATIC SIZE_T Copy(Ptr<Stream> pTo, LPCVOID pFrom, SIZE_T N)
      { return Copy(pTo, pFrom, N, 0); }
      STATIC SIZE_T Copy(LPVOID pTo, Ptr<Stream> pFrom, SIZE_T N)
      { return Copy(pTo, pFrom, N, 0); }

      STATIC ULONGLONG Copy(Ptr<Stream> pTo, Ptr<Stream> pFrom, ULONGLONG N, Progress* pProgress);
      STATIC SIZE_T Copy(Ptr<Stream> pTo, LPCVOID pFrom, SIZE_T N, Progress* pProgress);
      STATIC SIZE_T Copy(LPVOID pTo, Ptr<Stream> pFrom, SIZE_T N, Progress* pProgress);
    };

    class LIBRARY_API NullStream : public Stream
    {
      DeclareRuntimeType();
    public:
      virtual VOID Flush();
      virtual VOID Close();

      virtual SIZE_T Write(LPCVOID pData, SIZE_T N);
      virtual SIZE_T Read(LPVOID pData, SIZE_T N);

      virtual BOOL EndOfStream();
    };

    #include "StreamImpl.hpp"

  }
}
