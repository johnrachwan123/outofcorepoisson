/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/IO/BlockStore.hpp $
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

#include "IO/File.hpp"
#include "IO/MemoryMappedFile.hpp"
#include "Collections/Map.hpp"
#include "Utility/Nullable.hpp"

namespace Bolitho
{
  namespace IO
  {
      class LIBRARY_API BlockStoreException : public UserException
      {
        DeclareRuntimeType();
      public:
        BlockStoreException(CONST Object* Sender, LPCTSTR Description) : UserException(Sender)
        { m_Description = Description; }

        BlockStoreException(CONST BlockStoreException& E) : UserException(E)
        { m_Description = E.m_Description; }

        virtual String Description() CONST
        { return m_Description; }

      private:
        String m_Description;
      };

      /* A block-based data store */
      class LIBRARY_API BlockStore : public Object
      {
      public:
        typedef UINT32 BLOCKADDRESS;
        typedef UINT32 BLOCKCOUNT;
        typedef UINT32 BLOCKSIZE;

        DeclareRuntimeType();
      public:
        /* A table that presents a simple collection of blocks */
        class Table : public Object
        {
          friend class DataStore;
        public:
          class LIBRARY_API Stream : public IO::Stream
          {
          public:
            Stream(Table* pTable, ULONGLONG Offset);
            Stream(BlockStore* pStore, UINT Directory, UINT BlockCount, ULONGLONG Offset);

            virtual VOID Flush();
            virtual VOID Close();

            virtual SIZE_T Write(LPCVOID pData, SIZE_T N);
            virtual SIZE_T Read(LPVOID pData, SIZE_T N);

            virtual BOOL EndOfStream();

          private:
            Table* m_pTable;
            BLOCKADDRESS m_CurrentBlock;
            BLOCKSIZE m_CurrentOffset;
          };

          Table(BlockStore* pBlockStore);
          virtual ~Table();

          VOID Initialize(BLOCKADDRESS RootTranslationTable, BLOCKCOUNT Count);

          /* Resizes the table */
          VOID Resize(BLOCKCOUNT Blocks);

          /* Gets a pointer to the data block */
          LPCVOID GetBlock(BLOCKADDRESS Address) CONST;
          LPVOID GetBlock(BLOCKADDRESS Address);

          /* Locks a block in the table */
          VOID Lock(BLOCKADDRESS Address);
          /* Unlocks a block in the table */
          VOID Unlock(BLOCKADDRESS Address);

          /* Translates a table address into a data store address */
          BLOCKADDRESS Translate(BLOCKADDRESS TableAddress) CONST;

          /* Returns the size of the table in blocks */
          BLOCKADDRESS GetBlockCount() CONST
          { return m_BlockCount; }

          /* Gets the block size */
          BLOCKSIZE GetBlockSize() CONST
          { return m_pBlockStore->GetBlockSize(); }

          /* Flushes block directories to the data store */
          VOID Flush();

          /* Allocates a block from the underlying block store */
          BLOCKADDRESS AllocateBlock(BOOL InitializeToZero = FALSE)
          { return m_pBlockStore->AllocateBlock(InitializeToZero); }

          /* Gets a table stream */
          Ptr<Stream> GetStream(ULONGLONG Offset = 0);

          /* Updates a directory mapping */
          BLOCKADDRESS UpdateDirectory(BLOCKADDRESS TableAddress, BLOCKADDRESS PhysicalAddress) CONST;
        protected:
          
          mutable BLOCKADDRESS m_TranslationLookup[2];

          BLOCKADDRESS m_RootTranslationTableBlock;
          BLOCKADDRESS* m_pRootTranslationTable;
          BLOCKADDRESS** m_ppTranslationTable;
          
          BLOCKCOUNT m_BlockCount;

          BlockStore* m_pBlockStore;
        };
       
        struct STOREHEADER
        {
          UINT64 Signature;
          UINT32 Version;
          BLOCKCOUNT BlockCount;
          BLOCKCOUNT FreeBlockCount;
          BLOCKSIZE BlockSize;
          BLOCKADDRESS AllocationBitmapBlock[32];
          BYTE UserData[4058];
        };

        BlockStore();
        virtual ~BlockStore();

        /* Create an in-memory datastore */
        VOID InitializeInMemory(BLOCKSIZE BlockSize);
        /* Create a persistant datastore */
        VOID InitializeCreate(LPCTSTR Filename, BLOCKSIZE BlockSize);
        /* Open a persistant datastore */
        VOID InitializeOpen(LPCTSTR Filename);

        /* Closes the blockstore */
        VOID Close();

        /* Returns a pointer to the user-data section of the header block */
        template<class USERSTRUCT>
        USERSTRUCT* GetHeaderStruct()
        { return (USERSTRUCT*)&m_Header.UserData; }

        /* Returns a pointer to the user-data section of the header block */
        LPVOID GetHeaderData()
        { return &m_Header.UserData; }

        /* Returns a pointer to the data sotre header */
        CONST STOREHEADER* GetHeader()
        { return &m_Header; }

        /* Flushes all dirty data to disk */
        VOID Flush();

        /* Resizes the block store.
         * If BLOCKADDRESS is less than the current size of the store, blocks will be freed if not already allocated.
         */
        VOID Resize(BLOCKCOUNT Count);

        /* Allocates a new block.  Increases the size of the store if needed */
        BLOCKADDRESS AllocateBlock(BOOL InitializeToZero = FALSE);
        /* Frees a block */
        VOID FreeBlock(BLOCKADDRESS Block);

        /* The total number of blocks in the data store */
        BLOCKCOUNT GetBlockCount() CONST
        { return m_Header.BlockCount; }

        /* The number of free blocks in the data store */
        BLOCKCOUNT GetFreeBlockCount() CONST
        { return m_Header.FreeBlockCount; }

        /* The size of a block in bytes */
        BLOCKSIZE GetBlockSize() CONST
        { return m_Header.BlockSize; }

        /* Returns a pointer to the memory location corresponding to a given block */
        LPVOID GetBlock(BLOCKADDRESS Block)
        { if (m_Header.Version == 0) return m_pBlocks[Block]; else return (LPBYTE)m_pView->GetBaseAddress() + Block*m_Header.BlockSize; }
        LPCVOID GetBlock(BLOCKADDRESS Block) CONST
        { if (m_Header.Version == 0) return m_pBlocks[Block]; else return (LPCBYTE)m_pView->GetBaseAddress() + Block*m_Header.BlockSize; }

        /* Pins a block in memory */
        VOID Lock(BLOCKADDRESS Block);
        /* Unpins a block in memory */
        VOID Unlock(BLOCKADDRESS Block);

        STATIC BLOCKCOUNT GetMaximumStoreSize(BLOCKSIZE BlockSize)
        { return BlockSize * 8 * 32; }

        STATIC BLOCKCOUNT GetMaximumTableSize(BLOCKSIZE BlockSize)
        { return BlockSize / sizeof(BLOCKADDRESS) * BlockSize / sizeof(BLOCKADDRESS); }

      protected:
        /* The latest data store version number */
        STATIC CONST UINT32 BLOCKSTOREVERSION = 0x0101;
        STATIC CONST UINT64 BLOCKSTORESIGNATURE = 0x12345678;

        /* The Mesh file */
        Ptr<IO::File> m_pFile;
        Ptr<IO::FileView> m_pView;

        /* In memory block map */
        Array<LPVOID> m_pBlocks;

        /* Internal Allocation */
        BLOCKADDRESS AllocateBlock(BOOL InitializeToZero, BOOL IncreaseStoreSpace, BOOL IncreaseBitmapSpace);

        /* Header Information */
        STOREHEADER m_Header;

        /* Block Allocation Bitmap */
        DWORD** m_ppBlockBitmaps;

        /* Data Tables */
        Array<Ptr<Table>> m_Tables;

      };

      #include "BlockStoreImpl.hpp"

    }
}
