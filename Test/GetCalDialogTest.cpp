#include "GetCalDialogTest.h"


// Project
#include "GetCalDialog.h"

// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QTest>


GetCalDialogTest::GetCalDialogTest(QObject *parent) :
    VnaTestClass(parent)
{
    _applicationName = "GetCalDialog Test";
    _version         = "0.0";

    _logDir.setPath(SOURCE_DIR);
    _logDir.mkpath("GetCalDialogTest");
    _logDir.cd("GetCalDialogTest");
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

    _logFilenames << "1 - Accept Cal Group.txt"
                  << "2 - Accept Channel.txt"
                  << "3 - Reject Dialog.txt"
                  << "4 - No Channels.txt"
                  << "5 - No Cal Groups.txt"
                  << "6 - No Calibrations At All.txt";
}

GetCalDialogTest::~GetCalDialogTest()
{

}

void GetCalDialogTest::clearCalGroups() {
    QStringList calGroups = _vna->calGroups();
    foreach (QString calGroup, calGroups) {
        _vna->deleteCalGroup(calGroup);
    }

    QVERIFY(_vna->calGroups().isEmpty());
}
void GetCalDialogTest::copyCalGroups() {
    _vna->fileSystem().uploadFile(_calGroups.filePath(_calGroup1Filename), _calGroup1Filename, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->fileSystem().uploadFile(_calGroups.filePath(_calGroup2Filename), _calGroup2Filename, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);

    QVERIFY(_vna->isCalGroup(_calGroup1));
    QVERIFY(_vna->isCalGroup(_calGroup2));
}

void GetCalDialogTest::makeTwoCalibratedChannels() {
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

void GetCalDialogTest::acceptCalGroup() {
    clearCalGroups();
    copyCalGroups();
    makeTwoCalibratedChannels();

    CalibrationSource expectedCal(_calGroup2);

    GetCalDialog dialog(_vna.data());
    dialog.setWindowTitle("Accept CalGrp2");
    dialog.exec();
    QVERIFY(!_vna->isError());
    QCOMPARE(QDialog::DialogCode(dialog.result()), QDialog::Accepted);
    QCOMPARE(dialog.selectedCal(), expectedCal);
}
void GetCalDialogTest::acceptChannel() {
    clearCalGroups();
    copyCalGroups();
    makeTwoCalibratedChannels();

    CalibrationSource expectedCal(uint(2));

    GetCalDialog dialog(_vna.data());
    dialog.setWindowTitle("Accept ch2");
    dialog.exec();
    QVERIFY(!_vna->isError());
    QCOMPARE(QDialog::DialogCode(dialog.result()), QDialog::Accepted);
    QCOMPARE(dialog.selectedCal(), expectedCal);
}
void GetCalDialogTest::rejectDialog() {
    clearCalGroups();
    copyCalGroups();
    makeTwoCalibratedChannels();

    GetCalDialog dialog(_vna.data());
    dialog.setWindowTitle("Reject");
    dialog.exec();
    QVERIFY(!_vna->isError());
    QCOMPARE(QDialog::DialogCode(dialog.result()), QDialog::Rejected);
    QVERIFY(dialog.selectedCal().isEmpty());
}
void GetCalDialogTest::noChannels() {
    clearCalGroups();
    copyCalGroups();

    GetCalDialog dialog(_vna.data());
    dialog.setWindowTitle("No Channels");
    dialog.exec();
}
void GetCalDialogTest::noCalGroups() {
    clearCalGroups();
    makeTwoCalibratedChannels();

    GetCalDialog dialog(_vna.data());
    dialog.setWindowTitle("No CalGrp");
    dialog.exec();
}
void GetCalDialogTest::noCalibrationsAtAll() {
    clearCalGroups();

    GetCalDialog dialog(_vna.data());
    dialog.setWindowTitle("No Cals");
    dialog.exec();
}

