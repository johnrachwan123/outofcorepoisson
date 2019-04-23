/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/MeshPly.hpp $
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

#include "System/Version.hpp"

namespace Bolitho
{
  namespace Graphics
  {
    namespace Internal
    {
      class PlyElement;
      
      class Ply
      {
      public:
	enum DataFormat
        {
	  Ascii,
	  BinaryBigEndian,
	  BinaryLittleEndian
	};

	Ply();
	virtual ~Ply();
	
	VOID ReadHeader(IO::Stream* pStream);
	
	CONST DataFormat& Format() CONST
	{ return m_Format; }
	DataFormat& Format()
	{ return m_Format; }
	
	CONST Bolitho::Version& Version() CONST
	{ return m_Version; }
	Bolitho::Version& Version()
	{ return m_Version; }
	
      private:
	DataFormat m_Format;
	Bolitho::Version m_Version;
	
	Array<PlyElement*> m_Elements;
      };
      
      class PlyProperty : public Object
      {
	DeclareRuntimeType();
      public:
	enum PlyPropertyType
        { 
	  Unknown, 
	  Int8, 
	  UInt8, 
	  Int16, 
	  UInt16, 
	  Int32, 
	  UInt32, 
	  Float32, 
	  Float64, 
	  List 
	}; 

	PlyProperty();
	PlyProperty(CONST String& Name, CONST String& Type);
	virtual ~PlyProperty() {}

	CONST String& Name() CONST;
	CONST String& TypeString() CONST;
	PlyPropertyType Type();
	//VARIANT& Value();
	//CONST VARIANT& Value() CONST;

	BOOL operator == (CONST PlyProperty& P) CONST
	{ return m_Name == P.m_Name; }
	BOOL operator != (CONST PlyProperty& P) CONST
	{ return m_Name != P.m_Name; }
	
	virtual UINT Size();
      protected:
	String m_Name;
	PlyPropertyType m_Type;
	String m_TypeString;
	//VARIANT m_Value;

	PlyPropertyType GetPlyPropertyType(CONST String& Type);
      };

      class PlyPropertyList : public PlyProperty
      {
	DeclareRuntimeType();
      public:
	PlyPropertyList();
	PlyPropertyList(CONST String& Name, CONST String& CountType, CONST String& ElementType);
	
	PlyPropertyType CountType();
	PlyPropertyType ElementType();
	
	virtual UINT Size()
	{ return 0; }
	
	//Array<VARIANT>& Values();
	//CONST Array<VARIANT>& Values() CONST;
	
      protected:
	PlyPropertyType m_CountType;
	PlyPropertyType m_ElementType;
	//Array<VARIANT> m_Values;
      };

      class PlyElement : public Object
      {
	DeclareRuntimeType();
      public:
	
	PlyElement();
	PlyElement(CONST String& Name, LONG Count);
	virtual ~PlyElement();
	
	VOID AddProperty(String Name, String Type);
	VOID AddPropertyList(String Name, String CountType, String ElementType);
	
	VOID Parse(Ply::DataFormat F, Bolitho::IO::Stream* pStream);
	
	BOOL operator == (CONST PlyElement& E) CONST
	{ return m_Name == E.m_Name; }
	BOOL operator != (CONST PlyElement& E) CONST
	{ return m_Name != E.m_Name; }
	
	String Name();
	UINT Count();
	UINT Size();
	
	CONST Array<PlyProperty*>& Properties();
	
	PlyProperty* Property(CONST String& Name);
      protected:
	String m_Name;
	UINT m_Count;
	Array<PlyProperty*> m_Properties;   

      };
      
    }
  }
}
