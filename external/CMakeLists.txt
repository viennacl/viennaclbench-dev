#
# Build all ViennaCL Benchmark external library dependencies in one go
#

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

# Set package properties 
project(ViennaCL_Benchmark_Externals)

# Disable -rdynamic
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

# Disable fPIC
IF(MINGW)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC" )
ENDIF()

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

IF(MINGW)
  add_definitions(-O2 -D_FILE_OFFSET_BITS=64 -fPIC) 
ENDIF()

# Set release build:
IF(DEFINED CMAKE_BUILD_TYPE)
 SET (CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE})
ELSE()
 SET (CMAKE_BUILD_TYPE Release)
ENDIF()

# Includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
include_directories("${PROJECT_SOURCE_DIR}/libarchive/libarchive/")
include_directories("${PROJECT_SOURCE_DIR}/libarchive/test_utils/")
include_directories("${PROJECT_SOURCE_DIR}/zlib/")

#find_path(
#    zlibstatic_INCLUDE_DIR
#    zlib.h
#    HINTS "${PROJECT_SOURCE_DIR}/zlib/"
#)

# Add zlib
add_subdirectory("${PROJECT_SOURCE_DIR}/zlib")

set(BUILD_TESTING Off)
set(ENABLE_TEST Off)
set(ENABLE_ZLIB On)
#message(STATUS "ENABLE_TEST='${ENABLE_TEST}'")

# Define ZLIB_INCLUDE_DIR & ZLIB_LIBRARY
set(ZLIB_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/zlib/)

if(WIN32)
	set(ZLIB_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/zlib/libzlibstatic.a)
else()
	set(ZLIB_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/zlib/libzlibstatic.a) # Will this work on Linux?
endif()

# Add Libarchive
add_subdirectory("${PROJECT_SOURCE_DIR}/libarchive")
