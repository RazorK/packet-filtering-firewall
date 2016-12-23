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
#include <QStringList>


EditWindow::EditWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditWindow)
{
    ui->setupUi(this);

    setFixedSize(this->width(),this->height());
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
    connect(protoComoBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(protoComoBox_currentIndexChanged(QString)));

    //icmp type
    icmpComboBox = new QComboBox(this);
    ui->formLayout->addRow("ICMP Type",icmpComboBox);
    QStringList ls;
    ls<<"all"<<"0-echo-reply"<<"8-echo-request"<<"11-TTL0";
    icmpComboBox->addItems(ls);

    //port validator
    QValidator *vali =new QIntValidator(0,65535,this);

    //port edit
    sourcePortLineEdit = new QLineEdit();
    sourcePortLineEdit->setValidator(vali);
    ui->formLayout->addRow("SourcePort",sourcePortLineEdit);

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
    QSqlQuery query;
    query.exec("select max(id) from rule");
    query.next();
    int m_id = query.value(0).toInt();

    //create record && insert to the db
    QSqlRecord record = model->record();
    record.setValue("id",m_id+1);
    if(ui->checkBox->checkState()==2)
        record.setValue("SourceIP","all");
    else
        record.setValue("SourceIP",sourceEdit->text());

    if(ui->checkBox_2->checkState()==2)
        record.setValue("DestinationIP","all");
    else
        record.setValue("DestinationIP",destinationEdit->text());

    record.setValue("Protocol",protoComoBox->currentText());

    if(icmpComboBox->currentText()=="all")
        record.setValue("icmptype","all");
    else if(icmpComboBox->currentText()=="0-echo-reply")
        record.setValue("icmptype",0);
    else if(icmpComboBox->currentText()=="8-echo-request")
        record.setValue("icmptype",8);
    else if(icmpComboBox->currentText()=="11-TTL0")
        record.setValue("icmptype",11);

    if(ui->checkBox_3->checkState()==2)
        record.setValue("SourcePort","all");
    else
        record.setValue("SourcePort",sourcePortLineEdit->text().toInt());

    if(ui->checkBox_4->checkState()==2)
        record.setValue("DestinationPort","all");
    else
        record.setValue("DestinationPort",destinationPortLineEdit->text().toInt());

    record.setValue("Pass",(passComboBox->currentText()=="True"?1:0));
    model->insertRecord(rowNum,record);
    //qDebug()<<sourceEdit->text();

    accept();
}

void EditWindow::on_checkBox_3_stateChanged(int arg1)
{
    if(arg1==0)
        sourcePortLineEdit->setEnabled(1);
    else if(arg1==2)
        sourcePortLineEdit->setEnabled(0);
}

void EditWindow::on_checkBox_4_stateChanged(int arg1)
{
    if(arg1==0)
        destinationPortLineEdit->setEnabled(1);
    else if(arg1==2)
        destinationPortLineEdit->setEnabled(0);
}

void EditWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1==0)
        sourceEdit->setEnabled(1);
    else if(arg1==2)
        sourceEdit->setEnabled(0);
}

void EditWindow::on_checkBox_2_stateChanged(int arg1)
{
    if(arg1==0)
        destinationEdit->setEnabled(1);
    else if(arg1==2)
        destinationEdit->setEnabled(0);
}

void EditWindow::protoComoBox_currentIndexChanged(const QString &str){
    //qDebug()<<str<<endl;
    if(str=="icmp"){
        sourcePortLineEdit->setEnabled(0);
        destinationPortLineEdit->setEnabled(0);
    }
    else{
        sourcePortLineEdit->setEnabled(1);
        destinationPortLineEdit->setEnabled(1);
    }

    if(str=="tcp" || str=="udp"){
        icmpComboBox->setEnabled(0);
    }
    else{
        icmpComboBox->setEnabled(1);
    }
}
