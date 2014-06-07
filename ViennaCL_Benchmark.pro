#-------------------------------------------------
#
# Project created by QtCreator 2014-05-24T07:28:11
#
#-------------------------------------------------

QT       += quick qml core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = ViennaCL_Benchmark
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    benchmarks/benchmark_vector.cpp \
    benchmarks/benchmark_sparse.cpp \
    benchmarks/benchmark_solver.cpp \
    benchmarks/benchmark_scheduler.cpp \
    benchmarks/benchmark_blas3.cpp \
    benchmarks/benchmark_generatorblas1.cpp \
    benchmarks/benchmark_copy.cpp \
    benchmarks/benchmark_generatorblas2.cpp \
    benchmarks/benchmark_generatorblas3.cpp \
    benchmark_controller.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    benchmarks/benchmark-utils.hpp \
    benchmarks/benchmark_vector.h \
    benchmarks/benchmark_sparse.h \
    benchmarks/benchmark_solver.h \
    benchmarks/benchmark_scheduler.h \
    benchmarks/io.hpp \
    benchmarks/benchmark_blas3.h \
    benchmarks/Random.hpp \
    benchmarks/benchmark_generatorblas1.h \
    benchmarks/benchmark_copy.h \
    benchmarks/benchmark_generatorblas2.h \
    benchmarks/benchmark_generatorblas3.h \
    benchmark_controller.h

FORMS    += mainwindow.ui

#INCLUDEPATH += C:\ViennaCL-1.5.2
#INCLUDEPATH += C:\ViennaCL-1.5.2\examples\benchmarks
INCLUDEPATH += C:\Users\Namik\Documents\GitHub\viennacl-dev
INCLUDEPATH += C:\Users\Namik\Documents\GitHub\viennacl-dev\examples\benchmarks
INCLUDEPATH += C:\boost\boost_1_55_0
#INCLUDEPATH += ../qml/jbQuick/Charts

#DEPENDPATH += C:\ViennaCL-1.5.2
#DEPENDPATH += C:\ViennaCL-1.5.2\examples\benchmarks
DEPENDPATH += C:\Users\Namik\Documents\GitHub\viennacl-dev
DEPENDPATH += C:\Users\Namik\Documents\GitHub\viennacl-dev\examples\benchmarks
DEPENDPATH += C:\boost\boost_1_55_0

#OTHER_FILES +=    testdata/mat65k.mtx \
#    testdata/result65025.txt \
#    testdata/rhs65025.txt

RESOURCES += dataFiles.qrc \
    otherFiles.qrc

CONFIG += warn_off

#QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter
#QMAKE_CXXFLAGS_WARN_OFF -= -Wno-unused-local-typedefs
#QMAKE_CFLAGS += -Wno-unused
#QMAKE_CFLAGS += -Wunused-local-typedefs
