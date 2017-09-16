#include <QSettings>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    setObjectName("mainWindow");

    QSettings *settings = new QSettings("settings.conf", QSettings::NativeFormat);

    series = new QLineSeries();

    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTheme(QChart::ChartThemeDark);
    chart->setContentsMargins(0, 0, 0, 0);

    axisX = new QDateTimeAxis;
    axisX->setTickCount(15);
    axisX->setFormat("mm:ss");

    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTickCount(axisX->tickCount() / 2);
    chart->addAxis(axisY, Qt::AlignRight);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setObjectName("chartView");
    chartView->setContentsMargins(0, 0, 0, 0);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartDataModel = new ChartDataLocalService(100, 30);
    chartDataModel->srartRequestData(); //перенести в swov event

    dealUpButton = new QPushButton("выше");
    dealUpButton->setObjectName("dealUpButton");

    dealDownButton = new QPushButton("ниже");
    dealDownButton->setObjectName("dealDownButton");

    QVBoxLayout *rightMenuLO = new QVBoxLayout;

    rightMenuLO->addStretch();
    rightMenuLO->addWidget(dealUpButton);
    rightMenuLO->addWidget(dealDownButton);
    rightMenuLO->setContentsMargins(0, 13, 33, 38);
    rightMenuLO->setSpacing(15);

    QHBoxLayout *mainLO = new QHBoxLayout;
    QWidget *mainWidget =  new QWidget;

    mainLO->addWidget(chartView);
    mainLO->addLayout(rightMenuLO);

    mainLO->setContentsMargins(0, 0, 0, 0);
    mainLO->setSpacing(0);

    mainWidget->setLayout(mainLO);

    this->setCentralWidget(mainWidget);
    resize(900, 600);

    connect(chartDataModel, SIGNAL(haveNewData(double,QDateTime)), this, SLOT(onTakeNewValue(double,QDateTime)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::onTakeNewValue(double data, QDateTime dateTime)
{

//    qDebug() << "onTakeNewValue" << data << dateTime << dateTime.toMSecsSinceEpoch();

    series->append(dateTime.toMSecsSinceEpoch(), data);
    axisX->setRange(dateTime, dateTime.addSecs(SECONDS_TO_SHOW_ON_PLOT));
    axisY->setRange(chartDataModel->getDataMinimum(), chartDataModel->getDataMaximum());
    qreal x = chart->plotArea().width() / 2;

    chart->scroll(-x, 0);
}
