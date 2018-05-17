#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <time.h>
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
    informationWindow(NULL),
    scale(1.0f),
    configChange(false)
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
        mainWindow->openPicture->setEnabled(false);
        mainWindow->openDirectoryButton->setEnabled(false);
    } else if (objectDetector->isFromFile()) {
        QString path = QString(objectDetector->getFileDirectory());
        mainWindow->fileLable->setText(tr("Directory:\n") + path);
        mainWindow->openDirectoryButton->setText(tr("Change"));
        mainWindow->openCameraButton->setEnabled(false);
        mainWindow->openPicture->setEnabled(false);
        mainWindow->openVideoButton->setEnabled(false);
    } else if (objectDetector->isFromVideo()) {
        QString path = QString(objectDetector->getVideoname());
        mainWindow->fileLable->setText(tr("Video Name:\n") + path);
        mainWindow->openCameraButton->setEnabled(false);
        mainWindow->openPicture->setEnabled(false);
        mainWindow->openDirectoryButton->setEnabled(false);
    }

    QDesktopWidget* desktop = QApplication::desktop();
    imageLabel = new ImageLable(mainWindow->tabDetect);//move to tabwidget
    imageLabel->setGeometry(QRect(0, 0, 640, 600));
    imageLabel->setAutoFillBackground(true); // enable us can change the background
    imageLabel->setEnabled(true);
    connect(imageLabel, SIGNAL(MouseMoved(QMouseEvent *)), this, SLOT(on_imageLable_mouseMove(QMouseEvent *)));
    connect(imageLabel, SIGNAL(MousePressed(QMouseEvent *)), this, SLOT(on_imageLable_mouseClicked(QMouseEvent *)));
    connect(imageLabel, SIGNAL(MouseRelease(QMouseEvent *)), this, SLOT(on_imageLable_mouseRelease(QMouseEvent *)));
    connect(imageLabel, SIGNAL(WheelEvent(QWheelEvent *)), this, SLOT(on_imageLable_wheelRoll(QWheelEvent*)));
    connect(imageLabel, SIGNAL(MouseDoubliClick(QMouseEvent *)),this, SLOT(on_imageLable_doubleClicked(QMouseEvent *)));
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
    if (timer != NULL) {
        timer->stop();
        delete timer;
    }
    delete imageLabel;
    delete objectDetector;
    delete mainWindow;
    if (informationWindow != NULL) delete informationWindow;
}

bool MainWindow::detect(int step)
{
    cv::Mat frame;
    frame = objectDetector->loadFrame(step);
    if (frame.rows == 0 || frame.cols == 0) {
        return false;
    }
    // start time
    const clock_t begin_time = clock();
    objectDetector->detectObjects(frame);
    // end time
    float seconds = float(clock() - begin_time) / CLOCKS_PER_SEC;
    //colorful print
    fprintf(stdout," time cost: \033[0;32m%.3f\033[0m s\r", seconds);
    fflush(stdout);
    for (size_t i = 0; i < objectDetector->objects.size(); i++) {
        //draw rectangle
        rectangle(frame, objectDetector->objects[i], cv::Scalar(0, 0, 255));
    }
    if (frame.channels() == 3) {
        cv::cvtColor(frame,frame,cv::COLOR_BGR2RGB);
    }
    cv::Size size;
    size.width  = cvFloor((float)frame.cols * scale);
    size.height = cvFloor((float)frame.rows * scale);
//    cv::resize(frame,frame,size,0,0,CV_INTER_AREA);
    QImage qimage = QImage(static_cast<uchar *>(frame.data),frame.cols,frame.rows,QImage::Format_RGB888);
    QImage img = qimage.scaled(size.width,size.height,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    imageLabel->setPixmap(QPixmap::fromImage(img));
    return true;
}

bool MainWindow::detect(cv::Mat frame)
{
    if (frame.rows == 0 || frame.cols == 0) {
        return false;
    }
    const clock_t begin_time = clock();
    objectDetector->detectObjects(frame);
    // end time
    float seconds = float(clock() - begin_time) / CLOCKS_PER_SEC;
    //colorful print
    fprintf(stdout," time cost: \033[0;32m%.3f\033[0m s\r", seconds);
    fflush(stdout);
    for (size_t i = 0; i < objectDetector->objects.size(); i++) {
        //draw rectangle
        rectangle(frame, objectDetector->objects[i], cv::Scalar(0, 0, 255));
    }
    if (frame.channels() == 3) {
        cv::cvtColor(frame,frame,cv::COLOR_BGR2RGB);
    }
    QImage qimage = QImage(static_cast<uchar *>(frame.data),frame.cols,frame.rows,QImage::Format_RGB888);
    imageLabel->setPixmap(QPixmap::fromImage(qimage));
    return true;
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
    configChange = true;
}

void MainWindow::on_openDirectoryButton_clicked()
{
    if (mainWindow->openDirectoryButton->text() == (QString)"Change") {
        objectDetector->setFromFile(false);
        mainWindow->openDirectoryButton->setText(tr("OpenDirectory"));
        mainWindow->openCameraButton->setEnabled(true);
        mainWindow->openVideoButton->setEnabled(true);
        mainWindow->openPicture->setEnabled(true);
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
                mainWindow->openPicture->setEnabled(false);
                configChange = true;
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
        mainWindow->openPicture->setEnabled(true);
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
                mainWindow->openPicture->setEnabled(false);
                configChange = true;
            } else {
                mainWindow->fileLable->setText(tr("Video not loaded"));
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
        mainWindow->openPicture->setEnabled(true);
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
            mainWindow->openPicture->setEnabled(false);
            configChange = true;
        } else {
            mainWindow->fileLable->setText(tr("File not loaded"));
        }
    }
}

void MainWindow::on_detectButton_clicked()
{
    if (mainWindow->detectButton->text() == (QString)"Detect") {
        if (configChange) {
            if (objectDetector->isFromCamera() || objectDetector->isFromVideo()) {
                objectDetector->closeCapture();
            } else if (objectDetector->isFromFile()) {
                objectDetector->freeFilenames();
            }
            printf("config change init\n");
            objectDetector->init();
            configChange = false;
        }
        if (!objectDetector->isInitialized()) {
            printf("init\n");
            if (!objectDetector->init()) {
                QMessageBox message(QMessageBox::Warning,"Init","Initialize Failed",QMessageBox::Yes,NULL);
                message.exec();
                return;
            }
            if (informationWindow == NULL) {
                informationWindow = new InformationWindow();
                informationWindow->show();
            } else {
                delete informationWindow;
                informationWindow = new InformationWindow();
                informationWindow->show();
            }
            timer->start(1);
            mainWindow->detectButton->setText((QString)"Stop");
            mainWindow->openClassfilerButton->setEnabled(false);
            mainWindow->openPicture->setEnabled(false);
            mainWindow->openCameraButton->setEnabled(false);
            mainWindow->openVideoButton->setEnabled(false);
            mainWindow->openDirectoryButton->setEnabled(false);
            if (objectDetector->isFromFile()) {
                mainWindow->processBar->setMaximum(objectDetector->getFileCount());
            } else if (objectDetector->isFromVideo()) {
                mainWindow->processBar->setMaximum(objectDetector->getVideoFrames());
            }
        } else {
            if (informationWindow != NULL) {
                informationWindow->show();
            } else {
                informationWindow = new InformationWindow();
                informationWindow->show();
            }
            timer->start(1);
            mainWindow->detectButton->setText((QString)"Stop");
            mainWindow->openClassfilerButton->setEnabled(false);
            mainWindow->openPicture->setEnabled(false);
            mainWindow->openCameraButton->setEnabled(false);
            mainWindow->openVideoButton->setEnabled(false);
            mainWindow->openDirectoryButton->setEnabled(false);
            if (objectDetector->isFromFile()) {
                mainWindow->processBar->setMaximum(objectDetector->getFileCount());
            } else if (objectDetector->isFromVideo()) {
                mainWindow->processBar->setMaximum(objectDetector->getVideoFrames());
            }
        }
    } else {
        mainWindow->detectButton->setText((QString)"Detect");
        timer->stop();
        mainWindow->openClassfilerButton->setEnabled(true);
        mainWindow->openPicture->setEnabled(true);
        if (objectDetector->isFromCamera()) {
            mainWindow->openCameraButton->setEnabled(true);
        } else if (objectDetector->isFromFile()) {
            mainWindow->openDirectoryButton->setEnabled(true);
        } else if (objectDetector->isFromVideo()) {
            mainWindow->openVideoButton->setEnabled(true);
        }
    }
}

void MainWindow::on_timeout()
{
    if (!detect(1)) {
        timer->stop();
        objectDetector->deinit();
        mainWindow->openClassfilerButton->setEnabled(true);
        mainWindow->openPicture->setEnabled(true);
        mainWindow->openCameraButton->setEnabled(true);
        mainWindow->openVideoButton->setEnabled(true);
        mainWindow->openDirectoryButton->setEnabled(true);
        QMessageBox message(QMessageBox::Critical,"Error","Error reading Image",QMessageBox::Yes,NULL);
        message.exec();
        mainWindow->detectButton->setText((QString)"Detect");
        mainWindow->openClassfilerButton->setEnabled(true);
        mainWindow->openPicture->setEnabled(true);
        if (objectDetector->isFromCamera()) {
            mainWindow->openCameraButton->setEnabled(true);
        } else if (objectDetector->isFromFile()) {
            mainWindow->openDirectoryButton->setEnabled(true);
        } else if (objectDetector->isFromVideo()) {
            mainWindow->openVideoButton->setEnabled(true);
        }
        if (informationWindow != NULL) {
            informationWindow->close();
            delete informationWindow;
            informationWindow = NULL;
        }
        return;
    }

    if (objectDetector->isFromFile()) {
        mainWindow->processBar->setSliderPosition(objectDetector->getCurrentIndex());
    } else if (objectDetector->isFromVideo()) {
        mainWindow->processBar->setSliderPosition(objectDetector->getCurrentFrame());
    }
}

void MainWindow::on_restartButton_clicked()
{
    if (mainWindow->detectButton->text() == (QString)"Stop") {
        timer->stop();
    }
    QMessageBox message(QMessageBox::Question,"Restart","Really to Restart?",QMessageBox::No|QMessageBox::Yes,NULL);
    if (message.exec()==QMessageBox::Yes) {
        if (objectDetector->isInitialized()) {
            if (objectDetector->isFromCamera() || objectDetector->isFromVideo()) {
                objectDetector->closeCapture();
            } else if (objectDetector->isFromFile()) {
                objectDetector->freeFilenames();
            }
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
            mainWindow->detectButton->setText((QString)"Stop");
            mainWindow->openClassfilerButton->setEnabled(false);
            mainWindow->openPicture->setEnabled(false);
            mainWindow->openCameraButton->setEnabled(false);
            mainWindow->openVideoButton->setEnabled(false);
            mainWindow->openDirectoryButton->setEnabled(false);
        } else {
            if (!objectDetector->init()) {
                QMessageBox message(QMessageBox::Warning,"Init","Initialize Failed",QMessageBox::Yes,NULL);
                message.exec();
                return;
            }
            if (informationWindow == NULL) {
                informationWindow = new InformationWindow();
                informationWindow->show();
            } else {
                delete informationWindow;
                informationWindow = new InformationWindow();
                informationWindow->show();
            }
            timer->start(1);
            mainWindow->detectButton->setText((QString)"Stop");
            mainWindow->openClassfilerButton->setEnabled(false);
            mainWindow->openPicture->setEnabled(false);
            mainWindow->openCameraButton->setEnabled(false);
            mainWindow->openVideoButton->setEnabled(false);
            mainWindow->openDirectoryButton->setEnabled(false);
            if (objectDetector->isFromFile()) {
                mainWindow->processBar->setMaximum(objectDetector->getFileCount());
            } else if (objectDetector->isFromVideo()) {
                mainWindow->processBar->setMaximum(objectDetector->getVideoFrames());
            }
        }
    } else {
        if (mainWindow->detectButton->text() == (QString)"Stop") {
            timer->start(1);
        }
    }
}

void MainWindow::on_processBar_sliderMoved(int position)
{
    int index = 0;
    int step;
    if (objectDetector->isInitialized()) {
        if (objectDetector->isFromVideo()) {
            index = objectDetector->getCurrentFrame();
        } else if (objectDetector->isFromFile()) {
            index = objectDetector->getCurrentIndex();
        }
        if (objectDetector->isFromVideo() || objectDetector->isFromFile()) {
            step = position - index;
            detect(step);
        }
    }
}

void MainWindow::on_previousButton_clicked()
{
    if (objectDetector->isInitialized()) {
        if (objectDetector->isFromVideo() || objectDetector->isFromFile()) {
            if (!detect(-1)) {
                timer->stop();
                objectDetector->deinit();
                mainWindow->openClassfilerButton->setEnabled(true);
                mainWindow->openPicture->setEnabled(true);
                mainWindow->openCameraButton->setEnabled(true);
                mainWindow->openVideoButton->setEnabled(true);
                mainWindow->openDirectoryButton->setEnabled(true);
                QMessageBox message(QMessageBox::Critical,"Error","Error reading Image",QMessageBox::Yes,NULL);
                message.exec();
                return;
            }
        }
    }
}

void MainWindow::on_nextButton_clicked()
{
    if (objectDetector->isInitialized()) {
        if (objectDetector->isFromVideo() || objectDetector->isFromFile()) {
            if (!detect(1)) {
                timer->stop();
                objectDetector->deinit();
                mainWindow->openClassfilerButton->setEnabled(true);
                mainWindow->openPicture->setEnabled(true);
                mainWindow->openCameraButton->setEnabled(true);
                mainWindow->openVideoButton->setEnabled(true);
                mainWindow->openDirectoryButton->setEnabled(true);
                QMessageBox message(QMessageBox::Critical,"Error","Error reading Image",QMessageBox::Yes,NULL);
                message.exec();
                return;
            }
        }
    }
}

void MainWindow::on_openPicture_clicked()
{
    if (mainWindow->openPicture->text() == "DetectPicture") {
        QFileDialog *fileDialog = new QFileDialog(this);
        fileDialog->setWindowTitle(tr("Select Picture"));
        fileDialog->setDirectory(".");
        fileDialog->setNameFilter(tr("Image Files(*.jpg *.jpeg *.bmp *.png)"));
        if(fileDialog->exec() == QDialog::Accepted) {
            QString path = fileDialog->selectedFiles()[0];
            objectDetector->deinit();
            objectDetector->setFilename(path.toStdString());
            objectDetector->setFromCamera(false);
            objectDetector->setFromVideo(false);
            objectDetector->setFromFile(true);
            objectDetector->init();
            mainWindow->openClassfilerButton->setEnabled(false);
            mainWindow->openCameraButton->setEnabled(false);
            mainWindow->openVideoButton->setEnabled(false);
            mainWindow->openDirectoryButton->setEnabled(false);
            mainWindow->openPicture->setText("Change");
            mainWindow->fileLable->setText(tr(path.toStdString().c_str()));
            timer->start(1);
        }
    } else {
        timer->stop();
        objectDetector->deinit();
        mainWindow->openClassfilerButton->setEnabled(true);
        mainWindow->openCameraButton->setEnabled(true);
        mainWindow->openVideoButton->setEnabled(true);
        mainWindow->openDirectoryButton->setEnabled(true);
        mainWindow->openPicture->setText("DetectPicture");
        mainWindow->fileLable->setText(tr("File not loaded"));
    }
}

void MainWindow::on_imageLable_mouseMove(QMouseEvent *event)
{
    event->accept();
}

void MainWindow::on_imageLable_mouseRelease(QMouseEvent *event)
{
    event->accept();
}

void MainWindow::on_imageLable_mouseClicked(QMouseEvent *event)
{
    event->accept();
}

void MainWindow::on_imageLable_doubleClicked(QMouseEvent *event)
{
    event->accept();
}

void MainWindow::on_imageLable_wheelRoll(QWheelEvent *event)
{
    if (event->delta() > 0) {
        scale += 0.2;
    } else if (event->delta() < 0) {
        scale -= 0.2;
    }
    if (scale > 2.6) scale = 2.6;
    if (scale < 0.2) scale = 0.2;
    QString str = QString("Scale:\n%1").arg(scale);
    mainWindow->scaleLabe->setText(str);
    if (mainWindow->detectButton->text() == "Detect") {
        if (objectDetector->isInitialized()) {
            detect(0);
        }
    }
    event->accept();
}

