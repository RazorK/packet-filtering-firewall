#include "filterthread.h"
#include <QDebug>

FilterThread::FilterThread(QObject *parent) :
    QThread(parent)
{
    stopped = false;
}

void FilterThread::run()
{
    qreal i = 0;
    while (!stopped) {
        qDebug() << QString("in MyThread: %1").arg(i);
        msleep(1000);
        i++;
    }
    stopped = false;
}

void FilterThread::stop()
{
    stopped = true;
}
