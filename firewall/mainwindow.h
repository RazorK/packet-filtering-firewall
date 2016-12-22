#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QProcess>
#include <QDialog>
#include <QDebug>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QMainWindow>
#include <QtNetwork>
#include "filterthread.h"
#include "timer.h"

class QSqlTableModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void pringMsg(QtMsgType q_type,const QString &msg);

private slots:
    void on_addButton_clicked();

    void on_deleteButton_clicked();

    void on_pushButton_toggled(bool checked);

    void on_infoButton_toggled(bool checked);

    void on_startButtion_toggled(bool checked);

signals:
    void timeSet(QTime startTime,QTime endTime);

private:
    Ui::MainWindow *ui;
    QSqlTableModel *model;
    FilterThread filter;
    QProcess ipProcess;
    Timer *tm;

    //function
    void endSystem();
};

#endif // MAINWINDOW_H
