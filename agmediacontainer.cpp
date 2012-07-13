#include <agmediacontainer.h>

AGMediaContainer::AGMediaContainer(libvlc_instance_t *inst, QObject *parent) :
    QObject(parent)
{
    instance = inst;
    media_player = libvlc_media_player_new(instance);

    playing_timer = new QTimer();
    playing_timer->setInterval(100);
    playing_state = libvlc_Stopped;
    connect(playing_timer, SIGNAL(timeout()), this, SLOT(checkFinished()));
    oid = 0;
    channel = 0;
    volume = 0;
}

/*!
 * Checks if playback has finished, stops all timers and emits trackPosition() and finished()
 */
void AGMediaContainer::checkFinished() {
    emit trackPosition(libvlc_media_player_get_time(media_player), libvlc_media_player_get_length(media_player));
    if(libvlc_media_player_get_state(media_player) == libvlc_Ended) {
        playing_timer->stop();
        playing_file.clear();
        emit finished(channel);
        qDebug() << "emitting finished";
    }
}


/*!
 * Return channel
 */
int AGMediaContainer::getChannel() {
    return channel;
}


/*!
 * Store external channel in object
 */
void AGMediaContainer::setChannel(int channel) {
    this->channel = channel;
}


/*!
 * Return oid
 */
int AGMediaContainer::getOID() {
    return oid;
}


/*!
 * Store external oid in object
 */
void AGMediaContainer::setOID(int oid) {
    this->oid = oid;
}


/*!
 * Waits for msec, then starts playing.
 */
void AGMediaContainer::playDelayed(int msec) {
    delay_timer = new QTimer(this);
    delay_timer->setSingleShot(true);
    connect(delay_timer, SIGNAL(timeout()), this, SLOT(play()));
    delay_timer->start(msec);
}


/*!
 * return absolute filename of last enqueued file
 */
QString AGMediaContainer::getCurrentFilename() {
    return playing_file;
}


/*!
 * loads argument into media instance
 */
void AGMediaContainer::enqueue(QString absolute_filename) {
    playing_file = absolute_filename;
    qDebug() << playing_file;
    //if(!media) {
    //        delete media;
    //}
    #ifdef _WIN32
    absolute_filename.replace(":/", ":\\\\");
    qDebug() << absolute_filename;
    #endif
    media = libvlc_media_new_path(instance, absolute_filename.toLocal8Bit());
}


/*!
 * immediately plays back enqueued file, starts timers and sets virtual state to playing
 */
void AGMediaContainer::play() {
    libvlc_media_player_get_media(media_player);
    playing_timer->start();
    playing_state = libvlc_Playing;
    libvlc_media_player_set_media(media_player, media);
    libvlc_audio_set_volume(media_player, volume);
    libvlc_media_player_play(media_player);
    emit starting(channel);
    qDebug() << "emitting started";
}


/*!
 * pauses playback, stop timers and sets virtual state to paused
 */
void AGMediaContainer::pause() {
    playing_timer->stop();
    libvlc_media_player_pause(media_player);
    playing_state = libvlc_Paused;
}


/*!
 * value between 0 and 100
 */
void AGMediaContainer::setVolume(unsigned int volume) {
    if(volume > 100) {
        return;
    }
    this->volume = volume;
    libvlc_audio_set_volume(media_player, volume);
}



/*!
 * returns real audio state
 */
libvlc_state_t AGMediaContainer::state() {
    if(libvlc_media_player_get_state(media_player) == libvlc_NothingSpecial) {
        return libvlc_Stopped;
    }
    return libvlc_media_player_get_state(media_player);
}


/*!
 * returns virtual audio state (state set by ui)
 */
libvlc_state_t AGMediaContainer::virtual_state() {
    return playing_state;
}


/*!
 * stops playback, stops timers, sets virtual state to stopped
 */
void AGMediaContainer::stop() {
    if(!playing_timer) {
        playing_timer->stop();
    }
//    if(!delay_timer) {
//        delay_timer->stop();
//    }
    libvlc_media_player_stop(media_player);
    playing_state = libvlc_Stopped;
}

AGMediaContainer::~AGMediaContainer() {
    //libvlc_media_release(media);
    libvlc_media_player_stop(media_player);
    //delete media;
    libvlc_media_player_release(media_player);
    //delete media_player;
    //delete instance;

    if(!delay_timer) {
        delete delay_timer;
    }
    playing_timer->stop();
    delete playing_timer;
}
