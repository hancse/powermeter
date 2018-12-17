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

    mbf = new ModbusFrame();
    ui->stackedWidget->addWidget(mbf);
    sqlf = new SQLiteFrame();
    ui->stackedWidget->addWidget(sqlf);
    ui->stackedWidget->setCurrentIndex(0);

    //mbf->move(120, 20);
}

MainWindow::~MainWindow()
{
    delete ui;
}





















