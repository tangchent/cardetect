#ifndef INFORMATIONWINDOW_H
#define INFORMATIONWINDOW_H

#include <QWidget>

namespace Ui {
class InformationWindow;
}

class InformationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit InformationWindow(QWidget *parent = 0);
    ~InformationWindow();

private:
    Ui::InformationWindow *ui;
};

#endif // INFORMATIONWINDOW_H
