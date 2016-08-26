#------------------------------------------------------------------------------
# CMakeLists.txt
# Module : Witpy
# Package: 
#
#
#------------------------------------------------------------------------------
#cmake_minimum_required(VERSION 2.8)

#create a static library
#-------------------------

include_directories(
include
)

# Include Python
# CPYFLAGS should be known here
#set(CPYFLAGS "${CPYFLAGS}" CACHE STRING "cmake command line input")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPYFLAGS}" )
add_definitions(
${CPYFLAGS}
-DWITPY_SRCDIR="${CMAKE_SOURCE_DIR}/Witpy/srcPy"
)

add_library(
Witpy
STATIC src/SetDoPrm.cpp)



