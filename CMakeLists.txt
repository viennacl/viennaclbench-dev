cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)

# Set package properties 
project(ViennaCL_Benchmark)

#disable -rdynamic
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

add_definitions(-O2 -D_FILE_OFFSET_BITS=64 -fPIC) 

file( GLOB_RECURSE MAKE_HEADERS_VISIBLE_SRC
    *.hpp *.h *.cpp *.qrc *.ui)
add_custom_target( MAKE_HEADERS_VISIBLE SOURCES ${MAKE_HEADERS_VISIBLE_SRC} )

# Set release build:
IF(DEFINED CMAKE_BUILD_TYPE)
 SET (CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE})
ELSE()
 SET (CMAKE_BUILD_TYPE Release)
ENDIF()

#Include ViennaCL
include_directories(" C:/Users/Namik/Documents/GitHub/viennacl-dev")
find_package(ViennaCL QUIET REQUIRED)
include_directories(${VIENNACL_INCLUDE_DIRS})

# Find the Qt libraries
#find_package(Qt4 COMPONENTS Core Gui Quick Qml REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5Qml REQUIRED)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost)

# Build the app!
# Tell CMake to create the executable
#add_executable(ViennaCL_Benchmark
 #   main.cpp
  #  mainwindow.cpp mainwindow.h
   # )

file(GLOB ViennaCL_Benchmark_Sources
    "benchmarks/*.cpp"
    "*.cpp"
)

file(GLOB ViennaCL_Benchmark_Headers
    "benchmarks/*.h"
    "benchmarks/*.hpp"
    "*.h"
    "*.hpp"
)

set( BENCHMARK_UIS
    mainwindow.ui)

#ADD_DEFINITIONS( -Wall )
add_definitions( -Wno-unused)


#Generate ui headers
#qt5_wrap_ui(ui_headers ${ViennaCL_Benchmark_Headers} ${ViennaCL_Benchmark_Sources} ${BENCHMARK_UIS})
qt5_wrap_ui(ui_headers ${BENCHMARK_UIS})

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
#    add_executable(ViennaCL_Benchmark main.cpp mainwindow.cpp mainwindow.h mainwindow.ui)
    add_executable(ViennaCL_Benchmark ${ViennaCL_Benchmark_Sources} ${ViennaCL_Benchmark_Headers} ${ui_headers} )
    target_link_libraries(ViennaCL_Benchmark ${Boost_LIBRARIES})
qt5_use_modules(ViennaCL_Benchmark Core Gui Widgets)
target_link_libraries(ViennaCL_Benchmark)

endif()


