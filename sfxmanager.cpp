#include "sfxmanager.h"


/*!
 * Default startup + randomizer seed
 * Available containers:
 * objects: List of all objects.
 * objects_status: Status of objects in "objects", id == objects.id
 * objects_tracks: All available tracks
 * objects_range_start: Start of tracks for object in objects_tracks, id == objects.id
 * objects_range_stop: Stop of tracks for object in objects_tracks, id == objects.id
 * media_container: id == objects.id
 */
SfxManager::SfxManager(QString project_path, QSqlDatabase db, MediaManager *media, QObject *parent): SoundManager(project_path, QString("sfx"), db, media, parent)
{
    qsrand(QTime::currentTime().msec());

    createTables();

    library_model = new LibraryModel(library_identifier);
    objects_model = new ObjectsModel(objects_identifier);
    objects_tracks_model = new ObjectsTracksModel(objects_tracks_identifier, library_identifier);
    objects_tracks_model->setHeaderData(2, Qt::Horizontal, tr("Track"), Qt::DisplayRole);
    objects_tracks_model->setHeaderData(3, Qt::Horizontal, tr("Pause"), Qt::DisplayRole);
    objects_tracks_model->setHeaderData(4, Qt::Horizontal, tr("Maximum offset before"), Qt::DisplayRole);
    objects_tracks_model->setHeaderData(5, Qt::Horizontal, tr("Maximum offset after"), Qt::DisplayRole);

    rescanLibrary();
    createObjectsList();
    connect(objects_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(createObjectsList()));
}


/*!
 * Overwriting SoundManager::createTables()
 * objects_tracks_identifier table adjusted
 */
void SfxManager::createTables() {
    this->updateDatabase(this->library_identifier, QString("CREATE TABLE \"%1\" (\"id\" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , \"path\" TEXT, \"last-change\" FLOAT, \"length\" INTEGER, \"artist\" VARCHAR, \"album\" VARCHAR, \"title\" VARCHAR)").arg(this->library_identifier));
    this->updateDatabase(this->objects_identifier, QString("CREATE TABLE \"%1\" (\"id\" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , \"name\" VARCHAR NOT NULL )").arg(this->objects_identifier));
    this->updateDatabase(this->objects_tracks_identifier, QString("CREATE TABLE \"%1\"(\"id\" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , \"oid\" INTEGER, \"tid\" INTEGER, \"time-pause\" INTEGER DEFAULT(0), \"pause-lower-boundary-shift\" INTEGER DEFAULT(0), \"pause-upper-boundary-shift\" INTEGER DEFAULT(0))").arg(this->objects_tracks_identifier));
    this->updateDatabase(QString("%1-index").arg(this->objects_tracks_identifier), QString("CREATE UNIQUE INDEX \"%1-index\" on \"%2\" (oid, tid)").arg(this->objects_tracks_identifier, this->objects_tracks_identifier));
}


/*!
 * Creates list of all known objects
 */
void SfxManager::createObjectsList() {
    objects = QList<QStringList>();
    objects_status.clear();
    QSqlQuery query;
    query.prepare(QString("SELECT `id`, `name` FROM \"%1\"").arg(objects_identifier));
    query.exec();
    while(query.next()) {
        QStringList object;
        object.append(query.value(0).toString()); // DIRRRTY
        object.append(query.value(1).toString());
        objects.append(object);
        objects_status.append(false);
        //qDebug() << identifier << "object add" << object;
    }
    createChannels();
}


/*!
 * Overwriting SoundManager::createChannels(int))
 */
void SfxManager::createChannels() {
    for(int i=0; i<objects.length(); i++) {
        int oid = objects.at(i)[0].toInt();

        QSqlQuery query;
        query.prepare(QString("SELECT `tid`, `time-pause`, `pause-lower-boundary-shift`, `pause-upper-boundary-shift` FROM %1 WHERE `oid`  = :oid").arg(this->objects_tracks_identifier));
        query.bindValue(":oid", oid);
        query.exec();

        int j;
        if(objects_range_start.isEmpty()) {
            j = 0;
        } else {
            j = objects_range_start.last() + 1;
        }

        objects_range_start.append(objects_tracks.length());

        int m = 0;
        while(query.next()){
            QStringList object;
            object.append(query.value(0).toString());
            object.append(query.value(1).toString());
            object.append(query.value(2).toString());
            object.append(query.value(3).toString());
            object.append(QString("%1").arg(false));
            objects_tracks.append(object);
            m++;
        }

        if(m == 0) {
            QStringList object;
            object.append("");
            object.append("");
            object.append("");
            object.append("");
            objects_tracks.append(object);
        }

        objects_range_stop.append(objects_tracks.length() - 1);

        for(int k = objects_range_start[i]; k <= objects_range_stop[i]; k++) {
            container.append(media->createContainer());
            container.at(k)->setChannel(k);
            connect(container.at(k), SIGNAL(finished(int)), this, SLOT(playDelayed(int)));
            channels++;
        }

    }
}


/*!
 * Toggles playing on object
 */
void SfxManager::playPause(int pos_in_array) {
    if(objects_status.at(pos_in_array) == true) {
        pause(pos_in_array);
    } else {
        play(pos_in_array);
    }
}


/*!
 * Starts playing on object
 */
void SfxManager::play(int pos_in_array) {
    for(int i=objects_range_start.at(pos_in_array); i<=objects_range_stop.at(pos_in_array); i++) {
        objects_status[pos_in_array] = true;
        QStringList object;
        object.append(objects_tracks.at(i).at(0));
        object.append(objects_tracks.at(i).at(1));
        object.append(objects_tracks.at(i).at(2));
        object.append(objects_tracks.at(i).at(3));
        object.append(QString("%1").arg(objects_status.at(pos_in_array)));
        objects_tracks.replace(i, object);
        playDelayed(i);
        qDebug() << identifier << "state" << "\"playing\"";
    }
}


/*!
 * Pauses playing on Object. Clear queue to prevent queued objects from playing after pause
 */
void SfxManager::pause(int pos_in_array) {
    for(int i=objects_range_start.at(pos_in_array); i<=objects_range_stop.at(pos_in_array); i++) {
        objects_status[pos_in_array] = false;
        QStringList object;
        object.append(objects_tracks.at(i).at(0));
        object.append(objects_tracks.at(i).at(1));
        object.append(objects_tracks.at(i).at(2));
        object.append(objects_tracks.at(i).at(3));
        object.append(QString("%1").arg(objects_status.at(pos_in_array)));
        objects_tracks.replace(i, object);
        container.at(i)->delay_timer->stop();
        container.at(i)->stop();
        qDebug() << identifier << "state" << "\"paused\"";
    }
}


/*!
 * Sets delay and starts timefr in media_container
 */
void SfxManager::playDelayed(int cid) {
    #ifdef _WIN32
    qrand();
    #endif
    QStringList channel = objects_tracks.at(cid);
    int min = channel[1].toInt() - channel[2].toInt();
    int max = channel[1].toInt() + channel[3].toInt();
    int start = (qrand()%(max-min+1))+min;
    qDebug() << identifier << cid << "delay" << start;

    container.at(cid)->stop();

    QSqlQuery query;
    query.prepare(QString("SELECT `path` FROM %1 WHERE `id` = :tid LIMIT 1").arg(library_identifier));
    query.bindValue(":tid", channel[0]);
    query.exec();

    if (query.first() && channel.at(4).toInt() == true) {
        container.at(cid)->loadFile(absoluteFilePath(query.value(0).toString()), false);
        container.at(cid)->play(start);
    }
}
