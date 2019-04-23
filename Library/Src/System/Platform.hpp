/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/Platform.hpp $
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
  namespace System
  {
    #ifdef WINDOWS 
    #undef WINDOWS 
    #endif

    struct PlatformType { enum { WINDOWS, LINUX, FREEBSD, MACOS }; };
    struct ProcessorManufacturer { enum { INTEL, AMD }; };

    #undef BIGENDIAN
    #undef LITTLEENDIAN
    struct ByteOrder { enum { BIGENDIAN, LITTLEENDIAN }; };

    class LIBRARY_API ProcessorInfo
    {
      friend class Platform;  
    public:
      
    public:
      ProcessorInfo()
      {}
      ProcessorInfo(CONST ProcessorInfo& I)
      {
        m_NumberOfProcessors = I.m_NumberOfProcessors;
        m_EstimatedClockSpeed = I.m_EstimatedClockSpeed;
        m_L1Cache = I.m_L1Cache;
        m_L2Cache = I.m_L2Cache;
        m_L3Cache = I.m_L3Cache;
        m_Type = I.m_Type;
        m_Family = I.m_Family;
        m_Model = I.m_Model;
        m_Revision = I.m_Revision;
        m_ExtendedFamily = I.m_ExtendedFamily;
        m_ExtendedModel = I.m_ExtendedModel;
        m_SerialNumber = I.m_SerialNumber;
        m_Manufacturer = I.m_Manufacturer;
      }

      UINT NumberOfProcessors()
      { return m_NumberOfProcessors; }
      UINT EstimatedClockSpeed()
      { return m_EstimatedClockSpeed; }
      UINT L1Cache()
      { return m_L1Cache; }
      UINT L2Cache()
      { return m_L2Cache; }
      UINT L3Cache()
      { return m_L3Cache; }
      DWORD Type()
      { return m_Type; }
      DWORD Family()
      { return m_Family; }
      DWORD Model()
      { return m_Model; }
      DWORD Revision()
      { return m_Revision; }
      DWORD ExtendedFamily()
      { return m_ExtendedFamily; }
      DWORD ExtendedModel()
      { return m_ExtendedModel; }
      String SerialNumber()
      { return m_SerialNumber; }
      DWORD Manufacturer()
      { return m_Manufacturer; }

      String ProcessorName();
      String ManufacturerName();

      BOOL IsCpuIdSupported();
      BOOL GetCpuIdentity();
      BOOL GetCpuSpeed();
    protected:

      LONGLONG __cdecl GetCyclesDifference (VOID (*DelayFunction)(UINT uiMS), UINT);
      static VOID Delay (UINT);
      static VOID DelayOverhead (UINT);

      UINT m_NumberOfProcessors;
      UINT m_EstimatedClockSpeed;
      UINT m_L1Cache;
      UINT m_L2Cache;
      UINT m_L3Cache;
      DWORD m_Type;
      DWORD m_Family;
      DWORD m_Model;
      DWORD m_Revision;
      DWORD m_ExtendedFamily;
      DWORD m_ExtendedModel;
      String m_SerialNumber;
      String m_Vendor;
      DWORD m_Manufacturer;

    };

    class LIBRARY_API MemoryInfo
    {
    public:
      SIZE_T TotalPhysicalMemory;
      SIZE_T FreePhysicalMemory;
      SIZE_T TotalSwapSpace;
      SIZE_T FreeSwapSpace;
      SIZE_T TotalVirtualAddressSpace;
      SIZE_T FreeVirtualAddressSpace;
      SIZE_T PageSize;
    };

    class LIBRARY_API OperatingSystemInfo
    {
    public:
      OperatingSystemInfo()
      {}

      OperatingSystemInfo(CONST OperatingSystemInfo& I)
      {
        Vendor = I.Vendor;
        Name = I.Name;
        VersionNumber = I.VersionNumber;
      }

      String Vendor;
      String Name;
      Version VersionNumber;
    };

    LIBRARY_API SIZE_T GetPointerSize();
    LIBRARY_API DWORD GetByteOrder();
    LIBRARY_API ProcessorInfo GetProcessorInfo();
    LIBRARY_API OperatingSystemInfo GetOperatingSystemInfo();
    LIBRARY_API MemoryInfo GetMemoryInfo();
    LIBRARY_API DWORD GetPlatformType();
    LIBRARY_API UINT GetMemoryPressure();
    LIBRARY_API BOOL InRemoteDesktopSession();
    LIBRARY_API SIZE_T GetPageSize();
    LIBRARY_API UINT GetPhysicalProcessorCount();
    LIBRARY_API UINT GetLogicalProcessorCount();
  }
}
