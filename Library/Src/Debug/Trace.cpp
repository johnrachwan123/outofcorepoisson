/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Debug/Trace.cpp $
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

#include "CodeLibrary.hpp"

#include "IO/StdIO.hpp"
#include "System/Console.hpp"
#include "Debug/Trace.hpp"
#include "Time/DateTime.hpp"
#include "System/Threading/Thread.hpp"

#include <cstdarg>
#include <cstdio>

using namespace Bolitho;
using namespace Bolitho::IO;
using namespace Bolitho::System;

#if defined(PLATFORM_WINDOWS)

namespace Bolitho
{
  class ConsoleTraceSink : public ITraceSink
  {
  public:
    ConsoleTraceSink()
    {
      m_IndentString[0] =  TEXT("");
      m_IndentString[1] =  TEXT(" ");
      m_IndentString[2] =  TEXT("  ");
      m_IndentString[3] =  TEXT("   ");
      m_IndentString[4] =  TEXT("    ");
      m_IndentString[5] =  TEXT("     ");
      m_IndentString[6] =  TEXT("      ");
      m_IndentString[7] =  TEXT("       ");
      m_IndentString[8] =  TEXT("        ");
      m_IndentString[9] =  TEXT("         ");
      m_IndentString[10] = TEXT("          ");
      m_IndentString[11] = TEXT("           ");
      m_IndentString[12] = TEXT("            ");
      m_IndentString[13] = TEXT("             ");
      m_IndentString[14] = TEXT("              ");
      m_IndentString[15] = TEXT("               ");
    }

    virtual VOID Trace(DWORD TraceLevel, DWORD Indent, LPCVOID Cookie, LPCTSTR String)
    {
      StdOut.Write(TEXT("%s"), m_IndentString[Indent]);
      if (TraceLevel & TraceLevel::WARNING)
      {
        Console::SaveAttribute();
        Console::SetAttribute(ConsoleAttribute::YELLOW | ConsoleAttribute::WHITE, ConsoleAttribute::BLACK);
        StdOut.WriteLine("WARNING: %s", String);
        Console::RestoreAttribute();
      }
      else if (TraceLevel & TraceLevel::ERROR)
      {
        Console::SaveAttribute();
        Console::SetAttribute(ConsoleAttribute::RED | ConsoleAttribute::WHITE, ConsoleAttribute::BLACK);
        StdOut.WriteLine("ERROR: %s", String);
        Console::RestoreAttribute();
      }
      else
      {
        StdOut.WriteLine("%s", String);
      }

    }

  private:
    LPCTSTR m_IndentString[16];

  };

  class MessageBoxTraceSink : public ITraceSink
  {
  public:
    MessageBoxTraceSink()
    {
      m_IndentString[0] =  TEXT("");
      m_IndentString[1] =  TEXT(" ");
      m_IndentString[2] =  TEXT("  ");
      m_IndentString[3] =  TEXT("   ");
      m_IndentString[4] =  TEXT("    ");
      m_IndentString[5] =  TEXT("     ");
      m_IndentString[6] =  TEXT("      ");
      m_IndentString[7] =  TEXT("       ");
      m_IndentString[8] =  TEXT("        ");
      m_IndentString[9] =  TEXT("         ");
      m_IndentString[10] = TEXT("          ");
      m_IndentString[11] = TEXT("           ");
      m_IndentString[12] = TEXT("            ");
      m_IndentString[13] = TEXT("             ");
      m_IndentString[14] = TEXT("              ");
      m_IndentString[15] = TEXT("               ");    
    }

    virtual VOID Trace(DWORD TraceLevel, DWORD Indent, LPCVOID Cookie, LPCTSTR String)
    {
      Bolitho::String Message;
      if (TraceLevel & TraceLevel::WARNING)
        Message = String::Format("%s%s WARNING: %s", m_IndentString[Indent], DateTime::Now().ToString("dd/MM/yyyy h:mm:ss tt").ConstStr(), String);
      else if (TraceLevel & TraceLevel::ERROR)
        Message = String::Format("%s%s ERROR: %s", m_IndentString[Indent], DateTime::Now().ToString("dd/MM/yyyy h:mm:ss tt").ConstStr(), String);
      else
        Message = String::Format("%s%s %s", m_IndentString[Indent], DateTime::Now().ToString("dd/MM/yyyy h:mm:ss tt").ConstStr(), String);

      ::MessageBox(0, Message, "Trace", 0);
    }

  private:
    LPCTSTR m_IndentString[16];
  };
}

#else

namespace Bolitho
{
  class ConsoleTraceSink : public ITraceSink
  {
  public:
    ConsoleTraceSink()
    {
      m_IndentString[0] =  TEXT("");
      m_IndentString[1] =  TEXT(" ");
      m_IndentString[2] =  TEXT("  ");
      m_IndentString[3] =  TEXT("   ");
      m_IndentString[4] =  TEXT("    ");
      m_IndentString[5] =  TEXT("     ");
      m_IndentString[6] =  TEXT("      ");
      m_IndentString[7] =  TEXT("       ");
      m_IndentString[8] =  TEXT("        ");
      m_IndentString[9] =  TEXT("         ");
      m_IndentString[10] = TEXT("          ");
      m_IndentString[11] = TEXT("           ");
      m_IndentString[12] = TEXT("            ");
      m_IndentString[13] = TEXT("             ");
      m_IndentString[14] = TEXT("              ");
      m_IndentString[15] = TEXT("               ");
    }

    virtual VOID Trace(DWORD TraceLevel, DWORD Indent, LPCVOID Cookie, LPCTSTR String)
    {
      StdOut.Write("%s", m_IndentString[Indent]);
      if (TraceLevel & TraceLevel::WARNING)
      {
        StdOut.Write("WARNING: ");
        StdOut.WriteLine(String);
      }
      else if (TraceLevel & TraceLevel::ERROR)
      {
        StdOut.Write("ERROR: ");
        StdOut.WriteLine(String);
      }
      else
      {
        StdOut.WriteLine(String);
      }

    }

  private:
    LPCTSTR m_IndentString[16];

  };
}

#endif

namespace Bolitho
{
  class TextStreamTraceSink : public ITraceSink
  {
  public:
    TextStreamTraceSink(TextStream Stream) : m_Stream(Stream)
    {
      m_IndentString[0] =  TEXT("");
      m_IndentString[1] =  TEXT(" ");
      m_IndentString[2] =  TEXT("  ");
      m_IndentString[3] =  TEXT("   ");
      m_IndentString[4] =  TEXT("    ");
      m_IndentString[5] =  TEXT("     ");
      m_IndentString[6] =  TEXT("      ");
      m_IndentString[7] =  TEXT("       ");
      m_IndentString[8] =  TEXT("        ");
      m_IndentString[9] =  TEXT("         ");
      m_IndentString[10] = TEXT("          ");
      m_IndentString[11] = TEXT("           ");
      m_IndentString[12] = TEXT("            ");
      m_IndentString[13] = TEXT("             ");
      m_IndentString[14] = TEXT("              ");
      m_IndentString[15] = TEXT("               ");
    }

    virtual VOID Trace(DWORD TraceLevel, DWORD Indent, LPCVOID Cookie, LPCTSTR String)
    {
      if (TraceLevel & TraceLevel::WARNING)
        m_Stream.WriteLine(String::Format("%s%s WARNING: %s", DateTime::Now().ToString("h:mm:ss tt").ConstStr(), m_IndentString[Indent], String));
      else if (TraceLevel & TraceLevel::ERROR)
        m_Stream.WriteLine(String::Format("%s%s ERROR: %s", DateTime::Now().ToString("h:mm:ss tt").ConstStr(), m_IndentString[Indent], String));
      else
        m_Stream.WriteLine(String::Format("%s%s %s", DateTime::Now().ToString("h:mm:ss tt").ConstStr(), m_IndentString[Indent], String));

    }

  private:
    TextStream m_Stream;
    LPCTSTR m_IndentString[16];
  };

  class BinaryStreamTraceSink : public ITraceSink
  {
  public:
    BinaryStreamTraceSink(BinaryStream Stream) : m_Stream(Stream)
    {}

    virtual VOID Trace(DWORD TraceLevel, DWORD Indent, LPCVOID Cookie, LPCTSTR String)
    {
      m_Stream.Write(TraceLevel);
      m_Stream.Write(Indent);
      m_Stream.Write((UINT32)CharTraits<TCHAR>::Length(String));
      m_Stream.Write(String, CharTraits<TCHAR>::Length(String));
    }

  protected:
    BinaryStream m_Stream;
  };
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
Tracer Bolitho::Trace;
Tracer& Bolitho::Trace0(Bolitho::Trace);

#if defined(DEBUG)
Tracer& Bolitho::TraceD(Bolitho::Trace);
#else
NullTracer Bolitho::TraceD;
#endif

#if CONFIG_TRACELEVEL > 0
Tracer& Bolitho::Trace1(Bolitho::Trace);
#else
NullTracer Bolitho::Trace1;
#endif

#if CONFIG_TRACELEVEL > 1
Tracer& Bolitho::Trace2(Bolitho::Trace);
#else
NullTracer Bolitho::Trace2;
#endif

#if CONFIG_TRACELEVEL > 2
Tracer& Bolitho::Trace3(Bolitho::Trace);
#else
NullTracer Bolitho::Trace3;
#endif

#if CONFIG_TRACELEVEL > 3
Tracer& Bolitho::Trace4(Bolitho::Trace);
#else
NullTracer Bolitho::Trace4;
#endif

#if CONFIG_TRACELEVEL > 4
Tracer& Bolitho::Trace5(Bolitho::Trace);
#else
NullTracer Bolitho::Trace5;
#endif

#if CONFIG_TRACELEVEL > 5
Tracer& Bolitho::Trace6(Bolitho::Trace);
#else
NullTracer Bolitho::Trace6;
#endif

#if CONFIG_TRACELEVEL > 6
Tracer& Bolitho::Trace7(Bolitho::Trace);
#else
NullTracer Bolitho::Trace7;
#endif

//------------------------------------------------------------------------------------------------------------------------------------------------------
Tracer::Tracer()
{
  m_IndentLevelHandle = Thread::AllocateThreadLocalStorage();
  m_CurrentTraceLevel = CONFIG_TRACELEVEL;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Tracer::~Tracer()
{
  Thread::FreeThreadLocalStorage(m_IndentLevelHandle);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Tracer::AddSink(ITraceSink* pTrace)
{
  Lock();

  for (SIZE_T i=0; i<32; i++)
  {
    if (m_TraceSinks[i] == 0)
    {
      m_TraceSinks[i] = pTrace;
      Unlock();
      return i;
    }
  }
  Unlock();
  
  return (SIZE_T)-1;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Tracer::AddSink(StandardStream Stream)
{
  if (Stream.IsRedirected())
    return AddSink(new TextStreamTraceSink(Stream));
  else
    return AddSink(new ConsoleTraceSink());
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Tracer::AddSink(TextStream Stream)
{
  return AddSink(new TextStreamTraceSink(Stream));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Tracer::AddSink(BinaryStream Stream)
{
  return AddSink(new BinaryStreamTraceSink(Stream));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Tracer::AddSink(Ptr<IO::File> F)
{
  return AddSink(new TextStreamTraceSink(TextStream(F)));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Tracer::AddConsoleSink()
{
  if (StdOut.IsRedirected())
    return AddSink(StdOut);
  else
    return AddSink(new ConsoleTraceSink());
}
//------------------------------------------------------------------------------------------------------------------------------------------------------

#if defined(PLATFORM_WINDOWS)
//------------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T Tracer::AddMessageBoxSink()
{
  return AddSink(new MessageBoxTraceSink());
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
#endif

//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID Tracer::RemoveSink(SIZE_T i)
{
  Assert(i < 32);
  m_TraceSinks[i] = 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
DWORD Tracer::GetIndentLevel()
{ 
  return (DWORD)reinterpret_cast<SIZE_T>(Thread::GetThreadLocalValue(m_IndentLevelHandle));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID Tracer::Indent()
{ 
  Thread::SetThreadLocalValue(m_IndentLevelHandle, (LPVOID)(GetIndentLevel() + 1));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID Tracer::UnIndent()
{ 
  Thread::SetThreadLocalValue(m_IndentLevelHandle, (LPVOID)(GetIndentLevel() - 1));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID Tracer::WriteV(DWORD TraceLevel, LPCVOID Cookie, LPCTSTR Format, va_list Args)
{
  Lock();

  if ((TraceLevel & 0xff) >= m_CurrentTraceLevel)
    return;

  for (SIZE_T i=0; i<32; i++)
    if (m_TraceSinks[i] != 0)
      m_TraceSinks[i]->Trace(TraceLevel, GetIndentLevel(), Cookie, String::FormatV(Format, Args));

  Unlock();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID Tracer::Write(DWORD TraceLevel, LPCVOID Cookie, LPCTSTR String)
{
  Lock();

  if ((TraceLevel & 0xff) >= m_CurrentTraceLevel)
    return;

  for (SIZE_T i=0; i<32; i++)
    if (m_TraceSinks[i] != 0)
      m_TraceSinks[i]->Trace(TraceLevel, GetIndentLevel(), Cookie, String);

  Unlock();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID Tracer::operator() (LPCTSTR Format, ...)
{
  va_list Args;
  va_start(Args,Format);
  WriteV(TraceLevel::INFO,0,Format,Args);
  va_end(Args);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID Tracer::operator() (DWORD TraceLevel, LPCVOID Cookie, LPCTSTR Format, ...)
{
  va_list Args;
  va_start(Args,Format);
  WriteV(TraceLevel,Cookie,Format,Args);
  va_end(Args);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID Tracer::Lock()
{
  m_Lock.Lock();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID Tracer::Unlock()
{
  m_Lock.Unlock();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
FunctionTrace::FunctionTrace(LPCVOID ObjectId, LPCTSTR pFunctionName)
{
  m_ObjectId = ObjectId;
  m_Flags = 0;
  m_pFunctionName = pFunctionName;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
FunctionTrace::~FunctionTrace()
{
  Leave();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID FunctionTrace::Enter(LPCTSTR FunctionId, ...)
{
  va_list Args;
  va_start(Args,FunctionId);
  String FunctionArgs = String::FormatV(FunctionId, Args);
  va_end(Args);

  Trace(TraceLevel::FUNCTIONTRACE, m_ObjectId, TEXT("[%08x] [%p] %s(%s)"), Thread::GetCurrentThreadId(), m_ObjectId, m_pFunctionName, FunctionArgs.ConstStr());
  Trace.Indent();

  m_Timer.Start();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID FunctionTrace::Leave()
{
  if (m_Flags != 0)
    return;
  m_Flags = 1;

  m_Timer.Stop();

  Trace.UnIndent();
  Trace(TraceLevel::FUNCTIONTRACE, m_ObjectId, TEXT("[%08x] [%p] %s returned (%f seconds)"), Thread::GetCurrentThreadId(), m_ObjectId, m_pFunctionName, m_Timer.ElapsedSeconds());
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID FunctionTrace::Return(LPCTSTR FunctionReturn, ...)
{
  if (m_Flags != 0)
    return;
  m_Flags = 1;

  m_Timer.Stop();
  
  Trace.UnIndent();

  va_list Args;
  va_start(Args,FunctionReturn);
  String Return = String::FormatV(FunctionReturn, Args);
  va_end(Args);

  Trace(TraceLevel::FUNCTIONTRACE, m_ObjectId, TEXT("[%08x] [%p] %s returned %s (%f seconds)"), Thread::GetCurrentThreadId(), m_ObjectId, m_pFunctionName, Return.ConstStr(), m_Timer.ElapsedSeconds());
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
