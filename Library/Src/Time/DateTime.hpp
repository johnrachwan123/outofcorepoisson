/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Time/DateTime.hpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 1744 $
 * Last Updated: $Date: 2008-12-27 13:09:28 -0800 (Sat, 27 Dec 2008) $
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
struct SYSTEMTIME 
{
  WORD wYear;
  WORD wMonth;
  WORD wDayOfWeek;
  WORD wDay;
  WORD wHour;
  WORD wMinute;
  WORD wSecond;
  WORD wMilliseconds;
};

struct FILETIME 
{
  DWORD dwLowDateTime;
  DWORD dwHighDateTime;
};

LONG GetTickCount();
VOID GetSystemTime(SYSTEMTIME* pTime);
VOID GetLocalTime(SYSTEMTIME* pTime);
VOID GetSystemTimeAsFileTime(FILETIME* pTime);
VOID FileTimeToSystemTime(FILETIME* pFileTime, SYSTEMTIME* pSystemTime);
VOID SystemTimeToFileTime(SYSTEMTIME* pFileTime, FILETIME* pSystemTime);

#endif

namespace Bolitho
{
  class DateTime;

  /* Repesents the time between two DateTime objects */
  class LIBRARY_API TimeSpan : public Object
  {
    friend class DateTime;

    DeclareRuntimeType();
  public:
    /* Constructs a zero TimeSpan */
    TimeSpan();
    /* Constructs a TimeSpan of 'Ticks' time ticks */
    TimeSpan(LONGLONG Ticks);

    /* Constructs a TimeSpan from a FILETIME */
    TimeSpan(FILETIME T);

    /* Computes the sum of two TimeSpans */
    TimeSpan operator+ (CONST TimeSpan& T);
    /* Computes the difference of two TimeSpans */
    TimeSpan operator- (CONST TimeSpan& T);

    /* Returns the number of ticks in the TimeSpan */
    LONGLONG Ticks() CONST;
    
    /* Returns the number of nanoseconds the TimeSpan corresponds to */
    FLOAT TotalNanoSeconds() CONST;
    /* Returns the number of microseconds the TimeSpan corresponds to */
    FLOAT TotalMicroSeconds() CONST;
    /* Returns the number of milliseconds the TimeSpan corresponds to */
    FLOAT TotalMilliSeconds() CONST;
    /* Returns the number of seconds the TimeSpan corresponds to */
    FLOAT TotalSeconds() CONST;
    /* Returns the number of minutes the TimeSpan corresponds to */
    FLOAT TotalMinutes() CONST;
    /* Returns the number of hours the TimeSpan corresponds to */
    FLOAT TotalHours() CONST;
    /* Returns the number of days the TimeSpan corresponds to */
    FLOAT TotalDays() CONST;

  protected:
    /* The number of time ticks in the TimeSpan */
    LONGLONG m_Ticks;
  };

  IO::BinaryStream& operator << (IO::BinaryStream& S, TimeSpan x);
  IO::BinaryStream& operator >> (IO::BinaryStream& S, TimeSpan& x);
  
  /* Represents a date and time */
  class LIBRARY_API DateTime : public Object
  {
    DeclareRuntimeType();
  public:
    struct DayOfWeek
    {
      enum
      {
        SUNDAY = 0,
        MONDAY = 1,
        TUESDAY = 2,
        WEDNESDAY = 3,
        THURSDAY = 4,
        FRIDAY = 5,
        SATURDAY = 6
      };
    };

    struct MonthOfYear
    {
      enum
      {
        JANUARY = 1,
        FEBRUARY = 2,
        MARCH = 3,
        APRIL = 4,
        MAY = 5,
        JUNE = 6,
        JULY = 7,
        AUGUST = 8,
        SEPTEMBER = 9,
        OCTOBER = 10,
        NOVEMBER = 11,
        DECEMBER = 12
      };
    };

  public:
    /* Constructs a DateTime corresponding to the begin epoch */
    DateTime();
    /* Constructs a DateTime as a number of ticks from the epoch */
    DateTime(ULONGLONG Ticks);
    /* Constructs a DateTime from a CRT time_t */
    DateTime(time_t T);

    /* Constructs a DateTime from a FILETIME */
    DateTime(FILETIME Ft);
    
    #if defined(PLATFORM_POSIX)
    /* Constructs a DateTime from a unix timeval */
    DateTime(struct timeval);
    #endif

    /* Computes the difference of two DateTimes */
    TimeSpan  operator -  (CONST DateTime& d1) CONST;
    /* Adds a TimeSpan to a DateTime */
    DateTime  operator +  (CONST TimeSpan& dT) CONST;
    /* Adds a TimeSpan to a DateTime */
    DateTime& operator += (CONST TimeSpan& dT);

    /* Returns the year component of the DateTime */
    SHORT Year() CONST;
    /* Returns the month component of the DateTime */
    SHORT Month() CONST;
    /* Returns the day component of the DateTime */
    SHORT Day() CONST;
    /* Returns the day number of the year */
    SHORT DayOfYear() CONST;
    /* Returns the day of the week */
    SHORT DayOfWeek() CONST;
    /* Returns the hour component of the DateTime */
    SHORT Hour() CONST;
    /* Returns the minute component of the DateTime */
    SHORT Minute() CONST;
    /* Returns the second component of the DateTime */
    SHORT Second() CONST;
    /* Returns the nanosecond component of the DateTime */
    SHORT MilliSecond() CONST;

    DateTime Date() CONST;
    TimeSpan Time() CONST; 

    /* Returns the number of ticks since the epoch */
    ULONGLONG Ticks() CONST;
    operator ULONGLONG () CONST
    { return Ticks(); }

    operator SYSTEMTIME () CONST;
    DateTime& operator= (SYSTEMTIME S);
    operator FILETIME () CONST;
    DateTime& operator= (FILETIME S);

    /* Returns the name of the day of the week component of the DateTime */
    LPCTSTR DayOfWeekName() CONST;
    /* Returns the name of the month component of the DateTime */
    LPCTSTR MonthName() CONST;

    /* Changes from UTC to local time */
    DateTime ToLocalTime();
    /* Changes from local time to UTC */
    DateTime ToUTCTime();

    VOID AddNanoSeconds(INT N);
    VOID AddMicroSeconds(INT N);
    VOID AddMilliSeconds(INT N);
    VOID AddSeconds(INT N);
    VOID AddMinutes(INT N);
    VOID AddHours(INT N);
    VOID AddDays(INT N);
    VOID AddMonths(INT N);
    VOID AddYears(INT N);

    /* Returns the current system date and time adjusted from the current timezone to UTC */
    static DateTime NowUTC();
    /* Returns the current system date and time */
    static DateTime Now();

    String ToString(LPCTSTR Format) CONST;
    virtual String ToString() CONST
    { return ToString("dddd, MMMM dd, yyyy h:mm:ss tt"); }
    String ToLongDateString() CONST
    { return ToString("dddd, MMMM dd, yyyy"); }
    String ToShortDateString() CONST
    { return ToString("M/d/yyyy"); }
    String ToLongTimeString() CONST
    { return ToString("h:mm:ss tt"); }
    String ToShortString() CONST
    { return ToString("h:mm tt"); }

    /* Returns TRUE if a given year is a leap year */
    static BOOL IsLeapYear(INT Year)
    { return (Year % 4 == 0 && Year % 100 != 0) || (Year % 400 == 0); }

    /* The number of hours in a day */
    static CONST UINT HOURSPERDAY = 24;
    /* The number of minutes in an hour */
    static CONST UINT MINUTESPERHOUR = 60;
    /* The number of minutes in a day */
    static CONST UINT MINUTESPERDAY = 24 * 60;
    /* The number of seconds in a minute */
    static CONST UINT SECONDSPERMINUTE = 60;
    /* The number of seconds in an hour */
    static CONST UINT SECONDSPERHOUR = 3600;
    /* The number of seconds in a day */
    static CONST UINT SECONDSPERDAY = 3600 * 24;
    /* The number of days in a week */
    static CONST UINT DAYSPERWEEK = 7;
    /* The day of the week of the epoch (Monday) */
    static CONST UINT EPOCHWEEKDAY = 1;

    /* The number of days in 400 years */
    static CONST UINT DAYSPERQUADRICENTENNIUM = 365 * 400 + 97;
    /* The number of days in 100 years */
    static CONST UINT DAYSPERNORMALCENTURY = 365 * 100 + 24;
    /* The number of days in 4 years */
    static CONST UINT DAYSPERNORMALQUADRENNIUM = 365 * 4 + 1;

    /* The number of clock ticks that occur in one second */
    static CONST ULONGLONG TICKSPERSECOND;
    /* The number of clock ticks that occur in one milli-second */
    static CONST ULONGLONG TICKSPERMILLISECOND;
    /* The number of clock ticks that occur in one day */
    static CONST ULONGLONG TICKSPERDAY;

  protected:
    /* The number of ticks since the epoch */
    ULONGLONG m_Ticks;

    static CONST SHORT MonthDayInYear[];
    static LPCTSTR ShortMonthNames[];
    static LPCTSTR LongMonthNames[];
    static LPCTSTR ShortDayNames[];
    static LPCTSTR LongDayNames[];
  };
  
  IO::BinaryStream& operator << (IO::BinaryStream& S, DateTime x);
  IO::BinaryStream& operator >> (IO::BinaryStream& S, DateTime& x);

}


