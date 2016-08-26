#===========================================================
#
#        The Standard Makefile for cpplib on 64 bit Intel Mac
#     to compile a target library (TLIBOBJS) with other libraries (e.g. witgn)
#     No root involvement.
#
#(Usage)
# 	make U=<program_name>;  for compiling a single program
#              <program_name> = bar for bar.cpp 
#	make ; for compiling all (listed below all:)
#
#(Default Library) 
#
#(History)
#	160725 compile witpy with python header
#	131028 fixed for .f.o and .c.o
#	130930 created
#	130813 copied from MBP15::Galeo > to use with ROOT
#	130311 created  s.d.
#===========================================================
main:
	make library
	make compile PROGRAM=$(U)

lib:
	make library

all:
	make compile_all PROGRAM=main

WRKDIR = tmp
TLIBOBJS    = SetDoPrm.o

TLIBRARY_NAME     = witpy
TLIBRARY          = lib${TLIBRARY_NAME}.a

TLIBDIR = .
APPDIR = devPy
#====================================================
### INCLUDES ###
#------ include the python header with options
INCLUDE := $(INCLUDE) -I/Library/Frameworks/Python.framework/Versions/3.4/include/python3.4m
INCLUDE := $(INCLUDE) -fno-strict-aliasing -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes -arch i386 -arch x86_64 -g
#------ include TLIBRARY headers
INCLUDE := $(INCLUDE) -I$(TLIBDIR)/include

### LIBS ###
#------ link python library
LIBS := $(LIBS) -L/Library/Frameworks/Python.framework/Versions/3.4/lib/python3.4/config-3.4m -lpython3.4m -ldl -framework CoreFoundation
#------ link this library
LIBS := -L$(TLIBDIR)/ -l${TLIBRARY_NAME} 
	

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
#COMPOPT = -DWITPY_SRCDIR='"/Users/schin/Data/notes/cpplib/Witpy/srcPy"'
this_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
#OK COMPOPT = -DWITPY_SRCDIR="\"$(this_DIR)/srcPy\""
COMPOPT = -DWITPY_SRCDIR='"$(this_DIR)/srcPy"'
#echo $(COMPOPT)
LINKOPT = 
GOPTION = -DDEBUG
GOPTION = 
#====================================================
SRC         = $(APPDIR)/$(PROGRAM).$(LANGSFX)
OBJ         = $(PROGRAM).o
#TARGET    = $(PROGRAM)_cxx
TARGET    = exe

LDFLAGS    = $(LIBS)

COMPOPT := $(COMPOPT) $(GOPTION)
LINKOPT := $(LINKOPT) $(GOPTION)

LIBSRCS = $(addprefix $(WRKDIR)/,$(TLIBOBJS))

OBJDIR = $(WRKDIR)
#APPOBJ := $(addprefix $(APPDIR)/$(WRKDIR)/,$(APPOBJ))
APPOBJ := $(addprefix $(WRKDIR)/,$(APPOBJ))
UOBJS := $(addprefix $(WRKDIR)/,$(UOBJS))
#----------------------------------------------------
library:
	@if [ ! -d $(WRKDIR) ]; then mkdir $(WRKDIR); \
	echo "## --> tmp/ created...";fi
#	@echo "## --> Making" $@ "..."
	rm -f ${TLIBRARY}
	rm -f ${LIBSRCS}
	make ${TLIBRARY}

${TLIBRARY}: ${LIBSRCS}
#	@echo "## --> Making" $@ "..."
	ar cru $@ ${LIBSRCS}
	ranlib $@
#	@echo "## <== Done"

compile:
	@if [ ! -d $(WRKDIR) ]; then mkdir $(WRKDIR); \
	echo "## --> tmp/ created...";fi
#	rm -f $(UOBJS)
	rm -f $(WRKDIR)/*
#	make $(ROOTLINKSRC)Dist.cpp
#	cp $(APPDIR)/$(APPOBJ) $(OBJDIR)
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
$(TARGET):  $(OBJ) $(UOBJS) $(APPOBJ)
#	  @echo OBJ: $(OBJ)
#	  @echo APPOBJ: $(APPOBJ)
	$(LINKER) $(LINKOPT) $(OBJ) $(UOBJS) $(APPOBJ) -o $@ $(LDFLAGS)

#$(ROOTLINKSRC)Dist.cpp: $(ROOTGUIHDRS) $(ROOTLINKSRC)LinkDef.h
#$(ROOTLINKSRC)Dist.cpp: $(ROOTGUIHDRS) $(APPDIR)/$(ROOTLINKSRC)LinkDef.h
#	rootcint -f $(WRKDIR)/$(ROOTLINKSRC)Dist.cpp -c $(INCLUDE) $(ROOTGUIHDRS) $(APPDIR)/$(ROOTLINKSRC)LinkDef.h 

#The following line must be the first one
$(OBJDIR)/%.o :  $(OBJDIR)/%.cpp
	$(COMPILER) -c $(COMPOPT) $< $(INCLUDE) -o $@
  
$(OBJDIR)/%.o :  ./%.cpp
	$(COMPILER) -c $(COMPOPT) $< $(INCLUDE) -o $@
  
$(OBJDIR)/%.o :  %.cpp
	$(COMPILER) -c $(COMPOPT) $< $(INCLUDE) -o $@
  
$(OBJDIR)/%.o :  src/%.cpp
	$(COMPILER) -c $(COMPOPT) $< $(INCLUDE) -o $@

$(OBJDIR)/%.o :  $(APPDIR)/%.cpp
	$(COMPILER) -c $(COMPOPT) $< $(INCLUDE) -o $@
    
$(OBJDIR)/%.o :  $(APPDIR)/src/%.cpp
	$(COMPILER) -c $(COMPOPT) $< $(INCLUDE) -o $@
    
%.o: $(APPDIR)/%.cpp
	$(COMPILER) -c $(COMPOPT) $< $(INCLUDE) -o $@
  
.cpp.o:
	$(COMPILER) -c $(COMPOPT) $< $(INCLUDE) -o $@
  
.f.o:
	$(FCOMPILE) -c $(FCMPOPT) $(FINCLUDE) $<

.c.o:
	$(CCOMPILE) -c $(CCMPOPT) $(CINCLUDE) $<
