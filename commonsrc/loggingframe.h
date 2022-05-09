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
    void start();
    void stop();
    void insert();
    void browse();

private slots:


private:
    Ui::LoggingFrame *ui;
    QTimer* logTimer;

    QDir logDir = QDir();
    QFile logFile;
    QTextStream logStream;
    QString logHeader = QString("");
    QString commentLine = QString("");
    bool isLogging = false;

    void setupConnections();
};

#endif // LOGGINGFRAME_H
