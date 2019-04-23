# /********************************************************************************************
#  * File:		$URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Trunk/Makefile $
#  * Author:		$LastChangedBy: OBTUSE\matthew $
#  * Revision:		$Revision: 1363 $
#  * Last Updated:	$LastChangedDate: 2007-12-09 20:07:59 -0500 (Sun, 09 Dec 2007) $
#  ********************************************************************************************/

APPLICATIONSRC?=Src/
LIBRARYSRC?=Src/

LIBRARYNAME?=CodeLibrary
LIBRARYVERSION=1.0
LIBRARYBUILD=$(shell svnversion $(LIBRARYSRC) | cut -d: -f1 | tr -d MS)

APPLICATIONBUILD=$(shell svnversion $(APPLICATIONSRC) | cut -d: -f1 | tr -d MS)
APPLICATIONVERSION?=1.0

LIBRARYSOURCE?= \
	Object.cpp Type.cpp Exception.cpp PlatformException.cpp \
	Data/Xml/Reader.cpp Data/Xml/Writer.cpp \
	Debug/Assert.cpp Debug/Debug.cpp Debug/Trace.cpp \
	Text/String.cpp \
	Time/DateTime.cpp Time/Timer.cpp \
	IO/File.cpp IO/Stream.cpp IO/Path.cpp IO/Pipe.cpp IO/StdIO.cpp IO/MemoryMappedFile.cpp IO/BinaryStream.hpp IO/SerialPort.hpp \
	IO/MemoryStream.cpp \
	System/Configuration.cpp System/Application.cpp System/Module.cpp System/Process.cpp System/Version.cpp \
	System/Console.cpp System/ConsoleApplication.hpp \
	System/Threading/Mutex.cpp System/Threading/CriticalSection.cpp System/Threading/Thread.cpp System/Threading/ThreadPool.cpp \
	System/Threading/ConditionVariable.cpp System/Threading/Interlocked.cpp \
	Net/DNS.cpp Net/IPAddress.cpp Net/Socket.cpp Net/IPEndPoint.cpp \
	Parallel/Cuda/CudaException.hpp Parallel/Cuda/CudaDevice.cpp Parallel/Cuda/CudaContext.cpp Parallel/Cuda/CudaModule.hpp \
	Parallel/Cuda/CudaGlobal.cpp Parallel/Cuda/CudaTexture.cpp \
	Parallel/MPI/MPI.cpp Parallel/MPI/Communicator.cpp \
	Math/Math.cpp Math/Geometry/MinimalAreaTriangulation.cpp \
	Memory/Md5.cpp \
	Utility/Bits.cpp Utility/Sort.cpp Utility/Traits.cpp Utility/NumberTraits.cpp Utility/Progress.hpp \
	Memory/VirtualMemory.cpp Memory/Memory.cpp \
	Graphics/Color.cpp Graphics/Image.cpp Graphics/Mesh.cpp Graphics/MeshAlgorithms.cpp Graphics/MeshIO.cpp Graphics/MeshPly.cpp \


OS=$(shell uname -s)
ARCH?=$(shell uname -p)
NATIVEARCH=$(shell uname -p)

BIN=Bin/$(OS)/$(ARCH)/
OBJ=Obj/$(OS)/$(ARCH)/
INCLUDE +=/usr/include/ /usr/local/cuda/include /usr/include/libxml2 $(LIBRARYSRC) /usr/include/openmpi/1.2.4-gcc/

LIB +=/usr/local/cuda/lib /opt/intel/cce/10.1.008/lib /usr/lib64/openmpi/1.2.4-gcc
CC?=gcc
CXX?=g++
NVCC?=nvcc

CUFLAGS +=
CFLAGS += -fpermissive -pthread -fPIC
LFLAGS += -pthread -ldl -rdynamic -lxml2

ifeq ($(USECUDA),1)
LFLAGS += -lcuda -lcudart
endif

CUFLAGS_DEBUG += -DDEBUG
CFLAGS_DEBUG += -DDEBUG -g3
LFLAGS_DEBUG +=

CUFLAGS_RELEASE += -DRELEASE

ifeq ($(CC),icc)
	AR=xiar
  CFLAGS_RELEASE += -ipo -xhost -O3 -funroll-loops -fp-model fast -vec-report=0
  LFLAGS_RELEASE += -O3 
  CFLAGS_DEBUG += 
  LFLAGS_DEBUG += 
else
  CFLAGS_RELEASE += -O3 -funroll-loops -ffast-math
  LFLAGS_RELEASE += -O3
endif

ifeq ($(ARCH),i386)
  ifeq ($(NATIVEARCH),x86_64)
    CFLAGS += -m32
    LFLAGS += -m32
  endif
endif

ifeq ($(STATIC),1)
  CFLAGS += -static
  LFLAGS += -static
endif

ifeq ($(CUDAEMU),1)
  CUFLAGS += --device-emulation -DEMULATION
endif

LIBRARYOBJECTS=$(addprefix $(OBJ), $(addsuffix .o, $(basename $(LIBRARYSOURCE))))
APPLICATIONOBJECTS=$(addprefix $(OBJ), $(addsuffix .o, $(basename $(APPLICATIONSOURCE))))

DYNAMICLIBRARYTARGET=lib$(LIBRARYNAME).$(LIBRARYVERSION).$(LIBRARYBUILD).so
STATICLIBRARYTARGET=lib$(LIBRARYNAME).a

ifeq ($(STATIC),1)
  LINKLIBRARY=$(STATICLIBRARYTARGET)
else
  LINKLIBRARY=$(DYNAMICLIBRARYTARGET)
endif

TARGETS += $(BIN)$(LINKLIBRARY) $(BIN)$(APPLICATIONTARGET) $(addprefix $(BIN), $(OTHERTARGETS)) 
CLEAN += $(BIN)lib$(LIBRARYNAME).so $(BIN)lib$(LIBRARYNAME).$(LIBRARYVERSION).so

all: CFLAGS += $(CFLAGS_DEBUG)
all: CUFLAGS += $(CUFLAGS_DEBUG)
all: LFLAGS += $(LFLAGS_DEBUG)
all: $(TARGETS)

release: CFLAGS += $(CFLAGS_RELEASE)
release: CUFLAGS += $(CUFLAGS_RELEASE)
release: LFLAGS += $(LFLAGS_RELEASE)
release: $(TARGETS)

clean:
	@echo Cleaning...
	@rm -rf $(TARGETS)
	@rm -rf $(LIBRARYOBJECTS)
	@rm -rf $(APPLICATIONOBJECTS)
	@rm -rf $(CLEAN)

update:
	@echo Updating SVN...
	@svn update

tidy:
	@echo Tidying...
	@find -name "*~" | xargs /bin/rm -f

$(BIN)$(DYNAMICLIBRARYTARGET): $(LIBRARYOBJECTS)
	@echo Linking $@...
	@mkdir -p $(@D)
	@$(CXX) -shared -Wl,-soname,$(BIN)libCodeLibrary.$(LIBRARYVERSION).so -o $@ $(LIBRARYOBJECTS) $(LFLAGS) $(addprefix -L, $(LIB))
	@ln -sf $(DYNAMICLIBRARYTARGET) $(BIN)libCodeLibrary.so
	@ln -sf $(DYNAMICLIBRARYTARGET) $(BIN)libCodeLibrary.$(LIBRARYVERSION).so

$(BIN)$(STATICLIBRARYTARGET): $(LIBRARYOBJECTS)
	@echo Linking $@...
	@mkdir -p $(@D)
	@$(AR) rcs $@ $(LIBRARYOBJECTS)	

$(BIN)$(APPLICATIONTARGET): $(APPLICATIONOBJECTS) $(BIN)$(LINKLIBRARY)
	@echo Linking $@...
	@$(CXX) -o $@ $(APPLICATIONOBJECTS) $(LFLAGS) $(addprefix -L, $(LIB)) -L$(BIN) -lCodeLibrary


$(OBJ)%.o: $(LIBRARYSRC)%.c
	@echo Compiling $(<F)...
	@mkdir -p $(@D)
	@$(CC) -c -o $@ -DLIBRARYVERSION=$(LIBRARYVERSION).$(LIBRARYBUILD).0 $(CFLAGS) $(addprefix -I, $(INCLUDE)) $<

$(OBJ)%.o: $(LIBRARYSRC)%.cpp
	@echo Compiling $(<F)...
	@mkdir -p $(@D)
	@$(CXX) -c -o $@ -DLIBRARYVERSION=$(LIBRARYVERSION).$(LIBRARYBUILD).0 $(CFLAGS) $(addprefix -I, $(INCLUDE)) $<


$(OBJ)%.o: $(APPLICATIONSRC)%.c
	@echo Compiling $(<F)...
	@mkdir -p $(@D)
	@$(CC) -c -o $@ -DAPPLICATIONVERSION=$(APPLICATIONVERSION).$(APPLICATIONBUILD).0 $(CFLAGS) $(addprefix -I, $(INCLUDE)) $<

$(OBJ)%.o: $(APPLICATIONSRC)%.cpp
	@echo Compiling $(<F)...
	@mkdir -p $(@D)
	@$(CXX) -c -o $@ -DAPPLICATIONVERSION=$(APPLICATIONVERSION).$(APPLICATIONBUILD).0 $(CFLAGS) $(addprefix -I, $(INCLUDE)) $<

$(OBJ)%.o : $(APPLICATIONSRC)%.cu
	@echo Compiling $(<F)...
	@mkdir -p $(@D)
	@$(NVCC) -c -o $@ -DAPPLICATIONVERSION=$(APPLICATIONVERSION).$(APPLICATIONBUILD).0 $(CUFLAGS) $(addprefix -I, $(INCLUDE)) $<

$(BIN)%.cubin: $(APPLICATIONSRC)%.cu
	@echo Compiling $(<F)...
	@mkdir -p $(@D)
	@$(NVCC) -cubin -o $@ -DAPPLICATIONVERSION=$(APPLICATIONVERSION).$(APPLICATIONBUILD).0 $(CUFLAGS) $(addprefix -I, $(INCLUDE)) $<

