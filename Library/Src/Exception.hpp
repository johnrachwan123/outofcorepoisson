/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Exception.hpp $
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

#include "Text/CharTraits.hpp"
#include "Debug/Debug.hpp"
#include "Utility/Traits.hpp"
#include "Utility/SingleThreadedModel.hpp"
#include "Memory/Allocator.hpp"

namespace Bolitho
{
	template<class T, class ElementTraits=Traits<T>, class Allocator=DefaultAllocator<T>, class ThreadingModel=SingleThreadedModel>
	class Array;

  class LIBRARY_API Exception : public Object
  {
    DeclareRuntimeType();
  public:
    Exception(CONST Object* pSource);
    Exception(CONST Object* pSource, CONST Exception* pInnerException);
    Exception(CONST Exception& E);

    virtual ~Exception();
    virtual String Description() CONST;
    virtual String ToString() CONST;
    
		Array<Debug::StackFrame> StackTrace() CONST;

    CONST Object* Source();
  protected:
    CONST Object* m_pObject;
    Debug::StackFrame* m_pStackTrace;
		SIZE_T m_StackTraceCount;
    CONST Exception* m_pInnerException;

  };

  class LIBRARY_API UserException : public Exception
  {
    DeclareRuntimeType();
  public:
    UserException(CONST Object* pSource) :
      Exception(pSource)
    {}
    UserException(CONST Object* pSource, CONST Exception* pInnerException) :
      Exception(pSource, pInnerException)
    {}
  };

  class LIBRARY_API InvalidOperationException : public Exception
  {
    DeclareRuntimeType();
  public:
    InvalidOperationException(CONST Object* Sender, CONST TCHAR* Description) : Exception(Sender)
    {
      m_Description = CharTraits<TCHAR>::Duplicate(Description);
    }
    
    InvalidOperationException(CONST InvalidOperationException& E) :
      Exception(E)
    {
      m_Description = CharTraits<TCHAR>::Duplicate(E.m_Description);
    }

    virtual String Description() CONST;

    virtual ~InvalidOperationException()
    {
      if (m_Description)
        free(m_Description);
    }

  private:
    TCHAR* m_Description;
  };

  class LIBRARY_API OutOfMemoryException : public Exception
  {
    DeclareRuntimeType();
  public:
    OutOfMemoryException(CONST Object* Sender, CONST TCHAR* Description) : Exception(Sender)
    {
      m_Description = CharTraits<TCHAR>::Duplicate(Description);
    }
    
    OutOfMemoryException(CONST OutOfMemoryException& E) :
      Exception(E)
    {
      m_Description = CharTraits<TCHAR>::Duplicate(E.m_Description);
    }

    virtual String Description() CONST;

    virtual ~OutOfMemoryException()
    {
      if (m_Description)
        free(m_Description);
    }

  private:
    TCHAR* m_Description;
  };

  class LIBRARY_API TimeoutException : public Exception
  {
    DeclareRuntimeType();
  public:
    TimeoutException(CONST Object* Sender, CONST TCHAR* Description) : Exception(Sender)
    {
      m_Description = CharTraits<TCHAR>::Duplicate(Description);
    }
    
    TimeoutException(CONST TimeoutException& E) :
      Exception(E)
    {
      m_Description = CharTraits<TCHAR>::Duplicate(E.m_Description);
    }

    virtual String Description() CONST;

    virtual ~TimeoutException()
    {
      if (m_Description)
        free(m_Description);
    }

  private:
    TCHAR* m_Description;
  };

  class LIBRARY_API NotImplementedException : public Exception
  {
    DeclareRuntimeType();
  public:
    NotImplementedException(CONST Object* Sender, LPCTSTR Description = TEXT("Not Implemented on the current platform")) : Exception(Sender)
    {
      m_Description = CharTraits<TCHAR>::Duplicate(Description);
    }
    
    NotImplementedException(CONST NotImplementedException& E) :
      Exception(E)
    {
      m_Description = CharTraits<TCHAR>::Duplicate(E.m_Description);
    }

    virtual String Description() CONST;

    virtual ~NotImplementedException()
    {
      if (m_Description)
        free(m_Description);
    }

  private:
    TCHAR* m_Description;
  };

  class LIBRARY_API InvalidFormatException : public Exception
  {
    DeclareRuntimeType();
  public:
    InvalidFormatException(CONST Object* Sender, CONST TCHAR* Description) : Exception(Sender)
    {
      m_Description = CharTraits<TCHAR>::Duplicate(Description);
    }
    InvalidFormatException(CONST InvalidFormatException& E) :
      Exception(E)
    {
      m_Description = CharTraits<TCHAR>::Duplicate(E.m_Description);
    }
    virtual ~InvalidFormatException()
    {
      if (m_Description)
        free(m_Description);
    }

    virtual String Description() CONST;

  private:
    TCHAR* m_Description;
  };

  class LIBRARY_API InvalidCastException : public Exception
  {
    DeclareRuntimeType();
  public:
    InvalidCastException(CONST Object* O, CONST Type* To) : Exception(O)
    {
      m_To = To;
    }

    virtual String Description() CONST;

    CONST Type* ToType()
    { return m_To; }
    CONST Type* FromType()
    { return m_pObject->GetType(); }

  private:
    CONST Type* m_To;
  };

  class LIBRARY_API InvalidArgumentException : public Exception
  {
    DeclareRuntimeType();
  public:
    InvalidArgumentException(CONST Object* Sender, CONST TCHAR* Description) : Exception(Sender)
    {
      m_Description = CharTraits<TCHAR>::Duplicate(Description);
    }
    InvalidArgumentException(CONST InvalidArgumentException& E) : Exception(E)
    {
      m_Description = CharTraits<TCHAR>::Duplicate(E.m_Description);
    }

    virtual String Description() CONST;

  private:
    TCHAR* m_Description;
  };

  class LIBRARY_API InvalidIndexException : public Exception
  {
    DeclareRuntimeType();
  public:
    InvalidIndexException(CONST Object* Sender, CONST TCHAR* Variable, SIZE_T Value) : Exception(Sender)
    {
      m_Value = Value;
      m_Variable = CharTraits<TCHAR>::Duplicate(Variable);
    }
    
    InvalidIndexException(CONST Object* Sender, LPCTSTR Variable) : Exception(Sender)
    {
      m_Variable = CharTraits<TCHAR>::Duplicate(Variable);
    }

    InvalidIndexException(CONST InvalidIndexException& E) : Exception(E)
    {
      m_Value = E.m_Value;
      m_Variable = CharTraits<TCHAR>::Duplicate(E.m_Variable);
    }

    virtual ~InvalidIndexException()
    {
      if (m_Variable)
        free(m_Variable);
    }

    virtual String Description() CONST;

    SIZE_T Value()
    { return m_Value; }

  private:
    SIZE_T m_Value;
    TCHAR* m_Variable;
  };

  #define VerifyBounds(v, min, max) { if (v < min || v > max) throw Bolitho::InvalidArgumentException(this, TEXT("\'") TEXT(#v) TEXT("\' must in the range [ ") TEXT(#min) TEXT(", ") TEXT(#max) TEXT(" ]")); }

	#define VerifyLessThan(v, vv) { if (v >= vv) throw Bolitho::InvalidArgumentException(this, TEXT("\'") TEXT(#v) TEXT("\' must be < than ") TEXT(#vv)); }
  #define VerifyGreaterThan(v, vv) { if (v <= vv) throw Bolitho::InvalidArgumentException(this, TEXT("\'"#v"\' must be > than"#vv)); }
  #define VerifyLessEqualThan(v, vv) { if (v > vv) throw Bolitho::InvalidArgumentException(this, TEXT("\'") TEXT(#v) TEXT("\' must be <= than ") TEXT(#vv)); }
  #define VerifyGreaterEqualThan(v, vv) { if (v < vv) throw Bolitho::InvalidArgumentException(this, TEXT("\'"#v"\' must be >= than"#vv)); }
  #define VerifyNotEqual(v, vv) { if (v == vv) throw Bolitho::InvalidArgumentException(this, TEXT("\'"#v"\' must be equal to "#vv)); }
  #define VerifyNotNull(v, vv) { if (v == 0) throw Bolitho::InvalidArgumentException(this, TEXT("\'"#v"\' must be NULL")); }
  #define VerifyEqual(v, vv) { if (v != vv) throw Bolitho::InvalidArgumentException(this, TEXT("\'"#v"\' must not be equal to "#vv)); }
}
