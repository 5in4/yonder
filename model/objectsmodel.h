#ifndef OBJECTSMODEL_H
#define OBJECTSMODEL_H

#include <QSqlTableModel>

class ObjectsModel : public QSqlTableModel
{
public:
    ObjectsModel(QString identifier);

    bool removeRow(int row);
    bool removeRow(int row, const QModelIndex &parent);

    bool addObject(QString name);
};

#endif // OBJECTSMODEL_H
