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

#include <QDjangoQuerySet.h>


class SoundManager : public QObject {

public:
    explicit SoundManager(QObject *parent = 0);
    ~SoundManager();

    QSqlDatabase db;


    QStringList accepted_mimetypes;
    QStringList library_tracks;
    int channels;

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

signals:
    void playbackError(int channel);
    void loadingStatus(int now, int max);

public slots:

};

#endif // SOUNDMANAGER_H
