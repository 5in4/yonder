#include "mediamanager.h"

MediaManager::MediaManager(QProgressBar *progress_bar, QObject *parent) :
    QObject(parent)
{
    result = FMOD::System_Create(&system);
    if(result != FMOD_OK) {
        qDebug() << "FMOD error" << FMOD_ErrorString(result);
    }

    result = system->init(1000, FMOD_INIT_NORMAL, 0);
    if(result != FMOD_OK) {
        qDebug() << "FMOD error" << FMOD_ErrorString(result);
    }

    fmod_timer = new QTimer(this);
    connect(fmod_timer, SIGNAL(timeout()), this, SLOT(fmodLoop()));
    fmod_timer->start(10);

    this->progress_bar = progress_bar;

}


void MediaManager::fmodLoop() {
    QList<MediaContainer *>::const_iterator i;
    for(i=container.constBegin(); i!=container.constEnd(); ++i) {
        (*i)->checkFinished();

    }

    system->update();
}


MediaContainer* MediaManager::createContainer() {
    container.append(new MediaContainer(system));
    return container.last();
}





MediaContainer::MediaContainer(FMOD::System *system, QObject *parent) :
    QObject(parent)
{
    this->system = system;
    file_loaded = false;
    playing_file = "";
    playing_virtual = false;
    channel_nr = 0;
    oid = 0;
    volume = 0;

    delay_timer = new QTimer(this);
    delay_timer->setSingleShot(true);
    connect(delay_timer, SIGNAL(timeout()), this, SLOT(play()));
}


bool MediaContainer::loadFile(QString path, bool stream) {
    if(stream == false) {
        result = system->createSound(path.toAscii(), FMOD_DEFAULT, 0, &sound);
        if(result == FMOD_OK) {
            file_loaded = true;
            playing_file = path;
            return true;
        } else {
            file_loaded = false;
            return false;
        }
    } else {
        result = system->createStream(path.toAscii(), FMOD_DEFAULT, 0, &sound);
        if(result == FMOD_OK) {
            file_loaded = true;
            return true;
        } else {
            file_loaded = false;
            return false;
        }
    }
}


void MediaContainer::checkFinished() {
//  emit trackPosition(sound->);
//    sound->getLength();
//    sound->get
    if(!isPlaying() && playing_virtual == true) {
        playing_file.clear();
        emit finished(channel_nr);
        qDebug() << "emitting finished";
    }
}


bool MediaContainer::setVolume(float volume) {
    if(volume < 0 || volume > 1.0) {
        return false;
    } else {
        this->volume = volume;
        channel->setVolume(volume);
    }
}


QString MediaContainer::getCurrentFilename() {
    return playing_file;
}


bool MediaContainer::isPlaying() {
    bool playing;
    channel->isPlaying(&playing);
    return playing;
}


bool MediaContainer::isPlayingVirtual() {
    return playing_virtual;
}


bool MediaContainer::play() {
    if(file_loaded == true) {
        system->playSound(FMOD_CHANNEL_FREE, sound, true, &channel);
        channel->setPaused(false);
        emit starting(channel_nr);
        playing_virtual = true;
        return true;
    }
    return false;
}


bool MediaContainer::play(int msec_delay) {
    delay_timer->start(msec_delay);
}


void MediaContainer::pause() {
    playing_virtual = false;
    channel->setPaused(true);

}


void MediaContainer::stop() {
    playing_virtual = false;
    channel->stop();

}


int MediaContainer::getChannel() {
    return channel_nr;
}


void MediaContainer::setChannel(int channel_nr) {
    this->channel_nr = channel_nr;
}


int MediaContainer::getOID() {
    return oid;
}


void MediaContainer::setOID(int oid) {
    this->oid = oid;
}


MediaContainer::~MediaContainer() {
    delete sound;
}
