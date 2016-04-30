QT      += core gui widgets testlib
TEMPLATE = app
TARGET   = Test
CONFIG  += testcase

include(../Core/core.pri)
HEADERS += CalibrationTest.h \
    CorrectionsTest.h
SOURCES += main.cpp \
           CalibrationTest.cpp \
    CorrectionsTest.cpp

DEFINES += SOURCE_DIR=\\\"$$PWD/\\\"


