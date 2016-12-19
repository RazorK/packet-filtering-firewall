#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editwindow.h"

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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //fix the size of mainwindow
    setFixedSize(this->width(),this->height());

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

    //only select by rows
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButton_clicked()
{
    EditWindow ew;
    // refresh the tableview
    if(ew.exec()==QDialog::Accepted){
        model->setTable("rule");
        model->select();
    }
}

void MainWindow::on_deleteButton_clicked()
{
    // get row num
    int curRow = ui->tableView->currentIndex().row();
    // delete
    model->removeRow(curRow);
    int ok = QMessageBox::warning(this,"Deleting!",
                  tr("Deleting the selected rule! Are you sure?"),QMessageBox::Yes, QMessageBox::No);
    if(ok == QMessageBox::No)
    {
        model->revertAll();
    } else {
        model->submitAll();
    }
}
