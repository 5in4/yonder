#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include <model/manager/soundmanager.h>

class MusicManager : public SoundManager
{
Q_OBJECT
public:
    MusicManager(QObject *parent);

public slots:
    bool enqueue();
    void next();
    void playPause();
    void play();
    void pause();
};

#endif // MUSICMANAGER_H
