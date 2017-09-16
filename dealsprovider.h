#ifndef DEALSPROVIDER_H
#define DEALSPROVIDER_H

#include <QObject>

enum DealType {
    DealUp,
    DealDown
};

struct Deal {
    qreal quote;
    int price;
    DealType dealType;
};

class DealsProvider : public QObject
{
    Q_OBJECT
public:
    explicit DealsProvider(QObject *parent = nullptr);

    int expirationTime() const;
    void setExpirationTime(int expirationTime);

    void addDeal(Deal deal);

    qreal balance() const;
    void setBalance(const qreal &balance);

signals:

public slots:

private:
    qreal _balance;
    int _expirationTime;
    QList <Deal> _deals;

};

#endif // DEALSPROVIDER_H
