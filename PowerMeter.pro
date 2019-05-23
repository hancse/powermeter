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
    thirdparty/QCustomPlot200/qcustomplot.cpp

HEADERS += \
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
    thirdparty/QCustomPlot200/qcustomplot.h

FORMS += \
        src/mainwindow.ui \
    commonsrc/serialdialog.ui \
    commonsrc/tcpdialog.ui \
    src/modbusframe.ui \
    src/sqlframe.ui \
    commonsrc/loggingframe.ui \
    commonsrc/plotframe2.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += ./thirdparty/QCustomPlot200
INCLUDEPATH += ./thirdparty/json-develop/single_include/nlohmann
