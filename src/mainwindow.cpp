#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>

#include "commonsrc/aboutdialog.h"
#include "fonts/IconsFontAwesome5.h"


enum ModbusConnection {
    Serial,
    Tcp
};

/**
 * @brief create instance of MainWindow class.
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    logTimer = new QTimer(this);
    logTimer->setInterval(5000);
    //logTimer->start();

    //connect(this->logTimer, &QTimer::timeout,
           //this,&MainWindow::displayAllMeas);

    //QFont font("Font Awesome 5 Free Solid", 32);

// frames are added to stackedWidget, including dummy pages
    populateStack();

    connect(ui->listWidget, SIGNAL(currentRowChanged(int)),
             this, SLOT(setStackIndex(int)));
    ui->listWidget->setCurrentRow(1);
    ui->stackedWidget->setCurrentIndex(1);

    //iniPathFileName = searchIniFile();
    //qDebug() << iniPathFileName;
    //loadSettings(iniPathFileName);
    backend = new BackendHandler(this);

    for (int n = 0; n < NUMDEIFS; n++) {
        connect(mbf[n]->deif, &DEIFModbus::dataReady,
                this, &MainWindow::displayAllMeas);
    }
}

/**
 * @brief delete instance of MainWindow class.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief display all measurements and write to logfile
 * @param timestamp: from load cell bridge measurement
 * @param ratio: from load cell
 */
void MainWindow::displayAllMeas(int addr, UniversalAEParams ae)
{
    QDateTime timestamp = QDateTime(QDateTime::currentDateTime());
    if (isLogging) {
        //QString line;

        //QDateTime dt = QDateTime::currentDateTime();
        qint64 unixTime = timestamp.toMSecsSinceEpoch();
        QString dateStr = timestamp.toString("yyyy-MM-dd");
        QString timeStr = timestamp.toString("hh:mm:ss:zzz");

        //line.sprintf("%10i, %s, %s, %5.2lf, %5.2lf",
        //             unixTime, dateStr, timeStr, ratio, position);

        QString line = QString("%1, %2, %3, %4, %5")
                       .arg(unixTime)
                       .arg(dateStr)
                       .arg(timeStr)
                       .arg(ae.freq);
                       //.arg(position);

         logf->write(line);
         if (!commentLine.isEmpty()) {
             logf-> write(commentLine);
             commentLine.clear();
         }
    }
    timestamp.setTimeZone(QTimeZone::utc());
    qint64 unixTimestamp = timestamp.toMSecsSinceEpoch();

    if (addr == mbf[0]->deif->getServerAddress()) {
        //UniversalAEParams aep = mbf[0]->deif->getAep();
        topf->displayPVData(ae);
    } else if (addr == mbf[1]->deif->getServerAddress()) {
        //UniversalAEParams aep = mbf[1]->deif->getAep();
        topf->displayGridData(ae);
    } else if (addr == mbf[0]->deif->getServerAddress()) {
        //UniversalAEParams aep = mbf[0]->deif->getAep();
        topf->displayBatteryData(ae);
    } else if (addr == mbf[0]->deif->getServerAddress()) {
        //UniversalAEParams aep = mbf[0]->deif->getAep();
        topf->displayLoadData(ae);
    } else if (addr == -1) {
        //UniversalAEParams aep = mbf[0]->deif->getAep();
        topf->displayGridData(ae);
        topf->displayPVData(ae);
        topf->displayBatteryData(ae);
        topf->displayLoadData(ae);
    }

    QString strmsg = "{ ";
    strmsg.append( QString("\"phaseVoltageL1\": %1,").arg(ae.phaseVoltage[0], 0, 'f', 1) );
    strmsg.append( QString("\"phaseVoltageL2\": %1,").arg(ae.phaseVoltage[1], 0, 'f', 1) );
    strmsg.append( QString("\"phaseVoltageL3\": %1,").arg(ae.phaseVoltage[2], 0, 'f', 1) );
    strmsg.append( QString("\"currentL1\": %1,").arg(ae.phaseCurrent[0], 0, 'f', 1) );
    strmsg.append( QString("\"currentL2\": %1,").arg(ae.phaseCurrent[1], 0, 'f', 1) );
    strmsg.append( QString("\"currentL3\": %1,").arg(ae.phaseCurrent[2], 0, 'f', 1) );
    strmsg.append( QString("\"power\": %1,").arg(ae.systemPower, 0, 'f', 1) );
    strmsg.append( QString("\"energy\": %1,").arg(ae.energyTotal, 0, 'f', 1) );
    strmsg.append( QString("\"frequency\": %1,").arg(ae.freq, 0, 'f', 1) );
    strmsg.append( QString("\"timestamp\": %1").arg(ae.timestamp) );
    strmsg.append( QString("\"userId\": %1").arg(ae.userID) );
    strmsg.append( QString("\"socketId\": %1").arg(ae.socketID) );
    strmsg.append( " }_json");

    QByteArray bamsg = QByteArray();
    bamsg.append(strmsg);
    qDebug() << bamsg;

    //backend->postRequest("", bamsg);
    //backend->postRequest("http://localhost:8080/sevci_backend_war/measurements", bamsg);

    qDebug() << "POST done";
}

/**
 * @brief set Stack Index in stackWidget.
 * called by listWidget::currentIndexChanged
 * @param index
 */
void MainWindow::setStackIndex(int index)
{
    switch (index) {
    case 0:
        ui->stackedWidget->setCurrentIndex(index);
        break;
    case 1:
        ui->stackedWidget->setCurrentIndex(index);
        break;
    case 2:
        ui->stackedWidget->setCurrentIndex(index);
        break;
    case 3:
        ui->stackedWidget->setCurrentIndex(index);
        break;
    case 4:
        ui->stackedWidget->setCurrentIndex(index);
        break;
    case 5:
        ui->stackedWidget->setCurrentIndex(index);
        break;
    case 6:
        ui->stackedWidget->setCurrentIndex(index);
        break;
    case 7:
        qApp->quit();
        break;
    }
}



/**
   @brief adds frames to stackedWidget, including dummy pages.
 */
void MainWindow::populateStack()
{
    topf = new TopFrame();
    ui->stackedWidget->addWidget(topf);  //0

    for (int n = 0; n < NUMDEIFS; n++) {
        mbf[n] = new ModbusFrame();
        ui->stackedWidget->addWidget(mbf[n]);  //1,2,3,4
    }

    sqlf = new SQLFrame();
    ui->stackedWidget->addWidget(sqlf);  //5

    logf = new LoggingFrame();
    ui->stackedWidget->addWidget(logf);  //6

    qDebug() << "# pages:" <<ui->stackedWidget->count();
}

/**
 * @brief MainWindow::on_btnLogStart_clicked
 */
void MainWindow::on_btnLogStart_clicked()
{
// create a string from current datetime
    QDateTime datetime(QDateTime::currentDateTime());
    QString dtString = datetime.toString("yyyyMMdd_hhmmss");
// set log folder
    logf->setLogDir( QDir("C:/Data") );
// set log file with timestamp in filename
    logf->setLogFile("C:/Data/Phidgets_" + dtString + ".log");
// set header of logfile according to displayMeas method
    logf->setLogHeader("# unixTime, yyyy-MM-dd, hh:mm:ss, Freq, "
                       "V1, V2, V3, I1, I2, I3, P1, P2, P3, TotalEnergy");
// press on "Start" button in LogFrame
    logf->on_btnStart_clicked();
    isLogging = true;
// enable/disable buttons
    ui->btnLogStart->setEnabled(false);
    ui->btnLogStop->setEnabled(true);
}

/**
 * @brief MainWindow::on_btnLogStop_clicked
 */
void MainWindow::on_btnLogStop_clicked()
{
    isLogging = false;
    logf->on_btnStop_clicked();

    ui->btnLogStart->setEnabled(true);
    ui->btnLogStop->setEnabled(false);
}

/**
 * @brief insert comment line into logfile.
 * SLOT for SIGNAL btnInsert::clicked()
 */
void MainWindow::on_btnInsert_clicked()
{
    commentLine = ui->leComment->text();
    commentLine.prepend("# ");
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    qApp->aboutQt();
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog about;
    about.exec();  // Creates a modal about, which has focus until the user closes it
}

void MainWindow::on_actionExit_triggered()
{
    qApp->quit();
}

void MainWindow::on_pushButton_clicked()
{
    QByteArray ba = backend->test_serialize();
    backend->postRequest("https://hannl-sustainablecharching-be-app.azurewebsites.net", ba);
}

/**
 * Search the configuration file.
 * Aborts the application if not found.
 * @return The valid filename
 */
/*
QString MainWindow::searchIniFile()
{
    QString binDir = QApplication::applicationDirPath();
    QString appName = QApplication::applicationName();
    QFile file;
    file.setFileName(binDir + "/" + appName + ".ini");
    if (!file.exists()) {
        file.setFileName(binDir + "/../../" + appName + ".ini");
        if (!file.exists()) {
            file.setFileName(QDir::rootPath() + "etc/webapp1.ini");
        }
    }
    if (file.exists()) {
        QString configFileName=QDir(file.fileName()).canonicalPath();
        qDebug("using config file %s", qPrintable(configFileName));
        return configFileName;
    }
    else {
        qFatal("config file not found");
        return "config file not found";
    }
}
*/
/*
void MainWindow::loadSettings(QString iniFilename)
{
// beware that the dir names in *.ini match their case
// Windows is NOT case sensitive
// Qt is case sensitive and will create an extra entry in the QFileSystemModel

    qDebug() << "loading settings...";
    QSettings qs(iniFilename, QSettings::IniFormat);
    QString sText;

// dispatch data to frames
    qs.beginGroup("Bridge");
    int bSerial = qs.value("Serial", "0").toInt();
    int bChannel = qs.value("Channel", "0").toInt();
    bf->setIniParameters(bSerial, bChannel);
    qs.endGroup();

    qs.beginGroup("DCMotor");
    int mSerial = qs.value("Serial", "0").toInt();
    int mChannel = qs.value("Channel", "0").toInt();
    int mHubPort = qs.value("HubPort", "0").toInt();
    mf->setMotorIniParameters(mSerial, mChannel, mHubPort);
    qs.endGroup();

    qs.beginGroup("Potmeter");
    int rSerial = qs.value("Serial", "0").toInt();
    int rChannel = qs.value("Channel", "0").toInt();
    int rHubPort = qs.value("HubPort", "0").toInt();
    mf->setPotmeterIniParameters(rSerial, rChannel, rHubPort);

    double lowLim = (qs.value("LowLimit", "0.1").toDouble());
    mf->setLowLimitRatio(lowLim);
    double highLim = (qs.value("HighLimit", "0.9").toDouble());
    mf->setHighLimitRatio(highLim);
    double fullRange = (qs.value("FullRangeMM", "100.0").toDouble());
    mf->setFullRangeMM(fullRange);
    qs.endGroup();
}
*/
/*
void MainWindow::saveSettings(QString iniFilename)
{
    QSettings qs(iniFilename, QSettings::IniFormat);
    QString sText;

    qs.beginGroup("Dirs");
    QString dirText = projf->getPIDir().absolutePath();
    qs.setValue("PIDir", dirText);
    dirText = projf->getLogDir().absolutePath();
    qs.setValue("LogDir", dirText);
    qs.endGroup();

    qs.beginGroup("Form");
    //this->setGeometry( = qs.value("Top", "100.0").toString();
    sText = qs.value("Left", "100.0").toString();
    qs.endGroup();

    qs.beginGroup("PI_C884");
    qs.setValue("ComPort", "true").toInt());
    QString ip = (qs.value("IP", "true").toString());
    qs.endGroup();
}
*/


