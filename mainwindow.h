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

#include "chartdatalocalservice.h"
#include "dealsprovider.h"
#include "binaryoptionsmessagebox.h"

QT_CHARTS_USE_NAMESPACE

#define SECONDS_TO_SHOW_ON_PLOT 180
#define WIN_MULTIPLAYER 1.8

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    AbstractChartDataService *getChartDataModel() const;

protected:
    void showEvent(QShowEvent* event);

private:
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

    QPushButton *showDealsHistoryButton;
    QLabel *countOfActiveDealsLabel;

    BinaryOptionsMessageBox messageForUser;
    QWidget *mainWidget;

    double _currentData;


private slots:
    void onTakeNewValue(double data, QDateTime dateTime);
    void onGetDealUp();
    void onGetDealDown();
    void onChangeBalance(int addToBalance);
    void onNoDealsOnExpirationTime();
};

#endif // MAINWINDOW_H
