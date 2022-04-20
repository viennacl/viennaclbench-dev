#USE_QT5 variable determines which qt will be used for building
#ON - uses Qt5 (default)
#OFF - uses Qt4
set(USE_QT5 ON CACHE BOOL "Use Qt5 is ON by default, set to OFF to use Qt4")

if(USE_QT5)
    cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
else()
    cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
endif()

# Set package properties 
project(ViennaCLBench)

# Disable -rdynamic
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

# Disable fPIC
if(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC" )
endif()

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
if(WIN32)
    set(GUI_TYPE WIN32)
endif(WIN32)

# Disable console on Mac
if(APPLE)
    set(GUI_TYPE MACOSX_BUNDLE)
endif(APPLE)

#Disable debug output
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_NO_DEBUG_OUTPUT")

if(MINGW)
     add_definitions(-O2 -D_FILE_OFFset_BITS=64 -fPIC) 
endif()

# Show all source files in project
file(GLOB_RECURSE MAKE_HEADERS_VISIBLE_SRC
    *.hpp *.h *.cpp *.qrc *.ui)
add_custom_target(MAKE_HEADERS_VISIBLE SOURCES ${MAKE_HEADERS_VISIBLE_SRC})

# Set release build:
if(DEFINED CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE})
else()
    set (CMAKE_BUILD_TYPE Release)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/external/viennacl-dev")

option(BUILD_STATIC_LIBARCHIVE "Build libarchive and link statically" OFF)

if(BUILD_STATIC_LIBARCHIVE)
    option(ENABLE_BZip2 OFF)
    option(ENABLE_CAT OFF)
    option(ENABLE_CNG OFF)
    option(ENABLE_CPIO OFF)
    option(ENABLE_EXPAT OFF)
    option(ENABLE_ICONV OFF)
    option(ENABLE_INSTALL OFF)
    option(ENABLE_LIBB2 OFF)
    option(ENABLE_LIXML2 OFF)
    option(ENABLE_LZ4 OFF)
    option(ENABLE_LZMA OFF)
    option(ENABLE_LZO OFF)
    option(ENABLE_OPENSSL OFF)
    option(ENABLE_PCREPOSIX OFF)
    option(ENABLE_TAR OFF)
    option(ENABLE_TEST OFF)
    option(ENABLE_XATTR OFF)
    option(ENABLE_ZLIB ON)
    option(ENABLE_ZSTD OFF)
    add_subdirectory(external/libarchive)
else()
    find_package(LibArchive REQUIRED)
endif()

if(USE_QT5)# Find the Qt5 libraries
    find_package(Qt5 COMPONENTS Core REQUIRED)
    find_package(Qt5 COMPONENTS Network REQUIRED)
    find_package(Qt5 COMPONENTS WebKitWidgets REQUIRED)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    find_package(Qt5 COMPONENTS Gui REQUIRED)
else()# Find the Qt4 libraries and set required variables
    # Note that it's Qt4, not QT4 or qt4
    find_package(Qt4 REQUIRED)
    # (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
    include(${QT_USE_FILE})
endif()

#find OpenCL
find_package(OpenCL REQUIRED)
include_directories(${OPENCL_INCLUDE_DIR})

#set ViennaCL to use OpenCL
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVIENNACL_WITH_OPENCL")

file(GLOB ViennaCLBench_Sources
    "src/benchmarks/*.cpp"
    "src/*.cpp"
    "src/ui/*.cpp"
    "src/ui/widgets/*.cpp"
)

file(GLOB ViennaCLBench_Headers
    "src/benchmarks/*.h"
    "src/benchmarks/*.hpp"
    "src/*.h"
    "src/*.hpp"
    "src/ui/*.h"
    "src/ui/*.hpp"
    "src/ui/widgets/*.h"
)

file(GLOB ViennaCLBench_Ui_Files
    "src/ui/*.ui"
    "src/ui/widgets/*.ui"
)

file(GLOB ViennaCLBench_Resources
    "resources/*.qrc"
)

#Generate ui headers
if(USE_QT5)
    qt5_wrap_ui(wrapped_ui_headers ${ViennaCLBench_Ui_Files})
else()
    qt4_wrap_ui(wrapped_ui_headers ${ViennaCLBench_Ui_Files})
endif()

#Add the resource files
if(USE_QT5)
    qt5_add_resources(added_resources ${ViennaCLBench_Resources})
else()
    qt4_add_resources(added_resources ${ViennaCLBench_Resources})
endif()

set(QCustomPlot_Sources
    src/qcustomplot/qcustomplot.cpp
    src/qcustomplot/qcustomplot.h
)

add_executable(ViennaCLBench ${GUI_TYPE} ${ViennaCLBench_Sources} ${ViennaCLBench_Headers} ${wrapped_ui_headers} ${QCustomPlot_Sources} ${added_resources})

# Link Qt
if(USE_QT5)# Qt5
    target_link_libraries(ViennaCLBench ${OPENCL_LIBRARIES})
    target_link_libraries(ViennaCLBench Qt5::Core Qt5::Gui Qt5::Network Qt5::Widgets Qt5::WebKitWidgets)
else()# Qt4
    target_link_libraries(ViennaCLBench ${QT_QTMAIN_LIBRARY} ${QT_LIBRARIES} ${OPENCL_LIBRARIES})
    target_link_libraries(ViennaCLBench Qt4::QtGui Qt4::QtCore Qt4::QtWebKit)
endif()

if(BUILD_STATIC_LIBARCHIVE)
    set(LIBARCHIVE_LIBRARY archive_static)
    add_dependencies(ViennaCLBench ${LIBARCHIVE_LIBRARY})
else()
    set(LIBARCHIVE_LIBRARY archive)
endif()

# Link libarchive
target_link_libraries(ViennaCLBench ${LIBARCHIVE_LIBRARY})

# Install binary
install(TARGETS ViennaCLBench RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
