<h1>ViennaCL Benchmark Readme</h1>
This is the developer repository of ViennaCL Benchmark GUI.
<h2>About ViennaCL Benchmark</h2>
The ViennaCL Benchmark GUI is a fresh project aiming to create a cross-platform standalone benchmarking program for the ViennaCL library. It will utilize the capabilities of ViennaCL to perform hardware benchmarking and collection of users' test results.<br>
The project is being developed by <a href="http://zalomiga.ba/">Namik Karovic</a> as a part of 2014 Google Summer of Code.<br>
ViennaCL was developed under the aegis of the Institute for Microelectronics at the Vienna University of Technology.<br>
For any technical questions related to ViennaCL, please use <a href="mailto:viennacl-support%40lists.sourceforge.net">our
mailing list</a>.<br>

<h2>Requirements - First Prototype</h2>
-Qt 5.2.0 or higher<br>
-Qt Creator 3.0.0 or higher<br>
-ViennaCL 1.5.2<br>
-Boost 1.55<br>
<h2>Build Process</h2>
The following are instructions on how to run the prototype with QtCreator:<br>
1. open ViennaCL_Benchmark.pro with Qt Creator<br>
2. wen prompted to configure project, select a kit that uses at least Qt 5.2.0. ( essentially any Qt version above 5.0.0 should work, but I only tested 5.2.0 ) and click Configure Project<br>
3. edit ViennaCL_Benchmark.pro file: change the project's include and depend paths for ViennaCL and Boost<br>
INCLUDEPATH += C:\ViennaCL-1.5.2 to INCLUDEPATH += yourPathToViennacl<br>
DEPENDPATH += C:\ViennaCL-1.5.2 to DEPENDPATH += yourPathToViennacl<br>
INCLUDEPATH += C:\boost\boost_1_55_0 to INCLUDEPATH += yourPathToBoost<br>
DEPENDPATH += C:\boost\boost_1_55_0 to DEPENDPATH += yourPathToBoost<br>
4. in order to run solver and sparse benchmarks, you must add a make install step to your build process: <br>
In QtCreator, under Projects>Build Steps>Make:>add to Make arguments: install<br>
Do this for both debug and release modes<br>
5. you can now run the project (Ctrl+R); make sure you've selected a kit with at least Qt 5.2.0<br>
<h2>Important Notice</h2>
-Blas3 benchmark is extremely slow. Takes about 5 minutes to complete on my machine. Don't run it if you can't wait.<br>
-Solver and Sparse benchmarks currently do not work in release node. Fix will be applied soon.<br>
<h2>Troubleshooting</h2>
In case you get an error in QtGui/qopenglfunction.h : remove(or comment out) line 785 in that file (void (QOPENGLF_APIENTRYP MemoryBarrier)(GLbitfield barriers);); it is a known Qt bug (https://github.com/go-qml/qml/issues/56) and should be fixed in Qt 5.3<br>
<h2>Additional Info</h2>
I'll be working on making the build process automated in the near future. I'm running a <a href="http://zalomiga.ba/blog">devblog</a> dedicated to this project and will try to update it as much as possible. Check it out for detailed reports on the development process.