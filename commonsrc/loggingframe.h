#ifndef LOGGINGFRAME_H
#define LOGGINGFRAME_H

#include <QFrame>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QFile>

namespace Ui {
class LoggingFrame;
}

class LoggingFrame : public QFrame
{
    Q_OBJECT

public:
    explicit LoggingFrame(QWidget *parent = 0);
    ~LoggingFrame();

    QDir getLogDir() const;
    void setLogDir(const QDir &value);

    void write(const QString &line);
    void read();

    void setLogFile(const QString &value);
    void on_btnStartLog_clicked();
    void on_btnStopLog_clicked();

private slots:
    void on_btnWrite_clicked();
    void on_btnRead_clicked();

private:
    Ui::LoggingFrame *ui;
    QDir logDir;
    QFile logFile;
    QTextStream logStream;
};

#endif // LOGGINGFRAME_H
