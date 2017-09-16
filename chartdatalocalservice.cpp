#include "chartdatalocalservice.h"

ChartDataLocalService::ChartDataLocalService(int startValue, int deviation):AbstractChartDataService(),
  _previousValue(startValue), _dataMinimum(startValue), _dataMaximum(startValue), _deviation(deviation)
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
    _previousValue = _previousValue + 2 * _deviation * (getNormalizedRandom() - 0.5);
    _dataMinimum = _dataMinimum < _previousValue ? _dataMinimum : _previousValue;
    _dataMaximum = _dataMaximum > _previousValue ? _dataMaximum : _previousValue;
    emit haveNewData(_previousValue, QDateTime::currentDateTime());
}

int ChartDataLocalService::getDataMaximum() const
{
    return _dataMaximum;
}

int ChartDataLocalService::getDataMinimum() const
{
    return _dataMinimum;
}

double ChartDataLocalService::getNormalizedRandom()
{
    return (double)qrand() / RAND_MAX;
}



