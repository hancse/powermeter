#ifndef SQLFRAME_H
#define SQLFRAME_H

#include <QFrame>
#include "commonsrc/plotframe2.h"

namespace Ui {
class SQLFrame;
}

class SQLFrame : public QFrame
{
    Q_OBJECT

public:
    explicit SQLFrame(QWidget *parent = nullptr);
    ~SQLFrame();

private:
    void DatabaseConnect();
    void DatabaseInit();
    void DatabasePopulate();

private slots:
    void on_btnSearch_clicked();
    void on_leInput_textChanged(const QString &arg1);

private:
    Ui::SQLFrame *ui;
    PlotFrame2* plf;
    void DatabaseTest();
};

#endif // SQLFRAME_H
