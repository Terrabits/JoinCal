#ifndef JOINCALIBRATIONS_H
#define JOINCALIBRATIONS_H


// Project
#include "Calibration.h"

// RsaToolbox
#include <Definitions.h>
#include <Vna.h>

// Qt
#include <QVector>


class JoinCalibrations
{
public:
    JoinCalibrations(const QVector<Calibration> &sections, const QVector<uint> &ports, RsaToolbox::Vna *vna, QString saveAs);
    ~JoinCalibrations();

private:
    static RsaToolbox::QRowVector subsection(const RsaToolbox::QRowVector &freq, double start_Hz, double stop_Hz);
    static RsaToolbox::ComplexRowVector subsection(const RsaToolbox::ComplexRowVector &corrections, int startIndex, int stopIndex);
};

#endif // JOINCALIBRATIONS_H
