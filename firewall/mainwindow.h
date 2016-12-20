#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "filterthread.h"

class QSqlTableModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_addButton_clicked();

    void on_deleteButton_clicked();

    void on_pushButton_toggled(bool checked);

    void on_infoButton_toggled(bool checked);

    void on_startButtion_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    QSqlTableModel *model;
    FilterThread filter;
};

#endif // MAINWINDOW_H
