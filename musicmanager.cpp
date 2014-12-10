#include "musicmanager.h"

/*!
 * Performs default startup
 */
MusicManager::MusicManager(QObject *parent) {
    model_playlists = new SfxContainerTableModel<SfxContainer>(this);
    model_playlists->setFilter(QDjangoWhere("isPlaylist", QDjangoWhere::Equals, true));

    model_library = new TrackTableModel<Track>(this);
    model_library->setFilter(QDjangoWhere("isMusic", QDjangoWhere::Equals, true));
    //    this->createTables();

//    this->library_model = new LibraryModel(this->library_identifier);
//    this->objects_model = new ObjectsModel(this->objects_identifier);
//    this->objects_tracks_model = new ObjectsTracksModel(this->objects_tracks_identifier, this->library_identifier);

//    this->rescanLibrary();
//    this->createChannels(1);
//    connect(container.at(0), SIGNAL(finished(int)), this, SLOT(play()));
    qDebug() << "MusicManager summoned";
}


void MusicManager::reloadPlaylist() {
    if(!_playlist_active_id.isNull()) {
        loadPlaylist(_playlist_active_id);
    }
}


void MusicManager::loadPlaylist(QModelIndex playlist_index) {
    loadPlaylist(playlist_index.data().toString());
}


void MusicManager::loadPlaylist(QString playlist_id) {
    _playlist_active_id = playlist_id;

    QDjangoQuerySet<SfxBit> sfx_bit_query;
    SfxBit *sfx_bit = sfx_bit_query.get(QDjangoWhere("container_id", QDjangoWhere::Equals, playlist_id));
    QDjangoQuerySet<SfxBitTrack> sfx_bit_tracks;
    sfx_bit_tracks = sfx_bit_tracks.filter(QDjangoWhere("sfx_bit_id", QDjangoWhere::Equals, sfx_bit->pk()));
    QStringList track_ids;
    for(int i=0;i<sfx_bit_tracks.size();++i) {
        track_ids.append(sfx_bit_tracks.at(i)->track()->pk().toString());
    }

    model_playlist_active = new TrackTableModel<Track>(this);
    model_playlist_active->setFilter(QDjangoWhere("id", QDjangoWhere::IsIn, track_ids));
    emit playlistLoaded();
}


/*!
 * Switches play/pause state
 */
void MusicManager::playPause() {
//    if(container.at(0)->isPlaying() == true) {
//        pause();
//    } else {
//        play();
//    }
}


/*!
 * Plays on channel 0
 */
void MusicManager::play() {
//    if(container.at(0)->isPlaying() == false) {
//        if(!enqueue()) {
//            qDebug() << identifier << "nothing to play";
//            emit playbackError(0);
//            return;
//        }
//    }
//    container.at(0)->play();
//    qDebug() << this->identifier << "state" << "\"playing\"";
}


/*!
 * Pauses channel 0
 */
void MusicManager::pause() {
//    container.at(0)->pause();
//    qDebug() << this->identifier << "state" << "\"paused\"";
}


/*!
 * Resumes media_container if haltet, clears queue, enqueues and plays new track
 */
void MusicManager::next() {
//    bool resume;
//    if(container.at(0)->isPlaying() == true) {
//        resume = true;
//    } else {
//        resume = false;
//    }

//    container.at(0)->stop();
//    enqueue();
//    if(resume) {
//        container.at(0)->play();
//    }
}


/*!
 * Selects random track from objects_tracks_model and enqueues the full filepath to the media_container
 */
bool MusicManager::enqueue() {
//    int tid = this->objects_tracks_model->selectRandomTrack();
//    QString current_library_filter = this->library_model->filter();
//    this->library_model->setFilter(QString("id=%1").arg(tid));
//    QString choice_filename = this->library_model->data(this->library_model->index(0, 1)).toString();
//    if(choice_filename.isEmpty()) {
//        return false;
//    }
//    //this->library_model->setFilter(current_library_filter);
//    //container.at(0)->loadFile(absoluteFilePath(choice_filename));
//    return true;
}
