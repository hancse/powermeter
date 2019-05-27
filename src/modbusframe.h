#ifndef MODBUSFRAME_H
#define MODBUSFRAME_H

#include <QFrame>
#include <QModbusClient>

#include "commonsrc/serialdialog.h"
#include "commonsrc/tcpdialog.h"
#include "src/deifmodbus.h"

namespace Ui {
class ModbusFrame;
}

class ModbusFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ModbusFrame(QWidget *parent = nullptr);
    ~ModbusFrame();

    DEIFModbus* deif;

    void connectRTU();
    void connectTCP();
private slots:
    void on_btnConnect_clicked();
    //void on_btnDisconnect_clicked();
    void on_btnConfig_clicked();

    void on_connectType_currentIndexChanged(int index);
    void onStateChanged(int state);
    void on_readButton_clicked();
    void readReady();
    void on_writeButton_clicked();
    void on_readWriteButton_clicked();
    void on_writeTable_currentIndexChanged(int index);

private:
    QModbusDataUnit readRequest() const;
    QModbusDataUnit writeRequest() const;
    QModbusDataUnit DEIFReadRequest(int startAddress, int numEntries) const;

private:
    Ui::ModbusFrame *ui;

    QModbusReply* lastRequest;
    QModbusClient* modbusDevice;
    SerialDialog* sd;
    TcpDialog* tcpd;

    void displayData();
};

#endif // MODBUSFRAME_H
