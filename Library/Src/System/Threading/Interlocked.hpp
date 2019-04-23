/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/Threading/Interlocked.hpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1711 $
 * Last Updated: $Date: 2008-11-17 10:09:48 -0800 (Mon, 17 Nov 2008) $
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
  /* Performs an atomic floating point accumulate operation */
  FLOAT InterlockedAccumulate(VOLATILE FLOAT* pFloat, FLOAT V);
  /* Performs an atomic floating point accumulate operation */
  DOUBLE InterlockedAccumulate(VOLATILE DOUBLE* pFloat, DOUBLE V);
}

#if defined(PLATFORM_POSIX)

#if defined(ARCH_AMD64) /* || defined(ARCH_I386) */
INLINE LONG InterlockedCompareExchange(volatile LONG* dest, LONG exch, LONG comp)
{
  LONG old;

  __asm__ __volatile__ ("lock; cmpxchgl %k2, %0"
        : "=m" (*dest), "=a" (old)
        : "r" (exch), "m" (*dest), "a" (comp)); 
  return(old);
} 

INLINE LONGLONG InterlockedCompareExchange64(volatile LONGLONG* dest, LONGLONG exch, LONGLONG comp)
{
  LONGLONG old;

  __asm__ __volatile__ ("lock; cmpxchgq %2, %0"
        : "=m" (*dest), "=a" (old)
        : "r" (exch), "m" (*dest), "a" (comp)); 
  
  return(old);
}

INLINE LONG InterlockedExchangeAdd(LONG* Addend, LONG Increment)
{
  LONG ret;
  __asm__ __volatile__ ("lock; xaddl %k0, (%1)"
    :"=r" (ret)
    :"r" (Addend), "0" (Increment)
    :"memory");
  return ret + Increment;
}

INLINE LONG InterlockedIncrement(LONG* Addend)
{
  return InterlockedExchangeAdd(Addend, 1);
}

INLINE LONG InterlockedDecrement(LONG* Addend)
{
  return InterlockedExchangeAdd(Addend, -1);
}

#else

INLINE LONG InterlockedCompareExchange(volatile LONG* dest, LONG exch, LONG comp)
{
  LONG value = *dest;
  if (value == comp)
    *dest = exch;
  return value;
}

INLINE LONGLONG InterlockedCompareExchange64(volatile LONGLONG* dest, LONGLONG exch, LONGLONG comp)
{
  LONGLONG value = *dest;
  if (value == comp)
    *dest = exch;
  return value;
}

INLINE LONG InterlockedExchangeAdd(LONG* Addend, LONG Increment)
{
  LONG value = *Addend;
  *Addend += Increment;
  return value;
}

INLINE LONG InterlockedIncrement(LONG* Addend)
{
  return InterlockedExchangeAdd(Addend, 1) + 1;
}

INLINE LONG InterlockedDecrement(LONG* Addend)
{
  return InterlockedExchangeAdd(Addend, -1) - 1;
}

#endif



#endif
