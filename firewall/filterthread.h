#ifndef FILTERTHREAD_H
#define FILTERTHREAD_H

#include <QThread>

class FilterThread : public QThread
{
    Q_OBJECT
public:
    explicit FilterThread(QObject *parent = 0);
    void stop();

protected:
    void run();

private:
    volatile bool stopped;

signals:

public slots:

};

#endif // FILTERTHREAD_H
