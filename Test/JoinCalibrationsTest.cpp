#include "JoinCalibrationsTest.h"


// Project
#include "Calibration.h"
#include "Corrections.h"
#include "JoinCalibrations.h"

// RsaToolbox
#include <Test.h>
#include <VnaFileSystem.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>
#include <QTest>


JoinCalibrationsTest::JoinCalibrationsTest()
{
    _applicationName = "JoinCalibrations (class) Test";
    _version         = "0.0";

    _logDir.setPath(SOURCE_DIR);
    _logDir.cd("JoinCalibrationsTest");
    if (isZvaFamily()) {
        _logDir.cd("Zva");
    }
    else if (isZnbFamily()) {
        _logDir.cd("Znb");
    }
    _calGroups = _logDir;
    _logDir.cd("Logs");
    _calGroups.cd("CalGroups");

    _logFilenames << "1 - test1.txt";
}

JoinCalibrationsTest::~JoinCalibrationsTest()
{

}

void JoinCalibrationsTest::test1() {
    QString calGroup1 = "ports 1-2 1-4 ghz 100 mhz steps.cal";
    QString calGroup2 = "ports 1-2 4p1-8 ghz 100 mhz steps.cal";
    _vna->fileSystem().uploadFile(_calGroups.filePath(calGroup1), calGroup1, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->fileSystem().uploadFile(_calGroups.filePath(calGroup2), calGroup2, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);

    Calibration calibration1;
    calibration1.source().setCalGroup(calGroup1);
    calibration1.range().setStart(1.0E9);
    calibration1.range().setStop(4.0E9);
    calibration1.range().setStopInclusive(true);
    Corrections corrections1(calibration1, _vna.data());

    Calibration calibration2;
    calibration2.source().setCalGroup(calGroup2);
    calibration2.range().setStart(4.1E9);
    calibration2.range().setStop(8.0E9);
    calibration2.range().setStopInclusive(true);
    Corrections corrections2(calibration2, _vna.data());

    QVector<Corrections*> corrections;
    corrections << &corrections1
                << &corrections2;
    QVector<uint> ports;
    ports << 1 << 2;
    QString saveAs = "result.cal";

    JoinCalibrations(corrections, ports, _vna.data(), saveAs);
    QVERIFY(!_vna->isError());
    QVERIFY(_vna->isCalGroup("result"));
    QVERIFY(!_vna->channel(1).isCalibrated());

    _vna->channel().setCalGroup(saveAs);
    QVERIFY(!_vna->isError());
    QVERIFY(_vna->channel(1).isCalGroup());
    QCOMPARE(_vna->channel(1).calGroup(), QString("result"));
    QCOMPARE(_vna->channel(1).corrections().points(), uint(corrections1.frequencies_Hz().size() + corrections2.frequencies_Hz().size()));
}
