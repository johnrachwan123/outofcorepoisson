/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Text/RegularExpression.hpp $
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

#if defined(USE_REGEX)
#include "Utility/Pair.hpp"
#include "Greta/regexpr2.h"

namespace Bolitho
{
  struct RegularExpressionFlags
  { 
    enum 
    {
      DEFAULT       = 0x0000, // default
      NOCASE        = 0x0001, // ignore case
      GLOBAL        = 0x0002, // match everywhere in the string
      MULTILINE     = 0x0004, // ^ and $ can match internal line breaks
      SINGLELINE    = 0x0008, // . can match newline character
      RIGHTMOST     = 0x0010, // start matching at the right of the string
      NOBACKREFS    = 0x0020, // only meaningful when used with GLOBAL and substitute
      FIRSTBACKREFS = 0x0040, // only meaningful when used with GLOBAL
      ALLBACKREFS   = 0x0080, // only meaningful when used with GLOBAL
      NORMALIZE     = 0x0100, // Preprocess patterns: "\\n" => "\n", etc.
      EXTENDED      = 0x0200  // ignore whitespace in pattern
    };
  };

  class LIBRARY_API RegularExpression : public Object
  {
    DeclareRuntimeType();
  public:

    RegularExpression(LPCTSTR Pattern, DWORD CompileOptions = RegularExpressionFlags::DEFAULT);
    RegularExpression(CONST RegularExpression& RE);
    virtual ~RegularExpression();

    SIZE_T Match(LPCTSTR String, SIZE_T Offset = 0) CONST;
    SIZE_T Match(LPCTSTR String, Pair<SIZE_T,SIZE_T>& Match, SIZE_T Offset = 0) CONST;
    SIZE_T Match(LPCTSTR String, Pair<String::Iterator,String::Iterator>& Match, SIZE_T Offset = 0) CONST;

    static RegularExpression FromFileSpec(LPCTSTR FileSpec, DWORD CompileOptions = RegularExpressionFlags::DEFAULT);
  private:
		regex::rpattern_c m_RegEx;


  };

  class LIBRARY_API RegularExpressionException : public Exception
  {
    DeclareRuntimeType();
  public:
    RegularExpressionException(CONST Object* Sender, CONST TCHAR* Description) : Exception(Sender)
    {
      m_Description = CharTraits<TCHAR>::Duplicate(Description);
    }
    
    RegularExpressionException(CONST RegularExpressionException& E) :
      Exception(E)
    {
      m_Description = CharTraits<TCHAR>::Duplicate(E.m_Description);
    }

    virtual String Description() CONST;

    virtual ~RegularExpressionException()
    {
      if (m_Description)
        free(m_Description);
    }

  private:
    TCHAR* m_Description;
  };

}

#endif
