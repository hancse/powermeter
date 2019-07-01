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

    void readDEIF(int regType, int serverAddress, int StartAddress, int numberOfEntries);

    AnalogParams getAp() const;
    void RegsToAp(QModbusDataUnit du);

    void readAll();
    //QModbusDataUnit DEIFReadRequest(int startAddress, quint16 numEntries) const;

    int getServerAddress() const;
    void setServerAddress(int value);

signals:
    void dataReady();

public slots:

private:
    //QModbusReply* lastRequest;
    //QModbusClient* modbusDevice;

    AnalogParams ap;
    EnergyMeasurement em;
    int serverAddress = -1;

    //QModbusDataUnit readRequest(int regType, int startAddress, int numberOfEntries) const;
    //QModbusDataUnit writeRequest(int regType, int startAddress, int numberOfEntries) const;
    //QModbusDataUnit DEIFReadRequest(int regType, int startAddress, int numberOfEntries) const;

    double ByteArrayToDouble(QByteArray ba, double defaultValue);
    double RegistersToDouble(quint16 highWord, quint16 lowWord);

    void readReady();
    void RegsToEm(QModbusDataUnit du);
    quint32 RegistersToDWord(quint16 highWord, quint16 lowWord);
};

#endif // DEIFMODBUS_H
