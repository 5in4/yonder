#ifndef AGTAGBYIDDELEGATE_H
#define AGTAGBYIDDELEGATE_H

//#include <QtCore>
//#include <QtSql>
//#include <QtGui>
#include <QTableView>
#include <QStyledItemDelegate>
#include <QApplication>
#include <QTextDocument>
#include <QTextOption>
#include <QSqlQuery>
#include <QAbstractTextDocumentLayout>
#include <QPainter>

class AGTagByIDDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit AGTagByIDDelegate(QString identifier, QTableView *parent = 0);

    QString identifier;
    QString formatting;
    QTableView *parent;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

};

#endif // AGTAGBYIDDELEGATE_H
