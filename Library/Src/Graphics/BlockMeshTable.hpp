/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/BlockMeshTable.hpp $
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

public:
class LIBRARY_API DataTable
{
  friend class BlockMesh;
public:
  /* Applys a hint to a region of the tables data */
  VOID Hint(UINT Offset, UINT Length, DWORD Hint);

  /* Bulk loads the table */
  VOID Load(Ptr<IO::Stream> pSource, UINT TableOffset, UINT Length);
  VOID Load(LPCVOID pSource, UINT TableOffset, UINT Length);

  /* Bulk dumps the table */
  VOID Dump(Ptr<IO::Stream> pDestination, UINT TableOffset, UINT Length);
  VOID Dump(LPVOID pDestination, UINT TableOffset, UINT Length);

  /* Returns TRUE if the table contains a specific attribute */
  BOOL HasAttribute(CONST ElementDecl& A) CONST;

  /* Returns the offset of an attribute within the table, or -1 if the attribute does not exist */
  UINT GetAttributeOffset(CONST ElementDecl& A) CONST;
  UINT GetAttributeOffset(UINT i) CONST;

  /* Returns the size of an attribute */
  UINT GetAttributeSize(CONST ElementDecl& A) CONST;
  UINT GetAttributeSize(UINT i) CONST;

  /* Returns the index of an attribute in the table, or -1 if the attribute does not exist */
  UINT GetAttributeIndex(CONST ElementDecl& A) CONST;

  /* Gets the number of attributes */
  UINT GetAttributeCount() CONST
  { return m_pHeader->AttributeCount; }

  /* Adds an attribute to the table.  
  * If there is free space within the element, the space will be allocated to the new attribute
  * If there is no free space within the element, the table is resized and free space is added to each element */
  VOID AddAttribute(CONST ElementDecl& A);

  /* Removes an attribute from the table */
  VOID RemoveAttribute(CONST ElementDecl& A);

  /* Removes all attributes from the table */
  VOID RemoveAllAttributes();

  /* Sets the complete element declaration */
  VOID SetElementDecl(CONST Array<ElementDecl>& Decl);

  /* Gets the i-th attribute declaration */
  CONST ElementDecl& GetAttribute(UINT i);

  /* Gets all attribute declarations */
  CONST Array<ElementDecl>& GetAttributes() CONST;

  /* Returns the number of rows in the table */
  UINT GetCount() CONST
  { return m_pHeader->Count; }

  /* Returns the number of blocks in the table */
  UINT GetBlockCount() CONST
  { return m_pHeader->Storage.BlockCount; }

  /* Returns the number of rows in a given block */
  UINT GetBlockRowCount(UINT i) CONST;
  UINT GetBlockRowCount() CONST
  { return m_pHeader->CountPerBlock; }

  /* Returns the number of rows in a given block */
  UINT GetBlockSize(UINT i) CONST
  { return GetBlockRowCount(i) * GetElementSize(); }

  /* Returns the size of the table, in bytes */
  ULONGLONG GetSize() CONST
  { return (ULONGLONG)m_pHeader->Count * m_pHeader->ElementSize; }

  /* Returns the size of an element, in bytes */
  UINT GetElementSize() CONST
  { return m_pHeader->ElementSize; }

  /* Returns a pointer to the i-th block */
  LPVOID GetBlock(UINT i)
  { return m_Table.GetBlock(i); }

  /* Returns a pointer to the i-th block data */
  LPVOID GetBlockData(UINT i)
  { return m_Table.GetBlock(i); }

  /* Resizes the table */
  VOID Resize(UINT Count);

  /* Returns a pointer to the i-th row in the table */
  template<class T>
  T* Row(UINT i);

  /* Returns a pointer to the i-th row in the table */
  template<class T>
  CONST T* Row(UINT i) CONST;

  /* Returns a reference to an attribute in a given row */
  template<class T>
  T& Attribute(UINT Index, UINT Offset);

  /* Returns a reference to an attribute in a given row */
  template<class T>
  CONST T& Attribute(UINT Index, UINT Offset) CONST;

  /* Writes all dirty data blocks to disk */
  VOID Flush()
  { m_Table.Flush(); }

  /* Appends a new record to the end of the table.  Returns the new record index */
  UINT Append(LPVOID pRecordData);

protected:
  DataTable(BlockMesh& M);
  ~DataTable();

  VOID Initialize(TABLEHEADER* pHeader);

  /* The BlockMesh */
  BlockMesh& m_Mesh;

  IO::BlockStore::Table m_Table;

  /* The element type declaration */
  Array<ElementDecl> m_Declaration;

  /* A pointer to the Tables header information in the BlockMesh files data header */
  TABLEHEADER* m_pHeader;
};

public:
/* Performance hints when using table data */
struct TableHint
{ enum { PREFETCH = 1, FINISHED = 2, FLUSH = 4 }; };
