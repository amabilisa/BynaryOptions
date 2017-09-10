#ifndef ABSTRACTCHARTDATASERVICE_H
#define ABSTRACTCHARTDATASERVICE_H

#include <QObject>

class AbstractChartDataService: public QObject
{
public:
    virtual void getChartData() = 0;
};

#endif // ABSTRACTCHARTDATASERVICE_H
