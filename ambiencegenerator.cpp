#include "ambiencegenerator.h"
#include "ui_ambiencegenerator.h"

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
    settings.sync();
    config_path = QDir(QFileInfo(settings.fileName()).absoluteDir()).absolutePath();
    project_set = false;

    this->splash_screen = splash_screen;

    // Setup Ui and MediaManager
    ui->setupUi(this);
    {
        media = new MediaManager(ui->frame_sidebar->progress_bar, this);

        start_frame = new FrameStart(this);
        ui->frame_wrapper->addWidget(start_frame);

        generator_frame = new FrameGenerator(this);
        ui->frame_wrapper->addWidget(generator_frame);

        editor_frame = new FrameEditor(media, this);
        ui->frame_wrapper->addWidget(editor_frame);

        resource_frame = new FrameResourceBrowser(this);
        ui->frame_wrapper->addWidget(resource_frame);

        configure_frame = new FrameConfigure(this);
        ui->frame_wrapper->addWidget(configure_frame);

        help_frame = new FrameHelp(this);
        ui->frame_wrapper->addWidget(help_frame);

        ui->frame_sidebar->addTab(start_frame, tr("Start"), tr("Ctrl+P"));
        ui->frame_sidebar->addTab(generator_frame, tr("Generate"), tr("Ctrl+G"));
        ui->frame_sidebar->addTab(editor_frame, tr("Edit"), tr("Ctrl+E"));
        ui->frame_sidebar->addTab(resource_frame, tr("Resources"), tr("Ctrl+R"));
        ui->frame_sidebar->addTab(configure_frame, tr("Configure"), tr("Ctrl+C"));
        ui->frame_sidebar->addTab(help_frame, tr("Help"), tr("Ctrl+H"));

        // only enabled after project is loaded. Default to start tab
        ui->frame_sidebar->setTabEnabled(1, false);
        ui->frame_sidebar->setTabEnabled(2, false);
        ui->frame_sidebar->setActive(0);
    }

    // Quit immediately if clicked close. Open browser if clicked on webapp button
    connect(ui->frame_sidebar->quit_button, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->frame_sidebar->webapp_running, SIGNAL(clicked()), this, SLOT(webappOpenUrl()));

    // Load project setProject returns to default state if no path is set.
    connect(start_frame, SIGNAL(acceptedProjectFolder(QString)), this, SLOT(setProject(QString)));
    setProject(settings.value("Settings/last_project_path", "").toString());

    connect(generator_frame, SIGNAL(deactivated()), this, SLOT(webappStop()));
    connect(generator_frame, SIGNAL(activated()), generator_frame, SLOT(refreshSoundUi()));
    connect(generator_frame, SIGNAL(soundUiRefreshed()), this, SLOT(refreshProject()));

    generator_frame->refreshSoundUi(project_set);

    // Pause everything when editor is launched
    connect(editor_frame, SIGNAL(activated()), this, SLOT(ambienceEditor()));

    // Set style. Signals for changed settings while running
    applyStylesheet();
    connect(configure_frame, SIGNAL(deactivated()), this, SLOT(applyStylesheet()));

    this->setWindowTitle(QString("%1 %2").arg(QApplication::instance()->applicationName(), VERSION));
    this->restoreGeometry(settings.value("MainWindow/geometry").toByteArray());

    if(settings.value("Settings/check_update", 0).toBool()) {
        checkUpdate();
    }


}


/*!
 * Pauses all managers and launches AmbienceEditor
 */
void AmbienceGenerator::ambienceEditor() {

    for(int i=0; i < generator_frame->atmosphere_buttons.length(); i++) {
       generator_frame->atmosphere_buttons.at(i)->setChecked(false);
    }

    for(int i=0; i < generator_frame->sfx_buttons.length(); i++) {
        generator_frame->sfx_buttons.at(i)->setChecked(false);
    }

    for(int i=0; i < generator_frame->singleshot_buttons.length(); i++) {
        generator_frame->singleshot_buttons.at(i)->setChecked(false);
    }

    generator_frame->music_play_pause->setChecked(false);
}


/*!
 * \brief AmbienceGenerator::setProject
 * \param project_path
 * Resets all managers and loads new project. Should only be called with valid project path.
 */
void AmbienceGenerator::setProject(QString project_path) {
    // For security, check if project path really really exists. Return if not.
    if(!QDir().exists(project_path)) {
        ui->frame_sidebar->setTabEnabled(1, false);
        ui->frame_sidebar->setTabEnabled(2, false);
        ui->frame_sidebar->setActive(0);
        return;
    }
    ui->frame_sidebar->setTabEnabled(1, false);
    ui->frame_sidebar->setTabEnabled(2, false);
    this->project_path = project_path;

    webappStop();

    QSettings settings;
    settings.setValue("Settings/last_project_path", project_path);

    QString db_filename = QString("%1/project.db").arg(project_path);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(db_filename);
    db.open();

    qDebug() << "Setting project path to" << this->project_path;

    ui->frame_sidebar->progress_bar->show();

    if(!atmosphere) {
        delete atmosphere;
    }
    splash_screen->showMessage(tr("Loading atmosphere"), 0x0041, Qt::white);

    atmosphere = new AtmosphereManager(project_path, db, media, this);

    if(!music) {
        delete music;
    }
    splash_screen->showMessage(tr("Loading music"), 0x0041, Qt::white);
    music = new MusicManager(project_path, db, media, this);

    if(!sfx) {
        delete sfx;
    }
    splash_screen->showMessage(tr("Loading sfx"), 0x0041, Qt::white);
    sfx = new SfxManager(project_path, db, media, this);

    if(!singleshot) {
        delete singleshot;
    }
    splash_screen->showMessage(tr("Loading singleshots"), 0x0041, Qt::white);
    singleshot = new SingleshotManager(project_path, db, media, this);

    generator_frame->setSoundManagers(atmosphere, sfx, music, singleshot);

    ui->frame_sidebar->setTabEnabled(1, true);
    ui->frame_sidebar->setTabEnabled(2, true);
    start_frame->hide(); // weird behaviour fix
    ui->frame_sidebar->setActive(1);
    ui->frame_sidebar->progress_bar->hide();

    project_set = true;
}

void AmbienceGenerator::refreshProject() {
    if(project_set) {
        hotkeys = new HotkeysManager(db, music, generator_frame->atmosphere_buttons, generator_frame->sfx_buttons, generator_frame->singleshot_buttons, generator_frame->music_select_playlist, generator_frame->music_next, generator_frame->music_play_pause, this);
        generator_frame->setHotkeysManager(hotkeys);
        editor_frame->setManagers(atmosphere, sfx, music, singleshot, hotkeys);
        webappStart();
    }
}


void AmbienceGenerator::webappStart() {
    QSettings settings;
    if(!webapp) {
        webapp = new Webapp(settings.value("Settings/webapp_port").toInt(), 10, QString(":webservice/static"), this);
        if(settings.value("Settings/webapp_active").toBool() && settings.value("Settings/webapp_port").toInt() != 0) {
            webapp->setHotkeysManager(hotkeys);
            ui->frame_sidebar->webapp_running->setToolTip(tr("Open webapp in browser\nAddress:%1").arg(webapp->getUrl()));
            ui->frame_sidebar->webapp_running->show();
            webapp->setActive(true);
        }
    }
}


void AmbienceGenerator::webappStop() {
    ui->frame_sidebar->webapp_running->hide();
    if(webapp) {
        webapp->setActive(false);
        webapp->deleteLater();
    }
}


void AmbienceGenerator::webappOpenUrl() {
    QDesktopServices::openUrl(QUrl(webapp->getUrl()));
}

void AmbienceGenerator::applyStylesheet() {
    QSettings settings;
    if(settings.value("Settings/custom_style", 1).toBool()) {
        QFile stylesheet(":application/style.css");
        QFontDatabase::addApplicationFont(":application/ostrich_regular.ttf");
        stylesheet.open(QIODevice::ReadOnly);
        setStyleSheet(QString(stylesheet.readAll()));
        stylesheet.close();
        ui->frame_sidebar->applyStylesheet(true);
    } else {
        setStyleSheet(QString());
        ui->frame_sidebar->applyStylesheet(false);
    }
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

void AmbienceGenerator::openOnlineHelp() {
    QDesktopServices::openUrl(QUrl(WEBADDRESS));
}

AmbienceGenerator::~AmbienceGenerator()
{
    qDebug() << "Yonder is shutting down!";

    QSettings settings;
    settings.setValue("MainWindow/geometry", this->saveGeometry());
    settings.sync();

    delete ui;

    qDebug() << "Bye";
}
