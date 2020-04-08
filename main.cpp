#include "mainwindow.h"
#include "daemon.h"
#include <QApplication>
#include <searchtask.h>

static QSettings ReadMySettings("/home/olesya/.config/btunlock.conf", QSettings::NativeFormat);

int main(int argc, char *argv[])
{
    if (argc==1){
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    } else if (!(strcmp(argv[1], "-d"))){
        QCoreApplication a(argc, argv);
        QSharedPointer<Task> task(new Task(0, &ReadMySettings));
        return a.exec();
    }
}
