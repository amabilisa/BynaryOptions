#include "chartdatalocalservice.h"

ChartDataLocalService::ChartDataLocalService(int startValue, int deviation):AbstractChartDataService(),
  _lastDataValue(startValue), _dataMinimum(startValue), _dataMaximum(startValue), _deviation(deviation)
{

    _timer = new QTimer();
    connect(_timer, SIGNAL(timeout()), this, SLOT(getNextValue()));
}

void ChartDataLocalService::srartRequestData()
{
    _timer->setInterval(1000); // 1 value per second
    _timer->start();
}

void ChartDataLocalService::getNextValue()
{
    _lastDataValue = _lastDataValue + 2 * _deviation * (getNormalizedRandom() - 0.5);
    _dataMinimum = _dataMinimum < _lastDataValue ? _dataMinimum : _lastDataValue;
    _dataMaximum = _dataMaximum > _lastDataValue ? _dataMaximum : _lastDataValue;
    emit haveNewData(_lastDataValue, QDateTime::currentDateTime());
}

qreal ChartDataLocalService::getLastDataValue() const
{
    return (qreal)_lastDataValue;
}

qreal ChartDataLocalService::getDataMaximum() const
{
    return (qreal)_dataMaximum;
}

qreal ChartDataLocalService::getDataMinimum() const
{
    return (qreal)_dataMinimum;
}

double ChartDataLocalService::getNormalizedRandom()
{
    return (double)qrand() / RAND_MAX;
}



