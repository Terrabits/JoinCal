#include "CorrectionsTest.h"


// Project
#include "CalibrationSource.h"
#include "Calibration.h"
#include "Corrections.h"
#include "FrequencyRange.h"

// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QDebug>
#include <QTest>


CorrectionsTest::CorrectionsTest(QObject *parent) :
    VnaTestClass(parent)
{
    _applicationName = "Corrections (class) Test";
    _version         = "0.0";

    _logDir.setPath(SOURCE_DIR);
    _logDir.cd("CorrectionsTest");
    if (isZvaFamily()) {
        _logDir.cd("Zva");
    }
    else if (isZnbFamily()) {
        _logDir.cd("Znb");
    }
    _calGroups = _logDir;
    _logDir.cd("Logs");
    _calGroups.cd("CalGroups");

    _logFilenames << "1 - Full Corrections.txt"
                  << "2 - Subection.txt";
}

CorrectionsTest::~CorrectionsTest()
{

}

void CorrectionsTest::fullCorrections() {
    QString calGroup = "Ports 1-2, 1-8 GHz, 100 MHZ steps.cal";
    _vna->fileSystem().uploadFile(_calGroups.filePath(calGroup), calGroup, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->channel().setCalGroup(calGroup);
    QVERIFY(!_vna->channel().calGroup().isEmpty());

    Calibration calibration;
    calibration.source().setChannel(1);

    QVector<uint> ports;
    ports << 1 << 2;

    const int    points   = 71;
    const double start_Hz = 1.0E9;
    const double stop_Hz  = 8.0E9;
    Corrections corrections(calibration, _vna.data());
    QVERIFY(!corrections.isSwitchMatrix());
    QCOMPARE(corrections.ports(),                  ports);
    QCOMPARE(corrections.frequencies_Hz().size(),  points);
    QCOMPARE(corrections.frequencies_Hz().first(), start_Hz);
    QCOMPARE(corrections.frequencies_Hz().last(),  stop_Hz);
    QCOMPARE(corrections.directivity(1,1).size(),  uint(points));
}
void CorrectionsTest::subsection() {
    QString calGroup = "Ports 1-2, 1-8 GHz, 100 MHZ steps.cal";
    _vna->fileSystem().uploadFile(_calGroups.filePath(calGroup), calGroup, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->channel().setCalGroup(calGroup);
    QVERIFY(!_vna->channel().calGroup().isEmpty());

    Calibration calibration;
    calibration.source().setChannel(1);


    QVector<uint> ports;
    ports << 1 << 2;

    const int    points   = 50;
    const double start_Hz = 2.0E9;
    calibration.range().setStart(start_Hz);
    const double stop_Hz  = 7.0E9;
    calibration.range().setStop(stop_Hz);
    calibration.range().setStopInclusive(false);
    const double step_Hz = 100.0E6;

    Corrections corrections(calibration, _vna.data());
    QVERIFY(!corrections.isSwitchMatrix());
    QCOMPARE(corrections.ports(),                  ports);
    QCOMPARE(corrections.frequencies_Hz().size(),  points);
    QCOMPARE(corrections.frequencies_Hz().first(), start_Hz);
    QCOMPARE(corrections.frequencies_Hz().last(),  stop_Hz - step_Hz);
    QCOMPARE(corrections.directivity(1,1).size(),  uint(points));
}
