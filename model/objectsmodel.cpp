#include "objectsmodel.h"

ObjectsModel::ObjectsModel(QString identifier)
{
    this->setTable(identifier);
    this->setSort(1, Qt::AscendingOrder);
    this->select();
}

bool ObjectsModel::addObject(QString name) {
    this->insertRow(this->rowCount());
    this->setData(this->index(this->rowCount() -1, 1), name);
    this->submitAll();
    this->select();
}
