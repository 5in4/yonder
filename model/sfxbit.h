#ifndef SFXBIT_H
#define SFXBIT_H

#include <QDjangoModel.h>
#include <QDjangoQuerySet.h>

#include "model/sfxcontainer.h"
#include "model/track.h"


class SfxBit : public QDjangoModel {

    Q_OBJECT
    Q_PROPERTY(SfxContainer* container READ container WRITE setContainer NOTIFY containerChanged)
    Q_PROPERTY(int order READ order WRITE setOrder NOTIFY orderChanged)

public:
    explicit SfxBit(QDjangoModel *parent = 0);

    SfxContainer *container() const;
    void setContainer(SfxContainer *container);

    int order() const;
    void setOrder(const int &order);

    void tracks();

signals:
    emit void containerChanged();
    emit void orderChanged();

private:
    int _order = 0;
    QList<Track> _track_list;
};

#endif // SFXBIT_H
