/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Memory/VirtualMemory.cpp $
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

#include "CodeLibrary.hpp"
#include "Memory/VirtualMemory.hpp"

using namespace Bolitho;

#if defined(PLATFORM_WINDOWS)
//---------------------------------------------------------------------------------------------------
VOID VirtualHint(LPVOID pAddress, SIZE_T Length, DWORD Hint)
{
  if (Hint == PageHint::PAGEOUT)
    VirtualUnlock(pAddress, Length);
}
//---------------------------------------------------------------------------------------------------
LPVOID VirtualReserve(SIZE_T Length)
{
  return VirtualReserve(0, Length);
}
//---------------------------------------------------------------------------------------------------
LPVOID VirtualReserve(LPVOID pAddress, SIZE_T Length)
{
  return VirtualAlloc(pAddress, Length, MEM_RESERVE, PAGE_NOACCESS);
}
//---------------------------------------------------------------------------------------------------
BOOL VirtualRelease(LPVOID pAddress)
{
  return VirtualFree(pAddress, 0, MEM_RELEASE);
}
//---------------------------------------------------------------------------------------------------
BOOL VirtualCommit(LPVOID pAddress, SIZE_T Length, DWORD Protection)
{
  return VirtualAlloc(pAddress, Length, MEM_COMMIT, Protection) != 0;
}
//---------------------------------------------------------------------------------------------------
BOOL VirtualDecommit(LPVOID pAddress, SIZE_T Length)
{
  return VirtualFree(pAddress, Length, MEM_DECOMMIT);
}
//---------------------------------------------------------------------------------------------------
#endif

#if defined(PLATFORM_POSIX)
//---------------------------------------------------------------------------------------------------
BOOL VirtualLock(LPVOID pAddress, SIZE_T Length)
{
  return mlock(pAddress, Length) == 0;
}
//---------------------------------------------------------------------------------------------------
BOOL VirtualUnlock(LPVOID pAddress, SIZE_T Length)
{
  return munlock(pAddress, Length) == 0;
}
//---------------------------------------------------------------------------------------------------
BOOL VirtualProtect(LPVOID pAddress, SIZE_T Length, DWORD NewProtect, DWORD* pOldProtect)
{
  return mprotect(pAddress, Length, NewProtect) == 0;
}
//---------------------------------------------------------------------------------------------------
VOID VirtualHint(LPVOID pAddress, SIZE_T Length, DWORD Hint)
{
  if (Hint == PageHint::PAGEIN)
    madvise(pAddress, Length, MADV_WILLNEED);
  if (Hint == PageHint::PAGEOUT)
    madvise(pAddress, Length, MADV_DONTNEED);
}
//---------------------------------------------------------------------------------------------------
#endif
