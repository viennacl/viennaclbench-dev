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

#Handle warnings
#add_definitions(-Wall)
#add_definitions(-Wno-unused-local-typedefs)
#add_definitions(-Wno-unused-parameter)
add_definitions(-Wextra)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")


# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

add_definitions(-O2 -D_FILE_OFFSET_BITS=64 -fPIC) 

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

# Include Libarchive
include_directories("src/libarchive")
set(LIBARCHIVE_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/src/libarchiveDLL/)
#message(${LIBARCHIVE_LIBRARY})
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -llibarchive")

# Include ViennaCL
include_directories(" C:/Users/Namik/Documents/GitHub/viennacl-dev")
find_package(ViennaCL QUIET REQUIRED)
include_directories(${VIENNACL_INCLUDE_DIRS})

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

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost)

#find OpenCL
find_package(OpenCL REQUIRED)
#include_directories("C:/AMDAPPSDK/2.9/include")
include_directories(${OPENCL_INCLUDE_DIR})
#link_directories(${OPENCL_LIBRARIES})
#set(OPENCL_LIBRARIES "C:/AMDAPPSDK/2.9/lib/x86/OpenCL.lib")
#link_directories(${OPENCL_LIBRARIES})

#set ViennaCL to use OpenCL
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVIENNACL_WITH_OPENCL")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lOpenCL")

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

#if(OPENCL_FOUND)
#message(STATUS "FOUND OpenCL!")
#endif()

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
# Build the app!
    add_executable(ViennaCL_Benchmark ${ViennaCL_Benchmark_Sources} ${ViennaCL_Benchmark_Headers} ${wrapped_ui_headers} ${added_resources} )
IF(USE_QT5)# Qt5-specific linkage and module usage
    target_link_libraries(ViennaCL_Benchmark ${Boost_LIBRARIES} ${OPENCL_LIBRARIES} )
    qt5_use_modules(ViennaCL_Benchmark Core Gui Widgets WebKitWidgets)
ELSE() # Qt4-specific linkage and module usage
    target_link_libraries(ViennaCL_Benchmark ${Boost_LIBRARIES} ${QT_QTMAIN_LIBRARY} ${QT_LIBRARIES} ${OPENCL_LIBRARIES} )
    #qt4_use_modules(ViennaCL_Benchmark Core Gui)     #Does not work with CMake 2.8.7, was introduced in CMake 2.8.11. See discussion here: http://comments.gmane.org/gmane.comp.programming.tools.cmake.devel/6002
    target_link_libraries(ViennaCL_Benchmark Qt4::QtGui Qt4::QtCore Qt4:QtWebKit)
ENDIF()
target_link_libraries(ViennaCL_Benchmark)

endif()


