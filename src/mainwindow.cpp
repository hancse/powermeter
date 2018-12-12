#include "mainwindow.h"
#include "ui_mainwindow.h"

enum ModbusConnection {
    Serial,
    Tcp
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mbf = new ModbusFrame(this);
    mbf->move(100, 10);
}

MainWindow::~MainWindow()
{
    delete ui;
}





















