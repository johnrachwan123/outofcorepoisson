/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/IO/MemoryMappedFile.cpp $
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




#include "CodeLibrary.hpp"

#include "IO/MemoryMappedFile.hpp"

using namespace Bolitho;
using namespace Bolitho::IO;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::IO,MemoryMappedFile,Object);
ImplementRuntimeType(Bolitho::IO,MemoryMappedView,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
MemoryMappedView::MemoryMappedView(LPVOID pData, ULONGLONG Offset, SIZE_T Length)
{
  m_pData = pData;
	m_Offset = Offset;
	m_Length = Length;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
MemoryMappedFile::MemoryMappedFile()
{
	m_Length = 0;
	m_Access = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
MemoryMappedFile::~MemoryMappedFile()
{
  Close();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG MemoryMappedFile::Length() CONST
{
  return m_Length;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_WINDOWS)
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<MemoryMappedFile> MemoryMappedFile::OpenAnonymous(ULONGLONG Size, DWORD Access, DWORD Flags)
{
  MemoryMappedFile* pMapping = new MemoryMappedFile();

	if (Size == 0)
		throw InvalidArgumentException(0, TEXT("The size of an anonymous map must be non-zero"));
  
  pMapping->m_Flags = Flags;
  pMapping->m_Access = Access;	   
  pMapping->m_Length = Size;
  pMapping->m_hMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, Access | SEC_COMMIT | Flags, (DWORD)(Size >> 32), (DWORD)(Size & 0xffffffff), 0);

  if (!pMapping->m_hMapping)
    SystemException::ThrowError();

  return pMapping;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<MemoryMappedFile> MemoryMappedFile::Open(Ptr<File> pFile, DWORD Access, DWORD Flags)
{
  MemoryMappedFile* pMapping = new MemoryMappedFile();
  ULONGLONG Size = pFile->Length();
	if (Size == 0)
		Size = 1;

  pMapping->m_Flags = Flags;
  pMapping->m_Access = Access;
	pMapping->m_pFile = pFile;
  pMapping->m_Length = Size;

  pMapping->m_hMapping = CreateFileMapping(pFile->Handle(), NULL, Access | SEC_COMMIT | Flags, (DWORD)(Size >> 32), (DWORD)(Size & 0xffffffff), 0);

  if (!pMapping->m_hMapping)
    SystemException::ThrowError();


  return pMapping;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<MemoryMappedFile> MemoryMappedFile::Open(LPCTSTR Filename, DWORD FileMode, DWORD Access, DWORD Flags)
{
  Ptr<File> F = File::Open(Filename, FileMode);
	return Open(F, Access, Flags);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MemoryMappedFile::Close()
{
  if (m_hMapping)
    CloseHandle(m_hMapping);
	m_hMapping = 0;
	m_pFile = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG MemoryMappedFile::Resize(ULONGLONG Size)
{
	if (m_pFile == 0)
		throw InvalidOperationException(this, TEXT("Anonymous memory mapped files cannot be resized"));

	if (m_hMapping)
    CloseHandle(m_hMapping);

  if (Size == 0)
    Size = 1;
  
  m_Length = Size;
	m_pFile->Resize(Size);
  m_hMapping = CreateFileMapping(m_pFile->Handle(), NULL, m_Access | SEC_COMMIT | m_Flags, (DWORD)(Size >> 32), (DWORD)(Size & 0xffffffff), 0);
  if (!m_hMapping)
    SystemException::ThrowError(this);
  
  return Size;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<MemoryMappedView> MemoryMappedFile::CreateView()
{
  #if defined(PLATFORM_32BIT)
	if (m_Length > MAXLONG)
		throw OutOfMemoryException(this, TEXT("The mapping is larger than the system supports"));
  #endif

  DWORD MapAccess = 0;
  if (m_Access == PageAccess::READ)
    MapAccess = FILE_MAP_READ;
  else if (m_Access == PageAccess::READWRITE)
    MapAccess = FILE_MAP_WRITE;

	LPVOID pData = MapViewOfFile(m_hMapping, MapAccess, 0, 0, 0);
	if (pData == 0)
		SystemException::ThrowError(this);

  MEMORY_BASIC_INFORMATION Info;
  VirtualQuery(pData, &Info, sizeof(Info));
  
	return new MemoryMappedView(pData, 0, Info.RegionSize);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<MemoryMappedView> MemoryMappedFile::CreateView(ULONGLONG Offset, SIZE_T Length)
{
  DWORD MapAccess = 0;
  if (m_Access == PageAccess::READ)
    MapAccess = FILE_MAP_READ;
  else if (m_Access == PageAccess::READWRITE)
    MapAccess = FILE_MAP_WRITE;

  LPVOID pData = MapViewOfFile(m_hMapping, MapAccess, (DWORD)(Offset >> 32), (DWORD)(Offset & 0xffffffff), Length);
	if (pData == 0)
		SystemException::ThrowError(this);

  MEMORY_BASIC_INFORMATION Info;
  VirtualQuery(pData, &Info, sizeof(Info));

  return new MemoryMappedView(pData, Offset, Info.RegionSize);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<MemoryMappedView> MemoryMappedFile::CreateView(DWORD Access)
{
#if defined(PLATFORM_32BIT)
  if (m_Length > MAXLONG)
    throw OutOfMemoryException(this, TEXT("The mapping is larger than the system supports"));
#endif
  
  LPVOID pData = MapViewOfFile(m_hMapping, Access, 0, 0, 0);
  if (pData == 0)
    SystemException::ThrowError(this);
  
  MEMORY_BASIC_INFORMATION Info;
  VirtualQuery(pData, &Info, sizeof(Info));
  
  return new MemoryMappedView(pData, 0, Info.RegionSize);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<MemoryMappedView> MemoryMappedFile::CreateView(ULONGLONG Offset, SIZE_T Length, DWORD Access)
{
  LPVOID pData = MapViewOfFile(m_hMapping, Access, (DWORD)(Offset >> 32), (DWORD)(Offset & 0xffffffff), Length);
  if (pData == 0)
    SystemException::ThrowError(this);
  
  MEMORY_BASIC_INFORMATION Info;
  VirtualQuery(pData, &Info, sizeof(Info));
  
  return new MemoryMappedView(pData, Offset, Info.RegionSize);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
MemoryMappedView::~MemoryMappedView()
{
  if (m_pData)
    UnmapViewOfFile(m_pData);
  m_pData = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MemoryMappedView::Flush()
{
  Flush(0, m_Length);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MemoryMappedView::Flush(SIZE_T Offset, SIZE_T Length)
{
  FlushViewOfFile((LPBYTE)m_pData+Offset, Length);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MemoryMappedView::Lock(SIZE_T Offset, SIZE_T Length) CONST
{
  if (!VirtualLock((LPBYTE)m_pData+Offset,Length))
    SystemException::ThrowError(this);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MemoryMappedView::Unlock(SIZE_T Offset, SIZE_T Length) CONST
{
  VirtualUnlock((LPBYTE)m_pData+Offset,Length);
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_POSIX)
#if defined(PLATFORM_FREEBSD)
#define MAP_ANONYMOUS MAP_ANON
#endif

//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<MemoryMappedFile> MemoryMappedFile::OpenAnonymous(ULONGLONG Size, DWORD Access, DWORD Flags)
{
  MemoryMappedFile* pMap = new MemoryMappedFile();
  
  pMap->m_Access = Access;
  pMap->m_Length = Size;
  pMap->m_Flags = Flags;
  
  return pMap;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<MemoryMappedFile> MemoryMappedFile::Open(Ptr<File> pFile, DWORD Access, DWORD Flags)
{
  MemoryMappedFile* pMap = new MemoryMappedFile();
  
  pMap->m_pFile = pFile;
  pMap->m_Access = Access;
  pMap->m_Flags = Flags;
  pMap->m_Length = pFile->Length();
  
  return pMap;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<MemoryMappedFile> MemoryMappedFile::Open(LPCTSTR Filename, DWORD Mode, DWORD Access, DWORD Flags)
{
  MemoryMappedFile* pMap = new MemoryMappedFile();
  
  pMap->m_pFile = File::Open(Filename, Mode, (Access & PageAccess::READWRITE) ? FileAccess::READWRITE : FileAccess::READ);
  pMap->m_Access = Access;
  pMap->m_Flags = Flags;
  pMap->m_Length = pMap->m_pFile->Length();
  
  return pMap;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MemoryMappedFile::Close()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG MemoryMappedFile::Resize(ULONGLONG Size)
{
  return Size;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<MemoryMappedView> MemoryMappedFile::CreateView()
{
  DWORD Flags = MAP_SHARED;
  FILEHANDLE File = -1;
  
  if (m_Access & PageAccess::WRITECOPY)
    Flags = MAP_PRIVATE;
  
  if (m_pFile == 0)
    Flags |= MAP_ANONYMOUS;
  else
    File = m_pFile->Handle();
  
  LPVOID pData = mmap(0, m_Length, m_Access, Flags, File, 0);
  
  if (pData == MAP_FAILED)
    SystemException::ThrowError(this);
  
  return new MemoryMappedView(pData, 0, m_Length);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<MemoryMappedView> MemoryMappedFile::CreateView(ULONGLONG Offset, SIZE_T Length)
{
  DWORD Flags = MAP_SHARED;
  FILEHANDLE File = -1;
  
  if (m_Access & PageAccess::WRITECOPY)
    Flags = MAP_PRIVATE;
  
  if (m_pFile == 0)
    Flags |= MAP_ANONYMOUS;
  else
    File = m_pFile->Handle();
  
  LPVOID pData = mmap(0, Length, m_Access, Flags, File, Offset);
  
  if (pData == MAP_FAILED)
    SystemException::ThrowError(this);
  
  return new MemoryMappedView(pData, Offset, Length);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<MemoryMappedView> MemoryMappedFile::CreateView(DWORD Access)
{
  DWORD Flags = MAP_SHARED;
  FILEHANDLE File = -1;
  
  if (Access & PageAccess::WRITECOPY)
    Flags = MAP_PRIVATE;
  
  if (m_pFile == 0)
    Flags |= MAP_ANONYMOUS;
  else
    File = m_pFile->Handle();
  
  LPVOID pData = mmap(0, m_Length, Access, Flags, File, 0);
  
  if (pData == MAP_FAILED)
    SystemException::ThrowError(this);
  
  return new MemoryMappedView(pData, 0, m_Length);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<MemoryMappedView> MemoryMappedFile::CreateView(ULONGLONG Offset, SIZE_T Length, DWORD Access)
{
  DWORD Flags = MAP_SHARED;
  FILEHANDLE File = -1;
  
  if (Access & PageAccess::WRITECOPY)
    Flags = MAP_PRIVATE;
  
  if (m_pFile == 0)
    Flags |= MAP_ANONYMOUS;
  else
    File = m_pFile->Handle();
  
  LPVOID pData = mmap(0, Length, Access, Flags, File, Offset);
  
  if (pData == MAP_FAILED)
    SystemException::ThrowError(this);
  
  return new MemoryMappedView(pData, Offset, Length);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
MemoryMappedView::~MemoryMappedView()
{
  munmap(m_pData, m_Length);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MemoryMappedView::Flush(SIZE_T Offset, SIZE_T Length)
{
  if (msync((LPBYTE)m_pData + Offset, Length, MS_SYNC | MS_INVALIDATE))
    SystemException::ThrowError(this);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MemoryMappedView::Lock(SIZE_T Offset, SIZE_T Length) CONST
{
  if (mlock((LPBYTE)m_pData + Offset, Length))
    SystemException::ThrowError(this);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MemoryMappedView::Unlock(SIZE_T Offset, SIZE_T Length) CONST
{
  if (munlock((LPBYTE)m_pData + Offset, Length))
    SystemException::ThrowError(this);
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------

