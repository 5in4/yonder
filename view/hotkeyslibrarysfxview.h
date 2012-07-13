#ifndef HOTKEYSLIBRARYSFXVIEW_H
#define HOTKEYSLIBRARYSFXVIEW_H

#include <QtCore>
#include <QtGui>

class HotkeysLibrarySfxView : public QTableView
{
    Q_OBJECT
public:
    explicit HotkeysLibrarySfxView(QWidget *parent = 0);

signals:

public slots:

protected:
    void startDrag(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // HOTKEYSLIBRARYSFXVIEW_H
