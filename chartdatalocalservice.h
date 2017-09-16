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
    int getDataMinimum() const;

    //returns maximum of data interval
    int getDataMaximum() const;

private slots:
    void getNextValue();

private:
    QTimer *_timer;
    int _previousValue;
    int _deviation;
    int _dataMinimum;
    int _dataMaximum;
    double getNormalizedRandom();
};

#endif // CHARTDATAMODEL_H
