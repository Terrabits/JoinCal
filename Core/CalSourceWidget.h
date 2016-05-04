#ifndef CALSOURCEWIDGET_H
#define CALSOURCEWIDGET_H


// Project
#include "CalibrationSource.h"

// RsaToolbox
#include <Vna.h>

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

    void setVna(RsaToolbox::Vna *vna);

signals:
    void sourceChanged(const CalibrationSource &source);

private slots:
    void showCalDialog();

private:
    Ui::CalSourceWidget *ui;

    RsaToolbox::Vna *_vna;
    CalibrationSource _source;
};

#endif // CALSOURCEWIDGET_H
