#ifndef SFXBIT_H
#define SFXBIT_H

#include <QDjangoModel.h>

#include "model/sfxcontainer.h"
#include "sfxbasetype.h"


class SfxBit : public QDjangoModel {

    Q_OBJECT
    //Q_PROPERTY(SfxContainer* container READ container WRITE setContainer NOTIFY containerChanged)
    Q_PROPERTY(qint16 order READ order WRITE setOrder NOTIFY orderChanged)
    Q_PROPERTY(SfxBaseType* type READ type WRITE setType NOTIFY typeChanged)

public:
    explicit SfxBit(QDjangoModel *parent = 0);

//    SfxContainer *container() const;
//    void setContainer(SfxContainer *container);

    qint16 order() const;
    void setOrder(const qint16 order);

    SfxBaseType *type() const;
    void setType(SfxBaseType *type);

signals:
    emit void containerChanged();
    emit void orderChanged();
    emit void typeChanged();

private:
    qint16 m_order;
};

#endif // SFXBIT_H
