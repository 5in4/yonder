#ifndef FRAMESTART_H
#define FRAMESTART_H

#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#include <frameobject.h>

namespace Ui {
class FrameStart;
}

class FrameStart : public FrameObject
{
    Q_OBJECT
    
public:
    explicit FrameStart(QWidget *parent = 0);
    ~FrameStart();

signals:
    void acceptedProjectFolder(QString project_folder);

private:
    Ui::FrameStart *ui;

private slots:
    void openFileBrowser();
    bool testFile(QString path);
    void accept();
};

#endif // FRAMESTART_H
