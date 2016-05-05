#ifndef CALWIDGETROW_H
#define CALWIDGETROW_H


// Project
#include "Calibration.h"
#include "Corrections.h"
#include "FrequencyRange.h"
#include "CalSourceWidget.h"

// RsaToolbox
#include <FrequencyEdit.h>

// Qt
#include <QLabel>
#include <QObject>


class CalWidgetRow : public QObject
{
    Q_OBJECT
public:
    explicit CalWidgetRow(RsaToolbox::Vna *vna, QObject *parent = 0);
    ~CalWidgetRow();

    CalSourceWidget *sourceWidget;
    RsaToolbox::FrequencyEdit *startEdit;
    RsaToolbox::FrequencyEdit *stopEdit;
    QLabel *summaryLabel;

    void applyCalibration(Calibration &cal);

private:
    mutable RsaToolbox::Vna *_vna;
    QString summary(Corrections &c) const;
};

#endif // CALWIDGETROW_H
