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

private:
    Ui::MainWindow *ui;
    ObjectDetect * objectDetect;
};

#endif // MAINWINDOW_H
