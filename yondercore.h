#ifndef YONDERCORE_H
#define YONDERCORE_H

#include <QDebug>

#include <QObject>
#include <QTimer>
#include <QSettings>
#include <QFile>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QDjango.h>

#include <sfxbasetype.h>
#include <model/track.h>
#include <model/sfxbit.h>
#include <model/sfxcontainer.h>

#include <musicmanager.h>
#include <sfxmanager.h>


class YonderCore : public QObject {
    Q_OBJECT
public:
    explicit YonderCore(QObject *parent = 0);
    ~YonderCore();

    MusicManager *music;
    SfxManager *sfx;
    static FMOD_SYSTEM * system;

    TrackTableModel<Track> *model_library;

private:
    void checkUpdate();

    QString project_path;

    QTimer *sound_loop_timeout;

    QNetworkAccessManager *update_manager;
    QNetworkReply *update_reply;

signals:
//    void projectSet();
    void managerLoading(QString manager);
    void projectLoading();
    void projectLoadingFailed();
    void projectLoaded();
    void projectRefreshing();
    void projectRefreshed();
    void projectStopping();
    void projectStopped();

    void webappStarted(QUrl url);
    void webappStopped();

    void updateAvailable();

public slots:
    void projectCreate(QString path);
    bool projectLoad(QString path);
    void projectStop();
    void projectRefresh();
    void webappStart();
    void webappStop();
    void checkUpdateProcess(QNetworkReply *rep);

    void soundbankAddFiles(QStringList paths, bool is_music);
    void soundbankAddStream(QUrl path);
    void soundLoop();
};

#endif // YONDERCORE_H
