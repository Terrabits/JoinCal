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
    static void append(RsaToolbox::ComplexRowVector &vector, const RsaToolbox::ComplexRowVector &values);

};

#endif // JOINCALIBRATIONS_H
