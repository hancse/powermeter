#include "loggingframe.h"
#include "ui_loggingframe.h"

#include <QDebug>
#include <QDateTime>
#include <QFileDialog>

LoggingFrame::LoggingFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::LoggingFrame)
{
    ui->setupUi(this);
    //setLogDir( QDir("C:/Data") );
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
        ui->plainTextEdit->appendPlainText(line);
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
    //ui->leLogDir->setText(logDir.absolutePath());
}

void LoggingFrame::setLogFile(const QString &value)
{
    logFile.setFileName(logDir.absoluteFilePath(value));
    ui->leLogDir->setText(logFile.fileName());
}

QString LoggingFrame::getLogHeader() const
{
    return logHeader;
}

void LoggingFrame::setLogHeader(const QString &value)
{
    logHeader = value;
}



/**
 * @brief LoggingFrame::on_btnStart_clicked
 *
 * https://wiki.qt.io/Simple-logger
 */
void LoggingFrame::on_btnStart_clicked()
{
    qDebug() << "entry on btn_Start";
// create a string from current datetime
    QDateTime datetime(QDateTime::currentDateTime());
    QString dtString = datetime.toString("yyyyMMdd_hhmmss");
// set log folder
    //setLogDir( QDir("C:/Data") );
// set log file with timestamp in filename
    //setLogFile("C:/Data/Phidgets_" + dtString + ".log");
// set header of logfile according to displayMeas method
    setLogHeader("# unixTime, yyyy-MM-dd, hh:mm:ss:zzz, loadratio, act.position");

    qDebug() << logFile.fileName();
    if(logFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        // We're going to stream text to the file
        qDebug() << logFile.fileName();
        logStream.setDevice(&logFile);
        logStream.setCodec("UTF-8");

        // Write header line to log file:
        logStream << logHeader << endl;
        ui->plainTextEdit->appendPlainText(logHeader);
    }
    isLogging = true;
// enable/disable buttons
    ui->btnStart->setEnabled(false);
    ui->btnStop->setEnabled(true);
}

void LoggingFrame::on_btnStop_clicked()
{
    isLogging = false;

    if(logFile.isOpen()) {
        logFile.close();
    }
    ui->btnStart->setEnabled(true);
    ui->btnStop->setEnabled(false);
}

/**
 * @brief insert comment line into logfile.
 * SLOT for SIGNAL btnInsert::clicked()
 */
void LoggingFrame::on_btnInsert_clicked()
{
    commentLine = ui->leComment->text();
    commentLine.prepend("# ");
}

void LoggingFrame::setCommentLine(const QString &value)
{
    commentLine = value;
}

QString LoggingFrame::getCommentLine() const
{
    return commentLine;
}

bool LoggingFrame::getIsLogging() const
{
    return isLogging;
}


void LoggingFrame::on_btnBrowse_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Select Logfile"),
                                                    "C:/",
                                                    tr("Text Files (*.log)"));
    setLogDir(QDir(fileName));
    setLogFile(fileName);
    qDebug() << logFile.fileName();
}

void LoggingFrame::on_btnWrite_clicked()
{
    //write(QString line);
}

void LoggingFrame::on_btnRead_clicked()
{
    //read();
}
