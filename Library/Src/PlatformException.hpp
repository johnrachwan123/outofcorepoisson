/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/PlatformException.hpp $
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

#if defined(PLATFORM_POSIX)

#define SystemException PosixSystemException
namespace Bolitho
{
  class LIBRARY_API PosixSystemException : public Exception
  {
    DeclareRuntimeType();
  public:
    PosixSystemException(CONST Object* pSource, LPCTSTR Description, INT ErrorNumber) : Exception(pSource)
    {
      m_Description = CharTraits<TCHAR>::Duplicate(Description);
      m_ErrorNumber = ErrorNumber;
    }
    
		PosixSystemException(CONST PosixSystemException& E) : Exception(E)
    {
      m_Description = CharTraits<TCHAR>::Duplicate(E.m_Description);
      m_ErrorNumber = E.m_ErrorNumber;
    }
    ~PosixSystemException()
    {
      if (m_Description)
        free(m_Description);
    }

    virtual String Description() CONST;

    static VOID ThrowError(CONST Object* pObject = 0, INT ErrorCode = errno);
    static PosixSystemException* GetError(CONST Object* pObject = 0, INT ErrorCode = errno);
  private:
    LPTSTR m_Description;
    INT m_ErrorNumber;
  };


  class LIBRARY_API SegmentationFaultException : public Exception
  {
    DeclareRuntimeType();
  public:
    SegmentationFaultException();
    virtual String Description() CONST;
  private:
  };

}

#endif

#if defined(PLATFORM_WINDOWS)
#define SystemException Win32Exception
namespace Bolitho
{
  class LIBRARY_API Win32Exception : public Exception
  {
    DeclareRuntimeType();
  public:
    Win32Exception(CONST Object* pSource, CONST TCHAR* Description, UINT ErrorNumber) : Exception(pSource)
    {
      m_Description = CharTraits<TCHAR>::Duplicate(Description);
      m_ErrorNumber = ErrorNumber;
    }
    Win32Exception(CONST Win32Exception& E) : Exception(E)
    {
      m_Description = CharTraits<TCHAR>::Duplicate(E.m_Description);
      m_ErrorNumber = E.m_ErrorNumber;
    }
    
		virtual ~Win32Exception()
    {
      if (m_Description)
        free(m_Description);
    }

    virtual String Description() CONST;

    static VOID ThrowError(CONST Object* pObject = 0, DWORD ErrorCode = ::GetLastError());
    static Win32Exception* GetError(CONST Object* pObject = 0, DWORD ErrorCode = ::GetLastError());
  private:
    TCHAR* m_Description;
    UINT m_ErrorNumber;
  };

  
  class LIBRARY_API AccessViolationException : public Exception
  {
    DeclareRuntimeType();
  public:
    AccessViolationException(Debug::StackFrame* pContext, SIZE_T ContextLength, PVOID FaultingAddress, UINT Operation) : 
        Exception(0)
    {
      m_FaultingAddress = FaultingAddress;
      m_Operation = Operation;
      m_pStackTrace = pContext;
			m_StackTraceCount = ContextLength;
    }

    PVOID FaultingAddress()
    { return m_FaultingAddress; }

    virtual String Description() CONST;

  private:
    PVOID m_FaultingAddress;
    UINT m_Operation;
  };
  
  class LIBRARY_API IntegerMathException : public Exception
  {
    DeclareRuntimeType();
  public:
    IntegerMathException(Debug::StackFrame* pContext, SIZE_T ContextLength, UINT Operation) : 
        Exception(0)
    {
      m_Operation = Operation;
      m_pStackTrace = pContext;
			m_StackTraceCount = ContextLength;

    }

    UINT Operation()
    { return m_Operation; }

    virtual String Description() CONST;

  private:
    UINT m_Operation;
  };

  class LIBRARY_API FloatingPointMathException : public Exception
  {
    DeclareRuntimeType();
  public:
    FloatingPointMathException(Debug::StackFrame* pContext, SIZE_T ContextLength, UINT Operation) : 
        Exception(0)
    {
      m_Operation = Operation;
      m_pStackTrace = pContext;
			m_StackTraceCount = ContextLength;

    }

    UINT Operation()
    { return m_Operation; }

    virtual String Description() CONST;

  private:
    UINT m_Operation;
  };

  class LIBRARY_API StackOverflowException : public Exception
  {
    DeclareRuntimeType();
  public:
    StackOverflowException(Debug::StackFrame* pContext, SIZE_T ContextLength) :
        Exception(0)
    {
      m_pStackTrace = pContext;
			m_StackTraceCount = ContextLength;

    }

    virtual String Description() CONST;
  };


}

#endif
