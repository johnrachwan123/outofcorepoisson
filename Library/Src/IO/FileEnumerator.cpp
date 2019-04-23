/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/IO/FileEnumerator.cpp $
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

#include "IO/File.hpp"
#include "IO/Path.hpp"
#include "IO/FileEnumerator.hpp"
#include "Utility/Flags.hpp"

using namespace Bolitho;
using namespace Bolitho::IO;

//------------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::IO,FileEnumerator,Object);
//------------------------------------------------------------------------------------------------------------------------------------------------------
FileEnumerator::FileEnumerator(DWORD Flags) 
{
  m_Flags = Flags;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID FileEnumerator::Enumerate(LPCTSTR RootPath)
{
  String Path = Path::FullPath(RootPath); 
  if (Path.EndsWith(TEXT("\\")))
    Path = Path.Left(Path.Length()-1);
  m_Path.Push(Path);
  m_hFind.Push(NULL);
  m_Done = FALSE;
  m_Flags &= ~FileEnumeratorFlags::RECURSIVE;
  NextMatch();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID FileEnumerator::EnumerateRecursive(LPCTSTR RootPath)
{
  String Path = Path::FullPath(RootPath); 
  if (Path.EndsWith(TEXT("\\")))
    Path = Path.Left(Path.Length()-1);
  m_Path.Push(Path);
  m_hFind.Push(NULL);
  m_Done = FALSE;
  m_Flags |= FileEnumeratorFlags::RECURSIVE;
  NextMatch();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
FileEnumerator::~FileEnumerator()
{
  while (!m_hFind.IsEmpty())
    FindClose(m_hFind.Pop());
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL FileEnumerator::FileMatches() CONST
{
  return !m_Done;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
CONST FileInfo& FileEnumerator::CurrentFile() CONST
{
  return m_CurrentFile;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID FileEnumerator::NextMatch()
{
  if (m_Done)
    return;

  HANDLE hFind = m_hFind.Peek();
  String Path = m_Path.Peek();
  WIN32_FIND_DATA FindData;

  ZeroMemory(&FindData, sizeof(FindData));

  do
  {
    hFind = m_hFind.Peek();
    Path = m_Path.Peek();

    if (hFind == NULL)
    {
      hFind = FindFirstFile(Path + TEXT("\\*.*") , &FindData);
      if (hFind == INVALID_HANDLE_VALUE)
      {
        DWORD Error = GetLastError();
        if (Error == ERROR_FILE_NOT_FOUND || Error == ERROR_PATH_NOT_FOUND)
        {
          m_hFind.Pop();
          m_Path.Pop();

          FindClose(hFind);

          if (m_hFind.IsEmpty())
          {
            m_Done = TRUE;
            return;
          }
          hFind = m_hFind.Peek();
          Path = m_Path.Peek();

          continue;
        }
        SystemException::ThrowError(this, Error);
        m_Done = TRUE;
      }

      m_hFind.Pop();
      m_hFind.Push(hFind);
    }
    else
    {
      if (FindNextFile(hFind, &FindData) == 0)
      {
        DWORD Error = GetLastError();
        if (Error == ERROR_NO_MORE_FILES)
        {
          m_hFind.Pop();
          m_Path.Pop();

          FindClose(hFind);
          if (m_hFind.IsEmpty())
          {
            m_Done = TRUE;
            return;
          }
          hFind = m_hFind.Peek();
          Path = m_Path.Peek();

          continue;
        }
        SystemException::ThrowError(this, Error);
      }
    }

    if (FindData.cFileName[0] == '.')
      if (FindData.cFileName[1] == 0 || FindData.cFileName[1] == '.')
        continue;

    if (FindData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN && !(m_Flags & FileEnumeratorFlags::HIDDEN))
      continue;

    BOOL Matches = FALSE;

    if (m_Rules.IsEmpty())
      Matches = TRUE;
    else
    {
      String FullPath = Path + Path::Separator + FindData.cFileName;
      for (SIZE_T i=0; i<m_Rules.Length(); i++)
        if (m_Rules[i].SearchPattern.Match(FullPath) > 0)
          Matches = (m_Rules[i].RuleType == MatchType::INCLUDE);
    }

    if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (m_Flags & FileEnumeratorFlags::RECURSIVE))
    {
      m_hFind.Push(NULL);
      m_Path.Push(Path + TEXT("\\") + FindData.cFileName);
    }

    if (Matches > 0)
      break;
  }
  while(TRUE);

  m_CurrentFile.Directory = Path;
  m_CurrentFile.Filename = FindData.cFileName;

  m_CurrentFile.AccessedOn = DateTime(FindData.ftLastAccessTime);
  m_CurrentFile.CreatedOn = DateTime(FindData.ftCreationTime);
  m_CurrentFile.ModifiedOn = DateTime(FindData.ftLastWriteTime);

  m_CurrentFile.Attributes = FindData.dwFileAttributes;
  m_CurrentFile.Length = MAKEULONGLONG(FindData.nFileSizeLow,FindData.nFileSizeHigh);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Array<String> FileEnumerator::ExpandFilePattern(LPCTSTR FilePattern, BOOL Recursive, LPCTSTR Directory)
{
  Array<String> Filenames;
  FileEnumerator E;

  if (Recursive)
    E.EnumerateRecursive(Directory);
  else
    E.Enumerate(Directory);

  while (E.FileMatches())
  {
    if (!E.CurrentFile().IsDirectory())
      Filenames.Add(E.CurrentFile().Directory + Path::Separator + E.CurrentFile().Filename);
    E.NextMatch();
  }

  return Filenames;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID FileEnumerator::AddIncludeRule(LPCTSTR FileSpecification)
{
  m_Rules.Add(MatchRule(RegularExpression::FromFileSpec(FileSpecification), MatchType::INCLUDE));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID FileEnumerator::AddExcludeRule(LPCTSTR FileSpecification)
{
  m_Rules.Add(MatchRule(RegularExpression::FromFileSpec(FileSpecification), MatchType::EXCLUDE));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
