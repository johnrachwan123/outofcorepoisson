/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/Config.hpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 917 $
 * Last Updated: $Date: 2008-12-29 14:41:52 -0800 (Mon, 29 Dec 2008) $
 * 
 * 
 * Copyright (c) 2006-2007, Matthew G Bolitho;  Michael Kazhdan
 * All rights reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer. Redistributions in binary form must reproduce
 * the above copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the distribution. 
 * 
 * Neither the name of the Johns Hopkins University nor the names of its contributors
 * may be used to endorse or promote products derived from this software without specific
 * prior written permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 ***************************************************************************************************/

#pragma once

#define LIBRARYAPI_MINIMAL
#define LIBRARYAPI_STATIC
#define USE_XML

#include "CodeLibrary.hpp"
using Bolitho::Object;
using Bolitho::String;
using Bolitho::Buffer;
using Bolitho::StringArray;

#include "Debug/Assert.hpp"
#include "Debug/Trace.hpp"
using Bolitho::Trace;
using Bolitho::Trace1;
using Bolitho::Trace2;
using Bolitho::TraceD;
using Bolitho::TraceLevel;

#if defined(COMPILER_MSVC)
#include "Debug/MemoryChecker.hpp"
using Bolitho::DebugMemoryChecker;
#endif

#include "../Res/Resource.h"

#include "System/Configuration.hpp"
using Bolitho::System::Configuration;
using Bolitho::System::ConfigurationVariable;

#include "Debug/IntelThreadProfiler.hpp"

#if defined(COMPILER_MSVC)
#include <hash_map>
using stdext::hash_map;
#elif defined(COMPILER_GCC)
#include <ext/hash_map>
using namespace __gnu_cxx;

namespace __gnu_cxx
{
  template<> struct hash<LONGLONG> 
  {
    size_t operator()(LONGLONG x) const { return x; }
  };
  
  template<> struct hash<ULONGLONG> 
  {
    size_t operator()(ULONGLONG x) const { return x; }
  };
  
}
#endif

#define CONFIG_SOLVEDOUBLE
#define CONFIG_MULTITHREADED

#define CONFIG_REGIONDEPTH 4
#define CONFIG_REGIONRES (1<<CONFIG_REGIONDEPTH)
#define CONFIG_REGIONCOUNT ((1<<CONFIG_REGIONDEPTH)*(1<<CONFIG_REGIONDEPTH))

#define CONFIG_PROFILE
#define CONFIG_DIAG
//#define CONFIG_STOREDNEIGHBOURS

#include "Math/Vector3.hpp"
using Bolitho::Math::Vector3F;
typedef Vector3F InterpolatedAttribute;
#define InterpolatedAttributeEmpty InterpolatedAttribute()

#include "System/Threading/Interlocked.hpp"

extern Bolitho::System::CriticalSection g_Lock;

#if !defined(CONFIG_MULTITHREADED)
#define InterlockedAccumulate(x,y) (*x) += (y);
#define CONFIG_MAXTHREADS 1
#else
#define CONFIG_MAXTHREADS 8
#endif
