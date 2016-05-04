#ifndef CALSOURCEWIDGET_H
#define CALSOURCEWIDGET_H


// Project
#include "CalibrationSource.h"

// Qt
#include <QWidget>

namespace Ui {
class CalSourceWidget;
}

class CalSourceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CalSourceWidget(QWidget *parent = 0);
    ~CalSourceWidget();

private slots:
    void showCalDialog();

private:
    Ui::CalSourceWidget *ui;

    CalibrationSource _source;
};

#endif // CALSOURCEWIDGET_H
