#include "sqlframe.h"
#include "ui_sqlframe.h"

#include <QDebug>
#include <QIntValidator>

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QtSql>
#include <QList>
//#include "backendhandler.h"

SQLFrame::SQLFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SQLFrame)
{
    ui->setupUi(this);
    //plf = new PlotFrame2(this);
    //plf->move(450,0);

    ui->leInput->setValidator(new QIntValidator(ui->leInput));

// -- DATABASE INIT --
    qDebug() << QSqlDatabase::drivers();
    DatabaseConnect();
    //DatabaseInit();
    //DatabasePopulate();
    DatabaseTest();

    msg2db["pi"] = 3.14;
}

SQLFrame::~SQLFrame()
{
    delete ui;
}

void SQLFrame::DatabaseConnect()
{
    const QString DRIVER("QMYSQL");

    if(QSqlDatabase::isDriverAvailable(DRIVER))
    {
        QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);
// sustainablecharging_nl_evcharging
// unix timestamp
        db.setDatabaseName("sustainablecharging_nl_evcharging");
        //db.setHostName("ec2-54-208-101-222.compute-1.amazonaws.com");
        //db.setHostName("ec2-54-208-101-222.compute-1.amazonaws.com");
        //db.setHostName("hannl-lmrt-db.mysql.database.azure.com");
        db.setHostName("hannl-lmrt-db.mysql.database.azure.com");
        db.setPort(3306);
        //db.setPort(1433);
        //db.setUserName("sevci-admin");
        //db.setPassword("canterbury");
        db.setUserName("myadmin@hannl-lmrt-db");
        db.setPassword("Lmrt@2019");

        if(!db.open())
            qWarning() << "MainWindow::DatabaseConnect - ERROR: " << db.lastError().text();
        else
            ui->pteTables->appendPlainText(db.databaseName() + "\n");

    }
    else
        qWarning() << "MainWindow::DatabaseConnect - ERROR: no driver " << DRIVER << " available";
}

void SQLFrame::DatabaseInit()
{
    //QSqlQuery query("CREATE TABLE people (id INTEGER PRIMARY KEY, name TEXT)");

    QSqlQuery createQuery("CREATE TABLE source_pv "
                              "(id BIGINT(20) UNSIGNED ZEROFILL AUTO_INCREMENT NOT NULL PRIMARY KEY,"
                               "phaseVoltageL1 float NOT NULL"
                               "phase_voltage_L2 float NOT NULL,"
                               "phase_voltage_L3 float NOT NULL,"
                               "current_L1 float NOT NULL,"
                               "current_L2 float NOT NULL,"
                               "current_L3 float NOT NULL,"
                               "active_power` float NOT NULL,"
                               "energy float NOT NULL,"
                               "frequency float NOT NULL,"
                               "createdAt datetime NOT NULL DEFAULT CURRENT_TIMESTAMP) "
                          "ENGINE=InnoDB DEFAULT CHARSET=latin1)");
    if(!createQuery.isActive())
        qWarning() << "MainWindow::DatabaseInit - ERROR: " << createQuery.lastError().text();

}

void SQLFrame::DatabasePopulate()
{
    QSqlQuery query;

    if(!query.exec("INSERT INTO source_pv(phaseVoltageL1) VALUES(233.456)"))
        qWarning() << "MainWindow::DatabasePopulate - ERROR: " << query.lastError().text();
}

void SQLFrame::DatabaseTest()
{
    QSqlQuery query;
    query.prepare("SHOW FULL TABLES IN sustainablecharging_nl_evcharging "
                  "WHERE TABLE_TYPE LIKE 'BASE TABLE'");

    if(!query.exec())
        qWarning() << "MainWindow::DatabaseTest - ERROR: " << query.lastError().text();
    //if(query.first()) {
        //qDebug() << query.value(0).toString() << query.value(1).toString();
        //ui->pteTables->appendPlainText((query.value(0).toString()));
    //} else
       // ui->lblOutput->setText("tables not found");

    while (query.next()) {
        QString table = query.value(0).toString();
        QString type = query.value(1).toString();
        qDebug() << table << type;
        ui->pteTables->appendPlainText(table + "   " + type );
    }
}

// ===== PRIVATE SLOTS =====

void SQLFrame::on_btnSearch_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT name FROM people WHERE id = ?");
    query.addBindValue(ui->leInput->text().toInt());

    if(!query.exec())
        qWarning() << "MainWindow::OnSearchClicked - ERROR: " << query.lastError().text();

    if(query.first())
        ui->lblOutput->setText(query.value(0).toString());
    else
        ui->lblOutput->setText("person not found");
}

void SQLFrame::on_leInput_textChanged(const QString &arg1)
{
    if(ui->leInput->text().length() > 0)
        ui->btnSearch->setEnabled(true);
    else
        ui->btnSearch->setEnabled(false);
}
