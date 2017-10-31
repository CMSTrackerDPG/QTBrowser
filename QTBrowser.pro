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

CONFIG += console static

SOURCES += main.cpp\
    qrootcanvas.cpp \
    qtbrowser.cpp \
    settings/settingsdialog.cpp \
    settings/settingsmanager.cpp \
    dqmfiledownloader/dqmfiledownloader.cpp \
    dqmfiledownloader/remotefilesmodel.cpp \
    plugins/superimposeplugin.cpp \
    plugins/concatinateplugin.cpp

HEADERS  += \
    qtbrowser.h \
    qrootcanvas.h \
    tobjectcontainer.h \
    leafsortfilterproxymodel.h \
    previewqrootcanvas.h \
    settings/settingsdialog.h \
    settings/settingsmanager.h \
    dqmfiledownloader/dqmfiledownloader.h \
    dqmfiledownloader/filecontainer.h \
    dqmfiledownloader/remotefilesmodel.h \
    plugins/iplugin.h \
    plugins/superimposeplugin.h \
    plugins/concatinateplugin.h

FORMS    += \
    previewqrootcanvas.ui \
    qtbrowser.ui \
    settings/settingsdialog.ui \
    dqmfiledownloader/dqmfiledownloader.ui \
    plugins/superimposeplugin.ui \
    plugins/concatinateplugin.ui

# FIXME: Link statically
LIBS     += -L/usr/local/lib -lGui -lCore -lImt -lRIO -lNet \
            -lHist -lGraf -lGraf3d -lGpad -lTree -lTreePlayer \
            -lRint -lPostscript -lMatrix -lPhysics \
            -lMathCore -lThread -lMultiProc -pthread -lm -ldl -rdynamic
