/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Utility/Convert.hpp $
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

namespace Bolitho
{
  template<class F, class T>
  inline BOOL Convert(CONST F& From, T& To)
  { return false; }

  template<class F, class T>
  inline T Convert(CONST F& From)
  { T To; Convert(From, To); return To;}

#define SimpleCastConvert(f,t)                   \
  LIBRARY_API inline BOOL Convert(CONST f& From, t& To)  \
  { To = (t)From; return true; }                 \

  SimpleCastConvert(FLOAT,INT8);
  SimpleCastConvert(FLOAT,INT16);
  SimpleCastConvert(FLOAT,INT32);
  SimpleCastConvert(FLOAT,INT64);
  SimpleCastConvert(FLOAT,UINT8);
  SimpleCastConvert(FLOAT,UINT16);
  SimpleCastConvert(FLOAT,UINT32);
  SimpleCastConvert(FLOAT,UINT64);
  SimpleCastConvert(FLOAT,DOUBLE);
  SimpleCastConvert(FLOAT,FLOAT);

  SimpleCastConvert(DOUBLE,INT8);
  SimpleCastConvert(DOUBLE,INT16);
  SimpleCastConvert(DOUBLE,INT32);
  SimpleCastConvert(DOUBLE,INT64);
  SimpleCastConvert(DOUBLE,UINT8);
  SimpleCastConvert(DOUBLE,UINT16);
  SimpleCastConvert(DOUBLE,UINT32);
  SimpleCastConvert(DOUBLE,UINT64);
  SimpleCastConvert(DOUBLE,DOUBLE);
  SimpleCastConvert(DOUBLE,FLOAT);

  SimpleCastConvert(INT8,INT8);
  SimpleCastConvert(INT8,INT16);
  SimpleCastConvert(INT8,INT32);
  SimpleCastConvert(INT8,INT64);
  SimpleCastConvert(INT8,UINT8);
  SimpleCastConvert(INT8,UINT16);
  SimpleCastConvert(INT8,UINT32);
  SimpleCastConvert(INT8,UINT64);
  SimpleCastConvert(INT8,DOUBLE);
  SimpleCastConvert(INT8,FLOAT);

  SimpleCastConvert(UINT8,INT8);
  SimpleCastConvert(UINT8,INT16);
  SimpleCastConvert(UINT8,INT32);
  SimpleCastConvert(UINT8,INT64);
  SimpleCastConvert(UINT8,UINT8);
  SimpleCastConvert(UINT8,UINT16);
  SimpleCastConvert(UINT8,UINT32);
  SimpleCastConvert(UINT8,UINT64);
  SimpleCastConvert(UINT8,DOUBLE);
  SimpleCastConvert(UINT8,FLOAT);

  SimpleCastConvert(INT16,INT8);
  SimpleCastConvert(INT16,INT16);
  SimpleCastConvert(INT16,INT32);
  SimpleCastConvert(INT16,INT64);
  SimpleCastConvert(INT16,UINT8);
  SimpleCastConvert(INT16,UINT16);
  SimpleCastConvert(INT16,UINT32);
  SimpleCastConvert(INT16,UINT64);
  SimpleCastConvert(INT16,DOUBLE);
  SimpleCastConvert(INT16,FLOAT);

  SimpleCastConvert(UINT16,INT8);
  SimpleCastConvert(UINT16,INT16);
  SimpleCastConvert(UINT16,INT32);
  SimpleCastConvert(UINT16,INT64);
  SimpleCastConvert(UINT16,UINT8);
  SimpleCastConvert(UINT16,UINT16);
  SimpleCastConvert(UINT16,UINT32);
  SimpleCastConvert(UINT16,UINT64);
  SimpleCastConvert(UINT16,DOUBLE);
  SimpleCastConvert(UINT16,FLOAT);

  SimpleCastConvert(INT32,INT8);
  SimpleCastConvert(INT32,INT16);
  SimpleCastConvert(INT32,INT32);
  SimpleCastConvert(INT32,INT64);
  SimpleCastConvert(INT32,UINT8);
  SimpleCastConvert(INT32,UINT16);
  SimpleCastConvert(INT32,UINT32);
  SimpleCastConvert(INT32,UINT64);
  SimpleCastConvert(INT32,DOUBLE);
  SimpleCastConvert(INT32,FLOAT);

  SimpleCastConvert(UINT32,INT8);
  SimpleCastConvert(UINT32,INT16);
  SimpleCastConvert(UINT32,INT32);
  SimpleCastConvert(UINT32,INT64);
  SimpleCastConvert(UINT32,UINT8);
  SimpleCastConvert(UINT32,UINT16);
  SimpleCastConvert(UINT32,UINT32);
  SimpleCastConvert(UINT32,UINT64);
  SimpleCastConvert(UINT32,DOUBLE);
  SimpleCastConvert(UINT32,FLOAT);

  SimpleCastConvert(INT64,INT8);
  SimpleCastConvert(INT64,INT16);
  SimpleCastConvert(INT64,INT32);
  SimpleCastConvert(INT64,INT64);
  SimpleCastConvert(INT64,UINT8);
  SimpleCastConvert(INT64,UINT16);
  SimpleCastConvert(INT64,UINT32);
  SimpleCastConvert(INT64,UINT64);
  SimpleCastConvert(INT64,DOUBLE);
  SimpleCastConvert(INT64,FLOAT);

  SimpleCastConvert(UINT64,INT8);
  SimpleCastConvert(UINT64,INT16);
  SimpleCastConvert(UINT64,INT32);
  SimpleCastConvert(UINT64,INT64);
  SimpleCastConvert(UINT64,UINT8);
  SimpleCastConvert(UINT64,UINT16);
  SimpleCastConvert(UINT64,UINT32);
  SimpleCastConvert(UINT64,UINT64);
  SimpleCastConvert(UINT64,DOUBLE);
  SimpleCastConvert(UINT64,FLOAT);
}

