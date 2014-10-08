#include "yondergui.h"
#include "ui_yondergui.h"

extern QString NAME;
extern QString VERSION;
extern QString ACTION_KEY;
extern QString AUTHOR;
extern QString WEBADDRESS;
extern QString ACCEPTED_MIMETYPES;
extern QString YONDER_MIMETYPE;


YonderGui::YonderGui(QSplashScreen *splash_screen, QWidget *parent) : QMainWindow(parent), ui(new Ui::YonderGui) {

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings settings;
    // settings.sync();

    core = new YonderCore(this);
    connect(core, &YonderCore::updateAvailable, this, &YonderGui::openUpdater);

    this->splash_screen = splash_screen;

    // setup ui
    ui->setupUi(this);

    mapper_menu = new QSignalMapper(this);
    connect(ui->btn_start, SIGNAL(clicked()), mapper_menu, SLOT(map()));
    connect(ui->btn_generate, SIGNAL(clicked()), mapper_menu, SLOT(map()));
    connect(ui->btn_edit, SIGNAL(clicked()), mapper_menu, SLOT(map()));
    connect(ui->btn_resources, SIGNAL(clicked()), mapper_menu, SLOT(map()));
    connect(ui->btn_configure, SIGNAL(clicked()), mapper_menu, SLOT(map()));
    connect(ui->btn_help, SIGNAL(clicked()), mapper_menu, SLOT(map()));

    mapper_menu->setMapping(ui->btn_start, 0);
    mapper_menu->setMapping(ui->btn_generate, 1);
    mapper_menu->setMapping(ui->btn_edit, 2);
    mapper_menu->setMapping(ui->btn_resources, 3);
    mapper_menu->setMapping(ui->btn_configure, 4);
    mapper_menu->setMapping(ui->btn_help, 5);

    connect(mapper_menu, SIGNAL(mapped(int)), ui->frame, SLOT(setCurrentIndex(int)));
    //connect(ui->frame, SIGNAL(currentChanged(int)), this, SLOT() // fixme sort ui and core state
    connect(ui->btn_webapp, SIGNAL(clicked()), this, SLOT(openWebappUrl()));
    connect(ui->btn_about, SIGNAL(clicked()), this, SLOT(showAbout()));
    connect(ui->btn_quit, SIGNAL(clicked()), this, SLOT(close()));

    connect(ui->btn_soundbank_open, SIGNAL(clicked()), this, SLOT(soundbankOpen()));
    connect(ui->btn_soundbank_create, SIGNAL(clicked()), this, SLOT(soundbankCreate()));

    // setup core
    connect(core, &YonderCore::managerLoading, this, &YonderGui::setSplashScreen);
    connect(core, &YonderCore::projectLoading, this, &YonderGui::stateLoading);
    connect(core, &YonderCore::projectLoadingFailed, this, &YonderGui::stateLoadingFailed);
    connect(core, &YonderCore::projectLoaded, this, &YonderGui::stateLoaded);
    connect(core, &YonderCore::projectRefreshed, this, &YonderGui::stateRefreshed);
    connect(core, &YonderCore::webappStarted, this, &YonderGui::webappStarted);
    connect(core, &YonderCore::webappStopped, this, &YonderGui::webappStopped);

    // setup generator
    //    connect(generator_frame, SIGNAL(deactivated()), core, SLOT(projectStop()));
    //    connect(generator_frame, SIGNAL(activated()), core, SLOT(projectRefresh()));

    // setup editor
    ui->btn_library_add->addAction(ui->action_add_files_to_library);
    ui->btn_library_add->addAction(ui->action_add_stream_to_library);
    connect(ui->btn_library_add, SIGNAL(clicked()), this, SLOT(soundbankAddFilesMusic()));
    connect(ui->action_add_files_to_library, SIGNAL(triggered()), this, SLOT(soundbankAddFilesMusic()));
    connect(ui->action_add_stream_to_library, SIGNAL(triggered()), this, SLOT(soundbankAddStream()));

    connect(ui->checkbox_library_music, SIGNAL(toggled(bool)), this, SLOT(editorLibraryMusic(bool)));

    // Load project setProject returns to default state if no path is set.
    //connect(start_frame, SIGNAL(acceptedProjectFolder(QString)), core, SLOT(projectLoad(QString)));
    core->projectLoad(settings.value("Settings/LastSoundbank", "").toString());

    // Set style. Signals for changed settings while running
    applyStylesheet();
    //connect(configure_frame, &FrameConfigure::deactivated, this, &YonderGui::applyStylesheet);
    this->setWindowTitle(QString("%1 %2").arg(QApplication::instance()->applicationName(), VERSION));
    this->restoreGeometry(settings.value("MainWindow/Geometry").toByteArray());

}


/*!
 * \brief open dialog to set soundbank, perform brief check on file and load in core
 */
void YonderGui::soundbankOpen() {
    QString soundbank_path = QFileDialog::getOpenFileName(this, tr("Open soundbank"), QDir::homePath(), tr("Yonder soundbank (%1)").arg(YONDER_MIMETYPE));
    if(soundbank_path.isEmpty()) {
        return;
    }
    QFileInfo soundbank_check(soundbank_path);
    QErrorMessage alert;
    if(!soundbank_check.isFile()) {
        alert.showMessage(tr("This is not a file"));
        return;
    }
    if(!soundbank_check.exists()) {
        alert.showMessage(tr("This file does not exist"));
        return;
    }
    if(!soundbank_check.isWritable()) {
        alert.showMessage(tr("This file is not writable"));
        return;
    }
    core->projectLoad(soundbank_path);
}


void YonderGui::soundbankCreate() {
    QString soundbank_path = QFileDialog::getSaveFileName(this, tr("Create soundbank"), QDir::homePath(), tr("Yonder soundbank (%1)").arg(YONDER_MIMETYPE));
    if(soundbank_path.isEmpty()) {
        return;
    }
    soundbank_path += ".yfx";
    core->projectCreate(soundbank_path);
}


/*
 * \brief open dialog to add files
 */
void YonderGui::soundbankAddFiles() {
    QStringList paths = QFileDialog::getOpenFileNames(this, tr("Add sfx files to soundbank"), QDir::homePath(), tr("Music files (%1)").arg(ACCEPTED_MIMETYPES));
    if(!paths.isEmpty()) {
        core->soundbankAddFiles(paths, false);
    }
}


/*
 * \brief open dialog to add music files
 */
void YonderGui::soundbankAddFilesMusic() {
    QStringList paths = QFileDialog::getOpenFileNames(this, tr("Add music files to soundbank"), QDir::homePath(), tr("Music files (%1)").arg(ACCEPTED_MIMETYPES));
    if(!paths.isEmpty()) {
        core->soundbankAddFiles(paths, true);
    }
}


/*
 * \brief open dialog to enter stream url
 */
void YonderGui::soundbankAddStream() {

}


/*!
 * \brief YonderGui::stateLoading
 * Set ui to state for loading a project
 */
void YonderGui::stateLoading() {
//    ui->frame_sidebar->setTabEnabled(1, false);
//    ui->frame_sidebar->setTabEnabled(2, false);
//    ui->frame_sidebar->progress_bar->show(); //repair signals
}

/*!
 * \brief YonderGui::stateLoadingFailed
 * Set ui to state if loading a project failed
 */
void YonderGui::stateLoadingFailed() {
//    ui->frame_sidebar->setActive(0);
}

/*!
 * \brief YonderGui::stateLoaded
 * Set ui to state right after a project finished loading
 */
void YonderGui::stateLoaded() {
    ui->editor_music_playlists->setModel(core->music->model_playlists);
    ui->editor_music_playlists->setModelColumn(2);
    ui->editor_library->setModel(core->model_library);
    ui->editor_library->hideColumn(2);
    ui->editor_library->hideColumn(3);
    ui->editor_library->hideColumn(4);
//    ui->frame_sidebar->setTabEnabled(1, true);
//    ui->frame_sidebar->setTabEnabled(2, true);
//    start_frame->hide(); // weird behaviour fix
//    ui->frame_sidebar->setActive(1);
//    ui->frame_sidebar->progress_bar->hide();
}

void YonderGui::stateRefreshed() {
    //generator_frame->refreshUi();
}


/*!
 * \brief set library model to chosen filter
 */
void YonderGui::editorLibraryMusic(bool show) {
    QDjangoWhere filter = core->model_library->filter();
    filter = filter && QDjangoWhere("isMusic", QDjangoWhere::Equals, show);
    core->model_library->setFilter(filter);
}


/*!
 * \brief YonderGui::setSplashScreen
 * \param message
 * Update splash screen on first load
 */
void YonderGui::setSplashScreen(QString message) {
    splash_screen->showMessage(tr("Loading %1").arg(message), 0x0041, Qt::white);
}

/*!
 * \brief YonderGui::webappStarted
 * \param url
 * Update ui after webapp started
 */
void YonderGui::webappStarted(QUrl url) {
    this->webapp_url = url;
//    ui->frame_sidebar->webapp_running->setToolTip(tr("Open webapp in browser\nAddress:%1").arg(webapp_url.toString()));
//    ui->frame_sidebar->webapp_running->show();
}

/*!
 * \brief YonderGui::webappStopped
 * Update ui after webapp stopped
*/
void YonderGui::webappStopped() {
//    ui->frame_sidebar->webapp_running->hide();
}

/*!
 * \brief YonderGui::applyStylesheet
 * Apply fancy stylesheet if it is acitvated in preferences, otherwise set default desktop style
 */
void YonderGui::applyStylesheet() {
    QSettings settings;
    if(settings.value("Settings/custom_style", 1).toBool()) {
        QFile stylesheet(":application/style.css");
        QFontDatabase::addApplicationFont(":application/ostrich_regular.ttf");
        stylesheet.open(QIODevice::ReadOnly);
        qApp->setStyleSheet(QString(stylesheet.readAll()));
        stylesheet.close();
    } else {
        qApp->setStyleSheet(QString());
    }
}

/*!
 * \brief YonderGui::openWebappUrl
 * Open the webapp in the default browser
 */
void YonderGui::openWebappUrl() {
    QDesktopServices::openUrl(webapp_url);
}

/*!
 * \brief YonderGui::openOnlineHelp
 * Open help url in default browser.
 */
void YonderGui::openOnlineHelp() {
    QDesktopServices::openUrl(QUrl(WEBADDRESS));
}

/*!
 * \brief YonderGui::openUpdater
 * Launch updating mechanism. Right now it's only a messagebox.
 */
void YonderGui::openUpdater() {
    QMessageBox::information(this, tr("New version available"), tr("<p><b>A new version of %1 is available!</b></p><p>Visit <a href=\"%2\">%2</a> to download the recent version.</p>").arg(NAME, WEBADDRESS), QMessageBox::Close);
}


void YonderGui::showAbout() {
    QMessageBox::about(this, tr("About Yonder"), tr("<b>%1 %2</b><p>&copy; 2011-2013 by %3</p><p><a href=\"%4?pk_campaign=app&pk_kwd=about\">%4</a></p><p><i>Running with:</i><br />Qt %5<br />TagLib %6.%7<p><i>Audio engine:</i><br />FMOD Sound System by Firelight Technologies</p><br /><img src=\":/application/qt-logo.svg\" /><img src=\":/application/fmod-logo.svg\" />").arg(NAME, VERSION, AUTHOR, WEBADDRESS, QT_VERSION_STR, QString("%1").arg(TAGLIB_MAJOR_VERSION), QString("%1").arg(TAGLIB_MINOR_VERSION)));
}

YonderGui::~YonderGui() {
    QSettings settings;
    settings.setValue("MainWindow/Geometry", this->saveGeometry());
    settings.sync();

    delete ui;
}
