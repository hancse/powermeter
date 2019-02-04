#include "plotframe2.h"
#include "ui_plotframe2.h"

PlotFrame2::PlotFrame2(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PlotFrame2)
{
    ui->setupUi(this);
    for (int n = 0; n < 3; n++) {
        plot[n] = new QCustomPlot(ui->gbPlot);
        plot[n]->resize(420, 161);
        plot[n]->move(10, 16 + n*166);
    }
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
    ui->plot[n]->setNotAntialiasedElements(QCP::aeAll);
    QFont font;
    font.setStyleStrategy(QFont::NoAntialias);
    ui->plot[n]->xAxis->setTickLabelFont(font);
    ui->plot[n]->yAxis->setTickLabelFont(font);
    ui->plot[n]->legend->setFont(font);
*/
    for (int n = 0; n < 3; n++) {
        plot[n]->addGraph(); // blue line
        plot[n]->graph(0)->setPen(QPen(Qt::blue));
        plot[n]->graph(0)->setAntialiasedFill(false);

        plot[n]->addGraph(); // red line
        plot[n]->graph(1)->setPen(QPen(Qt::red));

        plot[n]->addGraph(); // blue dot
        plot[n]->graph(2)->setPen(QPen(Qt::blue));
        plot[n]->graph(2)->setLineStyle(QCPGraph::lsNone);
        plot[n]->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);

        plot[n]->addGraph(); // red dot
        plot[n]->graph(3)->setPen(QPen(Qt::red));
        plot[n]->graph(3)->setLineStyle(QCPGraph::lsNone);
       plot[n]->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);

// configure x-axis
        QSharedPointer<QCPAxisTickerDateTime> timeTicker(new QCPAxisTickerDateTime);
        timeTicker->setDateTimeFormat("hh:mm:ss");
        plot[n]->xAxis->setTicker(timeTicker);

        double initialKey = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
        plot[n]->xAxis->setRange(initialKey+0.02*xSpan, xSpan, Qt::AlignRight);

    //ui->plot[n]->xAxis->ticker()->setTickCount(0);
        timeTicker->setTickStepStrategy(QCPAxisTickerTime::tssReadability);
        plot[n]->xAxis->setLabel("time");

// configure y-axis
        plot[n]->yAxis->setRange(yMin, yMax);
        plot[n]->yAxis->setLabel("a.u.");

// complete box around plot and
// make left and bottom axes transfer their ranges to right and top axes:
        plot[n]->axisRect()->setupFullAxesBox();
        connect(plot[n]->xAxis, SIGNAL(rangeChanged(QCPRange)),
                plot[n]->xAxis2, SLOT(setRange(QCPRange)));
        connect(plot[n]->yAxis, SIGNAL(rangeChanged(QCPRange)),
                plot[n]->yAxis2, SLOT(setRange(QCPRange)));

// setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
// (dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
// connect(dataTimer, SIGNAL(timeout()), this, SLOT(realtimeTentacleSlot()));
// dataTimer->start(2000); // Interval 0 means to refresh as fast as possible
    }
}

void PlotFrame2::realtimeUSBSlot(int index, double value)
{
// calculate two new data points:
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    //value = value + (rand() % 100)/1000.0;

// add data to lines:
    plot[index]->graph(0)->addData(key, value);

// set data of dots:
    plot[index]->graph(2)->data()->clear();
    plot[index]->graph(2)->addData(key, value);

// remove data of lines that's outside visible range:
    plot[index]->graph(0)->data()->removeBefore(key-xSpan);

// rescale value (vertical) axis to fit the current data:
    //ui->plot[n]->yAxis->setRange(-1000, 1000);

    //ui->plot[n]->graph(0)->rescaleValueAxis();
    //ui->plot[n]->graph(1)->rescaleValueAxis(true);

// make key axis range scroll with the data (at a constant range size of xSpan):
    plot[index]->xAxis->setRange(key+0.02*xSpan, xSpan, Qt::AlignRight);

    plot[index]->replot();
}

void PlotFrame2::on_sbSpan_valueChanged(int arg1)
{
    xSpan = double(arg1);
}

void PlotFrame2::setYMinMax(int index, double valMin, double valMax)
{
    yMin = valMin;
    yMax = valMax;
// configure y-axis
    plot[index]->yAxis->setRange(yMin, yMax);
}

void PlotFrame2::on_btnDetach_clicked(bool checked)
{
    int index = 0;
    if (checked) {
        plot[index]->setParent(pd);
        pd->show();
        ui->btnDetach->setText("Attach");
    } else {
        ui->btnDetach->setText("Detach");
        plot[index]->setParent(ui->gbPlot);
        plot[index]->show();
        pd->hide();
    }
}

void PlotFrame2::setXYLabel(int index, const QString &xValue, const QString &yValue)
{
    xLabel = xValue;
    yLabel = yValue;
// configure axes
    plot[index]->xAxis->setLabel(xLabel);
    plot[index]->yAxis->setLabel(yLabel);
}



