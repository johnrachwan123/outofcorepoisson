/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/PlatformException.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 1741 $
 * Last Updated: $Date: 2008-12-24 06:08:06 -0800 (Wed, 24 Dec 2008) $
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
#include "PlatformException.hpp"

using namespace Bolitho;

#if defined(PLATFORM_POSIX)
//----------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho,PosixSystemException,Exception); 
//----------------------------------------------------------------------------------------------------------------------------
String PosixSystemException::Description() CONST
{
  return String::Format(TEXT("Posix System Error %d \'%s\'"), m_ErrorNumber, m_Description);
}
//----------------------------------------------------------------------------------------------------------------------------
VOID PosixSystemException::ThrowError(CONST Object* pObject, INT ErrorNumber)
{
  if (ErrorNumber != 0)
    throw PosixSystemException(pObject, strerror(ErrorNumber), ErrorNumber);
}
//----------------------------------------------------------------------------------------------------------------------------
PosixSystemException* PosixSystemException::GetError(CONST Object* pObject, INT ErrorNumber)
{
  if (ErrorNumber != 0)
    return new PosixSystemException(pObject, strerror(ErrorNumber), ErrorNumber);
  return 0;
}
//----------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho,SegmentationFaultException,SystemException); 
//----------------------------------------------------------------------------------------------------------------------------
SegmentationFaultException::SegmentationFaultException() : Exception(0)
{
}
//----------------------------------------------------------------------------------------------------------------------------
String SegmentationFaultException::Description() CONST
{
  return String::Format(TEXT("Segmentation Fault"));
}
//----------------------------------------------------------------------------------------------------------------------------

#endif

#if defined(PLATFORM_WINDOWS)
//----------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho,Win32Exception,Exception); 
ImplementRuntimeType(Bolitho,AccessViolationException,SystemException); 
ImplementRuntimeType(Bolitho,StackOverflowException,SystemException); 
ImplementRuntimeType(Bolitho,IntegerMathException,SystemException); 
ImplementRuntimeType(Bolitho,FloatingPointMathException,SystemException); 
//----------------------------------------------------------------------------------------------------------------------------
String Win32Exception::Description() CONST
{
  return String::Format(TEXT("Win32 Error %d \'%s\'"), m_ErrorNumber, m_Description);
}
//----------------------------------------------------------------------------------------------------------------------------
VOID Win32Exception::ThrowError(CONST Object* pObject, DWORD ErrorNumber)
{
  if (ErrorNumber > 0)
  {
    #if defined(UNICODE)
    LPWSTR pMsgBuf;
    FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | 
      FORMAT_MESSAGE_FROM_SYSTEM |
      FORMAT_MESSAGE_MAX_WIDTH_MASK | FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      ErrorNumber,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (LPWSTR)&pMsgBuf,
      0, NULL);
    #else
    LPSTR pMsgBuf;
    FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | 
      FORMAT_MESSAGE_FROM_SYSTEM |
      FORMAT_MESSAGE_MAX_WIDTH_MASK | FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      ErrorNumber,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (LPSTR)&pMsgBuf,
      0, NULL);
    #endif
    throw Win32Exception(pObject, (CONST TCHAR*)pMsgBuf, ErrorNumber);
  }
}
//----------------------------------------------------------------------------------------------------------------------------
Win32Exception* Win32Exception::GetError(CONST Object* pObject, DWORD ErrorNumber)
{
  if (ErrorNumber > 0)
  {
    #if defined(UNICODE)
    LPWSTR pMsgBuf;
    FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | 
      FORMAT_MESSAGE_FROM_SYSTEM |
      FORMAT_MESSAGE_MAX_WIDTH_MASK,
      NULL,
      ErrorNumber,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (LPWSTR)&pMsgBuf,
      0, NULL);
    #else
    LPSTR pMsgBuf;
    FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | 
      FORMAT_MESSAGE_FROM_SYSTEM |
      FORMAT_MESSAGE_MAX_WIDTH_MASK,
      NULL,
      ErrorNumber,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (LPSTR)&pMsgBuf,
      0, NULL);
    #endif
    return new Win32Exception(pObject, pMsgBuf, ErrorNumber);
  }
  return 0;
}
//----------------------------------------------------------------------------------------------------------------------------
String AccessViolationException::Description() CONST
{
  return String::Format(TEXT("The memory at 0x%p could not be %s"), m_FaultingAddress, (m_Operation == 0) ? TEXT("read") : TEXT("written"));
}
//----------------------------------------------------------------------------------------------------------------------------
String StackOverflowException::Description() CONST
{
  return String::Format(TEXT("Stack Overflow"));
}
//----------------------------------------------------------------------------------------------------------------------------
String IntegerMathException::Description() CONST
{
  switch (m_Operation)
  {
  case EXCEPTION_INT_DIVIDE_BY_ZERO:
    return TEXT("Divide by zero");
  case EXCEPTION_INT_OVERFLOW:
    return TEXT("Overflow");
  default:
    return TEXT("");
  };
}
//----------------------------------------------------------------------------------------------------------------------------
String FloatingPointMathException::Description() CONST
{
  switch (m_Operation)
  {
  case EXCEPTION_FLT_DENORMAL_OPERAND:
    return TEXT("Denormal operand");
  case EXCEPTION_FLT_DIVIDE_BY_ZERO:
    return TEXT("Divide by zero");
  case EXCEPTION_FLT_INEXACT_RESULT:
    return TEXT("Inexact result");
  case EXCEPTION_FLT_INVALID_OPERATION:
    return TEXT("Invalid operation");
  case EXCEPTION_FLT_OVERFLOW:
    return TEXT("Overflow");
  case EXCEPTION_FLT_STACK_CHECK:
    return TEXT("Stack underflow/overflow");
  case EXCEPTION_FLT_UNDERFLOW:
    return TEXT("Underflow");
  default:
    return TEXT("");
  };
}
//----------------------------------------------------------------------------------------------------------------------------
#endif
