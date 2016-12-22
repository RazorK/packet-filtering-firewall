#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editwindow.h"
#include "msghandlewapper.h"

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
    model->setSort(0, Qt::AscendingOrder);
    model->select();

    // set the edit policy
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setModel(model);

    //read only
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //set the column width
    ui->tableView->setColumnWidth(0,30);
    ui->tableView->setColumnWidth(1,106);
    ui->tableView->setColumnWidth(2,106);
    ui->tableView->setColumnWidth(3,50);
    ui->tableView->setColumnWidth(4,74);
    ui->tableView->setColumnWidth(5,91);
    ui->tableView->setColumnWidth(6,40);

    //only select by rows
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //hide textedit
    ui->textEdit->setReadOnly(1);
    //ui->textEdit->hide();

    connect(MsgHandlerWapper::instance(),SIGNAL(message(QtMsgType,QString)),SLOT(pringMsg(QtMsgType,QString)));
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
        model->setSort(0, Qt::AscendingOrder);
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

void MainWindow::on_pushButton_toggled(bool checked)
{
    //TODO: Add time filter logic
    if(checked) ui->pushButton->setText("Stop Time Filter");
    else ui->pushButton->setText("Start Time Filter");
}

void MainWindow::on_infoButton_toggled(bool checked)
{
    ui->textEdit->setVisible(!checked);
    if(checked) ui->infoButton->setText("Show Package Info");
    else ui->infoButton->setText("Hide Package Info");
}

void MainWindow::on_startButtion_toggled(bool checked)
{
    //TODO: add thread logic
    if(checked)
    {
        //qDebug()<<"start pressed"<<endl;
        filter.start();
        ui->startButtion->setText("Stop System");

        //set the output iptables
        QString program = "iptables";
        QStringList arguments;
        arguments <<"-A" <<"OUTPUT" <<"-j"<<"QUEUE";
        ipProcess.start(program, arguments);
        ipProcess.waitForFinished();

        QStringList inputArgus;
        inputArgus<<"-A"<<"INPUT"<<"-j"<<"QUEUE";
        ipProcess.start(program, inputArgus);
        ipProcess.waitForFinished();
        qDebug()<<"finish iptables set process";
    }
    else {
        ui->startButtion->setText("Start System");
        if(filter.isRunning()){
            filter.stop();
        }
        //try send package to end the recv proceed
        qDebug()<<"start sweep sending";
        QUdpSocket *sender;
        sender = new QUdpSocket(this);
        QByteArray datagram = "hello";
        sender->writeDatagram(datagram.data(),datagram.size(),QHostAddress::Broadcast,45454);
        qDebug()<<"finish sweep sending";

        //reset the iptables
        QStringList finArguments;
        finArguments<<"-D"<<"OUTPUT"<<"1";
        ipProcess.start("iptables",finArguments);
        ipProcess.waitForFinished();

        QStringList finInArgus;
        finInArgus<<"-D"<<"INPUT"<<"1";
        ipProcess.start("iptables",finInArgus);
        ipProcess.waitForFinished();
        qDebug()<<"finish reset the iptables";
    }
}

void MainWindow::pringMsg(QtMsgType q_type, const QString &msg)
{
    ui->textEdit->append(msg);
}
