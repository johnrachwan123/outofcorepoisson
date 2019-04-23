/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/Module.hpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1588 $
 * Last Updated: $Date: 2008-08-23 09:36:02 -0700 (Sat, 23 Aug 2008) $
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

#include "System/Version.hpp"
#include "Collections/Map.hpp"

namespace Bolitho
{
  namespace System
  {
    class LIBRARY_API Module : virtual public Object
    {
    protected:
      DeclareRuntimeType();
    public:
      Module();
      virtual ~Module();

      /* Initializes the module.  Called by the framework during the initialization of an application */
      VOID InitModule(MODULEHANDLE hInstance);

      /* Returns the OS handle for the module */
      MODULEHANDLE Handle() CONST
      { return m_hModule; }

      /* Populates the Product and Image Metadata, if found in the image */
      VOID GetModuleInformation();

      /* Returns a Module object that represents a module that has been already loaded */
      static Ptr<Module> GetModule(LPCTSTR Filename);
      /* Returns a Module object that represents a module that has been already loaded */
      static Ptr<Module> GetModule(MODULEHANDLE hModule);
      /* Returns a Module object that corresponds to the executable of the process */
      static Ptr<Module> GetProcessModule();

      /* Returns a Module object corresponding to the module that is in memory at a specific location */
      static Ptr<Module> GetModuleFromAddress(LPVOID p);

      /* Reads a resource string from the module */
      String GetResourceString(UINT ResourceId);
      String GetResourceString(LPCTSTR Type, UINT ResourceId);
      String GetResourceString(LPCTSTR Type, LPCTSTR ResourceName);
      StringA GetResourceStringA(LPCTSTR Type, UINT ResourceId);
      StringA GetResourceStringA(LPCTSTR Type, LPCTSTR ResourceName);

      #if defined(USE_UNICODE)
      StringW GetResourceStringW(LPCTSTR Type, UINT ResourceId);
      StringW GetResourceStringW(LPCTSTR Type, LPCTSTR ResourceName);
      #endif

      /* Returns the product company metadata information for the module */
      CONST String& ProductCompany() CONST
      { return m_ProductCompany; }
      /* Returns the product name metadata information for the module */
      CONST String& ProductName() CONST
      { return m_ProductName; }
      /* Returns the product copyright metadata information for the module */
      CONST String& ProductCopyright() CONST
      { return m_ProductCopyright; }

      /* Returns the name of the on disk image of the module */
      CONST String& ImageName() CONST
      { return m_ImageName; }

      /* Returns TRUE if a given pointer points to a region of memory mapped from the module */
      BOOL IsPointerInModule(LPVOID p) CONST;
    protected:
      /* The OS handle for the module */
      MODULEHANDLE m_hModule;

      Version m_ProductVersion;
      String m_ProductCompany;
      String m_ProductName;
      String m_ProductCopyright;
      String m_ProductDescription;

      Version m_ImageVersion;
      String m_ImageName;
      String m_ImageBuildDate;
      String m_ImageCommitDate;
      String m_ImageConfiguration;

      LPVOID m_pBaseAddress;
      SIZE_T m_ImageSize;

      /* A mapping of OS module handles to Module objects */
      static Map< MODULEHANDLE,Module*> m_HandleToObjectMap;
    };
  }
}
