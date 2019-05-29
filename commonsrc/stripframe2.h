#ifndef STRIPFRAME2_H
#define STRIPFRAME2_H

#include <QFrame>
#include <QDialog>
#include <QTimer>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_date.h>
#include <qwt_date_scale_draw.h>
#include <qwt_date_scale_engine.h>

namespace Ui {
class StripFrame2;
}

class StripFrame2 : public QFrame
{
    Q_OBJECT

public:
    explicit StripFrame2(QWidget *parent = nullptr);
    ~StripFrame2();

    void realTimeSlot(QDateTime newDT, double newData);
    void setXYLabel(int plotIndex, const QString &xLabel, const QString &yLabel);
    void setYMinMax(int plotIndex, const double &minValue, const double &maxValue);

    void setupPlot();
    void setXSpanSec(double value);

    int getHistory() const;
    void setHistory(int value);

    void setAutoY(bool autovalue);
    void scaleYAxisUp(int plotIndex);
    void scaleYAxisDown(int plotIndex);
    double getYMin(int plotIndex) const;
    double getYMax(int plotIndex) const;

private slots:
    void on_btnDetach_clicked(bool checked);
    void on_sbSpan_valueChanged(int arg1);
    void on_btnClear_clicked();    
    void realTimeDummySlot();

    void on_tBtnUp_clicked();

    void on_tBtnDown_clicked();

private:
    Ui::StripFrame2 *ui;
    QDialog* pd;
    QTimer* dataTimer;

// plot objects
    QwtPlot* plot[3];
    QwtPlotCurve* curve[9];
    QwtDateScaleDraw* scaleDraw[3];
    QwtDateScaleEngine* scaleEngine[3];

    int dataCount = 0; // initialize to 0
    int history = 60; // initialize to 60 points, to avoid out-of-range in constructor

// x-axis
    QVector<double> timeData = QVector<double>(60, 0.0);
    QVector<QDateTime> timeDT = QVector<QDateTime>(60, QDateTime::currentDateTime());
    QVector<qint64> timeMSecs = QVector<qint64>(60, QDateTime::currentMSecsSinceEpoch());
    double xSpanSec = 60.0;
    double xScaleLow, xScaleHigh;
    QString xLabel;

// y-axis
    QVector<double> data = QVector<double>(60, 0.0);
    double yMin[3] = {0.0, 0.0, 0.0};
    double yMax[3] = {1.0, 1.0, 1.0};
    QString yLabel;

    void clearPlot(int plotIndex);
};

#endif // STRIPFRAME2_H
