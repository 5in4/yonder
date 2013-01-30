#ifndef HOTKEYSLIBRARYSPECIALVIEW_H
#define HOTKEYSLIBRARYSPECIALVIEW_H

#include <QTableView>
#include <QMimeData>
#include <QDrag>

class HotkeysLibrarySpecialView : public QTableView
{
    Q_OBJECT
public:
    explicit HotkeysLibrarySpecialView(QWidget *parent = 0);

signals:

public slots:

protected:
    void startDrag(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

};

#endif // HOTKEYSLIBRARYSPECIALVIEW_H
