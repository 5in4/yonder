#include "track.h"

Track::Track(QDjangoModel *parent) {

}

QByteArray Track::data() const {
    return m_data;
}


void Track::setData(const QByteArray &data) {
    m_data = data;
}


QString Track::artist() const {
    return m_artist;
}


void Track::setArtist(const QString &artist) {
    m_artist = artist;
}


QString Track::album() const {
    return m_album;
}


void Track::setAlbum(const QString &album) {
    m_album = album;
}


QString Track::title() const {
    return m_title;
}


void Track::setTitle(const QString &title) {
    m_title = title;
}


bool Track::isMusic() const {
    return m_is_music;
}


void Track::setIsMusic(const bool &is_music) {
    m_is_music = is_music;
}
