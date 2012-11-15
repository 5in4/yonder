#ifndef LIBRARYMODEL_H
#define LIBRARYMODEL_H

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDateTime>
#include <QStringList>

#include <QDebug>
#include <QSqlError>

class LibraryModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    LibraryModel(QString identifier, QObject *parent = 0);

    bool addTrack(QString path, QDateTime last_change, QString artist, QString album, QString title, bool transaction = false);
    QString filter();
    bool select();

    QString identifier;

public slots:
    void applyFilter(QString search = "");
    void setFilter(QString filter = "");

private:
    QString query;
    QString query_filter;
    QString query_final;
};

#endif // LIBRARYMODEL_H
