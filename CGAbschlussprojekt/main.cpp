#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QDir>

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setVersion(4, 4); //allgemein für uns kompatible OpenGL Version
    //format.setProfile(QSurfaceFormat::CoreProfile); //geht nicht weil qt vbos etc nicht richtig initialisiert für 4.4
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    //format.setSwapBehavior(QSurfaceFormat::SingleBuffer);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    //format.setSwapBehavior(QSurfaceFormat::TripleBuffer);
    format.setSwapInterval(0); //"V-Sync" off
    //format.setSwapInterval(1); //"V-Sync" on
    format.setOption(QSurfaceFormat::DebugContext);
    format.setOption(QSurfaceFormat::ResetNotification); //Infos über GL Context
    QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
