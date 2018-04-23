#include "mianwindow.h"
#include "ui_mianwindow.h"

mianwindow::mianwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mianwindow)
{
    ui->setupUi(this);
}

mianwindow::~mianwindow()
{
    delete ui;
}
