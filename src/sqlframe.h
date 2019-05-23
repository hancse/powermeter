#ifndef SQLFRAME_H
#define SQLFRAME_H

#include <QFrame>
#include "commonsrc/plotframe2.h"
#include "person.h"
#include "personjsonserializer.h"
#include "personcontainer.h"
#include "thirdparty/json-develop/single_include/nlohmann/json.hpp"

namespace Ui {
class SQLFrame;
}

using json = nlohmann::json;

class SQLFrame : public QFrame
{
    Q_OBJECT

public:
    explicit SQLFrame(QWidget *parent = nullptr);
    ~SQLFrame();

    struct Measurement {
        int id;
        QVector<double> phaseVoltageL[3];
        QVector<double> currentL[3];
        double activePower;
        double energy;
        double frequency;
    };

private:
    void DatabaseConnect();
    void DatabaseInit();
    void DatabasePopulate();

private slots:
    void on_btnSearch_clicked();
    void on_leInput_textChanged(const QString &arg1);

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::SQLFrame *ui;
    PlotFrame2* plf;
    void DatabaseTest();
    void testSerializeAndDeserializePerson();
    void testSerializeAndDeserializeContainer();
    json msg2db;
};

#endif // SQLFRAME_H
