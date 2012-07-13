#ifndef AGHOTKEYACTIONDELEGATE_H
#define AGHOTKEYACTIONDELEGATE_H

#include <QStyledItemDelegate>
#include <QApplication>
#include <QPainter>
#include <QSqlQuery>

class AGHotkeyActionDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit AGHotkeyActionDelegate(QObject *parent = 0);
    
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    //QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    
};

#endif // AGHOTKEYACTIONDELEGATE_H
