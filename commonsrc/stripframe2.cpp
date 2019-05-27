#include "stripframe2.h"
#include "ui_stripframe2.h"

#include <qwt_plot_layout.h>
#include <qwt_scale_widget.h>
#include <qwt_symbol.h>
#include <cmath>

StripFrame2::StripFrame2(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::StripFrame2)
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

    plot[0] = new QwtPlot(this);
    plot[0]->move(0, 20);

    curve[0] = new QwtPlotCurve("RatioLine");
    curve[0]->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    curve[0]->attach(plot[0]);
    curve[1] = new QwtPlotCurve("CurrentRatio");
    curve[1]->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    curve[1]->attach(plot[0]);

    scaleDraw = new QwtDateScaleDraw(Qt::LocalTime);
    scaleEngine = new QwtDateScaleEngine( Qt::LocalTime );

    setupPlot();
}

StripFrame2::~StripFrame2()
{
    delete ui;
}

void StripFrame2::setupPlot()
{
// basic attributes of QwtPlot
    plot[0]->resize(440, 400);

    const int margin = 5;
    plot[0]->setContentsMargins( margin, margin, margin, margin );


    plot[0]->setAutoReplot( false );
    plot[0]->setAxisAutoScale(QwtPlot::xBottom, false);
    plot[0]->plotLayout()->setAlignCanvasToScales( true );

    setXYLabel("time", "voltage ratio (V/V)");
    setYMinMax(0, 50);


//Set x-axis scaling: these are permanent settings for QDateScaleDraw object
    scaleDraw->setDateFormat(QwtDate::Minute, "hh:mm:ss");
    scaleDraw->setDateFormat(QwtDate::Second, "hh:mm:ss");

    plot[0]->setAxisScaleDraw(QwtPlot::xBottom, scaleDraw);
    plot[0]->setAxisScaleEngine(QwtPlot::xBottom, scaleEngine);
    //plot[0]->setAxisScale(QwtPlot::xBottom, xScaleLow, xScaleHigh);

    QDateTime dt = QDateTime::currentDateTime();
    plot[0]->setAxisScale( QwtPlot::xBottom,
                           QwtDate::toDouble(dt.addSecs(-xSpanSec)),
                           QwtDate::toDouble(dt));
    plot[0]->setAxisFont(QwtPlot::xBottom, QFont("Arial", 8));

    //plot[0]->setAxisScale(QwtPlot::xBottom, x1, x2, 24*3600*1000);
    //plot[0]->setAxisLabelRotation( QwtPlot::xBottom, 0.0 );
    //setAxisLabelAlignment( QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom );
    //plot[0]->setAxisLabelAlignment( QwtPlot::xBottom, Qt::AlignHCenter | Qt::AlignBottom );

/*
 In situations, when there is a label at the most right position of the
 scale, additional space is needed to display the overlapping part
 of the label would be taken by reducing the width of scale and canvas.
 To avoid this "jumping canvas" effect, we add a permanent margin.
 We don't need to do the same for the left border, because there
 is enough space for the overlapping label below the left scale.
*/

    QwtScaleWidget* scaleWidget = plot[0]->axisWidget( QwtPlot::xBottom );
    const int fmh = QFontMetrics( scaleWidget->font() ).height();
    scaleWidget->setMinBorderDist( 0, fmh / 2 );
    scaleWidget->setMinBorderDist( 0, 20 );

    plot[0]->setAxisTitle( QwtPlot::yLeft, "Cpu Usage [%]" );
    plot[0]->setAxisScale( QwtPlot::yLeft, 0, 100 );
    plot[0]->setAxisFont(QwtPlot::yLeft, QFont("Arial", 8));

// basic attributes of QwtPlotCurve
    curve[0]->setPen(QPen(Qt::blue, 1));
    curve[1]->setSymbol(new QwtSymbol(QwtSymbol::Ellipse,
                                   QBrush(Qt::blue),
                                   QPen(Qt::blue),
                                   QSize(4,4)));
    //curve[0]->attach(plot[0]);
    curve[0]->setVisible(true);
    //curve[1]->attach(plot[0]);
    curve[1]->setVisible(true);

    xScaleLow = QwtDate::toDouble(dt.addSecs(-xSpanSec));
    xScaleHigh = QwtDate::toDouble(dt);
    timeData[0] = xScaleHigh;
    timeDT[0] = dt;

// optionally, setup a timer that repeatedly calls realTimeDummySlot:
//    connect(dataTimer, SIGNAL(timeout()),
//            this, SLOT(realtimeDummySlot()));
//    dataTimer->start(1000);
}

void StripFrame2::realTimeDummySlot()
{
    double newDummyData = 40.0 + 0.0002*qrand();
    realTimeSlot(QDateTime::currentDateTime(), newDummyData);
}

void StripFrame2::realTimeSlot(QDateTime newDT, double newData)
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

    plot[0]->setAxisScale( QwtPlot::xBottom,
                           QwtDate::toDouble(timeDT[0].addSecs(-1.02*xSpanSec)),
                           QwtDate::toDouble(timeDT[0].addSecs(0.02*xSpanSec)) );

// pass pointers to timestamps/data to curve with setRawSamples; use dataCount points!
    curve[0]->setRawSamples(timeData.begin(), data.begin(), dataCount );
    curve[1]->setRawSamples(timeData.begin(), data.begin(), 1 );

// check
    //qDebug() << timeDT[0].toString("hh:mm:ss:zzz")
             //<< timeDT[dataCount-1].toString("hh:mm:ss:zzz")
             //<< data[0] << data[dataCount-1] << dataCount;
// replot
    plot[0]->replot();
}

void StripFrame2::on_sbSpan_valueChanged(int arg1)
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

void StripFrame2::setYMinMax(const double &minValue, const double &maxValue)
{
// update private members
    yMin = minValue;
    yMax = maxValue;
// configure y-axis
    plot[0]->setAxisScale(QwtPlot::yLeft, yMin, yMax);
}

void StripFrame2::setXYLabel(const QString &xLabel, const QString &yLabel)
{
    QwtText xText(xLabel);
    xText.setFont(QFont("Arial", 10, QFont::Bold));
    plot[0]->setAxisTitle( QwtPlot::xBottom, xText);

    QwtText yText(yLabel);
    xText.setFont(QFont("Arial", 10, QFont::Bold));
    plot[0]->setAxisTitle( QwtPlot::yLeft, yText );
}

void StripFrame2::on_btnClear_clicked()
{
    //curve[0]->setRawSamples(timeData.begin(), data.begin(), 0 );
    //curve[1]->setRawSamples(timeData.begin(), data.begin(), 0 );
    curve[0]->setData( nullptr );
    curve[1]->setData( nullptr );
    dataCount = 0;
    plot[0]-> replot();
}

int StripFrame2::getHistory() const
{
    return history;
}

void StripFrame2::setHistory(int value)
{
    history = value;
    timeData.resize(history);
    timeDT.resize(history);
    data.resize(history);
}

void StripFrame2::setXSpanSec(double value)
{
    xSpanSec = value;
}

void StripFrame2::on_btnDetach_clicked(bool checked)
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

void StripFrame2::setAutoY(bool autovalue)
{
    plot[0]->setAxisAutoScale(QwtPlot::yLeft, autovalue);
}

void StripFrame2::scaleYAxisUp()
{
    double oldMax = yMax;
    double oldMaxLog = log10(oldMax);
    double newMaxLog = oldMaxLog + 0.1;
    yMax = pow(10, newMaxLog);
    yMin = -0.1*yMax;
    plot[0]->setAxisScale(QwtPlot::yLeft, yMin, yMax);
}

void StripFrame2::scaleYAxisDown()
{
    double oldMax = yMax;
    double oldMaxLog = log10(oldMax);
    double newMaxLog = oldMaxLog - 0.1;
    yMax = pow(10, newMaxLog);
    yMin = -0.1*yMax;
    plot[0]->setAxisScale(QwtPlot::yLeft, yMin, yMax);
}

void StripFrame2::on_tBtnUp_clicked()
{
    scaleYAxisUp();
}

void StripFrame2::on_tBtnDown_clicked()
{
    scaleYAxisDown();
}

double StripFrame2::getYMax() const
{
    return yMax;
}

double StripFrame2::getYMin() const
{
    return yMin;
}
