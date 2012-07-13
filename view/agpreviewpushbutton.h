#ifndef AGPREVIEWPUSHBUTTON_H
#define AGPREVIEWPUSHBUTTON_H

#include <QPushButton>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QByteArray>
#include <QMimeData>

#include <QDebug>

class AGPreviewPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit AGPreviewPushButton(QWidget *parent = 0);

    QStringList library_mime_type;
signals:
    void fileDropped(QString mime, int tid);

public slots:


protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

};

#endif // AGPREVIEWPUSHBUTTON_H
