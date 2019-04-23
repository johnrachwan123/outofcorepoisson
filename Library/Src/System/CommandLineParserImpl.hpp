/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/CommandLineParserImpl.hpp $
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




//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T CommandLineParser::GetValue( LPCTSTR Name ) CONST
{
	for (SIZE_T i=0; i<m_KeyedValues.Length(); i++)
		if (m_KeyedValues[i].m_Name == Name)
			return Bolitho::Parse<T>(m_KeyedValues[i].m_Value);

	for (SIZE_T i=0; i<m_Values.Length(); i++)
		if (m_Values[i].m_Name == Name)
      if (!m_Values[i].m_Values.IsEmpty())
			  return Bolitho::Parse<T>(m_Values[i].m_Values[0]);

	throw InvalidArgumentException(this, String::Format( TEXT("The value \'%s\' cannot be found"), Name ) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
Array<T> CommandLineParser::GetValues( LPCTSTR Name ) CONST
{
  Array<T> Values;
	for (SIZE_T i=0; i<m_Values.Length(); i++)
		if (m_Values[i].m_Name == Name)
    {
      Values.Resize(m_Values[i].m_Values.Length());
      for (SIZE_T j=0; j<Values.Length(); j++)
        Values[j] = Bolitho::Parse<T>(m_Values[i].m_Values[j]);
      return Values;
    }
  
	throw InvalidArgumentException(this, String::Format( TEXT("The value \'%s\' cannot be found"), Name ) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<>
INLINE Array<String> CommandLineParser::GetValues( LPCTSTR Name ) CONST
{
	for (SIZE_T i=0; i<m_Values.Length(); i++)
		if (m_Values[i].m_Name == Name)
      return m_Values[i].m_Values;

  throw InvalidArgumentException(this, String::Format( TEXT("The value \'%s\' cannot be found"), Name ) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<>
INLINE BOOL CommandLineParser::GetValue( LPCTSTR Name ) CONST
{
	for (SIZE_T i=0; i<m_Flags.Length(); i++)
		if (m_Flags[i].m_Name == Name)
			return m_Flags[i].m_Present;

	for (SIZE_T i=0; i<m_KeyedValues.Length(); i++)
		if (m_KeyedValues[i].m_Name == Name)
			return Bolitho::Parse<BOOL>(m_KeyedValues[i].m_Value);

	for (SIZE_T i=0; i<m_Values.Length(); i++)
		if (m_Values[i].m_Name == Name)
      if (!m_Values[i].m_Values.IsEmpty())
			  return Bolitho::Parse<BOOL>(m_Values[i].m_Values[0]);

	throw InvalidArgumentException(this, String::Format( TEXT("The value \'%s\' cannot be found"), Name ) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
