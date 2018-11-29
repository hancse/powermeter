#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QModbusTcpClient>
#include <QModbusRtuSerialMaster>
#include <QUrl>


enum ModbusConnection {
    Serial,
    Tcp
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
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

MainWindow::~MainWindow()
{
    delete ui;
    if (modbusDevice)
        modbusDevice->disconnectDevice();
    delete modbusDevice;
}

void MainWindow::on_btnConnect_clicked()
{
    if (!modbusDevice)  // no device defined, nothing to connect
        return;

    statusBar()->clearMessage();
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
            statusBar()->showMessage(tr("Connect failed: ") + modbusDevice->errorString(), 5000);
        } else {
            ui->btnConnect->setText("Disconnect");
        }
    } else {
        modbusDevice->disconnectDevice();
        ui->btnConnect->setText("Connect");
    }
}

void MainWindow::on_btnDisconnect_clicked()
{
    if (!modbusDevice)  // no device defined, nothing to connect
        return;

    statusBar()->clearMessage();
    if (modbusDevice->state() == QModbusDevice::ConnectedState) {
        modbusDevice->disconnectDevice();
        ui->btnConnect->setText("Connect");
    }
}

void MainWindow::on_btnConfig_clicked()
{
    sd->setModal(true);
    sd->show();
}

void MainWindow::on_connectType_currentIndexChanged(int index)
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
        statusBar()->showMessage(modbusDevice->errorString(), 5000);
    });

    if (!modbusDevice) {
        if (type == Serial)
            statusBar()->showMessage(tr("Could not create Modbus master."), 5000);
        else
            statusBar()->showMessage(tr("Could not create Modbus client."), 5000);
    } else {
        connect(modbusDevice, &QModbusClient::stateChanged,
                this, &MainWindow::onStateChanged);
    }
}

void MainWindow::onStateChanged(int state)
{
    if (state == QModbusDevice::UnconnectedState)
        ui->btnConnect->setText(tr("Connect"));
    else if (state == QModbusDevice::ConnectedState)
        ui->btnConnect->setText(tr("Disconnect"));
}

void MainWindow::on_readButton_clicked()
{
    if (!modbusDevice)
        return;
    ui->readValue->clear();
    statusBar()->clearMessage();

    if (auto *reply = modbusDevice->sendReadRequest(readRequest(), ui->serverEdit->value())) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &MainWindow::readReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        statusBar()->showMessage(tr("Read error: ") + modbusDevice->errorString(), 5000);
    }
}

void MainWindow::readReady()
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
        statusBar()->showMessage(tr("Read response error: %1 (Mobus exception: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->rawResult().exceptionCode(), -1, 16), 5000);
    } else {
        statusBar()->showMessage(tr("Read response error: %1 (code: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->error(), -1, 16), 5000);
    }

    reply->deleteLater();
}

void MainWindow::on_writeButton_clicked()
{
    if (!modbusDevice)
        return;
    statusBar()->clearMessage();

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
                    statusBar()->showMessage(tr("Write response error: %1 (Mobus exception: 0x%2)")
                        .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                        5000);
                } else if (reply->error() != QModbusDevice::NoError) {
                    statusBar()->showMessage(tr("Write response error: %1 (code: 0x%2)").
                        arg(reply->errorString()).arg(reply->error(), -1, 16), 5000);
                }
                reply->deleteLater();
            });
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }
    } else {
        statusBar()->showMessage(tr("Write error: ") + modbusDevice->errorString(), 5000);
    }
}

void MainWindow::on_readWriteButton_clicked()
{
    if (!modbusDevice)
        return;
    ui->readValue->clear();
    statusBar()->clearMessage();

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
            connect(reply, &QModbusReply::finished, this, &MainWindow::readReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        statusBar()->showMessage(tr("Read error: ") + modbusDevice->errorString(), 5000);
    }
}

void MainWindow::on_writeTable_currentIndexChanged(int index)
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

QModbusDataUnit MainWindow::readRequest() const
{
    const auto table =
        static_cast<QModbusDataUnit::RegisterType> (ui->writeTable->currentData().toInt());

    int startAddress = ui->readAddress->value();
    Q_ASSERT(startAddress >= 0 && startAddress < 10);

    // do not go beyond 10 entries
    int numberOfEntries = qMin(ui->readSize->currentText().toInt(), 10 - startAddress);
    return QModbusDataUnit(table, startAddress, numberOfEntries);
}

QModbusDataUnit MainWindow::writeRequest() const
{
    const auto table =
        static_cast<QModbusDataUnit::RegisterType> (ui->writeTable->currentData().toInt());

    int startAddress = ui->writeAddress->value();
    Q_ASSERT(startAddress >= 0 && startAddress < 10);

    // do not go beyond 10 entries
    int numberOfEntries = qMin(ui->writeSize->currentText().toInt(), 10 - startAddress);
    return QModbusDataUnit(table, startAddress, numberOfEntries);
}

