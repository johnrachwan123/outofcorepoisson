/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Collections/CircularArray.hpp $
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

#include "Utility/Traits.hpp"
#include "Memory/Buffer.hpp"

namespace Bolitho
{
  template<class T, class ElementTraits=Traits<T>, class Allocator=DefaultAllocator<T>, class ThreadingModel=SingleThreadedModel>
  class CircularArray : public Object
  {
    DeclareRuntimeType();
  public:
    /* Creates an empty circular array */
    CircularArray();
    /* Creates an N-element circular array */
    CircularArray(SIZE_T Length);
    /* Creates a circular array as a copy of another */
    CircularArray(CONST CircularArray& A);
    /* Creates a circular array from a regular array */
    CircularArray(CONST Array<T,ElementTraits>& A);
    virtual ~CircularArray();

    CircularArray& operator= (CONST CircularArray& A);

    /* Resizes the array to contain 'Length' elements */
    VOID Resize(SIZE_T Length);

    /* Returns the length of the array */
    SIZE_T Length() CONST;
    /* Returns TRUE if the array is empty */
    BOOL IsEmpty() CONST;

    CONST T& operator[] (PTRDIFF_T i) CONST;
    T& operator[] (PTRDIFF_T i);
    CONST T& operator() (PTRDIFF_T i) CONST;
    T& operator() (PTRDIFF_T i);

    T Get(PTRDIFF_T i) CONST;
    VOID Set(PTRDIFF_T i, CONST T& Item);

  private:
    SIZE_T m_Length;
    SharedBuffer<T> m_Buffer;
  };

  #include "CircularArrayImpl.hpp"
}

