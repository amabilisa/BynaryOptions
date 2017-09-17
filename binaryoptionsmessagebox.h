#ifndef BINARYOPTIONSMESSAGEBOX_H
#define BINARYOPTIONSMESSAGEBOX_H

#include <QObject>
#include <QMessageBox>
#include <QTimer>

class BinaryOptionsMessageBox : public QMessageBox
{
    Q_OBJECT

public:
    BinaryOptionsMessageBox(QWidget *parent = nullptr);
    ~BinaryOptionsMessageBox();

protected:
    void showEvent(QShowEvent *event);

private:
    QTimer _animationReduceOpacityTimer;
    QTimer _delayAnimationTimer;

private slots:
    void onTimerDalayEnded();
    void onOpacityTemerShot();

};

#endif // BINARYOPTIONSMESSAGEBOX_H
