#------------------------------------------------------------------------------
# Top Level CMakeLists.txt for Blsimu Build
#
# 2nd August 2016 Schin Date'
#
# Imitating a CMakeList.txt, v 1.6 2010-12-08 for Geant4 Build by Ben Morgan 
#
# $Id: CMakeLists.txt,v 1.1 160802 SDate/SP8 $
#
# Original sample is copied in prodev/cmake/samples/
#------------------------------------------------------------------------------
#
# - You should prepare this section for Python3
#===================================================
# --- set output of a shell command "python3.x-config --cflags" below
set(CPYFLAGS
-I/Library/Frameworks/Python.framework/Versions/3.4/include/python3.4m
-fno-strict-aliasing -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes -arch i386 -arch x86_64 -g
)
# --- quote output of a shell command "python3.x-config --ldflags" below
set(LDPYFLAGS
"-L/Library/Frameworks/Python.framework/Versions/3.4/lib/python3.4/config-3.4m -lpython3.4m -ldl -framework CoreFoundation"
)
#====== Basically you have no need to rewrite the following sections ========
#------------------------------------------------------------------------------
# - Enforce an out-of-source builds before anything else
#
    #message("D...CMAKE_SOURCE_DIR : ${CMAKE_SOURCE_DIR}")
    #message("D...CMAKE_BINARY_DIR : ${CMAKE_BINARY_DIR}")

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(STATUS "Blsimu cmake procedure requires an out-of-source build.")
    message(STATUS "Please remove these files from ${CMAKE_BINARY_DIR} first:")
    message(STATUS "CMakeCache.txt")
    message(STATUS "CMakeFiles")
    message(STATUS "Once these files are removed, create a separate directory")
    message(STATUS "and run CMake from there")
    message(FATAL_ERROR "in-source build detected")
endif()
#------------------------------------------------------------------------------
# - Define CMake requirements and override make rules as needed
#
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

#------------------------------------------------------------------------------
# - Project definition
#
project(Blsimu)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "-std=c++11")
#message("D...PROJECT_BINARY_DIR :" ${PROJECT_BINARY_DIR})
#message("D...PROJECT_SOURCE_DIR :" ${PROJECT_SOURCE_DIR})

#----------------------------------------------------------------------------
# - Prepend our own CMake Modules to the search path
# This allows us to customize, add, and factor out functionality
# NB: if our custom modules include others that we don't supply, those in the
# base path will be used, so watch for incompatibilities!!
#
#set(CMAKE_MODULE_PATH
#    ${PROJECT_SOURCE_DIR}/cmake/Modules
#    ${CMAKE_MODULE_PATH})

#----------------------------------------------------------------------------
# - Add functionality provided by standard and custom modules
# See the documentation in each of these modules for further details.
#
# - Versioning. We do this here for now
#set(${PROJECT_NAME}_VERSION "2.0")
#set(${PROJECT_NAME}_VERSION_MAJOR "2")
#set(${PROJECT_NAME}_VERSION_MINOR "0")
#set(${PROJECT_NAME}_VERSION_PATCH "2")

#----------------------------------------------------------------------------
# - Prepare directories
#
#set(LCSIMU_SOURCE_DIR "${PROJECT_SOURCE_DIR}/.." CACHE STRING "cmake command line input")

execute_process(COMMAND cp -r ${PROJECT_SOURCE_DIR}/accelerator_data ${PROJECT_BINARY_DIR})
execute_process(COMMAND cp -r ${PROJECT_SOURCE_DIR}/examples ${PROJECT_BINARY_DIR})

execute_process(COMMAND mkdir ${PROJECT_BINARY_DIR}/useBlsimu)
execute_process(COMMAND touch ${PROJECT_BINARY_DIR}/useBlsimu/readme)
file(APPEND ${PROJECT_BINARY_DIR}/useBlsimu/readme
	"copy contents of useBlsimu found in examples/...\n")
file(APPEND ${PROJECT_BINARY_DIR}/useBlsimu/readme
	"or create a new one by yourself.")

#======================================================================= build
#create static libraries in subdirectories
#--------------------------------------------
add_subdirectory(Witpy)
message("..................Witpy subdir added.")

#importing existing libraries
#--------------------------------------------
#5 add_library(Witgn STATIC IMPORTED GLOBAL)
#5 set_property(TARGET Witgn PROPERTY IMPORTED_LOCATION
#5 	<SourceDir>/Witgn/libwitgn.a)
#	/Users/schin/Data/notes/cpplib/Witgn/libwitgn.a)
#set_target_properties(Witgn PROPERTIES IMPORTED_LOCATION
#        "/Users/schin/Data/notes/cpplib/Witgn/libwitgn.a")

#build the main static library
#--------------------------------------------
include_directories(
include
${PROJECT_SOURCE_DIR}
${PROJECT_SOURCE_DIR}/include
${PROJECT_SOURCE_DIR}/Witpy/include
${PROJECT_SOURCE_DIR}/Plnck/include
#${ROOT_INCLUDE_DIR}
)
#message("D...PROJECT_SOURCE_DIR:" ${PROJECT_SOURCE_DIR})

#file(GLOB headers ${PROJECT_SOURCE_DIR}/source/Blsimu/include/*.h)
#file(GLOB sources ${PROJECT_SOURCE_DIR}/source/Blsimu/src/*.cpp)
#file(GLOB sources $(sources) ${PROJECT_SOURCE_DIR}/source/Blsimu/srcC/*.c)
set(headers main.h include/Blsimu.h include/EleTracer.h)
#3 set(headers include/Blsimu.h include/EleTracer.h)
set(sources
#3
main.cpp
src/Blsimu.cpp
src/EleTracer.cpp
srcC/bendmag.c
srcC/bmgen.c
srcC/compt.c
srcC/gauss.c
srcC/lattice.c
srcC/matrices.c
srcC/optics.c
srcC/scat_points.c
srcC/stdran48.c
srcC/tagger.c
)
#message("D...sources:" ${sources})

add_library(Blsimu STATIC
${sources}
)


#kill warnings
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
#message("...Blsimu...CMAKE_C_FLAGS" ${CMAKE_C_FLAGS})
#message("...Blsimu...CMAKE_CXX_FLAGS" ${CMAKE_CXX_FLAGS})


#create a source/executable
#--------------------------------------------
set(MODULE_NAME Blsimu)
#3 set(sources main.cpp)
#3 set(headers main.h)
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#link_directories(
#/Users/schin/Data/notes/prodev/cmake/testBuild/source/Witgn
#/Users/schin/Data/notes/prodev/cmake/testBuild/source/Galeo
#${ROOT_LIBRARY_DIR}
#)
#message("ROOT_LIBRARY_DIR:" ${ROOT_LIBRARY_DIR})

#extra packages in ROOT
#set(ROOT_LIBRARIES ${ROOT_LIBRARIES} -lEve)

# link Python library
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDPYFLAGS}" )

#
set(CMAKE_VERBOSE_MAKEFILE 1)

add_executable(${MODULE_NAME}.exe ${sources} ${headers})
target_link_libraries(${MODULE_NAME}.exe
	Witpy Blsimu)

set_target_properties(${MODULE_NAME}.exe
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
