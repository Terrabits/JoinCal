#ifndef CALSORT_H
#define CALSORT_H


// Project
#include "CalibrationSource.h"

// RsaToolbox
#include <Vna.h>

// Qt
#include <QVector>


class CalSort
{
public:
    CalSort(CalibrationSource &cal1, CalibrationSource &cal2, RsaToolbox::Vna *vna);
    ~CalSort();

private:
    void swap(CalibrationSource &cal1, CalibrationSource &cal2);
};

#endif // CALSORT_H
