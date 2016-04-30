#include "FrequencyRange.h"


// std lib
#include <cfloat>


FrequencyRange::FrequencyRange()
{
    clear();
}

FrequencyRange::~FrequencyRange()
{

}

bool FrequencyRange::isStopInclusive() const {
    return _isStopInclusive;
}
void FrequencyRange::setStopInclusive(bool isStopInclusive) {
    _isStopInclusive = isStopInclusive;
}

double FrequencyRange::start_Hz() const {
    return _start_Hz;
}
void FrequencyRange::setStart(double frequency_Hz) {
    _start_Hz = frequency_Hz;
}

double FrequencyRange::stop_Hz() const {
    return _stop_Hz;
}
void FrequencyRange::setStop(double frequency_Hz) {
    _stop_Hz = frequency_Hz;
}

bool FrequencyRange::isEmpty() const {
    if (_start_Hz != 0)
        return false;
    if (_stop_Hz != DBL_MAX)
        return false;

    return true;
}
void FrequencyRange::clear() {
    _isStopInclusive = false;
    _start_Hz = 0;
    _stop_Hz  = DBL_MAX;
}
