#include <ambiencegenerator.h>
#include <ui_ambiencegenerator.h>

extern QString NAME;
extern QString VERSION;
extern QString ACTION_KEY;
extern QString AUTHOR;
extern QString WEBADDRESS;
extern QString ACCEPTED_MIMETYPES;

AmbienceGenerator::AmbienceGenerator(QSplashScreen *splash_screen, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AmbienceGenerator)
{

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings settings;
    config_path = QDir(QFileInfo(settings.fileName()).absoluteDir()).absolutePath();

    this->splash_screen = splash_screen;

    ui->setupUi(this);
    progress_bar = new QProgressBar();
    ui->statusbar->addWidget(progress_bar);
    progress_bar->hide();


    this->setWindowTitle(QString("%1 %2").arg(QApplication::instance()->applicationName(), VERSION));

    ui->actionCheckForUpdates->setChecked(settings.value("Settings/check_update", 0).toBool());

    this->restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    ui->mainSplitter->restoreState(settings.value("MainWindow/splitter").toByteArray());

    ui->atmosphereVolume->setValue(settings.value("Settings/atmosphere_volume", 50).toInt());
    ui->musicVolume->setValue(settings.value("Settings/music_volume", 50).toInt());
    ui->sfxVolume->setValue(settings.value("Settings/sfx_volume", 50).toInt());
    ui->singleshotVolume->setValue(settings.value("Settings/singleshot_volume", 50).toInt());

    // Load project
    project_path = settings.value("Settings/last_project_path").toString();
    setProject(project_path);

    // s&s
    connect(ui->actionSetProject, SIGNAL(triggered()), this, SLOT(setProjectSlot()));
    connect(ui->actionAmbienceEditor, SIGNAL(triggered()), this, SLOT(ambienceEditor()));
    connect(ui->actionResourceBrowser, SIGNAL(triggered()), this, SLOT(resourceBrowser()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionOnlineHelp, SIGNAL(triggered()), this, SLOT(openOnlineHelp()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));

    if(ui->actionCheckForUpdates->isChecked()) {
        checkUpdate();
    }


}

bool AmbienceGenerator::prepareExit() {
    QSettings settings;
    settings.setValue("MainWindow/geometry", this->saveGeometry());
    settings.setValue("MainWindow/splitter", ui->mainSplitter->saveState());
    settings.setValue("Settings/singleshot_volume", ui->singleshotVolume->value());
    settings.setValue("Settings/sfx_volume", ui->sfxVolume->value());
    settings.setValue("Settings/music_volume", ui->musicVolume->value());
    settings.setValue("Settings/atmosphere_volume", ui->atmosphereVolume->value());
    settings.setValue("Settings/check_update", ui->actionCheckForUpdates->isChecked());

    settings.sync();
    return true;
}

void AmbienceGenerator::closeEvent(QCloseEvent *event) {
    if(prepareExit()) {
        QMainWindow::closeEvent(event);
    }
}


/*!
 * Pauses all managers and launches AmbienceEditor
 */
void AmbienceGenerator::ambienceEditor() {

    // pause managers
    for(int i=0; i < hotkeys_buttons.length(); i++) {
        hotkeys_buttons.at(i)->setChecked(false);
    }

    for(int i=0; i < atmosphere_buttons.length(); i++) {
        atmosphere_buttons.at(i)->setChecked(false);
    }

    for(int i=0; i < sfx_buttons.length(); i++) {
        sfx_buttons.at(i)->setChecked(false);
    }

    for(int i=0; i < singleshot_buttons.length(); i++) {
        singleshot_buttons.at(i)->setChecked(false);
    }

    ui->musicButtonPlayPause->setChecked(false);
    // pause managers end


    db.commit();
    //music->objects_model->select();

    if(!atmosphere) {
        delete atmosphere;
    }
    atmosphere = new AtmosphereManager(project_path, db, progress_bar, this);

    if(!music) {
        delete music;
    }
    music = new MusicManager(project_path, db, progress_bar, this);

    if(!sfx) {
        delete sfx;
    }
    sfx = new SfxManager(project_path, db, progress_bar, this);

    if(!singleshot) {
        delete singleshot;
    }
    singleshot = new SingleshotManager(project_path, db, progress_bar, this);

    AmbienceEditor ambience_editor(atmosphere, sfx, music, singleshot, hotkeys, this);
    if(ambience_editor.exec()) {

    }
    this->setProject(project_path);
}

void AmbienceGenerator::resourceBrowser() {

    ResourceBrowser resource_browser(config_path, this);
    if(resource_browser.exec()) {

    }
}

void AmbienceGenerator::setProjectSlot() {
    this->setProject(project_path, true);
}


/*!
 * Resets all managers and loads new project
 */
void AmbienceGenerator::setProject(QString project_path, bool force_dialog) {
    if(!QDir().exists(project_path) || force_dialog == true) {
        ProjectSelector project_selector(project_path, this);
        project_selector.exec();
        project_path = project_selector.getProjectPath();
    }
    this->project_path = project_path;

    QSettings settings;
    settings.setValue("Settings/last_project_path", project_path);

    QString db_filename = QString("%1/project.db").arg(project_path);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(db_filename);
    db.open();

    qDebug() << "Setting project path to" << this->project_path;

    if(!atmosphere) {
        delete atmosphere;
    }
    splash_screen->showMessage(tr("Loading atmosphere"), 0x0041, Qt::white);
    atmosphere = new AtmosphereManager(project_path, db, progress_bar, this);

    if(!music) {
        delete music;
    }
    splash_screen->showMessage(tr("Loading music"), 0x0041, Qt::white);
    music = new MusicManager(project_path, db, progress_bar, this);

    if(!sfx) {
        delete sfx;
    }
    splash_screen->showMessage(tr("Loading sfx"), 0x0041, Qt::white);
    sfx = new SfxManager(project_path, db, progress_bar, this);

    if(!singleshot) {
        delete singleshot;
    }
    splash_screen->showMessage(tr("Loading singleshots"), 0x0041, Qt::white);
    singleshot = new SingleshotManager(project_path, db, progress_bar, this);

    // Atmosphere
    atmosphereCreateButtons();

    // Music
    ui->musicComboBoxSelectPlaylist->setModel(music->objects_model);
    ui->musicComboBoxSelectPlaylist->setModelColumn(1);
    musicSetPlaylist(0);

    // Sfx
    sfxCreateButtons();

    // Singleshot
    singleshotCreateButtons();

    // s&s
    ui->atmosphereVolume->disconnect();
    connect(ui->atmosphereVolume, SIGNAL(valueChanged(int)), this, SLOT(atmosphereSetVolume()));

    ui->musicComboBoxSelectPlaylist->disconnect();
    connect(ui->musicComboBoxSelectPlaylist, SIGNAL(currentIndexChanged(int)), this, SLOT(musicSetPlaylist(int)));
    ui->musicButtonNext->disconnect();
    connect(ui->musicButtonNext, SIGNAL(clicked()), this, SLOT(musicNext()));
    ui->musicButtonPlayPause->disconnect();
    connect(ui->musicButtonPlayPause, SIGNAL(toggled(bool)), this, SLOT(musicPlayPause()));
    ui->musicVolume->disconnect();
    connect(ui->musicVolume, SIGNAL(valueChanged(int)), this, SLOT(musicSetVolume()));

    connect(music->media_container.at(0), SIGNAL(starting(int)), this, SLOT(musicTrackChanged()));

    connect(music->media_container.at(0), SIGNAL(trackPosition(int, int)), this, SLOT(musicSetSeek(int,int)));

    ui->sfxVolume->disconnect();
    connect(ui->sfxVolume, SIGNAL(valueChanged(int)), this, SLOT(sfxSetVolume()));

    ui->singleshotVolume->disconnect();
    connect(ui->singleshotVolume, SIGNAL(valueChanged(int)), this, SLOT(singleshotSetVolume()));

    // Hotkeys
    if(!hotkeys) {
        delete hotkeys;
    }
    hotkeys = new HotkeysManager(db, music, atmosphere_buttons, sfx_buttons, singleshot_buttons, ui->musicComboBoxSelectPlaylist, ui->musicButtonNext, ui->musicButtonPlayPause, this);
    hotkeysCreateButtons();
    hotkeys->getHotkeyButtons(hotkeys_buttons);

    singleshotSetVolume();
    atmosphereSetVolume();
    musicSetVolume();
    sfxSetVolume();

}


void AmbienceGenerator::hotkeysCreateButtons() {
    while(ui->hotkeysButtonLayout->count() > 0) {
        ui->hotkeysButtonLayout->removeItem(ui->hotkeysButtonLayout->takeAt(0));
    }
    this->repaint();

    while(hotkeys_buttons.length() > 0) {
        delete hotkeys_buttons.last();
        hotkeys_buttons.removeLast();
    }

    int row = 0;
    int col = 0;
    int max = qSqrt(hotkeys->hotkeys.length());

    for(int i=0; i < hotkeys->hotkeys.length(); i++) {
        hotkeys_buttons.append(new AGQPushButton(i, this));
        hotkeys_buttons.at(i)->setOID(hotkeys->hotkeys.at(i)[0].toInt());
        hotkeys_buttons.at(i)->setCheckable(true);
        hotkeys_buttons.at(i)->setObjectName(hotkeys->hotkeys.at(i)[1]);
        hotkeys_buttons.at(i)->setStyleSheet("text-align: left;");
        //hotkeys_buttons.at(i)->setPalette(QPalette(QColor(250, 250, 100)));
        //hotkeys_buttons.at(i)->setIcon(QIcon::fromTheme("media-playback-start"));

        QString hk_btn_view;
        if(hotkeys->hotkeys.at(i)[2] == "") {
            hk_btn_view = "%1";
                    hotkeys_buttons.at(i)->setHtml(QString(hk_btn_view).arg(hotkeys->hotkeys.at(i)[1]));
        } else {
            hk_btn_view = "%1 <b>[%2%3]</b>";
            hotkeys_buttons.at(i)->setHtml(QString(hk_btn_view).arg(hotkeys->hotkeys.at(i)[1], ACTION_KEY, hotkeys->hotkeys.at(i)[2]));
        }

        ui->hotkeysButtonLayout->addWidget(hotkeys_buttons.at(i));
        //ui->hotkeysButtonLayout->addWidget(hotkeys_buttons.at(i), row, col);
        if(col < max) {
            col++;
        } else {
            col = 0;
            row++;
        }

        hotkeys_buttons.at(i)->setShortcut(QKeySequence(QString("%1%2").arg(ACTION_KEY, hotkeys->hotkeys.at(i)[2])));
        connect(hotkeys_buttons.at(i), SIGNAL(toggled(int,bool)), this, SLOT(hotkeysControl(int,bool)));
    }
}


void AmbienceGenerator::hotkeysControl(int pos_in_array, bool checked) {
    hotkeys->callHotkey(hotkeys_buttons.at(pos_in_array)->getOID(), checked);
}


void AmbienceGenerator::atmosphereCreateButtons() {
    while(ui->atmosphereButtonLayout->count() > 0) {
        ui->atmosphereButtonLayout->removeItem(ui->atmosphereButtonLayout->takeAt(0));
    }
    this->repaint();

    while(atmosphere_buttons.length() > 0) {
        delete atmosphere_buttons.last();
        atmosphere_buttons.removeLast();
    }

    int row = 0;
    int col = 0;
    int max = qSqrt(atmosphere->objects.length());

    for(int i=0; i < atmosphere->objects.length(); i++) {
        atmosphere_buttons.append(new AGQPushButton(i, this));
        atmosphere_buttons.at(i)->setOID(atmosphere->objects.at(i)[0].toInt());
        atmosphere_buttons.at(i)->setCheckable(true);
        atmosphere_buttons.at(i)->setStyleSheet("text-align: left;");
        atmosphere_buttons.at(i)->setIcon(QIcon(":/application/icons/icon-atmosphere-start.png"));
        //atmosphere_buttons.at(i)->setPalette(QPalette(QColor(150, 200, 250)));
        atmosphere_buttons.at(i)->setObjectName(atmosphere->objects.at(i)[1]);
        atmosphere_buttons.at(i)->setText(atmosphere->objects.at(i)[1]);

        ui->atmosphereButtonLayout->addWidget(atmosphere_buttons.at(i), row, col);
        if(col < max) {
            col++;
        } else {
            col = 0;
            row++;
        }

        connect(atmosphere_buttons.at(i), SIGNAL(toggled(int,bool)), this, SLOT(atmosphereControl(int,bool)));
        //connect(atmosphere, SIGNAL(playbackError()), atmosphere_buttons.at(i), SLOT(setUnChecked()));
    }
}


void AmbienceGenerator::atmosphereControl(int pos_in_array, bool checked) {
    if(checked) {
        atmosphere->play(pos_in_array);
    } else {
        atmosphere->pause(pos_in_array);
    }
}


void AmbienceGenerator::atmosphereSetVolume() {
    atmosphere->setVolume(ui->atmosphereVolume->value());
}


void AmbienceGenerator::sfxCreateButtons() {
    while(ui->sfxButtonLayout->count() > 0) {
        ui->sfxButtonLayout->removeItem(ui->sfxButtonLayout->takeAt(0));
    }
    this->repaint();

    while(sfx_buttons.length() > 0) {
        delete sfx_buttons.last();
        sfx_buttons.removeLast();
    }

    int row = 0;
    int col = 0;
    int max = qSqrt(sfx->objects.length());

    for(int i=0; i < sfx->objects.length(); i++) {
        sfx_buttons.append(new AGQPushButton(i, this));
        sfx_buttons.at(i)->setOID(sfx->objects.at(i)[0].toInt());
        sfx_buttons.at(i)->setCheckable(true);
        sfx_buttons.at(i)->setStyleSheet("text-align: left;");
        sfx_buttons.at(i)->setIcon(QIcon(":/application/icons/icon-sfx-start.png"));
        //sfx_buttons.at(i)->setPalette(QPalette(QColor(250, 150, 150)));
        sfx_buttons.at(i)->setObjectName(sfx->objects.at(i)[1]);
        sfx_buttons.at(i)->setText(sfx->objects.at(i)[1]);

        ui->sfxButtonLayout->addWidget(sfx_buttons.at(i), row, col);
        if(col < max) {
            col++;
        } else {
            col = 0;
            row++;
        }

        connect(sfx_buttons.at(i), SIGNAL(toggled(int,bool)), this, SLOT(sfxControl(int,bool)));
    }
}

void AmbienceGenerator::sfxControl(int pos_in_array, bool checked) {
    if(checked) {
        sfx->play(pos_in_array);
    } else {
        sfx->pause(pos_in_array);
    }
}

void AmbienceGenerator::sfxSetVolume() {
    sfx->setVolume(ui->sfxVolume->value());
}


void AmbienceGenerator::singleshotCreateButtons() {
    while(ui->singleshotButtonLayout->count() > 0) {
        ui->singleshotButtonLayout->removeItem(ui->singleshotButtonLayout->takeAt(0));
    }
    this->repaint();

    while(singleshot_buttons.length() > 0) {
        delete singleshot_buttons.last();
        singleshot_buttons.removeLast();
    }

    int row = 0;
    int col = 0;
    int max = qSqrt(singleshot->objects.length());

    for(int i=0; i < singleshot->objects.length(); i++) {
        singleshot_buttons.append(new AGQPushButton(i, this));
        singleshot_buttons.at(i)->setOID(singleshot->objects.at(i)[0].toInt());
        singleshot_buttons.at(i)->setCheckable(true);
        singleshot_buttons.at(i)->setStyleSheet("text-align: left;");
        singleshot_buttons.at(i)->setIcon(QIcon(":/application/icons/icon-singleshot-start.png"));
        //singleshot_buttons.at(i)->setPalette(QPalette(QColor(200, 250, 200)));
        singleshot_buttons.at(i)->setObjectName(singleshot->objects.at(i)[1]);
        singleshot_buttons.at(i)->setText(singleshot->objects.at(i)[1]);

        ui->singleshotButtonLayout->addWidget(singleshot_buttons.at(i), row, col);
        if(col < max) {
            col++;
        } else {
            col = 0;
            row++;
        }

        connect(singleshot_buttons.at(i), SIGNAL(toggled(int,bool)), this, SLOT(singleshotControl(int,bool)));
        //connect(singleshot, SIGNAL(playbackError()), singleshot_buttons.at(i), SLOT(setUnChecked()));
        connect(singleshot->media_container.at(i), SIGNAL(finished(int)), singleshot_buttons.at(i), SLOT(setUnChecked()));
    }
}


void AmbienceGenerator::singleshotControl(int pos_in_array, bool checked) {
    if(checked) {
        singleshot->play(pos_in_array);
    } else {
        singleshot->stop(pos_in_array);
    }
}


void AmbienceGenerator::singleshotSetVolume() {
    singleshot->setVolume(ui->singleshotVolume->value());
}

void AmbienceGenerator::musicNext() {
    music->next();
}


/*!
 * Toggles music playing state
 */
void AmbienceGenerator::musicPlayPause() {
    if(ui->musicButtonPlayPause->isChecked()) {
        music->play();
    } else {
        music->pause();
    }
}


void AmbienceGenerator::musicTrackChanged() {
    QString title(music->getTag(music->relativeFilePath(music->media_container.at(0)->getCurrentFilename()), music->TITLE));
    QString artist(music->getTag(music->relativeFilePath(music->media_container.at(0)->getCurrentFilename()), music->ARTIST));
    QString album(music->getTag(music->relativeFilePath(music->media_container.at(0)->getCurrentFilename()), music->ALBUM));
    QString music_tag_label = this->tr("<b>%1</b> by <i>%2</i> from <i>%3</i>").arg(title, artist, album);
    ui->musicTagLabel->setText(music_tag_label);
}

void AmbienceGenerator::musicSetPlaylist(int index) {
    music->objects_tracks_model->selectObject(music->objects_model->data(music->objects_model->index(index, 0)).toInt());
    qDebug() << "\"music\"" << "playlist" << index;
}

void AmbienceGenerator::musicSetVolume() {
    music->setVolume(ui->musicVolume->value());
}


void AmbienceGenerator::musicSetSeek(int time, int length) {
    ui->musicSeek->setMaximum(length);
    ui->musicSeek->setValue(time);
}


void AmbienceGenerator::checkUpdate() {
    update_manager = new QNetworkAccessManager(this);
    connect(update_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(checkUpdateProcess(QNetworkReply*)));
    update_manager->get(QNetworkRequest(QUrl(QString("%1version").arg(WEBADDRESS))));
}


void AmbienceGenerator::checkUpdateProcess(QNetworkReply *rep) {
    QString version_raw;
    float version;
    version_raw = rep->readAll();
    version = version_raw.split("\n").at(0).toFloat();
    qDebug() << "This version" << VERSION.toFloat();
    qDebug() << "Remote version" << version;
    rep->close();
    rep->deleteLater();
    update_manager->deleteLater();
    if(version > VERSION.toFloat()) {
        QMessageBox::information(this, tr("New version available"), tr("<p><b>A new version of %1 is available!</b></p><p>Visit <a href=\"%2\">%2</a> to download the recent edition</p>").arg(NAME, WEBADDRESS), QMessageBox::Close);
    }
}


void AmbienceGenerator::showAbout() {
    qDebug() << "taglib:" << TAGLIB_MAJOR_VERSION << TAGLIB_MINOR_VERSION;
    qDebug() << "vlc:" << libvlc_get_version();
    QMessageBox::about(this, tr("About Yonder"), tr("<b>%1 %2</b><p>&copy; 2011 by %3</p><p><a href=\"%4\">%4</a></p><p><i>Running with:</i><br />Qt %5,<br />taglib %6.%7,<br />VLC %8</p>").arg(NAME, VERSION, AUTHOR, WEBADDRESS, QT_VERSION_STR, QString("%1").arg(TAGLIB_MAJOR_VERSION), QString("%1").arg(TAGLIB_MINOR_VERSION), QString("%1").arg(libvlc_get_version())));
}

void AmbienceGenerator::openOnlineHelp() {
    QDesktopServices::openUrl(QUrl(WEBADDRESS));
}

AmbienceGenerator::~AmbienceGenerator()
{
    delete ui;
    delete progress_bar;
    for(int i=0; i<atmosphere_buttons.length(); i++) {
        delete atmosphere_buttons.at(i);
    }
    for(int i=0; i<sfx_buttons.length(); i++) {
        delete sfx_buttons.at(i);
    }
    for(int i=0; i<singleshot_buttons.length(); i++) {
        delete singleshot_buttons.at(i);
    }
    delete singleshot;
    delete sfx;
    delete music;
    delete hotkeys;
    delete atmosphere;
}
