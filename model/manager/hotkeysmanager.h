#ifndef HOTKEYSMANAGER_H
#define HOTKEYSMANAGER_H

#include <QtSql>
#include <QKeySequence>

#include <model/manager/atmospheremanager.h>
#include <model/manager/musicmanager.h>
#include <model/manager/sfxmanager.h>
#include <model/manager/singleshotmanager.h>

#include <view/yonderpushbutton.h>
#include <view/hotkeyspushbutton.h>
#include <model/objectsmodel.h>

class HotkeysManager : public QObject
{
    Q_OBJECT
public:
    explicit HotkeysManager(QSqlDatabase db, MusicManager *music, QList<YonderPushButton *> atmosphere_buttons, QList<YonderPushButton *> sfx_buttons, QList<YonderPushButton *> singleshot_buttons, QListView* musicComboBoxSelectPlaylist, YonderPushButton *musicButtonNext, YonderPushButton *musicButtonPlayPause, QObject *parent = 0);
    explicit HotkeysManager(QSqlDatabase db, AtmosphereManager *atmosphere, MusicManager *music, SfxManager *sfx, SingleshotManager *singleshot, QObject *parent = 0);

    QPointer<AtmosphereManager> atmosphere;
    QPointer<MusicManager> music;
    QPointer<SfxManager> sfx;
    QPointer<SingleshotManager> singleshot;

    QList<YonderPushButton*> atmosphere_buttons;
    QList<YonderPushButton*> sfx_buttons;
    QList<YonderPushButton*> singleshot_buttons;
    QList<HotkeysPushButton*> hotkeys_buttons;

    QListView* musicComboBoxSelectPlaylist;
    YonderPushButton* musicButtonNext;
    YonderPushButton* musicButtonPlayPause;

    QSqlDatabase db;
    QString hotkeys_identifier;
    QString actions_identifier;
    QString special_identifier;

    QList<QStringList> hotkeys;

    ObjectsModel *hotkeys_model;
    QSqlTableModel *actions_model;
    QSqlTableModel *special_model;

    void getHotkeyButtons(QList<HotkeysPushButton*> hotkeys_buttons);
    bool updateDatabase(QString identifier, QString sql);
    void callHotkey(int oid, bool checked);
//    void createHotkeys();

signals:

public slots:
    void highlightButtons(int oid, bool highlight);
    void createHotkeysList();

};

#endif // HOTKEYSMANAGER_H
