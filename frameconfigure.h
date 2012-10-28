#ifndef FRAMECONFIGURE_H
#define FRAMECONFIGURE_H

#include <QSettings>

#include <frameobject.h>

namespace Ui {
class FrameConfigure;
}

class FrameConfigure : public FrameObject
{
    Q_OBJECT
    
public:
    explicit FrameConfigure(QWidget *parent = 0);
    ~FrameConfigure();

public slots:
    void saveSettings();

private:
    Ui::FrameConfigure *ui;
};

#endif // FRAMECONFIGURE_H
