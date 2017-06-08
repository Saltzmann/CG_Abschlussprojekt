#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QDir>

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setVersion(4, 4); //allgemein kompatible OpenGL Version
    format.setProfile(QSurfaceFormat::CoreProfile);
    //format.setProfile(QSurfaceFormat::StereoBuffers); //wenn das double buffering aktiviert wäre das gut - erst einkommentieren wenn Rest funktioniert
    //format.setProfile(QSurfaceFormat::CompatibilityProfile); // müssen wir testen
    //format.setOption(QSurfaceFormat::DeprecatedFunctions); // ob wir dies brauchen
    format.setOption(QSurfaceFormat::DebugContext);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //Setzt Working-Directory auf .exe Ordner
    //TODO richtige Aufrufstelle?
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    return a.exec();
}
