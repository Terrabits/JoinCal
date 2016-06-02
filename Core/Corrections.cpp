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
    _vna(vna),
    _state(State::NoLimits)
{
    initialize(source);
}

Corrections::Corrections(Calibration calibration, Vna *vna) :
    _isManaged(false),
    _channel(0),
    _vna(vna),
    _state(State::LimitsNotApplied)
{
    initialize(calibration);
    _range = calibration.range();
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

QString Corrections::displayText() {
    QString text = "%1 - %2, Ports %3";
    text = text.arg(formatValue(startFrequency_Hz(), 3, Units::Hertz));
    text = text.arg(formatValue(stopFrequency_Hz(),  3, Units::Hertz));
    text = text.arg(portString(ports()));
    return text;
}

bool Corrections::isSwitchMatrix() const {
    return _vna->channel(_channel).corrections().switchMatrices();
}
QVector<uint> Corrections::ports() const {
    return _vna->channel(_channel).corrections().ports();
}
uint Corrections::points() {
    if (_state == State::NoLimits)
        return _vna->channel(_channel).corrections().points();
    if (_state == State::LimitsNotApplied)
        applyLimits();
    if (_state == State::Empty)
        return 0;

    return frequencies_Hz().size();
}
double Corrections::startFrequency_Hz(){
    if (_state == State::NoLimits)
        return _vna->channel(_channel).corrections().startFrequency_Hz();
    if (_state == State::LimitsNotApplied)
        applyLimits();
    if (_state == State::Empty)
        return -1.0;

    return frequencies_Hz().first();
}
double Corrections::stopFrequency_Hz() {
    if (_state == State::NoLimits)
        return _vna->channel(_channel).corrections().stopFrequency_Hz();
    if (_state == State::LimitsNotApplied)
        applyLimits();
    if (_state == State::Empty)
        return -1.0;

    return frequencies_Hz().last();
}
QRowVector Corrections::frequencies_Hz() {
    if (_state == State::NoLimits)
        return _vna->channel(_channel).corrections().frequencies_Hz();
    if (_state == State::LimitsNotApplied)
        applyLimits();
    if (_state == State::Empty)
        return QRowVector();

    return subsection(_vna->channel(_channel).corrections().frequencies_Hz());
}
ComplexRowVector Corrections::directivity(uint outputPort, uint inputPort) {
    if (_state == State::NoLimits)
        return _vna->channel(_channel).corrections().directivity(outputPort, inputPort);
    if (_state == State::LimitsNotApplied)
        applyLimits();
    if (_state == State::Empty)
        return ComplexRowVector();

    return subsection(_vna->channel(_channel).corrections().directivity(outputPort, inputPort));
}
ComplexRowVector Corrections::sourceMatch(uint outputPort, uint inputPort) {
    if (_state == State::Empty)
        return ComplexRowVector();
    if (_state == State::LimitsNotApplied)
        applyLimits();
    if (_state == State::NoLimits)
        return _vna->channel(_channel).corrections().sourceMatch(outputPort, inputPort);

    return subsection(_vna->channel(_channel).corrections().sourceMatch(outputPort, inputPort));
}
ComplexRowVector Corrections::reflectionTracking(uint outputPort, uint inputPort) {
    if (_state == State::Empty)
        return ComplexRowVector();
    if (_state == State::LimitsNotApplied)
        applyLimits();
    if (_state == State::NoLimits)
        return _vna->channel(_channel).corrections().reflectionTracking(outputPort, inputPort);

    return subsection(_vna->channel(_channel).corrections().reflectionTracking(outputPort, inputPort));
}
ComplexRowVector Corrections::loadMatch(uint outputPort, uint inputPort) {
    if (_state == State::Empty)
        return ComplexRowVector();
    if (_state == State::LimitsNotApplied)
        applyLimits();
    if (_state == State::NoLimits)
        return _vna->channel(_channel).corrections().loadMatch(outputPort, inputPort);

    return subsection(_vna->channel(_channel).corrections().loadMatch(outputPort, inputPort));
}
ComplexRowVector Corrections::transmissionTracking(uint outputPort, uint inputPort) {
    if (_state == State::Empty)
        return ComplexRowVector();
    if (_state == State::LimitsNotApplied)
        applyLimits();
    if (_state == State::NoLimits)
        return _vna->channel(_channel).corrections().transmissionTracking(outputPort, inputPort);

    return subsection(_vna->channel(_channel).corrections().transmissionTracking(outputPort, inputPort));
}

void Corrections::initialize(CalibrationSource &source) {
    if (source.isChannel()) {
        _channel = source.channel();
    }
    else if (source.isCalGroup()) {
        _isManaged = true;
        _channel = _vna->createChannel();
        const QString calGroup = source.calGroup();
        _vna->channel(_channel).setCalGroup(calGroup);
    }
}
void Corrections::initialize(Calibration &calibration) {
    if (calibration.source().isChannel()) {
        _channel = calibration.source().channel();
    }
    else if (calibration.source().isCalGroup()) {
        _isManaged = true;
        _channel = _vna->createChannel();
        const QString calGroup = calibration.source().calGroup();
        _vna->channel(_channel).setCalGroup(calGroup);
    }
}

void Corrections::applyLimits() {
    const QRowVector frequencies_Hz = _vna->channel(_channel).corrections().frequencies_Hz();
    if (frequencies_Hz.isEmpty()) {
        _state = State::Empty;
        return;
    }
    _startIndex = findStart(frequencies_Hz, _range.start_Hz());
    if (_startIndex == -1) {
        _state = State::Empty;
        return;
    }
    else {
        _stopIndex  = findStop(frequencies_Hz, _range.stop_Hz(), _range.isStopInclusive());
        if (_stopIndex == -1) {
            _state = State::Empty;
            return;
        }
    }
    _state = State::LimitsApplied;
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
