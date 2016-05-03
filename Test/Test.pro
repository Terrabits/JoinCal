QT      += core gui widgets testlib
TEMPLATE = app
TARGET   = Test
CONFIG  += testcase

include(../Core/core.pri)
HEADERS += CalibrationTest.h \
    CorrectionsTest.h \
    JoinCalibrationsTest.h
SOURCES += main.cpp \
           CalibrationTest.cpp \
    CorrectionsTest.cpp \
    JoinCalibrationsTest.cpp

DEFINES += SOURCE_DIR=\\\"$$PWD/\\\"


