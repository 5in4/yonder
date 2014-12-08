#ifndef SFXCONTAINER_H
#define SFXCONTAINER_H

#include "QDjangoModel.h"

class SfxContainer : public QDjangoModel {

    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool isPlaylist READ isPlaylist WRITE setIsPlaylist NOTIFY isPlaylistChanged)

    Q_CLASSINFO("name", "primary_key=true")

public:
    explicit SfxContainer(QDjangoModel *parent = 0);

    QString name() const;
    void setName(const QString &name);

    bool isPlaylist() const;
    void setIsPlaylist(const bool &is_playlist);

signals:
    emit void nameChanged();
    emit void isPlaylistChanged();

private:
    QString _name;
    bool _is_playlist;
};

#endif // SFXCONTAINER_H
