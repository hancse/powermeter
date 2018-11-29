/***************************************************************************
**
**  This file is part of AtlasTerminal, a host computer GUI for
**  Atlas Scientific(TM) stamps
**  connected to an Arduino(TM)
**  with Tentacle (TM) interface shield.
**  Copyright (C) 2016 Paul JM van Kan
**
**  AtlasTerminal is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.

**  AtlasTerminal is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.

**  You should have received a copy of the GNU General Public License
**  along with AtlasTerminal.  If not, see <http://www.gnu.org/licenses/>.

***************************************************************************
**           Author: Paul JM van Kan                                     **
**  Website/Contact:                                                     **
**             Date:                                                     **
**          Version:                                                     **
***************************************************************************/

#include "serialdialog.h"
#include "ui_serialdialog.h"

#include <QtSerialPort/QSerialPortInfo>
#include <QIntValidator>
#include <QLineEdit>
#include <QPushButton>
#include <QDebug>

QT_USE_NAMESPACE

static const char blankString[] = QT_TRANSLATE_NOOP("SerialDialog", "N/A");

SerialDialog::SerialDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialDialog)
{
    ui->setupUi(this);

    intValidator = new QIntValidator(0, 4000000, this);

    ui->cbBaud->setInsertPolicy(QComboBox::NoInsert);

    connect( ui->cbSerialPortInfo, SIGNAL(currentIndexChanged(int)),
             this, SLOT(showPortInfo(int)) );
    connect( ui->cbBaud, SIGNAL(currentIndexChanged(int)),
             this, SLOT(checkCustomBaudRatePolicy(int)) );
    connect( ui->cbSerialPortInfo, SIGNAL(currentIndexChanged(int)),
             this, SLOT(checkCustomDevicePathPolicy(int)) );

    QPushButton* applyButton = ui->buttonBox->button(QDialogButtonBox::Apply);
    connect( applyButton, SIGNAL(clicked()),
             this, SLOT(apply()) );

    fillPortsParameters();
    fillPortsInfo();
    updateParameters();
}

SerialDialog::~SerialDialog()
{
    delete ui;
}

void SerialDialog::apply()
{
    qDebug() << "applied";
    updateParameters();
    hide();
}

SerialDialog::PortParameters SerialDialog::getCp() const  //getter for current parameters
{
    return cp;
}

void SerialDialog::showPortInfo(int idx)
{
    if (idx == -1)
        return;

    QStringList list = ui->cbSerialPortInfo->itemData(idx).toStringList();
    ui->labelDesc->setText(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : tr(blankString)));
    ui->labelManu->setText(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : tr(blankString)));
    ui->labelSerNum->setText(tr("Serial number: %1").arg(list.count() > 3 ? list.at(3) : tr(blankString)));
    ui->labelLocation->setText(tr("Location: %1").arg(list.count() > 4 ? list.at(4) : tr(blankString)));
    ui->labelVendor->setText(tr("Vendor Identifier: %1").arg(list.count() > 5 ? list.at(5) : tr(blankString)));
    ui->labelProd->setText(tr("Product Identifier: %1").arg(list.count() > 6 ? list.at(6) : tr(blankString)));
}

void SerialDialog::checkCustomBaudRatePolicy(int idx)
{
    bool isCustomBaudRate = !ui->cbBaud->itemData(idx).isValid();
    ui->cbBaud->setEditable(isCustomBaudRate);
    if (isCustomBaudRate) {
        ui->cbBaud->clearEditText();
        QLineEdit *edit = ui->cbBaud->lineEdit();
        edit->setValidator(intValidator);
    }
}

void SerialDialog::checkCustomDevicePathPolicy(int idx)
{
    bool isCustomPath = !ui->cbSerialPortInfo->itemData(idx).isValid();
    ui->cbSerialPortInfo->setEditable(isCustomPath);
    if (isCustomPath)
        ui->cbSerialPortInfo->clearEditText();
}

void SerialDialog::fillPortsParameters()
{
    ui->cbBaud->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->cbBaud->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->cbBaud->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->cbBaud->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
    ui->cbBaud->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->cbBaud->addItem(tr("Custom"));

    ui->cbDataBits->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->cbDataBits->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->cbDataBits->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->cbDataBits->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->cbDataBits->setCurrentIndex(3);

    ui->cbParity->addItem(tr("None"), QSerialPort::NoParity);
    ui->cbParity->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->cbParity->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->cbParity->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->cbParity->addItem(tr("Space"), QSerialPort::SpaceParity);

    ui->cbStopBits->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->cbStopBits->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->cbStopBits->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->cbFlowControl->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->cbFlowControl->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->cbFlowControl->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

void SerialDialog::fillPortsInfo()
{
    ui->cbSerialPortInfo->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        ui->cbSerialPortInfo->addItem(list.first(), list);
    }

    ui->cbSerialPortInfo->addItem(tr("Custom"));
}

void SerialDialog::updateParameters()
{
    cp.name = ui->cbSerialPortInfo->currentText();

    if (ui->cbBaud->currentIndex() == 5) {
        cp.baudRate = ui->cbBaud->currentText().toInt();
    } else {
        cp.baudRate = static_cast<QSerialPort::BaudRate>(
                    ui->cbBaud->itemData(ui->cbBaud->currentIndex()).toInt());
    }
    cp.stringBaudRate = QString::number(cp.baudRate);

    cp.dataBits = static_cast<QSerialPort::DataBits>(
                ui->cbDataBits->itemData(ui->cbDataBits->currentIndex()).toInt());
    cp.stringDataBits = ui->cbDataBits->currentText();

    cp.parity = static_cast<QSerialPort::Parity>(
                ui->cbParity->itemData(ui->cbParity->currentIndex()).toInt());
    cp.stringParity = ui->cbParity->currentText();

    cp.stopBits = static_cast<QSerialPort::StopBits>(
                ui->cbStopBits->itemData(ui->cbStopBits->currentIndex()).toInt());
    cp.stringStopBits = ui->cbStopBits->currentText();

    cp.flowControl = static_cast<QSerialPort::FlowControl>(
                ui->cbFlowControl->itemData(ui->cbFlowControl->currentIndex()).toInt());
    cp.stringFlowControl = ui->cbFlowControl->currentText();

    cp.localEchoEnabled = ui->localEchoCheckBox->isChecked();

    cp.responseTime = ui->timeoutSpinner->value();
    cp.numberOfRetries = ui->retriesSpinner->value();
}

