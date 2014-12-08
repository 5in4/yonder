#ifndef SfxBitTrackTRACKTABLEMODEL_H
#define SfxBitTrackTRACKTABLEMODEL_H

#include "model/qdjangotablemodel.h"
#include "model/sfxbittrack.h"


template <class Type>
class SfxBitTrackTableModel : public QDjangoTableModel<Type> {

public:
    explicit SfxBitTrackTableModel(SfxBit *sfx_bit, QObject *parent = 0);
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    SfxBitTrack modelData(const QModelIndex &index, int role) const;

signals:

public slots:

private:
    SfxBit *_sfx_bit;

};


template <class Type>
SfxBitTrackTableModel<Type>::SfxBitTrackTableModel(SfxBit *sfx_bit, QObject *parent) {
    this->_sfx_bit = sfx_bit;
    QDjangoTableModel<Type>::select();
    QDjangoTableModel<Type>::setFilter(QDjangoWhere("sfx_bit_id", QDjangoWhere::Equals, this->_sfx_bit->pk().toInt()));
}

template <class Type>
bool SfxBitTrackTableModel<Type>::setData(const QModelIndex &index, const QVariant &value, int role) {
    qDebug() << "SfxBitTrack set data";
    if (role == Qt::EditRole) {
        Type *t = new Type();
        t->setSfxBit(_sfx_bit);
        t->setTrack(value.toInt());
        QDjangoTableModel<Type>::beginResetModel();
        t->save();
        QDjangoTableModel<Type>::endResetModel();
        QDjangoTableModel<Type>::select();
        return true;
    }
    return false;
}

#endif // SfxBitTrackTRACKTABLEMODEL_H
