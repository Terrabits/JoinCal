#include "JoinCalibrations.h"


// Project
//

// RsaToolbox
#include <VnaChannel.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>
#include <QSharedPointer>


// Assumes:
// - sections are valid
// - sections are sorted
//   low to high frequency
JoinCalibrations::JoinCalibrations(Calibration cal1,
                                   Calibration cal2,
                                   RsaToolbox::Vna *vna,
                                   QString saveAs,
                                   bool load,
                                   QObject *parent)
    : QObject(parent),
      _cal1(cal1),
      _cal2(cal2),
      _vna(vna),
      _filename(saveAs),
      _load(load)
{
    // Hola. De nada.
}

JoinCalibrations::~JoinCalibrations()
{
    // Adios
}

bool JoinCalibrations::isValid(JoinError &error) {
    getCorrections();
    // 1st Corrections
    if (!_corr1->isReady()) {
        error.code  = JoinError::Code::Calibration1;
        error.index = 1;
        error.msg   = "*Can\'t load cal from %1";
        error.msg   = error.msg.arg(_corr1->source().displayText());
        clearCorrections();
        return false;
    }
    if (_corr1->isEmpty()) {
        error.code  = JoinError::Code::Calibration1;
        error.index = 1;
        error.msg   = "*No points from cal %1";
        error.msg   = error.msg.arg(_corr1->source().displayText());
        clearCorrections();
        return false;
    }
    // 2nd Corrections
    if (!_corr2->isReady()) {
        error.code  = JoinError::Code::Calibration2;
        error.index = 2;
        error.msg   = "*Can\'t load cal from %1";
        error.msg   = error.msg.arg(_corr2->source().displayText());
        clearCorrections();
        return false;
    }
    if (_corr2->isEmpty()) {
        error.code  = JoinError::Code::Calibration2;
        error.index = 2;
        error.msg   = "*No points from cal %1";
        error.msg   = error.msg.arg(_corr2->source().displayText());
        clearCorrections();
        return false;
    }
    // Frequency Overlap
    if (_corr1->stopFrequency_Hz() >= _corr2->startFrequency_Hz()) {
        error.code = JoinError::Code::FrequencyRange;
        error.msg = "*Frequency range error";
        clearCorrections();
        return false;
    }
    qDebug() << "corr1: " << _corr1->startFrequency_Hz() << _corr1->stopFrequency_Hz();
    qDebug() << "corr2: " << _corr2->startFrequency_Hz() << _corr2->stopFrequency_Hz();
    // Common ports
    if (ports().isEmpty()) {
        error.code = JoinError::Code::Ports;
        error.msg = "*No common ports";
        clearCorrections();
        return false;
    }

    clearCorrections();
    return true;
}
void JoinCalibrations::generate() {
    emit starting();
    _vna->isError();
    _vna->clearStatus();

    // _corr1, _corr2
    getCorrections();

    // New channel
    uint c = _vna->createChannel();
    VnaChannel channel = _vna->channel(c);
    if (channel.isCalGroup()) {
        channel.dissolveCalGroupLink();
    }

    // New sweep
    setSegmentedSweep(c);
    _vna->isError();
    _vna->clearStatus();

    // New (blank) correction set
    QVector<uint> _ports = ports();
    VnaCorrections newCorrections = channel.corrections();
    channel.calibrate().start("JoinCal", VnaCalibrate::CalType::Tosm, _ports);
    newCorrections.loadDefaultCorrections();
    _vna->isError();
    _vna->clearStatus();

    // Fill in concatenated corrections
    int _progress = 0;
    const int _totalSteps = _ports.size() * _ports.size();
    foreach (int p1, _ports) {
        foreach (int p2, _ports) {
            ComplexRowVector directivity;
            ComplexRowVector sourceMatch;
            ComplexRowVector reflectionTracking;
            ComplexRowVector loadMatch;
            ComplexRowVector transmissionTracking;

            // 1st Corrections
            directivity              = _corr1->directivity(p1, p2);
            sourceMatch              = _corr1->sourceMatch(p1, p2);
            reflectionTracking       = _corr1->reflectionTracking(p1, p2);
            if (p2 != p1) {
                loadMatch            = _corr1->loadMatch(p1, p2);
                transmissionTracking = _corr1->transmissionTracking(p1 ,p2);
            }

            // 2nd Corrections
            append(directivity,              _corr2->directivity(p1, p2));
            append(sourceMatch,              _corr2->sourceMatch(p1, p2));
            append(reflectionTracking,       _corr2->reflectionTracking(p1, p2));
            if (p2 != p1) {
                append(loadMatch,            _corr2->loadMatch(p1, p2));
                append(transmissionTracking, _corr2->transmissionTracking(p1 ,p2));
            }

            // Set
            newCorrections.setDirectivity(directivity, p1, p2);
            newCorrections.setSourceMatch(sourceMatch, p1, p2);
            newCorrections.setReflectionTracking(reflectionTracking, p1 ,p2);
            if (p2 != p1) {
                newCorrections.setLoadMatch(loadMatch, p1, p2);
                newCorrections.setTransmissionTracking(transmissionTracking, p1, p2);
            }
            _vna->isError();
            _vna->clearStatus();
            _progress++;
            emit progress(100.0*_progress/_totalSteps);
        }
    }

    // Save new cal
    channel.saveCalibration(_filename);
    channel.dissolveCalGroupLink();
    channel.setCalGroup(_filename);

    // cleanup
    clearCorrections();
    _vna->deleteChannel(c);
    _vna->isError();
    _vna->clearStatus();

    // Display result?
    if (_load) {
        loadResults();
    }

    _vna->isError();
    _vna->clearStatus();
    emit finished();
}

QVector<uint> JoinCalibrations::ports() {
    const QVector<uint> ports1 = _corr1->ports();
    const QVector<uint> ports2 = _corr2->ports();
    QVector<uint> result;
    foreach(uint p, ports1) {
        if (ports2.contains(p))
            result.append(p);
    }
    return result;
}

void JoinCalibrations::getCorrections() {
    _corr1.reset(new Corrections(_cal1, _vna));
    _corr2.reset(new Corrections(_cal2, _vna));
}
void JoinCalibrations::clearCorrections() {
    _corr1.reset();
    _corr2.reset();
}

void JoinCalibrations::setSegmentedSweep(uint channel) {
    qDebug() << "setSegmentedSweep in " << channel;
    // Create segmented sweep
    VnaChannel ch = _vna->channel(channel);
    VnaSegmentedSweep sweep = ch.segmentedSweep();
    if (_corr1->sweepType() == VnaChannel::Linear) {
        sweep.deleteSegments();
        sweep.addSegment(1);
        VnaSweepSegment s = sweep.segment(1);
        s.setStart(_corr1->startFrequency_Hz());
        s.setStop(_corr1->stopFrequency_Hz());
        s.setPoints(_corr1->points());
    }
    else {
        ch.setFrequencies(_corr1->frequencies_Hz());
    }
    if (_corr2->sweepType() == VnaChannel::Linear) {
        VnaSweepSegment s = sweep.segment(sweep.addSegment());
        s.setStart(_corr2->startFrequency_Hz());
        s.setStop(_corr2->stopFrequency_Hz());
        s.setPoints(_corr2->points());
    }
    else {
        QRowVector freq = _corr2->frequencies_Hz();
        for (int i = 0; i < freq.size(); i++) {
            VnaSweepSegment s = sweep.segment(sweep.addSegment());
            s.setSingleFrequency(freq[i]);
        }
    }
    ch.setSweepType(VnaChannel::SweepType::Segmented);
}

void JoinCalibrations::append(ComplexRowVector &vector, const ComplexRowVector &values) {
    vector.insert(vector.end(), values.begin(), values.end());
}

void JoinCalibrations::loadResults() {
    // setup channel, sweep
    qDebug() << "channels: " << _vna->channels();
    uint c = _vna->createChannel();
    qDebug() << "results channel: " << c;
    getCorrections();
    setSegmentedSweep(c);
    clearCorrections();

    // setup calibration
    VnaChannel channel = _vna->channel(c);
    channel.setCalGroup(_filename);

    // Show trace
    uint d = _vna->createDiagram();
    QString trc = _vna->createTrace(c);
    _vna->trace(trc).setDiagram(d);

    channel.continuousSweepOn();
    _vna->isError();
    _vna->clearStatus();
}
