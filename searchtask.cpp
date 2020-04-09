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

#include <searchtask.h>
#include <QDebug>

static int SearchResults [4];
static int mean=0, all=0, MassiveCount=0, min=0, max=0, NoSignalCount=0;
static bool ready=false, isLocked=false, firstResult=false;

void Task::update(int newresult){
    qDebug() <<"newresult: " <<newresult << endl;
    SearchTimer->start(5000);
    if (newresult==-255 && !isLocked){
        NoSignalCount++;
    } else if (newresult>-255){
        if (!firstResult){
            firstResult=true;
        }
        NoSignalCount=0;
        SearchResults[MassiveCount]=newresult;
        MassiveCount++;
        if (!ready && MassiveCount==4){
        ready=true;
        }
        if (MassiveCount==4){
            MassiveCount=0;
        }
        if(ready){
            for (int i=0; i<4; i++){
                if (SearchResults[i]<SearchResults[min]){
                    min=i;
                }
                if (SearchResults[i]>SearchResults[max]){
                    max=i;
                }
                all=all+SearchResults[i];
            }
            mean = (all-SearchResults[min]-SearchResults[max])/2;
            qDebug() <<"mean: "<< mean << endl;
            all=0; min=0; max=0;
        }
        if (Unlock && mean>=UnlockThreshold && isLocked){
            qDebug() <<"Разблокировать" << endl;
            system("loginctl unlock-session");
            isLocked=false;
        } else if (Lock && mean<=LockThreshold && !isLocked) {
            qDebug() <<"Заблокировать" << endl;
            system("loginctl lock-session");
            isLocked=true;
        }
    }
    if (firstResult && NoSignalCount==3 && Lock && !isLocked){
        qDebug() <<"Нет сигнала, заблокировать" << endl;
        system("loginctl lock-session");
        ready=false;
        isLocked=true;
        MassiveCount=0;
        for (int i=0; i<4; i++) {
            SearchResults[i]=0;
        }
    }
}

void Task::run(){
    update(-255);
}
