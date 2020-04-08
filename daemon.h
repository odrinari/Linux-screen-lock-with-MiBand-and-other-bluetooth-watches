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
