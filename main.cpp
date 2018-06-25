#include "mainwindow.h"
#include <QApplication>

int main(int countArg, char **listArg)
{

    QApplication a(countArg, listArg);
    MainWindow w;



    w.show();

    return a.exec();
}
