#include "deifmodbus.h"

#include <QDebug>
#include <QRandomGenerator>

DEIFModbus::DEIFModbus(QObject *parent) :
    QObject(parent),
    lastRequest(nullptr),
    modbusDevice(nullptr)
{
    //deifTimer = new QTimer(this); // removed from non-GUI class
    //connect(deifTimer, &QTimer::timeout, this, &DEIFModbus::readAll);
    //connect(deifTimer, &QTimer::timeout, this, &DEIFModbus::readDummyAll);
}

DEIFModbus::~DEIFModbus()
{
    if (modbusDevice)
        modbusDevice->disconnectDevice();
    delete modbusDevice;
}

void DEIFModbus::readReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        for (uint i = 0; i < unit.valueCount(); i++) {
            const QString entry = tr("Address: %1, Value: %2")
                                     .arg(unit.startAddress() + i)
                                     .arg(QString::number(unit.value(i),
                                          unit.registerType() <= QModbusDataUnit::Coils ? 10 : 16));
            qDebug() << entry;
        }

        switch (unit.startAddress()) {
        case MIC_ANALOG_BASE_ADDRESS:
            RegsToAp(unit);
            break;
        case MIC_ENERGY_BASE_ADDRESS:
            RegsToEm(unit);
            break;
        case MK2_ANALOG_BASE_ADDRESS:
            RegsToAp(unit);
            break;
        case MK2_ENERGY_BASE_ADDRESS:
            RegsToEm(unit);
            break;
        default:
            break;
        }

    } else if (reply->error() == QModbusDevice::ProtocolError) {
        QString errmsg(tr("Read response error: %1 (Modbus exception: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->rawResult().exceptionCode(), -1, 16));
        qDebug() << errmsg;
    } else {
        QString errmsg(tr("Read response error: %1 (code: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->error(), -1, 16));
        qDebug() << errmsg;
    }

    reply->deleteLater();
}

/*
QModbusDataUnit DEIFModbus::DEIFReadRequest(int regType, int startAddress, int numberOfEntries) const
{
    const auto table =
        static_cast<QModbusDataUnit::RegisterType>(regType);
    return QModbusDataUnit(table, startAddress, numberOfEntries);
}
*/

void DEIFModbus::readDEIF(int regType, int serverAddress,
                          int startAddress, int numberOfEntries)
{
    if (!modbusDevice)
        return;

    const auto table =
        static_cast<QModbusDataUnit::RegisterType>(regType);
    QModbusDataUnit unit = QModbusDataUnit(table, startAddress, numberOfEntries);

    //if ( auto* reply = modbusDevice->sendReadRequest(DEIFReadRequest(regType,
                                                    // METER_PARAM_BASE_ADDRESS,
                                                    // 18),
                                                    // serverAddress) ) {

    if ( auto* reply = modbusDevice->sendReadRequest(unit, serverAddress) ) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished,
                    this, &DEIFModbus::readReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        QString errmsg(tr("Read error: ") + modbusDevice->errorString());
        qDebug() << errmsg;
    }
}

double DEIFModbus::RegistersToDouble(quint16 highWord, quint16 lowWord)
{
    quint32 lw = (highWord << 16) + lowWord;
    float value;
    value = *reinterpret_cast<float*>(&lw);
    qDebug() << value;
    return static_cast<double>(value);
}

quint32 DEIFModbus::RegistersToDWord(quint16 highWord, quint16 lowWord)
{
    quint32 value = (highWord << 16) + lowWord;
    qDebug() << value;
    return value;
}

void DEIFModbus::readAll()
{

}

void DEIFModbus::readDummyAll()
{
    qDebug() << "read dummy data";
    aep.freq = 50.0;

    aep.phaseVoltage[0] = 230.0 + QRandomGenerator::global()->generateDouble();
    aep.phaseVoltage[1] = 230.0 + QRandomGenerator::global()->generateDouble();
    aep.phaseVoltage[2] = 230.0 + QRandomGenerator::global()->generateDouble();
    aep.avgVoltage      = 230.0 + QRandomGenerator::global()->generateDouble();

    aep.phaseCurrent[0] = 20.0 + QRandomGenerator::global()->generateDouble();
    aep.phaseCurrent[1] = 20.0 + QRandomGenerator::global()->generateDouble();
    aep.phaseCurrent[2] = 20.0 + QRandomGenerator::global()->generateDouble();
    aep.avgCurrent = 20.0 + QRandomGenerator::global()->generateDouble();

    aep.phaseLPower[0] = 5.0 + QRandomGenerator::global()->generateDouble();
    aep.phaseLPower[1] = 5.0 + QRandomGenerator::global()->generateDouble();
    aep.phaseLPower[2] = 5.0 + QRandomGenerator::global()->generateDouble();
    aep.systemPower = 5.0 + QRandomGenerator::global()->generateDouble();

    aep.energyTotal = 100.0 + QRandomGenerator::global()->generateDouble();
    aep.dt = QDateTime::currentDateTime();
    aep.timestamp = aep.dt.toSecsSinceEpoch();
    //aep.timestamp = aep.dt.toMSecsSinceEpoch();
    aep.userID = 0;
    aep.socketID = 0;

    emit dataReady(serverAddress, aep);
}

void DEIFModbus::RegsToAp(QModbusDataUnit du)
{
    if (isMK2) {
        aep.freq = RegistersToDouble(du.value(0), du.value(1));

        aep.phaseVoltage[0] = RegistersToDouble(du.value(2), du.value(3));
        aep.phaseVoltage[1] = RegistersToDouble(du.value(4), du.value(5));
        aep.phaseVoltage[2] = RegistersToDouble(du.value(6), du.value(7));
        aep.avgVoltage = RegistersToDouble(du.value(8), du.value(9));

        aep.phaseCurrent[0] = RegistersToDouble(du.value(18), du.value(19));
        aep.phaseCurrent[1] = RegistersToDouble(du.value(20), du.value(21));
        aep.phaseCurrent[2] = RegistersToDouble(du.value(22), du.value(23));
        aep.avgCurrent = RegistersToDouble(du.value(24), du.value(25));

        aep.phaseLPower[0] = RegistersToDouble(du.value(28), du.value(29));
        aep.phaseLPower[1] = RegistersToDouble(du.value(30), du.value(31));
        aep.phaseLPower[2] = RegistersToDouble(du.value(32), du.value(33));
        aep.systemPower = RegistersToDouble(du.value(34), du.value(35));
    } else {
        aep.freq = du.value(0)/ 100.0;

        aep.phaseVoltage[0] = du.value(1) / 10.0;
        aep.phaseVoltage[1] = du.value(2) / 10.0;
        aep.phaseVoltage[2] = du.value(3) / 10.0;
        aep.avgVoltage =    du.value(4) / 10.0;

        aep.phaseCurrent[0] = du.value(9) / 1000.0;
        aep.phaseCurrent[1] = du.value(10) / 1000.0;
        aep.phaseCurrent[2] = du.value(11) / 1000.0;
        aep.avgCurrent = du.value(12) / 1000.0;

        aep.phaseLPower[0] = du.value(14) / 5.0;
        aep.phaseLPower[1] = du.value(15) / 5.0;
        aep.phaseLPower[2] = du.value(16) / 5.0;
        aep.systemPower = du.value(17) / 5.0;
    }

    qDebug() << du.valueCount() << du.value(0) << du.value(1) << endl
             << "Phase Voltage: " << aep.phaseVoltage[0]
                                  << aep.phaseVoltage[1]
                                  << aep.phaseVoltage[2]
                                  << aep.avgVoltage << endl
             << "Current: " << aep.phaseCurrent[0]
                            << aep.phaseCurrent[1]
                            << aep.phaseCurrent[2] << endl
             << "PhaseL Power: " << aep.phaseLPower[0]
                                 << aep.phaseLPower[1]
                                 << aep.phaseLPower[2] << endl
             << "System power: " << aep.systemPower << endl
             << "Freq: " << aep.freq;
    //emit dataReady(serverAddress, aep);
}

void DEIFModbus::RegsToEm(QModbusDataUnit du)
{
    if (isMK2) {
        aep.energyTotal = RegistersToDWord(du.value(16), du.value(17));
        qDebug() << du.valueCount()
                 <<  du.value(16) << du.value(17)
                 << du.value(18) << du.value(19)
                 << "Total Energy: " << aep.energyTotal;
    } else {
        aep.energyTotal = RegistersToDWord(du.value(8), du.value(9)) / 10.0;
        qDebug() << du.valueCount()
                 << du.value(8) << du.value(9)
                 << "Total Energy: " << aep.energyTotal;
    }
    emit dataReady(serverAddress, aep);
}

double DEIFModbus::ByteArrayToDouble(QByteArray ba, double defaultValue = 0.0)
{
    float value = static_cast<float>(defaultValue);
    ba.resize(4);
    ba[3] = 0x42;
    ba[2] = 0x48;
    ba[1] = 0x0A;
    ba[0] = 0x3D;

    if ( ba.size() >= sizeof(value) ) {
        //value = *reinterpret_cast<float*>(ba.data());
        value = *reinterpret_cast<const float*>(ba.data());
    } else {
        // The array is not big enough.
    }
    qDebug() << value;
    return static_cast<double>(value);
}

//=======Getters and Setters===================================================

int DEIFModbus::getServerAddress() const
{
    return serverAddress;
}

void DEIFModbus::setServerAddress(int value)
{
    serverAddress = value;
}

bool DEIFModbus::getIsMK2() const
{
    return isMK2;
}

void DEIFModbus::setIsMK2(bool value)
{
    isMK2 = value;
}

UniversalAEParams DEIFModbus::getAep() const
{
    return aep;
}

void DEIFModbus::setAep(const UniversalAEParams &value)
{
    aep = value;
}

//=======================================================================

/*
QModbusDataUnit DEIFModbus::readRequest(int regType, int startAddress,
                                        int numberOfEntries) const
{
    const auto table =
        static_cast<QModbusDataUnit::RegisterType>(regType);
    return QModbusDataUnit(table, startAddress, numberOfEntries);
}

QModbusDataUnit DEIFModbus::writeRequest(int regType, int startAddress,
                                         int numberOfEntries) const
{
    const auto table =
        static_cast<QModbusDataUnit::RegisterType>(regType);
    return QModbusDataUnit(table, startAddress, numberOfEntries);
}
*/
