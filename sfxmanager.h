#ifndef SFXMANAGER_H
#define SFXMANAGER_H

#include <cstdlib>

#include <QtCore>

#include <vlc/vlc.h>

#include <soundmanager.h>


class SfxManager: public SoundManager
{
Q_OBJECT
public:
    SfxManager(QString project_path, QSqlDatabase db, QProgressBar *progress_bar, QObject *parent);

    QList<QStringList> objects;
    QList<QStringList> objects_tracks;
    QList<int> objects_status;
    QList<int> objects_range_start;
    QList<int> objects_range_stop;

    QList<int> channels_status;

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
