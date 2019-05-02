#include "mainwindow.h"
#include <QApplication>
#include <QtCore>
#include <QDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mainwindow;
    mainwindow.show();

    return a.exec();
}
