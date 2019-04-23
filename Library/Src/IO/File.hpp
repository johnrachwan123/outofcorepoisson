/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/IO/File.hpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 1724 $
 * Last Updated: $Date: 2008-12-12 08:50:05 -0800 (Fri, 12 Dec 2008) $
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

#include "IO/IOException.hpp"
#include "Time/DateTime.hpp"
#include "IO/Path.hpp"
#include "Memory/Md5.hpp"
#include "Memory/VirtualMemory.hpp"

namespace Bolitho
{
  namespace IO
  {
    class LIBRARY_API FileException : public Exception
    {
      DeclareRuntimeType();
    public:
      FileException(CONST Object* pSource, CONST Exception* pInnerException) : Exception(pSource, pInnerException)
      {}
      FileException(CONST Object* pSource, LPCTSTR Description) : Exception(pSource)
      {
        m_Description = CharTraits<TCHAR>::Duplicate(Description);
      }

      virtual String ToString() CONST
      { return m_Description; }
    private:
      String m_Description;
    };

    #if defined(PLATFORM_WINDOWS)
    struct FileAttribute
    { enum { NORMAL = FILE_ATTRIBUTE_NORMAL, DIRECTORY = FILE_ATTRIBUTE_DIRECTORY, ARCHIVE = FILE_ATTRIBUTE_ARCHIVE, READONLY = FILE_ATTRIBUTE_READONLY, SYSTEM = FILE_ATTRIBUTE_SYSTEM, HIDDEN = FILE_ATTRIBUTE_HIDDEN, COMPRESSED = FILE_ATTRIBUTE_COMPRESSED, OFFLINE = FILE_ATTRIBUTE_OFFLINE, SPARSE = FILE_ATTRIBUTE_SPARSE_FILE, ENCRYPTED = FILE_ATTRIBUTE_ENCRYPTED, TEMPORARY = FILE_ATTRIBUTE_TEMPORARY, REPARSEPOINT = FILE_ATTRIBUTE_REPARSE_POINT }; };
    
    #undef DELETE
    struct FileShare
    { enum { NONE = 0, READ = FILE_SHARE_READ, WRITE = FILE_SHARE_WRITE, READWRITE = READ | WRITE, DELETE = FILE_SHARE_DELETE }; };
    
    struct FileAccess
    { enum { READ = GENERIC_READ, WRITE = GENERIC_WRITE, READWRITE = GENERIC_READ | GENERIC_WRITE }; };
    
    struct FileFlags
    { enum { DELETEONCLOSE = FILE_FLAG_DELETE_ON_CLOSE, WRITETHROUGH = FILE_FLAG_WRITE_THROUGH, NOBUFFERING = FILE_FLAG_NO_BUFFERING, SEQUENTIAL = FILE_FLAG_SEQUENTIAL_SCAN, RANDOM = FILE_FLAG_RANDOM_ACCESS }; };
    
    struct FileMode
    { enum { CREATE = CREATE_ALWAYS, CREATENEW = CREATE_NEW, OPEN = OPEN_EXISTING, OPENORCREATE = OPEN_ALWAYS, TRUNCATE = TRUNCATE_EXISTING, APPEND = 6 }; };
    #endif

    #if defined(PLATFORM_MACOS) || defined(PLATFORM_FREEBSD)
    #define O_DSYNC O_FSYNC
    #endif    
    
    #if defined(PLATFORM_POSIX)
    struct FileAttribute
    { enum { NORMAL = S_IFREG, DIRECTORY = S_IFDIR, ARCHIVE = 0, READONLY = 0, SYSTEM = 0, HIDDEN = 0, COMPRESSED = 0, OFFLINE = 0, SPARSE = 0, ENCRYPTED = 0, TEMPORARY = 0, REPARSEPOINT = 0 }; };
    
    struct FileShare 
    { enum { NONE = 0, READ = 0, WRITE = 0, READWRITE = READ | WRITE, DELETE = 0 }; };
    
    struct FileAccess
    { enum { READ = O_RDONLY, WRITE = O_WRONLY, READWRITE = O_RDWR }; };
    
    struct FileFlags
    { enum { DELETEONCLOSE = 0x8000000, WRITETHROUGH = O_DSYNC, NOBUFFERING = 0, SEQUENTIAL = 0, RANDOM = 0 }; };
    
    struct FileMode
    { enum { CREATE = O_CREAT, CREATENEW = O_CREAT | O_EXCL , OPEN = 0, OPENORCREATE = O_CREAT, TRUNCATE = O_TRUNC, APPEND = O_APPEND | O_CREAT }; };
    #endif


    class File;

    /* Represents a memory-mapping of a file */
    class LIBRARY_API FileView : public Object
    {
      DeclareRuntimeType();

      friend class File;

    public:
      virtual ~FileView();

      /* The base address of the mapped region */
      LPVOID GetBaseAddress()
      { return m_pBaseAddress; }
      LPCVOID GetBaseAddress() CONST
      { return m_pBaseAddress; }

      /* The length of the mapped region */
      SIZE_T Length() CONST
      { return m_Length; }

      /* Lock/Unlock a region of the file into memory */
      VOID Lock(SIZE_T Offset, SIZE_T Length) CONST;
      VOID Unlock(SIZE_T Offset, SIZE_T Length) CONST;

      /* Flushes a porition of the view to disk */
      VOID Flush();
      VOID Flush(SIZE_T Offset, SIZE_T Length);

      /* Returns the page access used to map the view */
      DWORD GetPageAccess() CONST
      { return m_PageAccess; }
    private:
      FileView(Ptr<File> pFile, ULONGLONG Offset, SIZE_T Length, DWORD Access);
      
      LPVOID m_pBaseAddress;
      SIZE_T m_Length;
      DWORD m_PageAccess;

      MEMORYMAPPEDFILEHANDLE m_hMapping;
      Ptr<File> m_pFile;
    };

    /* Represents a directory */
    class LIBRARY_API Directory
    {
    public:
      static BOOL Create(LPCTSTR Name);
      static BOOL Delete(LPCTSTR Name);
      static BOOL DeleteRecursive(LPCTSTR Name);
    };

    /* Represents a file */
    class LIBRARY_API File : public Stream
    {
      DeclareRuntimeType();
      
      friend class MemoryMappedFile;
      friend class MemoryMappedView;
    public:
      /* Creates a temprary file and returns an open File object representing it */
      static Ptr<File> OpenTemporary();
      
      /* Determines if a file exists */
      static BOOL Exists(LPCTSTR Filename);
      /* Determines if a file is a directory, or other type of file */
      static BOOL IsDirectory(LPCTSTR Filename);
      /* Deletes a file */
      static VOID Delete(LPCTSTR Filename);
      /* Copys a file */
      static VOID Copy(LPCTSTR Source, LPCTSTR Dest);
      /* Copys a portion of a file */
      static VOID Copy(Ptr<File> pSource, ULONGLONG SourceOffset, ULONGLONG Length, Ptr<File> pDestination, ULONGLONG DestOffset);
      /* Moves a file */
      static VOID Move(LPCTSTR Source, LPCTSTR Dest);
      /* Renames a file */
      static VOID Rename(LPCTSTR From, LPCTSTR To)
      { Move(From, To); }
      /* Creates a soft/hard link to a file */
      static VOID Link(LPCTSTR Name, LPCTSTR Dest, BOOL HardLink = TRUE);
      
      /* Reads an array of N structs from a file */
      template<class T>
      static VOID ReadStruct(LPCTSTR Filename, ULONGLONG Offset, T* pStruct, SIZE_T N = 1)
      {
        Ptr<File> pFile = File::Open(Filename, FileMode::OPEN, FileAccess::READ);
        pFile->Seek(Offset);
        pFile->Read(pStruct, sizeof(T)*N);
      }

      /* Writes an array of N structs to a file */
      template<class T>
      static VOID WriteStruct(LPCTSTR Filename, ULONGLONG Offset, CONST T* pStruct, SIZE_T N = 1)
      {
        Ptr<File> pFile = File::Open(Filename, FileMode::OPENORCREATE, FileAccess::WRITE);
        pFile->Seek(Offset);
        pFile->Write(pStruct, sizeof(T)*N);
      }

      /* Reads and returns all bytes in a file */
      static Buffer<BYTE> ReadAllBytes(LPCTSTR Filename);
      /* Reads and returns an array of text lines in a file */
      static Array<String> ReadAllLines(LPCTSTR Filename);
      /* Reads and returns all text in a file */
      static String ReadAllText(LPCTSTR Filename);

      /* Opens a file */
      STATIC Ptr<File> Open(LPCTSTR Filename, DWORD Mode = FileMode::OPENORCREATE, DWORD Access = FileAccess::READWRITE, DWORD Share = FileShare::READWRITE, DWORD Flags = 0);
      /* Trys to open a file.  On failure, retruns a Ptr<File>::Null instead of throwing an exception */
      STATIC Ptr<File> TryOpen(LPCTSTR Filename, DWORD Mode = FileMode::OPENORCREATE, DWORD Access = FileAccess::READWRITE, DWORD Share = FileShare::READWRITE, DWORD Flags = 0);
      
      virtual ~File();
      
      /* Returns the date/time the file was last modified */
      DateTime LastModifiedOn() CONST;
      /* Returns the date/time the file was last accessed */
      DateTime LastAccessedOn() CONST;
      /* Returns the date/time the file was created */
      DateTime CreatedOn() CONST;
      
      /* Returns the length of the file */
      ULONGLONG Length();
      /* Resizes the file */
      ULONGLONG Resize(ULONGLONG NewLength);
      /* Returns the current file pointer location */
      virtual ULONGLONG Position();
      /* Sets the current file pointer */
      virtual ULONGLONG Seek(ULONGLONG NewPosition);

      virtual BOOL CanRead()
      { return TRUE; }
      virtual BOOL CanWrite()
      { return TRUE; }

      virtual BOOL CanSeek()
      { return TRUE; }

      /* Flushes all unwritten data to disk */
      virtual VOID Flush();
      /* Closes the file */
      virtual VOID Close();

      /* Writes data to the file at the current file pointer location */
      virtual SIZE_T Write(LPCVOID pData, SIZE_T N);
      /* Reads data from the file at the current file pointer location */
      virtual SIZE_T Read(LPVOID pData, SIZE_T N);

      /* Returns TRUE if the file pointer is at or beyond the end of the file */
      virtual BOOL EndOfStream();

      /* Sets the size of the internal read/write buffer */
      VOID SetBufferSize(SIZE_T BufferSize);

      /* Returns the OS handle for the file */
      FILEHANDLE Handle() CONST
      { return m_hFile; }

      /* Maps a view of the file into memory */
      Ptr<FileView> MapView(DWORD PageAccess);
      /* Maps a view of part of the file into memory */
      Ptr<FileView> MapView(ULONGLONG Offset, SIZE_T Length, DWORD PageAccess);

      /* Moves a block of data within the file */
      VOID MoveBlock(ULONGLONG FromOffset, ULONGLONG ToOffset, ULONGLONG Length);

      /* Returns a duplicate file class */
      Ptr<File> Duplicate() CONST;

      /* Closes and deletes the file */
      VOID Delete();

      /* Computes teh Md5 checksum of the file */
      Md5Hash ComputeMd5Hash();
    private:
      File(FILEHANDLE hFile);

      String m_Filename;
      FILEHANDLE m_hFile;

      SIZE_T RawWrite(LPCVOID pData, SIZE_T N);
      SIZE_T RawRead(LPVOID pData, SIZE_T N);

      VOID InternalFlush();
      VOID InternalWriteFlush();

      Buffer<BYTE> m_Buffer;
      SIZE_T m_BufferOffset;
      SIZE_T m_BufferLength;

      ULONGLONG m_CurrentPosition;
      
      struct BufferMode { enum { READ, WRITE, NONE }; };
      DWORD m_BufferMode;
    };

    /* Represents the meta-data associates with a file */
    struct LIBRARY_API FileInfo
    {
      FileInfo();
      FileInfo(LPCTSTR Filename);

      /* The filename */
      String Filename;
      /* The full path */
      String Directory;

      String GetFullPath() CONST
      { return Directory + Path::Separator + Filename; }

      /* The time the file was created */
      DateTime CreatedOn;
      /* The approximate time the file was last changed */
      DateTime ModifiedOn;
      /* The approximate time the file was last opened */
      DateTime AccessedOn;

      /* The length of the file */
      ULONGLONG Length;
      /* The file attributes */
      DWORD Attributes;

      /* Returns TRUE iff the file is a directory */
      INLINE BOOL IsDirectory() CONST
      { return Attributes & FileAttribute::DIRECTORY; }

      Ptr<File> Open(DWORD Mode = FileMode::OPENORCREATE, DWORD Access = FileAccess::READWRITE, DWORD Share = FileShare::READWRITE, DWORD Flags = 0) CONST
      { return File::Open(GetFullPath(), Mode, Access, Share, Flags); }

      Ptr<File> TryOpen(DWORD Mode = FileMode::OPENORCREATE, DWORD Access = FileAccess::READWRITE, DWORD Share = FileShare::READWRITE, DWORD Flags = 0) CONST
      { return File::TryOpen(GetFullPath(), Mode, Access, Share, Flags); }
    };

  }
}
