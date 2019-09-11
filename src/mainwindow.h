#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "src/topframe.h"
#include "src/modbusframe.h"
#include "src/sqlframe.h"
#include "commonsrc/loggingframe.h"
#include "backendhandler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static const int NUMDEIFS = 4;

private slots:
    void setStackIndex(int index);

    void on_btnLogStart_clicked();
    void on_btnLogStop_clicked();
    void on_btnInsert_clicked();

    void on_actionAbout_Qt_triggered();
    void on_actionAbout_triggered();
    void on_actionExit_triggered();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QTimer* logTimer;
    QString commentLine;
    bool isLogging = false;
    QString iniPathFileName;

    TopFrame* topf;
    ModbusFrame* mbf[NUMDEIFS];    ///<  array of NUMDEIFS ModbusFrame pointers on heap */
    SQLFrame* sqlf;
    LoggingFrame* logf;
    BackendHandler* backend;

    void populateStack();
    void displayAllMeas(int addr, UniversalAEParams ae);
    void loadSettings(QString iniFilename);
    void saveSettings(QString iniFilename);
    QString searchIniFile();
    void setupConnections();
};

#endif // MAINWINDOW_H
