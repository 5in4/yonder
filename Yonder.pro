CONFIG += warn_off
QT += core gui sql network
win32 {
    LIBS += C:/lib/qfrontdesk/release/libqfrontdesk.a C:/lib/qjsonbackport/release/libqjson-backport.a C:/lib/fmod/lib/libfmodex.a C:/lib/taglib/libtag.dll.a
    INCLUDEPATH += C:/lib/qfrontdesk/release/include C:/lib/qjsonbackport/release/include C:/lib/fmod/include C:/lib/taglib/include
    DEPENDPATH += C:/lib/qfrontdesk/release/include C:/lib/qjsonbackport/release/include C:/lib/fmod/include C:/lib/taglib/include
#    LIBS += C:/lib/qfrontdesk/release/qfrontdesk.lib C:/lib/qjsonbackport/release/qjson-backport.lib C:/lib/fmod/lib/fmodex_vc.lib C:/lib/taglib/tag.lib
#    INCLUDEPATH += C:/lib/qfrontdesk/release/include C:/lib/qjsonbackport/release/include C:/lib/fmod/include C:/lib/taglib/include
#    DEPENDPATH += C:/lib/qfrontdesk/release/include C:/lib/qjsonbackport/release/include C:/lib/fmod/include C:/lib/taglib/include
}

unix:!macx {
    LIBS += -L/usr/lib/ -ltag -lfmodex
    INCLUDEPATH += /usr/include
    DEPENDPATH += /usr/include
}

macx {
    LIBS += -L/usr/local/lib -ltag -L$$PWD/../fmod/osx/api/lib/ -lfmodex
    INCLUDEPATH += /usr/local/include $$PWD/../fmod/osx/api/inc
    DEPENDPATH += /usr/local/include $$PWD/../fmod/osx/api/inc
}

unix {
#libqjson
LIBS += -L$$PWD/../auxiliary/qjson-backport-build/ -lqjson-backport
INCLUDEPATH += $$PWD/../auxiliary/qjson-backport
DEPENDPATH += $$PWD/../auxiliary/qjson-backport-build

#libqfrontdesk
LIBS += -L$$PWD/../auxiliary/qfrontdesk-build/ -lqfrontdesk
INCLUDEPATH += $$PWD/../auxiliary/QFrontDesk
DEPENDPATH += $$PWD/../auxiliary/qfrontdesk-build
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


