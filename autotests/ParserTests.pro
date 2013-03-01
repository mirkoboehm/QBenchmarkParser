QT       += testlib
QT       -= gui

TARGET = tst_ParserTests
CONFIG   += console
CONFIG   -= app_bundle
include (../QBenchmarkParser.pri)
QMAKE_CXXFLAGS += -std=c++0x

TEMPLATE = app

RESOURCES += testdata/sample_threadweaver/sample_threadweaver.qrc

OTHER_FILES += \
    testdata/sample_threadweaver/QBenchmarkParser.ini \
    testdata/sample_threadweaver/896c847.xml \
    testdata/sample_threadweaver/35acbd3.xml

INCLUDEPATH += ../
SOURCES += tst_ParserTests.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
