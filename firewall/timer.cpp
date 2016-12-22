#include "timer.h"
#include <QDebug>

Timer::Timer(QObject *parent) :
    QThread(parent)
{
    flag = 1;
    connect(parent,SIGNAL(timeSet(QTime,QTime)),this,SLOT(getTimeInfo(QTime,QTime)));
}

void Timer::run()
{
    qreal i = 0;
    while (flag) {
        qDebug() << QString("in MyThread: %1").arg(i);
        msleep(1000);
        i++;
    }
    flag = 1;
}

void Timer::stop()
{
    flag = 0;
}

void Timer::getTimeInfo(QTime start, QTime end)
{
    starttime = start;
    endtime = end;
    qDebug()<<starttime.toString()<<endtime.toString();
}
