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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
//    Ui::MainWindow *ui;
    QChart *chart;
    QDateTimeAxis *axisX;
    QValueAxis *axisY;
    QLineSeries *series;

    AbstractChartDataService *chartDataModel;

private slots:
    void onTakeNewValue(double data, QDateTime dateTime);
};

#endif // MAINWINDOW_H
