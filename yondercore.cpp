#include "yondercore.h"

extern QString VERSION;
extern QString WEBADDRESS;

YonderCore::YonderCore(QObject *parent) :
    QObject(parent)
{
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings settings;

    if(settings.value("Settings/check_update", 0).toBool()) {
        checkUpdate();
    }

    this->media = new MediaManager(this);
}

/*!
 * \brief YonderCore::loadProject
 * \param path - path to project folder
 * \return true only if successfully loaded
 * Create all Managers and Webapp if activated.
 */
bool YonderCore::projectLoad(QString path) {
    emit projectLoading();
    // Check if project path really really exists. Return if not.
    if(!QDir().exists(path)) {
        emit projectLoadingFailed();
        qDebug() << "loadProject failed: path does not exist";
        return false;
    }
    this->project_path = path;

    webappStop();

    QSettings settings;
    settings.setValue("Settings/last_project_path", project_path);

    QString db_filename = QString("%1/project.db").arg(project_path);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(db_filename);
    db.open();

    qDebug() << "Setting project path to" << this->project_path;

    emit managerLoading("atmosphere");
    if(!atmosphere) {
        delete atmosphere;
    }
    atmosphere = new AtmosphereManager(project_path, db, media, this);

    emit managerLoading("music");
    if(!music) {
        delete music;
    }
    music = new MusicManager(project_path, db, media, this);

    emit managerLoading("sfx");
    if(!sfx) {
        delete sfx;
    }
    sfx = new SfxManager(project_path, db, media, this);

    emit managerLoading("singleshots");
    if(!singleshot) {
        delete singleshot;
    }
    singleshot = new SingleshotManager(project_path, db, media, this);

    hotkeys = new HotkeysManager(db, atmosphere, music, sfx, singleshot, this);

    webappStart();

    emit projectLoaded();
    return true;
}

void YonderCore::projectRefresh() {
    emit projectRefreshing();

    webappStart();
    emit projectRefreshed();
}

void YonderCore::projectStop() {
    emit projectStopping();
    webappStop();
    emit projectStopped();
}

/*!
 * \brief YonderCore::webappStart
 * Starts the webapp module if it is not running and it is activated in settings
 */
void YonderCore::webappStart() {
    QSettings settings;
    if(!webapp) {
        qDebug() << "Creating new webapp instance";
        webapp = new Webapp(settings.value("Settings/webapp_port").toInt(), 10, QString(":webservice/static"), this);
        if(settings.value("Settings/webapp_active").toBool() && settings.value("Settings/webapp_port").toInt() != 0) {
            webapp->setHotkeysManager(hotkeys);
            webapp->setActive(true);
            emit webappStarted(webapp->getUrl());
        }
    }
}

/*!
 * \brief YonderCore::webappStop
 * Stops the webapp if it is running
 */
void YonderCore::webappStop() {
    if(webapp) {
        webapp->setActive(false);
        webapp->deleteLater();
    }
    emit webappStopped();
}

/*!
 * \brief YonderCore::checkUpdate
 * Call updateinfo-file
 */
void YonderCore::checkUpdate() {
    update_manager = new QNetworkAccessManager(this);
    connect(update_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(checkUpdateProcess(QNetworkReply*)));
    update_manager->get(QNetworkRequest(QUrl(QString("%1version").arg(WEBADDRESS))));
}

/*!
 * \brief YonderCore::checkUpdateProcess
 * \param rep
 * emit signal updateAvailable() if update is available
 */
void YonderCore::checkUpdateProcess(QNetworkReply *rep) {
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
        emit updateAvailable();
    }
}
