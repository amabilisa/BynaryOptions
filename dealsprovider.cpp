#include <QDebug>

#include "dealsprovider.h"
#include "mainwindow.h"
#include "chartdatalocalservice.h"

DealsProvider::DealsProvider(MainWindow *mainWindow, QObject *parent) : QObject(parent),
    _mainWindow(mainWindow)
{
    _expirationTimeFrame = 60 /*seconds*/;

    _settings = new QSettings("bynarysettingsconfig1.conf", QSettings::NativeFormat);

    if (_settings->value("balance/current_balance", -1).toDouble() < 0) {
        this->setBalance(1000 * 100);
    } else {
        this->setBalance(_settings->value("balance/current_balance").toInt());
    }

    _oneExpirationIntervalTimer = new QTimer();
    _oneExpirationIntervalTimer->setSingleShot(true);
    connect(_oneExpirationIntervalTimer, SIGNAL(timeout()), this, SLOT(checkActiveDeals()));
}

DealsProvider::~DealsProvider()
{
    delete _settings;
    delete _oneExpirationIntervalTimer;
}

int DealsProvider::expirationTimeFrame() const
{
    return _expirationTimeFrame;
}

void DealsProvider::setExpirationTimeFrame(int expirationTimeFrame)
{
    _expirationTimeFrame = expirationTimeFrame;

    qint64 currentTimeInMSeconds = QDateTime::currentMSecsSinceEpoch();

    QDateTime expirationTime = QDateTime::fromMSecsSinceEpoch(
                currentTimeInMSeconds - currentTimeInMSeconds % (_expirationTimeFrame * 1000) + _expirationTimeFrame * 1000);

    qint64 timerDelay = QDateTime::fromMSecsSinceEpoch(currentTimeInMSeconds).msecsTo(expirationTime);

    _oneExpirationIntervalTimer->start(timerDelay);
}

void DealsProvider::addDeal(QDateTime dateTimeOFDeal, Deal *deal)
{    

    if (_expirationTimeFrame > 0) {

        qint64 datetimeOfDealInSeconds = dateTimeOFDeal.toSecsSinceEpoch();

        qint64 expirationTimeInSeconds = datetimeOfDealInSeconds - datetimeOfDealInSeconds % _expirationTimeFrame + _expirationTimeFrame;

        deal->_expirationTime = QDateTime::fromSecsSinceEpoch(expirationTimeInSeconds);

        _activeDeals.insert(dateTimeOFDeal, deal);

    }
}


double DealsProvider::getBalance() const
{
    return _balance;
}

void DealsProvider::setBalance(const double &balance)
{
    _balance = balance;
    _settings->setValue("balance/current_balance", this->getBalance());
    _settings->sync();
}

void DealsProvider::checkActiveDeals()
{
    if (_oneExpirationIntervalTimer->isSingleShot()) {
        _oneExpirationIntervalTimer->setSingleShot(false);
        _oneExpirationIntervalTimer->start(_expirationTimeFrame * 1000);
    }

    qreal lastChartValue = _mainWindow->getChartDataModel()->getLastDataValue();

    int moneyWon = 0;

    foreach (auto dealTime, _activeDeals.keys()) {
        Deal *deal = _activeDeals.value(dealTime);
        if (abs(deal->_expirationTime.secsTo(QDateTime::currentDateTime())) < 1) {

            switch (deal->_dealType) {
            case DealUp:
                if (lastChartValue > deal->_quote) {
                    deal->_won = deal->_price * 1.8;
                }
                break;
            case DealDown:
                if (lastChartValue < deal->_quote) {
                    deal->_won = deal->_price * 1.8;
                }
                break;

            }

            moneyWon += deal->_won;

            _hystoryOfDeals.insert(dealTime, deal);
            _activeDeals.remove(dealTime);

//            qDebug() << "deals:" << dealTime << _hystoryOfDeals.count() << _activeDeals.count();
        }
    }

    if (moneyWon > 0) {
        this->setBalance(_balance += moneyWon);
         emit needChangeBalance(moneyWon);
    } else {
        if (_activeDeals.isEmpty()) {
            qDebug() << "!!!!!!!needMoreDeals!!!!!";
            emit needMoreDeals();
        } else {
            qDebug() << "------needChangeBalance(0)-----" << _activeDeals.count();
            emit needChangeBalance(0);
        }
    }
}
