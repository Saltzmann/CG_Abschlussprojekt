#include "mainwindow.h"
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //Setzt Working-Directory auf .exe Ordner
    //TODO richtige Aufrufstelle?
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    return a.exec();
}
