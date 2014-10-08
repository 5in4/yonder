#ifndef SFXCONTAINERTABLEMODEL_H
#define SFXCONTAINERTABLEMODEL_H

#include "model/qdjangotablemodel.h"


template <class Type>
class SfxContainerTableModel : public QDjangoTableModel<Type> {

public:
    explicit SfxContainerTableModel(QObject *parent = 0);

signals:

public slots:

};


template <class Type>
SfxContainerTableModel<Type>::SfxContainerTableModel(QObject *parent) : QDjangoTableModel<Type>(parent) {
}


#endif // SFXCONTAINERTABLEMODEL_H
