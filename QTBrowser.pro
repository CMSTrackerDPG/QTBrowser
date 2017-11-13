QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTBrowser
TEMPLATE = app

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
    plugins/concatinateplugin.cpp \
    tfiletreeviewer/tfiletreeviewer.cpp

HEADERS  += \
    qtbrowser.h \
    qrootcanvas.h \
    tobjectcontainer.h \
    previewqrootcanvas.h \
    settings/settingsdialog.h \
    settings/settingsmanager.h \
    dqmfiledownloader/dqmfiledownloader.h \
    dqmfiledownloader/filecontainer.h \
    dqmfiledownloader/remotefilesmodel.h \
    plugins/iplugin.h \
    plugins/superimposeplugin.h \
    plugins/concatinateplugin.h \
    tfiletreeviewer/tfiletreeviewer.h \
    tfiletreeviewer/leafsortfilterproxymodel.h \

FORMS    += \
    previewqrootcanvas.ui \
    qtbrowser.ui \
    settings/settingsdialog.ui \
    dqmfiledownloader/dqmfiledownloader.ui \
    plugins/superimposeplugin.ui \
    plugins/concatinateplugin.ui \
    tfiletreeviewer/tfiletreeviewer.ui

INCLUDEPATH += /cvmfs/cms.cern.ch/slc6_amd64_gcc630/lcg/root/6.10.08/include/

#when compiling on vocms
LIBS += -L/cvmfs/cms.cern.ch/slc6_amd64_gcc630/lcg/root/6.10.08/lib -lGui -lCore -lImt -lRIO -lNet \
        -lHist -lGraf -lGraf3d -lGpad -lTree -lTreePlayer \
        -lRint -lPostscript -lMatrix -lPhysics \
        -lMathCore -lThread -lMultiProc \
	-lpthread \
	-lm -ldl 

#when developing locally and libraries are installed
#LIBS     += -L/usr/local/lib -lGui -lCore -lImt -lRIO -lNet \
#            -lHist -lGraf -lGraf3d -lGpad -lTree -lTreePlayer \
#            -lRint -lPostscript -lMatrix -lPhysics \
#            -lMathCore -lThread -lMultiProc -pthread -lm -ldl -rdynamic





