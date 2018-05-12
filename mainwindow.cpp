#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string.h>
#include <QtCore>
#include <QtGui>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>

inline void myIntToArray(int number,char * array)
{
    int num;
    int j = 0;
    int i = 1;
    num = number;
    while (num > 10) {
        i *= 10;
        num /= 10;
    }
    while (number > 10) {
        *(array + j++) = number / i + '0';
        number %= i;
        i /= 10;
    }
    *(array + j++) = number + '0';
    *(array + j)   = '\0';
}

MainWindow::MainWindow(QWidget *parent, int argc, char **argv) :
    QWidget(parent),
    mainWindow(new Ui::MainWindow),
    informationWindow(NULL)
{
    mainWindow->setupUi(this);

    objectDetector = new ObjectDetect();
    if (!objectDetector->readParameters(argc,argv)) {
        printf("some parameters read failed,now use default\n");
    }

    QString path = QString(objectDetector->getCascadeName());
    mainWindow->cascadeClassfierLabel->setText(tr("Classfiler:\n") + path);

    if (objectDetector->isFromCamera()) {
        char number[4];
        myIntToArray(objectDetector->getCameraDevice(),number);
        QString idx = QString(number);
        mainWindow->fileLable->setText(tr("Camera Device : ") + idx);
        mainWindow->openCameraButton->setText(tr("Change"));
        mainWindow->openVideoButton->setEnabled(false);
        mainWindow->openDirectoryButton->setEnabled(false);
    } else if (objectDetector->isFromFile()) {
        QString path = QString(objectDetector->getFileDirectory());
        mainWindow->fileLable->setText(tr("Directory:\n") + path);
        mainWindow->openDirectoryButton->setText(tr("Change"));
        mainWindow->openCameraButton->setEnabled(false);
        mainWindow->openVideoButton->setEnabled(false);
    } else if (objectDetector->isFromVideo()) {
        QString path = QString(objectDetector->getVideoname());
        mainWindow->fileLable->setText(tr("Video Name:\n") + path);
        mainWindow->openCameraButton->setEnabled(false);
        mainWindow->openDirectoryButton->setEnabled(false);
    }

    QDesktopWidget* desktop = QApplication::desktop();
    //attempt to center MainWindow widget
    this->move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(on_timeout()));
}

void MainWindow::closeEvent(QCloseEvent *)
{
    if (informationWindow != NULL) {
        informationWindow->close();
    }
    printf("\nclose\n");
}

//solt of time out
void MainWindow::changeEvent(QEvent *)
{

}

MainWindow::~MainWindow()
{
    delete objectDetector;
    delete mainWindow;
    if (informationWindow != NULL) delete informationWindow;
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
            mainWindow->cascadeClassfierLabel->setText(tr("Classfiler:\n") + path);
        } else {
            mainWindow->cascadeClassfierLabel->setText(tr("Cascade Classfiler not loaded"));
        }
    }
}

void MainWindow::on_openDirectoryButton_clicked()
{
    if (mainWindow->openDirectoryButton->text() == (QString)"Change") {
        objectDetector->setFromFile(false);
        mainWindow->openDirectoryButton->setText(tr("OpenDirectory"));
        mainWindow->openCameraButton->setEnabled(true);
        mainWindow->openVideoButton->setEnabled(true);
        mainWindow->fileLable->setText(tr("File not loaded"));
    } else {
        QFileDialog *fileDialog = new QFileDialog(this);
        fileDialog->setWindowTitle(tr("Select Directory"));
        fileDialog->setDirectory(".");
        fileDialog->setFilter(QDir::Dirs);
        fileDialog->setFileMode(QFileDialog::Directory);
        if(fileDialog->exec() == QDialog::Accepted) {
            QString path = fileDialog->selectedFiles()[0];
            if (objectDetector->setFileDirectory(path.toStdString().c_str())) {
                objectDetector->setFromFile(true);
                mainWindow->fileLable->setText(tr("Directory:\n") + path);
                mainWindow->openDirectoryButton->setText(tr("Change"));
                mainWindow->openCameraButton->setEnabled(false);
                mainWindow->openVideoButton->setEnabled(false);
            } else {
                mainWindow->fileLable->setText(tr("File not loaded"));
            }
        }
    }
}

void MainWindow::on_openVideoButton_clicked()
{
    if (mainWindow->openVideoButton->text() == (QString)"Change") {
        objectDetector->setFromVideo(false);
        mainWindow->openVideoButton->setText(tr("OpenVideo"));
        mainWindow->openCameraButton->setEnabled(true);
        mainWindow->openDirectoryButton->setEnabled(true);
        mainWindow->fileLable->setText(tr("File not loaded"));
    } else {
        QFileDialog *fileDialog = new QFileDialog(this);
        fileDialog->setWindowTitle(tr("Select Video"));
        fileDialog->setDirectory(".");
        fileDialog->setNameFilter(tr("Video Files(*.mp4 *.avi *.rmvb)"));
        if(fileDialog->exec() == QDialog::Accepted) {
            QString path = fileDialog->selectedFiles()[0];
            if (objectDetector->setVideoname(path.toStdString().c_str())) {
                objectDetector->setFromVideo(true);
                mainWindow->fileLable->setText(tr("Video Name:\n") + path);
                mainWindow->openVideoButton->setText(tr("Change"));
                mainWindow->openCameraButton->setEnabled(false);
                mainWindow->openDirectoryButton->setEnabled(false);
            } else {
                mainWindow->fileLable->setText(tr("File not loaded"));
            }
        }
    }
}

void MainWindow::on_openCameraButton_clicked()
{
    if (mainWindow->openCameraButton->text() == (QString)"Change") {
        objectDetector->setFromCamera(false);
        mainWindow->openCameraButton->setText(tr("OpenCamera"));
        mainWindow->openVideoButton->setEnabled(true);
        mainWindow->openDirectoryButton->setEnabled(true);
        mainWindow->fileLable->setText(tr("File not loaded"));
    } else {
        bool result;
        int index = QInputDialog::getInt(this,tr("Input Index"),tr("Please input index"),0,0,99,1,&result);
        if (result) {
            char number[4];
            myIntToArray(index,number);
            QString idx = QString(number);
            objectDetector->setCameraDevice(index);
            objectDetector->setFromCamera(true);
            mainWindow->fileLable->setText(tr("Camera Device : ") + idx);
            mainWindow->openCameraButton->setText(tr("Change"));
            mainWindow->openVideoButton->setEnabled(false);
            mainWindow->openDirectoryButton->setEnabled(false);
        } else {
            mainWindow->fileLable->setText(tr("File not loaded"));
        }
    }
}

void MainWindow::on_detectButton_clicked()
{
    objectDetector->init();
    if (informationWindow == NULL) {
        informationWindow = new InformationWindow();
        informationWindow->show();
    } else {
        delete informationWindow;
        informationWindow = new InformationWindow();
        informationWindow->show();
    }
    timer->start(1);
}

void MainWindow::on_timeout()
{
    cv::Mat frame;
    frame = objectDetector->loadFrame(1);
    // start time
    const clock_t begin_time = clock();
    objectDetector->detectObjects(frame);
    // end time
    float seconds = float(clock() - begin_time) / CLOCKS_PER_SEC;
    //colorful print
    fprintf(stdout," time cost: \033[0;32m%.3f\033[0m s %d\r", seconds,frame.channels());
    fflush(stdout);
    for (size_t i = 0; i < objectDetector->objects.size(); i++) {
        //draw rectangle
        rectangle(frame, objectDetector->objects[i], cv::Scalar(0, 0, 255));
    }
    if (frame.channels() == 3) {
        cv::cvtColor(frame,frame,cv::COLOR_BGR2RGB);
    }
    QImage qimage = QImage(static_cast<uchar *>(frame.data),frame.cols,frame.rows,QImage::Format_RGB888);
    mainWindow->imageLabel->setPixmap(QPixmap::fromImage(qimage));
}
