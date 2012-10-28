#include "atmospheremanager.h"


/*!
 * Performs default startup
 */
AtmosphereManager::AtmosphereManager(QString project_path, QSqlDatabase db, MediaManager *media, QObject *parent): SoundManager(project_path, QString("atmosphere"), db, media, parent)
{
    this->createTables();

    this->library_model = new LibraryModel(this->library_identifier);
    this->objects_model = new ObjectsModel(this->objects_identifier);
    this->objects_tracks_model = new ObjectsTracksModel(this->objects_tracks_identifier, this->library_identifier);

    this->rescanLibrary();
    this->createObjectsList();
    connect(this->objects_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(createObjectsList()));
}


/*!
 * creates list of all known objects
 */
void AtmosphereManager::createObjectsList() {
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
void AtmosphereManager::createChannels() {
    for(int i=0; i<this->objects.length(); i++) {
//        media_container.append(new AGMediaContainer(inst ,this));
//        media_container.at(i)->setOID(objects.at(i)[0].toInt());
//        media_container.at(i)->setChannel(i);

        container.append(media->createContainer());
        container.at(i)->setOID(objects.at(i)[0].toInt());
        container.at(i)->setChannel(i);
        channels++;
        //enqueue(media_container.at(i)->getChannel());
        //connect(media_container.at(i), SIGNAL(aboutToFinish(int)), this, SLOT(enqueue(int)));
        //connect(media_container.at(i), SIGNAL(finished(int)), this, SLOT(play(int)));
        connect(container.at(i), SIGNAL(finished(int)), this, SLOT(play(int)));
    }

}


/*!
 * Changes channel state to opposite
 */
void AtmosphereManager::playPause(int channel) {
    if(container.at(channel)->isPlaying() == true) {
//    if(media_container.at(channel)->state() == libvlc_Playing) {
        pause(channel);
    } else {
        play(channel);
    }
}


/*!
 * Starts playing on channel
 */
void AtmosphereManager::play(int channel) {
    if(container.at(channel)->getCurrentFilename().isEmpty()) {
//    if(media_container.at(channel)->getCurrentFilename().isEmpty()) {
        if(!enqueue(channel)) {
            qDebug() << identifier << "nothing to play on channel" << channel;
            emit playbackError(channel);
            return;
        }
    }
    container.at(channel)->play();
    //media_container.at(channel)->play();
    qDebug() << identifier << "playing on channel" << channel;
}


/*!
 * Pauses on channel
 */
void AtmosphereManager::pause(int channel) {
    container.at(channel)->pause();
    //    media_container.at(channel)->pause();
    qDebug() << identifier << "pausing on channel" << channel;
}


/*!
 * Enqueues track on channel
 */
bool AtmosphereManager::enqueue(int channel) {
    objects_tracks_model->selectObject(container.at(channel)->getOID());
//    objects_tracks_model->selectObject(media_container.at(channel)->getOID());
    int tid = objects_tracks_model->selectRandomTrack();
    QString current_library_filter = library_model->filter();
    library_model->setFilter(QString("id=%1").arg(tid));
    QString choice_filename = library_model->data(library_model->index(0, 1)).toString();
    library_model->setFilter(current_library_filter);
    if(choice_filename.isEmpty()) {
        return false;
    }
    container.at(channel)->loadFile(absoluteFilePath(choice_filename));
//    media_container.at(channel)->enqueue(absoluteFilePath(choice_filename));
//    if(media_container.at(channel)->virtual_state() == libvlc_Playing) {
//        media_container.at(channel)->play();
//    }
    return true;
}
