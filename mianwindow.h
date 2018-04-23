#ifndef MIANWINDOW_H
#define MIANWINDOW_H

#include <QWidget>

namespace Ui {
class mianwindow;
}

class mianwindow : public QWidget
{
    Q_OBJECT

public:
    explicit mianwindow(QWidget *parent = 0);
    ~mianwindow();

private:
    Ui::mianwindow *ui;
};

#endif // MIANWINDOW_H
