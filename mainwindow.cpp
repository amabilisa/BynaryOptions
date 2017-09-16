#include "mainwindow.h"
//#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)/*,
    ui(new Ui::MainWindow)*/
{
//    ui->setupUi(this);



    series = new QLineSeries();

//    QDateTime dateTime = QDateTime::currentDateTime();


//    series->append(dateTime.toMSecsSinceEpoch(), 100);
//    dateTime = dateTime.addSecs(1);
//    series->append(dateTime.toMSecsSinceEpoch(), 120);
//    dateTime = dateTime.addSecs(1);
//    series->append(dateTime.toMSecsSinceEpoch(), 90);
//    dateTime = dateTime.addSecs(1);
//    series->append(dateTime.toMSecsSinceEpoch(), 88);
//    dateTime = dateTime.addSecs(1);
//    series->append(dateTime.toMSecsSinceEpoch(), 103);
//    *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle("Simple line chart example");
    chart->setTheme(QChart::ChartThemeDark);

    axisX = new QDateTimeAxis;
    axisX->setTickCount(15);
    axisX->setFormat("mm:ss");
    axisX->setTitleText("Date");

    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Sunspots count");
//    axisY->setTickCount(axisX->tickCount());
//    axisY->setRange(60, 140);
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
//    delete ui;
}

void MainWindow::onTakeNewValue(double data, QDateTime dateTime)
{

    qDebug() << "onTakeNewValue" << data << dateTime << dateTime.toMSecsSinceEpoch();

//    axisX->setRange(dateTime, dateTime.addSecs(axisX->tickCount()));


    series->append(dateTime.toMSecsSinceEpoch(), data);
    axisX->setRange(dateTime, dateTime.addSecs(120));
    axisY->setRange(chartDataModel->getDataMinimum(), chartDataModel->getDataMaximum());
    qreal x = chart->plotArea().width() / 2;// / axisX->tickCount();
//    x = 100;

//    qDebug() << "x:" << x;
    chart->scroll(-x, 0);
}
