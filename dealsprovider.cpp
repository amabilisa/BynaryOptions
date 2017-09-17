#include <QDebug>

#include "dealsprovider.h"
#include "mainwindow.h"
#include "chartdatalocalservice.h"

DealsProvider::DealsProvider(MainWindow *mainWindow, QObject *parent) : QObject(parent),
    _mainWindow(mainWindow)
{
    _expirationTimeFrame = 60 /*seconds*/;

    _settings = new QSettings("bynarysettings.conf", QSettings::NativeFormat);

//    if (_settings->value("balance/current_balance", -1).toDouble() < 0) {
        this->setBalance(1000 * 100);
//    } else {
//        this->setBalance(_settings->value("balance/current_balance").toDouble());
//    }

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

    qDebug() << "current:" << QDateTime::fromMSecsSinceEpoch(currentTimeInMSeconds);
    qDebug() << "expiration:" << expirationTime;

    qDebug() << "timer delay:" << timerDelay << "seconds:" << timerDelay/1000;

    _oneExpirationIntervalTimer->start(timerDelay);
}

void DealsProvider::addDeal(QDateTime dateTimeOFDeal, Deal *deal)
{    

    if (_expirationTimeFrame > 0) {

        qint64 datetimeOfDealInSeconds = dateTimeOFDeal.toSecsSinceEpoch();

        qint64 expirationTimeInSeconds = datetimeOfDealInSeconds - datetimeOfDealInSeconds % _expirationTimeFrame + _expirationTimeFrame;

        deal->_expirationTime = QDateTime::fromSecsSinceEpoch(expirationTimeInSeconds);

        _activeDeals.insert(dateTimeOFDeal, deal);

        qDebug() << "deal added:" << deal->_price << dateTimeOFDeal;
        qDebug() << "time of end:" << deal->_expirationTime;
    }
}


double DealsProvider::getBalance() const
{
    return _balance;
}

void DealsProvider::setBalance(const double &balance)
{
    _balance = balance;
//    _settings->setValue("balance/current_balance", this->getBalance());
//    _settings->sync();

    emit needChangeBalance(balance);
}

void DealsProvider::checkActiveDeals()
{
    qDebug() << ">>>>>>>> expiration timer shot <<<<<<<<" << QDateTime::currentDateTime();
    if (_oneExpirationIntervalTimer->isSingleShot()) {
        _oneExpirationIntervalTimer->setSingleShot(false);
        _oneExpirationIntervalTimer->start(_expirationTimeFrame * 1000);
    }

    qreal lastChartValue = _mainWindow->getChartDataModel()->getLastDataValue();

    if (_activeDeals.isEmpty())
        emit needMoreDeals();

    foreach (auto deal, _activeDeals) {
//        qDebug() << "deal:" << _activeDeals.value(dateTimeOfDeal)->_expirationTime;
        if (abs(deal->_expirationTime.secsTo(QDateTime::currentDateTime())) < 1) {

            switch (deal->_dealType) {
            case DealUp:
                if (lastChartValue > deal->_quote) {
                    deal->_won = deal->_price * 0.8;

                    _hystoryOfDeals.insert(_activeDeals.key(deal), deal);
                    _activeDeals.remove(_activeDeals.key(deal));

                    this->setBalance(_balance += deal->_won);
                }
                break;
            case DealDown:
                if (lastChartValue < deal->_quote) {
                    deal->_won = deal->_price * 0.8;

                    _hystoryOfDeals.insert(_activeDeals.key(deal), deal);
                    _activeDeals.remove(_activeDeals.key(deal));

                    this->setBalance(_balance += deal->_won);
                }
                break;
            }
        }
    }
}
