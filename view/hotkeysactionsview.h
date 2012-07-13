#ifndef HOTKEYSACTIONSVIEW_H
#define HOTKEYSACTIONSVIEW_H

#include <QtCore>
#include <QtGui>

class HotkeysActionsView : public QTableView
{
    Q_OBJECT
public:
    explicit HotkeysActionsView(QWidget *parent = 0);

    QModelIndexList getSelectedIndexes();
    int oid;
    void setOID(int oid);

signals:

public slots:

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);


};

#endif // HOTKEYSACTIONSVIEW_H
