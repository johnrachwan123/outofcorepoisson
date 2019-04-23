/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Trunk/Src/IO/DebugStream.hpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1588 $
 * Last Updated: $Date: 2008-08-23 12:36:02 -0400 (Sat, 23 Aug 2008) $
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
    class LIBRARY_API BufferedStream : public Stream
    {
      DeclareRuntimeType();
    public:
      BufferedStream(Ptr<String> pStream);

      /* Flushes all data in the stream */
      virtual VOID Flush();
      /* Closes the stream.  After the stream is closed, no other operations are valid */
      virtual VOID Close();

      /* Writes data to the stream.  Returns the number of bytes written */
      virtual SIZE_T Write(LPCVOID pData, SIZE_T N);
      /* Reads data from the stream.  Returns the number of bytes read */
      virtual SIZE_T Read(LPVOID pData, SIZE_T N);

      /* Returns TRUE if the stream has reached the end (i.e. no more data can be read) */
      virtual BOOL EndOfStream();

      /* Returns TRUE iff the stream can be read from */
      virtual BOOL CanRead();
      /* Returns TRUE iff the stream can be written to */
      virtual BOOL CanWrite();

      /* Returns TRUE iff the stream can change position */
      virtual BOOL CanSeek();

      /* Returns the current stream position */
      virtual ULONGLONG Position();
      /* Sets the current stream position */
      virtual ULONGLONG Seek(ULONGLONG NewPosition);
      /* Returns the length of the stream */
      virtual ULONGLONG Length();

    private:
      /* The underlying stream which will be buffered */
      Ptr<Stream> m_pStream;

      Buffer<BYTE> m_Buffer;
      SIZE_T m_BufferLength;
      SIZE_T m_BufferOffset;

      VOID FlushBuffer();

      struct BufferMode
      { enum { NONE, READ, WRITE }; };
      DWORD m_BufferMode;
    };

  }
}

