#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // set font
    QFont font;
    font.setPointSize(10);
    font.setFamily("Microsoft Yahei");
    font.setBold(false);
    a.setFont(font);

    return a.exec();
}
