#include "CalibrationTest.h"


// Project
#include "Calibration.h"

// RsaToolbox
using namespace RsaToolbox;

#include <QTest>


CalibrationTest::CalibrationTest(QObject *parent) :
    VnaTestClass(parent)
{
    _applicationName = "Calibration (class) Test";
    _version         = "0.0";

    _logDir = QDir(SOURCE_DIR);
    _logDir.cd("CalibrationTest");
    if (isZvaFamily()) {
        _logDir.cd("Zva");
    }
    else if (isZnbFamily()) {
        _logDir.cd("Znb");
    }
    _calGroups = _logDir;
    _logDir.cd("Logs");
    _calGroups.cd("CalGroups");

    _logFilenames << "test1.txt";
}
CalibrationTest::~CalibrationTest()
{

}

void CalibrationTest::test1() {
    QVERIFY(true);
}
