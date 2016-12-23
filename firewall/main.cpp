#include "mainwindow.h"
#include "connection.h"

#include <QApplication>
#include <QTextCodec>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //set the code used in the program
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    //create the connection with the Qt sqlite
    if (!createConnection()) return 1;

    MainWindow w;
    w.show();

    a.exec();
    QProcess ipProcess;
    //reset the iptables
    QStringList finArguments;
    finArguments<<"-F"<<"OUTPUT";
    ipProcess.start("iptables",finArguments);
    ipProcess.waitForFinished();

    QStringList finInArgus;
    finInArgus<<"-F"<<"INPUT";
    ipProcess.start("iptables",finInArgus);
    ipProcess.waitForFinished();
    qDebug()<<"finish reset the iptables";
    return 1;
}
