#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    //UI SLOT / SIGNAL Connects
    //Connected "Anzeige FPS Counter"
    QObject::connect(ui->myglwid, SIGNAL(sendFPSValue(int)),
                     ui->lcdFPSCounter, SLOT(display(int)));
    //Connected "Mouse Captured?"
    QObject::connect(ui->myglwid, SIGNAL(mouseCaptured(bool)),
                     ui->cbxMouse, SLOT(setChecked(bool)));
    //Connected "value spinbox 1"
    QObject::connect(ui->dsbValue1, SIGNAL(valueChanged(double)),
                     ui->myglwid, SLOT(counter1Changed(double)));
    //Connected "value spinbox 2"
    QObject::connect(ui->dsbValue2, SIGNAL(valueChanged(double)),
                     ui->myglwid, SLOT(counter2Changed(double)));
}

MainWindow::~MainWindow() {
    delete ui;
}
