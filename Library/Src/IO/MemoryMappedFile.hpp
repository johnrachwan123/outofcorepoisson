/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/IO/MemoryMappedFile.hpp $
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

#include "Memory/VirtualMemory.hpp"
#include "IO/File.hpp"

namespace Bolitho
{
  namespace IO
  {
#if defined(PLATFORM_WINDOWS)
    struct MemoryMappedFileFlags { enum { LARGEPAGES = SEC_LARGE_PAGES, NOCACHE = SEC_NOCACHE }; };
#elif defined(PLATFORM_POSIX)
    struct MemoryMappedFileFlags { enum { LARGEPAGES = 0 /* Not Supported */, NOCACHE = 0 /* No Supported */}; };
#endif

    class LIBRARY_API MemoryMappedView;
    class LIBRARY_API MemoryMappedFile : public Object
    {
      friend class MemoryMappedView;
      DeclareRuntimeType();
    public:
      static Ptr<MemoryMappedFile> Open(LPCTSTR Filename, DWORD Mode = FileMode::OPENORCREATE, DWORD Access = PageAccess::READWRITE, DWORD Flags = 0);
      static Ptr<MemoryMappedFile> Open(Ptr<File> F, DWORD Access = PageAccess::READWRITE, DWORD Flags = 0);
      static Ptr<MemoryMappedFile> OpenAnonymous(ULONGLONG Size, DWORD Access = PageAccess::READWRITE, DWORD Flags = 0);

      virtual ~MemoryMappedFile();

      ULONGLONG Length() CONST;
      ULONGLONG Resize(ULONGLONG Length);
      
      Ptr<MemoryMappedView> CreateView();
      Ptr<MemoryMappedView> CreateView(ULONGLONG Offset, SIZE_T Length);
      
      Ptr<MemoryMappedView> CreateView(DWORD Access);
      Ptr<MemoryMappedView> CreateView(ULONGLONG Offset, SIZE_T Length, DWORD Access);

      VOID Close();
    protected:
      MemoryMappedFile();
      
      Ptr<File> m_pFile;
      DWORD m_Access;
      DWORD m_Flags;
      ULONGLONG m_Length;
      MEMORYMAPPEDFILEHANDLE m_hMapping;
    };

    class LIBRARY_API MemoryMappedView : public Object
    {
      friend class MemoryMappedFile;
      
      DeclareRuntimeType();
    public:
      virtual ~MemoryMappedView();
      
      SIZE_T Length() CONST
      { return m_Length; }
      ULONGLONG Offset() CONST
      { return m_Offset; }

      LPBYTE GetBasePointer()
      { return (LPBYTE)m_pData; }
      LPCBYTE GetBasePointer() CONST
      { return (LPCBYTE)m_pData; }

      template<class T>
      operator T* ()
      { return (T*)m_pData; }
      template<class T>
      operator CONST T* () CONST
      { return (CONST T*)m_pData; }
      
      VOID Flush();
      VOID Flush(SIZE_T Offset, SIZE_T Length);
      VOID Lock(SIZE_T Offset, SIZE_T Length) CONST;
      VOID Unlock(SIZE_T Offset, SIZE_T Length) CONST;
      
    protected:
      MemoryMappedView(LPVOID pData, ULONGLONG Offset, SIZE_T Length);
      LPVOID m_pData;
      ULONGLONG m_Offset;
      SIZE_T m_Length;
    };
    
  }
}
