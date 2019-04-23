/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/IO/Path.cpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1637 $
 * Last Updated: $Date: 2008-10-15 10:42:55 -0700 (Wed, 15 Oct 2008) $
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

#include "IO/Path.hpp"
#include "Utility/Predicate.hpp"

using namespace Bolitho::IO;
using namespace Bolitho;


#if defined(PLATFORM_WINDOWS)
TCHAR Path::Separator = TEXT('\\');
#elif defined(PLATFORM_POSIX)
TCHAR Path::Separator = TEXT('/');
#endif

//-----------------------------------------------------------------------------------------------------------------------------------------------------
Path::Path()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Path::Append(CONST String& Path, LPCTSTR S)
{
  if (Path.IsEmpty())
    return S;
  
  String NewPath = String(Path);
  if (Path(Path.Length()-1) == Separator)
    NewPath.Append(S);
  else
  {
    NewPath += Separator;
    NewPath += String(S);
  }
  return NewPath;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Array<String> Path::Directories(CONST String& Path)
{
  return Directory(Path).Split(Equal<TCHAR>(Separator));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Path::Filename(CONST String& Path)
{
  String::Iterator i = Path.FindLast(Separator);
  if (i == Path.End())
    return Path;
  return Path.Substr(i+1, Path.End());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Path::Extension(CONST String& Path)
{
  String::Iterator i = Path.FindLast(TEXT("."));
  if (i == Path.End())
    return String::Empty;
  return Path.Substr(i+1, Path.End());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Path::ExtensionMatches(LPCTSTR Path, LPCTSTR Ext)
{
  return Extension(Path).Compare(Ext,FALSE) == 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Path::Basename(CONST String& Path)
{
  String::Iterator i = Path.FindLast(TEXT("."));
  return Path.Substr(Path.Begin(), i);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Path::Relative(LPCTSTR BaseFilename, LPCTSTR FullFilename)
{
  String Base = Path::Directory(BaseFilename);

  SIZE_T BaseLength = CharTraits<TCHAR>::Length(Base);
  SIZE_T FullFilenameLength = CharTraits<TCHAR>::Length(FullFilename);

  SIZE_T MinLength = Min(BaseLength, FullFilenameLength);

  SIZE_T i = 0;
  for (i=0; i<MinLength; i++)
    if (Base[i] != FullFilename[i])
      break;

  return String(FullFilename + i);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_WINDOWS)
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Path::Root(CONST String& Path)
{
  String::Iterator i = Path.Find(TEXT(":\\"));
  if (i != Path.End())
    return Path.Substr(Path.Begin(), i+2);
  return String::Empty;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Path::Directory(CONST String& Path)
{
  String::Iterator i = Path.FindLast(Separator);
  if (i == Path.End())
    return String::Empty;
  return Path.Substr(Path.Begin(), i+1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Path::FullPath(LPCTSTR Path)
{
  DWORD Length = GetFullPathName(Path, 0, 0, 0);
  String S(Length-1);
  GetFullPathName(Path, Length, S.Str(), 0);
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Path::GetCurrentDirectory()
{
  DWORD N = ::GetCurrentDirectory(0, 0);
  String S(N-1);
  ::GetCurrentDirectory(N, &S(0));
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Path::GetApplicationDirectory()
{
  return "";
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Path::GetTemporaryDirectory()
{
  DWORD N = ::GetTempPath(0,0);
  String S(N);
  GetTempPath(N, &S(0));
  return S;
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_POSIX)
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Path::Root(CONST String& Path)
{
  return "/";
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Path::Directory(CONST String& Path)
{
  String::Iterator i = Path.FindLast(Separator);
  if (i == Path.End())
    return String::Empty;
  return Path.Substr(Path.Begin(), i+1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Path::FullPath(LPCTSTR Path)
{
	CHAR Str[PATH_MAX];
	realpath(Path, &Str[0]);
  return String(&Str[0]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Path::GetCurrentDirectory()
{
	CHAR Str[PATH_MAX];
	getcwd(&Str[0], PATH_MAX);
	return String(&Str[0]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Path::GetApplicationDirectory()
{
	return String::Empty;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Path::GetTemporaryDirectory()
{
  return String(TEXT("/tmp"));
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------


