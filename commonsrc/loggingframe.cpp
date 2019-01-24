#include "loggingframe.h"
#include "ui_loggingframe.h"
#include <QDebug>

LoggingFrame::LoggingFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::LoggingFrame)
{
    ui->setupUi(this);
}

LoggingFrame::~LoggingFrame()
{
    delete ui;
}

// http://www.bogotobogo.com/Qt/Qt5_QFile.php

void LoggingFrame::write(const QString &line)
{
    //QFile file("C:/Test/simple.txt");
    if(logFile.isOpen() ) {
        logStream << line << endl;
        //qDebug() << "Writing finished";
    } else {
        //qDebug() << "Error: logging stream not open";
    }
}

void LoggingFrame::read()
{
    //QFile file("C:/Test/simple.txt");
    if(logFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // We're going to streaming the file
        // to the QString
        QTextStream stream(&logFile);

        QString line;
        do {
            line = stream.readLine();
            qDebug() << line;
        } while(!line.isNull());

        logFile.close();
        //qDebug() << "Reading finished";
    }

}

QDir LoggingFrame::getLogDir() const
{
    return logDir;
}

void LoggingFrame::setLogDir(const QDir &value)
{
    logDir = value;
    ui->leLogDir->setText(logDir.absolutePath());
}

void LoggingFrame::on_btnWrite_clicked()
{
    //write(QString line);
}

void LoggingFrame::on_btnRead_clicked()
{
    //read();
}

void LoggingFrame::setLogFile(const QString &value)
{
    logFile.setFileName(logDir.absoluteFilePath(value));
    ui->leLogDir->setText(logFile.fileName());
}


void LoggingFrame::on_btnStartLog_clicked()
{
    if(logFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        // We're going to streaming text to the file
        logStream.setDevice(&logFile);

        // Write header line to log file:
        logStream << "# unixTime, year,month,day, hour,min,sec, sys,acc,mag, compAz,compAlt, measAz,measAlt, "
                  "temp1, temp2, temp3, temp4, sourceVolts, cellCurrentmA, north, east, south, west" << endl;
    }
}

void LoggingFrame::on_btnStopLog_clicked()
{
    if(logFile.isOpen()) {
        logFile.close();
    }
}
