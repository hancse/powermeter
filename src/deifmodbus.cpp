#include "deifmodbus.h"

#include <QDebug>

DEIFModbus::DEIFModbus(QObject *parent) : QObject(parent)
{
    deifTimer = new QTimer(this);
    //connect(deifTimer, &QTimer::timeout, this, &DEIFModbus::readAll);
}

DEIFModbus::~DEIFModbus()
{

}

QModbusDataUnit DEIFModbus::DEIFReadRequest(int startAddress, quint16 numEntries) const
{
    //const auto table =
        //static_cast<QModbusDataUnit::RegisterType>(ui->writeTable->currentData().toInt());

    return QModbusDataUnit(QModbusDataUnit::HoldingRegisters, startAddress, numEntries);
}

double DEIFModbus::RegistersToDouble(quint16 highWord, quint16 lowWord)
{
    quint32 lw = (highWord << 16) + lowWord;
    float value;
    value = *reinterpret_cast<float*>(&lw);
    qDebug() << value;
    return static_cast<double>(value);
}

void DEIFModbus::readAll()
{

}

AnalogParams DEIFModbus::getAp() const
{
    return ap;
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

    qDebug() << du.valueCount() << du.value(0) << du.value(1) << ap.freq
             << ap.phaseVoltage1 << ap.phaseVoltage2 << ap.phaseVoltage3 << ap.avgVoltage;
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
