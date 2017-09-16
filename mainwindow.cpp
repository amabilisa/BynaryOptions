#include <QStyledItemDelegate>
#include <QIntValidator>

#include "mainwindow.h"
#include "tools.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    setObjectName("mainWindow");



    dealsProvider = new DealsProvider;

    settings = new QSettings("settings.conf", QSettings::NativeFormat);
    dealsProvider->setBalance(settings->value("balance/current_balance").toDouble());

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

    QLabel *nameOfBalanceLabel = new QLabel(tr("Счёт:"));

    balanceLabel = new QLabel;
    balanceLabel->setText(getDollarsString(dealsProvider->getBalance()));
    balanceLabel->setObjectName("balanceLabel");

    QHBoxLayout *balanceLabelsLO = new QHBoxLayout;

    balanceLabelsLO->setContentsMargins(0, 0, 0, 0);

    balanceLabelsLO->addStretch();
    balanceLabelsLO->addWidget(nameOfBalanceLabel);
    balanceLabelsLO->addWidget(balanceLabel);

    priceOfDealCombo = new QComboBox;
    priceOfDealCombo->setObjectName("priceOfDealCombo");

    priceOfDealCombo->addItem("10");
    priceOfDealCombo->addItem("25");
    priceOfDealCombo->addItem("50");
    priceOfDealCombo->addItem("100");

    priceOfDealCombo->setEditable(true);
    priceOfDealCombo->lineEdit()->setAlignment(Qt::AlignRight);

    QStyledItemDelegate *comboboxDelegete = new QStyledItemDelegate;
    priceOfDealCombo->setItemDelegate(comboboxDelegete);

    QIntValidator *priceValidator = new QIntValidator(1, 999);
    priceOfDealCombo->lineEdit()->setValidator(priceValidator);

    fixComboBoxDropDownListSizeAdjustment(priceOfDealCombo);

    QHBoxLayout *priceOfDealLO =  new QHBoxLayout;
    priceOfDealLO->addStretch();
    priceOfDealLO->addWidget(priceOfDealCombo);
    priceOfDealLO->addStretch();

    QVBoxLayout *rightMenuLO = new QVBoxLayout;

    rightMenuLO->addLayout(balanceLabelsLO);
    rightMenuLO->addLayout(priceOfDealLO);
    rightMenuLO->addStretch();
    rightMenuLO->addWidget(dealUpButton);
    rightMenuLO->addWidget(dealDownButton);
    rightMenuLO->setContentsMargins(0, 13, 33, 38);
    rightMenuLO->setSpacing(15);

    QHBoxLayout *mainLO = new QHBoxLayout;
    QWidget *mainWidget =  new QWidget;

    mainLO->addWidget(chartView, 10);
    mainLO->addLayout(rightMenuLO, 1);

    mainLO->setContentsMargins(0, 0, 0, 0);
    mainLO->setSpacing(0);

    mainWidget->setLayout(mainLO);

    this->setCentralWidget(mainWidget);
    resize(900, 600);

    connect(chartDataModel, SIGNAL(haveNewData(double,QDateTime)), this, SLOT(onTakeNewValue(double,QDateTime)));
}

MainWindow::~MainWindow()
{
    delete dealsProvider;
    delete settings;
    delete series;
    delete chartDataModel;
}

void MainWindow::onTakeNewValue(double data, QDateTime dateTime)
{

//    qDebug() << "onTakeNewValue" << data << dateTime << dateTime.toMSecsSinceEpoch();

    series->append(dateTime.toMSecsSinceEpoch(), data);
    axisX->setRange(dateTime, dateTime.addSecs(SECONDS_TO_SHOW_ON_PLOT));
    axisY->setRange(chartDataModel->getDataMinimum(), chartDataModel->getDataMaximum());
    qreal x = chart->plotArea().width() / 2;

    dealsProvider->setBalance(dealsProvider->getBalance() + data);
    settings->setValue("balance/current_balance", dealsProvider->getBalance());
    settings->sync();
    balanceLabel->setText(getDollarsString(dealsProvider->getBalance()));

    qDebug() << "current balance:" << settings->value("balance/current_balance").toDouble();

    chart->scroll(-x, 0);
}
