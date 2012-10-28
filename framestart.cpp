#include "framestart.h"
#include "ui_framestart.h"

FrameStart::FrameStart(QWidget *parent) :
    FrameObject(parent),
    ui(new Ui::FrameStart)
{
    ui->setupUi(this);

    QSettings settings;
    ui->lineEditProjectFolder->setText(settings.value("Settings/last_project_path").toString());
    if(testFile(settings.value("Settings/last_project_path").toString())) {
        emit acceptedProjectFolder(ui->lineEditProjectFolder->text());
    }

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonSelectProjectFolder, SIGNAL(clicked()), this, SLOT(openFileBrowser()));
    connect(ui->lineEditProjectFolder, SIGNAL(textChanged(QString)), this, SLOT(testFile(QString)));
}

FrameStart::~FrameStart()
{
    delete ui;
}


void FrameStart::openFileBrowser() {
    QString preselected_path = QDir::homePath();
    if(QDir().exists(ui->lineEditProjectFolder->text())) {
        preselected_path = ui->lineEditProjectFolder->text();
    }
        ui->lineEditProjectFolder->setText(QFileDialog::getExistingDirectory(this, this->tr("Select project folder"), preselected_path, QFileDialog::ShowDirsOnly));
}

/*!
 * \brief FrameStart::testFile
 * \param path
 * \return true if path is an existing writeable folder. Otherwise returns false.
 */
bool FrameStart::testFile(QString path) {
    ui->error_label->clear();
    ui->buttonBox->setEnabled(false);
    QFileInfo folder_file(path);
    if(folder_file.isFile()) {
        ui->error_label->setText("This is not a folder.");
        return false;
    }
    if(!folder_file.exists()) {
        ui->error_label->setText(tr("This folder does not exist."));
        return false;
    }
    if(!folder_file.isWritable()) {
        ui->error_label->setText(tr("This folder is not writable."));
        return false;
    }
    ui->buttonBox->setEnabled(true);
    return true;
}


void FrameStart::accept() {
    QDir folder(ui->lineEditProjectFolder->text());
    QFileInfo folder_file(folder.absolutePath());
    if(!folder.exists() || !folder.isAbsolute() || !folder_file.isWritable()) {
        QMessageBox::critical(this, tr("Selected folder not valid"), tr("You selected an invalid project folder. Please select an existing folder to proceed."), QMessageBox::Ok);
        return;
    }
    ui->buttonBox->setEnabled(false);
    emit acceptedProjectFolder(folder.path());
}
