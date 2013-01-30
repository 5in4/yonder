#ifndef SINGLESHOTLIBRARYVIEW_H
#define SINGLESHOTLIBRARYVIEW_H

#include <QTableView>
#include <QMimeData>
#include <QDrag>

class SingleshotLibraryView : public QTableView
{
    Q_OBJECT
public:
    explicit SingleshotLibraryView(QWidget *parent = 0);

protected:
    void startDrag(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:

public slots:

};

#endif // SINGLESHOTLIBRARYVIEW_H
