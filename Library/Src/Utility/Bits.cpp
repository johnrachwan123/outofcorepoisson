/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Utility/Bits.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 1748 $
 * Last Updated: $Date: 2008-12-28 16:10:25 -0800 (Sun, 28 Dec 2008) $
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
#include "Utility/Bits.hpp"

//-----------------------------------------------------------------------------------------------------------------------------------------------------
UINT16 Bolitho::Interleave(UINT8 A, UINT8 B)
{
  UINT16 R = 0;
  
  R |= (A & 1u << 0) << 0 | (B & 1u << 0) << 1;
  R |= (A & 1u << 1) << 1 | (B & 1u << 1) << 2;
  R |= (A & 1u << 2) << 2 | (B & 1u << 2) << 3;
  R |= (A & 1u << 3) << 3 | (B & 1u << 3) << 4;
  R |= (A & 1u << 4) << 4 | (B & 1u << 4) << 5;
  R |= (A & 1u << 5) << 5 | (B & 1u << 5) << 6;
  R |= (A & 1u << 6) << 6 | (B & 1u << 6) << 7;
  R |= (A & 1u << 7) << 7 | (B & 1u << 7) << 8;
  
  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Bolitho::DeInterleave(UINT16 X, UINT8& A, UINT8& B)
{
  A = B = 0;

  A |= (X & 1u << 0);
  A |= (X & 1u << 2) >> 1;
  A |= (X & 1u << 4) >> 2;
  A |= (X & 1u << 6) >> 3;
  A |= (X & 1u << 8) >> 4;
  A |= (X & 1u << 10) >> 5;
  A |= (X & 1u << 12) >> 6;
  A |= (X & 1u << 14) >> 7;

  B |= (X & 1u << 1) >> 1;
  B |= (X & 1u << 3) >> 2;
  B |= (X & 1u << 5) >> 3;
  B |= (X & 1u << 7) >> 4;
  B |= (X & 1u << 9) >> 5;
  B |= (X & 1u << 11) >> 6;
  B |= (X & 1u << 13) >> 7;
  B |= (X & 1u << 15) >> 8;

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
UINT32 Bolitho::Interleave(UINT16 A, UINT16 B)
{
  UINT32 R = 0;
  
  R |= (A & 1u <<  0) <<  0 | (B & 1u <<  0) <<  1;
  R |= (A & 1u <<  1) <<  1 | (B & 1u <<  1) <<  2;
  R |= (A & 1u <<  2) <<  2 | (B & 1u <<  2) <<  3;
  R |= (A & 1u <<  3) <<  3 | (B & 1u <<  3) <<  4;
  R |= (A & 1u <<  4) <<  4 | (B & 1u <<  4) <<  5;
  R |= (A & 1u <<  5) <<  5 | (B & 1u <<  5) <<  6;
  R |= (A & 1u <<  6) <<  6 | (B & 1u <<  6) <<  7;
  R |= (A & 1u <<  7) <<  7 | (B & 1u <<  7) <<  8;
  R |= (A & 1u <<  8) <<  8 | (B & 1u <<  8) <<  9;
  R |= (A & 1u <<  9) <<  9 | (B & 1u <<  9) << 10;
  R |= (A & 1u << 10) << 10 | (B & 1u << 10) << 11;
  R |= (A & 1u << 11) << 11 | (B & 1u << 11) << 12;
  R |= (A & 1u << 12) << 12 | (B & 1u << 12) << 13;
  R |= (A & 1u << 13) << 13 | (B & 1u << 13) << 14;
  R |= (A & 1u << 14) << 14 | (B & 1u << 14) << 15;
  R |= (A & 1u << 15) << 15 | (B & 1u << 15) << 16;
  
  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Bolitho::DeInterleave(UINT32 X, UINT16& A, UINT16& B)
{
  A = B = 0;

  A |= (X & 1u << 0);
  A |= (X & 1u << 2) >> 1;
  A |= (X & 1u << 4) >> 2;
  A |= (X & 1u << 6) >> 3;
  A |= (X & 1u << 8) >> 4;
  A |= (X & 1u << 10) >> 5;
  A |= (X & 1u << 12) >> 6;
  A |= (X & 1u << 14) >> 7;
  A |= (X & 1u << 16) >> 8;
  A |= (X & 1u << 18) >> 9;
  A |= (X & 1u << 20) >> 10;
  A |= (X & 1u << 22) >> 11;
  A |= (X & 1u << 24) >> 12;
  A |= (X & 1u << 26) >> 13;
  A |= (X & 1u << 28) >> 14;
  A |= (X & 1u << 30) >> 15;

  B |= (X & 1u << 1) >> 1;
  B |= (X & 1u << 3) >> 2;
  B |= (X & 1u << 5) >> 3;
  B |= (X & 1u << 7) >> 4;
  B |= (X & 1u << 9) >> 5;
  B |= (X & 1u << 11) >> 6;
  B |= (X & 1u << 13) >> 7;
  B |= (X & 1u << 15) >> 8;
  B |= (X & 1u << 17) >> 9;
  B |= (X & 1u << 19) >> 10;
  B |= (X & 1u << 21) >> 11;
  B |= (X & 1u << 23) >> 12;
  B |= (X & 1u << 25) >> 13;
  B |= (X & 1u << 27) >> 14;
  B |= (X & 1u << 29) >> 15;
  B |= (X & 1u << 31) >> 16;

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
UINT64 Bolitho::Interleave(UINT32 a, UINT32 b)
{
  UINT64 A = a;
  UINT64 B = b;
  
  UINT64 R = 0;
  
  R |= (A & 1u <<  0) <<  0 | (B & 1u <<  0) <<  1;
  R |= (A & 1u <<  1) <<  1 | (B & 1u <<  1) <<  2;
  R |= (A & 1u <<  2) <<  2 | (B & 1u <<  2) <<  3;
  R |= (A & 1u <<  3) <<  3 | (B & 1u <<  3) <<  4;
  R |= (A & 1u <<  4) <<  4 | (B & 1u <<  4) <<  5;
  R |= (A & 1u <<  5) <<  5 | (B & 1u <<  5) <<  6;
  R |= (A & 1u <<  6) <<  6 | (B & 1u <<  6) <<  7;
  R |= (A & 1u <<  7) <<  7 | (B & 1u <<  7) <<  8;
  R |= (A & 1u <<  8) <<  8 | (B & 1u <<  8) <<  9;
  R |= (A & 1u <<  9) <<  9 | (B & 1u <<  9) << 10;
  R |= (A & 1u << 10) << 10 | (B & 1u << 10) << 11;
  R |= (A & 1u << 11) << 11 | (B & 1u << 11) << 12;
  R |= (A & 1u << 12) << 12 | (B & 1u << 12) << 13;
  R |= (A & 1u << 13) << 13 | (B & 1u << 13) << 14;
  R |= (A & 1u << 14) << 14 | (B & 1u << 14) << 15;
  R |= (A & 1u << 15) << 15 | (B & 1u << 15) << 16;
  R |= (A & 1u << 16) << 16 | (B & 1u << 16) << 17;
  R |= (A & 1u << 17) << 17 | (B & 1u << 17) << 18;
  R |= (A & 1u << 18) << 18 | (B & 1u << 18) << 19;
  R |= (A & 1u << 19) << 19 | (B & 1u << 19) << 20;
  R |= (A & 1u << 20) << 20 | (B & 1u << 20) << 21;
  R |= (A & 1u << 21) << 21 | (B & 1u << 21) << 22;
  R |= (A & 1u << 22) << 22 | (B & 1u << 22) << 23;
  R |= (A & 1u << 23) << 23 | (B & 1u << 23) << 24;
  R |= (A & 1u << 24) << 24 | (B & 1u << 24) << 25;
  R |= (A & 1u << 25) << 25 | (B & 1u << 25) << 26;
  R |= (A & 1u << 26) << 26 | (B & 1u << 26) << 27;
  R |= (A & 1u << 27) << 27 | (B & 1u << 27) << 28;
  R |= (A & 1u << 28) << 28 | (B & 1u << 28) << 29;
  R |= (A & 1u << 29) << 29 | (B & 1u << 29) << 30;
  R |= (A & 1u << 30) << 30 | (B & 1u << 30) << 31;
  R |= (A & 1u << 31) << 31 | (B & 1u << 31) << 32;
  
  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
UINT32 Bolitho::Interleave(UINT8 A, UINT8 B, UINT8 C)
{
  UINT32 R = 0;
  
  R |= (A & 1 << 0) <<  0 | (B & 1 << 0) <<  1 | (C & 1 << 0) <<  2;
  R |= (A & 1 << 1) <<  2 | (B & 1 << 1) <<  3 | (C & 1 << 1) <<  4;
  R |= (A & 1 << 2) <<  4 | (B & 1 << 2) <<  5 | (C & 1 << 2) <<  6;
  R |= (A & 1 << 3) <<  6 | (B & 1 << 3) <<  7 | (C & 1 << 3) <<  8;
  R |= (A & 1 << 4) <<  8 | (B & 1 << 4) <<  9 | (C & 1 << 4) << 10;
  R |= (A & 1 << 5) << 10 | (B & 1 << 5) << 11 | (C & 1 << 5) << 12;
  R |= (A & 1 << 6) << 12 | (B & 1 << 6) << 13 | (C & 1 << 6) << 14;
  R |= (A & 1 << 7) << 14 | (B & 1 << 7) << 15 | (C & 1 << 7) << 16;
  
  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
UINT64 Bolitho::Interleave(UINT16 a, UINT16 b, UINT16 c)
{
  UINT64 R = 0;
  UINT64 A = a;
  UINT64 B = b;
  UINT64 C = c;
  
  R |= (A & 1 <<  0) <<  0 | (B & 1 <<  0) <<  1 | (C & 1 <<  0) <<  2;
  R |= (A & 1 <<  1) <<  2 | (B & 1 <<  1) <<  3 | (C & 1 <<  1) <<  4;
  R |= (A & 1 <<  2) <<  4 | (B & 1 <<  2) <<  5 | (C & 1 <<  2) <<  6;
  R |= (A & 1 <<  3) <<  6 | (B & 1 <<  3) <<  7 | (C & 1 <<  3) <<  8;
  R |= (A & 1 <<  4) <<  8 | (B & 1 <<  4) <<  9 | (C & 1 <<  4) << 10;
  R |= (A & 1 <<  5) << 10 | (B & 1 <<  5) << 11 | (C & 1 <<  5) << 12;
  R |= (A & 1 <<  6) << 12 | (B & 1 <<  6) << 13 | (C & 1 <<  6) << 14;
  R |= (A & 1 <<  7) << 14 | (B & 1 <<  7) << 15 | (C & 1 <<  7) << 16;
  R |= (A & 1 <<  8) << 16 | (B & 1 <<  8) << 17 | (C & 1 <<  8) << 18;
  R |= (A & 1 <<  9) << 18 | (B & 1 <<  9) << 19 | (C & 1 <<  9) << 20;
  R |= (A & 1 << 10) << 20 | (B & 1 << 10) << 21 | (C & 1 << 10) << 22;
  R |= (A & 1 << 11) << 22 | (B & 1 << 11) << 23 | (C & 1 << 11) << 24;
  R |= (A & 1 << 12) << 24 | (B & 1 << 12) << 25 | (C & 1 << 12) << 26;
  R |= (A & 1 << 13) << 26 | (B & 1 << 13) << 27 | (C & 1 << 13) << 28;
  R |= (A & 1 << 14) << 28 | (B & 1 << 14) << 29 | (C & 1 << 14) << 30;
  R |= (A & 1 << 15) << 30 | (B & 1 << 15) << 31 | (C & 1 << 15) << 32;
  
  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
