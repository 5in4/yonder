#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include <QObject>
#include <QDebug>

#include "model/sfxcontainertablemodel.h"
#include "model/sfxcontainer.h"
#include "model/tracktablemodel.h"
#include "model/track.h"

class MusicManager : public QObject {
Q_OBJECT

public:
    MusicManager(QObject *parent);

    SfxContainerTableModel<SfxContainer> *model_playlists;
    TrackTableModel<Track> *model_library;

public slots:
    bool enqueue();
    void next();
    void playPause();
    void play();
    void pause();
};

#endif // MUSICMANAGER_H
