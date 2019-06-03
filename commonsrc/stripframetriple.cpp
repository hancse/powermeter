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

    pd = new QDialog();
    pd->setWindowFlags(Qt::WindowTitleHint);
    //pd->setStyleSheet("font-weight: bold;");  //does not inherit
    QFont font;
    font.setBold(true);
    pd->setFont(font);                          // inheritance OK

    dataTimer = new QTimer(this);
    setXSpanSec(ui->sbSpan->value());

    for ( int n=0; n < NUMPLOTS; n++) {
        plot[n] = new QwtPlot(this);
        plot[n]->move(0, 10 + n*170);

        curve[n][0] = new QwtPlotCurve("RatioLine");
        curve[n][0]->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        curve[n][0]->attach(plot[n]);
        curve[n][1] = new QwtPlotCurve("CurrentRatio");
        curve[n][1]->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        curve[n][1]->attach(plot[n]);

        scaleDraw[n] = new QwtDateScaleDraw(Qt::LocalTime);
        scaleEngine[n] = new QwtDateScaleEngine( Qt::LocalTime );
    }

    setupPlot();
}

StripFrameTriple::~StripFrameTriple()
{
    delete ui;
}

void StripFrameTriple::setupPlot()
{
// basic attributes of QwtPlot
    for ( int n=0; n < NUMPLOTS; n++) {
        plot[n]->resize(440, 160);

        const int margin = 5;
        plot[n]->setContentsMargins( margin, margin, margin, margin );

        plot[n]->setAutoReplot( false );
        plot[n]->setAxisAutoScale(QwtPlot::xBottom, false);
        plot[n]->plotLayout()->setAlignCanvasToScales( true );
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

        QDateTime dt = QDateTime::currentDateTime();
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

        //plot[n]->setAxisTitle( QwtPlot::yLeft, "Cpu Usage [%]" );
        //plot[n]->setAxisScale( QwtPlot::yLeft, 0, 100 );
        plot[n]->setAxisFont(QwtPlot::yLeft, QFont("Arial", 8));

// basic attributes of QwtPlotCurve
        curve[n][0]->setPen(QPen(Qt::blue, 1));
        curve[n][1]->setSymbol(new QwtSymbol(QwtSymbol::Ellipse,
                                   QBrush(Qt::blue),
                                   QPen(Qt::blue),
                                   QSize(4,4)));
        //curve[0]->attach(plot[0]);
        curve[n][0]->setVisible(true);
        //curve[1]->attach(plot[0]);
        curve[n][1]->setVisible(true);

        xScaleLow = QwtDate::toDouble(dt.addSecs(-xSpanSec));
        xScaleHigh = QwtDate::toDouble(dt);
        timeData[0] = xScaleHigh;
        timeDT[0] = dt;
    }
// optionally, setup a timer that repeatedly calls realTimeDummySlot:
//    connect(dataTimer, SIGNAL(timeout()),
//            this, SLOT(realtimeDummySlot()));
//    dataTimer->start(1000);
}

void StripFrameTriple::realTimeDummySlot()
{
    double newDummyData = 230.0 + 0.02*qrand();
    realTimeSlot(0, QDateTime::currentDateTime(), newDummyData);
}

void StripFrameTriple::realTimeSlot(int plotIndex, QDateTime newDT, double newData)
{
// use new timestamp, increment dataCount
   //QDateTime newDT = QDateTime::currentDateTime();
   double newTimeData = QwtDate::toDouble(newDT);
   //qint64 newTimeMSecs = newDT.toMSecsSinceEpoch();
   //qDebug() << newDT.toString() << newTimeData << newTimeMSecs;

   if ( dataCount < history )
       dataCount++;

// shift time arrays one up
    for ( int i = (history-1); i > 0; i--) {
        timeData[i] = timeData[i-1];
        timeDT[i] = timeDT[i-1];
        data[i] = data[i-1];
    }
// replace first element with new data
    data[0] = newData;
    timeData[0] = newTimeData;
    timeDT[0] = newDT;

    plot[plotIndex]->setAxisScale( QwtPlot::xBottom,
                           QwtDate::toDouble(timeDT[0].addSecs(-1.02*xSpanSec)),
                           QwtDate::toDouble(timeDT[0].addSecs(0.02*xSpanSec)) );

// pass pointers to timestamps/data to curve with setRawSamples; use dataCount points!
    curve[plotIndex][0]->setRawSamples(timeData.begin(), data.begin(), dataCount );
    curve[plotIndex][1]->setRawSamples(timeData.begin(), data.begin(), 1 );

// check
    //qDebug() << timeDT[0].toString("hh:mm:ss:zzz")
             //<< timeDT[dataCount-1].toString("hh:mm:ss:zzz")
             //<< data[0] << data[dataCount-1] << dataCount;
// replot
    plot[plotIndex]->replot();
}

void StripFrameTriple::on_sbSpan_valueChanged(int arg1)
{
    setXSpanSec(static_cast<double>(arg1));
    //history =
    plot[0]->setAxisScale( QwtPlot::xBottom,
                            QwtDate::toDouble(timeDT[0].addSecs(-1.02*xSpanSec)),
                            QwtDate::toDouble(timeDT[0].addSecs(0.02*xSpanSec)) );
    //if (arg1 < 240)
     //   scaleDraw->setDateFormat(QwtDate::Second, "hh:mm:ss");
   // else
    //    scaleDraw->setDateFormat(QwtDate::Minute, "hh:mm:ss");
}

void StripFrameTriple::setYMinMax(int plotIndex, const double &minValue, const double &maxValue)
{
// update private members
    yMin[plotIndex] = minValue;
    yMax[plotIndex] = maxValue;
// configure y-axis
    plot[plotIndex]->setAxisScale(QwtPlot::yLeft, yMin[plotIndex], yMax[plotIndex]);
}

void StripFrameTriple::setXYLabel(int plotIndex, const QString &xLabel, const QString &yLabel)
{
    QwtText xText(xLabel);
    xText.setFont(QFont("Arial", 10, QFont::Bold));
    plot[plotIndex]->setAxisTitle( QwtPlot::xBottom, xText);

    QwtText yText(yLabel);
    xText.setFont(QFont("Arial", 10, QFont::Bold));
    plot[plotIndex]->setAxisTitle( QwtPlot::yLeft, yText );
}

void StripFrameTriple::clearPlot(int plotIndex)
{
    //curve[0]->setRawSamples(timeData.begin(), data.begin(), 0 );
    //curve[1]->setRawSamples(timeData.begin(), data.begin(), 0 );
    curve[plotIndex][0]->setData( nullptr );
    curve[plotIndex][1]->setData( nullptr );
    curve[plotIndex][2]->setData( nullptr );
    dataCount = 0;
    plot[plotIndex]-> replot();
}
void StripFrameTriple::on_btnClear_clicked()
{
    for (int n=0; n<3; n++) {
        clearPlot(n);
    }
}

int StripFrameTriple::getHistory() const
{
    return history;
}

void StripFrameTriple::setHistory(int value)
{
    history = value;
    timeData.resize(history);
    timeDT.resize(history);
    data.resize(history);
}

void StripFrameTriple::setXSpanSec(double value)
{
    xSpanSec = value;
}

void StripFrameTriple::on_btnDetach_clicked(bool checked)
{
    if (checked) {
        plot[0]->setParent(pd);
        pd->show();
        ui->btnDetach->setText("Attach");
    } else {
        ui->btnDetach->setText("Detach");
        plot[0]->setParent(this);
        plot[0]->show();
        pd->hide();
    }
}

void StripFrameTriple::setAutoY(bool autovalue)
{
    plot[0]->setAxisAutoScale(QwtPlot::yLeft, autovalue);
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
