#ifndef STRIPFRAMETRIPLE_H
#define STRIPFRAMETRIPLE_H

#include <QFrame>
#include <QDialog>
#include <QTimer>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_date.h>
#include <qwt_date_scale_draw.h>
#include <qwt_date_scale_engine.h>

namespace Ui {
class StripFrameTriple;
}

class StripFrameTriple : public QFrame
{
    Q_OBJECT

public:
    explicit StripFrameTriple(QWidget *parent = nullptr);
    ~StripFrameTriple();

    static const int NUMPLOTS = 3;
    static const int NUMCURVESPERPLOT = 3;

    void realTimeSlot(int plotIndex, QDateTime newDT, double newData);
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


    void on_tBtnUp_clicked();

    void on_tBtnDown_clicked();

private:
    Ui::StripFrameTriple *ui;
    QDialog* pd;
    QTimer* dataTimer;

// plot objects
    QwtPlot* plot[NUMPLOTS];
    QwtPlotCurve* curve[NUMPLOTS][NUMCURVESPERPLOT];
    QwtDateScaleDraw* scaleDraw[NUMPLOTS];
    QwtDateScaleEngine* scaleEngine[NUMPLOTS];

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
    double yMin[NUMPLOTS] = {0.0, 0.0, 0.0};
    double yMax[NUMPLOTS] = {1.0, 1.0, 1.0};
    QString yLabel;

    void clearPlot(int plotIndex);
    void realTimeDummySlot();
};

#endif // STRIPFRAMETRIPLE_H
