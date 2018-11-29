#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModbusClient>

#include "commonsrc/serialdialog.h"
#include "commonsrc/tcpdialog.h"

namespace Ui {
class MainWindow;
//class SettingsDialog;
}

//class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    //void initActions();
    QModbusDataUnit readRequest() const;
    QModbusDataUnit writeRequest() const;

private slots:
    //void on_connectButton_clicked();

    void on_btnConnect_clicked();
    void on_btnDisconnect_clicked();
    void on_btnConfig_clicked();

    void on_connectType_currentIndexChanged(int index);
    void onStateChanged(int state);
    void on_readButton_clicked();
    void readReady();
    void on_writeButton_clicked();
    void on_readWriteButton_clicked();
    void on_writeTable_currentIndexChanged(int index);
private:
    Ui::MainWindow *ui;

    QModbusReply* lastRequest;
    QModbusClient* modbusDevice;
    //SettingsDialog *m_settingsDialog;
    SerialDialog* sd;
    TcpDialog* tcpd;
};

#endif // MAINWINDOW_H
