#-------------------------------------------------
#
# Project created by QtCreator 2014-05-24T07:28:11
#
#-------------------------------------------------

QT       += core gui webkit network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = ViennaCL_Benchmark
TEMPLATE = app

SOURCES += src/main.cpp \
    src/archiveextractor.cpp \
    src/benchmarks/benchmark_vector.cpp \
    src/benchmarks/benchmark_sparse.cpp \
    src/benchmarks/benchmark_solver.cpp \
    src/benchmarks/benchmark_scheduler.cpp \
    src/benchmarks/benchmark_blas3.cpp \
    src/benchmarks/benchmark_copy.cpp \
    src/benchmark_controller.cpp \
    src/benchmarks/benchmark_qr.cpp \
    src/benchmarks/abstractbenchmark.cpp \
    src/ui/mainwindow.cpp \
    src/ui/menulistwidget.cpp \
    src/ui/qcustomplot.cpp \
    src/ui/benchmarklistwidget.cpp \
    src/ui/collapsewidget.cpp \
    src/ui/matrixmarket_webview.cpp \
    src/ui/splashscreen.cpp \
    src/ui/matrixmarket_widget.cpp

HEADERS  +=    src/benchmark_controller.h \
    src/archiveextractor.h \
    src/benchmarks/benchmark-utils.hpp \
    src/benchmarks/benchmark_vector.h \
    src/benchmarks/benchmark_sparse.h \
    src/benchmarks/benchmark_solver.h \
    src/benchmarks/benchmark_scheduler.h \
    src/benchmarks/io.hpp \
    src/benchmarks/benchmark_blas3.h \
    src/benchmarks/Random.hpp \
    src/benchmarks/benchmark_copy.h \
    src/benchmarks/benchmark_qr.h \
    src/benchmarks/matrix_market.hpp \
    src/benchmarks/abstractbenchmark.h \
    src/ui/mainwindow.h \
    src/ui/qcustomplot.h \
    src/ui/menulistwidget.h \
    src/ui/benchmarklistwidget.h \
    src/ui/collapsewidget.h \
    src/ui/matrixmarket_webview.h \
    src/ui/splashscreen.h \
    src/ui/matrixmarket_widget.h

FORMS    += src/ui/mainwindow.ui \
    src/ui/collapsewidget.ui \
    src/ui/matrixmarket_widget.ui

RESOURCES += resources/otherFiles.qrc \
    resources/icons.qrc \
    resources/mmFiles.qrc

#Add benchmark data files
testData.path = $$OUT_PWD/testdata
testData.files += testdata/mat65k.mtx testdata/result65025.txt testdata/rhs65025.txt
INSTALLS += testData

#Notes on qmake variables
# $$myVar - user variable
# $${myVar} - supposed to be another version of user variable...
# $$(envVar) - system environment variable (at the time qmake is run)
# $(envVar) - system environment variable (at the time when makefile is processed)
# $$[varName] - qt config variable

#Add projects sources(MSVC requirement)
INCLUDEPATH += $$OUT_PWD
DEPENDPATH += $$OUT_PWD
INCLUDEPATH += $$OUT_PWD/src/ui
DEPENDPATH += $$OUT_PWD/src/ui
INCLUDEPATH += $$OUT_PWD/src/benchmarks
DEPENDPATH += $$OUT_PWD/src/benchmarks
INCLUDEPATH += .
DEPENDPATH += .

#Add Libarchive includes and dlls
INCLUDEPATH += libarchive/include
DEPENDPATH += libarchive/include
LIBS += "-L$$PWD/libarchive/bin/" -llibarchive

#Set Boost and ViennaCL include paths
BOOSTROOT = C:\boost\boost_1_55_0
VIENNACLROOT = C:\Users\Namik\Documents\GitHub\viennacl-dev

#Add Boost and ViennaCL include folders
INCLUDEPATH += $$VIENNACLROOT
INCLUDEPATH += $$BOOSTROOT
INCLUDEPATH += $$BOOSTROOT"\stage\lib"

DEPENDPATH += $$VIENNACLROOT
DEPENDPATH += $$BOOSTROOT
LIBS += "-L$$BOOSTROOT'/stage/lib/'"

#Disable unused warnings that come from Boost and QCustomPlot
#CONFIG += warn_off
gcc{
    QMAKE_CXXFLAGS += -Wno-unused-local-typedefs
#    QMAKE_CXXFLAGS += -Wno-unused-parameter
}

#Define Boost variables
DEFINES += BOOST_ALL_DYN_LINK

#Find OpenCL root folder
OPENCLROOT = $$(OPENCLROOT)

isEmpty(OPENCLROOT){
    OPENCLROOT = "C:\AMDAPPSDK\2.9"
    message("OpenCL not found in evironment, using hard-coded path: "$$OPENCLROOT)
} else{
    message("Automatically detected OpenCL: "$$OPENCLROOT)
}

#Find OpenCL headers include folder
OPENCL_INCLUDE_DIR += $$OPENCLROOT"/include"

isEmpty(OPENCL_INCLUDE_DIR){
    OPENCL_INCLUDE_DIR += "C:\AMDAPPSDK\2.9\include"
    message("Including hard-coded OpenCL headers folder: "$$OPENCL_INCLUDE_DIR)
} else{
    message("Automatically including OpenCL headers folder: "$$OPENCL_INCLUDE_DIR)
}

#Find OpenCL library folder
OPENCL_LIBRARIES = $$OPENCLROOT"/lib/x86"

isEmpty(OPENCL_LIBRARIES){
    OPENCL_LIBRARIES = "C:\AMDAPPSDK\2.9\include\lib\x86"
    message("Using hard-coded OpenCL library folder: "$$OPENCL_LIBRARIES)
} else{
    message("Automatically detected OpenCL library folder: "$$OPENCL_LIBRARIES)
}

#Release
win32:CONFIG(release, debug|release){
    msvc:QMAKE_CXXFLAGS += /MD #Force MSVC to use Dynamic Libs
    msvc:QMAKE_LFLAGS_RELEASE = /INCREMENTAL:NO
    #Enable OpenCL
    QMAKE_CXXFLAGS += -DVIENNACL_WITH_OPENCL
    message("OpenCL library path: "$$OPENCL_LIBRARIES)
    LIBS += -L$${OPENCL_LIBRARIES} -lOpenCL
    message("This is a Release build")
}
#Debug
else:win32:CONFIG(debug, debug|release){
    msvc:QMAKE_CXXFLAGS += /MDd
    message("This is a Debug build")
    #Do not enable OpenCL
}
else:unix:{
    LIBS += -L$${OPENCL_LIBRARIES} -lOpenCL
}

INCLUDEPATH += $${OPENCL_LIBRARIES}
DEPENDPATH += $${OPENCL_LIBRARIES}
