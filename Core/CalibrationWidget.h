#ifndef CALIBRATIONWIDGET_H
#define CALIBRATIONWIDGET_H


// Project
#include "Calibration.h"
#include "CalSourceWidget.h"
#include "CalWidgetRow.h"

// RsaToolbox
#include <FrequencyEdit.h>
#include <Vna.h>

// Qt
#include <QGridLayout>
#include <QList>


namespace Ui {
class CalibrationWidget;
}


class CalibrationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CalibrationWidget(RsaToolbox::Vna *vna, QWidget *parent = 0);
    ~CalibrationWidget();

private:
    Ui::CalibrationWidget *ui;

    RsaToolbox::Vna *_vna;

    QGridLayout *_layout;
//    QList<CalWidgetRow>   _rows;
    void addRow(Calibration cal = Calibration());
    void insertRow(int i, Calibration cal = Calibration());
    void removeRow(int i);
};

#endif // CALIBRATIONWIDGET_H
