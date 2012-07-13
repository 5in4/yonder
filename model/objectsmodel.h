#ifndef OBJECTSMODEL_H
#define OBJECTSMODEL_H

#include <QSqlTableModel>

class ObjectsModel : public QSqlTableModel
{
public:
    ObjectsModel(QString identifier);

    bool addObject(QString name);
};

#endif // OBJECTSMODEL_H
