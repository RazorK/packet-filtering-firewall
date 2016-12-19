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
               "SouceIP varchar, DestinationIP varchar, SourcePort int, DestinationPort int,"
               "Pass bool)"));

    // insert example rules
    query.exec(QString("insert into rule values(1, '59.78.26.140','59.78.26.141','80','80', False)"));
    query.exec(QString("insert into rule values(2, '59.78.26.141','59.78.26.140','80','80', False)"));
    return true;
}

#endif // CONNECTION

