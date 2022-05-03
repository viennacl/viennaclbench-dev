# ViennaCL Benchmark Readme

This is the developer repository of ViennaCL Benchmark GUI.

## About ViennaCL Benchmark

The ViennaCL Benchmark GUI is a fresh project aiming to create a cross-platform standalone benchmarking program for the ViennaCL library. It will utilize the capabilities of ViennaCL to perform hardware benchmarking and collection of users' test results.

The project is being developed by [Namik Karovic](http://zalomiga.ba/) as a part of 2014 Google Summer of Code.

ViennaCL was developed under the aegis of the Institute for Microelectronics at the Vienna University of Technology.

For any technical questions related to ViennaCL, please use [our mailing list](mailto:viennacl-support%40lists.sourceforge.net).

## Requirements

The following packages need to be available on your system (similar versions may work):

- Qt5 (tested with 5.15.2).
- Qt Creator or CMake 2.8.11 (tested with 3.18.4)
- OpenCL SDK (tested with Khronos 2020.12.18).
- libarchive (you an also clone it in `external/` directory and link to it statically).

ViennaCL is cloned through Git and doesn't require extra preparation. 

## Getting the sources

```sh
git clone --recurse-submodules https://github.com/viennacl/viennaclbench-dev.git
```

If you have already cloned, you can fetch required ViennaCL this way:

```sh
cd viennaclbench-dev
git submodule update --init
```

## System Environment Variables Prerequisites

### Qt

When building with CMake, you should have Qt's bin folder in your path.
Also, you need to set the environment `CMAKE_PREFIX_PATH` to the respective build configuration of Qt, for example to `C:\Qt5\5.3\msvc2012_opengl`

When using QMake, have `QT_QMAKE_EXECUTABLE` system environment variable defined to point to qmake.exe location of your local Qt bin folder.<br>

### OpenCL

If you wish to enable OpenCL usage in the benchmark, these steps should be followed:

- Make sure you have defined the OPENCLROOT system environment variable. It should point to the root folder (containing the bin/, include/ and lib/ folders) of your OpenCL SDK (e.g. C:\AMDAPPSDK). This variable is used to automatically detect and enable OpenCL.
- Make sure OpenCL.dll can be found in your system environment (AFAIK Windows' system32/ folder may contain a copy of OpenCL.dll; if this is not the case, add the bin/ folder of your OpenCL SDK to the system PATH - e.g. add the folder C:\AMDAPPSDK\bin to your PATH variable)

## Build Process

### Qt Creator with the qmake build system (Windows):

_Those informations are from year 2014 and are very likely obsolete_.

1. open `ViennaCL_Benchmark.pro` with Qt Creator
2. when prompted to configure project, choose a Qt4 or Qt5 kit (tested with 4.8.5, 5.2.0 and 5.3.0) and click Configure Project
3. you can now run the project (Ctrl+R)

### CMake build system - outside Qt Creator (Windows)

_Those informations are from year 2014 and are very likely obsolete_.
**Default CMakeLists.txt now supports both Qt4 and Qt5, with OpenCL support. The build will default to Qt5, but if you want to build with Qt4 set USE_QT5 variable to OFF.**

Before you launch the CMake GUI, please make sure that the environment `CMAKE_PREFIX_PATH` to the respective build configuration of Qt, for example to `C:\Qt5\5.3\msvc2012_opengl`.

Also, make sure your Qt-libraries can be found by the system (see above).

#### Preparing packages (only required once)

1. Navigate into your project folder with the Git Bash and run `git submodule update --init` to clone the external repository viennacl-dev.
2. If you want to build statically against libarchive, navigate to external directory and clone the release version of libarchive (we've seen build problems with the latest master branch): `git clone https://github.com/libarchive/libarchive.git`
   - If you are on **Windows** you may have to use **Libarchive v3.1.2** (the latest master build was known to fail on Windows). Navigate to `external/libarchive` directory and checkout version 3.1.2: `cd external/libarchive && git checkout v3.1.2`
4. Use CMake to build zlib within its own source directory `../external/zlib` *(the build directory is the same as the source directory)*. Use the CMake GUI (if you wish to link zlib **statically on Windows**, make sure to turn **CMAKE_GNUtoMS ON**): click on 'Configure' twice and then on 'Generate', open the Visual Studio project file and compile the `zlibstatic` target. If you are using MinGW, call `make`( or `mingw32-make` on Windows ).
5. Use CMake to build libarchive within its own source directory `../external/libarchive` *(the build directory is the same as the source directory)*. Use CMake GUI (if you wish to link libarchive **statically on Windows**, make sure to turn **CMAKE_GNUtoMS ON**): set ZLIB_INCLUDE_DIR to `../external/zlib` and `ZLIB_LIBRARY` to the `zlibstatic.lib` (`libzlib.lib` on Windows) in either `../external/zlib` or one of the subfolders (typically Release or Debug)

#### Building the GUI

  1. Open CMakeLists.txt in the main repository folder with the CMake GUI.
  2. Create a subfolder build/ and select this as your build directory. This ensures that the repository tree does not get cluttered with build files.
  3. Click on 'Configure' and select your compiler. This should match the configuration you use with Qt when specifying the environment variable `CMAKE_PREFIX_PATH`. In the above example the compiler should be "Visual Studio 11". Note that by default Qt5 is shipped in 32bit, so you should *not* select 64-bit builds here.
  4. Click on "Configure" and wait for all checks to complete (might take a little).
  5. If CMake cannot detect the path to OpenCL, specify it manually now.
  6. If you want to build against libarchive statically, set BUILD_STATIC_LIBARCHIVE to ON.
  7. Click on "Configure" again. You should not get any errors now.
  8. Click on "Generate".
  9. Open the generated project file in build/ with your version of Visual Studio. If you use MinGW, navigate to the build folder in the terminal.<br>
  10. Compile the target `ViennaCL_Benchmark_Gui` in Visual Studio, or type 'make' in your MinGW terminal.
  11. You may now run the executable `ViennaCL_Benchmark`. Depending on your compiler, this may be located in the subfolder `Debug` or `Release` of your build directory.

### CMake build system - outside Qt Creator (Linux)

Make sure you have `libqt5-dev` or `libqt4-dev` installed through your package manager.

When using OpenCL you should also have `opencl-headers` and a proprietary graphics driver installed.

1. Change into the `viennaclbench-dev` folder:  
   `cd viennaclbench-dev`
2. Clone the external repository `viennacl-dev`:  
   `git submodule update --init`
3. Grab and build libarchive:    
`cd external && git clone https://github.com/libarchive/libarchive.git`.
4. Create the build folder in the main repository folder and change into it:  
   `cd ../.. && mkdir build && cd build`.
5. Run CMake:  
   `cmake ..`
   - Or tell CMake to build libarchive statically and link to it:  
     `cmake .. -DBUILD_STATIC_LIBARCHIVE=ON`
6. Build everyting:  
   `make`
7. Run the GUI:  
   `./ViennaCLBench`

## Important Notice

- When not using OpenCL: Blas3 benchmark is extremely slow. Takes about 5 minutes to complete, depending on the machine. Don't run it if you can't wait.

## Additional Info

The main developer Namik is running a [devblog](http://zalomiga.ba/blog) dedicated to this project and tries to update it as much as possible. Check it out for detailed reports and updates on the development process.
