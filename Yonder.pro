CONFIG += warn_off
QT += core gui sql network

win32 {
    LIBS += C:/lib/taglib-1.7/taglib/libtag.dll.a
    INCLUDEPATH += C:/lib/taglib-1.7/include
}

unix:!macx {
    LIBS += -L/usr/lib/ -ltag -lgrantlee_core -lfmodex
    INCLUDEPATH += /usr/include
    DEPENDPATH += /usr/include
}

macx {
    LIBS += -L/usr/local/lib -ltag -grantlee_core -L$$PWD/../fmod/osx/api/lib/ -lfmodex
    INCLUDEPATH += /usr/local/include $$PWD/../fmod/osx/api/inc
    DEPENDPATH += /usr/local/include $$PWD/../fmod/osx/api/inc
}


TARGET = yonder
TEMPLATE = app
SOURCES += main.cpp ambiencegenerator.cpp soundmanager.cpp \
    musicmanager.cpp \
    hotkeysmanager.cpp \
    agmediacontainer.cpp \
    singleshotmanager.cpp \
    atmospheremanager.cpp \
    sfxmanager.cpp \
    delegate/qdoublespinboxdelegate.cpp \
    delegate/agtagbyiddelegate.cpp \
    delegate/agresourcedelegate.cpp \
    delegate/aghotkeyactiondelegate.cpp \
    view/singleshotobjectstracksview.cpp \
    view/singleshotlibraryview.cpp \
    view/sfxobjectstracksview.cpp \
    view/sfxlibraryview.cpp \
    view/musicplaylisttracksview.cpp \
    view/musiclibraryview.cpp \
    view/hotkeyslibraryspecialview.cpp \
    view/hotkeyslibrarysingleshotview.cpp \
    view/hotkeyslibrarysfxview.cpp \
    view/hotkeyslibrarymusicview.cpp \
    view/hotkeyslibraryatmosphereview.cpp \
    view/hotkeysactionsview.cpp \
    view/atmosphereobjectstracksview.cpp \
    view/atmospherelibraryview.cpp \
    view/agqpushbutton.cpp \
    view/agpreviewpushbutton.cpp \
    view/qlineeditfilter.cpp \
    model/objectstracksmodel.cpp \
    model/objectsmodel.cpp \
    model/librarymodel.cpp \
    view/solidtabwidget.cpp \
    frameobject.cpp \
    framestart.cpp \
    frameresourcebrowser.cpp \
    framehelp.cpp \
    frameconfigure.cpp \
    frameeditor.cpp \
    framegenerator.cpp \
    webapp.cpp \
    mediamanager.cpp \
    view/hotkeyspushbutton.cpp
HEADERS += ambiencegenerator.h soundmanager.h \
    musicmanager.h \
    hotkeysmanager.h \
    agmediacontainer.h \
    singleshotmanager.h \
    atmospheremanager.h \
    sfxmanager.h \
    delegate/qdoublespinboxdelegate.h \
    delegate/agtagbyiddelegate.h \
    delegate/agresourcedelegate.h \
    delegate/aghotkeyactiondelegate.h \
    view/singleshotobjectstracksview.h \
    view/singleshotlibraryview.h \
    view/sfxobjectstracksview.h \
    view/sfxlibraryview.h \
    view/musicplaylisttracksview.h \
    view/musiclibraryview.h \
    view/hotkeyslibraryspecialview.h \
    view/hotkeyslibrarysingleshotview.h \
    view/hotkeyslibrarysfxview.h \
    view/hotkeyslibrarymusicview.h \
    view/hotkeyslibraryatmosphereview.h \
    view/hotkeysactionsview.h \
    view/atmosphereobjectstracksview.h \
    view/atmospherelibraryview.h \
    view/agqpushbutton.h \
    view/agpreviewpushbutton.h \
    view/qlineeditfilter.h \
    model/objectstracksmodel.h \
    model/objectsmodel.h \
    model/librarymodel.h \
    view/solidtabwidget.h \
    frameobject.h \
    framestart.h \
    frameresourcebrowser.h \
    framehelp.h \
    frameconfigure.h \
    frameeditor.h \
    framegenerator.h \
    webapp.h \
    mediamanager.h \
    view/hotkeyspushbutton.h
FORMS += ambiencegenerator.ui \
    framestart.ui \
    frameresourcebrowser.ui \
    framehelp.ui \
    frameconfigure.ui \
    frameeditor.ui \
    framegenerator.ui

RESOURCES += \
    yonder.qrc \

RC_FILE = yonder.rc

#libqjson
LIBS += -L$$PWD/../aux/qjson-backport-build/ -lqjson-backport
INCLUDEPATH += $$PWD/../aux/qjson-backport
DEPENDPATH += $$PWD/../aux/qjson-backport-build

#libqfrontdesk
LIBS += -L$$PWD/../aux/qfrontdesk-build/ -lqfrontdesk
INCLUDEPATH += $$PWD/../aux/qfrontdesk
DEPENDPATH += $$PWD/../aux/qfrontdesk-build
