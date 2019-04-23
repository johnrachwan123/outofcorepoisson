/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/IO/TextStreamImpl.hpp $
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





//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementTemplateRuntimeType(Bolitho::IO,TTextStream,Stream);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class CharType>
TTextStream<CharType>::TTextStream()
{}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class CharType>
TTextStream<CharType>::TTextStream(Ptr<Stream> pBaseStream)
{
  m_pStream = pBaseStream;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class CharType>
TTextStream<CharType>& TTextStream<CharType>::operator= (Ptr<Stream> pBaseStream)
{
	m_pStream = pBaseStream;
	return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class CharType>
TTextStream<CharType>::~TTextStream()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class CharType>
VOID TTextStream<CharType>::Flush()
{
  if (m_pStream)
    m_pStream->Flush();
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class CharType>
VOID TTextStream<CharType>::Close()
{
  if (m_pStream)
    m_pStream->Close();
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class CharType>
SIZE_T TTextStream<CharType>::Write(LPCVOID pData, SIZE_T N)
{
  if (m_pStream)
    return m_pStream->Write(pData,N);
  else
    return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class CharType>
SIZE_T TTextStream<CharType>::Read(LPVOID pData, SIZE_T N)
{
  if (m_pStream)
    return m_pStream->Read(pData,N);
  else
    return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class CharType>
BOOL TTextStream<CharType>::EndOfStream()
{
  if (m_pStream)
    return m_pStream->EndOfStream();
  else
    return true;
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class CharType>
VOID TTextStream<CharType>::Write(CONST CharType* Format, ...)
{
  if (Format == 0)
    return;

  va_list Args;
  va_start(Args,Format);
  TString<CharType> Str = TString<CharType>::FormatV(Format, Args);
  va_end(Args);
  Write((LPCVOID)Str.ConstStr(), sizeof(CharType)*Str.Length());
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class CharType>
VOID TTextStream<CharType>::WriteLine(CONST CharType* Format, ...)
{
  if (Format != 0)
	{
		va_list Args;
		va_start(Args,Format);
		TString<CharType> Str = TString<CharType>::FormatV(Format, Args);
		va_end(Args);
		Write((LPCVOID)Str.ConstStr(), sizeof(CharType)*Str.Length());
	}
  Write((LPCVOID)TEXT("\r\n"), 2*sizeof(CharType)); 
}
//-----------------------------------------------------------------------------------------------------------------------------
template<class CharType>
TString<CharType> TTextStream<CharType>::ReadLine()
{
  String Str;
  CharType B;
  while (Read(&B, sizeof(CharType)) && B != '\n')
    if (B != '\r')
      Str += B;
  return Str;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class CharType>
inline VOID TTextStream<CharType>::Write(CONST TString<CharType>& x)
{
  Write("%s", x.ConstStr());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class CharType>
inline VOID TTextStream<CharType>::WriteLine(CONST TString<CharType>& x)
{
  WriteLine("%s", x.ConstStr());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class CharType>
template<class T>
VOID TTextStream<CharType>::Write(CONST T& x)
{
  TString<CharType> S = Bolitho::ToString(x);
  Write((LPCVOID)S.ConstStr(), sizeof(CharType)*S.Length());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class CharType>
template<class T>
VOID TTextStream<CharType>::WriteLine(CONST T& x)
{
  TString<CharType> S = Bolitho::ToString(x);
  Write((LPCVOID)S.ConstStr(), sizeof(CharType)*S.Length());
  Write((LPCVOID)TEXT("\r\n"), sizeof(CHAR));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
