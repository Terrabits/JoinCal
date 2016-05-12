#include "CalSort.h"


// Project
#include "Corrections.h"


CalSort::CalSort(QVector<Calibration> &calibrations, RsaToolbox::Vna *vna)
{
    const int size = calibrations.size();
    for (int i = 0; i < size; i++) {
        for (int j = i+1; j < size; j++) {
            Corrections c1(calibrations[i], vna);
            Corrections c2(calibrations[j], vna);
            if (c1.frequencies_Hz().first() > c2.frequencies_Hz().first()) {
                Calibration temp = calibrations[i];
                calibrations[i]  = calibrations[j];
                calibrations[j]  = temp;
            }
            else if (c1.frequencies_Hz().first() == c2.frequencies_Hz().first()) {
                if (c2.frequencies_Hz().last() < c1.frequencies_Hz().last()) {
                    Calibration temp = calibrations[i];
                    calibrations[i]  = calibrations[j];
                    calibrations[j]  = temp;
                }
            }
        }
    }
}

CalSort::~CalSort()
{

}

