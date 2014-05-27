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
    benchmarkcopy.cpp \
    benchmarkvector.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    benchmarkcopy.h \
    benchmarkvector.h

FORMS    += mainwindow.ui

INCLUDEPATH += C:\ViennaCL-1.5.1
INCLUDEPATH += C:\ViennaCL-1.5.1\examples\benchmarks
#INCLUDEPATH += C:\boost\boost_1_53_0
#INCLUDEPATH += ../qml/jbQuick/Charts

DEPENDPATH += C:\ViennaCL-1.5.1
DEPENDPATH += C:\ViennaCL-1.5.1\examples\benchmarks
#DEPENDPATH += C:\boost\boost_1_53_0

OTHER_FILES +=

RESOURCES += \
    qmlFiles.qrc
