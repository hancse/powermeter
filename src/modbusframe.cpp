#include "modbusframe.h"
#include "ui_modbusframe.h"

#include <QModbusTcpClient>
#include <QModbusRtuSerialMaster>
#include <QUrl>
#include <QDebug>
#include <QVector>

enum ModbusConnection {
    Serial,
    Tcp
};

ModbusFrame::ModbusFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ModbusFrame)
{
    ui->setupUi(this);
    strpf = new StripFrameTriple(this);
    strpf->move(550,0);

    deif = new DEIFModbus(this);

    //connect(deif->deifTimer, &QTimer::timeout,
         //   this, &ModbusFrame::on_readButton_clicked);

    //connect(deif, &DEIFModbus::dataReady,
     //      this, &ModbusFrame::displayData);

    sd = new SerialDialog(this);
    tcpd = new TcpDialog(this);

    ui->writeTable->addItem(tr("Coils"), QModbusDataUnit::Coils);
    ui->writeTable->addItem(tr("Discrete Inputs"), QModbusDataUnit::DiscreteInputs);
    ui->writeTable->addItem(tr("Input Registers"), QModbusDataUnit::InputRegisters);
    ui->writeTable->addItem(tr("Holding Registers"), QModbusDataUnit::HoldingRegisters);
    ui->writeTable->setCurrentIndex(3);

    ui->connectType->setCurrentIndex(0);
    on_connectType_currentIndexChanged(0);

    setupConnections();

}

ModbusFrame::~ModbusFrame()
{
    delete ui;
}

/**
 * @brief setup connections with child SIGNALS
 *
 * connect SIGNAL DEIFModbus::deifTimer::timeout to SLOT on_readButton_clicked
 * connect SIGNAL DEIFModbus::dataReady to SLOT displayData
 */
void ModbusFrame::setupConnections()
{
    //connect(ui->serverEdit, &QSpinBox::valueChanged,
      //      this, &ModbusFrame::setMbAddress);

    connect(deif->deifTimer, &QTimer::timeout,
            this, &ModbusFrame::on_readButton_clicked);

    connect(deif, &DEIFModbus::dataReady,
            this, &ModbusFrame::displayData);
}

void ModbusFrame::setMbAddress(int addr)
{
    ui->serverEdit->setValue(addr);
    //deif->setServerAddress(addr);
}

void ModbusFrame::displayData(int addr, UniversalAEParams ae)
{
    //UniversalAEParams ae = deif->getAep();
    ui->lblFreq->setText(QString("%1 Hz").arg(ae.freq, 0, 'f', 1));
    ui->lblV1->setText(QString("%1 V").arg(ae.phaseVoltage[0], 0, 'f', 1));
    ui->lblV2->setText(QString("%1 V").arg(ae.phaseVoltage[1], 0, 'f', 1));
    ui->lblV3->setText(QString("%1 V").arg(ae.phaseVoltage[2], 0, 'f', 1));
    ui->lblCurr1->setText(QString("%1 A").arg(ae.phaseCurrent[0], 0, 'f', 1));
    ui->lblCurr2->setText(QString("%1 A").arg(ae.phaseCurrent[1], 0, 'f', 1));
    ui->lblCurr3->setText(QString("%1 A").arg(ae.phaseCurrent[2], 0, 'f', 1));
    ui->lblPower1->setText(QString("%1 W").arg(ae.phaseLPower[0], 0, 'f', 1));
    ui->lblPower2->setText(QString("%1 W").arg(ae.phaseLPower[1], 0, 'f', 1));
    ui->lblPower3->setText(QString("%1 W").arg(ae.phaseLPower[2], 0, 'f', 1));
    ui->lblTotalEnergy->setText(QString("%1 kWh").arg(ae.energyTotal, 0, 'f', 1));

    //QDateTime dt = QDateTime::currentDateTime();

    QVector<double> yd;
    yd.push_back(ae.phaseVoltage[0]);
    yd.push_back(ae.phaseVoltage[1]);
    yd.push_back(ae.phaseVoltage[2]);
    yd.push_back(ae.phaseCurrent[0]);
    yd.push_back(ae.phaseCurrent[1]);
    yd.push_back(ae.phaseCurrent[2]);
    yd.push_back(ae.phaseLPower[0]);
    yd.push_back(ae.phaseLPower[1]);
    yd.push_back(ae.phaseLPower[2]);
    strpf->realTimeAllSlot(ae.dt, yd, true);
}

void ModbusFrame::connectRTU()
{
    SerialDialog::PortParameters p = sd->getCp();
    ui->portEdit->setText(p.name);
    deif->modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
                                            ui->portEdit->text());
    deif->modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,
                                            p.parity);
    deif->modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
                                            p.baudRate);
    deif->modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
                                            p.dataBits);
    deif->modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
                                            p.stopBits);
    deif->modbusDevice->setTimeout(p.responseTime);
    deif->modbusDevice->setNumberOfRetries(p.numberOfRetries);
}

void ModbusFrame::connectTCP()
{
    TcpDialog::PortParameters u = tcpd->getUp();
    //const QUrl url = QUrl::fromUserInput(ui->portEdit->text());
    deif->modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, u.url.port());
    deif->modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, u.url.host());
    deif->modbusDevice->setTimeout(u.responseTime);
    deif->modbusDevice->setNumberOfRetries(u.numberOfRetries);
}

void ModbusFrame::on_btnConnect_clicked()
{
    if (!deif->modbusDevice)  // no device defined, nothing to connect or disconnect
        return;

    ui->lblStatus->clear();
    if (deif->modbusDevice->state() != QModbusDevice::ConnectedState) {
        if (static_cast<ModbusConnection> (ui->connectType->currentIndex()) == Serial) {
            connectRTU();
        } else {
            connectTCP();
        }

        if (!deif->modbusDevice->connectDevice()) {
            ui->lblStatus->setText(tr("Connect failed: ") + deif->modbusDevice->errorString());
        } else {
            ui->btnConnect->setText("Disconnect");
        }
    } else {
        deif->modbusDevice->disconnectDevice();
        ui->btnConnect->setText("Connect");
    }
}

void ModbusFrame::on_btnConfig_clicked()
{
    auto type = static_cast<ModbusConnection>(ui->connectType->currentIndex());
    if (type == Serial) {
        sd->setModal(true);
        sd->show();
    } else if (type == Tcp) {
        tcpd->setModal(true);
        tcpd->show();
    }
}

void ModbusFrame::on_connectType_currentIndexChanged(int index)
{
    if (deif->modbusDevice) {
        deif->modbusDevice->disconnectDevice();
        delete deif->modbusDevice;
        deif->modbusDevice = nullptr;
    }

    auto type = static_cast<ModbusConnection>(index);
    if (type == Serial) {
        deif->modbusDevice = new QModbusRtuSerialMaster(this);
    } else if (type == Tcp) {
        deif->modbusDevice = new QModbusTcpClient(this);
        if (ui->portEdit->text().isEmpty())
            ui->portEdit->setText(QLatin1Literal("127.0.0.1:502"));
    }

    connect(deif->modbusDevice, &QModbusClient::errorOccurred, [this](QModbusDevice::Error) {
        ui->lblStatus->setText(deif->modbusDevice->errorString());
    });

    if (!deif->modbusDevice) {
        if (type == Serial)
            ui->lblStatus->setText(tr("Could not create Modbus master."));
        else
            ui->lblStatus->setText(tr("Could not create Modbus client."));
    } else {
        connect(deif->modbusDevice, &QModbusClient::stateChanged,
                this, &ModbusFrame::onStateChanged);
    }
}

void ModbusFrame::onStateChanged(int state)
{
    if (state == QModbusDevice::UnconnectedState)
        ui->btnConnect->setText(tr("Connect"));
    else if (state == QModbusDevice::ConnectedState)
        ui->btnConnect->setText(tr("Disconnect"));
}

void ModbusFrame::on_readButton_clicked()
{
    //readDEIF();
    if ( deif->getIsMK2() ) {
        deif->readDEIF(ui->writeTable->currentData().toInt(),
                       ui->serverEdit->value(),
                       MK2_ANALOG_BASE_ADDRESS,
                       18);
        //deif->readDEIF(deif->getServerAddress(), ui->writeTable->currentData().toInt());
    } else {
        deif->readDEIF(ui->writeTable->currentData().toInt(),
                       ui->serverEdit->value(),
                       MIC_ANALOG_BASE_ADDRESS,
                       18);
    }

}

void ModbusFrame::on_writeButton_clicked()
{
    if (!deif->modbusDevice)
        return;
    ui->lblStatus->clear();
/*
    QModbusDataUnit writeUnit = writeRequest(ui->writeTable->currentData().toInt(),
                                             ui->writeAddress->value(),
                                             10);
    QModbusDataUnit::RegisterType table = writeUnit.registerType();

    if (auto *reply = deif->modbusDevice->sendWriteRequest(writeUnit, ui->serverEdit->value())) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    ui->lblStatus->setText(tr("Write response error: %1 (Mobus exception: 0x%2)")
                        .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16));
                } else if (reply->error() != QModbusDevice::NoError) {
                    ui->lblStatus->setText(tr("Write response error: %1 (code: 0x%2)").
                        arg(reply->errorString()).arg(reply->error(), -1, 16));
                }
                reply->deleteLater();
            });
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }
    } else {
        ui->lblStatus->setText(tr("Write error: ") + deif->modbusDevice->errorString());
    }
    */
}

/*
void ModbusFrame::on_readWriteButton_clicked()
{
    if (!deif->modbusDevice)
        return;
    ui->readValue->clear();
    ui->lblStatus->clear();

    QModbusDataUnit writeUnit = writeRequest();
    QModbusDataUnit::RegisterType table = writeUnit.registerType();

    if (auto *reply = deif->modbusDevice->sendReadWriteRequest(readRequest(), writeUnit,
        ui->serverEdit->value())) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished,
                    this, &ModbusFrame::readReady);
       } else {
            delete reply; // broadcast replies return immediately
       }
    } else {
        ui->lblStatus->setText(tr("Read error: ") + deif->modbusDevice->errorString());
    }
}
*/

void ModbusFrame::on_writeTable_currentIndexChanged(int index)
{
    const bool coilsOrHolding = index == 0 || index == 3;

    ui->readWriteButton->setEnabled(index == 3);
    ui->writeButton->setEnabled(coilsOrHolding);
    ui->writeGroupBox->setEnabled(coilsOrHolding);
}

void ModbusFrame::on_checkAuto_clicked(bool checked)
{
    (checked) ? deif->deifTimer->start(2000) : deif->deifTimer->stop();
}



void ModbusFrame::on_serverEdit_valueChanged(int arg1)
{
    deif->setServerAddress(arg1);
}
