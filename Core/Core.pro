QT           += core gui widgets
TEMPLATE      = lib
CONFIG       += staticlib
DESTDIR       = $$PWD/Library

win32: TARGET = Core
macx:  TARGET = libCore
CONFIG(debug, debug|release) {
       TARGET = $$join(TARGET,,,d)
}

include(../RsaToolbox/rsatoolbox.pri)
HEADERS += CalibrationWidget.h \
           CalibrationSource.h \
           Corrections.h \
           JoinCalibrations.h \
           Calibration.h \
           FrequencyRange.h \
    CalSourceWidget.h \
    GetCalDialog.h
SOURCES += CalibrationWidget.cpp \
           CalibrationSource.cpp \
           Corrections.cpp \
           JoinCalibrations.cpp \
           Calibration.cpp \
           FrequencyRange.cpp \
    CalSourceWidget.cpp \
    GetCalDialog.cpp
FORMS   += CalibrationWidget.ui \
    CalSourceWidget.ui \
    GetCalDialog.ui

DEFINES += SOURCE_DIR=\\\"$$PWD/\\\"
CONFIG(debug, debug|release):  DEFINES += DEBUG_MODE
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
