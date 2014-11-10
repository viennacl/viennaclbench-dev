#USE_QT5 variable determines which qt will be used for building
#ON - uses Qt5 (default)
#OFF - uses Qt4
SET(USE_QT5 ON CACHE BOOL "Use Qt5 is ON by default, set to OFF to use Qt4")

IF(USE_QT5)
cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
ELSE()
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
ENDIF()

# Set package properties 
project(ViennaCL_Benchmark)

# Disable -rdynamic
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

# Disable fPIC
IF(MINGW)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC" )
ENDIF()

#Handle warnings
#add_definitions(-Wall)
#add_definitions(-Wno-unused-local-typedefs)
#add_definitions(-Wno-unused-parameter)
#add_definitions(-Wextra)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Disable console window
#set(QT_USE_QTMAIN TRUE)


# Disable console on Windows
IF(WIN32)
SET(GUI_TYPE WIN32)
ENDIF(WIN32)
# Disable console on Mac
IF(APPLE)
SET(GUI_TYPE MACOSX_BUNDLE)
ENDIF(APPLE)

#Disable debug output
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_NO_DEBUG_OUTPUT")

IF(MINGW)
  add_definitions(-O2 -D_FILE_OFFSET_BITS=64 -fPIC) 
ENDIF()

# Show all source files in project
file( GLOB_RECURSE MAKE_HEADERS_VISIBLE_SRC
    *.hpp *.h *.cpp *.qrc *.ui)
add_custom_target( MAKE_HEADERS_VISIBLE SOURCES ${MAKE_HEADERS_VISIBLE_SRC} )

# Set release build:
IF(DEFINED CMAKE_BUILD_TYPE)
 SET (CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE})
ELSE()
 SET (CMAKE_BUILD_TYPE Release)
ENDIF()

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
include_directories("${PROJECT_SOURCE_DIR}/external/libarchive/libarchive/")
include_directories("${PROJECT_SOURCE_DIR}/external/libarchive/test_utils/")
include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/external/viennacl-dev")

# find zlib for libarchive:
find_package(archive_static REQUIRED)
find_package(zlibstatic REQUIRED)


IF(USE_QT5)# Find the Qt5 libraries
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)

ELSE()# Find the Qt4 libraries and set required variables
# Note that it's Qt4, not QT4 or qt4
FIND_PACKAGE( Qt4 REQUIRED )  
# (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
INCLUDE( ${QT_USE_FILE} )
ENDIF()

#find OpenCL
find_package(OpenCL REQUIRED)
include_directories(${OPENCL_INCLUDE_DIR})

#set ViennaCL to use OpenCL
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVIENNACL_WITH_OPENCL")

file(GLOB ViennaCL_Benchmark_Sources
    "src/benchmarks/*.cpp"
    "src/*.cpp"
    "src/ui/*.cpp"
)

file(GLOB ViennaCL_Benchmark_Headers
    "src/benchmarks/*.h"
    "src/benchmarks/*.hpp"
    "src/*.h"
    "src/*.hpp"
    "src/ui/*.h"
    "src/ui/*.hpp"
)

file(GLOB ViennaCL_Benchmark_Ui_Files
    "src/ui/*.ui"
)

file(GLOB ViennaCL_Benchmark_Resources
    "resources/*.qrc"
)

#Generate ui headers
IF(USE_QT5)
qt5_wrap_ui(wrapped_ui_headers ${ViennaCL_Benchmark_Ui_Files})
ELSE()
qt4_wrap_ui(wrapped_ui_headers ${ViennaCL_Benchmark_Ui_Files} )
ENDIF()

#Add the resource files
IF(USE_QT5)
qt5_add_resources(added_resources ${ViennaCL_Benchmark_Resources} )
ELSE()
qt4_add_resources(added_resources ${ViennaCL_Benchmark_Resources} )
ENDIF()

add_executable(ViennaCL_Benchmark ${GUI_TYPE} ${ViennaCL_Benchmark_Sources} ${ViennaCL_Benchmark_Headers} ${wrapped_ui_headers} ${added_resources} )

# Link Qt
IF(USE_QT5)# Qt5
    target_link_libraries(ViennaCL_Benchmark ${OPENCL_LIBRARIES} )
    qt5_use_modules(ViennaCL_Benchmark Core Gui Widgets WebKitWidgets)
ELSE()# Qt4
    target_link_libraries(ViennaCL_Benchmark ${QT_QTMAIN_LIBRARY} ${QT_LIBRARIES} ${OPENCL_LIBRARIES} )
    #qt4_use_modules(ViennaCL_Benchmark Core Gui)     #Does not work with CMake 2.8.7, was introduced in CMake 2.8.11. See discussion here: http://comments.gmane.org/gmane.comp.programming.tools.cmake.devel/6002
    target_link_libraries(ViennaCL_Benchmark Qt4::QtGui Qt4::QtCore Qt4::QtWebKit)
ENDIF()

# Link Zlib and Libarchive
target_link_libraries(ViennaCL_Benchmark ${archive_static_LIBRARY} ${zlibstatic_LIBRARY})

