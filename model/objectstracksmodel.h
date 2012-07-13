#ifndef OBJECTSTRACKSMODEL_H
#define OBJECTSTRACKSMODEL_H

#include <iostream>
#include <cstdlib>
#include <QtCore>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>

class ObjectsTracksModel : public QSqlTableModel
{
public:
    ObjectsTracksModel(QString identifier, QString library_identifier);

    Qt::ItemFlags flags ( const QModelIndex & index ) const;

    void selectObject(int oid);
    int selectRandomTrack();
    bool insertTrackIntoObject(int track);
    bool removeTrackFromObject(QModelIndex model_index, bool isSfx=false);

    QString identifier;
    QString library_identifier;
    int oid;
};

#endif // OBJECTSTRACKSMODEL_H
