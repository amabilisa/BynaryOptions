#include <QStyledItemDelegate>

#include "historywidget.h"
#include "tools.h"

HistoryWidget::HistoryWidget(QWidget *parent) : QWidget(parent)
{
//    setWindowFlag(Qt::Popup);
//    setWindowFlag(Qt::FramelessWindowHint);
//    setModal(false);
    setWindowFlags(Qt::Dialog);
    setObjectName("messageForUser");

    QVBoxLayout *mainLO = new QVBoxLayout;
    mainLO->setContentsMargins(0, 0, 0, 0);

    historyListView = new QListView;
    historyListView->setItemDelegate(new QStyledItemDelegate);
    historyListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    historyListView->setAlternatingRowColors(true);
    listModel = new QStringListModel;

    QHBoxLayout *buttonLO = new QHBoxLayout;
    buttonLO->setContentsMargins(0, 16, 16, 16);
    okButton = new QPushButton(tr("OK"));
    buttonLO->addStretch();
    buttonLO->addWidget(okButton);

    mainLO->addWidget(historyListView);
    mainLO->addLayout(buttonLO);

    setLayout(mainLO);

    connect(okButton, SIGNAL(clicked()), this, SLOT(onOkButttonClicked()));
}

HistoryWidget::~HistoryWidget()
{

}

void HistoryWidget::setHistory(QMap<QDateTime, Deal *> historyOfDeals)
{
    QStringList historyList;
    foreach (auto dealDateTime, historyOfDeals.keys()) {
        Deal *deal = historyOfDeals.value(dealDateTime);
        historyList << tr("Сделка от ") +
                       dealDateTime.toString("dd MMMM yyyy hh:mm:ss.zzz") +
                       tr(" доход: ") +
                       getDollarsString(deal->_won);

    }

    listModel->setStringList(historyList);
    historyListView->setModel(listModel);
    historyListView->setFixedWidth(400);
}

void HistoryWidget::onOkButttonClicked()
{
    this->close();
}
