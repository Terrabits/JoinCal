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
    CalSort(QVector<Calibration> &calibrations, RsaToolbox::Vna *vna);
    ~CalSort();
};

#endif // CALSORT_H
