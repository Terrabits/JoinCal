#include "CalibrationWidgetTest.h"


// Project
#include "CalibrationWidget.h"

// RsaToolbox
#include "Test.h"
using namespace RsaToolbox;

// Qt
#include <QEventLoop>
#include <QTest>


CalibrationWidgetTest::CalibrationWidgetTest(QObject *parent) :
    VnaTestClass(parent)
{
    _applicationName = "Calibration Widget Test";
    _version         = "0.0";

    _logDir.setPath(SOURCE_DIR);
    _logDir.mkpath("CalibrationWidgetTest");
    _logDir.cd("CalibrationWidgetTest");
    if (isZvaFamily()) {
        _logDir.mkpath("Zva");
        _logDir.cd("Zva");
    }
    else if (isZnbFamily()) {
        _logDir.mkpath("Zva");
        _logDir.cd("Znb");
    }
    _calGroups = _logDir;
    _logDir.mkpath("Logs");
    _logDir.cd("Logs");
    _calGroups.mkpath("CalGroups");
    _calGroups.cd("CalGroups");

    _calGroup1Filename = "Cal Group 1.cal";
    _calGroup1 = _calGroup1Filename;
    _calGroup1.chop(4);

    _calGroup2Filename = "Cal Group 2.cal";
    _calGroup2 = _calGroup2Filename;
    _calGroup2.chop(4);

    _logFilenames << "1 - test.txt";
}

CalibrationWidgetTest::~CalibrationWidgetTest()
{

}

void CalibrationWidgetTest::clearCalGroups() {
    QStringList calGroups = _vna->calGroups();
    foreach (QString calGroup, calGroups) {
        _vna->deleteCalGroup(calGroup);
    }

    QVERIFY(_vna->calGroups().isEmpty());
}
void CalibrationWidgetTest::copyCalGroups() {
    _vna->fileSystem().uploadFile(_calGroups.filePath(_calGroup1Filename), _calGroup1Filename, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->fileSystem().uploadFile(_calGroups.filePath(_calGroup2Filename), _calGroup2Filename, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);

    QVERIFY(_vna->isCalGroup(_calGroup1));
    QVERIFY(_vna->isCalGroup(_calGroup2));
}

void CalibrationWidgetTest::makeTwoCalibratedChannels() {
    _vna->createChannel(1);
    QVERIFY(_vna->isChannel(1));
    _vna->createChannel(2);
    QVERIFY(_vna->isChannel(2));

    if (!_vna->calGroups().isEmpty()) {
        const QString calGroup = _vna->calGroups().first();
        _vna->channel(1).setCalGroup(calGroup);
        _vna->channel(1).dissolveCalGroupLink();
        _vna->channel(2).setCalGroup(calGroup);
        _vna->channel(2).dissolveCalGroupLink();
    }
    else {
        copyCalGroups();
        _vna->channel(1).setCalGroup(_calGroup1);
        _vna->channel(1).dissolveCalGroupLink();
        _vna->channel(2).setCalGroup(_calGroup1);
        _vna->channel(2).dissolveCalGroupLink();
        clearCalGroups();
    }

    QVERIFY(!_vna->isError());
    QVERIFY(_vna->channel(1).isCalibrated());
    QVERIFY(_vna->channel(2).isCalibrated());
}

void CalibrationWidgetTest::test() {
    clearCalGroups();
    copyCalGroups();
    makeTwoCalibratedChannels();

    QEventLoop loop;
    CalibrationWidget widget(_vna.data());
    widget.show();
    loop.exec();
}
