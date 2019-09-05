#ifndef MODBUSFRAME_H
#define MODBUSFRAME_H

#include <QFrame>
#include "commonsrc/stripframetriple.h"

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
    void on_btnConfig_clicked();

    void on_connectType_currentIndexChanged(int index);
    void onStateChanged(int state);
    void on_readButton_clicked();
    void on_writeButton_clicked();
    //void on_readWriteButton_clicked();
    void on_writeTable_currentIndexChanged(int index);

    void on_checkAuto_clicked(bool checked);

private:
    QModbusDataUnit writeRequest(int regType, int startAddress, int numOfEntries) const;

private:
    Ui::ModbusFrame *ui;

    SerialDialog* sd;
    TcpDialog* tcpd;
    StripFrameTriple* strpf;

    void displayData(int addr, UniversalAEParams ae);
    void readDEIF();
};

#endif // MODBUSFRAME_H
