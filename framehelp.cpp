#include "framehelp.h"
#include "ui_framehelp.h"

extern QString NAME;
extern QString VERSION;
extern QString AUTHOR;
extern QString WEBADDRESS;

FrameHelp::FrameHelp(QWidget *parent) :
    FrameObject(parent),
    ui(new Ui::FrameHelp)
{
    ui->setupUi(this);

    connect(ui->about_button, SIGNAL(clicked()), this, SLOT(showAbout()));
}

FrameHelp::~FrameHelp()
{
    delete ui;
}


void FrameHelp::showAbout() {
    QMessageBox::about(this, tr("About Yonder"), tr("<b>%1 %2</b><p>&copy; 2011-2013 by %3</p><p><a href=\"%4\">%4</a></p><p><i>Running with:</i><br />Qt %5<br />TagLib %6.%7<p><i>Audio engine:</i><br />FMOD Sound System by Firelight Technologies</p><br /><img src=\":/application/qt-logo.svg\" /><img src=\":/application/fmod-logo.svg\" />").arg(NAME, VERSION, AUTHOR, WEBADDRESS, QT_VERSION_STR, QString("%1").arg(TAGLIB_MAJOR_VERSION), QString("%1").arg(TAGLIB_MINOR_VERSION)));
}
