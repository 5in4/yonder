#ifndef SFXMANAGER_H
#define SFXMANAGER_H

#include <cstdlib>

#include <QSignalMapper>
#include <QDebug>

#include "model/track.h"
#include "model/tracktablemodel.h"

class SfxManager : QObject {
Q_OBJECT
public:
    SfxManager(QObject *parent);

    QList<QStringList> objects;
    QList<QStringList> objects_tracks;
    QList<bool> objects_status;
    QList<int> objects_range_start;
    QList<int> objects_range_stop;

    QList<bool> channels_status;

    QSignalMapper *mapper;

    virtual void createChannels();
    void createTables();

public slots:
    void createObjectsList();
    void playPause(int pos_in_array);
    void play(int pos_in_array);
    void pause(int pos_in_array);
    void playDelayed(int cid);
};

#endif // SFXMANAGER_H
