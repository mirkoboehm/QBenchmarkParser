QT       += core
QT       -= gui

TARGET = QBenchmarkParser
QMAKE_CXXFLAGS += -std=c++0x
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

SOURCES += main.cpp

include (QBenchmarkParser.pri)

