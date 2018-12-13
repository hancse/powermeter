#include "deifmodbus.h"

#include <QDebug>

DEIFModbus::DEIFModbus(QObject *parent) : QObject(parent)
{

}

double DEIFModbus::RegistersToDouble(quint16 highWord, quint16 lowWord)
{
    quint32 lw = (highWord << 16) + lowWord;
    float value;
    value = *reinterpret_cast<float*>(&lw);
    qDebug() << value;
    return static_cast<double>(value);
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
/*
    ap.lineVoltage1 = RegistersToDouble(du.value(10), du.value(11));
    ap.lineVoltage2 = RegistersToDouble(du.value(12), du.value(13));
    ap.lineVoltage3 = RegistersToDouble(du.value(14), du.value(15));
    ap.avgLineVoltage = RegistersToDouble(du.value(16), du.value(17));

    ap.phaseCurrent1 = RegistersToDouble(du.value(18), du.value(19));
    ap.phaseCurrent2 = RegistersToDouble(du.value(20), du.value(21));
    ap.phaseCurrent3 = RegistersToDouble(du.value(22), du.value(23));
    ap.avgCurrent = RegistersToDouble(du.value(24), du.value(25));

    ap.phaseL1Power = RegistersToDouble(du.value(26), du.value(27));
    ap.phaseL2Power = RegistersToDouble(du.value(28), du.value(29));
    ap.phaseL3Power = RegistersToDouble(du.value(30), du.value(31));
    ap.systemPower = RegistersToDouble(du.value(32), du.value(33));
*/
    qDebug() << du.valueCount() << du.value(0) << du.value(1) << ap.freq
             << ap.phaseVoltage1 << ap.phaseVoltage2 << ap.phaseVoltage3 << ap.avgVoltage;
    emit dataReady();
}
