#include "Corrections.h"


// RsaToolbox
#include <VnaChannel.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>


Corrections::Corrections(Calibration calibration, Vna *vna) :
    _isManaged(false),
    _channel(0),
    _vna(vna)
{
    if (calibration.source().isChannel()) {
        _channel = calibration.source().channel();
    }
    else if (calibration.source().isCalGroup()) {
        _isManaged = true;
        _channel = _vna->createChannel();
        const QString calGroup = calibration.source().calGroup();
        _vna->channel(_channel).setCalGroup(calGroup);
    }

    const QRowVector frequencies_Hz = _vna->channel(_channel).corrections().frequencies_Hz();
    _startIndex = findStart(frequencies_Hz, calibration.range().start_Hz());
    _stopIndex  = findStop(frequencies_Hz, calibration.range().stop_Hz(), calibration.range().isStopInclusive());
}
Corrections::~Corrections()
{
    cleanup();
}

bool Corrections::isChannel() const {
    return _vna->isChannel(_channel);
}
bool Corrections::isCalibrated() const {
    return _vna->channel(_channel).isCalibrated();
}
bool Corrections::isReady() const {
    return isChannel() && isCalibrated();
}

bool Corrections::isSwitchMatrix() const {
    return _vna->channel(_channel).corrections().switchMatrices();
}
QVector<uint> Corrections::ports() const {
    return _vna->channel(_channel).corrections().ports();
}
QRowVector Corrections::frequencies_Hz() const {
    return subsection(_vna->channel(_channel).corrections().frequencies_Hz());
}
ComplexRowVector Corrections::directivity(uint outputPort, uint inputPort) const {
    return subsection(_vna->channel(_channel).corrections().directivity(outputPort, inputPort));
}
ComplexRowVector Corrections::sourceMatch(uint outputPort, uint inputPort) const {
    return subsection(_vna->channel(_channel).corrections().sourceMatch(outputPort, inputPort));
}
ComplexRowVector Corrections::reflectionTracking(uint outputPort, uint inputPort) const {
    return subsection(_vna->channel(_channel).corrections().reflectionTracking(outputPort, inputPort));
}
ComplexRowVector Corrections::loadMatch(uint outputPort, uint inputPort) const {
    return subsection(_vna->channel(_channel).corrections().loadMatch(outputPort, inputPort));
}
ComplexRowVector Corrections::transmissionTracking(uint outputPort, uint inputPort) const {
    return subsection(_vna->channel(_channel).corrections().transmissionTracking(outputPort, inputPort));
}

uint Corrections::findStart(const QRowVector &frequencies_Hz, double start_Hz) {
    const int size = frequencies_Hz.size();
    int i = 0;
    while (i+1 < size && frequencies_Hz[i+1] <= start_Hz) {
        i++;
    }

    return i;
}
uint Corrections::findStop(const QRowVector &frequencies_Hz, double stop_Hz, bool isInclusive) {
    const int size = frequencies_Hz.size();
    int i = size-1;
    while (i-1 >= 0 && stop_Hz <= frequencies_Hz[i-1]) {
        i--;
    }
    if (!isInclusive && i > 0 && frequencies_Hz[i] == stop_Hz) {
        i--;
    }
    return i;
}

QRowVector Corrections::subsection(const QRowVector &frequencies_Hz) const {
    const uint length = _stopIndex - _startIndex + 1;
    return frequencies_Hz.mid(_startIndex, length);
}
ComplexRowVector Corrections::subsection(const ComplexRowVector &corrections) const {
    return ComplexRowVector(corrections.begin() + _startIndex, corrections.begin() + _stopIndex + 1);
}

void Corrections::cleanup() {
    if (_isManaged && _vna->isChannel(_channel)) {
        _vna->deleteChannel(_channel);
        _isManaged = false;
        _channel = 0;
    }
}
