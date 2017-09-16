#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QLineSeries>
#include <QChart>
#include <QDateTime>

#include <QChartView>
#include <QDateTimeAxis>
#include <QValueAxis>

#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSettings>

#include "chartdatalocalservice.h"
#include "dealsprovider.h"

QT_CHARTS_USE_NAMESPACE

#define SECONDS_TO_SHOW_ON_PLOT 180

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QSettings *settings;

    QChart *chart;
    QDateTimeAxis *axisX;
    QValueAxis *axisY;
    QLineSeries *series;

    DealsProvider *dealsProvider;
    AbstractChartDataService *chartDataModel;

    QLabel *balanceLabel;
    QComboBox *priceOfDealCombo;
    QPushButton *dealUpButton;
    QPushButton *dealDownButton;


private slots:
    void onTakeNewValue(double data, QDateTime dateTime);
};

#endif // MAINWINDOW_H
