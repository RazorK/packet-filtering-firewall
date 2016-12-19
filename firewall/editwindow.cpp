#include "editwindow.h"
#include "ui_editwindow.h"

EditWindow::EditWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditWindow)
{
    ui->setupUi(this);

    //set value for pass selection
    ui->passComboBox->addItem("False");
    ui->passComboBox->addItem("True");
}

EditWindow::~EditWindow()
{
    delete ui;
}
