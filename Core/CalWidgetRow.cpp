#include "CalWidgetRow.h"


// Project
//

// RsaToolbox
#include <Vna.h>
using namespace RsaToolbox;

// std lib
#include <climits>


CalWidgetRow::CalWidgetRow(Vna *vna, QObject *parent) :
    QObject(parent),
    _vna(vna)
{
    sourceWidget = new CalSourceWidget(_vna);
    startEdit    = new FrequencyEdit;
    stopEdit     = new FrequencyEdit;
    summaryLabel = new QLabel;
}

CalWidgetRow::~CalWidgetRow()
{
//    if (sourceWidget)
//        sourceWidget->deleteLater();
//    if (startEdit)
//        startEdit->deleteLater();
//    if (stopEdit)
//        stopEdit->deleteLater();
//    if (summaryLabel)
//        summaryLabel->deleteLater();
}

void CalWidgetRow::applyCalibration(Calibration &cal) {
    sourceWidget->setSource(cal.source());

    Corrections corrections(cal, _vna);
    QRowVector freq_Hz = corrections.frequencies_Hz();
    startEdit->setFrequency(cal.range().start_Hz());
    startEdit->clearMinimum();
    startEdit->setMaximum(freq_Hz.last());

    stopEdit->setFrequency(cal.range().stop_Hz());
    stopEdit->setMinimum(freq_Hz.first());
    stopEdit->clearMaximum();

    summaryLabel->setText(summary(corrections));
}

QString CalWidgetRow::summary(Corrections &c) const {
    return QString();
}
