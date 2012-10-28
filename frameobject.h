#ifndef FRAMEOBJECT_H
#define FRAMEOBJECT_H

#include <QFrame>
#include <QDebug>

class FrameObject : public QFrame
{
    Q_OBJECT
public:
    explicit FrameObject(QWidget *parent = 0);
    
signals:
    void accept();
    void reject();

    void activated();
    void deactivated();
public slots:
    void setVisible(bool visible);

    void refresh();
};

#endif // FRAMEOBJECT_H
