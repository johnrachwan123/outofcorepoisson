/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/CommandLineParser.cpp $
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




#include "CodeLibrary.hpp"
#include "System/CommandLineParser.hpp"
#include "IO/StdIO.hpp"

using namespace Bolitho;
using namespace Bolitho::System;
using namespace Bolitho::IO;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::System,CommandLineParser,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CommandLineParser::IsPresent( LPCTSTR Name ) CONST
{
  for (SIZE_T i=0; i<m_Flags.Length(); i++)
    if (m_Flags[i].m_Name == Name)
      return m_Flags[i].m_Present;

  for (SIZE_T i=0; i<m_KeyedValues.Length(); i++)
    if (m_KeyedValues[i].m_Name == Name)
      return m_KeyedValues[i].m_Present;

  for (SIZE_T i=0; i<m_Values.Length(); i++)
    if (m_Values[i].m_Name == Name)
      return m_Values[i].m_Values.Length() > 0;

  throw InvalidArgumentException(this, String::Format( TEXT("The value \'%s\' cannot be found"), Name ) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CommandLineParser::Parse( LPCTSTR CmdLineString )
{
  Array<String> Tokens;
  String CurrentToken;

  SIZE_T i = 0;
  enum { WHITESPACE, TEXT, QUOTEDTEXT } State = WHITESPACE;

  while (CmdLineString[i] != 0)
  {
    switch(State)
    {
    case WHITESPACE:
      if (CmdLineString[i] == '\"')
      {
        State = QUOTEDTEXT;
        break;
      }
      else if (!String::IsWhitespace(CmdLineString[i]))
      {
        State = TEXT;
        CurrentToken += CmdLineString[i];
        break;
      }
      break;
    case QUOTEDTEXT:
      if (CmdLineString[i] == '\\' && CmdLineString[i+1] == '\"')
      {
        CurrentToken += '\"';
        i++;
        break;
      }
      else if (CmdLineString[i] == '\"')
      {
        if (String::IsWhitespace(CmdLineString[i+1]))
        {
          State = WHITESPACE;
          Tokens.Add(CurrentToken);
          CurrentToken = String::Empty;
          break;
        }
        else
          State = TEXT;
      }
      CurrentToken += CmdLineString[i];
      break;
    case TEXT:
      if (String::IsWhitespace(CmdLineString[i]))
      {
        State = WHITESPACE;
        Tokens.Add(CurrentToken);
        CurrentToken = String::Empty;
        break;
      }
      else if (CmdLineString[i] == '\"')
      {
        State = QUOTEDTEXT;
        break;
      }
      CurrentToken += CmdLineString[i];
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
    if (Tokens(i)(0) == '/')
    {
      ValueItemIndex = 0;
      ValueIndex++;

      String::Iterator Colon = Tokens(i).Find(TEXT(":"));
      if (Colon == Tokens(i).End())
      {
        String FlagName = Tokens(i).Substr(1);
        if (FlagName.Compare( TEXT("?"), FALSE) == 0 || FlagName.Compare( TEXT("Help"), FALSE) == 0 )
          return FALSE;

        for (SIZE_T j=0; j<m_Flags.Length(); j++)
        {
          if (FlagName.Compare(m_Flags[j].m_Name, FALSE) == 0)
          {
            m_Flags[j].m_Present = TRUE;
            break;
          }
        }
      }
      else
      {
        String Key = Tokens(i).Substr( Tokens(i).Begin()+1, Colon );
        String Value = Tokens(i).Substr( Colon+1 );
        for (SIZE_T j=0; j<m_KeyedValues.Length(); j++)
        {
          if (Key.Compare(m_KeyedValues[j].m_Name, FALSE) == 0)
          {
            m_KeyedValues[j].m_Present = TRUE;
            m_KeyedValues[j].m_Value = Value;
          }
        }
      }
    }
    else
    {
      if (ValueIndex < m_Values.Length())
      {
        m_Values[ValueIndex].m_Values.Add(Tokens(i));
        ValueItemIndex++;
        if (ValueItemIndex >= m_Values[ValueIndex].m_MaximumItems)
        {
          ValueItemIndex = 0;
          ValueIndex++;
        }
      }
    }

  }

  return Validate();

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID CommandLineParser::DefineFlag(LPCTSTR Name, LPCTSTR ConfigurationName, LPCTSTR Description)
{
  m_Flags.Add( Flag(Name, Description, ConfigurationName) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID CommandLineParser::DefineKeyedValue(LPCTSTR Name, LPCTSTR ConfigurationName, LPCTSTR Description, BOOL Required, LPCTSTR DefaultValue)
{
  m_KeyedValues.Add( KeyedValue(Name, Description, ConfigurationName, Required, DefaultValue) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID CommandLineParser::DefineValueList(LPCTSTR Name, LPCTSTR Description, UINT MinimumRequired, UINT MaximumItems)
{
  m_Values.Add( ValueList(Name, Description, MinimumRequired, MaximumItems) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID CommandLineParser::DefineValue(LPCTSTR Name, LPCTSTR Description, BOOL Required)
{
  m_Values.Add( ValueList(Name, Description, Required ? 1 : 0, 1) );
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
String CommandLineParser::ActualUsage()
{
  String S;

  for (SIZE_T i=0; i<m_Flags.Length(); i++)
    if (m_Flags[i].m_Present)
      S += String::Format( TEXT("/%s "), m_Flags[i].m_Name.ConstStr(), m_Flags[i].m_Description.ConstStr() );

  for (SIZE_T i=0; i<m_KeyedValues.Length(); i++)
    if (m_KeyedValues[i].m_Present)
      S += String::Format( TEXT("/%s:%s "), m_KeyedValues[i].m_Name.ConstStr(), m_KeyedValues[i].m_Value.ConstStr() );

  for (SIZE_T i=0; i<m_Values.Length(); i++)
    for (SIZE_T j=0; j<m_Values[i].m_Values.Length(); j++)
      S += String::Format( TEXT("%s "), m_Values[i].m_Values[j].ConstStr() );

  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String CommandLineParser::Usage( LPCTSTR ApplicationName )
{
  String S;

  S += String::Format( TEXT("Usage: %s "), ApplicationName );

  S += TEXT("[/?] [/Help] ");
  for (SIZE_T i=0; i<m_Flags.Length(); i++)
    S += String::Format( TEXT("[/%s] "), m_Flags[i].m_Name.ConstStr(), m_Flags[i].m_Description.ConstStr() );

  for (SIZE_T i=0; i<m_KeyedValues.Length(); i++)
    if (m_KeyedValues[i].m_Required)
      S += String::Format( TEXT("/%s:<Value> "), m_KeyedValues[i].m_Name.ConstStr() );
    else if (m_KeyedValues[i].m_Value.IsEmpty())
      S += String::Format( TEXT("[/%s:<Value>] "), m_KeyedValues[i].m_Name.ConstStr() );
    else
      S += String::Format( TEXT("[/%s:<Value=%s>] "), m_KeyedValues[i].m_Name.ConstStr(), m_KeyedValues[i].m_Value.ConstStr() );

    for (SIZE_T i=0; i<m_Values.Length(); i++)
      if (m_Values[i].m_RequiredItems > 0)
        if (m_Values[i].m_MaximumItems > 1)
          S += String::Format( TEXT("<%s> ..."), m_Values[i].m_Name.ConstStr() );
        else
          S += String::Format( TEXT("<%s> "), m_Values[i].m_Name.ConstStr() );
      else
        if (m_Values[i].m_MaximumItems > 1)
          S += String::Format( TEXT("[<%s> ...] "), m_Values[i].m_Name.ConstStr() );
        else
          S += String::Format( TEXT("[<%s>] "), m_Values[i].m_Name.ConstStr() );

    S += String::NewLine;
    S += String::NewLine;

    for (SIZE_T i=0; i<m_Flags.Length(); i++)
      S += String::Format( TEXT("\t/%-10s\t\t%s\r\n"), m_Flags[i].m_Name.ConstStr(), m_Flags[i].m_Description.ConstStr() );

    for (SIZE_T i=0; i<m_KeyedValues.Length(); i++)
      if (m_KeyedValues[i].m_Value.IsEmpty())
        S += String::Format( TEXT("\t/%-10s\t\t%s\r\n"), String::Format( TEXT("%s:<Value>"), m_KeyedValues[i].m_Name.ConstStr()).ConstStr() , m_KeyedValues[i].m_Description.ConstStr() );
      else
        S += String::Format( TEXT("\t/%-10s\t\t%s\r\n"), String::Format( TEXT("%s:<Value=%s>"), m_KeyedValues[i].m_Name.ConstStr(), m_KeyedValues[i].m_Value.ConstStr() ).ConstStr() , m_KeyedValues[i].m_Description.ConstStr() );

    for (SIZE_T i=0; i<m_Values.Length(); i++)
      S += String::Format( TEXT("\t%-10s\t\t%s\r\n"), m_Values[i].m_Name.ConstStr(), m_Values[i].m_Description.ConstStr() );

    S += String::NewLine;

    return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CommandLineParser::Validate()
{
  for (SIZE_T i=0; i<m_KeyedValues.Length(); i++)
  {
    if (m_KeyedValues[i].m_Required && !m_KeyedValues[i].m_Present)
      return FALSE;
  }

  for (SIZE_T i=0; i<m_Values.Length(); i++)
  {
    if (m_Values[i].m_Values.Length() < m_Values[i].m_RequiredItems)
      return FALSE;
  }

  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
