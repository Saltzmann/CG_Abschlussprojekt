#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*UI SLOT / SIGNAL Connects

    //Connected "Anzeige FPS Counter"
    QObject::connect(ui->myglwid, SIGNAL(sendFPSValue(int)),
                     ui->lcdFPSCounter, SLOT(display(int)));
    //*/
}

MainWindow::~MainWindow()
{
    delete ui;
}
