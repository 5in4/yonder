#ifndef SINGLESHOTOBJECTSTRACKSVIEW_H
#define SINGLESHOTOBJECTSTRACKSVIEW_H

#include <QTableView>
#include <QMimeData>
#include <QDrag>
#include <QDragEnterEvent>
#include <QMouseEvent>

#include <model/objectstracksmodel.h>

class SingleshotObjectsTracksView : public QTableView
{
    Q_OBJECT
public:
    explicit SingleshotObjectsTracksView(QWidget *parent = 0);
    QModelIndexList getSelectedIndexes();

    QString library_mime_type;
    ObjectsTracksModel *custom_model;

protected:
    void startDrag(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

signals:

public slots:

};

#endif // SINGLESHOTOBJECTSTRACKSVIEW_H
