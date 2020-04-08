#include "mainwindow.h"
#include "ui_mainwindow.h"

static QSettings Mysettings(QSettings::NativeFormat, QSettings::UserScope, "btunlock");

void MainWindow::ReadSettings() {
    ui->MAC->setText(Mysettings.value("main/MAC", "aa").toString());
    ui->Lock->setChecked(Mysettings.value("features/Lock", false).toBool());
    ui->Unlock->setChecked(Mysettings.value("features/Unlock", false).toBool());
    ui->LockOnInputEvent->setChecked(Mysettings.value("features/LockOnInputEvent", false).toBool());
    ui->LockThreshold->setValue(Mysettings.value("main/LockThreshold", -60).toInt());
    ui->UnlockThreshold->setValue(Mysettings.value("main/UnlockThreshold", -40).toInt());
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ReadSettings();
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_DialogButtons_accepted()
{
    Mysettings.beginGroup("main");
    Mysettings.setValue("MAC", ui->MAC->text());
    Mysettings.setValue("UnlockThreshold", ui->UnlockThreshold->value());
    Mysettings.setValue("LockThreshold", ui->LockThreshold->value());
    Mysettings.endGroup();
    Mysettings.beginGroup("features");
    Mysettings.setValue("Unlock", ui->Unlock->isChecked());
    Mysettings.setValue("Lock", ui->Lock->isChecked());
    Mysettings.setValue("LockOnInputEvent", ui->LockOnInputEvent->isChecked());
    Mysettings.endGroup();
}

void MainWindow::on_DialogButtons_rejected()
{
    MainWindow::close();
}

void MainWindow::on_FactorySettings_clicked()
{
    Mysettings.clear();
    ReadSettings();
}
