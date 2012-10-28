#include "singleshotmanager.h"


/*!
 * Performs default startup
 */
SingleshotManager::SingleshotManager(QString project_path, QSqlDatabase db, MediaManager *media, QObject *parent) :
    SoundManager(project_path, QString("singleshot"), db, media, parent)
{
    this->path = QString("%1/%2").arg(this->project_path, "sfx");
    this->library_identifier = QString("sfx_library");
    createTables();
    library_model = new LibraryModel(library_identifier);
    objects_model = new ObjectsModel(objects_identifier);
    objects_tracks_model = new ObjectsTracksModel(objects_tracks_identifier, library_identifier);

    rescanLibrary();
    createObjectsList();
    connect(objects_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(createObjectsList()));
}

/*!
 * Creates list of all known objects
 */
void SingleshotManager::createObjectsList() {
    objects = QList<QStringList>();
    QSqlQuery query;
    query.prepare(QString("SELECT `id`, `name` FROM \"%1\"").arg(objects_identifier));
    query.exec();
    while(query.next()) {
        QStringList object;
        object.append(query.value(0).toString()); // DIRRRTY
        object.append(query.value(1).toString());
        objects.append(object);
        //qDebug() << identifier << "object add" << object;
    }
    createChannels();
}


/*!
 * Overwriting SoundManager::createChannels(int)
 */
void SingleshotManager::createChannels() {
    for(int i=0; i<this->objects.length(); i++) {
        container.append(media->createContainer());
        container.at(i)->setOID(objects.at(i)[0].toInt());
        container.at(i)->setChannel(i);
        channels++;
    }

}


/*!
 * Changes channel state to opposite
 */
void SingleshotManager::playPause(int channel) {
    if(container.at(channel)->isPlaying() == true) {
        pause(channel);
    } else {
        play(channel);
    }
}


/*!
 * Starts playing on channel
 */
void SingleshotManager::play(int channel) {
    if(enqueue(channel)) {
        container.at(channel)->play();
        qDebug() << identifier << "playing on channel" << channel;
    } else {
        qDebug() << identifier << "nothing to play on channel" << channel;
        emit playbackError(channel);
    }
}


/*!
 * Pauses on channel
 */
void SingleshotManager::pause(int channel) {
    container.at(channel)->pause();
    qDebug() << identifier << "pausing on channel" << channel;
}


/*!
 * Stops on channel and erases queue
 */
void SingleshotManager::stop(int channel) {
    container.at(channel)->stop();
}


/*!
 * Enqueues track on channel
 */
bool SingleshotManager::enqueue(int channel) {
    objects_tracks_model->selectObject(container.at(channel)->getOID());
    int tid = objects_tracks_model->selectRandomTrack();
    QString current_library_filter = library_model->filter();
    library_model->setFilter(QString("id=%1").arg(tid));
    QString choice_filename = library_model->data(library_model->index(0, 1)).toString();
    library_model->setFilter(current_library_filter);
    if(choice_filename.isEmpty()) {
        return false;
    }
    container.at(channel)->loadFile(absoluteFilePath(choice_filename));
    return true;
}
