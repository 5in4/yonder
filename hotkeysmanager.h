#ifndef HOTKEYSMANAGER_H
#define HOTKEYSMANAGER_H

#include <QtCore>
#include <QtSql>
#include <QKeySequence>

#include <musicmanager.h>
#include <view/agqpushbutton.h>
#include <model/objectsmodel.h>

class HotkeysManager : public QObject
{
    Q_OBJECT
public:
    explicit HotkeysManager(QSqlDatabase db, MusicManager *music, QList<AGQPushButton*> atmosphere_buttons, QList<AGQPushButton*> sfx_buttons, QList<AGQPushButton*> singleshot_buttons, QComboBox* musicComboBoxSelectPlaylist, QPushButton* musicButtonNext, QPushButton* musicButtonPlayPause, QObject *parent = 0);

    MusicManager *music;

    QList<AGQPushButton*> atmosphere_buttons;
    QList<AGQPushButton*> sfx_buttons;
    QList<AGQPushButton*> singleshot_buttons;
    QList<AGQPushButton*> hotkeys_buttons;

    QComboBox* musicComboBoxSelectPlaylist;
    QPushButton* musicButtonNext;
    QPushButton* musicButtonPlayPause;

    QSqlDatabase db;
    QString hotkeys_identifier;
    QString actions_identifier;
    QString special_identifier;

    QList<QStringList> hotkeys;

    ObjectsModel *hotkeys_model;
    QSqlTableModel *actions_model;
    QSqlTableModel *special_model;

    void getHotkeyButtons(QList<AGQPushButton*> hotkeys_buttons);
    bool updateDatabase(QString identifier, QString sql);
    void createHotkeysList();
    void callHotkey(int oid, bool checked);
    //void createHotkeys();

signals:

public slots:

};

#endif // HOTKEYSMANAGER_H
