#ifndef JOINCALIBRATIONS_H
#define JOINCALIBRATIONS_H


// Project
#include "Corrections.h"

// RsaToolbox
#include <Definitions.h>
#include <Vna.h>

// Qt
#include <QVector>


class JoinCalibrations
{
public:
    JoinCalibrations(const QVector<Corrections*> &corrections, const QVector<uint> &ports, RsaToolbox::Vna *vna, QString saveAs);
    ~JoinCalibrations();

private:
    static void append(RsaToolbox::ComplexRowVector &vector, const RsaToolbox::ComplexRowVector &values);

};

#endif // JOINCALIBRATIONS_H
