#ifndef FILTERTHREAD_H
#define FILTERTHREAD_H

#include <QThread>


#include <unistd.h> // fork, close
#include <arpa/inet.h>  // inet_ntoa
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
extern "C" {
#include <linux/netfilter.h>
//#include <libipq.h>
}
//#include <linux/netfilter.h>
#include <libnetfilter_queue/libnetfilter_queue.h>
#include <string.h>
#include<time.h>
#include<sys/time.h>
#include<linux/ip.h>
#include<netinet/tcp.h>
#include<linux/udp.h>
#include<linux/icmp.h>

#define MATCH 0
#define NMATCH 1


class FilterThread : public QThread
{
    Q_OBJECT
public:
    explicit FilterThread(QObject *parent = 0);
    void stop();

protected:
    void run();


private:
    volatile bool enable_flag;

public slots:

};

#endif // FILTERTHREAD_H
