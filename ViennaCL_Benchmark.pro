#-------------------------------------------------
#
# Project created by QtCreator 2014-05-24T07:28:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ViennaCL_Benchmark
TEMPLATE = app

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/qcustomplot.cpp \
    src/benchmarks/benchmark_vector.cpp \
    src/benchmarks/benchmark_sparse.cpp \
    src/benchmarks/benchmark_solver.cpp \
    src/benchmarks/benchmark_scheduler.cpp \
    src/benchmarks/benchmark_blas3.cpp \
    src/benchmarks/benchmark_copy.cpp \
    src/benchmark_controller.cpp \
    src/benchmarks/benchmark_qr.cpp \
    src/menulistwidget.cpp

HEADERS  += src/mainwindow.h \
    src/qcustomplot.h \
    src/benchmarks/benchmark-utils.hpp \
    src/benchmarks/benchmark_vector.h \
    src/benchmarks/benchmark_sparse.h \
    src/benchmarks/benchmark_solver.h \
    src/benchmarks/benchmark_scheduler.h \
    src/benchmarks/io.hpp \
    src/benchmarks/benchmark_blas3.h \
    src/benchmarks/Random.hpp \
    src/benchmarks/benchmark_copy.h \
    src/benchmark_controller.h \
    src/benchmarks/benchmark_qr.h \
    src/benchmarks/matrix_market.hpp \
    src/menulistwidget.h

FORMS    += src/mainwindow.ui


#Add include folders
#INCLUDEPATH += C:\AMDAPPSDK\2.9\include
INCLUDEPATH += C:\Users\Namik\Documents\GitHub\viennacl-dev
INCLUDEPATH += C:\boost\boost_1_55_0
#INCLUDEPATH += "C:\AMDAPPSDK\2.9\lib\x86_64"

#DEPENDPATH += C:\AMDAPPSDK\2.9\include
DEPENDPATH += C:\Users\Namik\Documents\GitHub\viennacl-dev
DEPENDPATH += C:\boost\boost_1_55_0
#DEPENDPATH += "C:\AMDAPPSDK\2.9\lib\x86_64"

#LIBS+=  -L"C:\AMDAPPSDK\2.9\lib\x86_64"
#        -L"C:\AMDAPPSDK\2.9\bin\x86_64"

#Disable unused warnings that come from Boost and QCustomPlot
#CONFIG += warn_off
QMAKE_CXXFLAGS += -Wno-unused-local-typedefs
QMAKE_CXXFLAGS += -Wno-unused-parameter

#Add benchmark data files
testData.path = $$OUT_PWD/testdata
testData.files += testdata/mat65k.mtx testdata/result65025.txt testdata/rhs65025.txt
INSTALLS += testData

RESOURCES += src/resources/otherFiles.qrc \
    src/resources/icons.qrc

#Enable OpenCL in ViennaCL
#QMAKE_CXXFLAGS += -DVIENNACL_WITH_OPENCL
#QMAKE_CXXFLAGS += -lOpenCL


#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../AMDAPPSDK/2.9/lib/x86_64/ -lOpenCL
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../AMDAPPSDK/2.9/lib/x86_64/ -lOpenCLd
#else:unix: LIBS += -L$$PWD/../../../../../AMDAPPSDK/2.9/lib/x86_64/ -lOpenCL

#INCLUDEPATH += $$PWD/../../../../../AMDAPPSDK/2.9/lib/x86_64
#DEPENDPATH += $$PWD/../../../../../AMDAPPSDK/2.9/lib/x86_64
