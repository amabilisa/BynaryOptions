#ifndef CHARTDATAMODEL_H
#define CHARTDATAMODEL_H

#include "abstractchartdataservice.h"

#include <QDebug>
#include <QTimer>

class ChartDataLocalService: public AbstractChartDataService
{
    Q_OBJECT
public:
    ChartDataLocalService(int startValue, int deviation);
    void getChartData();

private slots:
    void getNextValue();

private:
    QTimer *_timer;
    int _startValue;
    int _deviation;
    double getNormalizedRandom();
};

#endif // CHARTDATAMODEL_H
