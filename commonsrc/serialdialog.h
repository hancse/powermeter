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

#ifndef SERIALDIALOG_H
#define SERIALDIALOG_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE

namespace Ui {
class SerialDialog;
}

class QIntValidator;

QT_END_NAMESPACE

class SerialDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SerialDialog(QWidget *parent = nullptr);
    ~SerialDialog();

    struct PortParameters {
        QString name;
        qint32 baudRate = QSerialPort::Baud19200;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits = QSerialPort::Data8;
        QString stringDataBits;
        QSerialPort::Parity parity = QSerialPort::EvenParity;
        QString stringParity;
        QSerialPort::StopBits stopBits = QSerialPort::OneStop;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
        int responseTime = 1000;
        int numberOfRetries = 3;
    };

    PortParameters getCp() const;

private slots:
    void showPortInfo(int idx);
    void checkCustomBaudRatePolicy(int idx);
    void checkCustomDevicePathPolicy(int idx);

    void apply();

private:
    void fillPortsParameters();
    void fillPortsInfo();
    void updateParameters();

    Ui::SerialDialog *ui;
    PortParameters cp;             //geen pointer?
    QIntValidator *intValidator;
};

#endif // SERIALDIALOG_H
