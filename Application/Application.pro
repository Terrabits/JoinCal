#-------------------------------------------------
#
# Project created by QtCreator 2016-04-28T11:42:14
#
#-------------------------------------------------

QT       += core gui widgets
TARGET    = JoinCals
TEMPLATE  = app

include(../Core/core.pri)
SOURCES       += main.cpp \
                 MainWindow.cpp
HEADERS       += MainWindow.h
FORMS         += MainWindow.ui
RESOURCES     += resources.qrc
OTHER_FILES   += DEBUG_LOGFILE.txt
win32: RC_FILE = VS2010Resources.rc

                              DEFINES += SOURCE_DIR=\\\"$$PWD\\\"
CONFIG(debug, debug|release): DEFINES += DEBUG_MODE
