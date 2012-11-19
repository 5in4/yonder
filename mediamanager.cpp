#include "mediamanager.h"

MediaManager::MediaManager(QProgressBar *progress_bar, QObject *parent) :
    QObject(parent)
{

    result = FMOD_System_Create(&system);
    if(result != FMOD_OK) {
        qDebug() << "FMOD error" << FMOD_ErrorString(result);
    }

    result = FMOD_System_Init(system, 1000, FMOD_INIT_NORMAL, 0);
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

    FMOD_System_Update(system);
}


MediaContainer* MediaManager::createContainer() {
    container.append(new MediaContainer(system, this));
    return container.last();
}





MediaContainer::MediaContainer(FMOD_SYSTEM *system, QObject *parent) :
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
    // while releasing is broke, force streaming
    stream = true;
    const char * encoded_name = QFile::encodeName(path).constData();

    if(stream == false) {
        result = FMOD_System_CreateSound(system, encoded_name, FMOD_DEFAULT, 0, &sound);
        if(result == FMOD_OK) {
            file_loaded = true;
            playing_file = path;
            return true;
        } else {
            file_loaded = false;
            return false;
        }
    } else {
        result = FMOD_System_CreateStream(system, encoded_name, FMOD_DEFAULT, 0, &sound);
        if(result == FMOD_OK) {
            file_loaded = true;
            playing_file = path;
            return true;
        } else {
            file_loaded = false;
            return false;
        }
    }
}


void MediaContainer::checkFinished() {
    if(playing_virtual == true) {
        unsigned int len;
        unsigned int pos;
        FMOD_RESULT rs = FMOD_Sound_GetLength(sound, &len, FMOD_TIMEUNIT_MS);
        FMOD_RESULT rc = FMOD_Channel_GetPosition(channel, &pos, FMOD_TIMEUNIT_MS);
        if(rs == FMOD_OK && rc == FMOD_OK) {
            emit trackPosition(pos, len);
        }

        if(!isPlaying()) {
            playing_file.clear();
            file_loaded =false;
            FMOD_Sound_Release(sound);
            emit finished(channel_nr);
        }
    }
}


bool MediaContainer::setVolume(float volume) {
    if(volume < 0 || volume > 1.0) {
        return false;
    } else {
        this->volume = volume;
        FMOD_Channel_SetVolume(channel, volume);
    }
}


QString MediaContainer::getCurrentFilename() {
    return playing_file;
}


QStringList MediaContainer::getTagList() {
    QStringList tag_list;
    QString title;
    QString artist;
    QString album;
    if(file_loaded) {
        FMOD_TAG tag;
        int num_tags;
        int num_tags_updated;
        FMOD_Sound_GetNumTags(sound, &num_tags, &num_tags_updated);
        for(int i=0; i <= num_tags; i++) {
            FMOD_Sound_GetTag(sound, 0, i, &tag);
            if(QString((char *)tag.name) == "TITLE") {
                title = QString((char *)tag.data);
            } else if (QString((char *)tag.name) == "ARTIST") {
                artist = QString((char *)tag.data);
            } else if(QString((char *)tag.name) == "ALBUM") {
                album = QString((char *)tag.data);
            }
        }
    }
    tag_list.append(title.trimmed());
    tag_list.append(artist.trimmed());
    tag_list.append(album.trimmed());
    qDebug() << tag_list;
    return tag_list;
}


bool MediaContainer::isPlaying() {
    FMOD_BOOL playing;
    FMOD_Channel_IsPlaying(channel, &playing);
    return playing;
}


bool MediaContainer::isPlayingVirtual() {
    return playing_virtual;
}


bool MediaContainer::play() {
    if(file_loaded == true) {
        FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound, true, &channel);
        FMOD_Channel_SetVolume(channel, volume);
        FMOD_Channel_SetPaused(channel, false);
        emit starting(channel_nr);
        playing_virtual = true;
        return true;
    }
    return false;
}


bool MediaContainer::play(int msec_delay) {
    delay_timer->start(msec_delay);
    return true;
}


void MediaContainer::pause() {
    playing_virtual = false;
    FMOD_Channel_SetPaused(channel, true);
}


void MediaContainer::stop() {
    playing_virtual = false;
    FMOD_Channel_Stop(channel);

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

}
