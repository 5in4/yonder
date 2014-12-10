#include "track.h"

#include "yondercore.h"

Track::Track(QDjangoModel *parent) {

}


/**
 * \brief verify that loaded data really is audio and load in fmod
 */
bool Track::loadDataToSystem() {
    data_size = data().size();
    raw_data = new char[data_size];
    raw_data = _data.data();
    memset(&info, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    info.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    info.length = data_size;
    FMOD_RESULT result = FMOD_System_CreateStream(YonderCore::system, raw_data, FMOD_OPENMEMORY, &info, &sound);
    if(result == FMOD_OK) {
        return true;
    }
    return false;
}


bool Track::insert(QString path, bool is_music) {
    QFile f(path);
    f.open(QFile::ReadOnly);
    setData(f.readAll());
    setIsMusic(is_music);
    if(!loadDataToSystem()) {
        return false;
    }

    TagLib::FileRef file_tag(QFile::encodeName(path).constData());
    if(!file_tag.isNull()) {
            setArtist(TStringToQString(file_tag.tag()->artist()).trimmed());
            setAlbum(TStringToQString(file_tag.tag()->album()).trimmed());
            setTitle(TStringToQString(file_tag.tag()->title()).trimmed());
    }

    bool success = save();
    QDjango::database().commit();
    if(success) {
        return true;
    }
    return false;
}

QByteArray Track::data() const {
    return _data;
}


void Track::setData(const QByteArray &data) {
    _data = data;
}


QString Track::artist() const {
    return _artist;
}


void Track::setArtist(const QString &artist) {
    _artist = artist;
}


QString Track::album() const {
    return _album;
}


void Track::setAlbum(const QString &album) {
    _album = album;
}


QString Track::title() const {
    return _title;
}


void Track::setTitle(const QString &title) {
    _title = title;
}


bool Track::isMusic() const {
    return _is_music;
}


void Track::setIsMusic(const bool &is_music) {
    _is_music = is_music;
}


bool Track::isSfx() const {
    return _is_sfx;
}


void Track::setIsSfx(const bool &is_sfx) {
    _is_sfx = is_sfx;
}
