/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/System/Application.hpp $
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

#pragma once

#include "Exception.hpp"
#include "System/Module.hpp"
#include "System/Threading/Thread.hpp"
#include "System/Process.hpp"
#include "System/Library.hpp"
#include "System/CommandLineParser.hpp"
#include "Time/Timer.hpp"

#if defined(PLATFORM_WINDOWS)
#include "System/Registry.hpp"
#endif

namespace Bolitho
{
  namespace System
  {
    struct CommandLineTokenType { enum { VALUE, KEYEDVALUE, FLAG }; };

    class LIBRARY_API CommandLineToken
    {
    public:
      CommandLineToken() {}
      CommandLineToken(LPCTSTR Name, LPCTSTR Value, DWORD Type) : m_Name(Name), m_Value(Value), m_Type(Type) {}

      DWORD Type()
      { return m_Type; }
      String Name()
      { return m_Name; }
      String Value()
      { return m_Value; }

      template<class T>
      T GetValue() CONST
      { return Parse<T>(m_Value); }
    private:
      String m_Name;
      String m_Value;
      DWORD m_Type;
    };

    class LIBRARY_API Application : public Module, public Runnable, public Thread, public Process
    {
      DeclareRuntimeType();

    public:
      Application();
      virtual ~Application() {}

      #if defined(PLATFORM_POSIX)
      /* Called internally by the framework */
      virtual VOID InitApplication(LPCTSTR VersionString);
      #else
      /* Called internally by the framework */
      virtual VOID InitApplication();
      #endif

      /* Loads a library module into the process */
      Ptr<Library> LoadLibrary(LPCTSTR FileName) CONST;

      /* Returns the command line used to launch the application */
      static String GetCommandLine();
      static StringArray GetCommandLineTokens();

      /* Returns a value from the applications environment variables */
      static String GetEnvironment(LPCTSTR Name);
      /* Sets the value in the applications environment variables */
      static VOID SetEnvironment(LPCTSTR Name, LPCTSTR Value);
      /* Terminates the application with a given exit code */
      static VOID Exit(UINT ExitCode);

      /* Called when an unhandled exception occurs */
      virtual VOID OnException(Exception& E);

      #if defined(PLATFORM_WINDOWS)
      /* The registry key that stores per user information for this application */
      Ptr<RegistryKey> GetUserSettingsKey()
      { return m_UserSettingsKey; }
      /* The registry key that stores per machine information for this application */
      Ptr<RegistryKey> GetMachineSettingsKey()
      { return m_MachineSettingsKey; }
      #endif

      /* A pointer to the (singleton) instance of the Application object */
      static Application* m_pApplication;
    protected:
      /* Called by the framework to initialize the application */
      virtual BOOL OnInit();
      /* Called by the framework to deinitialize the application */
      virtual VOID OnExit();

      virtual BOOL ThreadInit()
      { return OnInit(); }
      virtual VOID ThreadExit()
      { OnExit(); }
      virtual INT ThreadMain()
      { return 0; }

      /* Loads the application configuration values from the commandline, disk, registry and other sources */
      BOOL ParseConfiguration();
      /* Parses the command line into tokens */
      Array<CommandLineToken> ParseCommandLine();

      /* A general purpose timer.  The timer is started by the framework during initialization */
      Timer m_Timer;

      #if defined(PLATFORM_WINDOWS)
      /* The registry key that stores per user information for this application */
      Ptr<RegistryKey> m_UserSettingsKey;
      /* The registry key that stores per machine information for this application */
      Ptr<RegistryKey> m_MachineSettingsKey;
      #endif

      /* The name of the configuration file that stores per user information for this application */
      String m_UserConfigFile;
      /* The name of the configuration file that stores per machine information for this application */
      String m_MachineConfigFile;

    private:
      #if defined(PLATFORM_WINDOWS)
      STATIC LONG UnhandledSEHException(PEXCEPTION_POINTERS ExceptionPtrs);
      #if defined(USE_SOCKETS)
      WSADATA m_WinSockData;
      #endif
      #elif defined(PLATFORM_POSIX)
      STATIC VOID UnhandledSignal(INT Signal);
      #endif

    };
  }
}

#if defined(PLATFORM_WINDOWS)

#define DeclareApplication(x)                                                                     \
  INLINE x* GetApplication()                                                                      \
  {                                                                                               \
    return (x*)Bolitho::System::Application::m_pApplication;                                      \
  }                                                                                               \


#define ImplementApplicationEntryPoint(x)                                                         \
                                                                                                  \
  int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nShow)     \
  {                                                                                               \
    x App;                                                                                        \
    App.InitModule(hInstance);                                                                    \
    App.InitApplication();                                                                        \
    x::m_pApplication = &App;                                                                     \
                                                                                                  \
    try                                                                                           \
    {                                                                                             \
      if (!App.OnInit())                                                                          \
        return -1;                                                                                \
      INT R = App.Main();                                                                         \
      App.OnExit();                                                                               \
      return R;                                                                                   \
    }                                                                                             \
    catch(Bolitho::Exception& E)                                                                  \
    {                                                                                             \
      App.OnException(E);                                                                         \
      return -1;                                                                                  \
    }                                                                                             \
                                                                                                  \
  }                                                                                               \
                                                                                                  \
  int main(int argc, char* argv[])                                                                \
  {                                                                                               \
    INT Result = WinMain(GetModuleHandle(NULL), 0, 0, 0);                                         \
    return Result;                                                                                \
  }                                                                                               \

#endif

#if defined(PLATFORM_POSIX)

#define DeclareApplicationClass(x)                                                                \
  x* GetApplication();                                                                            \

#define STR2(x) #x
#define STR(x) STR2(x)


#define ImplementApplicationEntryPoint(x)				\
  x* g_pApp;								\
  CHAR* g_CommandLine;							\
									\
  int main(int argc, char* argv[])					\
  {									\
    x App;								\
    									\
    SIZE_T Length = 0;							\
    for (SIZE_T i=0; i<argc; i++)					\
      Length += Bolitho::CharTraits<CHAR>::Length(argv[i])+2;		\
    									\
    g_CommandLine = new CHAR[Length];					\
    CHAR* p = g_CommandLine;						\
    for (SIZE_T i=0; i<argc; i++)					\
      {									\
	Length = Bolitho::CharTraits<CHAR>::Length(argv[i]);		\
	CopyMemory(p, argv[i], Length);				\
	p += Length;							\
	*p = ' ';							\
	p++;								\
      }									\
    *p = 0;								\
    									\
    App.InitModule(dlopen(0, RTLD_LAZY));				\
    App.InitApplication(STR(APPLICATIONVERSION));			\
    g_pApp = &App;							\
    									\
    try									\
      {									\
	if (!App.OnInit())						\
	  return -1;							\
	INT R = App.Main();						\
	App.OnExit();							\
	return R;							\
      }									\
    catch(Bolitho::Exception& E)					\
      {									\
	g_pApp->OnException(E);						\
	return -1;							\
      }									\
    									\
  }									\
  									\
  x* GetApplication()							\
  {									\
    return g_pApp;							\
  }									\
  
#endif
