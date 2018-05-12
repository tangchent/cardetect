#include "informationwindow.h"
#include "ui_informationwindow.h"

InformationWindow::InformationWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InformationWindow)
{
    ui->setupUi(this);
}

InformationWindow::~InformationWindow()
{
    delete ui;
}
