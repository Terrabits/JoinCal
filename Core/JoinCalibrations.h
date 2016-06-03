#ifndef JOINCALIBRATIONS_H
#define JOINCALIBRATIONS_H


// Project
#include "Corrections.h"
#include "JoinError.h"

// RsaToolbox
#include <Definitions.h>
#include <Vna.h>

// Qt
#include <QVector>


class JoinCalibrations
{
public:
    JoinCalibrations(Corrections *c1, Corrections *c2, RsaToolbox::Vna *vna, QString saveAs);
    ~JoinCalibrations();

    bool isValid(JoinError &error);
    void generate();

private:
    Corrections *_corr1, *_corr2;
    RsaToolbox::Vna *_vna;
    QString _filename;
    QVector<uint> ports();

    static void append(RsaToolbox::ComplexRowVector &vector, const RsaToolbox::ComplexRowVector &values);
};

#endif // JOINCALIBRATIONS_H
