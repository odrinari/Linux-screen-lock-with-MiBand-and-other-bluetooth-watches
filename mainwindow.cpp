#include "mainwindow.h"
#include "ui_mainwindow.h"

static QSettings Mysettings(QSettings::NativeFormat, QSettings::UserScope, "btunlock");

void MainWindow::ReadSettings() {
    ui->MAC->setText(Mysettings.value("main/MAC", "aa").toString());
    ui->Lock->setChecked(Mysettings.value("features/Lock", false).toBool());
    ui->Unlock->setChecked(Mysettings.value("features/Unlock", false).toBool());
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

void MainWindow::on_help_triggered()
{
    QMessageBox help;
    help.setWindowTitle("Справка");
    help.setText("Настройка программы: \n   1. Введите MAC адрес вашего BLE устройства \n   2. Поставьте галочки на желаемых режимах работы \n"
                 "   3. Настройте пороги под себя (Чем больше отрицательное число, тем устройство ближе) \n"
                 "Для более подробной справки о входящих значениях, советуем запустить процесс через командную строку: btunlock -d \n");
    help.exec();
}

void MainWindow::on_about_triggered()
{
    QMessageBox about;
    about.setWindowTitle("Справка");
    about.setText("Программа: BtUnlock \nВерсия: 0.1 \nДата релиза: 09.04.2020 \nРепозиторий на GitHub: https://github.com/odrinari/btunlock \n"
                  "В программе используется алгоритм поиска Bluetooth устройств от Damian Kołakowski \n"
                  "Ссылка на репозиторий GitHub: https://github.com/damian-kolakowski/intel-edison-playground");
    about.exec();
}
