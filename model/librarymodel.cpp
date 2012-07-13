#include "librarymodel.h"

LibraryModel::LibraryModel(QString identifier, QObject *parent): QSqlQueryModel(parent)
{
    this->identifier = identifier;
    query = QString("SELECT id, path, `last-change`, length, artist, album, title FROM %1").arg(this->identifier);
    applyFilter();
    select();
}

bool LibraryModel::addTrack(QString path, QDateTime last_change, QString artist, QString album, QString title, bool transaction) {
    QSqlQuery query_insert;
    query_insert.prepare(QString("INSERT INTO %1 (\"path\", \"last-change\", \"length\", \"artist\", \"album\", \"title\") VALUES (:path, :lastchange, :length, :artist, :album, :title)").arg(identifier));
    query_insert.bindValue(":path", path);
    query_insert.bindValue(":lastchange", last_change.toTime_t());
    query_insert.bindValue(":length", 0);
    query_insert.bindValue(":artist", artist);
    query_insert.bindValue(":album", album);
    query_insert.bindValue(":title", title);
    query_insert.exec();
    return true;
}


QString LibraryModel::filter() {
    return query_filter;
}

/*!
 * Implements search filter with LIKE
 */
void LibraryModel::applyFilter(QString search) {
    query_filter.clear();
    QStringList search_substrings = search.split(" ");
    foreach(QString sub, search_substrings) {
        if(sub.isEmpty()) {
            continue;
        }
        QString raw_filter = QString("(album LIKE \"%%1%\" OR artist LIKE \"%%1%\" OR title LIKE \"%%1%\" OR path LIKE \"%%1%\")").arg(sub);
        query_filter.append(raw_filter);
        query_filter.append(" AND ");
    }
    query_filter.remove(-5, 5);

    if(!query_filter.isEmpty()) {
        query_final = QString("%1 WHERE %2").arg(query, query_filter);
    } else {
        query_final = query;
    }
    select();
}


/*!
 * Sets explicit filter
 */
void LibraryModel::setFilter(QString filter) {
    if(filter.length() > 0) {
        query_final = QString("%1 WHERE %2").arg(query, filter);
    } else {
        query_final = query;
    }
    select();
}

bool LibraryModel::select() {
    setQuery(query_final);
    if(this->lastError().isValid()) {
        qDebug() << this->lastError().text();
        return false;
    }
    return true;
}
