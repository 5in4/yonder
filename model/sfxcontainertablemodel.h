#ifndef SFXCONTAINERTABLEMODEL_H
#define SFXCONTAINERTABLEMODEL_H

#include "model/qdjangotablemodel.h"
#include "model/sfxcontainer.h"


template <class Type>
class SfxContainerTableModel : public QDjangoTableModel<Type> {

public:
    explicit SfxContainerTableModel(QObject *parent = 0);
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    SfxContainer modelData(const QModelIndex &index, int role) const;
signals:

public slots:

};


template <class Type>
SfxContainerTableModel<Type>::SfxContainerTableModel(QObject *parent) {
    QDjangoTableModel<Type>::select();
}


template <class Type>
bool SfxContainerTableModel<Type>::setData(const QModelIndex &index, const QVariant &value, int role) {
    qDebug() << "sfxcontainer set data";
    if (role == Qt::EditRole) {
        Type *t = new Type();
        t->setName(value.toString());
        t->setIsPlaylist(true);
        QDjangoTableModel<Type>::beginResetModel();
        t->save();
        QDjangoTableModel<Type>::endResetModel();
        QDjangoTableModel<Type>::select();
        return true;
    }
    return false;
}

#endif // SFXCONTAINERTABLEMODEL_H
