# /********************************************************************************************
#  * File:		$URL: svn://bolitho.us/Projects/CodeLibrary/Trunk/Makefile.Unix $
#  * Author:		$LastChangedBy: matthew $
#  * Revision:		$Revision: 675 $
#  * Last Updated:	$LastChangedDate: 2007-02-02 16:00:01 -0500 (Fri, 02 Feb 2007) $
#  ********************************************************************************************/


APPLICATIONTARGET=Reconstructor
APPLICATIONSRC=Reconstructor/Src/
LIBRARYSRC=Library/Src/
USECUDA=0
CFLAGS+=-Wno-deprecated
STATIC=1

CC=icc
CXX=icpc

APPLICATIONSOURCE=\
	SurfaceReconstruction.cpp \
	FunctionDataInfo.cpp \
	MarchingCubes.cpp \
	StreamingOctreeConstructor.cpp \
	StreamingOctreeTraversal.cpp \
	StreamingSurfaceExtractor.cpp \
	StreamingLaplacianSolver.cpp \
	StreamingPointConverter.cpp \
	DataStream.cpp \
	StreamManager.cpp \
	Tree.cpp \
	Octree.cpp

LIBRARYSOURCE=\
	Object.cpp Type.cpp Exception.cpp \
	Collections/HashMap.cpp \
	Debug/Assert.cpp Debug/Debug.cpp Debug/Trace.cpp \
	Text/String.cpp \
	Time/DateTime.cpp Time/Timer.cpp \
	IO/File.cpp IO/Stream.cpp IO/Path.cpp IO/Pipe.cpp IO/StdIO.cpp IO/BinaryStream.cpp IO/MemoryStream.cpp \
	Memory/Md5.cpp Memory/VirtualMemory.cpp Memory/Memory.cpp \
	PlatformException.cpp System/Application.cpp System/Module.cpp System/Process.cpp System/Version.cpp \
	System/Console.cpp System/ConsoleApplication.hpp System/Configuration.hpp \
	System/Threading/Mutex.cpp System/Threading/CriticalSection.cpp System/Threading/Thread.cpp System/Threading/Event.cpp System/Threading/Interlocked.cpp System/Threading/ThreadPool.cpp System/Threading/ConditionVariable.cpp \
	Math/Math.cpp Math/Geometry/MinimalAreaTriangulation.cpp \
	Utility/Traits.cpp Utility/NumberTraits.cpp Utility/Progress.hpp Utility/Bits.cpp 

include Library/CodeLibrary.mk


