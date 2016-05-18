#include "Corrections.h"


// RsaToolbox
#include <General.h>
#include <VnaChannel.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>

Corrections::Corrections(CalibrationSource source, Vna *vna) :
    _isManaged(false),
    _channel(0),
    _isEmpty(false),
    _vna(vna)
{
    if (source.isChannel()) {
        _channel = source.channel();
    }
    else if (source.isCalGroup()) {
        _isManaged = true;
        _channel = _vna->createChannel();
        const QString calGroup = source.calGroup();
        _vna->channel(_channel).setCalGroup(calGroup);
    }

    if (_vna->channel(_channel).corrections().frequencies_Hz().isEmpty()) {
        _isEmpty = true;
        _startIndex = -1;
        _stopIndex  = -1;
    }
    else {
        _startIndex = 0;
        _stopIndex  = frequencies_Hz().size() - 1;
    }
}

Corrections::Corrections(Calibration calibration, Vna *vna) :
    _isManaged(false),
    _channel(0),
    _isEmpty(false),
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
    if (_startIndex == -1) {
        _isEmpty = true;
    }
    else {
        _stopIndex  = findStop(frequencies_Hz, calibration.range().stop_Hz(), calibration.range().isStopInclusive());
        if (_stopIndex == -1)
            _isEmpty = true;
    }
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

QString Corrections::displayText() const {
    QRowVector freq = frequencies_Hz();

    QString text = "%1 - %2, Ports %3";
    text = text.arg(formatValue(freq.first(), 3, Units::Hertz));
    text = text.arg(formatValue(freq.last(),  3, Units::Hertz));
    text = text.arg(portString(ports()));
    return text;
}

bool Corrections::isSwitchMatrix() const {
    return _vna->channel(_channel).corrections().switchMatrices();
}
QVector<uint> Corrections::ports() const {
    return _vna->channel(_channel).corrections().ports();
}
QRowVector Corrections::frequencies_Hz() const {
    if (_isEmpty)
        return QRowVector();

    return subsection(_vna->channel(_channel).corrections().frequencies_Hz());
}
ComplexRowVector Corrections::directivity(uint outputPort, uint inputPort) const {
    if (_isEmpty)
        return ComplexRowVector();

    return subsection(_vna->channel(_channel).corrections().directivity(outputPort, inputPort));
}
ComplexRowVector Corrections::sourceMatch(uint outputPort, uint inputPort) const {
    if (_isEmpty)
        return ComplexRowVector();

    return subsection(_vna->channel(_channel).corrections().sourceMatch(outputPort, inputPort));
}
ComplexRowVector Corrections::reflectionTracking(uint outputPort, uint inputPort) const {
    if (_isEmpty)
        return ComplexRowVector();

    return subsection(_vna->channel(_channel).corrections().reflectionTracking(outputPort, inputPort));
}
ComplexRowVector Corrections::loadMatch(uint outputPort, uint inputPort) const {
    if (_isEmpty)
        return ComplexRowVector();

    return subsection(_vna->channel(_channel).corrections().loadMatch(outputPort, inputPort));
}
ComplexRowVector Corrections::transmissionTracking(uint outputPort, uint inputPort) const {
    if (_isEmpty)
        return ComplexRowVector();

    return subsection(_vna->channel(_channel).corrections().transmissionTracking(outputPort, inputPort));
}

uint Corrections::findStart(const QRowVector &frequencies_Hz, double start_Hz) {
    if (start_Hz > frequencies_Hz.last())
        return -1;

    const int size = frequencies_Hz.size();
    int i = 0;
    while (i+1 < size && frequencies_Hz[i] < start_Hz) {
        i++;
    }
    return i;
}
uint Corrections::findStop(const QRowVector &frequencies_Hz, double stop_Hz, bool isInclusive) {
    if (stop_Hz < frequencies_Hz.first())
        return -1;

    const int size = frequencies_Hz.size();
    int i = size-1;
    while (i-1 >= 0 && stop_Hz < frequencies_Hz[i]) {
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
