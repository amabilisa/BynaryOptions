#include "chartdatalocalservice.h"

ChartDataLocalService::ChartDataLocalService(int startValue, int deviation):AbstractChartDataService(),
  _startValue(startValue), _deviation(deviation)
{

    _timer = new QTimer();
    connect(_timer, SIGNAL(timeout()), this, SLOT(getNextValue()));
}

void ChartDataLocalService::getChartData()
{
    _timer->start(1000); // 1 value per second
}

void ChartDataLocalService::getNextValue()
{
    qDebug() << "value:" << _startValue + 2 * _deviation * (getNormalizedRandom() - 0.5);
}

double ChartDataLocalService::getNormalizedRandom()
{
    return (double)qrand() / RAND_MAX;
}



