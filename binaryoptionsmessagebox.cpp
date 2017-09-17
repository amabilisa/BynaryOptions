#include "binaryoptionsmessagebox.h"

BinaryOptionsMessageBox::BinaryOptionsMessageBox(QWidget *parent):QMessageBox(parent)
{
    connect(&_animationReduceOpacityTimer, SIGNAL(timeout()), this, SLOT(onOpacityTemerShot()));
    _delayAnimationTimer.setSingleShot(true);
    connect(&_delayAnimationTimer, SIGNAL(timeout()), this, SLOT(onTimerDalayEnded()));
}

BinaryOptionsMessageBox::~BinaryOptionsMessageBox()
{

}

void BinaryOptionsMessageBox::showEvent(QShowEvent *event)
{
    setWindowOpacity(1.0);
    _delayAnimationTimer.start(3000);

    QMessageBox::showEvent(event);
}

void BinaryOptionsMessageBox::onTimerDalayEnded()
{
    _animationReduceOpacityTimer.start(80);
}

void BinaryOptionsMessageBox::onOpacityTemerShot()
{
    if (windowOpacity() > 0) {
        setWindowOpacity(windowOpacity() - 0.1);
    } else {
        _animationReduceOpacityTimer.stop();
        hide();
    }
}
