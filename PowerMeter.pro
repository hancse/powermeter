#-------------------------------------------------
#
# Project created by QtCreator 2018-11-20T11:29:35
#
#-------------------------------------------------

QT       += core gui
QT       += serialport serialbus sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PowerMeter
TEMPLATE = app

VERSION = 1.0.0
QMAKE_TARGET_COMPANY = Hogeschool va Arnhem en Nijmegen
QMAKE_TARGET_PRODUCT = PowerMeter - OEM
QMAKE_TARGET_DESCRIPTION = control of DEIF power meter
QMAKE_TARGET_COPYRIGHT = 2018-2019 PJM van Kan

#win32: RC_ICONS += images/lid.ico

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    commonsrc/aboutdialog.cpp \
    commonsrc/stripframe2.cpp \
    src/backendhandler.cpp \
    src/personcontainer.cpp \
        src/main.cpp \
        src/mainwindow.cpp \
    commonsrc/serialdialog.cpp \
    commonsrc/tcpdialog.cpp \
    src/modbusframe.cpp \
    src/deifmodbus.cpp \
    src/person.cpp \
    src/personjsonserializer.cpp \
    src/sqlframe.cpp \
    commonsrc/loggingframe.cpp \
    commonsrc/plotframe2.cpp \
    src/topframe.cpp \
    thirdparty/QCustomPlot200/qcustomplot.cpp

HEADERS += \
    commonsrc/aboutdialog.h \
    commonsrc/stripframe2.h \
    src/backendhandler.h \
    src/personcontainer.h \
    src/jsonserializable.h \
        src/mainwindow.h \
    commonsrc/serialdialog.h \
    commonsrc/tcpdialog.h \
    src/modbusframe.h \
    src/deifmodbus.h \
    src/person.h \
    src/personjsonserializer.h \
    src/sqlframe.h \
    commonsrc/loggingframe.h \
    commonsrc/plotframe2.h \
    src/topframe.h \
    thirdparty/QCustomPlot200/qcustomplot.h

FORMS += \
    commonsrc/aboutdialog.ui \
    commonsrc/stripframe2.ui \
        src/mainwindow.ui \
    commonsrc/serialdialog.ui \
    commonsrc/tcpdialog.ui \
    src/modbusframe.ui \
    src/sqlframe.ui \
    commonsrc/loggingframe.ui \
    commonsrc/plotframe2.ui \
    src/topframe.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += ./thirdparty/QCustomPlot200
INCLUDEPATH += ./thirdparty/json-develop/single_include/nlohmann

RESOURCES += \
    awesome5.qrc \
    powermeter.qrc

# compiled with MSVC2015_64 also works in MSVC2017_64
include ( C:\Qwt-6.1.4\features\qwt.prf )
#include ( C:\QwtPolar-1.1.1\features\qwtpolar.prf )
