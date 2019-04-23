/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Time/DateTime.cpp $
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
#include "Time/DateTime.hpp"

using namespace Bolitho;

#if defined(PLATFORM_POSIX)
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LONG GetTickCount()
{
  tms tm;
  return times(&tm);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID GetSystemTime(SYSTEMTIME* pTime)
{
  FILETIME Ft;
  GetSystemTimeAsFileTime(&Ft);
  FileTimeToSystemTime(&Ft, pTime);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID GetLocalTime(SYSTEMTIME* pTime)
{
  GetSystemTime(pTime);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID GetSystemTimeAsFileTime(FILETIME* pTime)
{
  time_t T = time(0);
  ULONGLONG Ticks;
  
  Ticks = (ULONGLONG)T;
  Ticks += 11644473600;
  Ticks *= DateTime::TICKSPERSECOND;

  pTime->dwLowDateTime = (DWORD)(Ticks & 0xffffffff);
  pTime->dwHighDateTime = (DWORD)(Ticks >> 32);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID FileTimeToSystemTime(FILETIME* pFileTime, SYSTEMTIME* pSystemTime)
{
  ULONGLONG liTime = MAKEULONGLONG(pFileTime->dwLowDateTime, pFileTime->dwHighDateTime);

  int SecondsInDay;
  long int cleaps, years, yearday, months;
  long int Days;
  
  LONGLONG Time;

  /* Extract millisecond from time and convert time into seconds */
  pSystemTime->wMilliseconds = (WORD) ((liTime % DateTime::TICKSPERSECOND) / DateTime::TICKSPERMILLISECOND);
  Time = liTime / DateTime::TICKSPERSECOND;

  /* The native version of RtlTimeToTimeFields does not take leap seconds
   * into account */

  /* Split the time into days and seconds within the day */
  Days = Time / DateTime::SECONDSPERDAY;
  SecondsInDay = Time % DateTime::SECONDSPERDAY;

  /* Compute time of day */
  pSystemTime->wHour = (WORD) (SecondsInDay / DateTime::SECONDSPERHOUR);
  SecondsInDay = SecondsInDay % DateTime::SECONDSPERHOUR;
  pSystemTime->wMinute = (WORD) (SecondsInDay / DateTime::SECONDSPERMINUTE);
  pSystemTime->wSecond = (WORD) (SecondsInDay % DateTime::SECONDSPERMINUTE);

  /* Compute day of week */
  pSystemTime->wDayOfWeek = (WORD) ((DateTime::EPOCHWEEKDAY + Days) % DateTime::DAYSPERWEEK);

  /* Compute year, month and day of month. */
  cleaps=(3 * ((4 * Days + 1227) / DateTime::DAYSPERQUADRICENTENNIUM) + 3) / 4;
  Days += 28188 + cleaps;
  years = (20 * Days - 2442) / (5 * DateTime::DAYSPERNORMALQUADRENNIUM);
  yearday = Days - (years * DateTime::DAYSPERNORMALQUADRENNIUM)/4;
  months = (64 * yearday) / 1959;
  
  /* the result is based on a year starting on March.
   * To convert take 12 from Januari and Februari and
   * increase the year by one. */
  if(months < 14) 
  {
    pSystemTime->wMonth = months - 1;
    pSystemTime->wYear = years + 1524;
  } 
  else 
  {
    pSystemTime->wMonth = months - 13;
    pSystemTime->wYear = years + 1525;
  }
  
  /* calculation of day of month is based on the wonderful
   * sequence of INT(n * 30.6): it reproduces the 
   * 31-30-31-30-31-31 month lengths exactly for small n's */
  pSystemTime->wDay = yearday - (1959 * months) / 64 ;
  return;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID SystemTimeToFileTime(SYSTEMTIME* pSystemTime, FILETIME* pFileTime)
{
  int month, year, cleaps, day;
 
  /* FIXME: normalize the TIME_FIELDS structure here */

  /* now calculate a day count from the date
   * First start counting years from March. This way the leap days
   * are added at the end of the year, not somewhere in the middle.
   * Formula's become so much less complicate that way.
   * To convert: add 12 to the month numbers of Jan and Feb, and 
   * take 1 from the year */
  
  if(pSystemTime->wMonth < 3) 
  {
    month = pSystemTime->wMonth + 13;
    year = pSystemTime->wYear - 1;
  } 
  else 
  {
    month = pSystemTime->wMonth + 1;
    year = pSystemTime->wYear;
  }

  cleaps = (3 * (year / 100) + 3) / 4;   /* nr of "century leap years"*/
  day =  (36525 * year) / 100 - cleaps + /* year * dayperyr, corrected */
         (1959 * month) / 64 +           /* months * daypermonth */
         pSystemTime->wDay -             /* day of the month */
         584817 ;                      /* zero that on 1601-01-01 */
  /* done */

  ULONGLONG Ticks = (((((ULONGLONG) day * DateTime::HOURSPERDAY +
			pSystemTime->wHour) * DateTime::MINUTESPERHOUR +
		       pSystemTime->wMinute) * DateTime::SECONDSPERMINUTE +
		     pSystemTime->wSecond) * 1000 +
		     pSystemTime->wMilliseconds) * DateTime::TICKSPERMILLISECOND;

  pFileTime->dwLowDateTime = (DWORD)(Ticks & 0xffffffff);
  pFileTime->dwHighDateTime = (DWORD)(Ticks >> 32);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#endif


//-----------------------------------------------------------------------------------------------------------------------------------------------------
CONST ULONGLONG DateTime::TICKSPERSECOND = 10 * 1000 * 1000;
CONST ULONGLONG DateTime::TICKSPERMILLISECOND = 10 * 1000;
CONST ULONGLONG DateTime::TICKSPERDAY = DateTime::TICKSPERSECOND * DateTime::SECONDSPERDAY;
//-----------------------------------------------------------------------------------------------------------------------------------------------------
CONST SHORT DateTime::MonthDayInYear[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
LPCTSTR DateTime::ShortMonthNames[13] = { TEXT(""), TEXT("Jan"), TEXT("Feb"), TEXT("Mar"), TEXT("Apr"), TEXT("May"), TEXT("Jun"), TEXT("Jul"), TEXT("Aug"), TEXT("Sep"), TEXT("Oct"), TEXT("Nov"), TEXT("Dec") };
LPCTSTR DateTime::LongMonthNames[13] = { TEXT(""), TEXT("January"), TEXT("February"), TEXT("March"), TEXT("April"), TEXT("May"), TEXT("June"), TEXT("July"), TEXT("August"), TEXT("September"), TEXT("October"), TEXT("November"), TEXT("December") };
LPCTSTR DateTime::ShortDayNames[7] = { TEXT("Sun"), TEXT("Mon"), TEXT("Tue"), TEXT("Wed"), TEXT("Thu"), TEXT("Fri"), TEXT("Sat") };
LPCTSTR DateTime::LongDayNames[7] = { TEXT("Sunday"), TEXT("Monday"), TEXT("Tuesday"), TEXT("Wednesday"), TEXT("Thursday"), TEXT("Friday"), TEXT("Saturday") };
//-----------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho,DateTime,Object);
//-----------------------------------------------------------------------------------------------------------------------------
DateTime::DateTime()
{
  m_Ticks = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------
DateTime::DateTime(ULONGLONG Ticks)
{
  m_Ticks = Ticks;
}
//-----------------------------------------------------------------------------------------------------------------------------
DateTime::DateTime(time_t T)
{
  m_Ticks = (ULONGLONG)T;
  m_Ticks += 11644473600;
  m_Ticks *= TICKSPERSECOND;
}
//-----------------------------------------------------------------------------------------------------------------------------
DateTime::DateTime(FILETIME Ft)
{
  m_Ticks = MAKELONGLONG(Ft.dwLowDateTime, Ft.dwHighDateTime);
}
//-----------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_WINDOWS)
DateTime DateTime::Now()
{
  SYSTEMTIME St;
  GetLocalTime(&St);
  FILETIME Ft;
  SystemTimeToFileTime(&St, &Ft);

  return DateTime(Ft);
}
//-----------------------------------------------------------------------------------------------------------------------------
DateTime DateTime::NowUTC()
{
  SYSTEMTIME St;
  GetSystemTime(&St);
  FILETIME Ft;
  SystemTimeToFileTime(&St, &Ft);

  return DateTime(Ft);
}
//-----------------------------------------------------------------------------------------------------------------------------
DateTime DateTime::ToLocalTime()
{
  FILETIME UTC = (*this);
  FILETIME Local;
  FileTimeToLocalFileTime(&UTC, &Local);
  return DateTime(Local);
}
//-----------------------------------------------------------------------------------------------------------------------------
DateTime DateTime::ToUTCTime()
{
  FILETIME Local = (*this);
  FILETIME UTC;
  LocalFileTimeToFileTime(&Local, &UTC);
  return DateTime(UTC);
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------
DateTime::operator SYSTEMTIME () CONST
{
  FILETIME Ft;
  Ft.dwLowDateTime = (DWORD)(m_Ticks & 0xffffffff);
  Ft.dwHighDateTime = (DWORD)(m_Ticks >> 32);
  
  SYSTEMTIME St;
  FileTimeToSystemTime(&Ft, &St);
  return St;
}
//-----------------------------------------------------------------------------------------------------------------------------
DateTime::operator FILETIME () CONST
{
  FILETIME Ft;
  Ft.dwLowDateTime = (DWORD)(m_Ticks & 0xffffffff);
  Ft.dwHighDateTime = (DWORD)(m_Ticks >> 32);
  return Ft;
}
//-----------------------------------------------------------------------------------------------------------------------------
DateTime& DateTime::operator= (SYSTEMTIME S)
{
  FILETIME Ft;
  SystemTimeToFileTime(&S, &Ft);
  m_Ticks = Ft.dwLowDateTime + ((ULONGLONG)Ft.dwHighDateTime << 32);
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------
DateTime& DateTime::operator= (FILETIME Ft)
{
  m_Ticks = Ft.dwLowDateTime + ((ULONGLONG)Ft.dwHighDateTime << 32);
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_POSIX)
DateTime::DateTime(struct timeval T)
{
  m_Ticks = (ULONGLONG)T.tv_sec;
  m_Ticks += 11644473600;
  m_Ticks *= 1000000;
  m_Ticks += (ULONGLONG)T.tv_usec;
  m_Ticks *= 100;
}
//-----------------------------------------------------------------------------------------------------------------------------
DateTime DateTime::Now()
{
  return DateTime(time(0));
}
//-----------------------------------------------------------------------------------------------------------------------------
DateTime DateTime::NowUTC()
{
  return DateTime(time(0));
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
SHORT DateTime::Year() CONST
{
  SYSTEMTIME P = (*this);
  return P.wYear;
}

//-----------------------------------------------------------------------------------------------------------------------------
SHORT DateTime::Month() CONST
{
  SYSTEMTIME P = (*this);
  return P.wMonth;
}

//-----------------------------------------------------------------------------------------------------------------------------
LPCTSTR DateTime::MonthName() CONST
{
  SYSTEMTIME P = (*this);
  return LongMonthNames[P.wMonth];
}

//-----------------------------------------------------------------------------------------------------------------------------
SHORT DateTime::Day() CONST
{
  SYSTEMTIME P = (*this);
  return P.wDay;
}

//-----------------------------------------------------------------------------------------------------------------------------
SHORT DateTime::DayOfWeek() CONST
{
  SYSTEMTIME P = (*this);
  return P.wDayOfWeek;
}

//-----------------------------------------------------------------------------------------------------------------------------
LPCTSTR DateTime::DayOfWeekName() CONST
{
  SYSTEMTIME P = (*this);
  return LongDayNames[P.wDayOfWeek];
}

//-----------------------------------------------------------------------------------------------------------------------------
SHORT DateTime::Hour() CONST
{
  SYSTEMTIME P = (*this);
  return P.wHour;
}

//-----------------------------------------------------------------------------------------------------------------------------
SHORT DateTime::Minute() CONST
{
  SYSTEMTIME P = (*this);
  return P.wMinute;
}

//-----------------------------------------------------------------------------------------------------------------------------
SHORT DateTime::Second() CONST
{
  SYSTEMTIME P = (*this);
  return P.wSecond;
}

//-----------------------------------------------------------------------------------------------------------------------------
ULONGLONG DateTime::Ticks() CONST
{
  return m_Ticks;
}
//-----------------------------------------------------------------------------------------------------------------------------
SHORT DateTime::MilliSecond() CONST
{
  SYSTEMTIME P = (*this);
  return P.wMilliseconds;
}
//-----------------------------------------------------------------------------------------------------------------------------
DateTime DateTime::Date() CONST
{
  return DateTime(m_Ticks - (m_Ticks % TICKSPERDAY));
}
//-----------------------------------------------------------------------------------------------------------------------------
TimeSpan DateTime::Time() CONST
{
  return TimeSpan(m_Ticks % TICKSPERDAY);
}
//-----------------------------------------------------------------------------------------------------------------------------
VOID DateTime::AddNanoSeconds(INT N)
{
  m_Ticks += (N / 100);
}
//-----------------------------------------------------------------------------------------------------------------------------
VOID DateTime::AddMicroSeconds(INT N)
{
  m_Ticks += (N * 10);
}
//-----------------------------------------------------------------------------------------------------------------------------
VOID DateTime::AddMilliSeconds(INT N)
{
  m_Ticks += (N * 10000);
}
//-----------------------------------------------------------------------------------------------------------------------------
VOID DateTime::AddSeconds(INT N)
{
  m_Ticks += (LONGLONG)TICKSPERSECOND * (LONGLONG)N;
}
//-----------------------------------------------------------------------------------------------------------------------------
VOID DateTime::AddMinutes(INT N)
{
  m_Ticks += (LONGLONG)TICKSPERSECOND * 60L * N;
}
//-----------------------------------------------------------------------------------------------------------------------------
VOID DateTime::AddHours(INT N)
{
  m_Ticks += (LONGLONG)TICKSPERSECOND * 3600L * N;
}
//-----------------------------------------------------------------------------------------------------------------------------
VOID DateTime::AddDays(INT N)
{
  m_Ticks += (LONGLONG)TICKSPERSECOND * 86400L * N;
}
//-----------------------------------------------------------------------------------------------------------------------------
VOID DateTime::AddMonths(INT N)
{
  SYSTEMTIME P = (*this);
  P.wMonth += N;
  (*this) = P;
}
//-----------------------------------------------------------------------------------------------------------------------------
VOID DateTime::AddYears(INT N)
{
  SYSTEMTIME P = (*this);
  P.wYear += N;
  (*this) = P;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
INT TokenLength(LPCTSTR Format)
{
  LPCTSTR Start = Format;
  while (*Format == *Start)
    Format++;

  return (INT)(Format - Start);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
String DateTime::ToString(LPCTSTR Format) CONST
{
  SYSTEMTIME Date = (*this);
  
  if (Format == 0)
    return String::Empty;

  TCHAR String[1024];
  LPTSTR pOutput = String;
  ZeroMemory(String, 1024*sizeof(TCHAR));
  
  LPCTSTR Ch = Format;
  while (*Ch != 0)
  {
    INT Length = TokenLength(Ch);
    switch (*Ch)
    {
      case 'd':
        switch (Length)
        {
        case 1:
          sprintf(pOutput, "%d", Date.wDay);
          while (*pOutput != 0)
            pOutput++;
          break;
        case 2:
          sprintf(pOutput, "%02d", Date.wDay);
          while (*pOutput != 0)
            pOutput++;
          break;
        case 3:
          sprintf(pOutput, "%s", ShortDayNames[Date.wDayOfWeek]);
          while (*pOutput != 0)
            pOutput++;
          break;
        case 4:
          sprintf(pOutput, "%s", LongDayNames[Date.wDayOfWeek]);
          while (*pOutput != 0)
            pOutput++;
          break;
        };

        break;
      case 'M':
        switch (Length)
        {
        case 1:
          sprintf(pOutput, "%d", Date.wMonth);
          while (*pOutput != 0)
            pOutput++;
          break;
        case 2:
          sprintf(pOutput, "%02d", Date.wMonth);
          while (*pOutput != 0)
            pOutput++;
          break;
        case 3:
          sprintf(pOutput, "%s", ShortMonthNames[Date.wMonth]);
          while (*pOutput != 0)
            pOutput++;
          break;
        case 4:
          sprintf(pOutput, "%s", LongMonthNames[Date.wMonth]);
          while (*pOutput != 0)
            pOutput++;
          break;
        };
        break;

      case 'y':
        switch (Length)
        {
        case 1:
        case 2:
          sprintf(pOutput, "%02d", Date.wYear % 100);
          while (*pOutput != 0)
            pOutput++;
          break;
        case 4:
          sprintf(pOutput, "%04d", Date.wYear);
          while (*pOutput != 0)
            pOutput++;
          break;
        };
        break;

      case 'h':
        switch (Length)
        {
        case 1:
          sprintf(pOutput, "%d", Date.wHour > 12 ? Date.wHour - 12 : Date.wHour == 0 ? 12 : Date.wHour);
          while (*pOutput != 0)
            pOutput++;
          break;
        case 2:
          sprintf(pOutput, "%02d", Date.wHour > 12 ? Date.wHour - 12 : Date.wHour == 0 ? 12 : Date.wHour);
          while (*pOutput != 0)
            pOutput++;
          break;
        };
        break;

      case 'H':
        switch (Length)
        {
        case 1:
          sprintf(pOutput, "%d", Date.wHour);
          while (*pOutput != 0)
            pOutput++;
          break;
        case 2:
          sprintf(pOutput, "%02d", Date.wHour);
          while (*pOutput != 0)
            pOutput++;
          break;
        };
        break;

      case 'm':
        switch (Length)
        {
        case 1:
          sprintf(pOutput, "%d", Date.wMinute);
          while (*pOutput != 0)
            pOutput++;
          break;
        case 2:
          sprintf(pOutput, "%02d", Date.wMinute);
          while (*pOutput != 0)
            pOutput++;
          break;
        };
        break;

      case 's':
        switch (Length)
        {
        case 1:
          sprintf(pOutput, "%d", Date.wSecond);
          while (*pOutput != 0)
            pOutput++;
          break;
        case 2:
          sprintf(pOutput, "%02d", Date.wSecond);
          while (*pOutput != 0)
            pOutput++;
          break;
        };
        break;

       case 'f':
        switch (Length)
        {
        case 1:
          sprintf(pOutput, "%.1f", Date.wMilliseconds / 1000.0f);
          while (*pOutput != 0)
          { pOutput[0] = pOutput[2]; pOutput++; }
          break;
        case 2:
          sprintf(pOutput, "%.2f", Date.wMilliseconds / 1000.0f);
          while (*pOutput != 0)
          { pOutput[0] = pOutput[2]; pOutput++; }
          break;
        case 3:
          sprintf(pOutput, "%.3f", Date.wMilliseconds / 1000.0f);
          while (*pOutput != 0)
          { pOutput[0] = pOutput[2]; pOutput++; }
          break;
        };
        break;

      case 't':
        switch (Length)
        {
        case 1:
          sprintf(pOutput, "%s", Date.wHour < 12 ? "A" : "P");
          while (*pOutput != 0)
            pOutput++;
          break;
        case 2:
          sprintf(pOutput, "%s", Date.wHour < 12 ? "AM" : "PM");
          while (*pOutput != 0)
            pOutput++;
          break;
        };
        break;

      
      default:
      *pOutput = *Ch;
      pOutput++;
      Ch++;
      continue;
    };

    Ch += Length;
  }
  
  return String;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
TimeSpan DateTime::operator - (CONST DateTime& D1) CONST
{
  return TimeSpan(m_Ticks - D1.m_Ticks);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
DateTime DateTime::operator +  (CONST TimeSpan& dT) CONST
{
  return DateTime(m_Ticks + dT.m_Ticks); 
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
DateTime& DateTime::operator += (CONST TimeSpan& dT)
{
  m_Ticks += dT.m_Ticks;
  return *this;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho,TimeSpan,Object);
//---------------------------------------------------------------------------------------------------------------------------------------------------
TimeSpan::TimeSpan()
{
  m_Ticks = 0;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
TimeSpan::TimeSpan(LONGLONG Ticks)
{
  m_Ticks = Ticks;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------------------------------
TimeSpan::TimeSpan(FILETIME T)
{
  m_Ticks = MAKELONGLONG(T.dwLowDateTime, T.dwHighDateTime);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------------------------------
TimeSpan TimeSpan::operator+ (CONST TimeSpan& T)
{
  return TimeSpan(m_Ticks+T.m_Ticks);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
TimeSpan TimeSpan::operator- (CONST TimeSpan& T)
{
  return TimeSpan(m_Ticks-T.m_Ticks);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
LONGLONG TimeSpan::Ticks() CONST
{
  return m_Ticks;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT TimeSpan::TotalNanoSeconds() CONST
{
  return m_Ticks * 100.0f;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT TimeSpan::TotalMicroSeconds() CONST
{
  return m_Ticks / 10.0f;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT TimeSpan::TotalMilliSeconds() CONST
{
  return m_Ticks / 10.0f / 1000.0f;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT TimeSpan::TotalSeconds() CONST
{
  return m_Ticks / (FLOAT)DateTime::TICKSPERSECOND;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT TimeSpan::TotalMinutes() CONST
{
  return m_Ticks / (FLOAT)DateTime::TICKSPERSECOND / 60.0f;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT TimeSpan::TotalHours() CONST
{
  return m_Ticks / (FLOAT)DateTime::TICKSPERSECOND / 60.0f / 60.0f;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT TimeSpan::TotalDays() CONST
{
  return m_Ticks / (FLOAT)DateTime::TICKSPERSECOND / 60.0f / 60.0f / 24.0f;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------------------------------------------------
IO::BinaryStream& Bolitho::operator << (IO::BinaryStream& S, DateTime x)
{
  S.Write(x.Ticks());  
  return S;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
IO::BinaryStream& Bolitho::operator >> (IO::BinaryStream& S, DateTime& x)
{
  x = DateTime(S.ReadUInt64());
  return S;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------------------------------
IO::BinaryStream& Bolitho::operator << (IO::BinaryStream& S, TimeSpan x)
{
  S.Write(x.Ticks());
  return S;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
IO::BinaryStream& Bolitho::operator >> (IO::BinaryStream& S, TimeSpan& x)
{
  x = TimeSpan(S.ReadInt64());
  return S;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------

