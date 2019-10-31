#ifndef LOGGINGFRAME_H
#define LOGGINGFRAME_H

#include <QFrame>

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
    explicit LoggingFrame(QWidget *parent = nullptr);
    ~LoggingFrame();

// getters and setters
    QDir getLogDir() const;
    void setLogDir(const QDir &value);
    void setLogFile(const QString &value);

    void write(const QString &line);
    void read();

    QString getLogHeader() const;
    void setLogHeader(const QString &value);

    bool getIsLogging() const;
    QString getCommentLine() const;
    void setCommentLine(const QString &value);

public slots:
    void on_btnStart_clicked();
    void on_btnStop_clicked();

private slots:
    void on_btnBrowse_clicked();
    void on_btnInsert_clicked();

private:
    Ui::LoggingFrame *ui;
    QTimer* logTimer;

    QDir logDir;
    QFile logFile;
    QTextStream logStream;
    QString logHeader;
    QString commentLine;
    bool isLogging = false;
};

#endif // LOGGINGFRAME_H
