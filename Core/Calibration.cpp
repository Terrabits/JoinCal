#include "Calibration.h"

Calibration::Calibration()
{

}

Calibration::~Calibration()
{

}

CalibrationSource &Calibration::source() {
    return _source;
}
FrequencyRange &Calibration::range() {
    return _range;
}
