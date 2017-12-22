#include <QApplication>
#include <QDesktopWidget>

#include <function.h>
#include <iostream>
#include <map>

#include "mainwindow.h"



int main(int argc, char *argv[])
{

    QApplication app(argc,argv);
    MainWindow w;

    w.show();






    return app.exec();

}
