#include "objectsmodel.h"

ObjectsModel::ObjectsModel(QString identifier)
{
    setTable(identifier);
    setSort(1, Qt::AscendingOrder);
    select();
}


bool ObjectsModel::addObject(QString name) {
    insertRow(rowCount());
    setData(index(rowCount() -1, 1), name);
    submitAll();
    select();

    // This should be emitted by itself. It isn't.
    emit dataChanged(index(rowCount(), columnCount()), index(rowCount(), columnCount()));
}

// This should be emitted by itself. It isn't.
bool ObjectsModel::removeRow(int row, const QModelIndex &parent = QModelIndex()) {
    if(QSqlTableModel::removeRow(row, parent)) {
        emit dataChanged(index(rowCount(), columnCount()), index(rowCount(), columnCount()));
        return true;
    }
    return false;
}


bool ObjectsModel::removeRow(int row) {
    return removeRow(row, QModelIndex());
}
