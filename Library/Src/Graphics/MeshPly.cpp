/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/MeshPly.cpp $
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




#include "CodeLibrary.hpp"

#include "Graphics/MeshPly.hpp"
#include "IO/TextStream.hpp"

using namespace Bolitho::Graphics::Internal;
using namespace Bolitho::IO;
using namespace Bolitho;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Graphics::Internal,PlyProperty,Object);
ImplementRuntimeType(Bolitho::Graphics::Internal,PlyPropertyList,PlyProperty);
ImplementRuntimeType(Bolitho::Graphics::Internal,PlyElement,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
PlyProperty::PlyProperty()
{ 
  m_Name = ""; 
  m_Type = Unknown;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
PlyProperty::PlyProperty(CONST String& Name, CONST String& Type)
{ 
  m_Name = Name; 
  m_Type = GetPlyPropertyType(Type);
  m_TypeString = Type;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
CONST String& PlyProperty::Name() CONST
{ 
  return m_Name; 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
CONST String& PlyProperty::TypeString() CONST
{ 
  return m_TypeString; 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//VARIANT& PlyProperty::Value()
//{ 
//  return m_Value; 
//}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//CONST VARIANT& PlyProperty::Value() CONST
//{ 
//  return m_Value; 
//}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
PlyProperty::PlyPropertyType PlyProperty::Type()
{ 
  return m_Type; 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
PlyProperty::PlyPropertyType PlyProperty::GetPlyPropertyType(CONST String& Type)
{
  PlyPropertyType T = Unknown;
  if (Type == "int8" || Type == "char")
    T = Int8;
  if (Type == "int16" || Type == "short")
    T = Int16;
  if (Type == "int32" || Type == "int")
    T = Int32;

  if (Type == "uint8" || Type == "uchar")
    T = UInt8;
  if (Type == "uint16" || Type == "ushort")
    T = UInt16;
  if (Type == "uint32" || Type == "uint")
    T = UInt32;

  if (Type == "float32" || Type == "float")
    T = Float32;
  if (Type == "float64" || Type == "double")
    T = Float64;

  return T;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
UINT PlyProperty::Size()
{
  switch (m_Type)
  {
  case Int8:
  case UInt8:
    return 1;
  case Int16:
  case UInt16:
    return 2;
  case Int32:
  case UInt32:
    return 4;
  case Float32:
    return 4;
  case Float64:
    return 8;
  default:
    return 0;
  };
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
PlyPropertyList::PlyPropertyList()
{ 
  m_Name = ""; 
  m_Type = Unknown;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
PlyPropertyList::PlyPropertyList(CONST String& Name, CONST String& CountType, CONST String& ElementType)
{ 
  m_Name = Name;
  m_Type = List;
  m_CountType = GetPlyPropertyType(CountType); 
  m_ElementType = GetPlyPropertyType(ElementType);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
PlyProperty::PlyPropertyType PlyPropertyList::CountType()
{ 
  return m_CountType; 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
PlyProperty::PlyPropertyType PlyPropertyList::ElementType()
{ 
  return m_ElementType; 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//Array<VARIANT>& PlyPropertyList::Values()
//{
//  return m_Values;
//}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//CONST Array<VARIANT>& PlyPropertyList::Values() CONST
//{
//  return m_Values;
//}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
PlyElement::PlyElement()
{ 
  m_Name = ""; 
  m_Count = 0; 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
PlyElement::~PlyElement()
{ 
  for (SIZE_T i=0; i<m_Properties.Length(); i++)
    delete m_Properties[i];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
PlyElement::PlyElement(CONST String& Name, LONG Count)
{ 
  m_Name = Name; 
  m_Count = Count; 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID PlyElement::AddProperty(String Name, String Type)
{ 
  m_Properties.Add(new PlyProperty(Name, Type)); 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID PlyElement::AddPropertyList(String Name, String CountType, String ElementType)
{ 
  m_Properties.Add(new PlyPropertyList(Name, CountType, ElementType)); 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String PlyElement::Name()
{ 
  return m_Name; 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
UINT PlyElement::Count()
{ 
  return m_Count; 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
UINT PlyElement::Size()
{
  UINT S = 0;
  for (SIZE_T i=0; i<m_Properties.Length(); i++)
  {
    UINT s = m_Properties[i]->Size();
    if (s == 0)
      return 0;
    else
      S += s;
  }
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
CONST Array<PlyProperty*>& PlyElement::Properties()
{ 
  return m_Properties; 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
PlyProperty* PlyElement::Property(CONST String& Name)
{
  for(SIZE_T i=0; i<m_Properties.Length(); i++)
    if (m_Properties[i]->Name() == Name)
      return m_Properties[i];
  return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID PlyElement::Parse(Ply::DataFormat F, Stream* pStream)
{
  if (F == Ply::Ascii)
  {
    TextStream S = TextStream(pStream);
    Array<String> V = S.ReadLine().Split(&String::IsWhitespace);    

    SIZE_T j=0;
    for(SIZE_T i=0; i<m_Properties.Length(); i++)
    {
        /*
      switch(m_Properties[i]->Type())
      {
      case PlyProperty::Float32:
        m_Properties[i]->Value().Float = Bolitho::Parse<FLOAT>(V[j++]);
        break;
      case PlyProperty::Float64:
        m_Properties[i]->Value().Double = Bolitho::Parse<DOUBLE>(V[j++]);
        break;
      case PlyProperty::UInt8:
        m_Properties[i]->Value().UInt8 = Bolitho::Parse<UINT8>(V[j++]);
        break;
      case PlyProperty::Int8:
        m_Properties[i]->Value().Int8 = Bolitho::Parse<INT8>(V[j++]);
        break;
      case PlyProperty::UInt16:
        m_Properties[i]->Value().UInt16 = Bolitho::Parse<UINT16>(V[j++]);
        break;
      case PlyProperty::Int16:
        m_Properties[i]->Value().Int16 = Bolitho::Parse<INT16>(V[j++]);
        break;
      case PlyProperty::UInt32:
        m_Properties[i]->Value().UInt32 = Bolitho::Parse<UINT32>(V[j++]);
        break;
      case PlyProperty::Int32:
        m_Properties[i]->Value().Int32 = Bolitho::Parse<INT32>(V[j++]);
        break;
      case PlyProperty::List:
        {
          PlyPropertyList* pList = (PlyPropertyList*)m_Properties[i];
          SIZE_T N = Bolitho::Parse<SIZE_T>(V[j++]);
          pList->Values().Resize(N);
          for (SIZE_T k=0; k<N; k++)
          {
            switch(pList->ElementType())
            {
            case PlyProperty::Float32:
              pList->Values()[k].Float = Bolitho::Parse<FLOAT>(V[j++]);
              break;
            case PlyProperty::Float64:
              pList->Values()[k].Double = Bolitho::Parse<DOUBLE>(V[j++]);
              break;
            case PlyProperty::UInt8:
              pList->Values()[k].UInt8 = Bolitho::Parse<UINT8>(V[j++]);
              break;
            case PlyProperty::Int8:
              pList->Values()[k].Int8 = Bolitho::Parse<INT8>(V[j++]);
              break;
            case PlyProperty::UInt16:
              pList->Values()[k].UInt16 = Bolitho::Parse<UINT16>(V[j++]);
              break;
            case PlyProperty::Int16:
              pList->Values()[k].Int16 = Bolitho::Parse<INT16>(V[j++]);
              break;
            case PlyProperty::UInt32:
              pList->Values()[k].UInt32 = Bolitho::Parse<UINT32>(V[j++]);
              break;
            case PlyProperty::Int32:
              pList->Values()[k].Int32 = Bolitho::Parse<INT32>(V[j++]);
              break;
            };
          }
        }
        break;
      };
        */
    }
  }
  else if (F == Ply::BinaryBigEndian)
  {
    
  }
  else if (F == Ply::BinaryLittleEndian)
  {


  }

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ply::Ply()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ply::~Ply()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
