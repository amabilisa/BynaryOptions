#include <QComboBox>
#include <QApplication>
#include <QAbstractItemView>

void fixComboBoxDropDownListSizeAdjustment(QComboBox *cb)
{
    int scroll = cb->count() <= cb->maxVisibleItems() ? 0 :
                                                        QApplication::style()->pixelMetric(QStyle::PixelMetric::PM_ScrollBarExtent);

    int max = 0;

    int correct = 32;

    for (int i = 0; i < cb->count(); i++)
    {
        int width = cb->view()->fontMetrics().width(cb->itemText(i));
        if (max < width)max = width;
    }

    int minimumWidth = scroll + max + correct;

    if (minimumWidth < cb->maximumWidth()) {
        cb->view()->setMinimumWidth(minimumWidth);
        cb->setMinimumWidth(minimumWidth);
    } else {
        cb->view()->setMinimumWidth(cb->maximumWidth());
        cb->setMinimumWidth(cb->maximumWidth());
    }
}



QString getDollarsString(int value)
{
    return QString(QString::number(value / 100) + "$");
}
