Qt      += core gui widgets testlib
TEMPLATE = lib
TARGET   = Core
CONFIG  += staticlib
DESTDIR  = $$PWD/Library

include(../RsaToolbox/rsatoolbox.pri)
#HEADERS += 
#SOURCES +=

DEFINES += SOURCE_DIR=\\\"$PWD/\\\"