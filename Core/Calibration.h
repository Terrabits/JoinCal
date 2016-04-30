#ifndef CALIBRATION_H
#define CALIBRATION_H


// Project
#include "CalibrationSource.h"
#include "FrequencyRange.h"


class Calibration
{
public:
    Calibration();
    ~Calibration();

    CalibrationSource& source();
    FrequencyRange&    range();

private:
    CalibrationSource _source;
    FrequencyRange    _range;
};


#endif // CALIBRATION_H
