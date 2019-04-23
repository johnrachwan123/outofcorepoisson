/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Utility/SortImpl.hpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1597 $
 * Last Updated: $Date: 2008-08-28 13:59:42 -0700 (Thu, 28 Aug 2008) $
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

//---------------------------------------------------------------------------------------------------
template<class Comparator>
VOID Sort(LPVOID pArray, SIZE_T Count, SIZE_T ElementSize, Comparator& C)
{
  Sort((LPBYTE)pArray, 0, Count, ElementSize, C);
}
//---------------------------------------------------------------------------------------------------
template<class Comparator>
VOID Sort(LPVOID* ppArray, SIZE_T PartitionCount, SIZE_T Count, SIZE_T* pElementSize, Comparator& C)
{
  Sort((LPBYTE*)ppArray, PartitionCount, 0, Count, pElementSize, C);
}
//---------------------------------------------------------------------------------------------------
template<class Comparator>
VOID Sort(LPBYTE* ppData, SIZE_T PartitionCount, SIZE_T Begin, SIZE_T End, SIZE_T* pElementSize, Comparator& C)
{
  CONST SIZE_T STKSIZE = (8 * sizeof(VOID*) - 2);

  SIZE_T Pivot;
  SIZE_T LowStack[STKSIZE];
  SIZE_T HighStack[STKSIZE];
  UINT StackP = 1;

  do
  {
    if (Begin < End)
    {
      /*
      if (End-Begin < 10*ElementSize)
      {
        InsertionSort(Begin, End, ElementSize, C);

        StackP--;
        Begin = LowStack[StackP];
        End = HighStack[StackP];
        continue;
      }
      */

      Pivot = SortPartition(ppData, PartitionCount, Begin, End, pElementSize, C);
      if (Pivot-1-Begin < End-Pivot+1)
      {
        // Sort(l, Pivot-1);
        LowStack[StackP] = Pivot+1;
        HighStack[StackP] = End;
        StackP++;
        End = Pivot;
      }
      else
      {
        // Sort(Pivot+1, h);
        LowStack[StackP] = Begin;
        HighStack[StackP] = Pivot;
        StackP++;
        Begin = Pivot + 1;
      }
    }
    else
    {
      StackP--;
      Begin = LowStack[StackP];
      End = HighStack[StackP];
    }
  }
  while (StackP > 0);
}
//---------------------------------------------------------------------------------------------------
template<class Comparator>
VOID Sort(LPBYTE pData, SIZE_T Begin, SIZE_T End, SIZE_T ElementSize, Comparator& C)
{
  CONST SIZE_T STKSIZE = (8 * sizeof(VOID*) - 2);

  SIZE_T Pivot;
  SIZE_T LowStack[STKSIZE];
  SIZE_T HighStack[STKSIZE];
  UINT StackP = 1;

  do
  {
    if (Begin < End)
    {
      /*
      if (End-Begin < 10*ElementSize)
      {
        InsertionSort(Begin, End, ElementSize, C);

        StackP--;
        Begin = LowStack[StackP];
        End = HighStack[StackP];
        continue;
      }
      */

      Pivot = SortPartition(pData, Begin, End, ElementSize, C);
      if (Pivot-1-Begin < End-Pivot+1)
      {
        // Sort(l, Pivot-1);
        LowStack[StackP] = Pivot+1;
        HighStack[StackP] = End;
        StackP++;
        End = Pivot;
      }
      else
      {
        // Sort(Pivot+1, h);
        LowStack[StackP] = Begin;
        HighStack[StackP] = Pivot;
        StackP++;
        Begin = Pivot + 1;
      }
    }
    else
    {
      StackP--;
      Begin = LowStack[StackP];
      End = HighStack[StackP];
    }
  }
  while (StackP > 0);
}
//---------------------------------------------------------------------------------------------------
#define SortDataElement(i, x) &(ppData[i][pElementSize[i]*x])
#define MultiSwap(x, y) for (SIZE_T i=0; i<PartitionCount; i++) Swap(SortDataElement(i, x), SortDataElement(i, y), pElementSize[i]);
template<class Comparator>
SIZE_T SortPartition(LPBYTE* ppData, SIZE_T PartitionCount, SIZE_T Begin, SIZE_T End, SIZE_T* pElementSize, Comparator& C)
{
  SIZE_T Count = (End - Begin);
  SIZE_T Middle = Begin + (Count >> 1);
  End--;

  if (C.CompareLessThan(SortDataElement(0, Middle), SortDataElement(0, Begin)))
    MultiSwap(Middle, Begin);
  if (C.CompareLessThan(SortDataElement(0, End), SortDataElement(0, Begin)))
    MultiSwap(End, Begin);
  if (C.CompareLessThan(SortDataElement(0, End), SortDataElement(0, Middle)))
    MultiSwap(End, Middle);

  MultiSwap(Middle, Begin);

  SIZE_T Pivot = Begin;

  while (Begin < End) 
  {
    /* Move left while item <= pivot */
    while(Begin != End)
      if (C.CompareLessThan(SortDataElement(0, Begin), SortDataElement(0, Pivot))) 
        Begin++;   // less
      else if (C.CompareLessThan(SortDataElement(0, Pivot),SortDataElement(0, Begin)))
        break;
      else
        Begin++;   // equal

      /* Move right while item > pivot */
      while(C.CompareLessThan(SortDataElement(0, Pivot),SortDataElement(0, End))) 
        End--;

      if (Begin < End) 
        MultiSwap(Begin, End);
  }
  /* right is final position for the pivot */
  MultiSwap(Pivot, End);
  return End;
}
//---------------------------------------------------------------------------------------------------
#undef SortDataElement
#define SortDataElement(x) &pData[ElementSize*x]
template<class Comparator>
SIZE_T SortPartition(LPBYTE pData, SIZE_T Begin, SIZE_T End, SIZE_T ElementSize, Comparator& C)
{
  SIZE_T Count = (End - Begin);
  SIZE_T Middle = Begin + (Count >> 1);
  End--;

  if (C.CompareLessThan(SortDataElement(Middle), SortDataElement(Begin)))
    Swap(SortDataElement(Middle), SortDataElement(Begin), ElementSize);
  if (C.CompareLessThan(SortDataElement(End), SortDataElement(Begin)))
    Swap(SortDataElement(End), SortDataElement(Begin), ElementSize);
  if (C.CompareLessThan(SortDataElement(End), SortDataElement(Middle)))
    Swap(SortDataElement(End), SortDataElement(Middle), ElementSize);

  Swap(SortDataElement(Middle), SortDataElement(Begin), ElementSize);

  SIZE_T Pivot = Begin;

  while (Begin < End) 
  {
    /* Move left while item <= pivot */
    while(Begin != End)
      if (C.CompareLessThan(SortDataElement(Begin), SortDataElement(Pivot))) 
        Begin++;   // less
      else if (C.CompareLessThan(SortDataElement(Pivot),SortDataElement(Begin)))
        break;
      else
        Begin++;   // equal

      /* Move right while item > pivot */
      while(C.CompareLessThan(SortDataElement(Pivot),SortDataElement(End))) 
        End--;

      if (Begin < End) 
        Swap(SortDataElement(Begin), SortDataElement(End), ElementSize);
  }
  /* right is final position for the pivot */
  Swap(SortDataElement(Pivot), SortDataElement(End), ElementSize);
  return End;
}
#undef SortDataElement
//---------------------------------------------------------------------------------------------------
template<class Comparator>
VOID InsertionSort(LPVOID pBegin, LPVOID pEnd, SIZE_T ElementSize, Comparator& C)
{
}
//---------------------------------------------------------------------------------------------------
