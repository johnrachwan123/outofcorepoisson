/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/IO/FileEnumerator.hpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1616 $
 * Last Updated: $Date: 2008-09-13 12:11:24 -0700 (Sat, 13 Sep 2008) $
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
#include "Text/RegularExpression.hpp"
#include "Collections/Stack.hpp"

#if defined(PLATFORM_WINDOWS)

namespace Bolitho
{
  namespace IO
  {
    struct FileEnumeratorFlags
    { enum { RECURSIVE = 1, HIDDEN = 2, FOLLOWJUNCTIONS = 4, FOLLOWSYMLINKS = 8, DEFAULT = 0 }; };

    /* Represents an object that can enumerate entries in a filesystem */
    class LIBRARY_API FileEnumerator : public Object
    {
      DeclareRuntimeType();
    public:
      /* Constructs a new FileEnumerator with a given path and search pattern */ 
      FileEnumerator(DWORD Flags = FileEnumeratorFlags::DEFAULT);
      virtual ~FileEnumerator();
      
      VOID Enumerate(LPCTSTR Path);
      VOID EnumerateRecursive(LPCTSTR Path);

      /* Adds an inclusion rule */
      VOID AddIncludeRule(LPCTSTR FileSpecification);
      /* Adds an exclusion rule */
      VOID AddExcludeRule(LPCTSTR FileSpecification);

      /* Returns TRUE if there are more file system entries that match */
      BOOL FileMatches() CONST;
      /* Returns the current element */ 
      CONST FileInfo& CurrentFile() CONST;
      /* Moves to the next element */
      VOID NextMatch();
      
      CONST FileInfo* operator-> () CONST
      { return &m_CurrentFile; }
      operator BOOL() CONST
      { return FileMatches(); }
      
      VOID operator++(int)
      { NextMatch(); }
      
      /* Takes a path and file pattern and returns an array of all filenames that match the search */
      STATIC Array<String> ExpandFilePattern(LPCTSTR FilePattern, BOOL Recursive = FALSE, LPCTSTR Directory = TEXT("."));
    private:
      struct MatchRule
      {
        MatchRule() : SearchPattern(".*") {}
        MatchRule(RegularExpression& Pattern, DWORD Type) : SearchPattern(Pattern), RuleType(Type) {}
        MatchRule(CONST MatchRule& R) : SearchPattern(R.SearchPattern), RuleType(R.RuleType) {}

        RegularExpression SearchPattern;
        DWORD RuleType;
      };
      struct MatchType { enum { INCLUDE, EXCLUDE }; };

      Array<MatchRule> m_Rules;
      FileInfo m_CurrentFile;
      Stack<HANDLE> m_hFind;
      Stack<String> m_Path;
      BOOL m_Done;
      DWORD m_Flags;
    };
  }
}

#endif
