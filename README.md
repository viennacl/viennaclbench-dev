<h1>ViennaCL Benchmark Readme</h1>
This is the developer repository of ViennaCL Benchmark GUI.
<h2>About ViennaCL Benchmark</h2>
The ViennaCL Benchmark GUI is a fresh project aiming to create a cross-platform standalone benchmarking program for the ViennaCL library. It will utilize the capabilities of ViennaCL to perform hardware benchmarking and collection of users' test results.<br>
The project is being developed by <a href="http://zalomiga.ba/">Namik Karovic</a> as a part of 2014 Google Summer of Code.<br>
ViennaCL was developed under the aegis of the Institute for Microelectronics at the Vienna University of Technology.<br>
For any technical questions related to ViennaCL, please use <a href="mailto:viennacl-support%40lists.sourceforge.net">our
mailing list</a>.<br>

<h2>Requirements</h2>
The following packages need to be available on your system:
  - Qt4 (tested with 4.8.5) or Qt5 (tested with 5.2.0 and 5.3.0)<br>
  - Qt Creator with QMake; alternatively, a CMake build is now available<br>
  - CMake 2.8.11 (Qt5) or 2.6 (Qt4)<br>
  - Boost 1.55<br>
  - OpenCL SDK (tested with AMD APP SDK 2.9)
Other dependencies are libarchive, zlib, and ViennaCL, which are cloned automatically through git and don't require extra preparation. 

<h2>System Environment Variables Prerequisites</h2>
<h3>Qt</h3>
When building with CMake, you should have Qt's bin folder in your path.
Also, you need to set the environment `CMAKE_PREFIX_PATH` to the respective build configuration of Qt, for example to `C:\Qt5\5.3\msvc2012_opengl`

When using QMake, have `QT_QMAKE_EXECUTABLE` system environment variable defined to point to qmake.exe location of your local Qt bin folder.<br>

<h3>Boost</h3>
Your system path should contain the path to Boost (e.g. `C:\boost\boost_1_55_0`)
<h3>OpenCL</h3>
If you wish to enable OpenCL usage in the benchmark, these steps should be followed:<br>
* Make sure you have defined the OPENCLROOT system environment variable. It should point to the root folder (containing the bin/, include/ and lib/ folders) of your OpenCL SDK (e.g. C:\AMDAPPSDK). This variable is used to automatically detect and enable OpenCL.<br>
* Make sure OpenCL.dll can be found in your system environment (AFAIK Windows' system32/ folder may contain a copy of OpenCL.dll; if this is not the case, add the bin/ folder of your OpenCL SDK to the system PATH - e.g. add the folder C:\AMDAPPSDK\bin to your PATH variable)<br>



<h2>Build Process</h2>
<h3>Qt Creator with the qmake build system (Windows):</h3>
1. open `ViennaCL_Benchmark.pro` with Qt Creator<br>
2. when prompted to configure project, choose a Qt4 or Qt5 kit (tested with 4.8.5, 5.2.0 and 5.3.0) and click Configure Project<br>
3. in the case you have not (properly) set your environment variables, you may edit the `ViennaCL_Benchmark.pro` file and manually change the project's include and depend paths for ViennaCL and Boost<br>
`BOOSTROOT = C:\boost\boost_1_55_0 ==> BOOSTROOT = yourPathToViennacl`<br>
4. in order to run solver and sparse benchmarks, you must add a make install step to your build process: <br>
In QtCreator, under `Projects>Build Steps>Make:>add` to Make arguments: install<br>
Do this for both debug and release modes<br>
5. you can now run the project (Ctrl+R)<br>

<h3>CMake build system - outside Qt Creator (Windows)</h3>
<b>Default CMakeLists.txt now supports both Qt4 and Qt5, with OpenCL support. The build will default to Qt5, but if you want to build with Qt4 set USE_QT5 variable to OFF.</b>
Before you launch the CMake GUI, please make sure that the environment `CMAKE_PREFIX_PATH` to the respective build configuration of Qt, for example to `C:\Qt5\5.3\msvc2012_opengl`.
Also, make sure your Qt-libraries can be found by the system (see above).
<br />
<h4>Preparing packages (only required once)</h4>
  1. Navigate into your project folder with the Git Bash and run `git submodule update --init` to clone the external repository viennacl-dev.
  2. Navigate to external and clone the release version of libarchive (we've seen build problems with the latest master branch): `git clone https://github.com/libarchive/libarchive && cd libarchive && git checkout v3.1.2`
  3. Navigate back to external and clone zlib: `git clone https://github.com/madler/zlib`
  4. Build zlib using CMake by using the project directory `/path/to/external/zlib` and *the same build directory*. Use the CMake GUI, click on 'Configure' twice and then on 'Generate', open the Visual Studio project file and compile the `zlibstatic` target. If you are using MinGW, call `make`.
  5. Build libarchive using CMake by using the project directory `/path/to/external/zlib` and *the same build directory*. Set ZLIB_INCLUDE_DIR to `../external/zlib` and `ZLIB_LIBRARY` to the `zlibstatic.lib` in either `../external/zlib` or one of the subfolders (typically Release or Debug)
  

<h4>Building the GUI</h4>
  2. Open CMakeLists.txt in the main repository folder with the CMake GUI<br>
  3. Create a subfolder build/ and select this as your build directory. This ensures that the repository tree does not get cluttered with build files.
  4. Click on 'Configure' and select your compiler. This should match the configuration you use with Qt when specifying the environment variable `CMAKE_PREFIX_PATH`. In the above example the compiler should be "Visual Studio 11". Note that by default Qt5 is shipped in 32bit, so you should *not* select 64-bit builds here.
  5. Click on "Configure" and wait for all checks to complete (might take a little).
  6. If CMake cannot detect the paths to Boost or OpenCL, specify them manually now.
  7. Click on "Configure" again. You should not get any errors now.
  8. Click on "Generate".
  9. Open the generated project file in build/ with your version of Visual Studio. If you use MinGW, navigate to the build folder in the terminal.<br>
  10. Compile the target `ViennaCL_Benchmark_Gui` in Visual Studio, or type 'make' in your MinGW terminal.<br>
  11, If you get linker errors in Visual Studio for Boost, specify the location of the dll files manually in the project configuration.
  12. You may now run the executable `ViennaCL_Benchmark`. Depending on your compiler, this may be located in the subfolder `Debug` or `Release` of your build directory.<br>

<h3>CMake build system - outside Qt Creator (Linux)</h3>
Make sure you have `libboost-all-dev` (or similar) as well as `libqt5-dev` or `libqt4-dev` installed through your package manager.
When using OpenCL you should also have `opencl-headers` and a proprietary graphics driver installed.
<br />
  1. Change into the viennacl-benchmark-gui folder<br />
  2. Run `git submodule update --init` to clone the external repository viennacl-dev.
  3. Grab and build zlib: `cd external && git clone https://github.com/madler/zlib && cd zlib && cmake . && make`
  4. Grab and build libarchive: `cd .. && git clone https://github.com/libarchive/libarchive && cd libarchive && cmake . -DENABLE_BZip2=Off && make` 
  5. Create the build folder in the main repository folder and change into it: `cd ../.. && mkdir build && cd build`<br />
  6. Run CMake. When using Qt5: `cmake ..` Use `cmake .. -DUSE_QT5=Off` if you only have Qt4 available on your system. <br />
  7. Build everyting: `make`
  8. Run the GUI: `./ViennaCL_Benchmark`




<h2>Important Notice</h2>
-When not using OpenCL: Blas3 benchmark is extremely slow. Takes about 5 minutes to complete on my machine. Don't run it if you can't wait; Solver benchmark crashes in Qt5 release builds without OpenCL.<br>
<h2>Troubleshooting</h2>
In case you get an error in QtGui/qopenglfunction.h : remove(or comment out) line 785 in that file `(void (QOPENGLF_APIENTRYP MemoryBarrier)(GLbitfield barriers););` it is a known Qt bug (https://github.com/go-qml/qml/issues/56) and should be fixed in Qt 5.3 (recently tested with Qt 5.3 - the bug is now gone)<br>
<h2>Additional Info</h2>
e main developer Namik is running a <a href="http://zalomiga.ba/blog">devblog</a> dedicated to this project and tries to update it as much as possible. Check it out for detailed reports and updates on the development process.
