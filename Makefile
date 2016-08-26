#===========================================================
#
#        The Standard Makefile for cpplib on 64 bit Intel Mac
#       to compile a target library (TLIBOBJS) with other libraries (witgn)
#
#(Usage)
# 	make U=<program_name>;  for compiling a single program
#              <program_name> = bar for bar.cpp 
#	make ; for compiling all (listed below all:)
#
#(Default Library) 
#
#(History)
#	141125 renamed to Blsimu
#	141021 applied for blsimu+
#	130930 created
#	130813 copied from MBP15::Galeo > to use with ROOT
#	130311 created  s.d.
#===========================================================
APPDIR = <(User) Application>
APPDIR = .
APP_NAME = main
APPOBJS = main.o
APPSRC = .
	U = $(APP_NAME)
main:
	make compile PROGRAM=$(U)

lib:
	make library

all:
	make compile_all PROGRAM=$(U)

WRKDIR = tmp

TLIBDIR = <Target Library>
TLIBDIR = .
TLIBRARY_NAME     = Blsimu
TLIBRARY          = lib${TLIBRARY_NAME}.a
TLIBOBJS    = Blsimu.o EleTracer.o optics.o lattice.o bendmag.o matrices.o scat_points.o stdran48.o\
              tagger.o bmgen.o gauss.o compt.o
TLIBSRC = $(TLIBDIR)/src
TLIBSRCC = $(TLIBDIR)/srcC

ULIBDIR = <User Library>
ULIBDIR = Witpy
ULIBRARY_NAME = witpy
#ULIBOBJS = 
#====================================================
### INCLUDES ###
#------ Python3 header with options
Py3include = /Library/Frameworks/Python.framework/Versions/3.4/include/python3.4m
Py3option = -fno-strict-aliasing -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes -arch i386 -arch x86_64 -g
INCLUDE := $(INCLUDE) -I$(Py3include) $(Py3option)
#------ TLIBRARY headers
INCLUDE := $(INCLUDE) -I$(TLIBDIR)/include 
#------ ULIBRARY headers
INCLUDE := $(INCLUDE) -IPlnck/include
INCLUDE := $(INCLUDE) -I$(ULIBDIR)/include
#------ APP headers
INCLUDE := $(INCLUDE) -I$(APPDIR)


### LIBS ###
#------ Python3 library with options
Py3libDir = /Library/Frameworks/Python.framework/Versions/3.4/lib/python3.4/config-3.4m
Py3lib = python3.4m
LIBS := $(LIBS) -L$(Py3libDir) -l$(Py3lib) -ldl -framework CoreFoundation
#------ link User library
LIBS := $(LIBS) -L$(ULIBDIR)  -l$(ULIBRARY_NAME)
#------ link Target library
LIBS := $(LIBS) -L$(TLIBDIR) -l${TLIBRARY_NAME}

UOBJS    = <list your objects to be linked>
UOBJS    = 


LANGSFX=cc
LANGSFX=cxx
LANGSFX=cpp
COMPILER = g++
LINKER = g++
COMPILER = clang++
LINKER = clang++

COMPOPT = $(ROOTCFLAGS)
COMPOPT = 
LINKOPT = 
GOPTION = -DDEBUG
GOPTION = 
COMPOPT := $(COMPOPT) $(GOPTION)
LINKOPT := $(LINKOPT) $(GOPTION)
#====================================================
SRC         = $(PROGRAM).$(LANGSFX)
OBJ         = $(PROGRAM).o
#EXEIMAGE    = $(PROGRAM)_cxx
TARGET    = exe

TOBJDIR = $(WRKDIR)
TLIBOBJS := $(addprefix $(TOBJDIR)/,$(TLIBOBJS))

#UOBJDIR = $(WRKDIR)
#ULIBOBJS := $(addprefix $(UOBJDIR)/,$(ULIBOBJS))
#UOBJS := $(addprefix $(WRKDIR)/,$(UOBJS))

LDFLAGS    = $(LIBS)
#----------------------------------------------------
library:
	@if [ ! -d $(WRKDIR) ]; then mkdir $(WRKDIR); \
	echo "## --> tmp/ created...";fi
#	@echo "## --> Making" $@ "..."
	rm -f ${TLIBRARY}
	rm -f ${TLIBOBJS}
	make ${TLIBRARY}

compile:
	@if [ -f $(SRC) ]; \
	then make $(TARGET); \
	rm $(OBJ); \
	else make all; fi

compile_all:
	rm -f $(TARGET)
	make $(TARGET)
	rm $(OBJ)
#----------------------------------------------------

.SUFFIXES: 
.SUFFIXES: .exe .f .c .o .a .cpp .cxx .cc

#=================================================     
#   Select particular language by changing make
#   descriptions below.
#=================================================     
$(TARGET):  $(OBJ) $(UOBJS)
#	  @echo OBJ: $(OBJ)
#	  @echo UOBJS: $(UOBJS)
	$(LINKER) $(LINKOPT) $(OBJ) $(UOBJS) -o $@ $(LDFLAGS)

${TLIBRARY}: ${TLIBOBJS}
#	@echo "## --> Making" $@ "..."
	ar cru $@ ${TLIBOBJS}
	ranlib $@
#	@echo "## <== Done"


#$(TOBJDIR)/%.o :  $(TOBJDIR)/%.cpp
#	$(COMPILER) -c $(COMPOPT) $< $(INCLUDE) -o $@
  
$(TOBJDIR)/%.o :  $(TLIBSRC)/%.cpp
	$(COMPILER) -c $(COMPOPT) $< $(INCLUDE) -o $@
  
$(TOBJDIR)/%.o :  $(TLIBSRCC)/%.c
	$(COMPILER) -c $(COMPOPT) $< $(INCLUDE) -o $@
  
#$(TOBJDIR)/%.o :  $(TLIBSRCC)/%.c
#	gcc -c $(COMPOPT) $< $(INCLUDE) -o $@
##	gcc -Wall -c $(COMPOPT) $< $(INCLUDE) -o $@
  
#$(TOBJDIR)/%.o :  src/%.cpp
#	$(COMPILER) -c $(COMPOPT) $< $(INCLUDE) -o $@
  
#$(TOBJDIR)/%.o :  srcC/%.c
#	$(COMPILER) -c $(COMPOPT) $< $(INCLUDE) -o $@
  
#$(TOBJDIR)/%.o :  %.cpp
#	$(COMPILER) -c $(COMPOPT) $< $(INCLUDE) -o $@
  
.cpp.o:
	$(COMPILER) -c $(COMPOPT) $< $(INCLUDE) -o $@
  
.f.o:
	$(FCOMPILE) -c $(FCMPOPT) $(FINCLUDE) $<

.c.o:
	$(CCOMPILE) -c $(CCMPOPT) $(CINCLUDE) $<
