#ifndef ATMOSPHERELIBRARYVIEW_H
#define ATMOSPHERELIBRARYVIEW_H

#include <QTableView>
#include <QMimeData>
#include <QDrag>

class AtmosphereLibraryView : public QTableView
{
    Q_OBJECT
public:
    explicit AtmosphereLibraryView(QWidget *parent);

protected:
    void startDrag(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // ATMOSPHERELIBRARYVIEW_H
