#ifndef SINGLESHOTMANAGER_H
#define SINGLESHOTMANAGER_H

#include <model/manager/mediamanager.h>
#include <model/manager/soundmanager.h>

class SingleshotManager : public SoundManager
{
    Q_OBJECT
public:
    explicit SingleshotManager(QString project_path, QSqlDatabase db, MediaManager *media, QObject *parent = 0);

    void createChannels();

    QList<QStringList> objects;
signals:

public slots:
    void createObjectsList();
    void playPause(int channel);
    void play(int channel);
    void pause(int channel);
    void stop(int channel);
    bool enqueue(int channel);
};

#endif // SINGLESHOTMANAGER_H
