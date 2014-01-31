#ifndef YONDERCORE_H
#define YONDERCORE_H

#include <QDebug>

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <model/manager/mediamanager.h>
#include <model/manager/atmospheremanager.h>
#include <model/manager/hotkeysmanager.h>
#include <model/manager/musicmanager.h>
#include <model/manager/sfxmanager.h>
#include <model/manager/singleshotmanager.h>

#include <webapp.h>

class YonderCore : public QObject
{
    Q_OBJECT
public:
    explicit YonderCore(QObject *parent = 0);

    QPointer<MediaManager> media;
    QPointer<AtmosphereManager> atmosphere;
    QPointer<MusicManager> music;
    QPointer<SfxManager> sfx;
    QPointer<SingleshotManager> singleshot;
    QPointer<HotkeysManager> hotkeys;

private:
    void checkUpdate();

    QString project_path;

    QPointer<Webapp> webapp;

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
    bool projectLoad(QString path);
    void projectStop();
    void projectRefresh();
    void webappStart();
    void webappStop();
    void checkUpdateProcess(QNetworkReply *rep);

};

#endif // YONDERCORE_H
