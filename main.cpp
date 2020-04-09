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
#include "daemon.h"
#include <QApplication>
#include <searchtask.h>

static QSettings ReadMySettings( QSettings::NativeFormat, QSettings::UserScope, "btunlock");

int main(int argc, char *argv[])
{
    if (argc==1){
        QApplication a(argc, argv);
        MainWindow w;
        w.setWindowTitle("BtUnlock 0.1");
        w.show();
        return a.exec();
    } else if (!(strcmp(argv[1], "-d"))){
        QCoreApplication a(argc, argv);
        QSharedPointer<Task> task(new Task(0, &ReadMySettings));
        return a.exec();
    }
}
