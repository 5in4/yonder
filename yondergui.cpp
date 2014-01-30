#include "yondergui.h"
#include "ui_yondergui.h"

extern QString NAME;
extern QString VERSION;
extern QString ACTION_KEY;
extern QString AUTHOR;
extern QString WEBADDRESS;
extern QString ACCEPTED_MIMETYPES;

YonderGui::YonderGui(QSplashScreen *splash_screen, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::YonderGui)
{

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings settings;
    // settings.sync();

    core = new YonderCore(this);
    connect(core, &YonderCore::updateAvailable, this, &YonderGui::openUpdater);

    this->splash_screen = splash_screen;

    // Setup Ui
    ui->setupUi(this);
    {
        start_frame = new FrameStart(this);
        ui->frame_wrapper->addWidget(start_frame);

        generator_frame = new FrameGenerator(core, this);
        ui->frame_wrapper->addWidget(generator_frame);

        editor_frame = new FrameEditor(core->media, this);
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
    connect(ui->frame_sidebar->quit_button, &SolidTabButton::clicked, this, &YonderGui::close);
    connect(ui->frame_sidebar->webapp_running, &SolidTabButton::clicked, this, &YonderGui::openWebappUrl);

    connect(core, &YonderCore::managerLoading, this, &YonderGui::setSplashScreen);
    connect(core, &YonderCore::projectLoading, this, &YonderGui::stateLoading);
    connect(core, &YonderCore::projectLoadingFailed, this, &YonderGui::stateLoadingFailed);
    connect(core, &YonderCore::projectLoaded, this, &YonderGui::stateLoaded);
    connect(core, &YonderCore::projectRefreshed, this, &YonderGui::stateRefreshed);
    connect(core, &YonderCore::webappStarted, this, &YonderGui::webappStarted);
    connect(core, &YonderCore::webappStopped, this, &YonderGui::webappStopped);

    connect(generator_frame, SIGNAL(deactivated()), core, SLOT(projectStop()));
    connect(generator_frame, SIGNAL(activated()), core, SLOT(projectRefresh()));


    // Load project setProject returns to default state if no path is set.
    connect(start_frame, SIGNAL(acceptedProjectFolder(QString)), core, SLOT(projectLoad(QString)));
    core->projectLoad(settings.value("Settings/last_project_path", "").toString());

    // Set style. Signals for changed settings while running
    applyStylesheet();
    connect(configure_frame, &FrameConfigure::deactivated, this, &YonderGui::applyStylesheet);
    this->setWindowTitle(QString("%1 %2").arg(QApplication::instance()->applicationName(), VERSION));
    this->restoreGeometry(settings.value("MainWindow/geometry").toByteArray());

}

/*!
 * \brief YonderGui::stateLoading
 * Set ui to state for loading a project
 */
void YonderGui::stateLoading() {
    ui->frame_sidebar->setTabEnabled(1, false);
    ui->frame_sidebar->setTabEnabled(2, false);
    ui->frame_sidebar->progress_bar->show(); //repair signals
}

/*!
 * \brief YonderGui::stateLoadingFailed
 * Set ui to state if loading a project failed
 */
void YonderGui::stateLoadingFailed() {
    ui->frame_sidebar->setActive(0);
}

/*!
 * \brief YonderGui::stateLoaded
 * Set ui to state right after a project finished loading
 */
void YonderGui::stateLoaded() {
    ui->frame_sidebar->setTabEnabled(1, true);
    ui->frame_sidebar->setTabEnabled(2, true);
    start_frame->hide(); // weird behaviour fix
    ui->frame_sidebar->setActive(1);
    ui->frame_sidebar->progress_bar->hide();
    editor_frame->setManagers(core->atmosphere, core->sfx, core->music, core->singleshot, core->hotkeys);
}

void YonderGui::stateRefreshed() {
    generator_frame->refreshUi();
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
    ui->frame_sidebar->webapp_running->setToolTip(tr("Open webapp in browser\nAddress:%1").arg(webapp_url.toString()));
    ui->frame_sidebar->webapp_running->show();
}

/*!
 * \brief YonderGui::webappStopped
 * Update ui after webapp stopped
*/
void YonderGui::webappStopped() {
    ui->frame_sidebar->webapp_running->hide();
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
        setStyleSheet(QString(stylesheet.readAll()));
        stylesheet.close();
        ui->frame_sidebar->applyStylesheet(true);
    } else {
        setStyleSheet(QString());
        ui->frame_sidebar->applyStylesheet(false);
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

YonderGui::~YonderGui()
{
    qDebug() << "Yonder is shutting down!";

    QSettings settings;
    settings.setValue("MainWindow/geometry", this->saveGeometry());
    settings.sync();

    delete ui;

    qDebug() << "So this is goodbye";
}
