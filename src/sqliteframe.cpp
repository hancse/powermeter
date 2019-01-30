#include "sqliteframe.h"
#include "ui_sqliteframe.h"

#include <QDebug>
#include <QIntValidator>

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QtSql>

SQLiteFrame::SQLiteFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SQLiteFrame)
{
    ui->setupUi(this);
    ui->leInput->setValidator(new QIntValidator(ui->leInput));

// -- DATABASE INIT --
    DatabaseConnect();
    //DatabaseInit();
    //DatabasePopulate();
    DatabaseTest();
}

SQLiteFrame::~SQLiteFrame()
{
    delete ui;
}

void SQLiteFrame::DatabaseConnect()
{
    const QString DRIVER("QMYSQL");

    if(QSqlDatabase::isDriverAvailable(DRIVER))
    {
        QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);
// sustainablecharging_nl_evcharging
// unix timestamp
        db.setDatabaseName("sustainablecharging_nl_evcharging");
        db.setHostName("ec2-54-208-101-222.compute-1.amazonaws.com");
        db.setPort(3306);
        db.setUserName("sevci-admin");
        db.setPassword("canterbury");

        if(!db.open())
            qWarning() << "MainWindow::DatabaseConnect - ERROR: " << db.lastError().text();
        else
            ui->pteTables->appendPlainText(db.databaseName() + "\n");

    }
    else
        qWarning() << "MainWindow::DatabaseConnect - ERROR: no driver " << DRIVER << " available";
}

void SQLiteFrame::DatabaseInit()
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

void SQLiteFrame::DatabasePopulate()
{
    QSqlQuery query;

    if(!query.exec("INSERT INTO source_pv(phaseVoltageL1) VALUES(233.456)"))
        qWarning() << "MainWindow::DatabasePopulate - ERROR: " << query.lastError().text();
}

void SQLiteFrame::DatabaseTest()
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

void SQLiteFrame::on_btnSearch_clicked()
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

void SQLiteFrame::on_leInput_textChanged(const QString &arg1)
{
    if(ui->leInput->text().length() > 0)
        ui->btnSearch->setEnabled(true);
    else
        ui->btnSearch->setEnabled(false);
}
