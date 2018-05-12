#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "objectdetect.h"
#include "informationwindow.h"
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

private:
    Ui::MainWindow *mainWindow;
    InformationWindow *informationWindow;
    ObjectDetect *objectDetector;
    QTimer *timer;
};

#endif // MAINWINDOW_H
