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

#ifndef DAEMON_H
#define DAEMON_H

#include <QThread>
#include <QString>

class DiscoveryThread : public QThread
{
Q_OBJECT
public:
explicit DiscoveryThread(const char* mac);

void run();
signals:
int newRes(int);

private:
    char searching_addr[18];
};


#endif // DAEMON_H
