#include "deifmodbus.h"

#include <QDebug>

DEIFModbus::DEIFModbus(QObject *parent) :
    QObject(parent),
    lastRequest(nullptr),
    modbusDevice(nullptr)
{
    deifTimer = new QTimer(this);
    //connect(deifTimer, &QTimer::timeout, this, &DEIFModbus::readAll);
    connect(deifTimer, &QTimer::timeout, this, &DEIFModbus::readDummyAll);
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
            RegsToMicAp(unit);
            break;
        case MIC_ENERGY_BASE_ADDRESS:
            RegsToMicEm(unit);
            break;
        case MK2_ANALOG_BASE_ADDRESS:
            RegsToMk2Ap(unit);
            break;
        case MK2_ENERGY_BASE_ADDRESS:
            RegsToMk2Em(unit);
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

void DEIFModbus::readDummyAll()
{
    qDebug() << "read dummy data";
    if ( isMK2) {
        mk2ap.freq = 50.0;

        mk2ap.phaseVoltage1 = 230.0;
        mk2ap.phaseVoltage2 = 230.0;
        mk2ap.phaseVoltage3 = 230.0;
        mk2ap.avgVoltage =    230.0;

        mk2ap.phaseCurrent1 = 20.0;
        mk2ap.phaseCurrent2 = 20.0;
        mk2ap.phaseCurrent3 = 20.0;
        //mk2ap.avgCurrent = 20.0;

        mk2ap.phaseL1Power = 5.0;
        mk2ap.phaseL2Power = 5.0;
        mk2ap.phaseL3Power = 5.0;
        mk2ap.systemPower = 5.0;

        mk2em.energyTotal = 100.0;
    } else {
        micap.freq = 50.0;

        micap.phaseVoltage1 = 230.0;
        micap.phaseVoltage2 = 230.0;
        micap.phaseVoltage3 = 230.0;
        micap.avgVoltage =    230.0;

        micap.phaseCurrent1 = 20.0;
        micap.phaseCurrent2 = 20.0;
        micap.phaseCurrent3 = 20.0;
        micap.avgCurrent = 20.0;

        micap.phaseL1Power = 5.0;
        micap.phaseL2Power = 5.0;
        micap.phaseL3Power = 5.0;
        micap.systemPower = 5.0;

        micem.energyTotal = 100.0;
    }
    emit dataReady();
}

void DEIFModbus::RegsToMicAp(QModbusDataUnit du)
{
    micap.freq = du.value(0)/ 100.0;

    micap.phaseVoltage1 = du.value(1) / 10.0;
    micap.phaseVoltage2 = du.value(2) / 10.0;
    micap.phaseVoltage3 = du.value(3) / 10.0;
    micap.avgVoltage =    du.value(4) / 10.0;

    micap.phaseCurrent1 = du.value(9) / 1000.0;
    micap.phaseCurrent2 = du.value(10) / 1000.0;
    micap.phaseCurrent3 = du.value(11) / 1000.0;
    micap.avgCurrent = du.value(12) / 1000.0;

    micap.phaseL1Power = du.value(14) / 5.0;
    micap.phaseL2Power = du.value(15) / 5.0;
    micap.phaseL3Power = du.value(16) / 5.0;
    micap.systemPower = du.value(17) / 5.0;

    qDebug() << du.valueCount() << du.value(0) << du.value(1) << endl
             << "Phase Voltage: " << micap.phaseVoltage1
                                  << micap.phaseVoltage2
                                  << micap.phaseVoltage3
                                  << micap.avgVoltage << endl
             << "Current: " << micap.phaseCurrent1
                            << micap.phaseCurrent2
                            << micap.phaseCurrent3 << endl
             << "PhaseL Power: " << micap.phaseL1Power
                                 << micap.phaseL2Power
                                 << micap.phaseL3Power << endl
             << "System power: " << micap.systemPower << endl
             << "Freq: " << micap.freq;
    emit dataReady();
}

void DEIFModbus::RegsToMicEm(QModbusDataUnit du)
{
    micem.energyTotal = RegistersToDWord(du.value(8), du.value(9)) / 10.0;

    qDebug() << du.valueCount()
             << du.value(8) << du.value(9)
             << "Total Energy: " << micem.energyTotal;

    emit dataReady();
}

void DEIFModbus::RegsToMk2Ap(QModbusDataUnit du)
{
    mk2ap.freq = RegistersToDouble(du.value(0), du.value(1));

    mk2ap.phaseVoltage1 = RegistersToDouble(du.value(2), du.value(3));
    mk2ap.phaseVoltage2 = RegistersToDouble(du.value(4), du.value(5));
    mk2ap.phaseVoltage3 = RegistersToDouble(du.value(6), du.value(7));
    mk2ap.avgVoltage = RegistersToDouble(du.value(8), du.value(9));

    mk2ap.phaseCurrent1 = RegistersToDouble(du.value(18), du.value(19));
    mk2ap.phaseCurrent2 = RegistersToDouble(du.value(20), du.value(21));
    mk2ap.phaseCurrent3 = RegistersToDouble(du.value(22), du.value(23));
    //ap.avgCurrent = RegistersToDouble(du.value(24), du.value(25));

    mk2ap.phaseL1Power = RegistersToDouble(du.value(28), du.value(29));
    mk2ap.phaseL2Power = RegistersToDouble(du.value(30), du.value(31));
    mk2ap.phaseL3Power = RegistersToDouble(du.value(32), du.value(33));
    mk2ap.systemPower = RegistersToDouble(du.value(34), du.value(35));

    qDebug() << du.valueCount() << du.value(0) << du.value(1) << endl
             << "Phase Voltage: " << mk2ap.phaseVoltage1 << mk2ap.phaseVoltage2 << mk2ap.phaseVoltage3 << mk2ap.avgVoltage << endl
             << "Current: " << mk2ap.phaseCurrent1 << mk2ap.phaseCurrent2 << mk2ap.phaseCurrent3 << endl
             << "PhaseL Power: " << mk2ap.phaseL1Power << mk2ap.phaseL2Power << mk2ap.phaseL3Power << endl
             << "System power: " << mk2ap.systemPower << endl
             << "Freq: " << mk2ap.freq;
    emit dataReady();
}

void DEIFModbus::RegsToMk2Em(QModbusDataUnit du)
{
    mk2em.energyTotal = RegistersToDWord(du.value(16), du.value(17));

    qDebug() << du.valueCount()
             <<  du.value(16) << du.value(17)
             << du.value(18) << du.value(19)
             << "Total Energy: " << mk2em.energyTotal;

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

Mk2AnalogParams DEIFModbus::getMk2Ap() const
{
    return mk2ap;
}

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

MicAnalogParams DEIFModbus::getMicap() const
{
    return micap;
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
