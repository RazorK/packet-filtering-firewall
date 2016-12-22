#ifndef TIMER_H
#define TIMER_H

#include <QThread>
#include <QTime>

class Timer : public QThread
{
    Q_OBJECT
public:
    explicit Timer(QObject *parent = 0);
    void stop();
public slots:
    void getTimeInfo(QTime start,QTime end);

protected:
    void run();

private:
    volatile bool flag;
    QTime starttime;
    QTime endtime;

signals:
    //void enterPeriod();
    //void leavePeriod();

public slots:

};
#endif // TIMER_H
