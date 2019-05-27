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
    void setXYLabel(const QString &xLabel, const QString &yLabel);
    void setYMinMax(const double &minValue, const double &maxValue);

    void setupPlot();
    void setXSpanSec(double value);

    int getHistory() const;
    void setHistory(int value);

    void setAutoY(bool autovalue);
    void scaleYAxisUp();
    void scaleYAxisDown();
    double getYMin() const;
    double getYMax() const;

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
    QwtPlot* plot[1];
    QwtPlotCurve* curve[2];
    QwtDateScaleDraw* scaleDraw;
    QwtDateScaleEngine* scaleEngine;

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
    double yMin = 0.0;
    double yMax = 1.0;
    QString yLabel;


};

#endif // STRIPFRAME2_H
