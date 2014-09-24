#include "frameconfigure.h"

FrameConfigure::FrameConfigure(QWidget *parent) :
    FrameObject(parent)

{
//    ui->setupUi(this);

//    QSettings settings;
//    ui->check_update->setChecked(settings.value("Settings/check_update", 0).toBool());
//    ui->custom_style->setChecked(settings.value("Settings/custom_style", 1).toBool());
//    ui->lock_menu->setChecked(settings.value("Settings/lock_menu", 0).toBool());
//    ui->lock_menu->hide();

//    ui->webapp_active->setChecked(settings.value("Settings/webapp_active", 0).toBool());
//    ui->webapp_port->setValue(settings.value("Settings/webapp_port", 8888).toInt());
//    ui->webapp_sync->setChecked(settings.value("Settings/webapp_sync", 1).toBool());
//    ui->webapp_sync->hide();
//    ui->webapp_refresh->setValue(settings.value("Settings/webapp_refresh_interval", 10).toInt());

//    connect(ui->check_update, SIGNAL(stateChanged(int)), this, SLOT(saveSettings()));
//    connect(ui->custom_style, SIGNAL(stateChanged(int)), this, SLOT(saveSettings()));
//    connect(ui->lock_menu, SIGNAL(stateChanged(int)), this, SLOT(saveSettings()));

//    connect(ui->webapp_active, SIGNAL(stateChanged(int)), this, SLOT(saveSettings()));
//    connect(ui->webapp_port, SIGNAL(valueChanged(int)), this, SLOT(saveSettings()));
//    connect(ui->webapp_sync, SIGNAL(stateChanged(int)), this, SLOT(saveSettings()));
//    connect(ui->webapp_refresh, SIGNAL(valueChanged(int)), this, SLOT(saveSettings()));
}

FrameConfigure::~FrameConfigure()
{
//    saveSettings();
//    delete ui;
}


void FrameConfigure::saveSettings() {
//    QSettings settings;
//    settings.setValue("Settings/check_update", ui->check_update->isChecked());
//    settings.setValue("Settings/custom_style", ui->custom_style->isChecked());
//    settings.setValue("Settings/lock_menu", ui->lock_menu->isChecked());

//    settings.setValue("Settings/webapp_active", ui->webapp_active->isChecked());
//    settings.setValue("Settings/webapp_port", ui->webapp_port->value());
//    settings.setValue("Settings/webapp_sync", ui->webapp_sync->isChecked());
//    settings.setValue("Settings/webapp_refresh_interval", ui->webapp_refresh->value());

//    settings.sync();
//    qDebug() << "Settings saved";
}
