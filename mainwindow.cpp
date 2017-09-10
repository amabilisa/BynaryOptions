#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chartdatalocalservice.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    AbstractChartDataService *chartDataModel = new ChartDataLocalService(100, 30);
    chartDataModel->getChartData();
}

MainWindow::~MainWindow()
{
    delete ui;
}
