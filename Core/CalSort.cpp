#include "CalSort.h"


// Project
#include "Corrections.h"

// RsaToolbox
using namespace RsaToolbox;


CalSort::CalSort(Calibration &cal1, Calibration &cal2, RsaToolbox::Vna *vna)
{
    const QRowVector freq1 = Corrections(cal1, vna).frequencies_Hz();
    const QRowVector freq2 = Corrections(cal2, vna).frequencies_Hz();
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

void CalSort::swap(Calibration &cal1, Calibration &cal2) {
    Calibration temp;
    temp = cal1;
    cal1 = cal2;
    cal2 = temp;
}
