#include <QStyledItemDelegate>
#include <QIntValidator>
#include <QPoint>

#include "mainwindow.h"
#include "tools.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    setObjectName("mainWindow");

    messageForUser.setWindowFlag(Qt::Popup);
    messageForUser.setWindowFlag(Qt::FramelessWindowHint);
    messageForUser.setModal(false);
    messageForUser.setObjectName("messageForUser");

    dealsProvider = new DealsProvider(this);

    dealsProvider->setExpirationTimeFrame(60);

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

    //service of getting data to chart
    chartDataModel = new ChartDataLocalService(100, 30);
    chartDataModel->srartRequestData(); //перенести в swov event 

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

    QLabel *nameCountDeals = new QLabel(tr("Активных сделок:"));
    countOfActiveDealsLabel = new QLabel ("0");
    countOfActiveDealsLabel->setObjectName("countOfActiveDealsLabel");

    QHBoxLayout *activeDealsLO = new QHBoxLayout;

    activeDealsLO->addWidget(nameCountDeals);
    activeDealsLO->addWidget(countOfActiveDealsLabel);

    showDealsHistoryButton = new QPushButton(tr("История (0)"));
    showDealsHistoryButton->setObjectName("showDealsHistoryButton");

    dealUpButton = new QPushButton("выше");
    dealUpButton->setObjectName("dealUpButton");

    dealDownButton = new QPushButton("ниже");
    dealDownButton->setObjectName("dealDownButton");

    QVBoxLayout *rightMenuLO = new QVBoxLayout;

    rightMenuLO->addLayout(balanceLabelsLO);
    rightMenuLO->addLayout(priceOfDealLO);
    rightMenuLO->addStretch();
    rightMenuLO->addLayout(activeDealsLO);
    rightMenuLO->addWidget(showDealsHistoryButton);
    rightMenuLO->addStretch();
    rightMenuLO->addWidget(dealUpButton);
    rightMenuLO->addWidget(dealDownButton);
    rightMenuLO->setContentsMargins(0, 13, 33, 38);
    rightMenuLO->setSpacing(13);

    QHBoxLayout *mainLO = new QHBoxLayout;
    mainWidget =  new QWidget;

    mainLO->addWidget(chartView, 10);
    mainLO->addLayout(rightMenuLO, 1);

    mainLO->setContentsMargins(0, 0, 0, 0);
    mainLO->setSpacing(0);

    mainWidget->setLayout(mainLO);

    this->setCentralWidget(mainWidget);
    resize(1000, 600);

    historyWidget = new HistoryWidget(this);


    connect(chartDataModel, SIGNAL(haveNewData(double,QDateTime)), this, SLOT(onTakeNewValue(double,QDateTime)));
    connect(dealUpButton, SIGNAL(clicked()), this, SLOT(onGetDealUp()));
    connect(dealDownButton, SIGNAL(clicked()), this, SLOT(onGetDealDown()));
    connect(dealsProvider, SIGNAL(needChangeBalance(int)), this, SLOT(onChangeBalance(int)));
    connect(dealsProvider, SIGNAL(needMoreDeals()), this, SLOT(onNoDealsOnExpirationTime()));
    connect(showDealsHistoryButton, SIGNAL(clicked()), this, SLOT(onOpenHistoryButton()));
}

MainWindow::~MainWindow()
{
    delete dealsProvider;
    delete series;
    delete chartDataModel;
}

AbstractChartDataService *MainWindow::getChartDataModel() const
{
    return chartDataModel;
}

void MainWindow::showEvent(QShowEvent *event)
{
    messageForUser.move(mapToGlobal( mainWidget->rect().topLeft()) );
    QMainWindow::showEvent(event);
}


void MainWindow::onTakeNewValue(double data, QDateTime dateTime)
{

    _currentData = data;

    series->append(dateTime.toMSecsSinceEpoch(), data);
    axisX->setRange(dateTime, dateTime.addSecs(SECONDS_TO_SHOW_ON_PLOT));
    axisY->setRange(chartDataModel->getDataMinimum(), chartDataModel->getDataMaximum());
    qreal x = chart->plotArea().width() * 0.785;

    balanceLabel->setText(getDollarsString(dealsProvider->getBalance()));

    chart->scroll(-x, 0);
}

void MainWindow::onGetDealUp()
{
    int price = priceOfDealCombo->currentText().toInt();

    if ((dealsProvider->getBalance() - price * 100) > 0) {
        dealsProvider->setBalance(dealsProvider->getBalance() - price * 100);
        dealsProvider->addDeal(QDateTime::currentDateTime(), new Deal(_currentData, price, DealUp));
        countOfActiveDealsLabel->setText(QString::number(dealsProvider->getActiveDeals().count()));
    } else {
        messageForUser.setText(tr("Недостаточно средств для совершения сделки!"));
        messageForUser.show();
    }
}

void MainWindow::onGetDealDown()
{
    int price = priceOfDealCombo->currentText().toInt();
    if ((dealsProvider->getBalance() - price * 100) > 0) {
        dealsProvider->setBalance(dealsProvider->getBalance() - price * 100);
        dealsProvider->addDeal(QDateTime::currentDateTime(), new Deal(_currentData, price, DealDown));
        countOfActiveDealsLabel->setText(QString::number(dealsProvider->getActiveDeals().count()));
    } else {
        messageForUser.setText(tr("Недостаточно средств для совершения сделки!"));
        messageForUser.show();
    }
}

void MainWindow::onChangeBalance(int addToBalance)
{
    countOfActiveDealsLabel->setText(QString::number(dealsProvider->getActiveDeals().count()));

    showDealsHistoryButton->setText(tr("История (") +
                                    QString::number(dealsProvider->getHystoryOfDeals().count()) + ")");
    if (addToBalance > 0) {
        balanceLabel->setText(getDollarsString(dealsProvider->getBalance()));
        messageForUser.setText(tr("Ваш заработок:") + getDollarsString(addToBalance));
    } else {
         messageForUser.setText(tr("К сожалению, Вы ничего не заработали. Не отчаивайтесь! Совершите сделку!"));
    }
    messageForUser.show();
}

void MainWindow::onNoDealsOnExpirationTime()
{
    countOfActiveDealsLabel->setText(QString::number(dealsProvider->getActiveDeals().count()));
    messageForUser.setText(tr("Вы не заключили ни одной сделки. Очень жаль. Вы могли бы заработать ") +
                           getDollarsString(priceOfDealCombo->currentText().toInt() * 100 * 1.8));
    messageForUser.show();
}

void MainWindow::onOpenHistoryButton()
{
    historyWidget->setHistory(dealsProvider->getHystoryOfDeals());
    historyWidget->show();
}
