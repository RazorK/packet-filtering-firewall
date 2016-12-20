#include "mainwindow.h"
#include "connection.h"

#include <QProcess>
#include <QApplication>
#include <QTextCodec>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //set the code used in the program
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    //TODO: set the iptables
    QProcess ipProcess;
    QString program = "iptables";
    QStringList arguments;
    arguments <<"-A" <<"OUTPUT" <<"-j"<<"QUEUE";
    ipProcess.start(program, arguments);
    ipProcess.waitForFinished();
    qDebug()<<"finish iptables set process";


    //create the connection with the Qt sqlite
    if (!createConnection()) return 1;

    MainWindow w;
    w.show();

    a.exec();
    //TODO:reset the iptables
    QStringList finArguments;
    finArguments<<"-D"<<"OUTPUT"<<"1";
    ipProcess.start("iptables",finArguments);
    ipProcess.waitForFinished();
    qDebug()<<"finish reset the iptables";
    return 1;
}
