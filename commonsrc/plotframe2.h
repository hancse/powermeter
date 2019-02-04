#ifndef PLOTFRAME2_H
#define PLOTFRAME2_H

#include <QFrame>
#include <QDialog>

#include "thirdparty/QCustomPlot200/qcustomplot.h"

namespace Ui {
class PlotFrame2;
}

class PlotFrame2 : public QFrame
{
    Q_OBJECT

public:
    explicit PlotFrame2(QWidget *parent = nullptr);
    ~PlotFrame2();

    void realtimeUSBSlot(int index, double value);
    void setXYLabel(int index, const QString &xValue, const QString &yValue);
    void setYMinMax(int index, double valMin, double valMax);

private slots:
    void setupPlot3();
    void on_btnDetach_clicked(bool checked);
    void on_sbSpan_valueChanged(int arg1);

private:
    Ui::PlotFrame2 *ui;
    QDialog* pd;
    QCustomPlot* plot[3];

    QTimer* dataTimer;
    double xSpan = 1;
    double yMin = 0;
    double yMax = 14;
    QString xLabel;
    QString yLabel;
};

#endif // PLOTFRAME2_H
