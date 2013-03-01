QT       += core

QT       -= gui

TARGET = QBenchmarkParser
QMAKE_CXXFLAGS += -std=c++0x
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app


SOURCES += main.cpp

HEADERS += \
    BenchmarkResult.h

SOURCES += \
    BenchmarkResult.cpp

HEADERS += \
    QTestlibXmlParser.h

SOURCES += \
    QTestlibXmlParser.cpp

HEADERS += \
    Exception.h

SOURCES += \
    Exception.cpp

OTHER_FILES += \
    autotests/testdata/sample_threadweaver/QBenchmarkParser.ini \
    autotests/testdata/sample_threadweaver/896c847.xml \
    autotests/testdata/sample_threadweaver/35acbd3.xml

HEADERS += \
    CSVOutputFormatter.h

SOURCES += \
    CSVOutputFormatter.cpp

RESOURCES += \
    autotests/testdata/sample_threadweaver/sample_threadweaver.qrc
