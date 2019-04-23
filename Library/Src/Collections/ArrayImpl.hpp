/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Collections/ArrayImpl.hpp $
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

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementTemplate4RuntimeType(Bolitho,Array,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
Array<T,ElementTraits,Allocator,ThreadingModel>::Array()
{
  m_Length = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
Array<T,ElementTraits,Allocator,ThreadingModel>::Array(CONST T* pData, SIZE_T N)
{
  m_Length = 0;
  Resize(N);
  if (N > 0)
    CopyObjects(&m_Buffer[0], pData, N);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
Array<T,ElementTraits,Allocator,ThreadingModel>::Array(SIZE_T N)
{
  m_Buffer.Allocate(N);
  m_Length = N;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
Array<T,ElementTraits,Allocator,ThreadingModel>::Array(SIZE_T N, CONST T& Item)
{
  m_Buffer.Allocate(N);
  m_Length = N;
  for (SIZE_T i=0; i<N; i++)
    m_Buffer[i] = Item;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
Array<T,ElementTraits,Allocator,ThreadingModel>::Array(CONST Array& rkReference)
{
  m_Length = rkReference.m_Length;
  m_Buffer = rkReference.m_Buffer;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
VOID Array<T,ElementTraits,Allocator,ThreadingModel>::Resize(SIZE_T N)
{
  m_Buffer.Resize(N);
  m_Length = N;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
VOID Array<T,ElementTraits,Allocator,ThreadingModel>::ResizeRelative(PTRDIFF_T d)
{
  Resize(m_Length + d);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
VOID Array<T,ElementTraits,Allocator,ThreadingModel>::InternalResize(SIZE_T N)
{
  if (N > m_Buffer.Length())
    if (m_Length < 16)
      m_Buffer.Resize(N);
    else
      m_Buffer.Resize((INT)((FLOAT)N * 1.5));
  m_Length = N;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
VOID Array<T,ElementTraits,Allocator,ThreadingModel>::Reserve(SIZE_T N)
{
  m_Buffer.Resize(N);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
VOID Array<T,ElementTraits,Allocator,ThreadingModel>::Swap(SIZE_T From, SIZE_T To)
{
  ElementTraits::Swap(m_Buffer[From], m_Buffer[To]);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
Array<T,ElementTraits,Allocator,ThreadingModel>& Array<T,ElementTraits,Allocator,ThreadingModel>::operator += (CONST Array& rkReference)
{
  Add(rkReference);
  return *this;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
Array<T,ElementTraits,Allocator,ThreadingModel>& Array<T,ElementTraits,Allocator,ThreadingModel>::operator += (CONST T& Item)
{
  Add(Item);
  return *this;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
T& Array<T,ElementTraits,Allocator,ThreadingModel>::Add(CONST Array<T,ElementTraits,Allocator,ThreadingModel>& rkReference)
{
  SIZE_T Length = m_Length;
  InternalResize(m_Length+rkReference.m_Length);
  CopyObjects(&m_Buffer[Length], &rkReference.m_Buffer[0], rkReference.m_Length);
  return m_Buffer[Length];
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
T& Array<T,ElementTraits,Allocator,ThreadingModel>::Add(CONST T& Item)
{
  SIZE_T Length = m_Length;
  InternalResize(m_Length+1);
  m_Buffer[Length] = Item;
  return m_Buffer[Length];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
VOID Array<T,ElementTraits,Allocator,ThreadingModel>::Insert(CONST T& Item, SIZE_T Position)
{
  InternalResize(m_Length + 1);
  CopyObjects(&m_Buffer[Position+1], &m_Buffer[Position] , m_Length - 1 - Position);
  m_Buffer[Position] = Item;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
VOID Array<T,ElementTraits,Allocator,ThreadingModel>::Insert(CONST Array& rkArray, SIZE_T Position)
{
  SIZE_T OldLength = m_Length;
  InternalResize(m_Length + rkArray.m_Length);
  m_Buffer.DataCopy(&m_Buffer[Position+rkArray.m_Length], &m_Buffer[Position] , OldLength - Position);
  m_Buffer.DataCopy(&m_Buffer[Position], &rkArray.m_Buffer[0], rkArray.m_Length);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
SIZE_T Array<T,ElementTraits,Allocator,ThreadingModel>::Remove(CONST T& Item)
{
  SIZE_T Begin = this->Begin();
  SIZE_T End = this->End();

  while (Begin != End)
  {
    if (m_Buffer[Begin] == Item)
    {
      T Temp = m_Buffer[Begin];
      CopyObjects(&m_Buffer[Begin], &m_Buffer[Begin+1], End-Begin);
      End--;
      m_Buffer[End] = Temp;
    }
    else
      Begin++;
  }
  return End;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
template<class Predicate>
SIZE_T Array<T,ElementTraits,Allocator,ThreadingModel>::Remove(CONST Predicate& P)
{
  SIZE_T Begin = this->Begin();
  SIZE_T End = this->End();

  while (Begin != End)
  {
    if (P(m_Buffer[Begin]))
    {
      T Temp = m_Buffer[Begin];
      m_Buffer.DataCopy(&m_Buffer[Begin], &m_Buffer[Begin+1], End-Begin);
      End--;
      m_Buffer[End] = Temp;
    }
    else
      Begin++;
  }
  return End;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
T Array<T,ElementTraits,Allocator,ThreadingModel>::EraseLast()
{
  Assert(m_Length > 0);
  T Item = m_Buffer[m_Length-1];
  Resize(m_Length - 1);
  return Item;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
//template<class T, class ElementTraits, class Allocator, class ThreadingModel>
//SIZE_T Array<T,ElementTraits,Allocator,ThreadingModel>::Erase(typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator EraseBegin, typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator EraseEnd)
//{
//  typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Begin = this->Begin();
//  SIZE_T EraseSize = EraseEnd - EraseBegin;
//  SIZE_T BeforeSize = EraseBegin - Begin;
//  m_Buffer.DataCopy(EraseBegin.m_pItem, EraseEnd.m_pItem, m_Length - BeforeSize - EraseSize);
//  Resize(m_Length - EraseSize);
//  return EraseSize;
//}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
SIZE_T Array<T,ElementTraits,Allocator,ThreadingModel>::Erase(SIZE_T At)
{
  SIZE_T EraseSize = m_Length - At;
  Resize(At);
  return EraseSize;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
SIZE_T Array<T,ElementTraits,Allocator,ThreadingModel>::Erase(SIZE_T EraseBegin, SIZE_T EraseEnd)
{
  SIZE_T EraseSize = EraseEnd - EraseBegin;
  SIZE_T BeforeSize = EraseBegin;
  m_Buffer.DataCopy(&m_Buffer[EraseBegin], &m_Buffer[EraseEnd], m_Length - BeforeSize - EraseSize);
  Resize(m_Length - EraseSize);
  return EraseSize;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
SIZE_T Array<T,ElementTraits,Allocator,ThreadingModel>::Begin() CONST
{
  return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
SIZE_T Array<T,ElementTraits,Allocator,ThreadingModel>::End() CONST
{
  return m_Length;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
SIZE_T Array<T,ElementTraits,Allocator,ThreadingModel>::Find(CONST T& Item)
{
  return Find(Item, Begin(), End());
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
SIZE_T Array<T,ElementTraits,Allocator,ThreadingModel>::Find(CONST T& Item, SIZE_T Begin, SIZE_T End)
{
  while(Begin != End)
    if (ElementTraits::CompareEqual(m_Buffer[Begin],Item))
      return Begin;
    else
      Begin++;
  return End;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
SIZE_T Array<T,ElementTraits,Allocator,ThreadingModel>::BinarySearch(CONST T& Item)
{
  return BinarySearch(Item, Begin(), End());
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
SIZE_T Array<T,ElementTraits,Allocator,ThreadingModel>::BinarySearch(CONST T& Item, SIZE_T Begin, SIZE_T End)
{
  while (Begin != End)
  {
    if (ElementTraits::CompareLessThan(Item, m_Buffer[Begin]))
      return End;
    if (LessThan(m_Buffer[End-1], Item))
      return End;

    SIZE_T Middle = Begin + (End-Begin)/2;
    if (ElementTraits::CompareLessThan(Item, m_Buffer[Middle]))
      End = Middle;
    else if (ElementTraits::CompareLessThan(m_Buffer[Middle], Item))
      Begin = Middle;
    else
      return Middle;
  }
  return End;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
VOID Array<T,ElementTraits,Allocator,ThreadingModel>::Sort()
{
  return Sort(Begin(), End());
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
VOID Array<T,ElementTraits,Allocator,ThreadingModel>::Sort(SIZE_T Begin, SIZE_T End)
{
  CONST int STKSIZE = (8*sizeof(VOID*) - 2);

  SIZE_T Pivot;
  SIZE_T LowStack[STKSIZE];
  SIZE_T HighStack[STKSIZE];
  unsigned StackP = 1;

  do
  {
    if (Begin < End)
    {

      if (End-Begin < 10)
      {
        InsertionSort(Begin,End);

        StackP--;
        Begin = LowStack[StackP];
        End = HighStack[StackP];
        continue;
      }

      Pivot = SortPartition(Begin,End);
      if (Pivot-1-Begin < End-Pivot+1)
      {
        //QSort(l, Pivot-1);
        LowStack[StackP] = Pivot+1;
        HighStack[StackP] = End;
        StackP++;
        End = Pivot;
      }
      else
      {
        //QSort(Pivot+1, h);
        LowStack[StackP] = Begin;
        HighStack[StackP] = Pivot;
        StackP++;
        Begin = Pivot+1;

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

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
VOID Array<T,ElementTraits,Allocator,ThreadingModel>::InsertionSort(SIZE_T Begin, SIZE_T End)
{
  for (SIZE_T j = Begin + 1; j < End; j++)
  {
    T key = m_Buffer[j];
    SIZE_T i = j - 1;
    while (i >= Begin && i < End && ElementTraits::CompareLessThan(key, m_Buffer[i]))
    {
      m_Buffer[i+1] = m_Buffer[i];
      i--;
    }
    m_Buffer[i+1] = key;
  }
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
SIZE_T Array<T,ElementTraits,Allocator,ThreadingModel>::SortPartition(SIZE_T Begin, SIZE_T End)
{
  // Choosing Pivot Item as median of l-mid-h.
  SIZE_T Middle = Begin + ((End-Begin)>>1);
  End--;

  if (ElementTraits::CompareLessThan(m_Buffer[Middle], m_Buffer[Begin]))
    Swap(Middle, Begin);
  if (ElementTraits::CompareLessThan(m_Buffer[End], m_Buffer[Begin]))
    Swap(End, Begin);
  if (ElementTraits::CompareLessThan(m_Buffer[End], m_Buffer[Middle]))
    Swap(End, Middle);

  Swap(Middle, Begin);

  SIZE_T Pivot = Begin;

  while (Begin < End) 
  {
    /* Move left while item <= pivot */
    while(Begin != End)
      if (ElementTraits::CompareLessThan(m_Buffer[Begin], m_Buffer[Pivot])) 
        Begin++;   // less
      else if (ElementTraits::CompareLessThan(m_Buffer[Pivot], m_Buffer[Begin]))
        break;
      else
        Begin++;   // equal

      /* Move right while item > pivot */
      while(ElementTraits::CompareLessThan(m_Buffer[Pivot], m_Buffer[End])) 
        End--;

      if (Begin < End) 
        Swap(Begin,End);
  }
  /* right is final position for the pivot */
  Swap(Pivot, End);
  return End;

}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline Bolitho::IO::BinaryStream& operator << (Bolitho::IO::BinaryStream& S, CONST Array<T,ElementTraits,Allocator,ThreadingModel>& A)
{
  S << (ULONGLONG)A.Length();
  for(SIZE_T i=0; i<A.Length(); i++)
    S << A[i];
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline Bolitho::IO::BinaryStream& operator >> (Bolitho::IO::BinaryStream& S, Array<T,ElementTraits,Allocator,ThreadingModel>& A)
{
  ULONGLONG N = S.ReadUInt64();
#if defined(PLATFORM_32BIT)
  if (N > 0x7fffffff)
    throw InvalidOperationException(0, "Unable to load array -- there are more entries than can be addressed on PLATFORM_32BIT");
#endif
  A.Resize((SIZE_T)N);
  for(SIZE_T i=0; i<(SIZE_T)N; i++)
    S >> A[i];  
  return S;
}

