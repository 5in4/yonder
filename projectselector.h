#ifndef PROJECTSELECTOR_H
#define PROJECTSELECTOR_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class ProjectSelector;
}

class ProjectSelector : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectSelector(QString project_path, QWidget *parent = 0);
    ~ProjectSelector();

    QString getProjectPath();

private:
    Ui::ProjectSelector *ui;

private slots:
    void openFileBrowser();
    void testFile(QString path);
    void reject();
    void accept();
};

#endif // PROJECTSELECTOR_H
