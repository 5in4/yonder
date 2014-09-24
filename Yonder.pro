CONFIG += warn_off
QT += sql network widgets webkitwidgets qml quick

cache()

win32 {
    LIBS += C:/lib/qfrontdesk/release/libqfrontdesk.a C:/lib/qjsonbackport/release/libqjson-backport.a C:/lib/fmod/lib/libfmodex.a C:/lib/taglib/libtag.dll.a
    INCLUDEPATH += C:/lib/qfrontdesk/release/include C:/lib/qjsonbackport/release/include C:/lib/fmod/include C:/lib/taglib/include
    DEPENDPATH += C:/lib/qfrontdesk/release/include C:/lib/qjsonbackport/release/include C:/lib/fmod/include C:/lib/taglib/include
}

unix:!macx {
    LIBS += -L/usr/lib/ -ltag -lfmodexL -L$$PWD/lib/qdjango-db/ -lqdjango-db -L$$PWD/lib/qfrontdesk/ -lqfrontdesk
    INCLUDEPATH += /usr/include $$PWD/lib/qdjango-db $$/PWD/lib/qfrontdesk
    DEPENDPATH += /usr/include $$PWD/lib/qdjango-db $$/PWD/lib/qfrontdesk
    #QMAKE_POST_LINK += ../src/linux_postbuild
}

macx {
    LIBS += -L/usr/local/Cellar/taglib/1.9.1/lib -ltag -L$$PWD/third_party/fmod/osx/api/lib -lfmodex
    INCLUDEPATH += /usr/local/Cellar/taglib/1.9.1/include $$PWD/third_party/fmod/osx/api/inc
    DEPENDPATH += /usr/local/Cellar/taglib/1.9.1/include $$PWD/third_party/fmod/osx/api/inc
    #QMAKE_POST_LINK += ../src/osx_postbuild
}

TARGET = yonder
TEMPLATE = app
VERSION = 2.0
SOURCES += main.cpp \
    model/manager/soundmanager.cpp \
    model/manager/musicmanager.cpp \
    model/manager/sfxmanager.cpp \
    delegate/qdoublespinboxdelegate.cpp \
    delegate/agtagbyiddelegate.cpp \
    delegate/agresourcedelegate.cpp \
    delegate/aghotkeyactiondelegate.cpp \
    view/agpreviewpushbutton.cpp \
    view/qlineeditfilter.cpp \
    view/solidtabwidget.cpp \
    frameobject.cpp \
    frameresourcebrowser.cpp \
    frameconfigure.cpp \
    framegenerator.cpp \
    yondergui.cpp \
    yondercore.cpp \
    view/yonderpushbutton.cpp \
    model/track.cpp \
    model/sfxcontainer.cpp \
    model/sfxbit.cpp \
    sfxbasetype.cpp \
    model/manager/mediamanager.cpp \
    model/qdjangotablemodel.cpp

HEADERS += model/manager/soundmanager.h \
    model/manager/musicmanager.h \
    model/manager/sfxmanager.h \
    delegate/qdoublespinboxdelegate.h \
    delegate/agtagbyiddelegate.h \
    delegate/agresourcedelegate.h \
    delegate/aghotkeyactiondelegate.h \
    view/agpreviewpushbutton.h \
    view/qlineeditfilter.h \
    view/solidtabwidget.h \
    frameobject.h \
    frameresourcebrowser.h \
    frameconfigure.h \
    framegenerator.h \
    model/manager/mediamanager.h \
    yondergui.h \
    yondercore.h \
    view/yonderpushbutton.h \
    model/track.h \
    model/sfxcontainer.h \
    model/sfxbit.h \
    sfxbasetype.h \
    model/qdjangotablemodel.h

FORMS += \
    yondergui.ui

RESOURCES += \
    yonder.qrc \

RC_FILE = yonder.rc

OTHER_FILES += \
    BackgroundGenerator.qml


