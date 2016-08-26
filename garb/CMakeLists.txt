#------------------------------------------------------------------------------
# Top Level CMakeLists.txt for Blsimu Build
#
# 2nd August 2016 Schin Date'
#
# Imitating a CMakeList.txt, v 1.6 2010-12-08 for Geant4 Build by Ben Morgan 
#
# $Id: CMakeLists.txt,v 1.1 160802 SDate/SP8 $
#
#------------------------------------------------------------------------------
# CMakeLists.txt
# Module : Blsimu
# Package: Blsimu
#
#
#------------------------------------------------------------------------------
# - Enforce an out-of-source builds before anything else
#
    message("D...CMAKE_SOURCE_DIR : ${CMAKE_SOURCE_DIR}")
    message("D...CMAKE_BINARY_DIR : ${CMAKE_BINARY_DIR}")

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
set(CMAKE_VERBOSE_MSKEFILE 1)
message("D...PROJECT_BINARY_DIR :" ${PROJECT_BINARY_DIR})
message("D...PROJECT_SOURCE_DIR :" ${PROJECT_SOURCE_DIR})

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
set(CMAKE_VERBOSE_MSKEFILE 1)

#create static libraries
#--------------------------
add_subdirectory(Witpy)
message("..................Witpy subdir added<<<<<<<<<<")

#importing existing libraries
#-------------------------------
#5 add_library(Witgn STATIC IMPORTED GLOBAL)
#5 set_property(TARGET Witgn PROPERTY IMPORTED_LOCATION
#5 	${PROJECT_SOURCE_DIR}/Witgn/libwitgn.a)
#	/Users/schin/Data/notes/cpplib/Witgn/libwitgn.a)
#set_target_properties(Witgn PROPERTIES IMPORTED_LOCATION
#        "/Users/schin/Data/notes/cpplib/Witgn/libwitgn.a")

#add_library(Plnck STATIC IMPORTED GLOBAL) ################ Plnck is not a library
#set_property(TARGET Plnck PROPERTY IMPORTED_LOCATION
#    /Users/schin/Data/notes/cpplib/Plnck/include/plnck.h)


include_directories(
include
${PROJECT_SOURCE_DIR}
${PROJECT_SOURCE_DIR}/include
#${PROJECT_SOURCE_DIR}/Witgn/include
${PROJECT_SOURCE_DIR}/Witpy/include
${PROJECT_SOURCE_DIR}/Plnck/include
#${PROJECT_SOURCE_DIR}/Blsimu
#${PROJECT_SOURCE_DIR}/Blsimu/include
#${PROJECT_SOURCE_DIR}/Galeo/include
#${ROOT_INCLUDE_DIR}
)

link_directories(
/Library/Frameworks/Python.framework/Versions/3.4/lib/python3.4/config-3.4m
)
#link_directories(
#/Users/schin/Data/notes/prodev/cmake/testBuild/source/Witgn
#/Users/schin/Data/notes/prodev/cmake/testBuild/source/Galeo
#${ROOT_LIBRARY_DIR}
#)
#message("ROOT_LIBRARY_DIR:" ${ROOT_LIBRARY_DIR})

#extra packages in ROOT
#set(ROOT_LIBRARIES ${ROOT_LIBRARIES} -lEve)

#file(GLOB sources ${PROJECT_SOURCE_DIR}/source/Blsimu/src/*.cpp)
#file(GLOB headers ${PROJECT_SOURCE_DIR}/source/Blsimu/include/*.h)
#file(GLOB sources $(sources) ${PROJECT_SOURCE_DIR}/source/Blsimu/srcC/*.c)
#message("PROJECT_SOURCE_DIR:" ${PROJECT_SOURCE_DIR})
#set(headers main.h include/Blsimu.h include/EleTracer.h)
set(headers include/Blsimu.h include/EleTracer.h)
set(sources
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
#message("sources:" ${sources})



add_library(Blsimu STATIC
${sources}
)

#
#kill warnings   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<remove later
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
message("...Blsimu...CMAKE_C_FLAGS" ${CMAKE_C_FLAGS})
message("...Blsimu...CMAKE_CXX_FLAGS" ${CMAKE_CXX_FLAGS})


#create a source/executable
#==============================
set(MODULE_NAME Blsimu)
set(sources main.cpp)
set(headers main.h)
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<
include_directories(
include
#${PROJECT_SOURCE_DIR}/Witgn/include
${PROJECT_SOURCE_DIR}/Witpy/include
#/User/schin/Data/notes/cpplib/Witgn
${PROJECT_SOURCE_DIR}/Plnck/include
)
set(CMAKE_VERBOSE_MAKEFILE 1)
add_executable(${MODULE_NAME}.exe ${sources} ${headers})
target_link_libraries(${MODULE_NAME}.exe
	Witpy Blsimu python3.4m)
#	Witgn Blsimu)
#    Witgn Plnck Blsimu) ################ Plnck is not a library
set_target_properties(${MODULE_NAME}.exe
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})



if(FALSE)
#-------------------------------------------------------------

#create an executable
set(MODULE_NAME Blsimu)
#set(OUTPUT_BINARY_DIR ${PROJECT_BINARY_DIR}/${MODULE_NAME})
#execute_process(COMMAND mkdir ${OUTPUT_BINARY_DIR})
#add_executable(Blsimu.exe main.cpp main.h ${sources} ${headers})
#target_link_libraries(Blsimu.exe witgn galeo ${ROOT_LIBRARIES})
add_executable(${MODULE_NAME}.exe main.cpp main.h ${sources} ${headers})
#target_link_libraries(${MODULE_NAME}.exe witgn galeo ${ROOT_LIBRARIES})
#target_link_libraries(${MODULE_NAME}.exe witgn)
#target_link_libraries(${MODULE_NAME}.exe witpy)
target_link_libraries(${MODULE_NAME}.exe witpy)
set_target_properties(${MODULE_NAME}.exe
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/source)
#	RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BINARY_DIR})



# write run
file(APPEND ${PROJECT_BINARY_DIR}/examples/NS/Case1/useBlsimu/run
	"blsimu_SRCDIR=${PROJECT_SOURCE_DIR}/source/Blsimu\n")
file(APPEND ${PROJECT_BINARY_DIR}/examples/NS/Case1/useBlsimu/run
	"export blsimu_SRCDIR\n")
file(APPEND ${PROJECT_BINARY_DIR}/examples/NS/Case1/useBlsimu/run
#	"Blsimu_EXECUTABLE=../../../../source/Blsimu.exe\n")
	"${PROJECT_BINARY_DIR}/source/Blsimu.exe\n")
file(APPEND ${PROJECT_BINARY_DIR}/examples/NS/Case1/useBlsimu/run
	"export Blsimu_EXECUTABLE\n")
file(APPEND ${PROJECT_BINARY_DIR}/examples/NS/Case1/useBlsimu/run
	"\${Blsimu_EXECUTABLE}")

endif()
