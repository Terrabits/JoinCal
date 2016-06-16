#ifndef JOINCALIBRATIONS_H
#define JOINCALIBRATIONS_H


// Project
#include "Corrections.h"
#include "JoinError.h"

// RsaToolbox
#include <Definitions.h>
#include <Vna.h>

// Qt
#include <QObject>
#include <QVector>


class JoinCalibrations : public QObject
{
    Q_OBJECT
public:
    JoinCalibrations(Corrections *c1, Corrections *c2, RsaToolbox::Vna *vna, QString saveAs, QObject *parent = 0);
    ~JoinCalibrations();

    bool isValid(JoinError &error);

signals:
    void starting();
    void progress(int percent);
    void finished();

public slots:
    void generate();

private:
    Corrections *_corr1, *_corr2;
    RsaToolbox::Vna *_vna;
    QString _filename;
    QVector<uint> ports();

    static void append(RsaToolbox::ComplexRowVector &vector, const RsaToolbox::ComplexRowVector &values);
};

#endif // JOINCALIBRATIONS_H
