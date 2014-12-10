#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include <QObject>
#include <QDebug>

#include <QDjangoQuerySet.h>

#include "model/sfxcontainertablemodel.h"
#include "model/sfxcontainer.h"
#include "model/sfxbittracktablemodel.h"
#include "model/sfxbittrack.h"
#include "model/tracktablemodel.h"
#include "model/track.h"

class MusicManager : public QObject {
Q_OBJECT

public:
    MusicManager(QObject *parent);

    SfxContainerTableModel<SfxContainer> *model_playlists;
    TrackTableModel<Track> *model_playlist_active;
    TrackTableModel<Track> *model_library;

signals:
    void playlistLoaded();

public slots:
    void loadPlaylist(QModelIndex playlist_index);
    void loadPlaylist(QString playlist_id);
    void reloadPlaylist();

    bool enqueue();
    void next();
    void playPause();
    void play();
    void pause();

private:
    QString _playlist_active_id;
};

#endif // MUSICMANAGER_H
