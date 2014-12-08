CONFIG += warn_off
QT += sql network widgets webkitwidgets

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
    LIBS += -L/usr/local/Cellar/taglib/1.9.1/lib -ltag -L$$PWD/lib/fmod/lib -lfmodex
    INCLUDEPATH += /usr/local/Cellar/taglib/1.9.1/include $$PWD/lib/fmod/inc
    DEPENDPATH += /usr/local/Cellar/taglib/1.9.1/include $$PWD/lib/fmod/inc
    #QMAKE_POST_LINK += ../src/osx_postbuild
}

TARGET = yonder
TEMPLATE = app
VERSION = 2.0
SOURCES += main.cpp \
    delegate/qdoublespinboxdelegate.cpp \
    delegate/agtagbyiddelegate.cpp \
    delegate/agresourcedelegate.cpp \
    delegate/aghotkeyactiondelegate.cpp \
    view/agpreviewpushbutton.cpp \
    view/qlineeditfilter.cpp \
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
    view/libraryview.cpp \
    sfxmanager.cpp \
    musicmanager.cpp \
    trackadd.cpp \
    lib/duktape/src/duktape.c \
    model/sfxbittrack.cpp

HEADERS += \
    model/manager/musicmanager.h \
    model/manager/sfxmanager.h \
    delegate/qdoublespinboxdelegate.h \
    delegate/agtagbyiddelegate.h \
    delegate/agresourcedelegate.h \
    delegate/aghotkeyactiondelegate.h \
    view/agpreviewpushbutton.h \
    view/qlineeditfilter.h \
    frameobject.h \
    frameresourcebrowser.h \
    frameconfigure.h \
    framegenerator.h \
    yondergui.h \
    yondercore.h \
    view/yonderpushbutton.h \
    model/track.h \
    model/sfxcontainer.h \
    model/sfxbit.h \
    model/qdjangotablemodel.h \
    view/libraryview.h \
    sfxmanager.h \
    musicmanager.h \
    model/tracktablemodel.h \
    model/sfxcontainertablemodel.h \
    trackadd.h \
    lib/duktape/src/duktape.h \
    model/sfxbittrack.h \
    model/sfxbittablemodel.h \
    model/sfxbittracktablemodel.h

FORMS += \
    yondergui.ui \
    trackadd.ui

RESOURCES += \
    yonder.qrc \

RC_FILE = yonder.rc

OTHER_FILES +=


