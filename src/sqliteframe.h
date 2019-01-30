#ifndef SQLITEFRAME_H
#define SQLITEFRAME_H

#include <QFrame>

namespace Ui {
class SQLiteFrame;
}

class SQLiteFrame : public QFrame
{
    Q_OBJECT

public:
    explicit SQLiteFrame(QWidget *parent = nullptr);
    ~SQLiteFrame();

private:
    void DatabaseConnect();
    void DatabaseInit();
    void DatabasePopulate();

private slots:
    void on_btnSearch_clicked();
    void on_leInput_textChanged(const QString &arg1);

private:
    Ui::SQLiteFrame *ui;
    void DatabaseTest();
};

#endif // SQLITEFRAME_H
