#include "stripframetriple.h"
#include "ui_stripframetriple.h"

#include <qwt_plot_layout.h>
#include <qwt_scale_widget.h>
#include <qwt_symbol.h>
#include <cmath>

StripFrameTriple::StripFrameTriple(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::StripFrameTriple)
{
    ui->setupUi(this);










    dataTimer = new QTimer(this);       // dummy timer for testing
    setXSpanSec(ui->sbSpan->value());   // set x-span (time)

// initialize all plots: independent numbering from 0 .. NUMPLOTS
    for ( int n=0; n < NUMPLOTS; n++ ) {
        plot[n] = new QwtPlot(this);
        plot[n]->move(0, 10+n*170);
        scaleDraw[n] = new QwtDateScaleDraw(Qt::LocalTime);
        scaleEngine[n] = new QwtDateScaleEngine( Qt::LocalTime );        
    }

// initialize curves: independent numbering from 0 .. NUMCURVES
// each curve consists of 2 subcurves, [0] a line and [1] the dot for the last point
    int third = NUMCURVES/3;
    for (int m = 0; m < NUMCURVES; m++) {
        curve[m][0] = new QwtPlotCurve("RatioLine");
        curve[m][0]->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        curve[m][1] = new QwtPlotCurve("RatioLine");
        curve[m][1]->setRenderHint( QwtPlotItem::RenderAntialiased, true );

// curve pairs are assigned to plots (can be varied)
        if (m < third) {
            curve[m][0]->attach(plot[0]);
            curve[m][1]->attach(plot[0]);
        } else if ( m < 2*third ) {
            curve[m][0]->attach(plot[1]);
            curve[m][1]->attach(plot[1]);
        } else {
            curve[m][0]->attach(plot[2]);
            curve[m][1]->attach(plot[2]);
        }
    }
    setupPlot();
}

StripFrameTriple::~StripFrameTriple()
{
    delete ui;
}

void StripFrameTriple::setupPlot()
{
// first define current datetime outside of blocks
    QDateTime dt = QDateTime::currentDateTime();

// basic attributes of QwtPlot
    for ( int n = 0; n < NUMPLOTS; n++) {
        plot[n]->resize(440, 160);         // can be varied

        const int margin = 5;
        plot[n]->setContentsMargins( margin, margin, margin, margin );

        plot[n]->setAutoReplot( false );
        plot[n]->setAxisAutoScale(QwtPlot::xBottom, false);
        plot[n]->plotLayout()->setAlignCanvasToScales( true );
        setXYLabel(n, "xlabel", "ylabel");
        setYMinMax(n, 0.0, 1.0);
    }
    
    setXYLabel(0,"", "AC Voltage (V)");
    setYMinMax(0, 210, 250);
    setXYLabel(1,"", "AC Current (A)");
    setYMinMax(1, 0, 80);
    setXYLabel(2,"time", "AC Power (kW)");
    setYMinMax(2, 0, 50);

    for ( int n=0; n < NUMPLOTS; n++) {
//Set x-axis scaling: these are permanent settings for QDateScaleDraw object
        scaleDraw[n]->setDateFormat(QwtDate::Minute, "hh:mm:ss");
        scaleDraw[n]->setDateFormat(QwtDate::Second, "hh:mm:ss");

        plot[n]->setAxisScaleDraw(QwtPlot::xBottom, scaleDraw[n]);
        plot[n]->setAxisScaleEngine(QwtPlot::xBottom, scaleEngine[n]);
        //plot[n]->setAxisScale(QwtPlot::xBottom, xScaleLow, xScaleHigh);

        plot[n]->setAxisScale( QwtPlot::xBottom,
                               QwtDate::toDouble(dt.addSecs(-xSpanSec)),
                               QwtDate::toDouble(dt));
        plot[n]->setAxisFont(QwtPlot::xBottom, QFont("Arial", 8));

        //plot[n]->setAxisScale(QwtPlot::xBottom, x1, x2, 24*3600*1000);
        //plot[n]->setAxisLabelRotation( QwtPlot::xBottom, 0.0 );
        //setAxisLabelAlignment( QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom );
        //plot[n]->setAxisLabelAlignment( QwtPlot::xBottom, Qt::AlignHCenter | Qt::AlignBottom );

/*
 In situations, when there is a label at the most right position of the
 scale, additional space is needed to display the overlapping part
 of the label would be taken by reducing the width of scale and canvas.
 To avoid this "jumping canvas" effect, we add a permanent margin.
 We don't need to do the same for the left border, because there
 is enough space for the overlapping label below the left scale.
*/

        QwtScaleWidget* scaleWidget = plot[n]->axisWidget( QwtPlot::xBottom );
        const int fmh = QFontMetrics( scaleWidget->font() ).height();
        scaleWidget->setMinBorderDist( 0, fmh / 2 );
        scaleWidget->setMinBorderDist( 0, 20 );

        plot[n]->setAxisFont(QwtPlot::yLeft, QFont("Arial", 8));
    }

// predefine curve colors for curves
    QColor color[NUMCURVES] = {Qt::blue, Qt::red, Qt::green,
                               Qt::blue, Qt::red, Qt::green };
// basic attributes of QwtPlotCurve pairs
    for ( int m = 0; m < NUMCURVES; m++) {
        curve[m][0]->setPen(QPen(color[m], 1));
        curve[m][1]->setSymbol(new QwtSymbol(QwtSymbol::Ellipse,
                                   QBrush(color[m]),
                                   QPen(color[m]),
                                   QSize(4,4)));
        curve[m][0]->setVisible(true);
        curve[m][1]->setVisible(true);

        xScaleLow = QwtDate::toDouble(dt.addSecs(-xSpanSec));
        xScaleHigh = QwtDate::toDouble(dt);

        data[m].timeData[0] = xScaleHigh;
        data[m].timeDT[0] = dt;

    }
// optionally, setup a timer that repeatedly calls realTimeDummySlot:
    //connect(dataTimer, SIGNAL(timeout()),
            //this, SLOT(realtimeDummySlot()));
    //dataTimer->start(1000);
}

/**
 * @brief StripFrameDouble::realTimeDummySlot
 */
void StripFrameTriple::realTimeDummySlot()
{
    double newDummyData = 230.0 + 0.0002*qrand();
    realTimeSlot(0, QDateTime::currentDateTime(), newDummyData, true);
}

/**
 * @brief update a single curve in triple plot stripchart
 * @param curveIndex: choose from [0...NUMCURVES-1]
 * @param newDT: time stamp of point added
 * @param newData: y-value of point added
 * @param doReplot: choose to perform QwtPlot::replot or not
 */
void StripFrameTriple::realTimeSlot(int curveIndex,
                                    QDateTime newDT, double newData,
                                    bool doReplot)
{
// use new timestamp, increment dataCount
   double newTimeData = QwtDate::toDouble(newDT);
   //qint64 newTimeMSecs = newDT.toMSecsSinceEpoch();
   //qDebug() << newDT.toString() << newTimeData << newTimeMSecs;

    if ( dataCount < history )
        dataCount++;
   //qDebug() << "Data count:" << dataCount;

    // shift time arrays and y-axis data one up
    for ( int i = (history-1); i > 0; i--) {
        data[curveIndex].timeData[i] = data[curveIndex].timeData[i-1];
        data[curveIndex].timeDT[i] = data[curveIndex].timeDT[i-1];
        data[curveIndex].yData[i] = data[curveIndex].yData[i-1];
    }
// replace first element with new data
    data[curveIndex].yData[0] = newData;
    data[curveIndex].timeData[0] = newTimeData;
    data[curveIndex].timeDT[0] = newDT;

    for (int n = 0; n < NUMPLOTS; n++) {
        plot[n]->setAxisScale( QwtPlot::xBottom,
                     QwtDate::toDouble(data[curveIndex].timeDT[0].addSecs(-1.02*xSpanSec)),
                     QwtDate::toDouble(data[curveIndex].timeDT[0].addSecs(0.02*xSpanSec)) );
    }

// pass pointers to timestamps/data to curve with setRawSamples;
// use dataCount points!
        curve[curveIndex][0]->setRawSamples(data[curveIndex].timeData.begin(),
                                            data[curveIndex].yData.begin(),
                                            dataCount );
        curve[curveIndex][1]->setRawSamples(data[curveIndex].timeData.begin(),
                                           data[curveIndex].yData.begin(), 1 );

// check
        //qDebug() << data[curveIndex].timeDT[0].toString("hh:mm:ss:zzz")
             //    << data[curvIndex].timeDT[dataCount-1].toString("hh:mm:ss:zzz")
             //    << data[curveIndex].yData[0]
             //    << data[curveIndex].yData[dataCount-1]
             //    << dataCount;
// replot
    if (doReplot)
        for (int n = 0; n < NUMPLOTS; n++)
            plot[n]->replot();
}

/**
 * @brief update all curves in triple plot stripchart
 * @param newDT: common time stamp of point added
 * @param newDataVector: y-values[NUMCURVES] of point added
 * @param doReplot: choose to perform QwtPlot::replot or not
 */
void StripFrameTriple::realTimeAllSlot(QDateTime newDT,
                                       QVector<double> newDataVector,
                                       bool doReplot)
{
// use new timestamp, increment dataCount
    double newTimeData = QwtDate::toDouble(newDT);
    //qint64 newTimeMSecs = newDT.toMSecsSinceEpoch();
    //qDebug() << newDT.toString() << newTimeData << newTimeMSecs;

    double newData[NUMCURVES];
    for (int m = 0; m < NUMCURVES; m++)
       newData[m] = newDataVector[m];

    if ( dataCount < history )
        dataCount++;
    //qDebug() << "Data count:" << dataCount;

    for (int m = 0; m < NUMCURVES; m++) {
    // shift time arrays and y-axis data one up
        for ( int i = (history-1); i > 0; i--) {
            data[m].timeData[i] = data[m].timeData[i-1];
            data[m].timeDT[i] = data[m].timeDT[i-1];
            data[m].yData[i] = data[m].yData[i-1];
        }
        // replace first element with new data
        data[m].yData[0] = newData[m];
        data[m].timeData[0] = newTimeData;
        data[m].timeDT[0] = newDT;
    }

    for ( int n=0; n < NUMPLOTS; n++) {
        plot[n]->setAxisScale( QwtPlot::xBottom,
                     QwtDate::toDouble(data[0].timeDT[0].addSecs(-1.02*xSpanSec)),
                    QwtDate::toDouble(data[0].timeDT[0].addSecs(0.02*xSpanSec)) );
    }

    for (int m = 0; m < NUMCURVES; m++) {
        // pass pointers to timestamps/data to curve with setRawSamples;
        // use dataCount points!
        curve[m][0]->setRawSamples(data[m].timeData.begin(),
                                   data[m].yData.begin(),
                                   dataCount );
        curve[m][1]->setRawSamples(data[m].timeData.begin(),
                                   data[m].yData.begin(), 1 );

        // check
        //qDebug() << data[m].timeDT[0].toString("hh:mm:ss:zzz")
                 //<< data[m].timeDT[dataCount-1].toString("hh:mm:ss:zzz")
                 //<< data[m].yData[0]
                 // << data[m].yData[dataCount-1]
                 // << dataCount;
    }
    if (doReplot)
        for ( int n=0; n < NUMPLOTS; n++)
            plot[n]->replot();
}

void StripFrameTriple::on_sbSpan_valueChanged(int arg1)
{
    setXSpanSec(static_cast<double>(arg1));
    //history =
    for ( int n=0; n < NUMPLOTS; n++) {
        plot[n]->setAxisScale( QwtPlot::xBottom,
        QwtDate::toDouble(data[n].timeDT[0].addSecs(-1.02*xSpanSec)),
        QwtDate::toDouble(data[n].timeDT[0].addSecs(0.02*xSpanSec)) );
    //if (arg1 < 240)
     //   scaleDraw->setDateFormat(QwtDate::Second, "hh:mm:ss");
   // else
    //    scaleDraw->setDateFormat(QwtDate::Minute, "hh:mm:ss");
    }
}

void StripFrameTriple::setYMinMax(int plotIndex,
                                  const double &minValue,
                                  const double &maxValue)
{
// update private members
    yMin[plotIndex] = minValue;
    yMax[plotIndex] = maxValue;
// configure y-axis
    plot[plotIndex]->setAxisScale(QwtPlot::yLeft, yMin[plotIndex], yMax[plotIndex]);
}

void StripFrameTriple::setXYLabel(int plotIndex,
                                  const QString &xLabel,
                                  const QString &yLabel)
{
    QwtText xText(xLabel);
    xText.setFont(QFont("Arial", 10, QFont::Bold));
    plot[plotIndex]->setAxisTitle( QwtPlot::xBottom, xText);

    QwtText yText(yLabel);
    xText.setFont(QFont("Arial", 10, QFont::Bold));
    plot[plotIndex]->setAxisTitle( QwtPlot::yLeft, yText );
}

void StripFrameTriple::on_btnClear_clicked()
{
    for ( int n=0; n < NUMPLOTS; n++) {
        //curve[n][0]->setRawSamples(timeData.begin(), data.begin(), 0 );
        //curve[n][1]->setRawSamples(timeData.begin(), data.begin(), 0 );
        curve[n][0]->setData( nullptr );
        curve[n][1]->setData( nullptr );
        dataCount = 0;
        plot[n]-> replot();
    }
}

int StripFrameTriple::getHistory() const
{
    return history;
}

void StripFrameTriple::setHistory(int value)
{
    history = value;
    for ( int m = 0; m < NUMCURVES; m++) {
        data[m].timeData.resize(history);
        data[m].timeDT.resize(history);
        data[m].yData.resize(history);
    }
}

void StripFrameTriple::setXSpanSec(double value)
{
    xSpanSec = value;
}

void StripFrameTriple::setAutoY(int plotIndex, bool autovalue)
{
    plot[plotIndex]->setAxisAutoScale(QwtPlot::yLeft, autovalue);
}

void StripFrameTriple::scaleYAxisUp(int plotIndex)
{
    double oldMax = yMax[plotIndex];
    double oldMaxLog = log10(oldMax);
    double newMaxLog = oldMaxLog + 0.1;
    yMax[plotIndex] = pow(10, newMaxLog);
    yMin[plotIndex] = -0.1*yMax[plotIndex];
    plot[plotIndex]->setAxisScale(QwtPlot::yLeft, yMin[plotIndex], yMax[plotIndex]);
}

void StripFrameTriple::scaleYAxisDown(int plotIndex)
{
    double oldMax = yMax[plotIndex];
    double oldMaxLog = log10(oldMax);
    double newMaxLog = oldMaxLog - 0.1;
    yMax[plotIndex] = pow(10, newMaxLog);
    yMin[plotIndex] = -0.1*yMax[plotIndex];
    plot[plotIndex]->setAxisScale(QwtPlot::yLeft, yMin[plotIndex], yMax[plotIndex]);
}

void StripFrameTriple::on_tBtnUp_clicked()
{
    scaleYAxisUp(0);
}

void StripFrameTriple::on_tBtnDown_clicked()
{
    scaleYAxisDown(0);
}

double StripFrameTriple::getYMax(int plotIndex) const
{
    return yMax[plotIndex];
}

double StripFrameTriple::getYMin(int plotIndex) const
{
    return yMin[plotIndex];
}
