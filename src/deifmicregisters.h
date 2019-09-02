#ifndef DEIFMICREGISTERS_H
#define DEIFMICREGISTERS_H

#include <QModbusClient>

// DEIF is at comport (config button)
// 0x3000 = 12288 dec
// 0x4000 = 16384 dec
// 0x4048 = 16456 dec

struct MicSysParams {         // @address 0x0100 read 20 write 21
    uint16_t accesscode;      // offset 0 read/write (03/16)
    uint16_t mbAddress;
    uint16_t baudRate;
    uint16_t voltWiring;
    uint16_t currWiring;
    uint16_t pt1high;
    uint16_t pt1low;
    uint16_t pt2;
    uint16_t ct1;
    uint16_t DOtype;
    uint16_t evn1;
    uint16_t evn2;
    uint16_t pulsewidth;
    uint16_t pulserate;
    uint16_t relay1mode;
    uint16_t relay1width;
    uint16_t relay2mode;
    uint16_t relay2width;
    uint16_t lighttime;
    uint16_t windowtime;     // offset 19 (13h) read/write (03/16)
    uint16_t clearminmax;    // offset 20 (14h) write only (16)
};

struct MicClockParams {  // @address 0x032A read 6 write 6
    uint16_t year;       // offset 0 read/write (03/16)
    uint16_t mon;
    uint16_t day;
    uint16_t hour;
    uint16_t min;
    uint16_t sec;        // offset 5 (06h) read/write (03/16)
};

struct MicRunningParams {  // @address 0x03A5 read 2
    uint16_t runhigh;      // offset 0 read (03)
    uint16_t runlow;       // offset 1 (01h) read (03)
};

struct MicAnalogParams {  //@address 0x0130 read 36
    double freq;          // offset 0

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
    double apptPowerDemand;   // offset 35
};

struct MicEnergyMeasurement {  //@address 0x4048
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

// MIC Modbus registers
const int MIC_SYSTEM_PARAMS_BASE_ADDRESS = 0x0100;
const int MIC_CLOCK_PARAMS_BASE_ADDRESS  = 0x032A;
const int MIC_RUNNING_BASE_ADDRESS = 0x03A5;
const int MIC_ANALOG_BASE_ADDRESS = 0x0130;
const int MIC_ENERGY_BASE_ADDRESS = 0x0156;

//const int LIMIT_ALARM_BASE_ADDRESS = 0x1046;
//const int HARMONICS_BASE_ADDRESS = 0x405A;
//const int MIN_MAX_BASE_ADDRESS = 0x4136;

#endif // DEIFMICREGISTERS_H
