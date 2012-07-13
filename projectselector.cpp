#include <projectselector.h>
#include "ui_projectselector.h"

extern QString NAME;

ProjectSelector::ProjectSelector(QString project_path, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectSelector)
{
    ui->setupUi(this);
    if(QDir().exists(project_path)) {
        ui->lineEditProjectFolder->setText(project_path);
    }
    connect(ui->buttonSelectProjectFolder, SIGNAL(clicked()), this, SLOT(openFileBrowser()));
    connect(ui->lineEditProjectFolder, SIGNAL(textChanged(QString)), this, SLOT(testFile(QString)));
}


void ProjectSelector::openFileBrowser() {
    QString preselected_path = QDir::homePath();
    if(QDir().exists(ui->lineEditProjectFolder->text())) {
        preselected_path = ui->lineEditProjectFolder->text();
    }
        ui->lineEditProjectFolder->setText(QFileDialog::getExistingDirectory(this, this->tr("Select project folder"), preselected_path, QFileDialog::ShowDirsOnly));
}


void ProjectSelector::testFile(QString path) {
    ui->error_label->clear();
    QFileInfo folder_file(path);
    if(folder_file.isFile()) {
        ui->error_label->setText("This is not a folder.");
        return;
    }
    if(!folder_file.exists()) {
        ui->error_label->setText(tr("This folder does not exist."));
        return;
    }
    if(!folder_file.isWritable()) {
        ui->error_label->setText(tr("This folder is not writable."));
    }
}


QString ProjectSelector::getProjectPath() {
    return ui->lineEditProjectFolder->text();
}


void ProjectSelector::accept() {
    QDir folder(ui->lineEditProjectFolder->text());
    QFileInfo folder_file(folder.absolutePath());
    qDebug() << folder.path();
    if(!folder.exists() || !folder.isAbsolute() || !folder_file.isWritable()) {
        QMessageBox::critical(this, tr("Selected folder not valid"), tr("You selected an invalid project folder. Please select an existing folder to proceed or abort to quit Yonder."), QMessageBox::Ok);
        return;
    }
    QDialog::accept();
}


void ProjectSelector::reject() {
    exit(0);
}


ProjectSelector::~ProjectSelector()
{
    delete ui;
}
