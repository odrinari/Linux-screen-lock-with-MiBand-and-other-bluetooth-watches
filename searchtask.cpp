#include <searchtask.h>
#include <QDebug>

static int SearchResults [4];
static int mean=0, all=0, MassiveCount=0, min=0, max=0, NoSignalCount=0;
static bool ready=false, isLocked=false;

void Task::update(int newresult){
    qDebug() << newresult << endl;
    SearchTimer->start(5000);
    if (newresult==-255 && !isLocked){
        NoSignalCount++;
    } else if (newresult>-255){
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
            qDebug() <<"mean "<< mean << endl;
            all=0; min=0; max=0;
        }
        qDebug() <<Unlock <<Lock<< endl;
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
    if (NoSignalCount==3 && Lock && !isLocked){
        system("loginctl lock-session");
        qDebug() <<"Нет сигнала, заблокировать" << endl;
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
