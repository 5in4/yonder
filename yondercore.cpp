#include "yondercore.h"

extern QString VERSION;
extern QString WEBADDRESS;


FMOD_SYSTEM * YonderCore::system;


YonderCore::YonderCore(QObject *parent) : QObject(parent) {
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings settings;

    if(settings.value("Settings/check_update", 0).toBool()) {
        checkUpdate();
    }

    // setup database
    QDjango::setDebugEnabled(true);
    QDjango::registerModel<Track>();
    QDjango::registerModel<SfxContainer>();
    QDjango::registerModel<SfxBit>();
    QDjango::registerModel<SfxBitTrack>();

    // startup fmod
    FMOD_Debug_SetLevel(FMOD_DEBUG_LEVEL_ALL);
    Q_ASSERT(FMOD_OK == FMOD_System_Create(&system));
    Q_ASSERT(FMOD_OK == FMOD_System_Init(system, 1000, FMOD_INIT_NORMAL, 0));
    sound_loop_timeout = new QTimer(this);
    connect(sound_loop_timeout, SIGNAL(timeout()), this, SLOT(soundLoop()));
    sound_loop_timeout->start(10);

    // initialize duktape
    duk = duk_create_heap_default();
    Q_ASSERT(0 != duk);
}


void YonderCore::projectCreate(QString path) {
    qDebug() << "Creating new soundbank";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
    db.open();
    QDjango::setDatabase(db);
    QDjango::createTables();
    db.commit();
    db.close();
    projectLoad(path);
}

/*!
 * \brief YonderCore::loadProject
 * \param path - path to project folder
 * \return true only if successfully loaded
 * Create all Managers and Webapp if activated.
 */
bool YonderCore::projectLoad(QString path) {
    if(!QFile().exists(path)) {
        emit projectLoadingFailed();
        qDebug() << "Soundbank does not exist:" << path;
        return false;
    }

    emit projectLoading();

    this->project_path = path;

    //webappStop();

    QSettings settings;
    settings.setValue("Settings/LastSoundbank", project_path);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(project_path);
    db.open();
    QDjango::setDatabase(db);

    model_library = new TrackTableModel<Track>(this);

    emit managerLoading("sfx");
    sfx = new SfxManager(this);
    music = new MusicManager(this);

    //webappStart();
    qDebug() << "Opened soundbank" << this->project_path;
    emit projectLoaded();
    return true;
}


/*
 * \brief Read file, parse tags and insert in db
 */
void YonderCore::soundbankAddFiles(QStringList paths, bool is_music) {
    QStringList::iterator path;
    QDjango::database().transaction();
    for(path=paths.begin(); path!=paths.end(); ++path) {
        model_library->setData(model_library->index(0, 0), QVariant(*path), Qt::EditRole);
    }
    QDjango::database().commit();
}


/*
 * \brief Read stream, parse tags and insert in db
 */
void YonderCore::soundbankAddStream(QUrl path) {

}


void YonderCore::soundbankAddPlaylists(QStringList names) {
    QDjango::database().transaction();
    foreach(const QString name, names) {
        music->model_playlists->setData(music->model_playlists->index(0, 0), QVariant(name), Qt::EditRole);
    }
    QDjango::database().commit();

    QDjangoQuerySet<SfxContainer> sfx_container_query;
    QDjango::database().transaction();
    foreach(const QString name, names) {
        SfxBit sb;
        sb.setContainer(sfx_container_query.get(QDjangoWhere("name", QDjangoWhere::Equals, name)));
        sb.save();
    }
    QDjango::database().commit();
}


void YonderCore::projectRefresh() {
    emit projectRefreshing();

    //webappStart();
    emit projectRefreshed();
}

void YonderCore::projectStop() {
    emit projectStopping();
    //webappStop();
    emit projectStopped();
}


void YonderCore::soundLoop() {
//    QList<MediaContainer *>::const_iterator i;
//    for(i=containers.constBegin(); i!=containers.constEnd(); ++i) {
//        (*i)->checkFinished();
//    }
    Q_ASSERT(FMOD_OK == FMOD_System_Update(system));
}


void YonderCore::sfxBitAddTracks(int sfx_bit_id, QList<int> track_ids) {
    QDjango::database().transaction();
    QDjangoQuerySet<SfxBit> sfx_bit_query;
    QDjangoQuerySet<Track> track_query;
//    SfxBit *sfx_bit = sfx_bit_query.get(QDjangoWhere("id", QDjangoWhere::Equals, sfx_bit_id));
    foreach(const int track_id, track_ids) {
        SfxBitTrack sbt;
        sbt.setSfxBit(sfx_bit_query.get(QDjangoWhere("id", QDjangoWhere::Equals, sfx_bit_id)));
        sbt.setTrack(track_query.get(QDjangoWhere("id", QDjangoWhere::Equals, track_id)));
        sbt.save();
    }
    QDjango::database().commit();
    emit sfxBitTrackAdded();
}

/*!
 * \brief YonderCore::webappStart
 * Starts the webapp module if it is not running and it is activated in settings
 */
void YonderCore::webappStart() {
    return;
//    QSettings settings;
//    if(!webapp) {
//        qDebug() << "Creating new webapp instance";
//        webapp = new Webapp(settings.value("Settings/webapp_port").toInt(), 10, QString(":webservice/static"), this);
//        if(settings.value("Settings/webapp_active").toBool() && settings.value("Settings/webapp_port").toInt() != 0) {
//            webapp->setHotkeysManager(hotkeys);
//            webapp->setActive(true);
//            emit webappStarted(webapp->getUrl());
//        }
//    }
}

/*!
 * \brief YonderCore::webappStop
 * Stops the webapp if it is running
 */
void YonderCore::webappStop() {
//    if(webapp) {
//        webapp->setActive(false);
//        webapp->deleteLater();
//    }
//    emit webappStopped();
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


YonderCore::~YonderCore() {
    //QSqlQuery query_vacuum("VACUUM", QDjango::database());
    //query_vacuum.exec();
}
