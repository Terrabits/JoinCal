#ifndef CORRECTIONS_H
#define CORRECTIONS_H


// Project
#include "Calibration.h"
#include "CalibrationSource.h"

// RsaToolbox
#include <Vna.h>
#include <VnaCorrections.h>

// Qt
#include <Qt>
#include <QString>


class Corrections
{
public:
    Corrections(CalibrationSource source, RsaToolbox::Vna *vna);
    Corrections(Calibration calibration, RsaToolbox::Vna *vna);
    ~Corrections();

    enum /*class*/ State {
        NoLimits,
        LimitsNotApplied,
        LimitsApplied,
        Empty
    };

    CalibrationSource source() const;

    bool isChannel() const;
    bool isCalibrated() const;
    bool isReady() const;

    QString displayText();

    bool isSwitchMatrix() const;

    bool isEmpty();
    uint points();
    QVector<uint> ports() const;
    // Connector connector(uint port) const;
    // QVector<uint> connectors() const;

    double startFrequency_Hz();
    double stopFrequency_Hz();
    RsaToolbox::QRowVector frequencies_Hz();
    RsaToolbox::ComplexRowVector directivity(uint outputPort, uint inputPort);
    RsaToolbox::ComplexRowVector sourceMatch(uint outputPort, uint inputPort);
    RsaToolbox::ComplexRowVector reflectionTracking(uint outputPort, uint inputPort);
    RsaToolbox::ComplexRowVector loadMatch(uint outputPort, uint inputPort); // Only when out != in
    RsaToolbox::ComplexRowVector transmissionTracking(uint outputPort, uint inputPort); // Only when out != in

private:
    CalibrationSource _source;
    bool _isManaged;
    uint _channel;
    mutable RsaToolbox::Vna *_vna;
    void initialize();

    // Chop off unwanted parts
    State _state;
    FrequencyRange _range;
    void applyLimits();

    bool _isEmpty;
    int _startIndex;
    int _stopIndex;
    static uint findStart(const RsaToolbox::QRowVector& frequencies_Hz, double start_Hz);
    static uint findStop(const RsaToolbox::QRowVector& frequencies_Hz, double stop_Hz, bool isInclusive);
    RsaToolbox::QRowVector subsection(const RsaToolbox::QRowVector &frequencies_Hz) const;
    RsaToolbox::ComplexRowVector subsection(const RsaToolbox::ComplexRowVector &corrections) const;

    void cleanup();
};


#endif // CORRECTIONS_H
