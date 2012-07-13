#include <musicmanager.h>


/*!
 * Performs default startup
 */
MusicManager::MusicManager(QString project_path, QSqlDatabase db, QProgressBar *progress_bar, QObject *parent): SoundManager(project_path, QString("music"), db, progress_bar, parent)
{
    this->createTables();

    this->library_model = new LibraryModel(this->library_identifier);
    this->objects_model = new ObjectsModel(this->objects_identifier);
    this->objects_tracks_model = new ObjectsTracksModel(this->objects_tracks_identifier, this->library_identifier);

    this->rescanLibrary();
    this->createChannels(1);
    //connect(media_container.at(0), SIGNAL(aboutToFinish(int)), this, SLOT(enqueue()));
    connect(media_container.at(0), SIGNAL(finished(int)), this, SLOT(play()));
}


/*!
 * Switches play/pause state
 */
void MusicManager::playPause() {
    if(media_container.at(0)->state() == libvlc_Playing) {
        pause();
    } else {
        play();
    }
}


/*!
 * Plays on channel 0
 */
void MusicManager::play() {
    if(media_container.at(0)->state() == libvlc_Stopped || media_container.at(0)->state() == libvlc_Ended) {
        if(!enqueue()) {
            qDebug() << identifier << "nothing to play";
            emit playbackError(0);
            return;
        }
    }
    media_container.at(0)->play();
    qDebug() << this->identifier << "state" << "\"playing\"";
}


/*!
 * Pauses channel 0
 */
void MusicManager::pause() {
    media_container.at(0)->pause();
    qDebug() << this->identifier << "state" << "\"paused\"";
}


/*!
 * Resumes media_container if haltet, clears queue, enqueues and plays new track
 */
void MusicManager::next() {
    bool resume;
    if(media_container.at(0)->state() == libvlc_Playing) {
        resume = true;
    } else {
        resume = false;
    }

    media_container.at(0)->stop();
    enqueue();
    if(resume) {
        media_container.at(0)->play();
    }
}


/*!
 * Selects random track from objects_tracks_model and enqueues the full filepath to the media_container
 */
bool MusicManager::enqueue() {
    int tid = this->objects_tracks_model->selectRandomTrack();
    QString current_library_filter = this->library_model->filter();
    this->library_model->setFilter(QString("id=%1").arg(tid));
    QString choice_filename = this->library_model->data(this->library_model->index(0, 1)).toString();
    if(choice_filename.isEmpty()) {
        return false;
    }
    this->library_model->setFilter(current_library_filter);

    media_container.at(0)->enqueue(absoluteFilePath(choice_filename));
    return true;
}
