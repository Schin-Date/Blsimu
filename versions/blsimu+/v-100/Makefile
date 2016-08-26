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
#	141021 applied for blsimu+
#	130930 created
#	130813 copied from MBP15::Galeo > to use with ROOT
#	130311 created  s.d.
#===========================================================
	U = main
main:
#	make library
	make compile PROGRAM=$(U)

all:
	make compile_all PROGRAM=main

#TLIBOBJS    = KleinNishina.o
WRKDIR = tmp

#TLIBRARY_NAME     = nishina
#TLIBRARY          = lib${TLIBRARY_NAME}.a

#TLIBDIR = /Users/schin/Data/notes/basics/LBCS/lib/cpp
TLIBDIR = .
#====================================================
### INCLUDES ###
#------ include opt
#INCLUDE = -I/opt/local/include
#------ include user headers
INCLUDE := $(INCLUDE) -I$(TLIBDIR)/include
#INCLUDE := $(INCLUDE) -I$(TLIBDIR)/Lortz/include
INCLUDE := $(INCLUDE) -I/Users/schin/Data/notes/cpplib/Witgn/include
INCLUDE := $(INCLUDE) -I/Users/schin/Data/notes/cpplib/Plnck/include
INCLUDE := $(INCLUDE) -Iinclude -I.

### LIBS ###
#LIBS := -L./ -l${TLIBRARY_NAME} 
#LIBS := $(LIBS) -L./Lortz -llortz
LIBS := $(LIBS) -L/Users/schin/Data/notes/cpplib/Witgn -lwitgn
LIBS := $(LIBS) -L/opt/local/lib -lgsl
	

UOBJS    = <list your objects to be linked>
UOBJS    = optics.o lattice.o bendmag.o matrices.o scat_points.o stdran48.o\
           tagger.o bmgen.o gauss.o compt.o
# gauss.o\


LANGSFX=cc
LANGSFX=cxx
LANGSFX=cpp
COMPILER = g++
LINKER = g++

COMPOPT = $(ROOTCFLAGS)
#LINKOPT = -Wall
LINKOPT = 
GOPTION = -DDEBUG
GOPTION = 
#====================================================
SRC         = $(PROGRAM).$(LANGSFX)
OBJ         = $(PROGRAM).o
#EXEIMAGE    = $(PROGRAM)_cxx

#TARGET    = $(PROGRAM)_cxx
TARGET    = exe

LDFLAGS    = $(LIBS)

COMPOPT := $(COMPOPT) $(GOPTION)
LINKOPT := $(LINKOPT) $(GOPTION)

LIBSRCS = $(addprefix $(WRKDIR)/,$(TLIBOBJS))

ULIBSRC = $(TLIBDIR)/src
UOBJDIR = $(WRKDIR)
UOBJS := $(addprefix $(UOBJDIR)/,$(UOBJS))
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

$(UOBJDIR)/%.o :  $(UOBJDIR)/%.cpp
	$(COMPILER) -c $(COMPOPT) $< $(INCLUDE) -o $@
  
$(UOBJDIR)/%.o :  $(ULIBSRC)/%.cpp
	$(COMPILER) -c $(COMPOPT) $< $(INCLUDE) -o $@
  
$(UOBJDIR)/%.o :  src/%.cpp
	$(COMPILER) -c $(COMPOPT) $< $(INCLUDE) -o $@
  
$(UOBJDIR)/%.o :  %.cpp
	$(COMPILER) -c $(COMPOPT) $< $(INCLUDE) -o $@
  
$(UOBJDIR)/%.o :  src+/%.c
	gcc -c $(COMPOPT) $< $(INCLUDE) -o $@
#	gcc -Wall -c $(COMPOPT) $< $(INCLUDE) -o $@
  
.cpp.o:
	$(COMPILER) -c $(COMPOPT) $< $(INCLUDE) -o $@
  
.f.o:
	$(FCOMPILE) -c $(FCMPOPT) $(FINCLUDE) $<

.c.o:
	$(CCOMPILE) -c $(CCMPOPT) $(CINCLUDE) $<
