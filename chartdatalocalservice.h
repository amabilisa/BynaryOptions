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

    //genarator timer start
    void srartRequestData();

    //returns minimum of data interval
    qreal getDataMinimum() const;

    //returns maximum of data interval
    qreal getDataMaximum() const;

    qreal getLastDataValue() const;

private slots:
    void getNextValue();

private:
    QTimer *_timer;
    int _lastDataValue;
    int _deviation;
    int _dataMinimum;
    int _dataMaximum;
    double getNormalizedRandom();
};

#endif // CHARTDATAMODEL_H
