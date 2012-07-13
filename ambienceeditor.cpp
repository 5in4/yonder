#include <ambienceeditor.h>
#include <ui_ambienceeditor.h>

AmbienceEditor::AmbienceEditor(AtmosphereManager *atmosphere, SfxManager *sfx, MusicManager *music, SingleshotManager *singleshot, HotkeysManager *hotkeys, QWidget *parent) : QDialog(parent),
    ui(new Ui::AmbienceEditor)
{
    ui->setupUi(this);

    // Hidden in current version. It's not there yet.
    ui->button_atmosphere_library_refresh->hide();
    ui->button_sfx_library_refresh->hide();
    ui->button_singleshot_library_refresh->hide();
    ui->button_music_library_refresh->hide();

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings settings;

    restoreGeometry(settings.value("EditorWindow/geometry").toByteArray());
    ui->tabs_ambienceeditor->setCurrentIndex(settings.value("EditorWindow/tab_active", 0).toInt());

    // Splitters
    ui->splitter_atmosphere->restoreState(settings.value("EditorWindow/atmosphere_splitter").toByteArray());
    ui->splitter_sfx->restoreState(settings.value("EditorWindow/sfx_splitter").toByteArray());
    ui->splitter_singleshots->restoreState(settings.value("EditorWindow/singleshots_splitter").toByteArray());
    ui->splitter_music->restoreState(settings.value("EditorWindow/music_splitter").toByteArray());
    ui->splitter_hotkeys->restoreState(settings.value("EditorWindow/hotkeys_splitter").toByteArray());
    ui->hotkeys_libraries_toolBox->setCurrentIndex(settings.value("EditorWindow/hotkeys_toolbox", 0).toInt());

    // Atmosphere
    this->atmosphere = atmosphere;
    connect(ui->button_atmosphere_object_add, SIGNAL(clicked()), this, SLOT(atmosphereObjectAdd()));
    connect(ui->button_atmosphere_object_remove, SIGNAL(clicked()), this, SLOT(atmosphereObjectRemove()));

    ui->atmosphere_objects_view->setModel(this->atmosphere->objects_model);
    ui->atmosphere_objects_view->setModelColumn(1);
    connect(ui->atmosphere_objects_view, SIGNAL(clicked(QModelIndex)), this, SLOT(atmosphereUiRefresh()));

    ui->atmosphere_tracks_view->setModel(this->atmosphere->objects_tracks_model);
    ui->atmosphere_tracks_view->custom_model = this->atmosphere->objects_tracks_model;
    ui->atmosphere_tracks_view->hideColumn(0);
    ui->atmosphere_tracks_view->setItemDelegateForColumn(1, new AGTagByIDDelegate(this->atmosphere->identifier, ui->atmosphere_tracks_view));

    ui->atmosphere_tracks_view->setColumnWidth(0, ui->atmosphere_tracks_view->width());

    connect(this->atmosphere->objects_tracks_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), ui->atmosphere_tracks_view, SLOT(resizeColumnsToContents()));

    connect(ui->button_atmosphere_tracks_remove, SIGNAL(clicked()), this, SLOT(atmosphereObjectsTracksRemove()));

    ui->atmosphere_library_view->setModel(this->atmosphere->library_model);
    ui->atmosphere_library_view->setItemDelegateForColumn(0, new AGTagByIDDelegate(this->atmosphere->identifier, ui->atmosphere_library_view));
    ui->atmosphere_library_view->hideColumn(1);
    ui->atmosphere_library_view->hideColumn(2);
    ui->atmosphere_library_view->hideColumn(3);
    ui->atmosphere_library_view->hideColumn(4);
    ui->atmosphere_library_view->hideColumn(5);
    ui->atmosphere_library_view->hideColumn(6);
    /*ui->atmosphere_library_view->setColumnWidth(0, ui->atmosphere_library_view->width());*/
    ui->atmosphere_library_view->resizeColumnsToContents();
    ui->atmosphere_library_view->resizeRowsToContents();
    connect(ui->edit_atmosphere_filter, SIGNAL(textChanged(QString)), this->atmosphere->library_model, SLOT(applyFilter(QString)));

    // Sfx
    this->sfx = sfx;
    connect(ui->button_sfx_object_add, SIGNAL(clicked()), this, SLOT(sfxObjectAdd()));
    connect(ui->button_sfx_object_remove, SIGNAL(clicked()), this, SLOT(sfxObjectRemove()));

    ui->sfx_objects_view->setModel(this->sfx->objects_model);
    ui->sfx_objects_view->setModelColumn(1);
    connect(ui->sfx_objects_view, SIGNAL(clicked(QModelIndex)), this, SLOT(sfxUiRefresh()));

    ui->sfx_tracks_view->setModel(this->sfx->objects_tracks_model);
    ui->sfx_tracks_view->custom_model = this->sfx->objects_tracks_model;

    ui->sfx_tracks_view->hideColumn(0);
    ui->sfx_tracks_view->hideColumn(1);
    ui->sfx_tracks_view->setItemDelegateForColumn(2, new AGTagByIDDelegate(this->sfx->identifier, ui->sfx_tracks_view));
    ui->sfx_tracks_view->setItemDelegateForColumn(3, new QDoubleSpinBoxDelegate(1000, this));
    ui->sfx_tracks_view->setItemDelegateForColumn(4, new QDoubleSpinBoxDelegate(1000, this));
    ui->sfx_tracks_view->setItemDelegateForColumn(5, new QDoubleSpinBoxDelegate(1000, this));
    connect(this->sfx->objects_tracks_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(sfxObjectsTrackCheck(QModelIndex,QModelIndex)));

    ui->sfx_tracks_view->resizeColumnsToContents();
    ui->sfx_tracks_view->resizeRowsToContents();
    connect(ui->button_sfx_tracks_remove, SIGNAL(clicked()), this, SLOT(sfxObjectsTracksRemove()));

    ui->sfx_library_view->setModel(this->sfx->library_model);
    ui->sfx_library_view->setItemDelegateForColumn(0, new AGTagByIDDelegate(this->sfx->identifier, ui->sfx_library_view));
    ui->sfx_library_view->hideColumn(1);
    ui->sfx_library_view->hideColumn(2);
    ui->sfx_library_view->hideColumn(3);
    ui->sfx_library_view->hideColumn(4);
    ui->sfx_library_view->hideColumn(5);
    ui->sfx_library_view->hideColumn(6);
    ui->sfx_library_view->resizeColumnsToContents();
    ui->sfx_library_view->resizeRowsToContents();
    connect(ui->edit_sfx_filter, SIGNAL(textChanged(QString)), this->sfx->library_model, SLOT(applyFilter(QString)));


    // Singleshot
    this->singleshot = singleshot;
    connect(ui->button_singleshot_object_add, SIGNAL(clicked()), this, SLOT(singleshotObjectAdd()));
    connect(ui->button_singleshot_object_remove, SIGNAL(clicked()), this, SLOT(singleshotObjectRemove()));

    ui->singleshot_objects_view->setModel(this->singleshot->objects_model);
    ui->singleshot_objects_view->setModelColumn(1);
    connect(ui->singleshot_objects_view, SIGNAL(clicked(QModelIndex)), this, SLOT(singleshotUiRefresh()));

    ui->singleshot_tracks_view->setModel(this->singleshot->objects_tracks_model);
    ui->singleshot_tracks_view->custom_model = this->singleshot->objects_tracks_model;
    ui->singleshot_tracks_view->hideColumn(0);
    ui->singleshot_tracks_view->setItemDelegateForColumn(1, new AGTagByIDDelegate(this->sfx->identifier, ui->singleshot_tracks_view));
    ui->singleshot_tracks_view->resizeColumnsToContents();
    ui->singleshot_tracks_view->resizeRowsToContents();
    connect(ui->button_singleshot_tracks_remove, SIGNAL(clicked()), this, SLOT(singleshotObjectsTracksRemove()));

    ui->singleshot_library_view->setModel(this->singleshot->library_model);
    ui->singleshot_library_view->setItemDelegateForColumn(0, new AGTagByIDDelegate(this->sfx->identifier, ui->singleshot_library_view));
    ui->singleshot_library_view->hideColumn(1);
    ui->singleshot_library_view->hideColumn(2);
    ui->singleshot_library_view->hideColumn(3);
    ui->singleshot_library_view->hideColumn(4);
    ui->singleshot_library_view->hideColumn(5);
    ui->singleshot_library_view->hideColumn(6);
    ui->singleshot_library_view->resizeColumnsToContents();
    ui->singleshot_library_view->resizeRowsToContents();
    connect(ui->edit_singleshot_filter, SIGNAL(textChanged(QString)), this->singleshot->library_model, SLOT(applyFilter(QString)));


    // Music
    this->music = music;
    connect(ui->button_music_playlist_add, SIGNAL(clicked()), this, SLOT(musicPlaylistAdd()));
    connect(ui->button_music_playlist_remove, SIGNAL(clicked()), this, SLOT(musicPlaylistRemove()));

    ui->music_playlists_view->setModel(this->music->objects_model);
    ui->music_playlists_view->setModelColumn(1);
    connect(ui->music_playlists_view, SIGNAL(clicked(QModelIndex)), this, SLOT(musicUiRefresh()));


    ui->music_tracks_view->setModel(this->music->objects_tracks_model);
    ui->music_tracks_view->custom_model = this->music->objects_tracks_model;
    ui->music_tracks_view->hideColumn(0);
    ui->music_tracks_view->setItemDelegateForColumn(1, new AGTagByIDDelegate(this->music->identifier, ui->music_tracks_view));
    ui->music_tracks_view->resizeColumnsToContents();
    ui->music_tracks_view->resizeRowsToContents();

    connect(ui->button_music_tracks_remove, SIGNAL(clicked()), this, SLOT(musicPlaylistsTracksRemove()));

    ui->music_library_view->setModel(this->music->library_model);
    ui->music_library_view->setItemDelegateForColumn(0, new AGTagByIDDelegate(this->music->identifier, ui->music_library_view));
    ui->music_library_view->hideColumn(1);
    ui->music_library_view->hideColumn(2);
    ui->music_library_view->hideColumn(3);
    ui->music_library_view->hideColumn(4);
    ui->music_library_view->hideColumn(5);
    ui->music_library_view->hideColumn(6);
    ui->music_library_view->resizeRowsToContents();
    connect(ui->edit_music_filter, SIGNAL(textChanged(QString)), this->music->library_model, SLOT(applyFilter(QString)));

    // Hotkeys
    this->hotkeys = hotkeys;

    connect(ui->button_hotkeys_hotkey_add, SIGNAL(clicked()), this, SLOT(hotkeysHotkeyAdd()));
    connect(ui->button_hotkeys_hotkey_remove, SIGNAL(clicked()), this, SLOT(hotkeysHotkeyRemove()));
    ui->hotkeys_hotkey_view->setModel(this->hotkeys->hotkeys_model);
    ui->hotkeys_hotkey_view->hideColumn(0);
    connect(ui->hotkeys_hotkey_view, SIGNAL(clicked(QModelIndex)), this, SLOT(hotkeysUiRefresh()));
    ui->hotkeys_hotkey_view->selectRow(0);
    hotkeysUiRefresh();

    ui->hotkeys_actions_view->setModel(this->hotkeys->actions_model);
    ui->hotkeys_actions_view->setItemDelegateForColumn(0, new AGHotkeyActionDelegate(this));
    ui->hotkeys_actions_view->hideColumn(1);
    ui->hotkeys_actions_view->hideColumn(2);
    ui->hotkeys_actions_view->hideColumn(3);
    ui->hotkeys_actions_view->resizeColumnsToContents();

    connect(ui->button_hotkeys_actions_remove, SIGNAL(clicked()), this, SLOT(hotkeysHotkeyActionsRemove()));


    ui->hotkeys_library_atmosphere_view->setModel(this->atmosphere->objects_model);
    ui->hotkeys_library_atmosphere_view->hideColumn(0);
    ui->hotkeys_library_sfx_view->setModel(this->sfx->objects_model);
    ui->hotkeys_library_sfx_view->hideColumn(0);
    ui->hotkeys_library_music_view->setModel(this->music->objects_model);
    ui->hotkeys_library_music_view->hideColumn(0);
    ui->hotkeys_library_singleshot_view->setModel(this->singleshot->objects_model);
    ui->hotkeys_library_singleshot_view->hideColumn(0);
    ui->hotkeys_library_special_view->setModel(this->hotkeys->special_model);
    ui->hotkeys_library_special_view->hideColumn(0);

    // Preview
    inst = libvlc_new(0, NULL);
    media_preview = new AGMediaContainer(inst, this);
    connect(ui->preview_play_pause, SIGNAL(toggled(bool)), this, SLOT(previewPlayPause(bool)));
    connect(ui->preview_play_pause, SIGNAL(fileDropped(QString,int)), this, SLOT(previewEnqueue(QString,int)));
    connect(media_preview, SIGNAL(finished(int)), this, SLOT(previewStop(int)));
    connect(media_preview, SIGNAL(trackPosition(int,int)), this, SLOT(previewSetSeek(int,int)));
}


void AmbienceEditor::accept() {
    QSettings settings;
    settings.setValue("EditorWindow/geometry", this->saveGeometry());
    settings.setValue("EditorWindow/atmosphere_splitter", ui->splitter_atmosphere->saveState());
    settings.setValue("EditorWindow/sfx_splitter", ui->splitter_sfx->saveState());
    settings.setValue("EditorWindow/singleshots_splitter", ui->splitter_singleshots->saveState());
    settings.setValue("EditorWindow/music_splitter", ui->splitter_music->saveState());
    settings.setValue("EditorWindow/hotkeys_splitter", ui->splitter_hotkeys->saveState());
    settings.setValue("EditorWindow/tab_active", ui->tabs_ambienceeditor->currentIndex());
    settings.setValue("EditorWindow/hotkeys_toolbox", ui->hotkeys_libraries_toolBox->currentIndex());
}



void AmbienceEditor::atmosphereObjectAdd() {
    bool ok;
    QString object_name = QInputDialog::getText(this, tr("New Atmosphere Set"), tr("Atmosphere Set name"), QLineEdit::Normal, QString(), &ok);
    if(ok && !object_name.isEmpty()) {
        this->atmosphere->objects_model->addObject(object_name);
    }
}


void AmbienceEditor::atmosphereObjectRemove() {
    if(this->ui->atmosphere_objects_view->currentIndex().row() != -1) {
        if(QMessageBox::question(this, tr("Delete Atmosphere Set"), tr("Delete Atmosphere Set?"), QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes) {
            QSqlQuery remove_object_tracks(QString("DELETE FROM %1 WHERE oid = :oid").arg(atmosphere->objects_tracks_identifier));
            remove_object_tracks.bindValue(":oid", atmosphere->objects_model->index(ui->atmosphere_objects_view->currentIndex().row(), 0).data());
            qDebug() << "atmosphere playlist_tracks removed:" << remove_object_tracks.exec();

            QSqlQuery remove_object_hotkeys(QString("DELETE FROM %1 WHERE section = :section AND aid = :aid").arg(hotkeys->actions_identifier));
            remove_object_hotkeys.bindValue(":section", "atmosphere_objects");
            remove_object_hotkeys.bindValue(":aid", atmosphere->objects_model->index(ui->atmosphere_objects_view->currentIndex().row(), 0).data());
            qDebug() << "atmosphere hotkeys removed:" << remove_object_hotkeys.exec();

            this->atmosphere->objects_model->removeRow(this->ui->atmosphere_objects_view->currentIndex().row());

            atmosphere->objects_tracks_model->select();
        }
    }
}


void AmbienceEditor::atmosphereUiRefresh() {
    int oid = this->atmosphere->objects_model->record(this->ui->atmosphere_objects_view->currentIndex().row()).value(0).toInt();
    this->atmosphere->objects_tracks_model->selectObject(oid);
    this->ui->atmosphere_library_view->resizeColumnsToContents();
    this->ui->atmosphere_library_view->resizeRowsToContents();
}


void AmbienceEditor::atmosphereObjectsTracksRemove() {
    QModelIndexList selected_indices = this->ui->atmosphere_tracks_view->getSelectedIndexes();
    atmosphere->objects_tracks_model->database().transaction();
    for(int i=0; i<selected_indices.length(); ++i) {
        atmosphere->objects_tracks_model->removeTrackFromObject(selected_indices.at(i));
    }
    atmosphere->objects_tracks_model->database().commit();
    atmosphere->objects_tracks_model->select();
}


void AmbienceEditor::singleshotObjectAdd() {
    bool ok;
    QString object_name = QInputDialog::getText(this, tr("New singleshot Set"), tr("Singleshot Set name"), QLineEdit::Normal, QString(), &ok);
    if(ok && !object_name.isEmpty()) {
        this->singleshot->objects_model->addObject(object_name);
    }
}


void AmbienceEditor::singleshotObjectRemove() {
    if(this->ui->singleshot_objects_view->currentIndex().row() != -1) {
        if(QMessageBox::question(this, tr("Delete singleshot Set"), tr("Delete singleshot Set?"), QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes) {
            QSqlQuery remove_object_tracks(QString("DELETE FROM %1 WHERE oid = :oid").arg(singleshot->objects_tracks_identifier));
            remove_object_tracks.bindValue(":oid", singleshot->objects_model->index(ui->singleshot_objects_view->currentIndex().row(), 0).data());
            qDebug() << "singleshot object_tracks removed:" << remove_object_tracks.exec();

            QSqlQuery remove_object_hotkeys(QString("DELETE FROM %1 WHERE section = :section AND aid = :aid").arg(hotkeys->actions_identifier));
            remove_object_hotkeys.bindValue(":section", "singleshot_objects");
            remove_object_hotkeys.bindValue(":aid", singleshot->objects_model->index(ui->singleshot_objects_view->currentIndex().row(), 0).data());
            qDebug() << "singleshot hotkeys removed:" << remove_object_hotkeys.exec();

            this->singleshot->objects_model->removeRow(this->ui->singleshot_objects_view->currentIndex().row());

            singleshot->objects_tracks_model->select();
        }
    }
}


void AmbienceEditor::singleshotUiRefresh() {
    int oid = this->singleshot->objects_model->record(this->ui->singleshot_objects_view->currentIndex().row()).value(0).toInt();
    this->singleshot->objects_tracks_model->selectObject(oid);
    this->ui->singleshot_library_view->resizeColumnsToContents();
    this->ui->singleshot_library_view->resizeRowsToContents();
}


void AmbienceEditor::singleshotObjectsTracksRemove() {
    QModelIndexList selected_indices = this->ui->singleshot_tracks_view->getSelectedIndexes();
    singleshot->objects_tracks_model->database().transaction();
    for(int i=0; i<selected_indices.length(); ++i) {
        singleshot->objects_tracks_model->removeTrackFromObject(selected_indices.at(i));
    }
    singleshot->objects_tracks_model->database().commit();
    singleshot->objects_tracks_model->select();
}


void AmbienceEditor::sfxObjectAdd() {
    bool ok;
    QString object_name = QInputDialog::getText(this, tr("New Sfx Set"), tr("Sfx Set name"), QLineEdit::Normal, QString(), &ok);
    if(ok && !object_name.isEmpty()) {
        this->sfx->objects_model->addObject(object_name);
    }
}


void AmbienceEditor::sfxObjectRemove() {
    if(this->ui->sfx_objects_view->currentIndex().row() != -1) {
        if(QMessageBox::question(this, tr("Delete Sfx Set"), tr("Delete Sfx Set?"), QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes) {
            QSqlQuery remove_object_tracks(QString("DELETE FROM %1 WHERE oid = :oid").arg(sfx->objects_tracks_identifier));
            remove_object_tracks.bindValue(":oid", sfx->objects_model->index(ui->sfx_objects_view->currentIndex().row(), 0).data());
            qDebug() << "sfx playlist_tracks removed:" << remove_object_tracks.exec();

            QSqlQuery remove_object_hotkeys(QString("DELETE FROM %1 WHERE section = :section AND aid = :aid").arg(hotkeys->actions_identifier));
            remove_object_hotkeys.bindValue(":section", "sfx_objects");
            remove_object_hotkeys.bindValue(":aid", sfx->objects_model->index(ui->sfx_objects_view->currentIndex().row(), 0).data());
            qDebug() << "sfx hotkeys removed:" << remove_object_hotkeys.exec();

            this->sfx->objects_model->removeRow(this->ui->sfx_objects_view->currentIndex().row());

            sfx->objects_tracks_model->select();
        }
    }

}


void AmbienceEditor::sfxObjectsTracksRemove() {
    QModelIndexList selected_indices = this->ui->sfx_tracks_view->getSelectedIndexes();
    sfx->objects_tracks_model->database().transaction();
    for(int i=0; i<selected_indices.length(); ++i) {
        sfx->objects_tracks_model->removeTrackFromObject(selected_indices.at(i), true);
    }
    sfx->objects_tracks_model->database().commit();
    sfx->objects_tracks_model->select();
}


void AmbienceEditor::sfxObjectsTrackCheck(QModelIndex i1, QModelIndex i2) {
    int track_offset = sfx->objects_tracks_model->index(i1.row(), 3).data().toInt();
    int track_lower_boundary = sfx->objects_tracks_model->index(i1.row(), 4).data().toInt();
    if(i1.column() == 4 && i1.data().toInt() > track_offset) { // lower boundary offset
        sfx->objects_tracks_model->setData(i1, track_offset);
        sfx->objects_tracks_model->submit();
    }
    if(i1.column() == 3 && i1.data().toInt() < track_lower_boundary) {
        sfx->objects_tracks_model->setData(i1, track_lower_boundary);
        sfx->objects_tracks_model->submit();
    }
}


void AmbienceEditor::sfxUiRefresh() {
    int oid = this->sfx->objects_model->record(this->ui->sfx_objects_view->currentIndex().row()).value(0).toInt();
    this->sfx->objects_tracks_model->selectObject(oid);
    this->ui->sfx_library_view->resizeColumnsToContents();
    this->ui->sfx_library_view->resizeRowsToContents();

}


void AmbienceEditor::musicPlaylistAdd() {
    bool ok;
    QString playlist_name = QInputDialog::getText(this, tr("New playlist"), tr("Playlist name"), QLineEdit::Normal, QString(), &ok);
    if(ok && !playlist_name.isEmpty()) {
        this->music->objects_model->addObject(playlist_name);
    }
}


void AmbienceEditor::musicPlaylistRemove() {
    if(this->ui->music_playlists_view->currentIndex().row() != -1) {
        if(QMessageBox::question(this, tr("Delete playlist"), tr("Delete playlist?"), QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes) {
            QSqlQuery remove_object_tracks(QString("DELETE FROM %1 WHERE oid = :oid").arg(music->objects_tracks_identifier));
            remove_object_tracks.bindValue(":oid", music->objects_model->index(ui->music_playlists_view->currentIndex().row(), 0).data());
            qDebug() << "music playlist_tracks removed:" << remove_object_tracks.exec();

            QSqlQuery remove_object_hotkeys(QString("DELETE FROM %1 WHERE section = :section AND aid = :aid").arg(hotkeys->actions_identifier));
            remove_object_hotkeys.bindValue(":section", "music_objects");
            remove_object_hotkeys.bindValue(":aid", music->objects_model->index(ui->music_playlists_view->currentIndex().row(), 0).data());
            qDebug() << "music hotkeys removed:" << remove_object_hotkeys.exec();

            this->music->objects_model->removeRow(this->ui->music_playlists_view->currentIndex().row());

            music->objects_tracks_model->select();
        }
    }
}


void AmbienceEditor::musicPlaylistsTracksRemove() {
   QModelIndexList selected_indices = this->ui->music_tracks_view->getSelectedIndexes();
   music->objects_tracks_model->database().transaction();
   for(int i=0; i<selected_indices.length(); ++i) {
       music->objects_tracks_model->removeTrackFromObject(selected_indices.at(i));
   }
   music->objects_tracks_model->database().commit();
   music->objects_tracks_model->select();
}


void AmbienceEditor::musicUiRefresh() {
    int oid = this->music->objects_model->record(this->ui->music_playlists_view->currentIndex().row()).value(0).toInt();
    this->music->objects_tracks_model->selectObject(oid);
    this->ui->music_library_view->resizeColumnsToContents();
    this->ui->music_library_view->resizeRowsToContents();
}


void AmbienceEditor::hotkeysHotkeyAdd() {
    bool ok;
    QString object_name = QInputDialog::getText(this, tr("New hotkey"), tr("Set hotkey name"), QLineEdit::Normal, QString(), &ok);
    if(ok && !object_name.isEmpty()) {
        hotkeys->hotkeys_model->addObject(object_name);
    }
}


void AmbienceEditor::hotkeysHotkeyRemove() {
    if(this->ui->hotkeys_hotkey_view->currentIndex().row() != -1) {
        if(QMessageBox::question(this, tr("Delete hotkey"), tr("Delete hotkey?"), QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes) {
            QSqlQuery remove_object_tracks(QString("DELETE FROM %1 WHERE hid = :hid").arg(hotkeys->actions_identifier));
            remove_object_tracks.bindValue(":hid", hotkeys->hotkeys_model->index(ui->hotkeys_hotkey_view->currentIndex().row(), 0).data());
            qDebug() << "hotkey actions removed:" << remove_object_tracks.exec();

            this->hotkeys->hotkeys_model->removeRow(this->ui->hotkeys_hotkey_view->currentIndex().row());

            hotkeys->actions_model->select();
        }
    }
}


void AmbienceEditor::hotkeysUiRefresh() {
    int oid = this->hotkeys->hotkeys_model->record(this->ui->hotkeys_hotkey_view->currentIndex().row()).value(0).toInt();
    hotkeys->actions_model->setFilter(QString("hid = \"%1\"").arg(oid));
    hotkeys->actions_model->select();
    ui->hotkeys_actions_view->setOID(oid);
            //objects_tracks_model->selectObject(oid);
}


void AmbienceEditor::hotkeysHotkeyActionsRemove() {
    QModelIndexList selected_indices = this->ui->hotkeys_actions_view->getSelectedIndexes();
    hotkeys->actions_model->database().transaction();
    for(int i=0; i<selected_indices.length(); ++i) {
        QSqlQuery query = QSqlQuery(hotkeys->actions_model->database());
        query.prepare(QString("DELETE FROM %1 WHERE id = :id").arg(hotkeys->actions_identifier));
        query.bindValue(":id", selected_indices.at(i).data());
        query.exec();
    }
    hotkeys->actions_model->database().commit();
    hotkeys->actions_model->select();
}


/*
 * Look which library item is active, then play
 */
void AmbienceEditor::previewPlayPause(bool state) {
    if(media_preview->getCurrentFilename().isEmpty()) {
        ui->preview_play_pause->setChecked(false);
    }
    if(state == true && !media_preview->getCurrentFilename().isEmpty()) {
        media_preview->play();
        return;
    }
    media_preview->pause();
}

void AmbienceEditor::previewEnqueue(QString mime, int tid) {
    QSqlQuery query_preview;
    QString library;
    QString path;

    if(mime == "application/sg-music-library-reference") {
        library = music->library_identifier;
        path = music->path;
    } else if(mime == "application/sg-sfx-library-reference-drag" || mime == "application/sg-sfx-library-reference") {
        library = sfx->library_identifier;
        path = sfx->path;
    } else if(mime == "application/sg-atmosphere-library-reference") {
        library = atmosphere->library_identifier;
        path = atmosphere->path;
    } else if(mime == "application/sg-singleshot-library-reference") {
        library = singleshot->library_identifier;
        path = singleshot->path;
    }

    query_preview.prepare(QString("SELECT `path` FROM `%1` WHERE `id` = :tid").arg(library));
    query_preview.bindValue(":tid", tid);
    query_preview.exec();

    query_preview.first();
    QString preview_file = QString("%1%2").arg(path, query_preview.value(0).toString());
    qDebug() << preview_file;
//    media_preview->stop();
    media_preview->enqueue(preview_file);
    media_preview->setVolume(100);
    ui->preview_play_pause->setChecked(true);
}

void AmbienceEditor::previewStop(int channel) {
    ui->preview_play_pause->setChecked(false);
    ui->preview_seek->setValue(0);
}


void AmbienceEditor::previewSetSeek(int time, int length) {
    ui->preview_seek->setMaximum(length);
    ui->preview_seek->setValue(time);
}

AmbienceEditor::~AmbienceEditor()
{
    delete ui;
}
