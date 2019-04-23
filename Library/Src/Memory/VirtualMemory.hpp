/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Memory/VirtualMemory.hpp $
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

namespace Bolitho
{

  #if defined(PLATFORM_WINDOWS)
  struct PageAccess
  { enum { NOACCESS = PAGE_NOACCESS, READ = PAGE_READONLY, READWRITE = PAGE_READWRITE, EXECUTE = PAGE_EXECUTE, WRITECOPY = PAGE_WRITECOPY }; };

  struct PageAllocation
  { enum { COMMIT = MEM_COMMIT, RESERVE = MEM_RESERVE, RESET = MEM_RESET }; };

  struct PageFree
  { enum { FREE = MEM_RELEASE, DECOMMIT = MEM_DECOMMIT }; };

  struct PageType
  { enum { PRIVATE = MEM_PRIVATE, IMAGE = MEM_IMAGE, MAPPED = MEM_MAPPED, LARGE = MEM_LARGE_PAGES }; };

  #endif

  #if defined(PLATFORM_POSIX)
  struct PageAccess
  { enum { NOACCESS = PROT_NONE, READ = PROT_READ, READWRITE = PROT_READ | PROT_WRITE, EXECUTE = PROT_EXEC, WRITECOPY = 0x1000 }; };
  #endif

  struct PageHint
  { enum { PAGEIN = 1, PAGEOUT = 2 }; };

  struct LIBRARY_API VirtualMemorySpan
  {
    VirtualMemorySpan() : Offset(0), Length(0), Access(0), State(0), Type(0)
    {}
    VirtualMemorySpan(CONST VirtualMemorySpan& S) : Offset(S.Offset), Length(S.Length), Access(S.Access), State(S.State), Type(S.Type), Name(S.Name)
    {}

    PVOID Offset;
    SIZE_T Length;
    DWORD Access;
    DWORD State;
    DWORD Type;
    String Name;
  };

}

LPVOID VirtualReserve(LPVOID pAddress, SIZE_T Length);
LPVOID VirtualReserve(SIZE_T Length);
BOOL VirtualRelease(LPVOID pAddress);

BOOL VirtualCommit(LPVOID pAddress, SIZE_T Length, DWORD Protection);
BOOL VirtualDecommit(LPVOID pAddress, SIZE_T Length);

VOID VirtualHint(LPVOID pAddress, SIZE_T Length, DWORD Hint);


#if defined(PLATFORM_POSIX)
LPVOID VirtualAlloc(LPVOID pAddress, SIZE_T Length, DWORD AllocationType, DWORD Protection);
BOOL VirtualFree(LPVOID pAddress, SIZE_T Length, DWORD FreeType);
BOOL VirtualLock(LPVOID pAddress, SIZE_T Length);
BOOL VirtualUnlock(LPVOID pAddress, SIZE_T Length);
BOOL VirtualProtect(LPVOID pAddress, SIZE_T Length, DWORD NewProtect, DWORD* pOldProtect);
#endif
