/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/Registry.hpp $
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

namespace Bolitho
{
  namespace System
  {
		class LIBRARY_API RegistryKey : public Object
    {
    public:
      /* A class to provide enumeration of a set of subkeys or values */
      class LIBRARY_API Enumerator
      {
        
      
      };

      /* A RegistryKey object corresponding to the HKEY_LOCAL_MACHINE registry key */
      static RegistryKey LocalMachine;
      /* A RegistryKey object corresponding to the HKEY_CURRENT_USER registry key */
      static RegistryKey CurrentUser;

      virtual ~RegistryKey();

      /* Closes the key and frees all OS resources */
      VOID Close();

      /* Creates a new subkey */
      Ptr<RegistryKey> CreateSubkey(LPCTSTR Name, REGSAM Access);
      /* Creates a new subkey.  Failures return NULL pointer */
      Ptr<RegistryKey> TryCreateSubkey(LPCTSTR Name, REGSAM Access);
      /* Opens a subkey */
      Ptr<RegistryKey> OpenSubkey(LPCTSTR Name, REGSAM Access);
      /* Opens a subkey.  Failures return NULL pointer */
      Ptr<RegistryKey> TryOpenSubkey(LPCTSTR Name, REGSAM Access);

      /* Deletes a subkey */
      VOID DeleteSubKey(LPCTSTR Name);
      /* Delete a value key */
      VOID DeleteValue(LPCTSTR Name);

      /* Sets the value of a binary value key */
      VOID SetValue(LPCTSTR Name, LPCVOID Data, SIZE_T Length);
      /* Sets the value of a binary value key */
      VOID SetValue(LPCTSTR Name, CONST Buffer<BYTE>& Value);
      /* Sets the value of a DWORD value key */
      VOID SetValue(LPCTSTR Name, DWORD Value);
      /* Sets the value of a string value key */
      VOID SetValue(LPCTSTR Name, LPCTSTR Value);

      /* Gets the value of a binary value key */
      BOOL GetValue(LPCTSTR Name, Buffer<BYTE>& Value);
      /* Gets the value of a DWORD value key */
      BOOL GetValue(LPCTSTR Name, DWORD& Value);
      /* Gets the value of a string value key */
      BOOL GetValue(LPCTSTR Name, String& Value);

      /* Returns the OS handle to the key */
      HKEY Handle() CONST;
    protected:
      RegistryKey(HKEY hKey);
    
      /* The OS handle to the key */
      HKEY m_hKey;
	  };
  }
}