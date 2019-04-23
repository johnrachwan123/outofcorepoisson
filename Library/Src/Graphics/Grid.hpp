/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/Grid.hpp $
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

#include "Utility/FuncPtr.hpp"
#include "Utility/Progress.hpp"
#include "Utility/Flags.hpp"

#include "IO/MemoryMappedFile.hpp"
#include "Math/Math.hpp"

#include "Math/Vector4.hpp"
#include "Math/Vector3.hpp"
#include "Math/Vector2.hpp"

#include "Utility/ElementDecl.hpp"

namespace Bolitho
{
  namespace Graphics
  {
    class LIBRARY_API GridException : public UserException
    {
      DeclareRuntimeType();
    public:
      GridException(CONST Object* Sender, LPCTSTR Description) : UserException(Sender)
      {
        m_Description = CharTraits<TCHAR>::Duplicate(Description);
      }

      GridException(CONST GridException& E) : UserException(E)
      {
        m_Description = CharTraits<TCHAR>::Duplicate(E.m_Description);
      }

      virtual ~GridException()
      {
        if (m_Description)
          FreeMemory(m_Description);
        m_Description = 0;
      }

      virtual String Description() CONST
      {
        return m_Description;
      }

    private:
      TCHAR* m_Description;
    };

    struct GridFlags
    { enum { IMPORTDEFAULT = 0, EXPORTDEFAULT = 0, FORCEELEMENTTYPE = 1, FORCEGRIDLAYOUT = 2 }; };

    struct GridLayout
    { enum { ROWMAJOR, COLUMNMAJOR };  };

    struct GridBoundary
    { enum { NONE = 0, PERIODIC, REFLECTIVE, CLAMP, ZERO }; };
    
    struct GridDataAccess
    { enum { READ = PageAccess::READ, READWRITE = PageAccess::READWRITE, WRITECOPY = PageAccess::WRITECOPY }; };

    struct GridMode
    { enum { CREATE = IO::FileMode::CREATE, OPEN = IO::FileMode::OPEN }; };

    /* A simple grid structure */
    class LIBRARY_API Grid : public Object
    {
      DeclareRuntimeType();
    public:
      virtual ~Grid();

      /* Creates an in-memory grid */
      Grid();

      /* Creates/Opens an on-disk grid */
      Grid(LPCTSTR Filename, DWORD FileMode, DWORD PageAccess = PageAccess::READWRITE);

      /* Creates a shallow copy of another grid */
      Grid(CONST Grid& G);

      /* Creates a shallow copy of another grid */
      Grid& operator= (CONST Grid& G);

      /* Creates a deep copy of the grid */
      VOID Copy(CONST Grid& G, BOOL DeepCopy = TRUE);

      /* Frees the grid data */
      VOID Free();

      /* The element type declaration */
      Array<ElementDecl> GetElementType();

      /* Sets the element type declaration */
      VOID SetElementType(Array<ElementDecl> Decl);

      /* Determines if a given element type is compatible */
      BOOL IsElementTypeCompatible(Array<ElementDecl> Decl);

      /* Returns the total element size */
      UINT GetElementSize() CONST
      { return m_pHeader->ElementSize; }

      /* Gets the layout of the grid elements in memory */
      DWORD GetGridLayout() CONST
      { return m_pHeader->DataLayout; }
      
      /* Controls the layout of grid data */
      VOID SetGridLayout(DWORD LayoutType, UINT Param);

      /* Gets the current boundary/out-of-bound indexing rule */
      DWORD GetBoundaryConditions() CONST
      { return m_BoundaryConditions; }

      /* Sets the indexing/boundary conditions of the grid data */
      VOID SetBoundaryCondition(DWORD BoundaryConditions)
      { m_BoundaryConditions = BoundaryConditions; }

      /* Import a grid from another format.  
       * The format is determined by filename extension 
       */
      VOID Import(LPCTSTR Filename, DWORD Flags = GridFlags::IMPORTDEFAULT);

      /* Export the mesh to another format.  
       * The format is determined by filename extension 
       */
      VOID Export(LPCTSTR Filename, DWORD Flags = GridFlags::EXPORTDEFAULT);

      /* Flushes all dirty data to disk */
      VOID Flush();

      /* Adds a metadata value to the mesh */
      VOID SetMetaData(LPCTSTR Key, LPCTSTR Value);

      /* Gets a metadata value */
      String GetMetaData(LPCTSTR Key);

      /* Removes a metadata value */
      VOID RemoveMetaData(LPCTSTR Key);

      /* Removes all metadata */
      VOID RemoveAllMetaData();

      /* Resizes a grid */
      VOID Resize(UINT Width, UINT Height);
      VOID Resize(CONST Grid& G)
      { Resize(G.GetWidth(), G.GetHeight()); }

      /* Returns the width of the grid */
      UINT GetWidth(UINT Level = 0) CONST
      { return m_pHeader->Width[Level]; }
      /* Returns the height of the grid */
      UINT GetHeight(UINT Level = 0) CONST
      { return m_pHeader->Height[Level]; }

      UINT GetLevelCount() CONST
      { return m_pHeader->LevelCount; }
      
      /* Element access */
      template<class T>
      CONST T& Element(PTRDIFF_T x, PTRDIFF_T y, UINT Level = 0) CONST;
      template<class T>
      T& Element(PTRDIFF_T x, PTRDIFF_T y, UINT Level = 0);

      /* Bilinear Sampling of grid */
      template<class T>
      T Sample(FLOAT i, FLOAT j, UINT Level = 0) CONST;

      /* Generates a multi-resolution representation of the grid */
      template<class T>
      VOID GenerateMipMaps();

      /* Returns the width of a block */
      //UINT GetBlockCountX() CONST
      //{ return m_pHeader->BlockWidth; }
      /* Returns the height of a block */
      //UINT GetBlockCountY() CONST
      //{ return m_pHeader->BlockHeight; }
      //UINT GetBlockSize() CONST
      //{ return m_pHeader->BlockSize; }

      /* Block access */
      template<class T>
      CONST T* Block(UINT x, UINT y) CONST;
      template<class T>
      T* Block(UINT x, UINT y);

      /* Brings a block into memory */
      VOID LockBlock(UINT x, UINT y);
      /* Removes a block from memory */
      VOID UnlockBlock(UINT x, UINT y);

      template<class T>
      VOID RemapMinMax(FLOAT Power = 1.0f);

      template<class T>
      Pair<T,T> GetMinMax();
      template<class T>
      Pair<T,T> GetMeanStdDev();

    protected:
      /* The current version of the grid file format */
      static CONST DWORD GRIDVERSION = 0x202;
      static CONST ULONGLONG GRIDSIGNATURE = 0x0123456789abcdef;
      
      /* Initialize the grid */
      VOID InitializeInMemory();
      VOID InitializeMapping();
      VOID InitializeOpen(LPCTSTR Filename, DWORD Flags);
      VOID InitializeCreate(LPCTSTR Filename, DWORD Flags);

      struct ELEMENTDECL
      {
        CHAR Name[32];
        DWORD Type;
        UINT Length;
      };

      struct GRIDHEADER
      {
        ULONGLONG Signature;
        DWORD GridVersion;
        UINT Width[32];
        UINT Height[32];
        UINT ElementSize;
        UINT LevelCount;
        DWORD DataLayout;

        ULONGLONG GridDataLength[32];
        ULONGLONG GridDataOffset[32];
        ULONGLONG MetaDataOffset;
        ULONGLONG MetaDataLength;

        UINT ElementCount;
        ELEMENTDECL ElementDecl[32];

        /* Information used at runtime only */
        DWORD RefCount;
      };

      /* The grid header */
      GRIDHEADER* m_pHeader;
      /* The grid data */
      LPVOID m_pGridData[32];

      /* The File <-> Memory mapping for an on-disk grid */
      Ptr<IO::File> m_pFile;
      Ptr<IO::FileView> m_pMappedView;

      /* The current boundary conditions */
      DWORD m_BoundaryConditions;

      /* Meta data */
      Map<String, String> m_MetaData;

      /* Values used to speed up the computation of block and element addresses */
      /* The width of a block in bytes */
      UINT m_BlockSizeBytes[32];

      /* The number of bytes between block rows */
      UINT m_GridBlockStride[32];

      /* The number of bytes between rows */
      UINT m_GridRowStride[32];

      /* The number of bytes between rows in a block */
      UINT m_GridBlockRowStride[32];

      /* The shift count used to turn an element address into a block address */
      UINT m_BlockAddressShift[32];
      /* The mask used to extract the block offset */
      UINT m_BlockOffsetMask[32];

    };

    template<class T>
    class TGrid : public Grid
    {
    public:
      /* Create a new in-memory grid */
      TGrid();

      /* Create/Open an on-disk grid */
      TGrid(LPCTSTR Filename, DWORD FileMode, DWORD PageAccess = PageAccess::READWRITE);

      /* Element access */
      CONST T& Element(PTRDIFF_T i, PTRDIFF_T j) CONST
      { return Grid::Element<T>(i,j); }
      T& Element(PTRDIFF_T i, PTRDIFF_T j)
      { return Grid::Element<T>(i,j); }

      /* Element access */
      CONST T& operator() (PTRDIFF_T i, PTRDIFF_T j) CONST
      { return Grid::Element<T>(i,j); }
      T& operator() (PTRDIFF_T i, PTRDIFF_T j)
      { return Grid::Element<T>(i,j); }

      /* Sample grid */
      T Sample(FLOAT i, FLOAT j) CONST
      { return Grid::Sample<T>(i,j); }

      /* Import a grid from another format.  
       * The format is determined by filename extension 
       */
      VOID Import(LPCTSTR Filename, DWORD Flags = GridFlags::IMPORTDEFAULT);

      /* Convert a grid to this format */
      template<class U>
      VOID Convert(CONST TGrid<U>& G);

      /* Computational Operations */
      VOID RemapMinMax(FLOAT Power = 1.0f)
      { Grid::RemapMinMax<T>(Power); }

      /* Gets the largest and smallest elements in the grid */
      Pair<T,T> GetMinMax()
      { return Grid::GetMinMax<T>(); }

      /* Gets the mean and standard deviation of the values in the grid */
      Pair<T,T> GetMeanStdDev()
      { return Grid::GetMeanStdDev<T>(); }

      /* Computes the L-N norm of the grid */
      T Norm(UINT N);

      /* Arithmetic operations */
      TGrid& operator -= (CONST TGrid& G);
      TGrid& operator -= (T x);
      TGrid operator - (CONST TGrid& G);
      TGrid operator - (T x);
      TGrid operator - ();

      TGrid operator + (CONST TGrid& G);
      TGrid operator + (T x);
      TGrid& operator += (CONST TGrid& G);
      TGrid& operator += (T x);

      TGrid operator * (CONST TGrid& G);
      TGrid operator * (T x);
      TGrid& operator *= (CONST TGrid& G);
      TGrid& operator *= (T x);

      TGrid operator / (CONST TGrid& G);
      TGrid operator / (T x);
      TGrid& operator /= (CONST TGrid& G);
      TGrid& operator /= (T x);

    private:
      /* Private since element type is implied by template type */
      VOID SetElementType(Array<ElementDecl> Decl)
      { Grid::SetElementType(Decl); }

      static Array<ElementDecl> GetTemplateElementType();

    };

    typedef TGrid<FLOAT> GridF;
    typedef TGrid<DOUBLE> GridD;
    typedef TGrid<Math::Vector3<FLOAT> > Grid3F;

    #include "GridImpl.hpp"

  }
}
