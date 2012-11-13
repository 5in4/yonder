#ifndef FRAMEHELP_H
#define FRAMEHELP_H

#include <QMessageBox>
#include <QDebug>

#include <frameobject.h>

namespace Ui {
class FrameHelp;
}

class FrameHelp : public FrameObject
{
    Q_OBJECT
    
public:
    explicit FrameHelp(QWidget *parent = 0);
    ~FrameHelp();
    
private:
    Ui::FrameHelp *ui;

private slots:
    void showAbout();
};

#endif // FRAMEHELP_H
