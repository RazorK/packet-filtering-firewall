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

    void on_checkBox_3_stateChanged(int arg1);

    void on_checkBox_4_stateChanged(int arg1);

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_protoComoBox_currentIndexChanged(const QString &arg1);

private:
    Ui::EditWindow *ui;
    QSqlTableModel *model;
    MyIpAddrEdit *sourceEdit;
    MyIpAddrEdit *destinationEdit;
    QLineEdit *sourcePortLineEdit;
    QLineEdit *destinationPortLineEdit;
    QComboBox *passComboBox;
    QComboBox *protoComoBox;
    QComboBox *icmpComboBox;

};

#endif // EDITWINDOW_H
