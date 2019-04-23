/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Debug/Assert.cpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1641 $
 * Last Updated: $Date: 2008-10-21 06:46:27 -0700 (Tue, 21 Oct 2008) $
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
#include "Debug/Assert.hpp"
#include "Debug/Trace.hpp"
#include "Debug/Debug.hpp"
#include "IO/StdIO.hpp"
#include "IO/Path.hpp"

#include <cstdlib>
#include <cstdio>

using namespace Bolitho;
using namespace Bolitho::IO;

#if defined(PLATFORM_WINDOWS) 
int Bolitho::AssertFail(LPCTSTR StrExpr, LPCTSTR Filename, INT LineNumber)
{
  DebugBreak();
  return 1;
}
#elif defined(PLATFORM_POSIX)
#include <assert.h>
int Bolitho::AssertFail(LPCTSTR StrExpr, LPCTSTR Filename, INT LineNumber)
{
  StdErr.WriteLine("Assertion Failed: The expression '%s' is FALSE\nSource File: %s\nLine Number: %d\n", StrExpr, Filename, LineNumber);
  Debug::StackFrame Frames[32];
  SIZE_T StackFrameCount = Debug::GenerateStackTrace(Frames, 32);

  StdErr.WriteLine(TEXT("\nStack Trace:"));
  
  for(SIZE_T i=0; i<StackFrameCount; i++)
  {
    if (Frames[i].Symbol().SourceFile() != 0)
      StdErr.WriteLine(TEXT("%p %s()\r\n  at %s:%d"), Frames[i].InstructionPtr(), Frames[i].Symbol().Name(), Path::Filename(Frames[i].Symbol().SourceFile()).ConstStr(), Frames[i].Symbol().SourceLine());
    else if (Frames[i].Symbol().Name() != 0)
      StdErr.WriteLine(TEXT("%p %s()"), Frames[i].InstructionPtr(), Frames[i].Symbol().Name());
		else
      StdErr.WriteLine(TEXT("%p"), Frames[i].InstructionPtr());
  }
  
  StdErr.WriteLine(TEXT("\e[m"));


  assert(false);
  throw InvalidOperationException(0,String::Format(TEXT("ASSERT FAILED: \"%s\" at %s:%d"), StrExpr, Filename, LineNumber));
}
#endif

int Bolitho::VerifyFail(LPCTSTR StrExpr, LPCTSTR Filename, INT LineNumber)
{
  throw InvalidOperationException(0,String::Format(TEXT("VERIFY FAILED: \"%s\" at %s:%d"), StrExpr, Filename, LineNumber));
}
