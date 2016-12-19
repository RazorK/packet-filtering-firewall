#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include "myipaddredit.h"
class QSqlTableModel;
class MyIpAddrEdit;
//class QLineEdit;

namespace Ui {
class EditWindow;
}

class EditWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EditWindow(QWidget *parent = 0);
    ~EditWindow();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::EditWindow *ui;
    QSqlTableModel *model;
    MyIpAddrEdit *sourceEdit;
    MyIpAddrEdit *destinationEdit;
    QLineEdit *sourcePortLineEdit;
    QLineEdit *destinationPortLineEdit;
    QComboBox *passComboBox;

};

#endif // EDITWINDOW_H
