#include "deifmodbus.h"

#include <QDebug>

DEIFModbus::DEIFModbus(QObject *parent) :
    QObject(parent),
    lastRequest(nullptr),
    modbusDevice(nullptr)
{
    deifTimer = new QTimer(this);
    //connect(deifTimer, &QTimer::timeout, this, &DEIFModbus::readAll);
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
        case METER_PARAM_BASE_ADDRESS:
            RegsToAp(unit);
            break;
        case RT_ENERGY_BASE_ADDRESS:
            RegsToEm(unit);
            break;
        default:
            break;
        }

        //this->RegsToAp(unit);

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

void DEIFModbus::RegsToAp(QModbusDataUnit du)
{
    ap.freq = RegistersToDouble(du.value(0), du.value(1));

    ap.phaseVoltage1 = RegistersToDouble(du.value(2), du.value(3));
    ap.phaseVoltage2 = RegistersToDouble(du.value(4), du.value(5));
    ap.phaseVoltage3 = RegistersToDouble(du.value(6), du.value(7));
    ap.avgVoltage = RegistersToDouble(du.value(8), du.value(9));

    ap.phaseCurrent1 = RegistersToDouble(du.value(18), du.value(19));
    ap.phaseCurrent2 = RegistersToDouble(du.value(20), du.value(21));
    ap.phaseCurrent3 = RegistersToDouble(du.value(22), du.value(23));
    //ap.avgCurrent = RegistersToDouble(du.value(24), du.value(25));

    ap.phaseL1Power = RegistersToDouble(du.value(28), du.value(29));
    ap.phaseL2Power = RegistersToDouble(du.value(30), du.value(31));
    ap.phaseL3Power = RegistersToDouble(du.value(32), du.value(33));
    ap.systemPower = RegistersToDouble(du.value(34), du.value(35));

    qDebug() << du.valueCount() << du.value(0) << du.value(1) << endl
             << "Phase Voltage: " << ap.phaseVoltage1 << ap.phaseVoltage2 << ap.phaseVoltage3 << ap.avgVoltage << endl
             << "Current: " << ap.phaseCurrent1 << ap.phaseCurrent2 << ap.phaseCurrent3 << endl
             << "PhaseL Power: " << ap.phaseL1Power << ap.phaseL2Power << ap.phaseL3Power << endl
             << "System power: " << ap.systemPower << endl
             << "Freq: " << ap.freq;
    emit dataReady();
}

void DEIFModbus::RegsToEm(QModbusDataUnit du)
{
    em.energyTotal = RegistersToDWord(du.value(16), du.value(17));


    qDebug() << du.valueCount()
             <<  du.value(16) << du.value(17)
             << du.value(18) << du.value(19)
             << "Total Energy: " << em.energyTotal;

    emit dataReady();
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

AnalogParams DEIFModbus::getAp() const
{
    return ap;
}

int DEIFModbus::getServerAddress() const
{
    return serverAddress;
}

void DEIFModbus::setServerAddress(int value)
{
    serverAddress = value;
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
