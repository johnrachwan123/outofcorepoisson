/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Collections/HashMap.hpp $
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
  class LIBRARY_API KeyNotFoundException : public Exception
  {
    DeclareRuntimeType();
  public:
    KeyNotFoundException(CONST Object* Sender, String Key, UINT FirstBin) : Exception(Sender)
    { m_Value = Key; m_FirstBin = FirstBin; }
    KeyNotFoundException(CONST KeyNotFoundException& E) : Exception(E)
    { m_Value = E.m_Value; m_FirstBin = E.m_FirstBin; }

    virtual ~KeyNotFoundException()
    {}

    virtual String Description() CONST
    { return String::Format("The key \'%s\' was not found in the hashtable (FirstBin=%u)", m_Value.ConstStr(), m_FirstBin); }

    String Key()
    { return m_Value; }

  private:
    String m_Value;
    UINT m_FirstBin;
  };

  /*
  template<class K, class V, class KeyElementTraits=Traits<K>, class ValueElementTraits=Traits<V> >
  class DynamicHashMap : public Object
  {
    class Bin;

  public:
    class Enumerator
    {
      friend class DynamicHashMap;
    protected:
      Enumerator(DynamicHashMap* pHashMap);
    public:
      inline operator BOOL();

      inline Enumerator operator ++ ();
      inline Enumerator& operator++ (int);

      BOOL IsValid();
      Pair<K,V> CurrentElement();

    protected:
      DynamicHashMap<K,V>* m_pHashMap;
      SIZE_T m_BinIndex;
      Bin* m_pCurrentBin;
      SIZE_T m_CurrentOffset;
    };

  private:
    class Bin
    {
      friend class DynamicHashMap;
      friend class DynamicHashMap::Enumerator;
    public:
      Bin();
      virtual ~Bin();

      Bin* NextBin();
      CONST Bin* NextBin() CONST;
      
      CONST K& Key(SIZE_T i) CONST;
      K& Key(SIZE_T i);

      CONST V& Value(SIZE_T i) CONST;
      V& Value(SIZE_T i);

      SIZE_T Find(CONST K& Key) CONST;
      SIZE_T Find(CONST K& Key, CONST Bin*& pBin) CONST;
      SIZE_T Find(CONST K& Key, Bin*& pBin);
      SIZE_T Allocate(CONST K& Key, CONST V& Value);

      BOOL IsEmpty() CONST;
    protected:
      VOID Print() CONST;
      static CONST SIZE_T N = 4;

      K m_Keys[N];
      V m_Values[N];
      SIZE_T m_AllocBitmap;
      Bin* m_pNextBin;
    };

    DeclareRuntimeType();
  public:
    DynamicHashMap();
    DynamicHashMap(SIZE_T N);
    virtual ~DynamicHashMap();

    VOID Add(CONST K& Key, CONST V& Value);
    VOID Remove(CONST K& Key);
    BOOL Contains(CONST K& Key) CONST;

    V& Get(CONST K& Key);
    CONST V& Get(CONST K& Key) CONST;
    BOOL Get(CONST K& Key, V& Value) CONST;

//		CONST V& operator[] (CONST K& Key) CONST
//		{ return Get(Key); }
//		V& operator[] (CONST K& Key)
//		{ return Get(Key); }

    SIZE_T Count() CONST;
    SIZE_T Capacity() CONST;

    Enumerator Items();

  private:
    VOID Print() CONST;
    VOID Split(Bin* pBin);
    VOID Resize(SIZE_T N);
    SIZE_T AllocationSize(SIZE_T N) CONST;
    SIZE_T BaseAllocationSize(SIZE_T N) CONST;

    SIZE_T m_BinCount;
    SIZE_T m_BaseBinCount;
    Buffer<Bin> m_Bins;
  };
  */

  template<class K, class V, class KeyElementTraits=Traits<K>, class ValueElementTraits=Traits<V> >
  class HashMap : public Object
  {
    DeclareRuntimeType();
  public:
    HashMap();
    HashMap(SIZE_T InitialSize);

    VOID Resize(SIZE_T NewSize);

    VOID Add(CONST K& Key, CONST V& Value);
    VOID Remove(CONST K& Key);
    BOOL Contains(CONST K& Key) CONST;

    V& Get(CONST K& Key);
    CONST V& Get(CONST K& Key) CONST;
    BOOL Get(CONST K& Key, V& Value) CONST;

    SIZE_T Count() CONST
    { return m_Count; }
    SIZE_T Capacity() CONST
    { return m_BinCount; }
    UINT GetLoadFactor() CONST
    { return (UINT)(100 * m_Count / m_BinCount); }
    
    struct Bin
    {
      Bin()
      { Count = 0; }

      DWORD Count;
      K Key;
      V Value;
    };

  private:
    static CONST UINT INVALIDBIN = MAXUINT;
    static CONST SIZE_T PRIMES[];

    SIZE_T m_BinCountIndex;
    SIZE_T m_Count;
    SIZE_T m_Mask;
    SIZE_T m_MaximumBinCollisions;
    SIZE_T m_NumberOfZeroBins;

    SIZE_T m_BinCount;
    Bin* m_pBins;

    UINT Find(CONST K& Key, CONST Bin* pBins, SIZE_T BinCount) CONST;
    UINT ExhaustiveFind(CONST K& Key, CONST Bin* pBins, SIZE_T BinCount) CONST;

    BOOL Add(CONST K& Key, CONST V& Value, Bin* pBins, SIZE_T BinCount);

    VOID TraceTable(CONST Bin* pBins, SIZE_T BinCount) CONST;
    VOID ValidateCount(CONST Bin* pBins, SIZE_T BinCount) CONST;
  };

  #include "HashMapImpl.hpp"

}

