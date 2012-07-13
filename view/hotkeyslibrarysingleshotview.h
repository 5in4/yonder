#ifndef HOTKEYSLIBRARYSINGLESHOTVIEW_H
#define HOTKEYSLIBRARYSINGLESHOTVIEW_H

#include <QtCore>
#include <QtGui>

class HotkeysLibrarySingleshotView : public QTableView
{
    Q_OBJECT
public:
    explicit HotkeysLibrarySingleshotView(QWidget *parent = 0);

signals:

public slots:

protected:
    void startDrag(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // HOTKEYSLIBRARYSINGLESHOTVIEW_H
