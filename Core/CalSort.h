#ifndef CALSORT_H
#define CALSORT_H


// Project
#include "Calibration.h"

// RsaToolbox
#include <Vna.h>

// Qt
#include <QVector>


class CalSort
{
public:
    CalSort(Calibration &cal1, Calibration &cal2, RsaToolbox::Vna *vna);
    ~CalSort();

private:
    void swap(Calibration &cal1, Calibration &cal2);
};

#endif // CALSORT_H
