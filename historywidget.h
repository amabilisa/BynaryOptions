#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QtCore>
#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListView>
#include <QStringListModel>
#include <QPushButton>

#include "dealsprovider.h"

class HistoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HistoryWidget(QWidget *parent = nullptr);
    ~HistoryWidget();

    void setHistory(QMap <QDateTime, Deal*> historyOfDeals);

private:
    QListView *historyListView;
    QPushButton *okButton;
    QStringListModel *listModel;

private slots:
    void onOkButttonClicked();
};

#endif // HISTORYWIDGET_H
