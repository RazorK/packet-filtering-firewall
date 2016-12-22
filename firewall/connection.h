#ifndef CONNECTION
#define CONNECTION

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("Firewall_Rules.db");
    if (!db.open()) {
        QMessageBox::critical(0, "Cannot open database1",
                              "Unable to establish a database connection.", QMessageBox::Cancel);
        return false;
    }
    QSqlQuery query;
    //  create the table for rules
    query.exec(QString("create table rule (id int primary key, "
                       "SourceIP varchar, DestinationIP varchar, Protocol varchar,"
                       "SourcePort int, DestinationPort int,IcmpType int,"
                       "Pass bool)"));

    // insert example rules
    query.exec(QString("insert into rule values(1, '192.168.158.136','111.13.100.92', 'all','80','80','all', 0)"));
    query.exec(QString("insert into rule values(2, '127.0.0.1','127.0.0.1', 'all','all','all','all', 0)"));
    return true;
}

#endif // CONNECTION

