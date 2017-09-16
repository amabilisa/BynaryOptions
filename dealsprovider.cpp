#include "dealsprovider.h"

DealsProvider::DealsProvider(QObject *parent) : QObject(parent)
{
    _expirationTime = 10 /*seconds*/;
}

int DealsProvider::expirationTime() const
{
    return _expirationTime;
}

void DealsProvider::setExpirationTime(int expirationTime)
{
    _expirationTime = expirationTime;
}

void DealsProvider::addDeal(Deal deal)
{
    _deals << deal;
}

qreal DealsProvider::balance() const
{
    return _balance;
}

void DealsProvider::setBalance(const qreal &balance)
{
    _balance = balance;
}
