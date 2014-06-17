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
    src/menuwidget.cpp

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
    src/menuwidget.h \
    src/benchmarks/matrix_market.hpp

FORMS    += src/mainwindow.ui \
    src/menuwidget.ui

#INCLUDEPATH += C:\ViennaCL-1.5.2
#INCLUDEPATH += C:\ViennaCL-1.5.2\examples\benchmarks
INCLUDEPATH += C:\Users\Namik\Documents\GitHub\viennacl-dev
#INCLUDEPATH += C:\Users\Namik\Documents\GitHub\viennacl-dev\examples\benchmarks
INCLUDEPATH += C:\boost\boost_1_55_0
#INCLUDEPATH += ../qml/jbQuick/Charts

#DEPENDPATH += C:\ViennaCL-1.5.2
#DEPENDPATH += C:\ViennaCL-1.5.2\examples\benchmarks
DEPENDPATH += C:\Users\Namik\Documents\GitHub\viennacl-dev
#DEPENDPATH += C:\Users\Namik\Documents\GitHub\viennacl-dev\examples\benchmarks
DEPENDPATH += C:\boost\boost_1_55_0

testData.path = $$OUT_PWD/testdata
testData.files += testdata/mat65k.mtx testdata/result65025.txt testdata/rhs65025.txt
INSTALLS += testData

RESOURCES += src/resources/otherFiles.qrc \

#CONFIG += warn_off

#QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter
#QMAKE_CXXFLAGS_WARN_OFF -= -Wno-unused-local-typedefs
#QMAKE_CFLAGS += -Wno-unused
#QMAKE_CFLAGS += -Wunused-local-typedefs
