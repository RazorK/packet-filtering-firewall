#include "editwindow.h"
#include "ui_editwindow.h"
#include "myipaddredit.h"
#include <QDebug>
#include <QSqlRecord>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>


EditWindow::EditWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditWindow)
{
    ui->setupUi(this);

    //set value for pass selection
    ui->passComboBox->addItem("False");
    ui->passComboBox->addItem("True");

    //try ipedit
    //MyIpAddrEdit *ipAddr = new MyIpAddrEdit();
    sourceEdit = new MyIpAddrEdit(this);
    sourceEdit->settext("127.0.0.1");
    ui->formLayout->addRow("IPtest",sourceEdit);

    //sql part
    model = new QSqlTableModel(this);
    model->setTable("rule");
    model->select();
}

EditWindow::~EditWindow()
{
    delete ui;
}

void EditWindow::on_buttonBox_accepted()
{
    //get row number
    int rowNum = model->rowCount();

    //create record && insert to the db
    QSqlRecord record = model->record();
    record.setValue("id",rowNum+1);
    record.setValue("SourceIP",ui->sourceIPLineEdit->text());
    record.setValue("DestinationIP",ui->destinationIPLineEdit->text());
    record.setValue("SourcePort",ui->sourcePortLineEdit->text().toInt());
    record.setValue("DestinationPort",ui->destinationPortLineEdit->text().toInt());
    record.setValue("Pass",(ui->passComboBox->currentText()=="True"?1:0));
    model->insertRecord(rowNum,record);

    qDebug()<<sourceEdit->text();

    accept();
}
