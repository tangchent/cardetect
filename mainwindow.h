#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "objectdetect.h"
#include <QWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, int argc = 0, char **argv = NULL);
    ~MainWindow();

private slots:
    void on_openClassfilerButton_clicked();

    void on_openDirectoryButton_clicked();

    void on_openVideoButton_clicked();

    void on_openCameraButton_clicked();

private:
    Ui::MainWindow *ui;
    ObjectDetect * objectDetector;
};

#endif // MAINWINDOW_H
