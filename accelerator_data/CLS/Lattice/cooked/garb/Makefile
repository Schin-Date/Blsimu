#===========================================================
# If this line appears, the original version is makes/Makefile_simplest
#111114 created by S. D. 
#
#   use this as %make U=<program_name>
#  ------------------------------------
#  put source.cc files under <USRCDIR>/
#  and header.hh files under include/
#===========================================================
include /sw/share/root/test/Makefile.arch

#INCLUDE = -I/sw/include -Iinclude 
INCLUDE = -I/sw/include
LIBDIR =  /sw/lib 
LIBS = -lgsl -lm
LIBS = $(ROOTGLIBS) -lgsl -lm

ULIBDIR = /Users/schin/Data/notes/cpplib
ULIBINC = -I$(ULIBDIR)/include -Iinclude
ULIBS = -lschinpp
ULIBS = 
USRCDIR = $(ULIBDIR)/src
UOBJS        = StrTable-v02.2.o StrManip-v02.1.o MyRoot-v02.3.o

INCLUDE := $(INCLUDE) $(ULIBINC)
#LIBS := $(LIBS) -L$(ULIBDIR)/libi386 $(ULIBS)
TMPDIR = tmp


PROGRAM      = main
PROGRAM      = $(U)
LANGSFX=cxx
LANGSFX=cc
LANGSFX=cpp
COMPILER = g++ -m32
LINKER = g++ -m32

COMPOPT = $(ROOTCFLAGS)
LINKOPT = 
GOPTION = -DDEBUG
GOPTION = 


#----------------------------------------------------
compile:
#	make exe
	@if [ ! -d $(TMPDIR) ]; then mkdir $(TMPDIR); \
      echo "tmp/ created...";fi
	touch $(UOBJS) $(OBJ)
	rm $(UOBJS) $(OBJ)
	make $(EXEIMAGE)
	@make cleano

clean:
	make cleanx
#----------------------------------------------------

SRC         = $(PROGRAM).$(LANGSFX)
OBJ         = $(PROGRAM).o
EXEIMAGE    = $(PROGRAM).exe
EXEIMAGE    = exe

LDFLAGS    = -L$(LIBDIR) $(LIBS)

COMPOPT := $(COMPOPT) $(GOPTION)
LINKOPT := $(LINKOPT) $(GOPTION)

UOBJDIR = $(TMPDIR)
UOBJS := $(addprefix $(UOBJDIR)/,$(UOBJS))

.SUFFIXES: 
.SUFFIXES: .exe .f .c .o .a .cpp .cxx .cc

#=================================================     
#   Select particular language by changing make
#   descriptions below.
#=================================================     
$(EXEIMAGE):  $(OBJ) $(UOBJS)
#	  @echo OBJ: $(OBJ)
#	  @echo UOBJS: $(UOBJS)
	$(LINKER) $(LINKOPT) $(OBJ) $(UOBJS) -o $(EXEIMAGE) $(LDFLAGS)


$(UOBJDIR)/%.o :  src/%.cpp
	$(COMPILER) -c $(COMPOPT) $< $(INCLUDE) -o $@
  
#$(UOBJDIR)/%.o :  $(USRCDIR)/%.cc
$(UOBJDIR)/%.o :  $(USRCDIR)/%.cpp
	$(COMPILER) -c $(COMPOPT) $< $(INCLUDE) -o $@
  
$(OBJ) :  $(SRC) 
	$(COMPILER) -c $(COMPOPT) $(SRC) $(INCLUDE) -o $@
  
.f.o:
	$(FCOMPILE) -c $(FCMPOPT) $(FINCLUDE) $<

.c.o:
	$(CCOMPILE) -c $(CCMPOPT) $(CINCLUDE) $<

.cpp.o:
	$(CPPCOMPILE) -c $(CPPCMPOPT) $(CPPINCLUDE) $<

.cxx.o:
	$(CPPCOMPILE) -c $(CPPCMPOPT) $(CPPINCLUDE) $<

.cc.o:
	$(CPPCOMPILE) -c $(CPPCMPOPT) $(CPPINCLUDE) $<

cleano :
	@ rm $(OBJ)

cleanx :
	 rm $(EXEIMAGE)

