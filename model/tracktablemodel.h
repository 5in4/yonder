#ifndef TRACKTABLEMODEL_H
#define TRACKTABLEMODEL_H

#include "model/qdjangotablemodel.h"
#include "model/track.h"

template<class Type>
class TrackTableModel : public QDjangoTableModel<Type> {

public:
    explicit TrackTableModel(QObject *parent = 0);
    bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
    QDjangoWhere _filter;
};


template<class Type>
TrackTableModel<Type>::TrackTableModel(QObject *parent) {
    QDjangoTableModel<Type>::select();
}


template <class Type>
bool TrackTableModel<Type>::setData(const QModelIndex &index, const QVariant &value, int role) {
    qDebug() << "track set data";
    if (role == Qt::EditRole) {
        Type *t = new Type();
        if(t->insert(value.toString(), true)) {
            QDjangoTableModel<Type>::beginResetModel();
            t->save();
            QDjangoTableModel<Type>::endResetModel();
            QDjangoTableModel<Type>::select();
            return true;
        }
    }
    return false;
}
#endif // TRACKTABLEMODEL_H
