#include "modbusframe.h"
#include "ui_modbusframe.h"

#include <QModbusTcpClient>
#include <QModbusRtuSerialMaster>
#include <QUrl>
#include <QDebug>

enum ModbusConnection {
    Serial,
    Tcp
};

ModbusFrame::ModbusFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ModbusFrame),
    lastRequest(nullptr),
    modbusDevice(nullptr)
{
    ui->setupUi(this);

    sd = new SerialDialog(this);
    tcpd = new TcpDialog(this);

    ui->writeTable->addItem(tr("Coils"), QModbusDataUnit::Coils);
    ui->writeTable->addItem(tr("Discrete Inputs"), QModbusDataUnit::DiscreteInputs);
    ui->writeTable->addItem(tr("Input Registers"), QModbusDataUnit::InputRegisters);
    ui->writeTable->addItem(tr("Holding Registers"), QModbusDataUnit::HoldingRegisters);

    ui->connectType->setCurrentIndex(0);
    on_connectType_currentIndexChanged(0);

}

ModbusFrame::~ModbusFrame()
{
    delete ui;
    if (modbusDevice)
        modbusDevice->disconnectDevice();
    delete modbusDevice;
}

void ModbusFrame::on_btnConnect_clicked()
{
    if (!modbusDevice)  // no device defined, nothing to connect
        return;

    ui->lblStatus->clear();
    if (modbusDevice->state() != QModbusDevice::ConnectedState) {
        if (static_cast<ModbusConnection> (ui->connectType->currentIndex()) == Serial) {

            SerialDialog::PortParameters p = sd->getCp();
            ui->portEdit->setText(p.name);
            modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
                                                    ui->portEdit->text());
            modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,
                                                    p.parity);
            modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
                                                    p.baudRate);
            modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
                                                    p.dataBits);
            modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
                                                    p.stopBits);
            modbusDevice->setTimeout(p.responseTime);
            modbusDevice->setNumberOfRetries(p.numberOfRetries);

        } else {

            TcpDialog::PortParameters u = tcpd->getUp();
            //const QUrl url = QUrl::fromUserInput(ui->portEdit->text());
            modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, u.url.port());
            modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, u.url.host());
            modbusDevice->setTimeout(u.responseTime);
            modbusDevice->setNumberOfRetries(u.numberOfRetries);

        }

        if (!modbusDevice->connectDevice()) {
            ui->lblStatus->setText(tr("Connect failed: ") + modbusDevice->errorString());
        } else {
            ui->btnConnect->setText("Disconnect");
        }
    } else {
        modbusDevice->disconnectDevice();
        ui->btnConnect->setText("Connect");
    }
}

void ModbusFrame::on_btnDisconnect_clicked()
{
    if (!modbusDevice)  // no device defined, nothing to connect
        return;

    ui->lblStatus->clear();
    if (modbusDevice->state() == QModbusDevice::ConnectedState) {
        modbusDevice->disconnectDevice();
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
    if (modbusDevice) {
        modbusDevice->disconnectDevice();
        delete modbusDevice;
        modbusDevice = nullptr;
    }

    auto type = static_cast<ModbusConnection>(index);
    if (type == Serial) {
        modbusDevice = new QModbusRtuSerialMaster(this);
    } else if (type == Tcp) {
        modbusDevice = new QModbusTcpClient(this);
        if (ui->portEdit->text().isEmpty())
            ui->portEdit->setText(QLatin1Literal("127.0.0.1:502"));
    }

    connect(modbusDevice, &QModbusClient::errorOccurred, [this](QModbusDevice::Error) {
        ui->lblStatus->setText(modbusDevice->errorString());
    });

    if (!modbusDevice) {
        if (type == Serial)
            ui->lblStatus->setText(tr("Could not create Modbus master."));
        else
            ui->lblStatus->setText(tr("Could not create Modbus client."));
    } else {
        connect(modbusDevice, &QModbusClient::stateChanged,
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
    if (!modbusDevice)
        return;

    ui->readValue->clear();
    ui->lblStatus->clear();

    if ( auto* reply = modbusDevice->sendReadRequest(readRequest(),
                                                    ui->serverEdit->value()) ) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished,
                    this, &ModbusFrame::readReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        ui->lblStatus->setText(tr("Read error: ") + modbusDevice->errorString());
    }
}

void ModbusFrame::readReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        for (uint i = 0; i < unit.valueCount(); i++) {
            const QString entry = tr("Address: %1, Value: %2").arg(unit.startAddress() + i)
                                     .arg(QString::number(unit.value(i),
                                          unit.registerType() <= QModbusDataUnit::Coils ? 10 : 16));
            ui->readValue->addItem(entry);
        }
    } else if (reply->error() == QModbusDevice::ProtocolError) {
        ui->lblStatus->setText(tr("Read response error: %1 (Mobus exception: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->rawResult().exceptionCode(), -1, 16));
    } else {
        ui->lblStatus->setText(tr("Read response error: %1 (code: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->error(), -1, 16));
    }

    reply->deleteLater();
}

void ModbusFrame::on_writeButton_clicked()
{
    if (!modbusDevice)
        return;
    ui->lblStatus->clear();

    QModbusDataUnit writeUnit = writeRequest();
    QModbusDataUnit::RegisterType table = writeUnit.registerType();
    //for (uint i = 0; i < writeUnit.valueCount(); i++) {
        //if (table == QModbusDataUnit::Coils)
            //writeUnit.setValue(i, writeModel->m_coils[i + writeUnit.startAddress()]);
        //else
            //writeUnit.setValue(i, writeModel->m_holdingRegisters[i + writeUnit.startAddress()]);
    //}

    if (auto *reply = modbusDevice->sendWriteRequest(writeUnit, ui->serverEdit->value())) {
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
        ui->lblStatus->setText(tr("Write error: ") + modbusDevice->errorString());
    }
}

void ModbusFrame::on_readWriteButton_clicked()
{
    if (!modbusDevice)
        return;
    ui->readValue->clear();
    ui->lblStatus->clear();

    QModbusDataUnit writeUnit = writeRequest();
    QModbusDataUnit::RegisterType table = writeUnit.registerType();
    //for (uint i = 0; i < writeUnit.valueCount(); i++) {
        //if (table == QModbusDataUnit::Coils)
            //writeUnit.setValue(i, writeModel->m_coils[i + writeUnit.startAddress()]);
        //else
            //writeUnit.setValue(i, writeModel->m_holdingRegisters[i + writeUnit.startAddress()]);
    //}

    if (auto *reply = modbusDevice->sendReadWriteRequest(readRequest(), writeUnit,
        ui->serverEdit->value())) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished,
                    this, &ModbusFrame::readReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        ui->lblStatus->setText(tr("Read error: ") + modbusDevice->errorString());
    }
}

void ModbusFrame::on_writeTable_currentIndexChanged(int index)
{
    const bool coilsOrHolding = index == 0 || index == 3;
    //if (coilsOrHolding) {
        //ui->writeValueTable->setColumnHidden(1, index != 0);
        //ui->writeValueTable->setColumnHidden(2, index != 3);
        //ui->writeValueTable->resizeColumnToContents(0);
    //}

    ui->readWriteButton->setEnabled(index == 3);
    ui->writeButton->setEnabled(coilsOrHolding);
    ui->writeGroupBox->setEnabled(coilsOrHolding);
}

QModbusDataUnit ModbusFrame::readRequest() const
{
    const auto table =
        static_cast<QModbusDataUnit::RegisterType>(ui->writeTable->currentData().toInt());

    int startAddress = ui->readAddress->value();
    //Q_ASSERT(startAddress >= 0 && startAddress < 10);

    // do not go beyond 10 entries
    int numberOfEntries = qMin(ui->readSize->currentText().toInt(), 10 - startAddress);
    return QModbusDataUnit(table, startAddress, numberOfEntries);
}

QModbusDataUnit ModbusFrame::writeRequest() const
{
    const auto table =
        static_cast<QModbusDataUnit::RegisterType> (ui->writeTable->currentData().toInt());

    int startAddress = ui->writeAddress->value();
    //Q_ASSERT(startAddress >= 0 && startAddress < 10);

    // do not go beyond 10 entries
    int numberOfEntries = qMin(ui->writeSize->currentText().toInt(), 10 - startAddress);
    return QModbusDataUnit(table, startAddress, numberOfEntries);
}

double ModbusFrame::ByteArrayToDouble(QByteArray ba, double defaultValue = 0.0)
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

void ModbusFrame::on_pushButton_clicked()
{
    QByteArray ba;
    ba.resize(4);
    ba[3] = 0x42;
    ba[2] = 0x48;
    ba[1] = 0x0A;
    ba[0] = 0x3D;
    float value;
    //value = *reinterpret_cast<float*>(ba.data());
    value = *reinterpret_cast<const float*>(ba.data());
    qDebug() << value;
}
