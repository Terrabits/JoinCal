#include "CalSort.h"


// Project
#include "Calibration.h"
#include "Corrections.h"

// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QDebug>


CalSort::CalSort(CalibrationSource &cal1, CalibrationSource &cal2, RsaToolbox::Vna *vna)
{
    Calibration _cal1;
    _cal1.source() = cal1;
    const QRowVector freq1 = Corrections(_cal1, vna).frequencies_Hz();
    Calibration _cal2;
    _cal2.source() = cal2;
    const QRowVector freq2 = Corrections(_cal2, vna).frequencies_Hz();
    if (freq1.first() > freq2.first()) {
        swap(cal1, cal2);
    }
    else if (freq1.first() == freq2.first() && freq2.last() < freq1.last()) {
        swap(cal1, cal2);
    }
}

CalSort::~CalSort()
{

}

void CalSort::swap(CalibrationSource &cal1, CalibrationSource &cal2) {
    CalibrationSource temp;
    temp = cal1;
    cal1 = cal2;
    cal2 = temp;
}
