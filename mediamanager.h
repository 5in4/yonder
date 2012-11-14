#include <QObject>
#include <QList>
#include <QStringList>
#include <QTimer>
#include <QProgressBar>
#include <QFile>

#include <QDebug>

#ifdef Q_OS_WIN
#include <fmod.hpp>
#include <fmod_errors.h>
#endif

#ifdef Q_OS_LINUX
#include <fmodex/fmod.hpp>
#include <fmodex/fmod_errors.h>
#endif

#ifdef Q_OS_MAC
#include <fmod.hpp>
#include <fmod_errors.h>
#endif

#ifndef MEDIACONTAINER_H
#define MEDIACONTAINER_H

class MediaContainer : public QObject
{
    Q_OBJECT
public:
    explicit MediaContainer(FMOD::System *system, QObject *parent = 0);
    ~MediaContainer();

    FMOD_RESULT result;
    FMOD::System *system;
    FMOD::Channel *channel;
    FMOD::Sound *sound;

    QTimer *delay_timer;

    bool loadFile(QString path, bool stream=true);
    bool isPlaying();
    bool isPlayingVirtual();
    bool setVolume(float volume);
    QString getCurrentFilename();
    QStringList getTagList();

    int getChannel();
    void setChannel(int channel_nr);
    int getOID();
    void setOID(int oid);

private:
    bool file_loaded;
    QString playing_file;
    float volume;

    int channel_nr;
    int oid;
    bool playing_virtual;

signals:
    void finished(int channel_nr);
    void starting(int channel_nr);
    void trackPosition(int time, int length);

public slots:
    void pause();
    bool play();
    bool play(int msec_delay);
    void stop();
    void checkFinished();
};
#endif // MEDIACONTAINER_H




#ifndef MEDIAMANAGER_H
#define MEDIAMANAGER_H

class MediaManager : public QObject
{
    Q_OBJECT
public:
    explicit MediaManager(QProgressBar *progress_bar, QObject *parent = 0);

    FMOD_RESULT result;
    FMOD::System *system;

    QList<MediaContainer*> container;
    MediaContainer *createContainer();
    
    QTimer *fmod_timer;

    QProgressBar *progress_bar;

signals:
    
public slots:
    void fmodLoop();
};

#endif // MEDIAMANAGER_H



