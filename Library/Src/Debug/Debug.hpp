/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Debug/Debug.hpp $
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

namespace Bolitho
{
  
  class Debug
  {
  public:
    class LIBRARY_API Symbol : public Object
    {
      DeclareRuntimeType();
    public:
      Symbol();
      Symbol(CONST Symbol& S);
      virtual ~Symbol();
      
      Symbol& operator = (CONST Symbol& S);
      
      static Symbol FromAddress(VOID* Address);
      static Symbol FromName(CONST CHAR* SymbolName);
      
      CONST CHAR* Name() CONST
      { return m_Name; }
      VOID SetName(CHAR* S)
      { m_Name = CharTraits<CHAR>::Duplicate(S); }
      
      CONST CHAR* SourceFile() CONST
      { return m_SourceFile; }
      VOID SetSourceFile(CHAR* S)
      { m_SourceFile = CharTraits<CHAR>::Duplicate(S); }
      
      CONST SIZE_T& SourceLine() CONST
      { return m_SourceLine; }
      SIZE_T& SourceLine()
      { return m_SourceLine; }
      
      CONST PTRDIFF_T& SourceLineByteOffset() CONST
      { return m_SourceLineByteOffset; }
      PTRDIFF_T& SourceLineByteOffset()
      { return m_SourceLineByteOffset; }
      
      CONST PVOID& Address() CONST
      { return m_pAddress; }
      PVOID& Address()
      { return m_pAddress; }
      
      CONST PTRDIFF_T& AddressOffset() CONST
      { return m_AddressOffset; }
      PTRDIFF_T& AddressOffset()
      { return m_AddressOffset; }
      
      static String ParseFunctionName(String FunctionSignature);

    private:
      CHAR* m_Name;
      CHAR* m_SourceFile;
      SIZE_T m_SourceLine;
      PTRDIFF_T m_SourceLineByteOffset;
      PVOID m_pAddress;
      PTRDIFF_T m_AddressOffset;
    };
    
    class LIBRARY_API StackFrame : public Object
    {
      DeclareRuntimeType();
    public:
      StackFrame()
      {
        m_pInstruction = 0;
        m_pStack = 0;
      }
      StackFrame(CONST StackFrame& F)
      {
        m_pInstruction = F.m_pInstruction;
        m_pStack = F.m_pStack;
        m_Symbol = F.m_Symbol;
      }
      virtual ~StackFrame()
      {}
      
      VOID*& InstructionPtr()
      { return m_pInstruction; }
      
      VOID*& StackPtr()
      { return m_pStack; }
      
      CONST Debug::Symbol& Symbol() CONST
      { return m_Symbol; }
      Debug::Symbol& Symbol()
      { return m_Symbol; }
      
    private:
      Debug::Symbol m_Symbol;
      LPVOID m_pInstruction;
      LPVOID m_pStack;
    };
    
    static LPVOID AddressFromSymbol(CONST CHAR* Name);
    static String SymbolFromAddress(LPVOID Address);
    
    #if defined(PLATFORM_WINDOWS)
    static SIZE_T GenerateStackTrace(StackFrame* pFrames, SIZE_T MaximumNumerOfFrames, PCONTEXT Context = 0);
    static VOID GenerateCoreDump(LPCTSTR Filename = 0, PEXCEPTION_POINTERS pException = 0, MINIDUMP_TYPE Type = MiniDumpWithFullMemory);
    #endif
    
    #if defined(PLATFORM_POSIX)
    static SIZE_T GenerateStackTrace(StackFrame* pFrames, SIZE_T MaximumNumerOfFrames);
    static VOID GenerateCoreDump(LPCTSTR Filename = 0);
    
  private:
    static VOID ParseStackTraceString(CHAR* String, Symbol& Sym);
    #endif

    
  };



}
