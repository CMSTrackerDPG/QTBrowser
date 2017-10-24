#-------------------------------------------------
#
# Project created by QtCreator 2017-10-20T17:00:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTBrowser
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += console

SOURCES += main.cpp\
    qrootcanvas.cpp \
    qtbrowser.cpp \
    superimposeplugin.cpp

HEADERS  += \
    tobjectcontainer.h \
    leafsortfilterproxymodel.h \
    qrootcanvas.h \
    previewqrootcanvas.h \
    qtbrowser.h \
    iplugin.h \
    superimposeplugin.h

FORMS    += \
    previewqrootcanvas.ui \
    qtbrowser.ui \
    superimposeplugin.ui

LIBS     += -L/usr/local/lib -lGui -lCore -lImt -lRIO -lNet \
            -lHist -lGraf -lGraf3d -lGpad -lTree -lTreePlayer \
            -lRint -lPostscript -lMatrix -lPhysics \
            -lMathCore -lThread -lMultiProc -pthread -lm -ldl -rdynamic
