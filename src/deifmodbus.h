#ifndef DEIFMODBUS_H
#define DEIFMODBUS_H

#include <QObject>
#include <QModbusClient>
#include <QTimer>

#include "deifmicregisters.h"
#include "deifmk2registers.h"

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

    Mk2AnalogParams getMk2Ap() const;

    void readAll();
    //QModbusDataUnit DEIFReadRequest(int startAddress, quint16 numEntries) const;

    int getServerAddress() const;
    void setServerAddress(int value);

    bool getIsMK2() const;
    void setIsMK2(bool value);

    MicAnalogParams getMicap() const;

signals:
    void dataReady();

public slots:

private:
    //QModbusReply* lastRequest;
    //QModbusClient* modbusDevice;

    bool isMK2 = false;
    MicAnalogParams micap;
    MicEnergyMeasurement micem;
    Mk2AnalogParams mk2ap;
    Mk2EnergyMeasurement mk2em;
    int serverAddress = -1;

    //QModbusDataUnit readRequest(int regType, int startAddress, int numberOfEntries) const;
    //QModbusDataUnit writeRequest(int regType, int startAddress, int numberOfEntries) const;
    //QModbusDataUnit DEIFReadRequest(int regType, int startAddress, int numberOfEntries) const;

    double ByteArrayToDouble(QByteArray ba, double defaultValue);
    double RegistersToDouble(quint16 highWord, quint16 lowWord);

    void readReady();
    void RegsToMicAp(QModbusDataUnit du);
    void RegsToMicEm(QModbusDataUnit du);
    void RegsToMk2Ap(QModbusDataUnit du);
    void RegsToMk2Em(QModbusDataUnit du);
    quint32 RegistersToDWord(quint16 highWord, quint16 lowWord);
    void readDummyAll();
};

#endif // DEIFMODBUS_H
