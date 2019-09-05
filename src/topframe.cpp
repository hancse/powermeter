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
    ui->lblFreqGrid->setText(QString("%1 Hz").arg(ae.freq));
    ui->lblVoltGrid->setText(QString("%1 V").arg(ae.avgVoltage));
    ui->lblCurrGrid->setText(QString("%1 A").arg(ae.avgCurrent));
    ui->lblPowerGrid->setText(QString("%1 W").arg(ae.systemPower));
    ui->lblTotalEnergyGrid->setText(QString("%1 kWh").arg(ae.energyTotal));
}

void TopFrame::displayPVData(UniversalAEParams ae)
{
    qDebug() << "PV";
    ui->lblFreqPV->setText(QString("%1 Hz").arg(ae.freq));
    ui->lblVoltPV->setText(QString("%1 V").arg(ae.avgVoltage));
    ui->lblCurrPV->setText(QString("%1 A").arg(ae.avgCurrent));
    ui->lblPowerPV->setText(QString("%1 W").arg(ae.systemPower));
    ui->lblTotalEnergyPV->setText(QString("%1 kWh").arg(ae.energyTotal));
}

void TopFrame::displayBatteryData(UniversalAEParams ae)
{
    qDebug() << "Battery";
    ui->lblFreqBatt->setText(QString("%1 Hz").arg(ae.freq));
    ui->lblVoltBatt->setText(QString("%1 V").arg(ae.avgVoltage));
    ui->lblCurrBat->setText(QString("%1 A").arg(ae.avgCurrent));
    ui->lblPowerBat->setText(QString("%1 W").arg(ae.systemPower));
    ui->lblTotalEnergyBatt->setText(QString("%1 kWh").arg(ae.energyTotal));
}

void TopFrame::displayLoadData(UniversalAEParams ae)
{
    qDebug() << "Load";
    ui->lblFreqLoad->setText(QString("%1 Hz").arg(ae.freq));
    ui->lblVoltLoad->setText(QString("%1 V").arg(ae.avgVoltage));
    ui->lblCurrLoad->setText(QString("%1 A").arg(ae.avgCurrent));
    ui->lblPowerLoad->setText(QString("%1 W").arg(ae.systemPower));
    ui->lblTotalEnergyLoad->setText(QString("%1 kWh").arg(ae.energyTotal));
}
