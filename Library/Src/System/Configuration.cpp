/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/Configuration.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 1735 $
 * Last Updated: $Date: 2008-12-21 03:47:38 -0800 (Sun, 21 Dec 2008) $
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
#include "System/Configuration.hpp"
#include "System/Application.hpp"
#include "IO/File.hpp"
#include "IO/TextStream.hpp"
#include "Debug/Trace.hpp"

using namespace Bolitho;
using namespace Bolitho::System;
using namespace Bolitho::IO;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::System,ConfigurationException,Exception);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Map<String,String> Configuration::m_ConfigValues;
Map<String,Configuration::Item> Configuration::m_Schema;
Array<String> Configuration::m_CommandLineBindings;
Map<String,String> Configuration::m_NameMapping;
Map<String,StringArray> Configuration::m_ConfigListValues;
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Configuration::Declare(LPCTSTR FullName, LPCTSTR DefaultValue, LPCTSTR Description, BOOL Required, DWORD LoadFrom, DWORD Persist)
{
  Declare(FullName, FullName, DefaultValue, Description, Required, LoadFrom, Persist);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Configuration::Declare(LPCTSTR FullName, LPCTSTR ShortName, LPCTSTR DefaultValue, LPCTSTR Description, BOOL Required, DWORD Source, DWORD Persist)
{
  Item I;
  I.m_FullName = FullName;
  I.m_ShortName = ShortName;
  I.m_DefaultValue = DefaultValue;
  I.m_Description = Description;
  I.m_MinimumCount = 1;
  I.m_MaximumCount = 1;
  I.m_IsList = FALSE;
  I.m_Count = 0;
  I.m_Present = FALSE;
  I.m_Source = Source;
  I.m_Persist = Persist;

  m_Schema.Add(FullName, I);

  if (DefaultValue != 0)
    m_ConfigValues.Add(FullName, DefaultValue);

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Configuration::DeclareList(LPCTSTR FullName, LPCTSTR ShortName, LPCTSTR DefaultValue, LPCTSTR Description, UINT MinimumListLength, UINT MaximumListLength, DWORD Source, DWORD Persist)
{
  Item I;
  I.m_FullName = FullName;
  I.m_ShortName = ShortName;
  I.m_DefaultValue = DefaultValue;
  I.m_Description = Description;
  I.m_MinimumCount = MinimumListLength;
  I.m_MaximumCount = MaximumListLength;
  I.m_IsList = TRUE;
  I.m_Count = 0;
  I.m_Present = FALSE;
  I.m_Source = Source;
  I.m_Persist = Persist;

  m_Schema.Add(FullName, I);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Configuration::BindCommandLineValue(LPCTSTR Name)
{
  m_CommandLineBindings.Add(Name);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Configuration::IsPresent(LPCTSTR Name)
{
  if (m_Schema.Contains(Name))
  {
    Item& I = m_Schema[Name];
    return I.m_Present;
  }
  else
  {
    return m_ConfigValues.Contains(Name);
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Configuration::IsSet(LPCTSTR Name)
{
  if (IsPresent(Name))
    return GetValue<BOOL>(Name) != 0;
  return FALSE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Configuration::LoadEnvironment()
{
  for(Map<String,Item>::Iterator i=m_Schema.Begin(); i != m_Schema.End(); i++)
  {
    if (i.Value().m_Source & ConfigurationSource::ENVIRONMENT && !i.Value().m_Present)
    {
      String V = Application::GetEnvironment(i.Value().m_FullName);
      if (!V.IsEmpty())
      {
        m_ConfigValues.Add(i.Value().m_FullName, V);
        i.Value().m_Present = TRUE;
      }
    }
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Configuration::LoadConfigFile(LPCTSTR Filename)
{
  TextStream T(File::Open(Filename, FileMode::OPEN));

  while(!T.EndOfStream())
  {
    String L = T.ReadLine();
    if (L.BeginsWith("#"))
      continue;

    String::Iterator i = L.Find('=');
    if (i != L.End())
    {
      String Name = L.Substr(L.Begin(), i);
      String Value = L.Substr(i+1, L.End());

      if (m_Schema.Contains(Name))
      {
        Item I = m_Schema.Get(Name);
        if ((I.m_Source & ConfigurationSource::FILE) == 0)
          continue;
        I.m_Present = TRUE;
      }

      m_ConfigValues.Add(Name,Value);
    }
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Configuration::LoadCommandLine(String CmdLineString)
{
  Array<String> Tokens;
  String CurrentToken;

  SIZE_T i = 0;
  enum { WHITESPACE, TEXT, QUOTEDTEXT } State = WHITESPACE;

  while (i < CmdLineString.Length())
  {
    switch(State)
    {
    case WHITESPACE:
      if (CmdLineString(i) == '\"')
      {
        State = QUOTEDTEXT;
        break;
      }
      else if (!String::IsWhitespace(CmdLineString(i)))
      {
        State = TEXT;
        CurrentToken += CmdLineString(i);
        break;
      }
      break;
    case QUOTEDTEXT:
      if (CmdLineString(i) == '\\' && CmdLineString(i+1) == '\"')
      {
        CurrentToken += '\"';
        i++;
        break;
      }
      else if (CmdLineString(i) == '\"')
      {
        if (String::IsWhitespace(CmdLineString(i+1)))
        {
          State = WHITESPACE;
          Tokens.Add(CurrentToken);
          CurrentToken = String::Empty;
          break;
        }
        else
          State = TEXT;
      }
      CurrentToken += CmdLineString(i);
      break;
    case TEXT:
      if (String::IsWhitespace(CmdLineString(i)))
      {
        State = WHITESPACE;
        Tokens.Add(CurrentToken);
        CurrentToken = String::Empty;
        break;
      }
      else if (CmdLineString(i) == '\"')
      {
        State = QUOTEDTEXT;
        break;
      }
      CurrentToken += CmdLineString(i);
      break;
      
    };

    i++;
  };

  if (State == TEXT || State == QUOTEDTEXT)
    Tokens.Add(CurrentToken);

  SIZE_T ValueIndex = 0;
  SIZE_T ValueItemIndex = 0;

  for (SIZE_T i=1; i<Tokens.Length(); i++)
  {
    if (Tokens(i)(0) == '-' && Tokens(i)(1) == '-')
    {
      ValueItemIndex = 0;
      ValueIndex++;
      
      String::Iterator Colon = Tokens(i).Find(TEXT("="));
      if (Colon == Tokens(i).End())
      {
        String FlagName = Tokens(i).Substr(2);
        if (FlagName.Compare(TEXT("?"), FALSE) == 0 || FlagName.Compare(TEXT("Help"), FALSE) == 0)
          return;

        if (m_NameMapping.Contains(FlagName))
          FlagName = m_NameMapping[FlagName];
        
        if (m_Schema.Contains(FlagName))
        {
          Item& I = m_Schema[FlagName];
          I.m_Count = 1;
          I.m_Present = TRUE;
        }
        SetValue<String>(FlagName, "1");
      }
      else
      {
        String Key = Tokens(i).Substr(Tokens(i).Begin()+2, Colon);
        String Value = Tokens(i).Substr(Colon+1);

        if (m_NameMapping.Contains(Key))
          Key = m_NameMapping[Key];
        
        if (m_Schema.Contains(Key))
        {
          Item& I = m_Schema[Key];
          I.m_Count = 1;
          I.m_Present = TRUE;
        }
        SetValue<String>(Key, Value);
      }
      
    }
    #if defined(PLATFORM_WINDOWS)
    else if (Tokens(i)(0) == '/')
    {
      ValueItemIndex = 0;
      ValueIndex++;
      
      String::Iterator Colon = Tokens(i).Find(TEXT(":"));
      if (Colon == Tokens(i).End())
      {
        String FlagName = Tokens(i).Substr(1);
        if (FlagName.Compare(TEXT("?"), FALSE) == 0 || FlagName.Compare(TEXT("Help"), FALSE) == 0)
          return;

        if (m_NameMapping.Contains(FlagName))
          FlagName = m_NameMapping[FlagName];
        
        if (m_Schema.Contains(FlagName))
        {
          Item& I = m_Schema[FlagName];
          I.m_Count = 1;
          I.m_Present = TRUE;
        }
        SetValue<String>(FlagName, "1");

      }
      else
      {
        String Key = Tokens(i).Substr(Tokens(i).Begin()+1, Colon);
        String Value = Tokens(i).Substr(Colon+1);

        if (m_NameMapping.Contains(Key))
          Key = m_NameMapping[Key];
        
        if (m_Schema.Contains(Key))
        {
          Item& I = m_Schema[Key];
          I.m_Count = 1;
          I.m_Present = TRUE;
        }
        SetValue<String>(Key, Value);
      }
    }
    #endif
    else
    {
      if (ValueIndex < m_CommandLineBindings.Length())
      {
        String Name = m_CommandLineBindings[ValueIndex];
        Item& I = m_Schema[Name];

        if (I.m_IsList)
        {
          if (!m_ConfigListValues.Contains(Name))
            m_ConfigListValues.Add(Name, StringArray());
          m_ConfigListValues[Name].Add(Tokens(i));
        }
        else
        {
          SetValue<String>(Name, Tokens(i));
        }
        
        ValueItemIndex++;
        if (ValueItemIndex >= I.m_MaximumCount)
        {
          ValueItemIndex = 0;
          ValueIndex++;
        }
      }
    }

	}

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_WINDOWS)
VOID Configuration::LoadRegistry(Ptr<RegistryKey> pKey)
{

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Configuration::SaveRegistry(Ptr<RegistryKey> pKey)
{

}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Configuration::SaveConfigFile(LPCTSTR Filename)
{

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Configuration::Validate()
{
  for(Map<String,Item>::Iterator i=m_Schema.Begin(); i!=m_Schema.End(); i++)
  {
    Trace("%s", i.Key().ConstStr());
  }

  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Configuration::TraceSettings()
{
  Trace("Configuration Values:");
  Trace.Indent();
  for (Map<String,String>::Iterator i=m_ConfigValues.Begin(); i!=m_ConfigValues.End(); i++)
    Trace("%s=%s", i.Key().ConstStr(), i.Value().ConstStr());
  Trace.UnIndent();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

