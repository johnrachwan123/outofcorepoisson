/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/Configuration.hpp $
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

#include "Collections/Map.hpp"
#include "System/Threading/CriticalSection.hpp"

#if defined(PLATFORM_WINDOWS)
#include "System/Registry.hpp"
#endif

namespace Bolitho
{
  namespace System
  {
    class LIBRARY_API ConfigurationException : public Exception
    {
      DeclareRuntimeType();
    public:
      ConfigurationException(LPCTSTR Message) : Exception(0)
      {
        m_Description = CharTraits<TCHAR>::Duplicate(Message);
      }

      ConfigurationException(CONST ConfigurationException& E) : Exception(E)
      {
        m_Description = CharTraits<TCHAR>::Duplicate(E.m_Description);
      }

      virtual String Description() CONST
      { return m_Description; }

      virtual ~ConfigurationException()
      {
        if (m_Description)
          free(m_Description);
      }

    private:
      TCHAR* m_Description;
    };

    struct ConfigurationSource
    { enum { REGISTRY = 1, FILE = 4, ENVIRONMENT = 16, COMMANDLINE = 32, ALL = 63, NONE = 0 }; };

    class LIBRARY_API Configuration
    {
    public:
      /* Declares a configuration item */
      static VOID Declare(LPCTSTR FullName, LPCTSTR ShortName, LPCTSTR DefaultValue, LPCTSTR Description, BOOL Required, DWORD LoadFrom = ConfigurationSource::ALL, DWORD Persist = ConfigurationSource::NONE);
      static VOID Declare(LPCTSTR FullName, LPCTSTR DefaultValue, LPCTSTR Description, BOOL Required, DWORD LoadFrom = ConfigurationSource::ALL, DWORD Persist = ConfigurationSource::NONE);
      static VOID DeclareList(LPCTSTR FullName, LPCTSTR ShortName, LPCTSTR DefaultValue, LPCTSTR Description, UINT MinimumListLength, UINT MaximumListLength, DWORD LoadFrom = ConfigurationSource::ALL, DWORD Persist = ConfigurationSource::NONE);

      /* Binds configuration items to unnamed command line values */
      static VOID BindCommandLineValue(LPCTSTR Name);

      /* Gets the value of a configuration item */
      template<class T>
      static T GetValue(LPCTSTR Name);

      template<class T>
      static T GetValue(LPCTSTR Name, UINT i);

      /* Determines if a configuration item has a value */
      static BOOL IsPresent(LPCTSTR Name);

      static BOOL IsSet(LPCTSTR Name);

      /* Sets the values of a configuration item */
      template<class T>
      static VOID SetValue(LPCTSTR Name, T Value);

      /* Loads configuration settings from the envronment */
      static VOID LoadEnvironment();

      /* Loads configuration settings from a file */
      static VOID LoadConfigFile(LPCTSTR Filename);

      /* Loads configuration settings from a command line string */
      static VOID LoadCommandLine(String Commandline);

      #if defined(PLATFORM_WINDOWS)
      /* Loads configuration settings from a registry key */
      static VOID LoadRegistry(Ptr<RegistryKey> pKey);

      /* Persists settings to a registry key */
      static VOID SaveRegistry(Ptr<RegistryKey> pKey);
      #endif

      /* Persists settings to a file */
      static VOID SaveConfigFile(LPCTSTR Filename);

      /* Validates configuration settings */
      static BOOL Validate();

      /* Outputs all current configuration settings to the Trace device */
      static VOID TraceSettings();

      static CONST Map<String,String>::Enumerator GetRunningValues()
      { return m_ConfigValues.Elements(); }

    private:
      /* The table of running values */
      static Map<String,String> m_ConfigValues;
      static Map<String,StringArray> m_ConfigListValues;

      /* A lock to protect access to the running value table */
      static CriticalSection m_Lock;

      class Item
      {
      public:
        Item()
        { m_MinimumCount = 0;  m_MaximumCount = MAXUINT; m_Count = 0; m_Present = FALSE; m_Source = 0; m_Persist = 0; m_IsList = FALSE; }
        Item(CONST Item& I) : m_FullName(I.m_FullName), m_ShortName(I.m_ShortName), m_DefaultValue(I.m_DefaultValue), m_Description(I.m_Description), m_MinimumCount(I.m_MinimumCount), m_MaximumCount(I.m_MaximumCount), m_Count(I.m_Count), m_Present(I.m_Present), m_Source(I.m_Source), m_Persist(I.m_Persist), m_IsList(I.m_IsList)
        {}

        String m_FullName;
        String m_ShortName;
        String m_DefaultValue;
        String m_Description;
        UINT m_MinimumCount;
        UINT m_MaximumCount;
        UINT m_Count;
        BOOL m_IsList;
        BOOL m_Present;
        DWORD m_Source;
        DWORD m_Persist;
      };

      /* The configuration schema */
      static Map<String, Item> m_Schema;

      /* Unnamed Command line argument bindings */
      static Array<String> m_CommandLineBindings;

      /* The Short->LongName mapping */
      static Map<String,String> m_NameMapping;
      
      /* Private constructor to prevent instanitation */
      Configuration()
      {}

    };

    template<class T>
    class ConfigurationVariable
    {
    public:
      ConfigurationVariable(LPCTSTR Name)
      { m_ConfigurationItemName = Name; }

      operator T ()
      { return Configuration::GetValue<T>(m_ConfigurationItemName); }
      VOID operator = (CONST T& Value)
      { Configuration::SetValue<T>(m_ConfigurationItemName); }

    private:
      String m_ConfigurationItemName;
    };

    #include "ConfigurationImpl.hpp"
  }
}
