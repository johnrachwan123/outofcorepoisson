/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Com/ComPointerImpl.hpp $
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
ComPtr<T>::ComPtr()
{
  m_pInterface = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
ComPtr<T>::ComPtr(T* pInterface)
{
  m_pInterface = pInterface;
  if (m_pInterface)
    m_pInterface->AddRef();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
ComPtr<T>::ComPtr(ComPtr<T>& P)
{
  m_pInterface = P.m_pInterface;
  if (m_pInterface)
    m_pInterface->AddRef();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
ComPtr<T>::~ComPtr()
{
  if (m_pInterface)
    m_pInterface->Release();
  m_pInterface = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
ComPtr<T>& ComPtr<T>::operator= (T* pInterface)
{
  if (m_pInterface)
    m_pInterface->Release();
  m_pInterface = pInterface;
  if (m_pInterface)
    m_pInterface->AddRef();
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
ComPtr<T>& ComPtr<T>::operator= (ComPtr<T>& P)
{
  if (m_pInterface)
    m_pInterface->Release();
  m_pInterface = P.m_pInterface;
  if (m_pInterface)
    m_pInterface->AddRef();
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
ComPtr<T>::operator T* () CONST
{
  return m_pInterface;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T* ComPtr<T>::operator-> ()
{
  return m_pInterface;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T** ComPtr<T>::operator& ()
{
  Assert(m_pInterface == 0);
  return &m_pInterface;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
HRESULT ComPtr<T>::CreateInstance(CONST IID& ClassId, IUnknown* pOuterClass, DWORD ClassContext)
{
  return CoCreateInstance(ClassId, pOuterClass, ClassContext, __uuidof(T), (VOID**)&m_pInterface);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
HRESULT ComPtr<T>::CreateInstance(LPCTSTR ProgramId, IUnknown* pOuterClass, DWORD ClassContext)
{
  return E_FAIL;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
template<class U>
HRESULT ComPtr<T>::QueryInterface(ComPtr<U> pOtherInterface)
{
  return pOtherInterface->QueryInterface(__uuidof(T), (VOID**)&m_pInterface);
}

