#ifndef STRIPFRAMETRIPLE_H
#define STRIPFRAMETRIPLE_H

#include <QFrame>
#include <QDialog>
#include <QTimer>
#include <QVector>

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
    static const int NUMCURVES = 9;
    static const int NUMCURVESPERPLOT = 3;

    struct PlotData {
        // x-axis
        QVector<double> timeData = QVector<double>(60, 0.0);
        QVector<QDateTime> timeDT = QVector<QDateTime>(60, QDateTime::currentDateTime());
        QVector<qint64> timeMSecs = QVector<qint64>(60, QDateTime::currentMSecsSinceEpoch());
        // y-axis
        QVector<double> yData = QVector<double>(60, 0.0);
    };

    void realTimeSlot(int curveIndex,
                      QDateTime newDT, double newData,
                      bool doReplot);
    void realTimeAllSlot(QDateTime newDT, QVector<double> newDataVector,
                         bool doReplot);

    void setXYLabel(int plotIndex, const QString &xLabel, const QString &yLabel);
    void setYMinMax(int plotIndex, const double &minValue, const double &maxValue);

    void setupPlot();
    void setXSpanSec(double value);

    int getHistory() const;
    void setHistory(int value);

    void setAutoY(int plotIndex, bool autovalue);
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
    Ui::StripFrameTriple *ui;
    QDialog* pd;
    QTimer* dataTimer;

// plot objects
    QwtPlot* plot[NUMPLOTS];
    QwtPlotCurve* curve[NUMCURVES][2];   //line and dot on end
    QwtDateScaleDraw* scaleDraw[NUMPLOTS];
    QwtDateScaleEngine* scaleEngine[NUMPLOTS];

    int dataCount = 0; // initialize to 0
    int history = 60; // initialize to 60 points, to avoid out-of-range in constructor

// x-axis
    //QVector<double> timeData = QVector<double>(60, 0.0);
    //QVector<QDateTime> timeDT = QVector<QDateTime>(60, QDateTime::currentDateTime());
    //QVector<qint64> timeMSecs = QVector<qint64>(60, QDateTime::currentMSecsSinceEpoch());
    double xSpanSec = 60.0;
    double xScaleLow, xScaleHigh;
    QString xLabel;

// y-axis
    //QVector<double> data = QVector<double>(60, 0.0);
    //QVector< QVector<double> > data2D{NUMPLOTS, QVector<double>(NUMCURVESPERPLOT)};
    double yMin[NUMPLOTS]; // = 0.0;
    double yMax[NUMPLOTS]; // = 1.0;
    QString yLabel[NUMPLOTS];

// other way out: NUMPLOTS*NUMCURVESPERPLOT PlotData structs; all curves have independent x and y
    PlotData data[NUMCURVES];

    // Create; use curly braces, otherwise compiler thinks it is a function declaration
      //std::vector< std::vector<int> > v{4, std::vector<int>(4)};
      //QVector< QVector<double> > qv{NUMPLOTS, QVector<double>(NUMCURVESPERPLOT)};
      // Write
      //v[2][3] = 10;
      // Read
      //int a = v[2][3];
};

#endif // STRIPFRAMETRIPLE_H
