#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QDialog>
class QSqlTableModel;

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
};

#endif // EDITWINDOW_H
