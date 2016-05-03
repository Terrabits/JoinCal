#include "JoinCalibrationsTest.h"


// Project
#include "Calibration.h"
#include "Corrections.h"
#include "JoinCalibrations.h"

// RsaToolbox
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
    QString calGroup1 = "Ports 1-2 1-4 GHz 100 MHz steps.cal";
    QString calGroup2 = "Ports 1-2 4p1-8 GHz 100 MHz steps.cal";
    _vna->fileSystem().uploadFile(_calGroups.filePath(calGroup1), calGroup1, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->fileSystem().uploadFile(_calGroups.filePath(calGroup2), calGroup2, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);

    Calibration calibration1;
    calibration1.source().setCalGroup(calGroup1);
    calibration1.range().setStart(1.0E9);
    calibration1.range().setStop(4.0E9);
    calibration1.range().setStopInclusive(true);

    Calibration calibration2;
    calibration2.source().setCalGroup(calGroup2);
    calibration2.range().setStart(4.1E9);
    calibration2.range().setStop(8.0E9);
    calibration2.range().setStopInclusive(true);

    QVector<Calibration> calibrations;
    calibrations << calibration1
                 << calibration2;
    QVector<uint> ports;
    ports << 1 << 2;
    QString saveAs = "Result.cal";

    QCOMPARE(_vna->channels().size(), 1);
    JoinCalibrations join(calibrations, ports, _vna.data(), saveAs);
    QCOMPARE(_vna->channels().size(), 1);

    QVERIFY(!_vna->isError());

    _vna->preset();
    _vna->pause();

    QVERIFY(_vna->isCalGroup("Result"));
    _vna->channel().setCalGroup(saveAs);
    QVERIFY(!_vna->isError());
    QVERIFY(_vna->channel().isCalGroup());
    QCOMPARE(_vna->channel().calGroup(), QString("Result"));
}
