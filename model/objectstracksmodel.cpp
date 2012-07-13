#include "objectstracksmodel.h"

ObjectsTracksModel::ObjectsTracksModel(QString identifier, QString library_identifier): QSqlTableModel()
{
    this->identifier = identifier;
    this->library_identifier = library_identifier;
    setTable(this->identifier);
    setSort(0, Qt::AscendingOrder);
    selectObject(0);
    select();
}

void ObjectsTracksModel::selectObject(int oid) {
    this->oid = oid;
    setFilter(QString("oid=\"%1\"").arg(this->oid));
    select();
}

int ObjectsTracksModel::selectRandomTrack() {
    int seed = QTime::currentTime().msec();
    qsrand(seed);
    #ifdef _WIN32
    qrand();
    #endif
    int row = int( qrand() / (RAND_MAX + 1.0) * rowCount());
    return this->data(index(row, 1)).toInt();
}

bool ObjectsTracksModel::insertTrackIntoObject(int track) {
    if(oid > 0) {
        insertRow(rowCount());
        setData(index(rowCount() - 1, 0), oid);
        setData(index(rowCount() - 1, 1), track);
        submitAll();
        select();
        return true;
    }
    return false;
}

bool ObjectsTracksModel::removeTrackFromObject(QModelIndex model_index, bool isSfx) {
    QModelIndex field1 = createIndex(model_index.row(), 0);
    QModelIndex field2 = createIndex(model_index.row(), 1);
    if(isSfx == true) {
        field1 = createIndex(model_index.row(), 1);
        field2 = createIndex(model_index.row(), 2);
    }
    QSqlQuery query = QSqlQuery(database());
    query.prepare(QString("DELETE FROM %1 WHERE oid = :oid AND tid = :tid").arg(identifier));
    query.bindValue(":oid", field1.data());
    query.bindValue(":tid", field2.data());
    qDebug() << identifier << field1.data().toString() << field2.data().toString();
    query.exec();
}

Qt::ItemFlags ObjectsTracksModel::flags(const QModelIndex &index) const
{
    if(index.column() != 2) {
        Qt::ItemFlags flags = QAbstractTableModel::flags(index);
        flags |= Qt::ItemIsEditable;
        return flags;
    }
    return QAbstractTableModel::flags(index);
}

