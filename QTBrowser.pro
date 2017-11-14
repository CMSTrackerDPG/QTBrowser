QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTBrowser
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += console

SOURCES += \
    src/dqmfiledownloader.cpp \
    src/main.cpp              \
    src/qrootcanvas.cpp       \
    src/qtbrowser.cpp         \
    src/tfiletreeviewer.cpp   \
    src/models/remotefilesmodel.cpp   \
    src/plugins/concatinateplugin.cpp \
    src/plugins/superimposeplugin.cpp \
    src/settings/settingsdialog.cpp   \
    src/settings/settingsmanager.cpp  \

HEADERS  += \
    include/dqmfiledownloader.h \
    include/previewqrootcanvas.h\
    include/qrootcanvas.h       \
    include/qtbrowser.h         \
    include/tfiletreeviewer.h   \
    include/container/filecontainer.h    \
    include/container/tobjectcontainer.h \
    include/models/leafsortfilterproxymodel.h \
    include/models/remotefilesmodel.h         \
    include/plugins/concatinateplugin.h  \
    include/plugins/iplugin.h            \
    include/plugins/superimposeplugin.h  \
    include/settings/settingsdialog.h  \
    include/settings/settingsmanager.h \


FORMS    += \
    form/concatinateplugin.ui  \
    form/dqmfiledownloader.ui  \
    form/previewqrootcanvas.ui \
    form/qtbrowser.ui          \
    form/settingsdialog.ui     \
    form/superimposeplugin.ui  \
    form/tfiletreeviewer.ui    \

INCLUDEPATH += $$PWD/include

#  ------------------------- VOCMS -------------------------
#
#INCLUDEPATH += /cvmfs/cms.cern.ch/slc6_amd64_gcc630/lcg/root/6.10.08/include/
#LIBS += -L/cvmfs/cms.cern.ch/slc6_amd64_gcc630/lcg/root/6.10.08/lib -lGui -lCore -lImt -lRIO -lNet \
#        -lHist -lGraf -lGraf3d -lGpad -lTree -lTreePlayer \
#        -lRint -lPostscript -lMatrix -lPhysics \
#        -lMathCore -lThread -lMultiProc \
#        -lpthread \
#	 -lm -ldl
#
#
#  ------------------------- LOCALLY -------------------------
#when developing locally and libraries are installed
LIBS     += -L/usr/local/lib -lGui -lCore -lImt -lRIO -lNet \
            -lHist -lGraf -lGraf3d -lGpad -lTree -lTreePlayer \
            -lRint -lPostscript -lMatrix -lPhysics \
            -lMathCore -lThread -lMultiProc -pthread -lm -ldl




