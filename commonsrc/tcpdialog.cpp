#include "tcpdialog.h"
#include "ui_tcpdialog.h"

#include <QLineEdit>
#include <QPushButton>
#include <QDebug>

TcpDialog::TcpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpDialog)
{
    ui->setupUi(this);

    QPushButton* applyButton = ui->buttonBox->button(QDialogButtonBox::Apply);
    connect( applyButton, SIGNAL(clicked()),
             this, SLOT(apply()) );
}

TcpDialog::~TcpDialog()
{
    delete ui;
}

void TcpDialog::apply()
{
    qDebug() << "applied";
    updateParameters();
    hide();
}

TcpDialog::PortParameters TcpDialog::getUp() const  //getter for current parameters
{
    return up;
}

void TcpDialog::fillPortsParameters()
{
    ui->leIPAddress->setText(up.url.host());
    ui->lePort->setText(QString::number(up.url.port()));
    ui->timeoutSpinner->setValue(up.responseTime);
    ui->retriesSpinner->setValue(up.numberOfRetries);

}

void TcpDialog::updateParameters()
{
    //up.name = ui->cbSerialPortInfo->currentText();

    up.url.setHost(ui->leIPAddress->text());
    up.url.setPort(ui->lePort->text().toInt());
    up.responseTime = ui->timeoutSpinner->value();
    up.numberOfRetries = ui->retriesSpinner->value();
}
