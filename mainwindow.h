#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "objectdetect.h"
#include "informationwindow.h"
#include "imagelable.h"
#include <QWidget>
#include <QCloseEvent>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, int argc = 0, char **argv = NULL);
    ~MainWindow();
    bool detect(int step);
    bool detect(cv::Mat frame);

protected:
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *event);

private slots:
    void on_openClassfilerButton_clicked();

    void on_openDirectoryButton_clicked();

    void on_openVideoButton_clicked();

    void on_openCameraButton_clicked();

    void on_detectButton_clicked();

    void on_timeout();

    void on_restartButton_clicked();

    void on_processBar_sliderMoved(int position);

    void on_previousButton_clicked();

    void on_nextButton_clicked();

    void on_openPicture_clicked();

    void on_imageLable_mouseMove(QMouseEvent *);

    void on_imageLable_mouseRelease(QMouseEvent *);

    void on_imageLable_mouseClicked(QMouseEvent *);

    void on_imageLable_wheelRoll(QWheelEvent *event);

    void on_imageLable_doubleClicked(QMouseEvent *event);

private:
    Ui::MainWindow *mainWindow;
    InformationWindow *informationWindow;
    ObjectDetect *objectDetector;
    QTimer *timer;
    ImageLable *imageLabel;
    float scale;
    bool configChange;
};

#endif // MAINWINDOW_H
