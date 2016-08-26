#------------------------------------------------------------------------------
# CMakeLists.txt
# Module : Witpy
# Package: 
#
#
#------------------------------------------------------------------------------
#cmake_minimum_required(VERSION 2.8)

#create a static library
#==============================
include_directories(
include
)

# Include Python
#-------------------------
# CPYFLAGS should be obtained as the output of a shell command "python3.x-config --cflags"
#set(CPYFLAGS
#-I/Library/Frameworks/Python.framework/Versions/3.4/include/python3.4m
#-fno-strict-aliasing -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-#prototypes -arch i386 -arch x86_64 -g
#)
#set(CPYFLAGS "${CPYFLAGS}" CACHE STRING "cmake command line input")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPYFLAGS}" )
add_definitions(
${CPYFLAGS}
-DWITPY_SRCDIR="${CMAKE_SOURCE_DIR}/Witpy/srcPy"
)

add_library(
Witpy
STATIC src/SetDoPrm.cpp)



