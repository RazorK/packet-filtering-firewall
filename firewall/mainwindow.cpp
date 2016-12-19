#include "mainwindow.h"
#include "ui_mainwindow.h"
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //fix the size of mainwindow
    //setFixedSize(this->width(),this->height());

    //load in the rule data from db
    model = new QSqlTableModel(this);
    model->setTable("rule");
    model->select();

    // set the edit policy
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setModel(model);

    //set the column width
    ui->tableView->setColumnWidth(0,38);
    ui->tableView->setColumnWidth(1,111);
    ui->tableView->setColumnWidth(2,111);
    ui->tableView->setColumnWidth(3,74);
    ui->tableView->setColumnWidth(4,91);
    ui->tableView->setColumnWidth(5,54);
}

MainWindow::~MainWindow()
{
    delete ui;
}

