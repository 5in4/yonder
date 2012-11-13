#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QtCore>
#include <QtGui/QProgressBar>
#include <QtSql>
#include <qapplication.h>

#include <agmediacontainer.h>
#include <mediamanager.h>
#include <model/librarymodel.h>
#include <model/objectsmodel.h>
#include <model/objectstracksmodel.h>

class SoundManager : public QObject
{
Q_OBJECT
public:
    explicit SoundManager(QString project_path, QString identifier, QSqlDatabase db, MediaManager *media, QObject *parent = 0);
    ~SoundManager();

    QSqlDatabase db;

    QString project_path;
    QString identifier;

    QString library_identifier;
    QString objects_identifier;
    QString objects_tracks_identifier;
    QString path;

    QStringList accepted_mimetypes;
    QString objects;
    QString objects_tracks;
    QStringList library_tracks;
    int channels;
    MediaManager *media;
    QList<MediaContainer *> container;

    LibraryModel *library_model;
    ObjectsModel *objects_model;
    ObjectsTracksModel *objects_tracks_model;

    virtual void createTables();
    bool rescanLibrary();
    bool createChannels(int channels);
    bool setVolume(unsigned int volume);
    bool updateDatabase(QString identifier, QString sql);
    QString getTag(QString file, int field);
    QString absoluteFilePath(QString relative);
    QString relativeFilePath(QString absolute);

    // status information
    QProgressBar *progress_bar;

private:
    QStringList scanLibraryDirectory(QString dir);

signals:
    void playbackError(int channel);
    void loadingStatus(int now, int max);

public slots:

};

#endif // SOUNDMANAGER_H
