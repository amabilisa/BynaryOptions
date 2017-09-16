#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    series = new QLineSeries();

    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle("Current trading active: random");
    chart->setTheme(QChart::ChartThemeDark);

    axisX = new QDateTimeAxis;
    axisX->setTickCount(15);
    axisX->setFormat("mm:ss");
    axisX->setTitleText("Time");

    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Current price");
    axisY->setTickCount(axisX->tickCount() / 2);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    this->setCentralWidget(chartView);

    chartDataModel = new ChartDataLocalService(100, 30);
    chartDataModel->srartRequestData();
    resize(800, 600);

    connect(chartDataModel, SIGNAL(haveNewData(double,QDateTime)), this, SLOT(onTakeNewValue(double,QDateTime)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::onTakeNewValue(double data, QDateTime dateTime)
{

    qDebug() << "onTakeNewValue" << data << dateTime << dateTime.toMSecsSinceEpoch();

    series->append(dateTime.toMSecsSinceEpoch(), data);
    axisX->setRange(dateTime, dateTime.addSecs(SECONDS_TO_SHOW_ON_PLOT));
    axisY->setRange(chartDataModel->getDataMinimum(), chartDataModel->getDataMaximum());
    qreal x = chart->plotArea().width() / 2;

    chart->scroll(-x, 0);
}
