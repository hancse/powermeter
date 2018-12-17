#include "sqliteframe.h"
#include "ui_sqliteframe.h"

#include <QDebug>
#include <QIntValidator>

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

SQLiteFrame::SQLiteFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SQLiteFrame)
{
    ui->setupUi(this);
    ui->leInput->setValidator(new QIntValidator(ui->leInput));

// -- DATABASE INIT --
    DatabaseConnect();
    DatabaseInit();
    DatabasePopulate();
}

SQLiteFrame::~SQLiteFrame()
{
    delete ui;
}

void SQLiteFrame::DatabaseConnect()
{
    const QString DRIVER("QSQLITE");

    if(QSqlDatabase::isDriverAvailable(DRIVER))
    {
        QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);

        db.setDatabaseName(":memory:");

        if(!db.open())
            qWarning() << "MainWindow::DatabaseConnect - ERROR: " << db.lastError().text();
    }
    else
        qWarning() << "MainWindow::DatabaseConnect - ERROR: no driver " << DRIVER << " available";
}

void SQLiteFrame::DatabaseInit()
{
    QSqlQuery query("CREATE TABLE people (id INTEGER PRIMARY KEY, name TEXT)");

    if(!query.isActive())
        qWarning() << "MainWindow::DatabaseInit - ERROR: " << query.lastError().text();

}

void SQLiteFrame::DatabasePopulate()
{
    QSqlQuery query;

    if(!query.exec("INSERT INTO people(name) VALUES('Eddie Guerrero')"))
        qWarning() << "MainWindow::DatabasePopulate - ERROR: " << query.lastError().text();
    if(!query.exec("INSERT INTO people(name) VALUES('Gordon Ramsay')"))
        qWarning() << "MainWindow::DatabasePopulate - ERROR: " << query.lastError().text();
    if(!query.exec("INSERT INTO people(name) VALUES('Alan Sugar')"))
        qWarning() << "MainWindow::DatabasePopulate - ERROR: " << query.lastError().text();
    if(!query.exec("INSERT INTO people(name) VALUES('Dana Scully')"))
        qWarning() << "MainWindow::DatabasePopulate - ERROR: " << query.lastError().text();
    if(!query.exec("INSERT INTO people(name) VALUES('Lila	Wolfe')"))
        qWarning() << "MainWindow::DatabasePopulate - ERROR: " << query.lastError().text();
    if(!query.exec("INSERT INTO people(name) VALUES('Ashley Williams')"))
        qWarning() << "MainWindow::DatabasePopulate - ERROR: " << query.lastError().text();
    if(!query.exec("INSERT INTO people(name) VALUES('Karen Bryant')"))
        qWarning() << "MainWindow::DatabasePopulate - ERROR: " << query.lastError().text();
    if(!query.exec("INSERT INTO people(name) VALUES('Jon Snow')"))
        qWarning() << "MainWindow::DatabasePopulate - ERROR: " << query.lastError().text();
    if(!query.exec("INSERT INTO people(name) VALUES('Linus Torvalds')"))
        qWarning() << "MainWindow::DatabasePopulate - ERROR: " << query.lastError().text();
    if(!query.exec("INSERT INTO people(name) VALUES('Hayley Moore')"))
        qWarning() << "MainWindow::DatabasePopulate - ERROR: " << query.lastError().text();
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
