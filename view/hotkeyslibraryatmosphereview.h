#ifndef HOTKEYSLIBRARYATMOSPHEREVIEW_H
#define HOTKEYSLIBRARYATMOSPHEREVIEW_H

#include <QtCore>
#include <QtGui>

class HotkeysLibraryAtmosphereView : public QTableView
{
    Q_OBJECT
public:
    explicit HotkeysLibraryAtmosphereView(QWidget *parent = 0);

signals:

public slots:

protected:
    void startDrag(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // HOTKEYSLIBRARYATMOSPHEREVIEW_H
