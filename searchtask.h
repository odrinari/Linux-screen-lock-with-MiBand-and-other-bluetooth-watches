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

#ifndef SEARCHTASK_H
#define SEARCHTASK_H
#include <QObject>
#include <QTimer>
#include <daemon.h>
#include <QSettings>
#include <QDebug>

class Task : public QObject{
    Q_OBJECT

public:

    QTimer *SearchTimer = new QTimer(this);
    Task(QObject *parent = 0, QSettings *MySets = 0) : QObject(parent) {

        Unlock = MySets->value("features/Unlock", false).toBool();
        qDebug() << Unlock << endl;
        Lock = MySets->value("features/Lock", false).toBool();
        qDebug() << Lock << endl;
        LockThreshold = MySets->value("main/LockThreshold", -60).toInt();
        qDebug() << LockThreshold << endl;
        UnlockThreshold= MySets->value("main/UnlockThreshold", -40).toInt();
        qDebug() << UnlockThreshold << endl;

            QObject::connect(SearchTimer, SIGNAL(timeout()), this, SLOT(run()));
            SearchTimer->start(5000);



    DiscoveryThread *SearchThread= new DiscoveryThread(MySets->value("main/MAC", "aa").toString().toStdString().c_str());
            connect(SearchThread, SIGNAL(newRes(int)), this, SLOT(update(int)));
            SearchThread->start();

    }
public slots:

    void run();
    void update (int);
private:
    bool Unlock, Lock;
    int LockThreshold, UnlockThreshold;
};

#endif // SEARCHTASK_H
