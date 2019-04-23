/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/Platform.cpp $
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

#include "Platform.hpp"
 
using namespace Bolitho;
using namespace Bolitho::System;

//----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD Bolitho::System::GetByteOrder()
{
  static USHORT ByteOrderTest = 0x0001;
  return *((BYTE*)&ByteOrderTest) == 0x01 ? ByteOrder::LITTLEENDIAN : ByteOrder::BIGENDIAN;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Bolitho::System::GetPageSize()
{
  return 4096;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
OperatingSystemInfo Bolitho::System::GetOperatingSystemInfo()
{
  OSVERSIONINFOEX Ver;
  ::ZeroMemory(&Ver, sizeof(Ver));
  Ver.dwOSVersionInfoSize = sizeof(Ver);
  ::GetVersionEx((LPOSVERSIONINFO)&Ver);

  OperatingSystemInfo OsInfo;

  OsInfo.VersionNumber = Version((USHORT)Ver.dwMajorVersion, (USHORT)Ver.dwMinorVersion, (USHORT)Ver.dwBuildNumber, 0);

  if (Ver.dwMajorVersion == 4)
    OsInfo.Name = String::Format(TEXT("Windows NT 4.0 %s"), Ver.szCSDVersion);
  else if (Ver.dwMajorVersion == 5 && Ver.dwMinorVersion == 0)
    OsInfo.Name = String::Format(TEXT("Windows 2000 %s"), Ver.szCSDVersion);
  else if (Ver.dwMajorVersion == 5 && Ver.dwMinorVersion == 1)
    OsInfo.Name = String::Format(TEXT("Windows XP %s"), Ver.szCSDVersion);
  else if (Ver.dwMajorVersion == 5 && Ver.dwMinorVersion == 2 && Ver.wProductType == 0)
    OsInfo.Name = String::Format(TEXT("Windows XP x64 %s"), Ver.szCSDVersion);
  else if (Ver.dwMajorVersion == 5 && Ver.dwMinorVersion == 2)
  {
#if PLATFORM_WINDOWS >= PLATFORM_WINDOWS_XP
    if (GetSystemMetrics(SM_SERVERR2) == SM_SERVERR2)
      OsInfo.Name = String::Format(TEXT("Windows Server 2003 %s"), Ver.szCSDVersion);
    else
      OsInfo.Name = String::Format(TEXT("Windows Server 2003 R2 %s"), Ver.szCSDVersion);
#endif
  }
  else if (Ver.dwMajorVersion == 6 && Ver.dwMinorVersion == 0)
  {
#if defined(PLATFORM_64BIT)
    OsInfo.Name = String::Format(TEXT("Windows Vista %s x64"), Ver.szCSDVersion);
#endif

#if defined(PLATFORM_32BIT) && PLATFORM_WINDOWS >= PLATFORM_WINDOWS_XP
    BOOL IsWow64;
    IsWow64Process(GetCurrentProcess(), &IsWow64);

    if (IsWow64)
      OsInfo.Name = String::Format(TEXT("Windows Vista %s x64"), Ver.szCSDVersion);
    else
      OsInfo.Name = String::Format(TEXT("Windows Vista %s"), Ver.szCSDVersion);
#endif
  }
  else  
    OsInfo.Name = TEXT("Unknown");
  OsInfo.Vendor = TEXT("Microsoft");

  return OsInfo;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD Bolitho::System::GetPlatformType()
{
#if defined(PLATFORM_WINDOWS)
  return PlatformType::WINDOWS;
#elif defined(PLATFORM_LINUX)
  return PlatformType::LINUX;
#elif defined(PLATFORM_FREEBSD)
  return PlatformType::FREEBSD;
#elif defined(PLATFORM_MACOS)
  return PlatformType::MACOS;
#endif
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Bolitho::System::GetPointerSize()
{
  return sizeof(PVOID);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
MemoryInfo Bolitho::System::GetMemoryInfo()
{
  MEMORYSTATUSEX Mem;
  SYSTEM_INFO Info;
  ZeroMemory(&Mem, sizeof(Mem));
  ZeroMemory(&Info, sizeof(Info)); 
  Mem.dwLength = sizeof(Mem);
  GlobalMemoryStatusEx(&Mem);
  GetSystemInfo(&Info);

  MemoryInfo MemInfo;

  MemInfo.TotalPhysicalMemory = (SIZE_T)Mem.ullTotalPhys;
  MemInfo.FreePhysicalMemory = (SIZE_T)Mem.ullAvailPhys;
  MemInfo.TotalSwapSpace = (SIZE_T)Mem.ullTotalPageFile;
  MemInfo.FreeSwapSpace = (SIZE_T)Mem.ullAvailPageFile;
  MemInfo.TotalVirtualAddressSpace = (SIZE_T)Mem.ullTotalVirtual;
  MemInfo.FreeVirtualAddressSpace = (SIZE_T)Mem.ullAvailVirtual;
  MemInfo.PageSize = (SIZE_T)Info.dwPageSize;

  return MemInfo;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
ProcessorInfo Bolitho::System::GetProcessorInfo()
{
  ProcessorInfo Info;

  if (Info.IsCpuIdSupported())
  {
    Info.GetCpuIdentity();
    Info.GetCpuSpeed();
  }

  return Info;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL ProcessorInfo::IsCpuIdSupported()
{
#if defined(ARCH_AMD64)
  return false;
#else
  __try 
  {
    _asm 
    {
      push eax
        push ebx
        push ecx
        push edx

        mov eax, 0
        cpuid

        pop edx
        pop ecx
        pop ebx
        pop eax
    }
  }

  __except (1)
  {
    return false;
  }
  return true;
#endif
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL ProcessorInfo::GetCpuIdentity()
{
#if defined(ARCH_AMD64)
  return false;
#else
  INT CPUVendor[3];
  INT CPUSignature;

  __try 
  {
    _asm 
    {

      push eax
        push ebx
        push ecx
        push edx

        ; <<CPUID>>
        ; eax = 0 --> eax: maximum value of CPUID instruction.
        ;			  ebx: part 1 of 3; CPU signature.
        ;			  edx: part 2 of 3; CPU signature.
        ;			  ecx: part 3 of 3; CPU signature.
        mov eax, 0
        cpuid

        mov CPUVendor[0 * TYPE int], ebx
        mov CPUVendor[1 * TYPE int], edx
        mov CPUVendor[2 * TYPE int], ecx

        ; <<CPUID>> 
        ; eax = 1 --> eax: CPU ID - bits 31..16 - unused, bits 15..12 - type, bits 11..8 - family, bits 7..4 - model, bits 3..0 - mask revision
        ;			  ebx: 31..24 - default APIC ID, 23..16 - logical processsor ID, 15..8 - CFLUSH chunk size , 7..0 - brand ID
        ;			  edx: CPU feature flags
        mov eax,1
        cpuid
        mov CPUSignature, eax

        pop edx
        pop ecx
        pop ebx
        pop eax

    }
  }

  __except (1) 
  {
    return false;
  }

  m_Vendor = TEXT("0123456789abcdef");
  // Process the returned information.
  memcpy (&(m_Vendor[0]), &(CPUVendor[0]), sizeof (int));
  memcpy (&(m_Vendor[4]), &(CPUVendor[1]), sizeof (int));
  memcpy (&(m_Vendor[8]), &(CPUVendor[2]), sizeof (int));
  m_Vendor[12] = '\0';

  // Attempt to retrieve the manufacturer from the vendor string.
  if (m_Vendor == TEXT("GenuineIntel"))		
    m_Manufacturer = ProcessorManufacturer::INTEL;
  else if (m_Vendor == TEXT("AuthenticAMD"))
    m_Manufacturer = ProcessorManufacturer::AMD;
  else if (m_Vendor == TEXT("AMD ISBETTER")) 
    m_Manufacturer = ProcessorManufacturer::AMD;

  // Retrieve the family of CPU present.
  m_ExtendedFamily =  ((CPUSignature & 0x0FF00000) >> 20);	// Bits 27..20 Used
  m_ExtendedModel =		((CPUSignature & 0x000F0000) >> 16);	// Bits 19..16 Used
  m_Type =				    ((CPUSignature & 0x0000F000) >> 12);	// Bits 15..12 Used
  m_Family =				  ((CPUSignature & 0x00000F00) >> 8);		// Bits 11..8 Used
  m_Model =				    ((CPUSignature & 0x000000F0) >> 4);		// Bits 7..4 Used
  m_Revision =			  ((CPUSignature & 0x0000000F) >> 0);		// Bits 3..0 Used

  return true;
#endif
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL ProcessorInfo::GetCpuSpeed()
{	
  UINT Repetitions = 1;
  UINT MSecPerRepetition = 50;
  INT64	Total = 0, Overhead = 0;

  for (UINT i = 0; i < Repetitions; i++)
  {
    Total += GetCyclesDifference (ProcessorInfo::Delay, MSecPerRepetition);
    Overhead += GetCyclesDifference (ProcessorInfo::DelayOverhead, MSecPerRepetition);
  }

  // Calculate the MHz speed.
  Total -= Overhead;
  Total /= Repetitions;
  Total /= MSecPerRepetition;
  Total /= 1000;

  // Save the CPU speed.
  m_EstimatedClockSpeed = (UINT)Total;

  return true;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
INT64 __cdecl ProcessorInfo::GetCyclesDifference (VOID (*DelayFunction)(UINT uiMS), UINT uiParameter)
{
#if defined(ARCH_AMD64)
  return 0;
#else
  unsigned int edx1, eax1;
  unsigned int edx2, eax2;

  // Calculate the frequency of the CPU instructions.
  __try
  {
    _asm 
    {
      push uiParameter		; push parameter param
        mov ebx, DelayFunction	; store func in ebx

        rdtsc

        mov esi, eax			; esi = eax
        mov edi, edx			; edi = edx

        call ebx				; call the delay functions

        rdtsc

        pop ebx

        mov edx2, edx			; edx2 = edx
        mov eax2, eax			; eax2 = eax

        mov edx1, edi			; edx2 = edi
        mov eax1, esi			; eax2 = esi
    }
  }

  __except (1)
  {
    return -1;
  }

  return (((INT64) edx2 << 32) + eax2) - (((INT64) edx1 << 32) + eax1);

#endif

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ProcessorInfo::Delay (UINT uiMS)
{
  LARGE_INTEGER Frequency, StartCounter, EndCounter;
  __int64 x;

  // Get the frequency of the high performance counter.
  if (!QueryPerformanceFrequency (&Frequency)) return;
  x = Frequency.QuadPart / 1000 * uiMS;

  // Get the starting position of the counter.
  QueryPerformanceCounter (&StartCounter);

  do {
    // Get the ending position of the counter.	
    QueryPerformanceCounter (&EndCounter);
  } while (EndCounter.QuadPart - StartCounter.QuadPart < x);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ProcessorInfo::DelayOverhead (UINT uiMS)
{
  LARGE_INTEGER Frequency, StartCounter, EndCounter;
  __int64 x;

  // Get the frequency of the high performance counter.
  if (!QueryPerformanceFrequency (&Frequency)) return;
  x = Frequency.QuadPart / 1000 * uiMS;

  // Get the starting position of the counter.
  QueryPerformanceCounter (&StartCounter);

  do {
    // Get the ending position of the counter.	
    QueryPerformanceCounter (&EndCounter);
  } while (EndCounter.QuadPart - StartCounter.QuadPart == x);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String ProcessorInfo::ProcessorName()
{
  switch (m_Manufacturer) 
  {
  case ProcessorManufacturer::INTEL:
    switch (m_Family) 
    {
    case 6:
      switch (m_Model) 
      {
      case 3:   return "Pentium II"; 
      case 5:   return "Pentium II";
      case 6:   return "Pentium II";
      case 7:   return "Pentium III";
      case 8:   return "Pentium III";
      case 0xa: return "Pentium III";
      case 0xb: return "Pentium III";
      case 0xc: return "";
      case 0xd: return "Pentium M";
      default:  return "Unknown P6";
      }
      break;
    case 0xf:
      switch (m_ExtendedFamily) 
      {
      case 0:
        switch (m_Model) 
        {
        case 0:  return "Pentium 4";
        case 1:  return "Pentium 4";
        case 2:  return "Pentium 4";
        default: return "Pentium 4";
        }
        break;
      default:
        return "Unknown";
      }
      break;
    };
  case ProcessorManufacturer::AMD:
    return "Unknown";
  };

  return "Unknown";
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String ProcessorInfo::ManufacturerName()
{
  switch(m_Manufacturer)
  {
  case ProcessorManufacturer::INTEL:
    return "Intel";
  case ProcessorManufacturer::AMD:
    return "AMD";
  default:
    return "Unknown";
  };
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
UINT Bolitho::System::GetMemoryPressure()
{
  MemoryInfo MemInfo = System::GetMemoryInfo();

  return (UINT)(100 * (MemInfo.TotalPhysicalMemory - MemInfo.FreePhysicalMemory) / MemInfo.TotalPhysicalMemory);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Bolitho::System::InRemoteDesktopSession()
{
  return GetSystemMetrics(SM_REMOTESESSION) != 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
UINT Bolitho::System::GetPhysicalProcessorCount()
{
  return 1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
UINT Bolitho::System::GetLogicalProcessorCount()
{
  return 1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
