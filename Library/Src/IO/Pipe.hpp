/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/IO/Pipe.hpp $
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

#include "IO/Stream.hpp"

namespace Bolitho
{
  namespace IO
  {

    #if defined(PLATFORM_WINDOWS)
    struct PipeAccess
    { enum { READ = GENERIC_READ, WRITE = GENERIC_WRITE, READWRITE = GENERIC_READ | GENERIC_WRITE }; };

    struct PipeType
    { enum { BYTE = PIPE_TYPE_BYTE, MESSAGE = PIPE_TYPE_MESSAGE, WAIT = PIPE_WAIT, NOWAIT = PIPE_NOWAIT }; };

    struct PipeMode
    { enum { DUPLEX = PIPE_ACCESS_DUPLEX, INBOUND = PIPE_ACCESS_INBOUND, OUTBOUND = PIPE_ACCESS_OUTBOUND }; };
    #endif

    #if defined(PLATFORM_POSIX)
    struct PipeAccess
    { enum { READ = 0, WRITE = 0, READWRITE = 0 }; };

    struct PipeType
    { enum { BYTE = 0, MESSAGE = 0, WAIT = 0, NOWAIT = 0 }; };

    struct PipeMode
    { enum { DUPLEX = 0, INBOUND = 0, OUTBOUND = 0 }; };
    #endif


    class LIBRARY_API Pipe : public Stream
    {
      DeclareRuntimeType();
    public:
      static VOID CreateAnonymous(Ptr<Pipe>& pRead, Ptr<Pipe>& pWrite);
      static Ptr<Pipe> CreateNamed(LPCTSTR Name, DWORD Mode = PipeMode::DUPLEX, DWORD Type = PipeType::BYTE | PipeType::NOWAIT); 
      static Ptr<Pipe> OpenNamed(LPCTSTR Name, DWORD Access = PipeAccess::READWRITE);

      static Ptr<Pipe> OpenStdIn();
      static Ptr<Pipe> OpenStdOut();
      static Ptr<Pipe> OpenStdError();

      virtual ~Pipe();

      virtual VOID Flush();
      virtual VOID Close();

      virtual SIZE_T Write(LPCVOID pData, SIZE_T N);
      virtual SIZE_T Read(LPVOID pData, SIZE_T N);

      virtual BOOL EndOfStream();

      virtual BOOL CanRead()
      { return TRUE; }
      virtual BOOL CanWrite()
      { return TRUE; }

      virtual BOOL CanSeek()
      { return FALSE; }

      virtual ULONGLONG Position()
      { return 0; }
      virtual ULONGLONG Seek(ULONGLONG NewPosition)
      { return 0; }
      virtual ULONGLONG Length()
      { return 0; }

      PIPEHANDLE Handle() CONST
      { return m_hPipe; }
    private:
      Pipe(PIPEHANDLE hPipe);

      String m_Filename;
      PIPEHANDLE m_hPipe;

    };

  }
}
