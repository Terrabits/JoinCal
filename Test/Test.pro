QT      += core gui widgets testlib
TEMPLATE = app
TARGET   = Test
CONFIG  += testcase

include($PWD/../Core/core.pri)
HEADERS += CalibrationTest.h \
           CorrectionsTest.h \
           JoinCalibrationsTest.h \
           GetCalDialogTest.h \
           CalibrationSourceTest.h \
           CalSortTest.h
SOURCES += main.cpp \
           CalibrationTest.cpp \
           CorrectionsTest.cpp \
           JoinCalibrationsTest.cpp \
           GetCalDialogTest.cpp \
           CalibrationSourceTest.cpp \
           CalSortTest.cpp

DEFINES += SOURCE_DIR=\\\"$$PWD/\\\"
