/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Types.hpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1697 $
 * Last Updated: $Date: 2008-10-31 06:36:06 -0700 (Fri, 31 Oct 2008) $
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

#if defined(PLATFORM_POSIX)

typedef float               FLOAT;
typedef double              DOUBLE;

typedef ptrdiff_t           SSIZE_T;
typedef size_t              SIZE_T;

typedef int                 BOOL;
typedef unsigned char       BYTE;

typedef short               SHORT;
typedef unsigned short      USHORT;
typedef unsigned short      WORD;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        DWORD;

typedef long                LONG;
typedef unsigned long       ULONG;
typedef long long           LONGLONG;
typedef unsigned long long  ULONGLONG;

typedef signed char         INT8;
typedef unsigned char       UINT8;
typedef signed short        INT16;
typedef unsigned short      UINT16;
typedef signed int          INT32;
typedef unsigned int        UINT32;
typedef signed long long    INT64;
typedef unsigned long long  UINT64;

typedef const void*         LPCVOID;
typedef void*               LPVOID;
typedef void*  		    PVOID;

typedef int                 *PBOOL;
typedef unsigned char       *PBYTE;
typedef const unsigned char *LPCBYTE;
typedef unsigned char       *LPBYTE;


typedef short               *PSHORT;
typedef unsigned short      *PUSHORT;
typedef unsigned short      *PWORD;

typedef int                 *PINT;
typedef unsigned int        *PUINT;
typedef unsigned int        *PDWORD;

typedef long                *PLONG;
typedef long long           *PLONGLONG;

typedef float		    *PFLOAT;
typedef double		    *PDOUBLE;

typedef char                CHAR;
typedef wchar_t             WCHAR;

typedef char                TCHAR;
typedef unsigned char       UCHAR;

typedef CHAR*               LPSTR;
typedef const CHAR*         LPCSTR;

typedef WCHAR*              LPWSTR;
typedef const WCHAR*        LPCWSTR;

typedef TCHAR*              LPTSTR;
typedef const TCHAR*        LPCTSTR;

typedef signed char         *PINT8;
typedef unsigned char       *PUINT8;
typedef signed short        *PINT16;
typedef unsigned short      *PUINT16;
typedef signed int          *PINT32;
typedef unsigned int        *PUINT32;
typedef signed long long    *PINT64;
typedef unsigned long long  *PUINT64;

typedef LONG                LPARAM;

#if defined(PLATFORM_64BIT)
typedef INT64 INT_PTR, *PINT_PTR;
typedef UINT64 UINT_PTR, *PUINT_PTR;
typedef INT64 LONG_PTR, *PLONG_PTR;
typedef UINT64 ULONG_PTR, *PULONG_PTR;

#else
typedef INT INT_PTR, *PINT_PTR;
typedef UINT UINT_PTR, *PUINT_PTR;
typedef LONG LONG_PTR, *PLONG_PTR;
typedef ULONG ULONG_PTR, *PULONG_PTR;
#endif

typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;


#define VOID void
#define CONST const

#define TRUE 1
#define FALSE 0

#define WINAPI 

typedef pthread_t THREADHANDLE;
typedef pthread_mutex_t MUTEXHANDLE;
typedef VOID* EVENTHANDLE;
typedef VOID* MODULEHANDLE;
typedef pid_t PROCESSHANDLE;
typedef INT FILEHANDLE;
typedef INT PIPEHANDLE;
typedef pthread_mutex_t CRITICALSECTIONHANDLE;
typedef pthread_cond_t CONDITIONVARIABLEHANDLE;
typedef INT MEMORYMAPPEDFILEHANDLE;
typedef sem_t SEMAPHOREHANDLE;
typedef INT SOCKETHANDLE;

typedef DWORD (*PTHREAD_START_ROUTINE)(LPVOID);
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;

#define INFINITE 0xFFFFFFFF

#define LOWORD(l) ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l) ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff)) 

#endif

#if defined(PLATFORM_WINDOWS)

typedef HANDLE THREADHANDLE;
typedef HANDLE MUTEXHANDLE;
typedef HANDLE EVENTHANDLE;
typedef HANDLE PROCESSHANDLE;
typedef HMODULE MODULEHANDLE;
typedef HANDLE FILEHANDLE;
typedef HANDLE PIPEHANDLE;
typedef CRITICAL_SECTION CRITICALSECTIONHANDLE;

#if defined(USE_SOCKETS)
typedef SOCKET SOCKETHANDLE;
#endif

#if PLATFORM_WINDOWS >= PLATFORM_WINDOWS_VISTA
typedef CONDITION_VARIABLE CONDITIONVARIABLEHANDLE;
#endif

typedef HANDLE MEMORYMAPPEDFILEHANDLE;
typedef HANDLE SEMAPHOREHANDLE;
#endif

typedef DOUBLE               *PDOUBLE;
typedef SSIZE_T              PTRDIFF_T;
typedef FLOAT                *LPFLOAT;

#define MAKELONGLONG(lo,hi) ((LONGLONG)lo + ((LONGLONG)hi << 32))
#define MAKEULONGLONG(lo,hi) ((ULONGLONG)lo + ((ULONGLONG)hi << 32))

#if defined(PLATFORM_POSIX) || PLATFORM_WINDOWS < PLATFORM_WINDOWS_VISTA

#define MAXUINT8    ((UINT8)~((UINT8)0))
#define MAXINT8     ((INT8)(MAXUINT8 >> 1))
#define MININT8     ((INT8)~MAXINT8)

#define MAXUINT16   ((UINT16)~((UINT16)0))
#define MAXINT16    ((INT16)(MAXUINT16 >> 1))
#define MININT16    ((INT16)~MAXINT16)

#define MAXUINT32   ((UINT32)~((UINT32)0))
#define MAXINT32    ((INT32)(MAXUINT32 >> 1))
#define MININT32    ((INT32)~MAXINT32)

#define MAXUINT64   ((UINT64)~((UINT64)0))
#define MAXINT64    ((INT64)(MAXUINT64 >> 1))
#define MININT64    ((INT64)~MAXINT64)

#define MAXULONG32  ((ULONG32)~((ULONG32)0))
#define MAXLONG32   ((LONG32)(MAXULONG32 >> 1))
#define MINLONG32   ((LONG32)~MAXLONG32)

#define MAXULONG64  ((ULONG64)~((ULONG64)0))
#define MAXLONG64   ((LONG64)(MAXULONG64 >> 1))
#define MINLONG64   ((LONG64)~MAXLONG64)

#define MAXULONGLONG ((ULONGLONG)~((ULONGLONG)0))
#define MINLONGLONG ((LONGLONG)~MAXLONGLONG)

#define MAXSIZE_T   ((SIZE_T)~((SIZE_T)0))
#define MAXSSIZE_T  ((SSIZE_T)(MAXSIZE_T >> 1))
#define MINSSIZE_T  ((SSIZE_T)~MAXSSIZE_T)

#define MAXUINT     ((UINT)~((UINT)0))
#define MAXINT      ((INT)(MAXUINT >> 1))
#define MININT      ((INT)~MAXINT)

#define MAXULONG    ((ULONG)~((ULONG)0))
#undef MAXLONG
#undef MINLONG
#define MAXLONG     ((LONG)(MAXULONG >> 1))
#define MINLONG     ((LONG)~MAXLONG)

#define MAXDWORD32  ((DWORD32)~((DWORD32)0))
#define MAXDWORD64  ((DWORD64)~((DWORD64)0))

#ifndef LLONG_MAX
#define LLONG_MAX    9223372036854775807LL 
#define LLONG_MIN    (-LLONG_MAX - 1LL) 
#define ULLONG_MAX   18446744073709551615ULL 
#endif

#endif

#if defined(PLATFORM_MACOS) || defined(PLATFORM_FREEBSD)
#undef MAXFLOAT
#endif

#define MINFLOAT   1.175494351e-38F
#define MAXFLOAT   3.402823466e+38F
#define MINDOUBLE  2.2250738585072014e-308
#define MAXDOUBLE  1.7976931348623158e+308

#define INFINITYFLOAT 1.0e+128F
#define INFINITYDOUBLE (MAXDOUBL+1)

#define INLINE inline
#define STATIC static
#define VOLATILE volatile

#if defined(COMPILER_NVCC)
  #define DEVICE __device__
  #define GLOBAL __global__
  #define HOST __host__
  #define CONSTANT __constant__
  #define SHARED __shared__
#endif
