#include "mainwindow.h"
#include <QApplication>
#include <cstdlib>

int main(int argc, char *argv[])
{
    srand(unsigned (std::time(nullptr)));
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    w.HandleAITurn();

    return a.exec();
}
