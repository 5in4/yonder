#include "hotkeysmanager.h"

HotkeysManager::HotkeysManager(QSqlDatabase db, AtmosphereManager *atmosphere, MusicManager *music, SfxManager *sfx, SingleshotManager *singleshot, QObject *parent) {

}

HotkeysManager::HotkeysManager(QSqlDatabase db, MusicManager *music, QList<AGQPushButton *> atmosphere_buttons, QList<AGQPushButton *> sfx_buttons, QList<AGQPushButton *> singleshot_buttons, QListView *musicComboBoxSelectPlaylist, AGQPushButton *musicButtonNext, AGQPushButton *musicButtonPlayPause, QObject *parent) : QObject(parent)
{
    this->music = music;
    this->atmosphere_buttons = atmosphere_buttons;
    this->sfx_buttons = sfx_buttons;
    this->singleshot_buttons = singleshot_buttons;
    this->musicComboBoxSelectPlaylist = musicComboBoxSelectPlaylist;
    this->musicButtonNext =  musicButtonNext;
    this->musicButtonPlayPause =  musicButtonPlayPause;
    this->db = db;
    hotkeys_identifier = "hotkeys_objects";
    actions_identifier = "hotkeys_objects_actions";
    special_identifier = "special_objects";

    updateDatabase(hotkeys_identifier, QString("CREATE TABLE \"%1\" (\"id\" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , \"name\" VARCHAR NOT NULL, \"shortcut\" VARCHAR UNIQUE )").arg(hotkeys_identifier));
    updateDatabase(special_identifier, QString("CREATE TABLE \"%1\" (\"id\" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , \"name\" VARCHAR NOT NULL )").arg(special_identifier));
    updateDatabase(actions_identifier, QString("CREATE TABLE \"%1\" (\"id\" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , \"hid\" INTEGER NOT NULL , \"section\" VARCHAR NOT NULL , \"aid\" INTEGER NOT NULL )").arg(actions_identifier));

    hotkeys_model = new ObjectsModel(hotkeys_identifier);
    actions_model = new QSqlTableModel();
    actions_model->setTable(actions_identifier);
    actions_model->select();
    special_model = new QSqlTableModel();
    special_model->setTable(special_identifier);
    special_model->select();

    qDebug() << QString("Manager hotkeys summoned");

    createHotkeysList();
    connect(hotkeys_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(createHotkeysList()));

}

bool HotkeysManager::updateDatabase(QString identifier, QString sql) {
    QSqlQuery query;
    QString query_string = QString("SELECT COUNT(name) FROM sqlite_master WHERE type = \"table\" and name = \"%1\"").arg(identifier);
    query.exec(query_string);
    while(query.next()) {
        if(query.value(0) == 0) {
            db.transaction();
            query.exec(sql);

            if(identifier == special_identifier) {
                query.exec("INSERT INTO \"special_objects\" VALUES(1,'Start music')");
                query.exec("INSERT INTO \"special_objects\" VALUES(2,'Stop music')");
                query.exec("INSERT INTO \"special_objects\" VALUES(3,'Toggle music')");
                query.exec("INSERT INTO \"special_objects\" VALUES(4,'Stop atmosphere')");
                query.exec("INSERT INTO \"special_objects\" VALUES(5,'Stop singleshots')");
                query.exec("INSERT INTO \"special_objects\" VALUES(6,'Stop sfx');");
                //qDebug() << query.lastError().text();
            }
            db.commit();
            return true;
        }
    }
    return false;
}


void HotkeysManager::createHotkeysList() {
    hotkeys = QList<QStringList>();
    QSqlQuery query;
    query.prepare(QString("SELECT `id`, `name`, `shortcut` FROM \"%1\"").arg(hotkeys_identifier));
    query.exec();
    while(query.next()) {
        QStringList hotkey;
        hotkey.append(query.value(0).toString()); // DIRRRTY
        hotkey.append(query.value(1).toString());
        hotkey.append(query.value(2).toString());
        hotkeys.append(hotkey);
        //qDebug() << hotkeys_identifier << "hotkey add" << hotkey;
    }
}


void HotkeysManager::getHotkeyButtons(QList<HotkeysPushButton *> hotkeys_buttons) {
    this->hotkeys_buttons = hotkeys_buttons;
}


void HotkeysManager::highlightButtons(int oid, bool highlight) {
    QSqlQuery query;
    QString object = QString("%1").arg(oid);
    query.prepare(QString("SELECT `section`, `aid` FROM \"%1\" WHERE hid=\"%2\"").arg(actions_identifier, object));
    query.exec();
    while(query.next()) {
        if(query.value(0).toString() == "atmosphere_objects") {
            for(int i=0; i<atmosphere_buttons.length(); i++) {
                if(atmosphere_buttons.at(i)->getOID() == query.value(1).toInt()) {
                    if(highlight) {
                        atmosphere_buttons.at(i)->setHighlightedStyle();
                    } else {
                        atmosphere_buttons.at(i)->setStandardStyle();
                    }
                }
            }
        } else if(query.value(0).toString() == "sfx_objects") {
            for(int i=0; i<sfx_buttons.length(); i++) {
                if(sfx_buttons.at(i)->getOID() == query.value(1).toInt()) {
                    if(highlight) {
                        sfx_buttons.at(i)->setHighlightedStyle();
                    } else {
                        sfx_buttons.at(i)->setStandardStyle();
                    }

                }
            }
        } else if(query.value(0).toString() == "singleshot_objects") {
            for(int i=0; i<singleshot_buttons.length(); i++) {
                if(singleshot_buttons.at(i)->getOID() == query.value(1).toInt()) {
                    if(highlight) {
                        singleshot_buttons.at(i)->setHighlightedStyle();
                    } else {
                        singleshot_buttons.at(i)->setStandardStyle();
                    }
                }
            }

        } else if(query.value(0).toString() == "special_objects") {
            int special = query.value(1).toInt();

            if(special == 1) {
                if(highlight) {
                    musicButtonPlayPause->setHighlightedStyle();
                } else {
                    musicButtonPlayPause->setStandardStyle();
                }
            } else if (special == 2) {
//                musicButtonPlayPause->setChecked(false);
            } else if (special == 3) {
//                musicButtonPlayPause->setChecked(checked);
            } else if (special == 4) {
                for(int i=0; i<atmosphere_buttons.length(); i++) {
//                    atmosphere_buttons.at(i)->setChecked(false);
                }
            } else if (special == 5) {
                for(int i=0; i<singleshot_buttons.length(); i++) {
//                    singleshot_buttons.at(i)->setChecked(false);
                }
            } else if (special == 6) {
                for(int i=0; i<sfx_buttons.length(); i++) {
//                    sfx_buttons.at(i)->setChecked(false);
                }
            }
        }
    }
}


void HotkeysManager::callHotkey(int oid, bool checked) {
    bool uncheck = true;
    QSqlQuery query;
    QString object = QString("%1").arg(oid);
    query.prepare(QString("SELECT `section`, `aid` FROM \"%1\" WHERE hid=\"%2\"").arg(actions_identifier, object));
    query.exec();
    while(query.next()) {
        if(query.value(0).toString() == "atmosphere_objects") {
            for(int i=0; i<atmosphere_buttons.length(); i++) {
                if(atmosphere_buttons.at(i)->getOID() == query.value(1).toInt()) {
                    atmosphere_buttons.at(i)->setChecked(checked);
                    uncheck = false;
                }
            }
        } else if(query.value(0).toString() == "sfx_objects") {
            for(int i=0; i<sfx_buttons.length(); i++) {
                if(sfx_buttons.at(i)->getOID() == query.value(1).toInt()) {
                    sfx_buttons.at(i)->setChecked(checked);
                    uncheck = false;
                }
            }
        } else if(query.value(0).toString() == "singleshot_objects") {
            for(int i=0; i<singleshot_buttons.length(); i++) {
                if(singleshot_buttons.at(i)->getOID() == query.value(1).toInt()) {
                    singleshot_buttons.at(i)->setChecked(checked);
                    uncheck = false;
                }
            }
        } else if(query.value(0).toString() == "music_objects" && checked == true) {
            QModelIndex row;
            for(int i=0; i < musicComboBoxSelectPlaylist->model()->rowCount(); i++) {
                if(musicComboBoxSelectPlaylist->model()->index(i, 0).data().toInt() == query.value(1).toInt()) {
                    row = musicComboBoxSelectPlaylist->model()->index(i, 0);
                    break;
                }
            }
//            musicComboBoxSelectPlaylist->selectionModel()->setCurrentIndex(row, QItemSelectionModel::Select);
            musicComboBoxSelectPlaylist->setCurrentIndex(row);
            musicComboBoxSelectPlaylist->update(row);

//            musicComboBoxSelectPlaylist->selectionModel()->select(row, QItemSelectionModel::Select);


        } else if(query.value(0).toString() == "special_objects" && checked == true) {
            //legend:
            //1: start music
            //2: stop music
            //3: toggle music
            //4: stop atmosphere
            //5: stop singleshots
            //6: stop sfx
            int special = query.value(1).toInt();

            if(special == 1 || special == 3) {
                musicButtonPlayPause->setChecked(true);
            } else if (special == 2) {
                musicButtonPlayPause->setChecked(false);
                music->enqueue();
            } else if (special == 4) {
                for(int i=0; i<atmosphere_buttons.length(); i++) {
                    atmosphere_buttons.at(i)->setChecked(false);
                }
            } else if (special == 5) {
                for(int i=0; i<singleshot_buttons.length(); i++) {
                    singleshot_buttons.at(i)->setChecked(false);
                }
            } else if (special == 6) {
                for(int i=0; i<sfx_buttons.length(); i++) {
                    sfx_buttons.at(i)->setChecked(false);
                }

            }
        } else if(query.value(0).toString() == "special_objects" && checked == false) {
            int special = query.value(1).toInt();
            if (special == 3) {
                musicButtonPlayPause->setChecked(false);
            }
        }
    }
    if(uncheck == true) {
        for(int i=0; i<hotkeys.length(); i++) {
            if(hotkeys_buttons.at(i)->getOID() == oid) {
                hotkeys_buttons.at(i)->setChecked(false);
            }
        }
    }
}
