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

    //QTimer* deifTimer;  // removed from non-GUI class

    void readDEIF(int regType, int serverAddress, int StartAddress, int numberOfEntries);
    void readDummyAll();
    void readAll();
    //QModbusDataUnit DEIFReadRequest(int startAddress, quint16 numEntries) const;

    int getServerAddress() const;
    void setServerAddress(int value);

    bool getIsMK2() const;
    void setIsMK2(bool value);

    UniversalAEParams getAep() const;
    void setAep(const UniversalAEParams &value);

signals:
    void dataReady(int mbaddress, UniversalAEParams anAep);

public slots:

private:
    //QModbusReply* lastRequest;
    //QModbusClient* modbusDevice;

    bool isMK2 = false;
    UniversalAEParams aep;
    int serverAddress = -1;

    //QModbusDataUnit readRequest(int regType, int startAddress, int numberOfEntries) const;
    //QModbusDataUnit writeRequest(int regType, int startAddress, int numberOfEntries) const;
    //QModbusDataUnit DEIFReadRequest(int regType, int startAddress, int numberOfEntries) const;

    double ByteArrayToDouble(QByteArray ba, double defaultValue);
    double RegistersToDouble(quint16 highWord, quint16 lowWord);

    void readReady();
    void RegsToAp(QModbusDataUnit du);
    void RegsToEm(QModbusDataUnit du);
    quint32 RegistersToDWord(quint16 highWord, quint16 lowWord);

};

#endif // DEIFMODBUS_H
