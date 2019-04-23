/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Debug/Debug.cpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1702 $
 * Last Updated: $Date: 2008-11-03 08:09:07 -0800 (Mon, 03 Nov 2008) $
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

#include "Debug/Debug.hpp"

using namespace Bolitho;

#include <stdlib.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho,Debug::StackFrame,Object);
ImplementRuntimeType(Bolitho,Debug::Symbol,Object);
//----------------------------------------------------------------------------------------------------------------------------------------------------
Debug::Symbol::Symbol()
{
  m_Name = 0;
  m_SourceFile = 0;
  m_SourceLine = 0;
  m_SourceLineByteOffset = 0;
  m_pAddress = 0;
  m_AddressOffset = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Debug::Symbol::Symbol(CONST Debug::Symbol& S)
{
  m_Name = CharTraits<CHAR>::Duplicate(S.m_Name);
  m_SourceFile = CharTraits<CHAR>::Duplicate(S.m_SourceFile);
  m_SourceLine = S.m_SourceLine;
  m_SourceLineByteOffset = S.m_SourceLineByteOffset;
  m_pAddress = S.m_pAddress;
  m_AddressOffset = S.m_AddressOffset;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Debug::Symbol& Debug::Symbol::operator = (CONST Debug::Symbol& S)
{
  m_Name = CharTraits<CHAR>::Duplicate(S.m_Name);
  m_SourceFile = CharTraits<CHAR>::Duplicate(S.m_SourceFile);
  m_SourceLine = S.m_SourceLine;
  m_SourceLineByteOffset = S.m_SourceLineByteOffset;
  m_pAddress = S.m_pAddress;
  m_AddressOffset = S.m_AddressOffset;
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Debug::Symbol::~Symbol()
{
  if (m_Name)
  {
    free(m_Name);
    m_Name = 0;
  }
  if (m_SourceFile)
  {
    free(m_SourceFile);
    m_SourceFile = 0;
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_WINDOWS)
VOID* Debug::AddressFromSymbol(CONST CHAR *Name)
{
  SymInitialize(GetCurrentProcess(), 0, TRUE);
  SYMBOL_INFO* pSymInfo = (SYMBOL_INFO*)malloc(sizeof(SYMBOL_INFO)+1024);
  RtlZeroMemory(pSymInfo, sizeof(SYMBOL_INFO)+1024);
  pSymInfo->MaxNameLen = 1024;
  pSymInfo->SizeOfStruct = sizeof(SYMBOL_INFO)+1024;

  SymFromName(GetCurrentProcess(), (LPSTR)Name, pSymInfo);
  SymCleanup(GetCurrentProcess());

  VOID* pAddress = (VOID*)pSymInfo->Address;
  free(pSymInfo);
  return pAddress;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
String Debug::SymbolFromAddress(VOID *Address)
{
  SymInitialize(GetCurrentProcess(), 0, TRUE);
  SYMBOL_INFO* pSymInfo = (SYMBOL_INFO*)malloc(sizeof(SYMBOL_INFO)+1024);
  RtlZeroMemory(pSymInfo, sizeof(SYMBOL_INFO)+1024);
  pSymInfo->MaxNameLen = 1024;
  pSymInfo->SizeOfStruct = sizeof(SYMBOL_INFO)+1024;
  DWORD64 SymDisplacement = 0;

  SymFromAddr(GetCurrentProcess(), (DWORD64)Address, &SymDisplacement, pSymInfo);
  SymCleanup(GetCurrentProcess());

  String S = (pSymInfo->Name != 0) ? pSymInfo->Name : "";
  free(pSymInfo);
  return S;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
#include <intrin.h>
SIZE_T Debug::GenerateStackTrace(StackFrame* pFrames, SIZE_T MaximumNumberOfFrames, PCONTEXT pCtxt)
{
  if (pCtxt == 0)
  {
    pCtxt = new CONTEXT();

    #if PLATFORM_WINDOWS >= PLATFORM_WINDOWS_XP
    RtlCaptureContext(pCtxt);  
    #else
    LPVOID SaveEIp = _ReturnAddress();
    LPVOID SaveEBp;
    LPVOID SaveESp;
    __asm 
    { 
      mov DWORD PTR [SaveEBp], ebp
      mov DWORD PTR [SaveESp], esp
    } 

    pCtxt->Eip = (DWORD)SaveEIp;
    pCtxt->Ebp = (DWORD)SaveEBp;
    pCtxt->Esp = (DWORD)SaveESp;
    #endif
  }

  STACKFRAME64 Frame;
  RtlZeroMemory(&Frame, sizeof(STACKFRAME64));

  SymInitialize(GetCurrentProcess(), 0, TRUE);
  BOOL First = TRUE;

  #if defined(ARCH_AMD64)
  DWORD MachineType = IMAGE_FILE_MACHINE_AMD64;
  Frame.AddrPC.Offset = pCtxt->Rip;
  Frame.AddrPC.Mode = AddrModeFlat;
  Frame.AddrFrame.Offset = pCtxt->Rsp;
  Frame.AddrFrame.Mode = AddrModeFlat;
  Frame.AddrStack.Offset = pCtxt->Rsp;
  Frame.AddrStack.Mode = AddrModeFlat;
  #elif defined(ARCH_I386)
  DWORD MachineType = IMAGE_FILE_MACHINE_I386;
  Frame.AddrPC.Offset = pCtxt->Eip;
  Frame.AddrPC.Mode = AddrModeFlat;
  Frame.AddrFrame.Offset = pCtxt->Ebp;
  Frame.AddrFrame.Mode = AddrModeFlat;
  Frame.AddrStack.Offset = pCtxt->Esp;
  Frame.AddrStack.Mode = AddrModeFlat;
  #else
    #error "Platform not supported"
  #endif

  BOOL Result = StackWalk64(MachineType, ::GetCurrentProcess(), ::GetCurrentThread(), &Frame, pCtxt, 0, &SymFunctionTableAccess64, &SymGetModuleBase64, 0);
  SIZE_T i = 0;
  while(i < MaximumNumberOfFrames && Result && Frame.AddrPC.Offset != 0)
  {
    StackFrame S;

    S.InstructionPtr() = (VOID*)Frame.AddrPC.Offset;
    S.StackPtr() = (VOID*)Frame.AddrStack.Offset;

    SYMBOL_INFO* pSymInfo = (SYMBOL_INFO*)malloc(sizeof(SYMBOL_INFO)+1024);
    RtlZeroMemory(pSymInfo, sizeof(SYMBOL_INFO)+1024);
    pSymInfo->MaxNameLen = 1024;
    pSymInfo->SizeOfStruct = sizeof(SYMBOL_INFO);
    DWORD64 SymDisplacement = 0;

    SymFromAddr(GetCurrentProcess(), Frame.AddrPC.Offset, &SymDisplacement, pSymInfo);

    if (pSymInfo->Name != 0)
    {
      S.Symbol().SetName(CharTraits<CHAR>::Duplicate(pSymInfo->Name));
      S.Symbol().AddressOffset() = (SSIZE_T)SymDisplacement;
      S.Symbol().Address() = (PVOID)pSymInfo->Address;
    }

    IMAGEHLP_LINE64 LineInfo;
    RtlZeroMemory(&LineInfo, sizeof(IMAGEHLP_LINE64));
    LineInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
    DWORD LineDisplacement = 0;

    SymGetLineFromAddr64(GetCurrentProcess(), Frame.AddrPC.Offset, &LineDisplacement, &LineInfo);

    if (LineInfo.FileName != 0)
    {

      S.Symbol().SetSourceFile(CharTraits<CHAR>::Duplicate(LineInfo.FileName));
      S.Symbol().SourceLine() = LineInfo.LineNumber;
      S.Symbol().SourceLineByteOffset() = (BYTE*)S.Symbol().Address() - (BYTE*)LineInfo.Address;
    }

    Result = StackWalk64(MachineType, ::GetCurrentProcess(), ::GetCurrentThread(), &Frame, pCtxt, 0, &SymFunctionTableAccess64, &SymGetModuleBase64, 0);

    pFrames[i] = S;
    i++;
  }

  SymCleanup(GetCurrentProcess());

  return i;

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Debug::GenerateCoreDump(LPCTSTR Filename, PEXCEPTION_POINTERS pExceptionInfo, MINIDUMP_TYPE Type)
{
  String Tmp;

  if (Filename == 0)
    Tmp = String::Format(TEXT("Core.%d.dmp"), ::GetCurrentProcessId());
  else
    Tmp = Filename;

  MINIDUMP_EXCEPTION_INFORMATION ExInfo;
  ExInfo.ClientPointers = true;
  ExInfo.ExceptionPointers = (PEXCEPTION_POINTERS)pExceptionInfo;
  ExInfo.ThreadId = ::GetCurrentThreadId();

  HANDLE hFile = CreateFile(Tmp.ConstStr(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
  if (pExceptionInfo)
    MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, Type, &ExInfo, 0, 0);
  else
    MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, Type, 0, 0, 0);

  CloseHandle(hFile);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
String Debug::Symbol::ParseFunctionName(String FunctionSignature)
{
  String::Iterator i = FunctionSignature.Find("::");
  if (i == FunctionSignature.End())
    return FunctionSignature;

  String::Iterator NameBegin = FunctionSignature.FindLast(' ', FunctionSignature.Begin(), i) + 1;
  String::Iterator ParamsBegin = FunctionSignature.Find('(', i, FunctionSignature.End());

  return FunctionSignature.Substr(NameBegin, ParamsBegin);
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_LINUX)
#include <execinfo.h>
VOID* Debug::AddressFromSymbol(CONST CHAR *Name)
{
  // TODO: Implement
  return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
String Debug::SymbolFromAddress(VOID *Address)
{
  // TODO: Implement
  return "";
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Debug::GenerateStackTrace(StackFrame* pFrames, SIZE_T MaximumNumberOfFrames)
{
  VOID* StackTrace[128];

  SIZE_T N = backtrace(StackTrace, 128);
  CHAR** StackTraceNames = backtrace_symbols(StackTrace, N);

  for (SIZE_T i=0; i<N; i++)
  {
    if (i >= MaximumNumberOfFrames)
      break;

    pFrames[i].InstructionPtr() = StackTrace[i];
    pFrames[i].Symbol().SetName(StackTraceNames[i]);
  }

  return N;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Debug::ParseStackTraceString(CHAR* String, Symbol& Sym)
{


}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Debug::GenerateCoreDump(LPCTSTR Filename)
{
  // TODO: Implement
  return;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
String Debug::Symbol::ParseFunctionName(String FunctionSignature)
{
  String::Iterator i = FunctionSignature.Find("::");
  if (i == FunctionSignature.End())
    return FunctionSignature;

  String::Iterator NameBegin = FunctionSignature.FindLast(' ', FunctionSignature.Begin(), i) + 1;
  String::Iterator ParamsBegin = FunctionSignature.Find('(', i, FunctionSignature.End());

  String::Iterator TemplateBegin = FunctionSignature.Find("[with", i, FunctionSignature.End());
  String Template;
  if (TemplateBegin != FunctionSignature.End())
  {
    String::Iterator TemplateEnd = FunctionSignature.Find(']', TemplateBegin, FunctionSignature.End());

    Template = "<" + FunctionSignature.Substr(TemplateBegin + 6, TemplateEnd) + ">";
  }

  return FunctionSignature.Substr(NameBegin, ParamsBegin) + Template;
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------

#if defined(PLATFORM_MACOS)
VOID* Debug::AddressFromSymbol(CONST CHAR *Name)
{
  return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
String Debug::SymbolFromAddress(VOID *Address)
{
  return "";
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Debug::GenerateStackTrace(StackFrame* pFrames, SIZE_T MaximumNumberOfFrames)
{
  return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Debug::ParseStackTraceString(CHAR* String, Symbol& Sym)
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Debug::GenerateCoreDump(LPCTSTR Filename)
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
String Debug::Symbol::ParseFunctionName(String FunctionSignature)
{
  String::Iterator i = FunctionSignature.Find("::");
  if (i == FunctionSignature.End())
    return FunctionSignature;

  String::Iterator NameBegin = FunctionSignature.FindLast(' ', FunctionSignature.Begin(), i) + 1;
  String::Iterator ParamsBegin = FunctionSignature.Find('(', i, FunctionSignature.End());

  return FunctionSignature.Substr(NameBegin, ParamsBegin);
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------
