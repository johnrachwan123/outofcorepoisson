/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Debug/Trace.hpp $
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

#include "IO/Stream.hpp"
#include "IO/TextStream.hpp"
#include "IO/StdIO.hpp"
#include "IO/File.hpp"
#include "System/Threading/CriticalSection.hpp"
#include "System/Threading/Thread.hpp"
#include "Time/Timer.hpp"

#if !defined(CONFIG_TRACELEVEL)
#define CONFIG_TRACELEVEL 1
#endif

namespace Bolitho
{
  struct TraceLevel
  {
    enum
    { 
      INFO = 0x00000000, 
      WARNING = 0x20000000,
      FUNCTIONTRACE = 0x40000000,
      ERROR = 0x80000000, 
      CONSOLE = 0x10000000
    };
  };

  class LIBRARY_API ITraceSink : virtual public Object
  {
  public:
    virtual VOID Trace(DWORD TraceLevel, DWORD Indent, LPCVOID Cookie, LPCTSTR String) = 0;
  };

  class LIBRARY_API Tracer
  {
    friend class FunctionTrace;
  public:
    Tracer();
    ~Tracer();

    SIZE_T AddConsoleSink();
    SIZE_T AddMessageBoxSink();
    SIZE_T AddSink(IO::TextStream S);
    SIZE_T AddSink(IO::BinaryStream S);
    SIZE_T AddSink(IO::StandardStream S);
    SIZE_T AddSink(Ptr<IO::File> F);
    SIZE_T AddSink(ITraceSink* pTrace);

    VOID RemoveSink(SIZE_T i);

    /* Outputs a string to the trace device */
    VOID operator() (LPCTSTR Format, ...);
    /* Outputs a string to the trace device */
    VOID operator() (DWORD TraceLevel, LPCVOID ObjectId, LPCTSTR Format, ...);

    /* Returns the current indentation level for the calling thread */
    DWORD GetIndentLevel();
    /* Increments the indentation level for the calling thread */
    VOID Indent();
    /* Decrements the indentation level for the calling thread */
    VOID UnIndent();

    /* Gets the current trace level */
    DWORD GetTraceLevel() CONST
    { return m_CurrentTraceLevel; }
    /* Sets the current trace level */
    VOID SetTraceLevel(DWORD TraceLevel)
    { m_CurrentTraceLevel = TraceLevel; }

    /* Aquires the Trace global lock */
    VOID Lock();
    /* Releases the Trace global lock */
    VOID Unlock();

  protected:
    Ptr<ITraceSink> m_TraceSinks[32];
    VOID WriteV(DWORD TraceLevel, LPCVOID Cookie, LPCTSTR Format, va_list Args);
    VOID Write(DWORD TraceLevel, LPCVOID Cookie, LPCTSTR String);

    DWORD m_IndentLevelHandle;
    DWORD m_CurrentTraceLevel;

    System::CriticalSection m_Lock;
  };

  /* A Tracer class that does nothing */
  class LIBRARY_API NullTracer
  {
    friend class FunctionTrace;
  public:
    SIZE_T AddConsoleSink()
    { return 0; }
    SIZE_T AddSink(IO::TextStream& S)
    { UNREFERENCED(S); return 0; }
    SIZE_T AddSink(IO::BinaryStream& S)
    { UNREFERENCED(S); return 0; }
    SIZE_T AddSink(IO::StandardStream& S)
    { UNREFERENCED(S); return 0; }
    SIZE_T AddSink(Ptr<IO::File> F)
    { UNREFERENCED(F); return 0; }
    SIZE_T AddSink(ITraceSink* pTrace)
    { UNREFERENCED(pTrace); return 0; }

    VOID RemoveSink(SIZE_T i)
    { UNREFERENCED(i); }

    VOID operator() (LPCTSTR Format, ...)
    { UNREFERENCED(Format); }
    VOID operator() (DWORD TraceLevel, LPCVOID ObjectId, LPCTSTR Format, ...)
    { UNREFERENCED(TraceLevel); UNREFERENCED(ObjectId); UNREFERENCED(Format); }

    VOID Indent()
    {}
    VOID UnIndent()
    {}

    VOID Lock()
    {}
    VOID Unlock()
    {}
  };

#if defined(COMPILER_NVCC)
  #if defined(EMULATION)
  #define CudaTrace Trace
  #else
  #define CudaTrace(Format, ...) ;
  #endif
#endif

  extern LIBRARY_API Tracer Trace;
  extern LIBRARY_API Tracer& Trace0;

#if defined(DEBUG)
  extern LIBRARY_API Tracer& TraceD;
#else
  extern LIBRARY_API NullTracer TraceD;
#endif

#if CONFIG_TRACELEVEL > 0
  extern LIBRARY_API Tracer& Trace1;
#else
  extern LIBRARY_API NullTracer Trace1;
#endif

#if CONFIG_TRACELEVEL > 1
  extern LIBRARY_API Tracer& Trace2;
#else
  extern LIBRARY_API NullTracer Trace2;
#endif

#if CONFIG_TRACELEVEL > 2
  extern LIBRARY_API Tracer& Trace3;
#else
  extern LIBRARY_API NullTracer Trace3;
#endif

#if CONFIG_TRACELEVEL > 3
  extern LIBRARY_API Tracer& Trace4;
#else
  extern LIBRARY_API NullTracer Trace4;
#endif

#if CONFIG_TRACELEVEL > 4
  extern LIBRARY_API Tracer& Trace5;
#else
  extern LIBRARY_API NullTracer Trace5;
#endif

#if CONFIG_TRACELEVEL > 5
  extern LIBRARY_API Tracer& Trace6;
#else
  extern LIBRARY_API NullTracer Trace6;
#endif

#if CONFIG_TRACELEVEL > 6
  extern LIBRARY_API Tracer& Trace7;
#else
  extern LIBRARY_API NullTracer Trace7;
#endif

  class FunctionTrace
  {
  public:
    FunctionTrace(LPCVOID ObjectId, LPCTSTR pFunctionName);
    ~FunctionTrace();

    VOID Enter(LPCTSTR FunctionName, ...);
    VOID Leave();
    VOID Return(LPCTSTR TEXT(FunctionReturn), ...);
  private:
    Timer m_Timer;
    LPCVOID m_ObjectId;
    DWORD m_Flags;
    LPCTSTR m_pFunctionName;
  };


#define TraceError(ObjectId, Message, ...) Trace(TraceLevel::ERROR, ObjectId, Message, ##__VA_ARGS__);
#define TraceWarning(ObjectId, Message, ...) Trace(TraceLevel::WARNING, ObjectId, Message, ##__VA_ARGS__);
#define TraceInfo(ObjectId, Message, ...) Trace(TraceLevel::INFO, ObjectId, Message, ##__VA_ARGS__);

#define TraceDError(ObjectId, Message, ...) TraceD(TraceLevel::ERROR, ObjectId, Message, ##__VA_ARGS__);
#define TraceDWarning(ObjectId, Message, ...) TraceD(TraceLevel::WARNING, ObjectId, Message, ##__VA_ARGS__);
#define TraceDInfo(ObjectId, Message, ...) TraceD(TraceLevel::INFO, ObjectId, Message, ##__VA_ARGS__);

#if defined(COMPILER_MSVC)
#define TraceEnter(FunctionInfo, ...) Bolitho::FunctionTrace __FunctionTrace(this, TEXT(__FUNCTION__)); __FunctionTrace.Enter(TEXT(FunctionInfo), ##__VA_ARGS__);
#define TraceLeave() __FunctionTrace.Leave();
#define TraceReturn(FunctionReturn, ...) __FunctionTrace.Return(TEXT(FunctionReturn), ##__VA_ARGS__);

#define Trace0Enter(FunctionInfo, ...) Bolitho::FunctionTrace __FunctionTrace(this, TEXT(__FUNCTION__)); __FunctionTrace.Enter(TEXT(FunctionInfo), ##__VA_ARGS__);
#define Trace0Leave() __FunctionTrace.Leave();
#define Trace0Return(FunctionReturn, ...) __FunctionTrace.Return(TEXT(FunctionReturn), ##__VA_ARGS__);

#if defined(DEBUG)
#define TraceDEnter(FunctionInfo, ...) Bolitho::FunctionTrace __FunctionTrace(this, TEXT(__FUNCTION__)); __FunctionTrace.Enter(TEXT(FunctionInfo), ##__VA_ARGS__);
#define TraceDLeave() __FunctionTrace.Leave();
#define TraceDReturn(FunctionReturn, ...) __FunctionTrace.Return(TEXT(FunctionReturn), ##__VA_ARGS__);
#else
#define TraceDEnter(FunctionInfo, ...) ;
#define TraceDLeave() ;
#define TraceDReturn(FunctionReturn, ...) ;
#endif

#if CONFIG_TRACELEVEL > 0
#define Trace1Enter(FunctionInfo, ...) Bolitho::FunctionTrace __FunctionTrace(this, TEXT(__FUNCTION__)); __FunctionTrace.Enter(TEXT(FunctionInfo), ##__VA_ARGS__);
#define Trace1Leave() __FunctionTrace.Leave();
#define Trace1Return(FunctionReturn, ...) __FunctionTrace.Return(TEXT(FunctionReturn), ##__VA_ARGS__);
#else
#define Trace1Enter(FunctionInfo, ...) ;
#define Trace1Leave() ;
#define Trace1Return(FunctionReturn, ...) ;
#endif

#if CONFIG_TRACELEVEL > 1
#define Trace2Enter(FunctionInfo, ...) Bolitho::FunctionTrace __FunctionTrace(this, TEXT(__FUNCTION__)); __FunctionTrace.Enter(TEXT(FunctionInfo), ##__VA_ARGS__);
#define Trace2Leave() __FunctionTrace.Leave();
#define Trace2Return(FunctionReturn, ...) __FunctionTrace.Return(TEXT(FunctionReturn), ##__VA_ARGS__);
#else
#define Trace2Enter(FunctionInfo, ...) ;
#define Trace2Leave() ;
#define Trace2Return(FunctionReturn, ...) ;
#endif

#if CONFIG_TRACELEVEL > 2
#define Trace3Enter(FunctionInfo, ...) Bolitho::FunctionTrace __FunctionTrace(this, TEXT(__FUNCTION__)); __FunctionTrace.Enter(TEXT(FunctionInfo), ##__VA_ARGS__);
#define Trace3Leave() __FunctionTrace.Leave();
#define Trace3Return(FunctionReturn, ...) __FunctionTrace.Return(TEXT(FunctionReturn), ##__VA_ARGS__);
#else
#define Trace3Enter(FunctionInfo, ...) ;
#define Trace3Leave() ;
#define Trace3Return(FunctionReturn, ...) ;
#endif

#if CONFIG_TRACELEVEL > 3
#define Trace4Enter(FunctionInfo, ...) Bolitho::FunctionTrace __FunctionTrace(this, TEXT(__FUNCTION__)); __FunctionTrace.Enter(TEXT(FunctionInfo), ##__VA_ARGS__);
#define Trace4Leave() __FunctionTrace.Leave();
#define Trace4Return(FunctionReturn, ...) __FunctionTrace.Return(TEXT(FunctionReturn), ##__VA_ARGS__);
#else
#define Trace4Enter(FunctionInfo, ...) ;
#define Trace4Leave() ;
#define Trace4Return(FunctionReturn, ...) ;
#endif

#if CONFIG_TRACELEVEL > 4
#define Trace5Enter(FunctionInfo, ...) Bolitho::FunctionTrace __FunctionTrace(this, TEXT(__FUNCTION__)); __FunctionTrace.Enter(TEXT(FunctionInfo), ##__VA_ARGS__);
#define Trace5Leave() __FunctionTrace.Leave();
#define Trace5Return(FunctionReturn, ...) __FunctionTrace.Return(TEXT(FunctionReturn), ##__VA_ARGS__);
#else
#define Trace5Enter(FunctionInfo, ...) ;
#define Trace5Leave() ;
#define Trace5Return(FunctionReturn, ...) ;
#endif

#if CONFIG_TRACELEVEL > 5
#define Trace6Enter(FunctionInfo, ...) Bolitho::FunctionTrace __FunctionTrace(this, TEXT(__FUNCTION__)); __FunctionTrace.Enter(TEXT(FunctionInfo), ##__VA_ARGS__);
#define Trace6Leave() __FunctionTrace.Leave();
#define Trace6Return(FunctionReturn, ...) __FunctionTrace.Return(TEXT(FunctionReturn), ##__VA_ARGS__);
#else
#define Trace6Enter(FunctionInfo, ...) ;
#define Trace6Leave() ;
#define Trace6Return(FunctionReturn, ...) ;
#endif

#if CONFIG_TRACELEVEL > 6
#define Trace7Enter(FunctionInfo, ...) Bolitho::FunctionTrace __FunctionTrace(this, TEXT(__FUNCTION__)); __FunctionTrace.Enter(TEXT(FunctionInfo), ##__VA_ARGS__);
#define Trace7Leave() __FunctionTrace.Leave();
#define Trace7Return(FunctionReturn, ...) __FunctionTrace.Return(TEXT(FunctionReturn), ##__VA_ARGS__);
#else
#define Trace7Enter(FunctionInfo, ...) ;
#define Trace7Leave() ;
#define Trace7Return(FunctionReturn, ...) ;
#endif
#endif

#if defined(COMPILER_GCC)
#define TraceEnter(FunctionInfo, ...) Bolitho::FunctionTrace __FunctionTrace(this, __PRETTY_FUNCTION__); __FunctionTrace.Enter(TEXT(FunctionInfo), ##__VA_ARGS__);
#define TraceLeave() __FunctionTrace.Leave();
#define TraceReturn(FunctionReturn, ...) __FunctionTrace.Return(TEXT(FunctionReturn), ##__VA_ARGS__);

#define Trace0Enter(FunctionInfo, ...) Bolitho::FunctionTrace __FunctionTrace(this, __PRETTY_FUNCTION__); __FunctionTrace.Enter(TEXT(FunctionInfo), ##__VA_ARGS__);
#define Trace0Leave() __FunctionTrace.Leave();
#define Trace0Return(FunctionReturn, ...) __FunctionTrace.Return(TEXT(FunctionReturn), ##__VA_ARGS__);

#if CONFIG_TRACELEVEL > 0
#define Trace1Enter(FunctionInfo, ...) Bolitho::FunctionTrace __FunctionTrace(this, __PRETTY_FUNCTION__); __FunctionTrace.Enter(TEXT(FunctionInfo), ##__VA_ARGS__);
#define Trace1Leave() __FunctionTrace.Leave();
#define Trace1Return(FunctionReturn, ...) __FunctionTrace.Return(TEXT(FunctionReturn), ##__VA_ARGS__);
#else
#define Trace1Enter(FunctionInfo, ...) ;
#define Trace1Leave() ;
#define Trace1Return(FunctionReturn, ...) ;
#endif

#if CONFIG_TRACELEVEL > 1
#define Trace2Enter(FunctionInfo, ...) Bolitho::FunctionTrace __FunctionTrace(this, __PRETTY_FUNCTION__); __FunctionTrace.Enter(TEXT(FunctionInfo), ##__VA_ARGS__);
#define Trace2Leave() __FunctionTrace.Leave();
#define Trace2Return(FunctionReturn, ...) __FunctionTrace.Return(TEXT(FunctionReturn), ##__VA_ARGS__);
#else
#define Trace2Enter(FunctionInfo, ...) ;
#define Trace2Leave() ;
#define Trace2Return(FunctionReturn, ...) ;
#endif

#if CONFIG_TRACELEVEL > 2
#define Trace3Enter(FunctionInfo, ...) Bolitho::FunctionTrace __FunctionTrace(this, __PRETTY_FUNCTION__); __FunctionTrace.Enter(TEXT(FunctionInfo), ##__VA_ARGS__);
#define Trace3Leave() __FunctionTrace.Leave();
#define Trace3Return(FunctionReturn, ...) __FunctionTrace.Return(TEXT(FunctionReturn), ##__VA_ARGS__);
#else
#define Trace3Enter(FunctionInfo, ...) ;
#define Trace3Leave() ;
#define Trace3Return(FunctionReturn, ...) ;
#endif

#if CONFIG_TRACELEVEL > 3
#define Trace4Enter(FunctionInfo, ...) Bolitho::FunctionTrace __FunctionTrace(this, __PRETTY_FUNCTION__); __FunctionTrace.Enter(TEXT(FunctionInfo), ##__VA_ARGS__);
#define Trace4Leave() __FunctionTrace.Leave();
#define Trace4Return(FunctionReturn, ...) __FunctionTrace.Return(TEXT(FunctionReturn), ##__VA_ARGS__);
#else
#define Trace4Enter(FunctionInfo, ...) ;
#define Trace4Leave() ;
#define Trace4Return(FunctionReturn, ...) ;
#endif

#if CONFIG_TRACELEVEL > 4
#define Trace5Enter(FunctionInfo, ...) Bolitho::FunctionTrace __FunctionTrace(this, __PRETTY_FUNCTION__); __FunctionTrace.Enter(TEXT(FunctionInfo), ##__VA_ARGS__);
#define Trace5Leave() __FunctionTrace.Leave();
#define Trace5Return(FunctionReturn, ...) __FunctionTrace.Return(TEXT(FunctionReturn), ##__VA_ARGS__);
#else
#define Trace5Enter(FunctionInfo, ...) ;
#define Trace5Leave() ;
#define Trace5Return(FunctionReturn, ...) ;
#endif

#if CONFIG_TRACELEVEL > 5
#define Trace6Enter(FunctionInfo, ...) Bolitho::FunctionTrace __FunctionTrace(this, __PRETTY_FUNCTION__); __FunctionTrace.Enter(TEXT(FunctionInfo), ##__VA_ARGS__);
#define Trace6Leave() __FunctionTrace.Leave();
#define Trace6Return(FunctionReturn, ...) __FunctionTrace.Return(TEXT(FunctionReturn), ##__VA_ARGS__);
#else
#define Trace6Enter(FunctionInfo, ...) ;
#define Trace6Leave() ;
#define Trace6Return(FunctionReturn, ...) ;
#endif

#if CONFIG_TRACELEVEL > 6
#define Trace7Enter(FunctionInfo, ...) Bolitho::FunctionTrace __FunctionTrace(this, __PRETTY_FUNCTION__); __FunctionTrace.Enter(TEXT(FunctionInfo), ##__VA_ARGS__);
#define Trace7Leave() __FunctionTrace.Leave();
#define Trace7Return(FunctionReturn, ...) __FunctionTrace.Return(TEXT(FunctionReturn), ##__VA_ARGS__);
#else
#define Trace7Enter(FunctionInfo, ...) ;
#define Trace7Leave() ;
#define Trace7Return(FunctionReturn, ...) ;
#endif
#endif

}

