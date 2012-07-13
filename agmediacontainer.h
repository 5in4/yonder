#ifndef AGMEDIACONTAINER_H
#define AGMEDIACONTAINER_H

#include <QObject>
#include <QTimer>
#include <QDebug>

#include <vlc/vlc.h>

class AGMediaContainer : public QObject
{
Q_OBJECT
public:
    explicit AGMediaContainer(libvlc_instance_t *inst,  QObject *parent);
    ~AGMediaContainer();

    QTimer *timer;

    int getChannel();
    void setChannel(int channel);
    int getOID();
    void setOID(int oid);

    libvlc_instance_t *instance;
    libvlc_media_player_t *media_player;
    libvlc_media_t *media;
    libvlc_state_t virtual_state();
    libvlc_state_t state();

    QString playing_file;

    QTimer *playing_timer;
    QTimer *delay_timer;
    libvlc_state_t playing_state;


    void playDelayed(int msec);
    QString getCurrentFilename();

    void enqueue(QString absolute_filename);
    void setVolume(unsigned int volume);

private:
    int channel;
    int oid;
    int volume;

signals:
    // testing
    void finished(int channel);
    void starting(int channel);
    void trackPosition(int time, int length);

public slots:
    // testing
    void checkFinished();

    void pause();
    void play();
    void stop();
};

#endif // AGMEDIACONTAINER_H
