#-------------------------------------------------
#
# Project created by QtCreator 2014-05-24T07:28:11
#
#-------------------------------------------------

QT       += core gui webkit network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

CONFIG -= console

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
    src/ui/matrixmarket_widget.cpp \
    src/benchmark_model.cpp \
    src/benchmarkinstance.cpp \
    src/ui/homescreen.cpp \
    src/ui/systeminfoscreen.cpp \
    src/ui/basicbenchmark.cpp \
    src/ui/expertbenchmark.cpp \
    src/benchmarksettings.cpp

HEADERS  +=    src/benchmark_controller.h \
    src/archiveextractor.h \
    src/benchmarks/benchmark-utils.hpp \
    src/benchmarks/benchmark_vector.h \
    src/benchmarks/benchmark_sparse.h \
    src/benchmarks/benchmark_solver.h \
    src/benchmarks/benchmark_scheduler.h \
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
    src/ui/matrixmarket_widget.h \
    src/benchmark_model.h \
    src/benchmarkinstance.h \
    src/ui/homescreen.h \
    src/ui/systeminfoscreen.h \
    src/ui/basicbenchmark.h \
    src/ui/expertbenchmark.h \
    src/benchmarksettings.h

FORMS    += src/ui/mainwindow.ui \
    src/ui/collapsewidget.ui \
    src/ui/matrixmarket_widget.ui \
    src/ui/homescreen.ui \
    src/ui/systeminfoscreen.ui \
    src/ui/basicbenchmark.ui \
    src/ui/expertbenchmark.ui

RESOURCES += resources/otherFiles.qrc \
    resources/icons.qrc \
    resources/mmFiles.qrc

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

#Add Libarchive includes
INCLUDEPATH += "./external/libarchive/libarchive"
DEPENDPATH += "./external/libarchive/libarchive"

#Note on adding libs with qmake
# -l is used to specify a single lib file
# -L is used to specify a folder containing targeted libs
# use -L in conjunction with -l to specify multiple lib files within a single folder
# (e.g. "-LC:/path/to/lib/folder" -lsomelibfile -lsomeotherlibfile )

#Add Libarchive & Zlib dynamically
#LIBS += "-L$$PWD/external/zlib" -llibzlib #Not needed if libarchive was linked with static zlib
LIBS += "-L$$PWD/external/libarchive/bin" -llibarchive

#Add Libarchive & Zlib statically
#LIBS += "-l$$PWD/external/zlib/libzlib.lib"
#LIBS += "-l$$PWD/external/libarchive/libarchive/libarchive.lib"
#Of course it doesn't work. Why should it work? It was never meant to work. I don't know why I even try.

#Add ViennaCL include folders
INCLUDEPATH += "./external/viennacl-dev"

#Disable unused warnings that come from QCustomPlot
#CONFIG += warn_off
gcc{
    QMAKE_CXXFLAGS += -Wno-unused-local-typedefs
#    QMAKE_CXXFLAGS += -DVIENNACL_DEBUG_ALL
#    QMAKE_CXXFLAGS += -Wno-unused-parameter
}

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
