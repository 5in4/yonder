#include "track.h"

#include "yondercore.h"

Track::Track(QDjangoModel *parent) {

}


void Track::loadDataToSystem() {
    int inbs = data().size();
    raw_data = new char[inbs];
    raw_data = data().data();

    memset(&info, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    info.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    info.length = inbs;

    Q_ASSERT(FMOD_OK == FMOD_System_CreateStream(YonderCore::system, raw_data, FMOD_OPENMEMORY, &info, &sound));
}


void Track::insert(QString path, bool is_music) {
    QFile f(path);
    f.open(QFile::ReadOnly);
    setData(f.readAll());
    setIsMusic(is_music);
    c->load(this, true);
    QMap<QString, QString> tags = c->getTags();
    setArtist(tags["ARTIST"]);
    setAlbum(tags["ALBUM"]);
    setTitle(tags["TITLE"]);
    save();
    QDjango::database().commit();
    QSqlQuery query_vacuum("VACUUM", QDjango::database());
    query_vacuum.exec();
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
