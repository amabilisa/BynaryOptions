#ifndef DEALSPROVIDER_H
#define DEALSPROVIDER_H

#include <QObject>

enum DealType {
    DealUp,
    DealDown
};

struct Deal {
    double quote;
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

    double getBalance() const;
    void setBalance(const double &getBalance);

signals:

public slots:

private:
    double _balance;
    int _expirationTime;
    QList <Deal> _deals;

};

#endif // DEALSPROVIDER_H
