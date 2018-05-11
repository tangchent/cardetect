#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QtGui>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent, int argc, char **argv) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    objectDetector = new ObjectDetect();
    if (!objectDetector->readParameters(argc,argv)) {
        printf("some parameters read failed,now use default\n");
    }

    QDesktopWidget* desktop = QApplication::desktop();
    //attempt to center MainWindow widget
    this->move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
}

MainWindow::~MainWindow()
{
    delete objectDetector;
    delete ui;
}

void MainWindow::on_openClassfilerButton_clicked()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Select Cascade Classifier"));
    fileDialog->setDirectory(".");
    fileDialog->setNameFilter(tr("XML Files(*.xml)"));
    if(fileDialog->exec() == QDialog::Accepted) {
        QString path = fileDialog->selectedFiles()[0];
        if (objectDetector->setObjectCascadeName(path.toStdString().c_str())) {
            ui->cascadeClassfierLabel->setText(tr("Classfiler:\n") + path);
        } else {
            ui->cascadeClassfierLabel->setText(tr("Cascade Classfiler not loaded"));
        }
    }
}

void MainWindow::on_openDirectoryButton_clicked()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Select Directory"));
    fileDialog->setDirectory(".");
    fileDialog->setFilter(QDir::Dirs);
    fileDialog->setFileMode(QFileDialog::Directory);
    if(fileDialog->exec() == QDialog::Accepted) {
        QString path = fileDialog->selectedFiles()[0];
        if (objectDetector->setFileDirectory(path.toStdString().c_str())) {
            ui->fileLable->setText(tr("Directory:\n") + path);
        } else {
            ui->cascadeClassfierLabel->setText(tr("File not loaded"));
        }
    }
}

void MainWindow::on_openVideoButton_clicked()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Select Video"));
    fileDialog->setDirectory(".");
    fileDialog->setNameFilter(tr("Video Files(*.mp4 *.avi *.rmvb)"));
    if(fileDialog->exec() == QDialog::Accepted) {
        QString path = fileDialog->selectedFiles()[0];
        if (objectDetector->setVideoname(path.toStdString().c_str())) {
            ui->fileLable->setText(tr("Video Name:\n") + path);
        } else {
            ui->cascadeClassfierLabel->setText(tr("File not loaded"));
        }
    }
}

void MainWindow::on_openCameraButton_clicked()
{

}
