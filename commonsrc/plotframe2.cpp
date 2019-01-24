#include "plotframe2.h"
#include "ui_plotframe2.h"

PlotFrame2::PlotFrame2(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PlotFrame2)
{
    ui->setupUi(this);

    pd = new QDialog();
    pd->setWindowFlags(Qt::WindowTitleHint);

    dataTimer = new QTimer(this);
    xSpan = ui->sbSpan->value();

    setupPlot3();
}

PlotFrame2::~PlotFrame2()
{
    delete ui;
}

void PlotFrame2::setupPlot3()
{
// include this section to fully disable antialiasing for higher performance:
/*
    ui->customPlot->setNotAntialiasedElements(QCP::aeAll);
    QFont font;
    font.setStyleStrategy(QFont::NoAntialias);
    ui->customPlot->xAxis->setTickLabelFont(font);
    ui->customPlot->yAxis->setTickLabelFont(font);
    ui->customPlot->legend->setFont(font);
*/
    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(0)->setAntialiasedFill(false);

    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));

    ui->customPlot->addGraph(); // blue dot
    ui->customPlot->graph(2)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);

    ui->customPlot->addGraph(); // red dot
    ui->customPlot->graph(3)->setPen(QPen(Qt::red));
    ui->customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);

// configure x-axis
    QSharedPointer<QCPAxisTickerDateTime> timeTicker(new QCPAxisTickerDateTime);
    timeTicker->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->xAxis->setTicker(timeTicker);

    double initialKey = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    ui->customPlot->xAxis->setRange(initialKey+0.02*xSpan, xSpan, Qt::AlignRight);

    //ui->customPlot->xAxis->ticker()->setTickCount(0);
    timeTicker->setTickStepStrategy(QCPAxisTickerTime::tssReadability);
    ui->customPlot->xAxis->setLabel("time");

// configure y-axis
    ui->customPlot->yAxis->setRange(yMin, yMax);
    ui->customPlot->yAxis->setLabel("a.u.");

// complete box around plot and
// make left and bottom axes transfer their ranges to right and top axes:
    ui->customPlot->axisRect()->setupFullAxesBox();
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)),
            ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)),
            ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

// setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
// (dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
// connect(dataTimer, SIGNAL(timeout()), this, SLOT(realtimeTentacleSlot()));
// dataTimer->start(2000); // Interval 0 means to refresh as fast as possible
}

void PlotFrame2::realtimeUSBSlot(double value)
{
// calculate two new data points:
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    //value = value + (rand() % 100)/1000.0;

// add data to lines:
    ui->customPlot->graph(0)->addData(key, value);

// set data of dots:
    ui->customPlot->graph(2)->data()->clear();
    ui->customPlot->graph(2)->addData(key, value);

// remove data of lines that's outside visible range:
    ui->customPlot->graph(0)->data()->removeBefore(key-xSpan);

// rescale value (vertical) axis to fit the current data:
    //ui->customPlot->yAxis->setRange(-1000, 1000);

    //ui->customPlot->graph(0)->rescaleValueAxis();
    //ui->customPlot->graph(1)->rescaleValueAxis(true);

// make key axis range scroll with the data (at a constant range size of xSpan):
    ui->customPlot->xAxis->setRange(key+0.02*xSpan, xSpan, Qt::AlignRight);

    ui->customPlot->replot();
}

void PlotFrame2::on_sbSpan_valueChanged(int arg1)
{
    xSpan = double(arg1);
}

void PlotFrame2::setYMinMax(double valMin, double valMax)
{
    yMin = valMin;
    yMax = valMax;
// configure y-axis
    ui->customPlot->yAxis->setRange(yMin, yMax);
}

void PlotFrame2::on_btnDetach_clicked(bool checked)
{
    if (checked) {
        ui->customPlot->setParent(pd);
        pd->show();
        ui->btnDetach->setText("Attach");
    } else {
        ui->btnDetach->setText("Detach");
        ui->customPlot->setParent(ui->gbPlot);
        ui->customPlot->show();
        pd->hide();
    }
}

void PlotFrame2::setXYLabel(const QString &xValue, const QString &yValue)
{
    xLabel = xValue;
    yLabel = yValue;
// configure axes
    ui->customPlot->xAxis->setLabel(xLabel);
    ui->customPlot->yAxis->setLabel(yLabel);
}



