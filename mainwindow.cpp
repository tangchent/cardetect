#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QtGui>
#include <QDesktopWidget>


MainWindow::MainWindow(QWidget *parent, int argc, char **argv) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    objectDetect = new ObjectDetect();
    if (!objectDetect->readParameters(argc,argv)) {
        printf("some parameters read failed,now use default\n");
    }
    if (!objectDetect->init()) {
        printf("some file can't open\n");
    }

    QDesktopWidget* desktop = QApplication::desktop();
    //attempt to center MainWindow widget
    this->move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
}

MainWindow::~MainWindow()
{
    delete objectDetect;
    delete ui;
}
