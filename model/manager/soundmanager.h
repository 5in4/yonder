#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QtSql>
#include <qapplication.h>

#ifdef Q_OS_WIN
#include <taglib.h>
#include <tag.h>
#include <fileref.h>
#else
#include <taglib/taglib.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#endif

#include <model/manager/mediamanager.h>
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
    QStringList getTagList(QString file);
    QString absoluteFilePath(QString relative);
    QString relativeFilePath(QString absolute);

    //refactoring
    QList<int> getObjects(); // returns var objects
    QString getObjectName(int object_id);

    QList<int> objects;
    QStringList objects_names;

private:
    QStringList scanLibraryDirectory(QString dir);

    //refactoring


signals:
    void playbackError(int channel);
    void loadingStatus(int now, int max);

public slots:

};

#endif // SOUNDMANAGER_H
