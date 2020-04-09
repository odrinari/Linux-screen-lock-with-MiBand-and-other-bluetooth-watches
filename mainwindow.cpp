//    BtUnlock
//    Copyright (c) 2020 Drobysh Olesya. All rights reserved.
//
//    This file is part of BtUnlock.
//
//    BtUnlock is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    BtUnlock is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with BtUnlock.  If not, see <https://www.gnu.org/licenses/>.

#include "mainwindow.h"
#include "ui_mainwindow.h"

static QSettings Mysettings(QSettings::NativeFormat, QSettings::UserScope, "btunlock");

void MainWindow::ReadSettings() {
    ui->MAC->setText(Mysettings.value("main/MAC", "00:00:00:00:00:00").toString());
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
    help.setText("Настройка программы: \n   1. Введите MAC адрес вашего BLE устройства \n   2. Сохраните и запустите процесс btunlock -d через консоль "
                 "\n   3. Поэкспериментируйте с входящими значениями при определенных расстояниях между устройством и компьютером."
                 " \n   4. Проставьте галочки на желаемых режимах работы \n"
                 "   5. Выставьте пороговые значения и сохраните конфигурацию\n   6. Перезапустите процесс");
    help.exec();
}

void MainWindow::on_about_triggered()
{
    QMessageBox about;
    about.setWindowTitle("О программе");
    about.setText("BtUnlock 0.1 \nРепозиторий на GitHub: https://github.com/odrinari/btunlock \n"
                  "В программе используется алгоритм поиска Bluetooth устройств от Damian Kołakowski \n"
                  "Ссылка на репозиторий GitHub: https://github.com/damian-kolakowski/intel-edison-playground");
    about.exec();
}
