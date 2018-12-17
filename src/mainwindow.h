#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "src/modbusframe.h"
#include "src/sqliteframe.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:


private slots:


private:
    Ui::MainWindow *ui;
    ModbusFrame* mbf;
    SQLiteFrame* sqlf;
};

#endif // MAINWINDOW_H
