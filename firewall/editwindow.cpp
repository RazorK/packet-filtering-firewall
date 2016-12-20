#include "editwindow.h"
#include "ui_editwindow.h"
#include "myipaddredit.h"

#include <QIntValidator>
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

    //try ipedit
    sourceEdit = new MyIpAddrEdit(this);
    sourceEdit->settext("127.0.0.1");
    ui->formLayout->addRow("SourceIP",sourceEdit);

    destinationEdit = new MyIpAddrEdit(this);
    destinationEdit->settext("127.0.0.1");
    ui->formLayout->addRow("DestinationIP",destinationEdit);

    //protocol select
    protoComoBox = new QComboBox(this);
    ui->formLayout->addRow("Protocol",protoComoBox);
    protoComoBox->addItem("all");
    protoComoBox->addItem("tcp");
    protoComoBox->addItem("icmp");
    protoComoBox->addItem("udp");


    //port validator
    QValidator *vali =new QIntValidator(0,65535,this);

    //port edit
    sourcePortLineEdit = new QLineEdit();
    sourcePortLineEdit->setValidator(vali);
    ui->formLayout->addRow("SourceProt",sourcePortLineEdit);

    destinationPortLineEdit = new QLineEdit(this);
    destinationPortLineEdit->setValidator(vali);
    ui->formLayout->addRow("DestinationPort",destinationPortLineEdit);

    //pass edit
    passComboBox = new QComboBox(this);
    ui->formLayout->addRow("Pass",passComboBox);
    passComboBox->addItem("False");
    passComboBox->addItem("True");

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
    record.setValue("SourceIP",sourceEdit->text());
    record.setValue("DestinationIP",destinationEdit->text());
    record.setValue("Protocol",protoComoBox->currentText());
    record.setValue("SourcePort",sourcePortLineEdit->text().toInt());
    record.setValue("DestinationPort",destinationPortLineEdit->text().toInt());
    record.setValue("Pass",(passComboBox->currentText()=="True"?1:0));
    model->insertRecord(rowNum,record);
    //qDebug()<<sourceEdit->text();

    accept();
}
