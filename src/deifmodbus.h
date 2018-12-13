#ifndef DEIFMODBUS_H
#define DEIFMODBUS_H

#include <QObject>
#include <QModbusClient>

struct AnalogParams {
    double freq;

    double phaseVoltage1;
    double phaseVoltage2;
    double phaseVoltage3;
    double avgVoltage;
    double lineVoltage1;
    double lineVoltage2;
    double lineVoltage3;
    double avgLineVoltage;

    double phaseCurrent1;
    double phaseCurrent2;
    double phaseCurrent3;
    double avgCurrent;
    double neutralCurrent;

    double phaseL1Power;
    double phaseL2Power;
    double phaseL3Power;
    double systemPower;
    double phaseL1ReactPower;
    double phaseL2ReactPower;
    double phaseL3ReactPower;
    double systemReactPower;
    double phaseL1ApptPower;
    double phaseL2ApptPower;
    double phaseL3ApptPower;
    double systemApptPower;

    double phaseL1PF;
    double phaseL2PF;
    double phaseL3PF;
    double systemPF;

    double voltUnbalFact;
    double currUnbalFact;
    double loadCharacter;
    double powerDemand;
    double reactPowerDemand;
    double apptPowerDemand;
};

const int SYSTEM_PARAMS_BASE_ADDRESS = 0x1000;
const int DATE_TIME_BASE_ADDRESS = 0x1040;
const int LIMIT_ALARM_BASE_ADDRESS = 0x1046;
const int METER_PARAM_BASE_ADDRESS = 0x3000;
const int BASIC_ANALOG_BASE_ADDRESS = 0x4000;
const int RT_ENERGY_BASE_ADDRESS = 0x4048;
const int HARMONICS_BASE_ADDRESS = 0x405A;
const int MIN_MAX_BASE_ADDRESS = 0x4136;

class DEIFModbus : public QObject
{
    Q_OBJECT
public:
    explicit DEIFModbus(QObject *parent = nullptr);

    AnalogParams getAp() const;
    void RegsToAp(QModbusDataUnit du);

signals:
    void dataReady();

public slots:

private:
    AnalogParams ap;
    double RegistersToDouble(quint16 highWord, quint16 lowWord);
};

#endif // DEIFMODBUS_H
