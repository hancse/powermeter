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
    setupConnections();
}

LoggingFrame::~LoggingFrame()
{
    delete ui;
}

void LoggingFrame::setupConnections()
{
    connect(ui->btnStart, &QPushButton::clicked, this, &LoggingFrame::start);
    connect(ui->btnStop, &QPushButton::clicked, this, &LoggingFrame::stop);
    connect(ui->btnInsert, &QPushButton::clicked, this, &LoggingFrame::insert);
    connect(ui->btnBrowse, &QPushButton::clicked, this, &LoggingFrame::browse);
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

/*
void LoggingFrame::addLine(PumpInfo::WMParameters p)
{
    if (this->getIsLogging()) {
        //QDateTime dt = QDateTime::currentDateTime();
        qint64 unixTime = p.dt.toMSecsSinceEpoch();
        //QString dateStr = p.dt.toString("yyyy-MM-dd");
        //QString timeStr = p.dt.toString("hh:mm:ss:zzz");

        int offset = p.dt.offsetFromUtc();
        p.dt.setOffsetFromUtc(offset);
        QString isoStr = p.dt.toString(Qt::ISODate);
        qDebug() << isoStr;

        //line.sprintf("%10i, %s, %s, %5.2lf, %5.2lf",
        //             unixTime, dateStr, timeStr, p.RPMvalue);

        QString line = QString("%1, %2, %3")
                               .arg(unixTime)
                               .arg(isoStr)
                               .arg(p.RPMvalue);

        write(line);
        QString commentLine = getCommentLine();
        if (!commentLine.isEmpty()) {
            write(commentLine);
            setCommentLine(QString());
        }
    }
}
*/

/**
 * @brief LoggingFrame::start
 *
 * https://wiki.qt.io/Simple-logger
 */
void LoggingFrame::start()
{
    qDebug() << "entry on btn_Start";

// set log folder: can be done from calling routine
    //setLogDir( QDir("C:/Data") );

// create a string from current datetime: can be done in caller
    //QDateTime datetime(QDateTime::currentDateTime());
    //QString dtString = datetime.toString("yyyyMMdd_hhmmss");

// set log file with timestamp in filename: can be done in caller
    //QString logPathFileExt = logDir.absoluteFilePath(dtString + ".log");
    //qDebug() << logPathFileExt;
    //setLogFile( logPathFileExt );

// set header of logfile according to displayMeas method: can be done in caller
    //setLogHeader("# unixTime, yyyy-MM-dd, hh:mm:ss:zzz, "
                //"param1, param2, param3,"
                // "loadratio, act.position");

    qDebug() << logFile.fileName();

    if(logFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        // We're going to stream text to the file
        qDebug() << logFile.fileName();
        logStream.setDevice(&logFile);
        logStream.setCodec("UTF-8");

        // Write header line to log file:
        logStream << logHeader << endl;
        ui->plainTextEdit->clear();
        ui->plainTextEdit->appendPlainText(logHeader);
    }
    isLogging = true;
// enable/disable buttons
    ui->btnStart->setEnabled(false);
    ui->btnStop->setEnabled(true);
}

void LoggingFrame::stop()
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
void LoggingFrame::insert()
{
    commentLine = ui->leComment->text();
    commentLine.prepend("# ");
}

void LoggingFrame::browse()
{
    QFileDialog dlg;
    QString fn = dlg.getSaveFileName(this,
                                     tr("Select Logfile"),
                                     this->logDir.canonicalPath(),
                                     tr("Text Files (*.log)"));

    qDebug() << fn << QDir(fn).absolutePath();
    if ( !fn.isEmpty() ) {
        QFileInfo info(fn);
        qDebug() << info.absoluteDir() << info.fileName();
        setLogDir(info.absoluteDir());
        setLogFile(info.absoluteFilePath());
        qDebug() << "logDir:" << logDir.canonicalPath()
                 << "logFile:" << logFile.fileName();
    }
}

//--GETTERS and SETTERS---------------------------------------

bool LoggingFrame::getIsLogging() const
{
    return isLogging;
}

QString LoggingFrame::getCommentLine() const
{
    return commentLine;
}

QString LoggingFrame::getLogHeader() const
{
    return logHeader;
}

QDir LoggingFrame::getLogDir() const
{
    return logDir;
}

void LoggingFrame::setCommentLine(const QString &value)
{
    commentLine = value;
}

void LoggingFrame::setLogHeader(const QString &value)
{
    logHeader = value;
}

void LoggingFrame::setLogDir(const QDir &value)
{
    logDir = value;
    //ui->leLogDir->setText(logDir.absolutePath());
}

void LoggingFrame::setLogFile(const QString &value)
{
    if ( !logFile.isOpen() ) {
        logFile.setFileName(value);
        ui->leLogDir->setText(logFile.fileName());
    }
}
