#ifndef ABSTRACTCHARTDATASERVICE_H
#define ABSTRACTCHARTDATASERVICE_H

#include <QObject>
#include <QDateTime>

class AbstractChartDataService: public QObject
{
    Q_OBJECT
public:
    virtual void srartRequestData() = 0;
    virtual qreal getDataMinimum() const = 0;
    virtual qreal getDataMaximum() const = 0;
    virtual qreal getLastDataValue() const = 0;

signals:
    void haveNewData(double data, QDateTime dateTime);
};

#endif // ABSTRACTCHARTDATASERVICE_H
