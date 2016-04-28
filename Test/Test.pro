Qt      += core gui widgets testlib
TEMPLATE = app
TARGET   = Test
CONFIG  += testcase

include(../Core/core.pri)
#HEADERS += 
SOURCES += main.cpp

DEFINES += SOURCE_DIR=\\\"$PWD/\\\"