#ifndef TRACK_H
#define TRACK_H

#include <fmodex/fmod.h>
#include <fmodex/fmod_errors.h>

#include "QDjango.h"
#include "QDjangoModel.h"


class Track : public QDjangoModel {

    Q_OBJECT
    Q_PROPERTY(QByteArray data READ data WRITE setData NOTIFY dataChanged)
    Q_PROPERTY(QString artist READ artist WRITE setArtist NOTIFY artistChanged)
    Q_PROPERTY(QString album READ album WRITE setAlbum NOTIFY albumChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(bool isMusic READ isMusic WRITE setIsMusic NOTIFY isMusicChanged)

public:
    explicit Track(QDjangoModel *parent = 0);

    QByteArray data() const;
    void setData(const QByteArray &data);

    QString artist() const;
    void setArtist(const QString &artist);

    QString album() const;
    void setAlbum(const QString &album);

    QString title() const;
    void setTitle(const QString &title);

    bool isMusic() const;
    void setIsMusic(const bool &is_music);

    void init(FMOD_SYSTEM *system);
    void loadDataToSystem();
    void insert(QString path, bool is_music);

signals:
    emit void dataChanged();
    emit void artistChanged();
    emit void albumChanged();
    emit void titleChanged();
    emit void isMusicChanged();

private:
    FMOD_CREATESOUNDEXINFO info;
    FMOD_SOUND *sound;
    char *raw_data;


    QByteArray _data;
    QString _artist = tr("Unknown artist");
    QString _album = tr("Unknown album");
    QString _title = tr("Unknown title");
    bool _is_music = false;
};

#endif // TRACK_H
