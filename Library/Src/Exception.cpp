/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Exception.cpp $
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
#include "Text/StringBuilder.hpp"
#include "IO/Path.hpp"

using namespace Bolitho;

//----------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho,Exception,Object);
ImplementRuntimeType(Bolitho,UserException,Exception);
ImplementRuntimeType(Bolitho,InvalidOperationException,Exception);
ImplementRuntimeType(Bolitho,OutOfMemoryException,Exception);
ImplementRuntimeType(Bolitho,TimeoutException,Exception);
ImplementRuntimeType(Bolitho,NotImplementedException,Exception);
ImplementRuntimeType(Bolitho,InvalidFormatException,Exception);
ImplementRuntimeType(Bolitho,InvalidCastException,Exception);
ImplementRuntimeType(Bolitho,InvalidArgumentException,Exception);
ImplementRuntimeType(Bolitho,InvalidIndexException,Exception);
//----------------------------------------------------------------------------------------------------------------------------
Exception::Exception(CONST Object* pSource)
{
	m_pInnerException = 0;
  m_pObject = pSource;

	m_pStackTrace = new Debug::StackFrame[32];
	m_StackTraceCount = Debug::GenerateStackTrace(m_pStackTrace, 32);
		
}
//----------------------------------------------------------------------------------------------------------------------------
Exception::Exception(CONST Object* pSource, CONST Exception* pInnerException)
{
  m_pInnerException = pInnerException;
  m_pObject = pSource;

	m_pStackTrace = new Debug::StackFrame[32];
	m_StackTraceCount = Debug::GenerateStackTrace(m_pStackTrace, 32);
}
//----------------------------------------------------------------------------------------------------------------------------
Exception::Exception(CONST Exception& E)
{
  m_pObject = E.m_pObject;
  m_pStackTrace = E.m_pStackTrace;
	m_StackTraceCount = E.m_StackTraceCount;
}
//----------------------------------------------------------------------------------------------------------------------------
Exception::~Exception()
{
	// TODO: Free m_pStackTrace
}
//----------------------------------------------------------------------------------------------------------------------------
String Exception::Description() CONST
{
  if (m_pInnerException)
    return m_pInnerException->Description();
  else
    return String::Empty;
}
//----------------------------------------------------------------------------------------------------------------------------
String Exception::ToString() CONST
{
  StringBuilder Message;

  Message.AppendFormatLine(TEXT("Exception Type: %s::%s"), GetType()->Namespace(), GetType()->Name());
  Message.AppendFormatLine(TEXT("Description: %s"), Description().ConstStr());

  Message.AppendLine(TEXT("\nStack Trace:"));
  
  for(SIZE_T i=0; i<m_StackTraceCount; i++)
  {
#if defined(UNICODE)
    if (m_pStackTrace[i].Symbol().SourceFile() != 0)
      Message.AppendFormatLine(TEXT("0x%p %S() at %S:%d"), m_pStackTrace[i].InstructionPtr(), m_pStackTrace[i].Symbol().Name(), m_pStackTrace[i].Symbol().SourceFile(), m_pStackTrace[i].Symbol().SourceLine());
    else
      Message.AppendFormatLine(TEXT("0x%p %S()"), m_pStackTrace[i].InstructionPtr(), m_pStackTrace[i].Symbol().Name());
#else
    if (m_pStackTrace[i].Symbol().SourceFile() != 0)
      Message.AppendFormatLine(TEXT("0x%p %s()\r\n  at %s:%d"), m_pStackTrace[i].InstructionPtr(), m_pStackTrace[i].Symbol().Name(), IO::Path::Filename(m_pStackTrace[i].Symbol().SourceFile()).ConstStr(), m_pStackTrace[i].Symbol().SourceLine());
    else
      Message.AppendFormatLine(TEXT("0x%p %s()"), m_pStackTrace[i].InstructionPtr(), m_pStackTrace[i].Symbol().Name());
#endif
  }

  return Message.ToString();
}
//----------------------------------------------------------------------------------------------------------------------------
Array<Debug::StackFrame> Exception::StackTrace() CONST
{
	return Array<Debug::StackFrame>(m_pStackTrace, m_StackTraceCount);
}
//----------------------------------------------------------------------------------------------------------------------------
CONST Object* Exception::Source()
{
  return m_pObject;
}
//----------------------------------------------------------------------------------------------------------------------------
String InvalidIndexException::Description() CONST
{
  return String::Format(TEXT("Index %s=%u is out of bounds"), m_Variable, m_Value);
}
//----------------------------------------------------------------------------------------------------------------------------
String InvalidCastException::Description() CONST
{
  return String::Format(TEXT("Unable to cast from %s to %s:  No acceptable conversion is available"), m_pObject->GetType()->Name(), m_To->Name());
}
//----------------------------------------------------------------------------------------------------------------------------
String OutOfMemoryException::Description() CONST
{
  return m_Description;
}
//----------------------------------------------------------------------------------------------------------------------------
String TimeoutException::Description() CONST
{
  return m_Description;
}
//----------------------------------------------------------------------------------------------------------------------------
String NotImplementedException::Description() CONST
{
  return m_Description;
}
//----------------------------------------------------------------------------------------------------------------------------
String InvalidOperationException::Description() CONST
{
  return m_Description;
}
//----------------------------------------------------------------------------------------------------------------------------
String InvalidFormatException::Description() CONST
{
  return m_Description;
}
//----------------------------------------------------------------------------------------------------------------------------
String InvalidArgumentException::Description() CONST
{
  return m_Description;
}
//----------------------------------------------------------------------------------------------------------------------------

