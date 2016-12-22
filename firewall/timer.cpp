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
    while (flag) {
        //qDebug() << "timer thread running and detecting";
        msleep(1000);
        //qDebug()<<"now in = "<<in;
        if(in && QTime::currentTime()>endtime){
            emit leavePeriod();
            in = 0;
            //qDebug()<<"send leaveperiod signal";
        }
        if(!in && QTime::currentTime()>starttime && QTime::currentTime()<endtime){
            emit enterPeriod();
            in = 1;
            //qDebug()<<"send enterperiod signal";
        }
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
    if(QTime::currentTime()>start && QTime::currentTime()<end){
        in = 1;
        qDebug()<<"in the period";
    }
    else{
        in = 0;
        qDebug()<<"not in period";
    }
    qDebug()<<starttime.toString()<<endtime.toString();
}
