#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include <QtCore>

#include <vlc/vlc.h>

#include <soundmanager.h>

class MusicManager : public SoundManager
{
Q_OBJECT
public:
    MusicManager(QString project_path, QSqlDatabase db, QProgressBar *progress_bar, QObject *parent);

public slots:
    bool enqueue();
    void next();
    void playPause();
    void play();
    void pause();
};

#endif // MUSICMANAGER_H
