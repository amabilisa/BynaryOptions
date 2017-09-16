#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QLineSeries>
#include <QChart>
#include <QChartView>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QValueAxis>

#include "chartdatalocalservice.h"

QT_CHARTS_USE_NAMESPACE

#define SECONDS_TO_SHOW_ON_PLOT 180

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QChart *chart;
    QDateTimeAxis *axisX;
    QValueAxis *axisY;
    QLineSeries *series;

    AbstractChartDataService *chartDataModel;

private slots:
    void onTakeNewValue(double data, QDateTime dateTime);
};

#endif // MAINWINDOW_H
