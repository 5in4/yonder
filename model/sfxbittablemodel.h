#ifndef SFXBITTABLEMODEL_H
#define SFXBITTABLEMODEL_H

#include "model/qdjangotablemodel.h"
#include "model/sfxbit.h"


template <class Type>
class SfxBitTableModel : public QDjangoTableModel<Type> {

public:
    explicit SfxBitTableModel(QObject *parent = 0);
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    SfxBit modelData(const QModelIndex &index, int role) const;
signals:

public slots:

};


template <class Type>
SfxBitTableModel<Type>::SfxBitTableModel(QObject *parent) : QDjangoTableModel<Type>(parent) {
}


//template <class Type>
//SfxBit SfxBitTableModel<Type>::modelData(const QModelIndex &index, int role) const {
//    if(role == Qt::DisplayRole) {
//        QString key = _m_ptr->values().at(index.row()).keys().at(index.column());
//        return _m_ptr->values().at(index.row());
//    }
//    return void;
//}


template <class Type>
bool SfxBitTableModel<Type>::setData(const QModelIndex &index, const QVariant &value, int role) {
    qDebug() << "SfxBit set data";
    if (role == Qt::EditRole) {
        Type *t = new Type();
        t->setContainer(value.toInt());
        QDjangoTableModel<Type>::beginResetModel();
        t->save();
        QDjangoTableModel<Type>::endResetModel();
        QDjangoTableModel<Type>::select();
        return true;
    }
    return false;
}

#endif // SFXBITTABLEMODEL_H
