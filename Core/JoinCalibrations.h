#ifndef JOINCALIBRATIONS_H
#define JOINCALIBRATIONS_H


// Project
#include "Calibration.h"
#include "Corrections.h"
#include "JoinError.h"

// RsaToolbox
#include <Definitions.h>
#include <Vna.h>

// Qt
#include <QObject>
#include <QScopedPointer>
#include <QVector>


class JoinCalibrations : public QObject
{
    Q_OBJECT
public:
    JoinCalibrations(Calibration cal1, Calibration cal2, RsaToolbox::Vna *vna, QString saveAs, bool load, QObject *parent = 0);
    ~JoinCalibrations();

    bool isValid(JoinError &error);

signals:
    void starting();
    void progress(int percent);
    void finished();

public slots:
    void generate();

private:
    Calibration _cal1;
    Calibration _cal2;
    RsaToolbox::Vna *_vna;
    QString _filename;
    bool _load;

    QScopedPointer<Corrections> _corr1;
    QScopedPointer<Corrections> _corr2;
    void getCorrections();
    void clearCorrections();

    QVector<uint> ports();

    void setSegmentedSweep(uint channel);
    static void append(RsaToolbox::ComplexRowVector &vector, const RsaToolbox::ComplexRowVector &values);

    void loadResults();
};

#endif // JOINCALIBRATIONS_H
