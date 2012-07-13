#ifndef AGRESOURCEDELEGATE_H
#define AGRESOURCEDELEGATE_H

#include <QSqlQuery>
#include <QTableView>
#include <QStyledItemDelegate>
#include <QTextDocument>
#include <QDesktopServices>
#include <QApplication>
#include <QTextOption>
#include <QAbstractTextDocumentLayout>
#include <QPainter>


class AGResourceDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit AGResourceDelegate(QTableView *parent = 0);

    QString formatting;
    QTableView *parent;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

};

#endif // AGRESOURCEDELEGATE_H
