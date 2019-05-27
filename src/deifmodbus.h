#ifndef DEIFMODBUS_H
#define DEIFMODBUS_H

#include <QObject>
#include <QModbusClient>
#include <QTimer>

// DEIF is at comport (config button)
// 0x3000 = 12288 dec
// 0x4000 = 16384 dec
// 0x4048 = 16456 dec

struct SysParams {  //@address 0x3000
    uint16_t passwd;
    uint16_t mbAddress;
    uint16_t mbAaddress;
    uint16_t baudRate;
    uint16_t voltWiring;
    uint16_t currWiring;
    uint16_t lcdTime;
};

struct SysStatus {  //@address 0x3000
    uint16_t passwd;
    uint16_t mbAddress;
    uint16_t mbAaddress;
    uint16_t baudRate;
    uint16_t voltWiring;
    uint16_t currWiring;
    uint16_t lcdTime;
};

struct MeterParams {  //@address 0x3000
    double freq;
    double phaseVoltage1;
    double phaseVoltage2;
    double phaseVoltage3;
    double avgVoltage;
    //double lineVoltage1;
    //double lineVoltage2;
    //double lineVoltage3;
    //double avgLineVoltage;

    double current1;
    double current2;
    double current3;
    //double avgCurrent;
    //double neutralCurrent;

    double phaseAPower;
    double phaseBPower;
    double phaseCPower;
    double systemPower;
    //double phaseAReactPower;
    //double phaseBReactPower;
    //double phaseCReactPower;
    //double systemReactPower;
    //double phaseAApptPower;
    //double phaseBApptPower;
    //double phaseCApptPower;
    //double systemApptPower;

    //double phaseAPF;
    //double phaseBPF;
    //double phaseCPF;
    //double systemPF;
};

struct AnalogParams {  //@address 0x4000
    double freq;

    double phaseVoltage1;
    double phaseVoltage2;
    double phaseVoltage3;
    double avgVoltage;
    //double lineVoltage1;
    //double lineVoltage2;
    //double lineVoltage3;
    //double avgLineVoltage;

    double phaseCurrent1;
    double phaseCurrent2;
    double phaseCurrent3;
    //double avgCurrent;
    //double neutralCurrent;

    double phaseL1Power;
    double phaseL2Power;
    double phaseL3Power;
    double systemPower;
    //double phaseL1ReactPower;
    //double phaseL2ReactPower;
    //double phaseL3ReactPower;
    //double systemReactPower;
    //double phaseL1ApptPower;
    //double phaseL2ApptPower;
    //double phaseL3ApptPower;
    //double systemApptPower;

    //double phaseL1PF;
    //double phaseL2PF;
    //double phaseL3PF;
    //double systemPF;

    //double voltUnbalFact;
    //double currUnbalFact;
    //double loadCharacter;
    //double powerDemand;
    //double reactPowerDemand;
    //double apptPowerDemand;
};

struct EnergyMeasurement {  //@address 0x4048
    //Energy IMP F4/F7 0~999999999 dword R/W
    //Energy EXP F4/F7 0~999999999 dword R/W
    //Reactive energy IMP F5/F8 0~999999999 dword R/W
    //Reactive energy EXP F5/F8 0~999999999 dword R/W
    double energyTotal;      //F4/F7 0~999999999 dword R/W
    //Energy NET F4/F7 0~999999999 dword R/W
   // Reactive energy TOTAL F5/F8 0~999999999 dword R/W
   // Reactive energy NET F5/F8 0~999999999 dword R/W
   // Apparent energy F6/F9 0~999999999 dword R/W
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
    ~DEIFModbus();

    AnalogParams getAp() const;
    void RegsToAp(QModbusDataUnit du);

    void readAll();


signals:
    void dataReady();

public slots:

private:
    QTimer* deifTimer;
    QModbusReply* lastRequest;
    QModbusClient* modbusDevice;

    AnalogParams ap;

    QModbusDataUnit readRequest() const;
    QModbusDataUnit writeRequest() const;
    QModbusDataUnit DEIFReadRequest(int startAddress, quint16 numEntries) const;

    double ByteArrayToDouble(QByteArray ba, double defaultValue);
    double RegistersToDouble(quint16 highWord, quint16 lowWord);
};

#endif // DEIFMODBUS_H
