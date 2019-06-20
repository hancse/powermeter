#ifndef DEIFMODBUS_H
#define DEIFMODBUS_H

#include <QObject>
#include <QModbusClient>
#include <QTimer>

#include "deifregisters.h"

class DEIFModbus : public QObject
{
    Q_OBJECT
public:
    explicit DEIFModbus(QObject *parent = nullptr);
    ~DEIFModbus();

    QModbusReply* lastRequest;
    QModbusClient* modbusDevice;

    QTimer* deifTimer;

    AnalogParams getAp() const;
    void RegsToAp(QModbusDataUnit du);

    void readAll();
    QModbusDataUnit DEIFReadRequest(int startAddress, quint16 numEntries) const;

signals:
    void dataReady();

public slots:

private:
    //QModbusReply* lastRequest;
    //QModbusClient* modbusDevice;

    AnalogParams ap;

    QModbusDataUnit readRequest() const;
    QModbusDataUnit writeRequest() const;

    double ByteArrayToDouble(QByteArray ba, double defaultValue);
    double RegistersToDouble(quint16 highWord, quint16 lowWord);
};

#endif // DEIFMODBUS_H
