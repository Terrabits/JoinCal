#ifndef CORRECTIONS_H
#define CORRECTIONS_H


// Project
#include "Calibration.h"

// RsaToolbox
#include <Vna.h>
#include <VnaCorrections.h>

// Qt
#include <Qt>
#include <QString>


class Corrections
{
public:
    Corrections(Calibration calibration, RsaToolbox::Vna *vna);
    ~Corrections();

    bool isChannel() const;
    bool isCalibrated() const;
    bool isReady() const;

    bool isSwitchMatrix() const;
    QVector<uint> ports() const;
    RsaToolbox::QRowVector frequencies_Hz() const;
    RsaToolbox::ComplexRowVector directivity(uint outputPort, uint inputPort) const;
    RsaToolbox::ComplexRowVector sourceMatch(uint outputPort, uint inputPort) const;
    RsaToolbox::ComplexRowVector reflectionTracking(uint outputPort, uint inputPort) const;
    RsaToolbox::ComplexRowVector loadMatch(uint outputPort, uint inputPort) const; // Only when out != in
    RsaToolbox::ComplexRowVector transmissionTracking(uint outputPort, uint inputPort) const; // Only when out != in

private:
    bool _isManaged;
    uint _channel;
    mutable RsaToolbox::Vna *_vna;

    // Chop off unwanted parts
    uint _startIndex;
    uint _stopIndex;
    static uint findStart(const RsaToolbox::QRowVector& frequencies_Hz, double start_Hz);
    static uint findStop(const RsaToolbox::QRowVector& frequencies_Hz, double stop_Hz, bool isInclusive);
    RsaToolbox::QRowVector subsection(const RsaToolbox::QRowVector &frequencies_Hz) const;
    RsaToolbox::ComplexRowVector subsection(const RsaToolbox::ComplexRowVector &corrections) const;

    void cleanup();
};


#endif // CORRECTIONS_H
