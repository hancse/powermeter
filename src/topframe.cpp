#include "topframe.h"
#include "ui_topframe.h"

#include <QDebug>

TopFrame::TopFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TopFrame)
{
    ui->setupUi(this);
    //strpf = new StripFrameTriple(this);
    //strpf->move(450,0);
}

TopFrame::~TopFrame()
{
    delete ui;
}


void TopFrame::displayGridData(UniversalAEParams ae)
{
    qDebug() << "Grid";
    ui->lblFreqGrid->setText(QString("%1 Hz").arg(ae.freq, 0, 'f', 1));
    ui->lblVoltGrid->setText(QString("%1 V").arg(ae.avgVoltage, 0, 'f', 1));
    ui->lblCurrGrid->setText(QString("%1 A").arg(ae.avgCurrent, 0, 'f', 1));
    ui->lblPowerGrid->setText(QString("%1 W").arg(ae.systemPower, 0, 'f', 1));
    ui->lblTotalEnergyGrid->setText(QString("%1 kWh").arg(ae.energyTotal, 0, 'f', 1));
}

void TopFrame::displayPVData(UniversalAEParams ae)
{
    qDebug() << "PV display";
    ui->lblFreqPV->setText(QString("%1 Hz").arg(ae.freq, 0, 'f', 1));
    ui->lblVoltPV->setText(QString("%1 V").arg(ae.avgVoltage, 0, 'f', 1));
    ui->lblCurrPV->setText(QString("%1 A").arg(ae.avgCurrent, 0, 'f', 1));
    ui->lblPowerPV->setText(QString("%1 W").arg(ae.systemPower, 0, 'f', 1));
    ui->lblTotalEnergyPV->setText(QString("%1 kWh").arg(ae.energyTotal, 0, 'f', 1));
}

void TopFrame::displayBatteryData(UniversalAEParams ae)
{
    qDebug() << "Battery";
    ui->lblFreqBatt->setText(QString("%1 Hz").arg(ae.freq, 0, 'f', 1));
    ui->lblVoltBatt->setText(QString("%1 V").arg(ae.avgVoltage, 0, 'f', 1));
    ui->lblCurrBat->setText(QString("%1 A").arg(ae.avgCurrent, 0, 'f', 1));
    ui->lblPowerBat->setText(QString("%1 W").arg(ae.systemPower, 0, 'f', 1));
    ui->lblTotalEnergyBatt->setText(QString("%1 kWh").arg(ae.energyTotal, 0, 'f', 1));
}

void TopFrame::displayLoadData(UniversalAEParams ae)
{
    qDebug() << "Load";
    ui->lblFreqLoad->setText(QString("%1 Hz").arg(ae.freq, 0, 'f', 1));
    ui->lblVoltLoad->setText(QString("%1 V").arg(ae.avgVoltage, 0, 'f', 1));
    ui->lblCurrLoad->setText(QString("%1 A").arg(ae.avgCurrent, 0, 'f', 1));
    ui->lblPowerLoad->setText(QString("%1 W").arg(ae.systemPower, 0, 'f', 1));
    ui->lblTotalEnergyLoad->setText(QString("%1 kWh").arg(ae.energyTotal, 0, 'f', 1));
}

