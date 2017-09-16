#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QFile syleSheetFile(":/styles/style_dark.qss");

    if (syleSheetFile.open(QFile::ReadOnly)) {
        QString styleSheet = syleSheetFile.readAll();
        a.setStyleSheet(styleSheet);
        syleSheetFile.close();
    }

    return a.exec();
}
