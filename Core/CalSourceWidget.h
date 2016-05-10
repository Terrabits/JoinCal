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
    CalSourceWidget(RsaToolbox::Vna *vna, QWidget *parent = 0);
    ~CalSourceWidget();

    void setVna(RsaToolbox::Vna *vna);

    void setSource(const CalibrationSource &source);
    CalibrationSource source() const;

signals:
    void sourceChanged(const CalibrationSource &source);

private slots:
    void showCalDialog();

private:
    Ui::CalSourceWidget *ui;

    RsaToolbox::Vna *_vna;
    CalibrationSource _source;
    void updateText();
};

#endif // CALSOURCEWIDGET_H
