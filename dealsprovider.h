#ifndef DEALSPROVIDER_H
#define DEALSPROVIDER_H

#include <QObject>
#include <QDateTime>
#include <QtCore>
#include <QTimer>
#include <QSettings>

class MainWindow;

enum DealType {
    DealUp,
    DealDown
};

struct Deal {
    Deal(double quote, int price, DealType dealType):
        _quote(quote), _price(price), _dealType(dealType) {}
    double _quote;
    int _price;
    int _won;
    QDateTime _expirationTime;
    DealType _dealType;
};

class DealsProvider : public QObject
{
    Q_OBJECT
public:
    explicit DealsProvider(MainWindow *mainWindow, QObject *parent = nullptr);
    ~DealsProvider();

    int expirationTimeFrame() const;
    void setExpirationTimeFrame(int expirationTimeFrame);

    void addDeal(QDateTime dateTimeOFDeal, Deal *deal);

    double getBalance() const;
    void setBalance(const double &getBalance);

    QMap<QDateTime, Deal *> getActiveDeals() const;

    QMap<QDateTime, Deal *> getHystoryOfDeals() const;

signals:
    void needChangeBalance(int money);
    void needMoreDeals();

private:

    QSettings *_settings;

    double _balance;
    int _expirationTimeFrame;

    QTimer *_oneExpirationIntervalTimer; // per one expiration time timer
    MainWindow *_mainWindow;

    QMap <QDateTime, Deal*> _activeDeals;
    QMap <QDateTime, Deal*> _hystoryOfDeals;

private slots:
    void checkActiveDeals();

};

#endif // DEALSPROVIDER_H
