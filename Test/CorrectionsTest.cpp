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
#include <QScopedPointer>
#include <QTest>


CorrectionsTest::CorrectionsTest(QObject *parent) :
    VnaTestClass(parent)
{

}
CorrectionsTest::CorrectionsTest(ConnectionType type, const QString &address, QObject *parent) :
    VnaTestClass(parent)
{
    setConnectionType(type);
    setAddress(address);
}

CorrectionsTest::~CorrectionsTest()
{

}

void CorrectionsTest::initTestCase() {
    _applicationName = "Corrections (class) Test";
    _version         = "0.0";

    _logDir.setPath(SOURCE_DIR);
    _logDir.mkpath("CorrectionsTest");
    _logDir.cd("CorrectionsTest");
    if (isZvaFamily()) {
        _logDir.mkpath("Zva");
        _logDir.cd("Zva");
    }
    else if (isZnbFamily()) {
        _logDir.mkpath("Znb");
        _logDir.cd("Znb");
    }
    _calGroups = _logDir;
    _logDir.mkpath("Logs");
    _logDir.cd("Logs");
    _calGroups.mkpath("CalGroups");
    _calGroups.cd("CalGroups");

    _logFilenames << "1 - From Channel.txt"
                  << "2 - From Cal Group.txt"
                  << "3 - Full Corrections.txt"
                  << "4 - Subection.txt"
                  << "5 - Stop Excluded.txt"
                  << "6 - Stop Included.txt"
                  << "7 - Start Too High.txt"
                  << "8 - Stop Too Low.txt"
                  << "9 - Display Text.txt";

    _initTestCase();
    _vna.reset(new Vna(_connectionType, _address));
    QString calGroup = "ports 1-2 1-8 ghz 100 mhz steps.cal";
    _vna->fileSystem().uploadFile(_calGroups.filePath(calGroup), calGroup, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->fileSystem().uploadFile(_calGroups.filePath("linear.cal"), "linear.cal", VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->fileSystem().uploadFile(_calGroups.filePath("log.cal"), "log.cal", VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->fileSystem().uploadFile(_calGroups.filePath("segmented.cal"), "segmented.cal", VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna.reset();
}
void CorrectionsTest::cleanupTestCase() {
    _vna.reset(new Vna(_connectionType, _address));
    _vna->fileSystem().deleteFile("ports 1-2 1-8 ghz 100 mhz steps.cal");
    _vna->fileSystem().deleteFile("linear.cal");
    _vna->fileSystem().deleteFile("log.cal");
    _vna->fileSystem().deleteFile("segmented.cal");
    _vna.reset();
    _cleanupTestCase();
}

void CorrectionsTest::fromChannel() {
    QString calGroup = "ports 1-2 1-8 ghz 100 mhz steps.cal";
    _vna->createChannel(2);
    _vna->channel(2).setCalGroup(calGroup);
    QVERIFY(!_vna->channel(2).calGroup().isEmpty());
    Calibration calibration;
    calibration.source().setChannel(2);

    QVector<uint> ports;
    ports << 1 << 2;

    const int    points   = 50;
    const double start_Hz = 2.0E9;
    calibration.range().setStart(start_Hz);
    const double stop_Hz  = 7.0E9;
    calibration.range().setStop(stop_Hz);
    calibration.range().setStopInclusive(false);
    const double step_Hz = 100.0E6;

    QScopedPointer<Corrections> corrections(new Corrections(calibration, _vna.data()));
    QCOMPARE(_vna->channels().size(), 2);
    QVERIFY(_vna->channel(2).isCalibrated());
    QVERIFY(_vna->channel(2).isCalGroup());

    QVERIFY(!corrections->isSwitchMatrix());
    QCOMPARE(corrections->ports(),                  ports);
    QCOMPARE(corrections->frequencies_Hz().size(),  points);
    QCOMPARE(corrections->frequencies_Hz().first(), start_Hz);
    QCOMPARE(corrections->frequencies_Hz().last(),  stop_Hz - step_Hz);
    QCOMPARE(corrections->directivity(1,1).size(),  uint(points));

    corrections.reset();
    QCOMPARE(_vna->channels().size(), 2);
}
void CorrectionsTest::fromCalGroup() {
    QString calGroup = "ports 1-2 1-8 ghz 100 mhz steps.cal";
    Calibration calibration;
    calibration.source().setCalGroup(calGroup);


    QVector<uint> ports;
    ports << 1 << 2;

    const int    points   = 50;
    const double start_Hz = 2.0E9;
    calibration.range().setStart(start_Hz);
    const double stop_Hz  = 7.0E9;
    calibration.range().setStop(stop_Hz);
    calibration.range().setStopInclusive(false);
    const double step_Hz = 100.0E6;

    QScopedPointer<Corrections> corrections(new Corrections(calibration, _vna.data()));
    QCOMPARE(_vna->channels().size(), 2);
    QVERIFY(_vna->channel(2).isCalibrated());
    QVERIFY(_vna->channel(2).isCalGroup());

    QVERIFY(!corrections->isSwitchMatrix());
    QCOMPARE(corrections->ports(),                  ports);
    QCOMPARE(corrections->frequencies_Hz().size(),  points);
    QCOMPARE(corrections->frequencies_Hz().first(), start_Hz);
    QCOMPARE(corrections->frequencies_Hz().last(),  stop_Hz - step_Hz);
    QCOMPARE(corrections->directivity(1,1).size(),  uint(points));

    corrections.reset();
    QCOMPARE(_vna->channels().size(), 1);
}
void CorrectionsTest::fullCorrections() {
    QString calGroup = "ports 1-2 1-8 ghz 100 mhz steps.cal";
    _vna->channel().setCalGroup(calGroup);
    QVERIFY(!_vna->channel().calGroup().isEmpty());

    Calibration calibration;
    calibration.source().setChannel(1);

    QVector<uint> ports;
    ports << 1 << 2;

    const int    points   = 71;
    const double start_Hz = 0;
    calibration.range().setStart(start_Hz);
    const double stop_Hz  = DBL_MAX;
    calibration.range().setStop(stop_Hz);
    Corrections corrections(calibration, _vna.data());
    QVERIFY(!corrections.isSwitchMatrix());
    QCOMPARE(corrections.ports(),                  ports);
    QCOMPARE(corrections.frequencies_Hz().size(),  points);
    QCOMPARE(corrections.frequencies_Hz().first(), 1.0E9);
    QCOMPARE(corrections.frequencies_Hz().last(),  8.0E9);
    QCOMPARE(corrections.directivity(1,1).size(),  uint(points));
}
void CorrectionsTest::subsection() {
    QString calGroup = "ports 1-2 1-8 ghz 100 mhz steps.cal";
    _vna->channel().setCalGroup(calGroup);
    QVERIFY(!_vna->channel().calGroup().isEmpty());

    Calibration calibration;
    calibration.source().setChannel(1);


    QVector<uint> ports;
    ports << 1 << 2;

    const double step_Hz = 100.0E6;
    const double start_Hz = 2.0E9 - (1.0/2.0)*step_Hz;
    calibration.range().setStart(start_Hz);
    const double stop_Hz  = 7.0E9 + (1.0/2.0)*step_Hz;
    calibration.range().setStop(stop_Hz);
    const int    points   = 51;

    Corrections corrections(calibration, _vna.data());
    QVERIFY(!corrections.isSwitchMatrix());
    QCOMPARE(corrections.ports(),                  ports);
    QCOMPARE(corrections.frequencies_Hz().size(),  points);
    QCOMPARE(corrections.frequencies_Hz().first(), 2.0E9);
    QCOMPARE(corrections.frequencies_Hz().last(),  7.0E9);
    QCOMPARE(corrections.directivity(1,1).size(),  uint(points));
}
void CorrectionsTest::stopExcluded() {
    QString calGroup = "ports 1-2 1-8 ghz 100 mhz steps.cal";
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
void CorrectionsTest::stopIncluded() {
    QString calGroup = "ports 1-2 1-8 ghz 100 mhz steps.cal";
    _vna->channel().setCalGroup(calGroup);
    QVERIFY(!_vna->channel().calGroup().isEmpty());

    Calibration calibration;
    calibration.source().setChannel(1);


    QVector<uint> ports;
    ports << 1 << 2;

    const int    points   = 51;
    const double start_Hz = 2.0E9;
    calibration.range().setStart(start_Hz);
    const double stop_Hz  = 7.0E9;
    calibration.range().setStop(stop_Hz);
    calibration.range().setStopInclusive(true);
//    const double step_Hz = 100.0E6;

    Corrections corrections(calibration, _vna.data());
    QVERIFY(!corrections.isSwitchMatrix());
    QCOMPARE(corrections.ports(),                  ports);
    QCOMPARE(corrections.frequencies_Hz().size(),  points);
    QCOMPARE(corrections.frequencies_Hz().first(), start_Hz);
    QCOMPARE(corrections.frequencies_Hz().last(),  stop_Hz);
    QCOMPARE(corrections.directivity(1,1).size(),  uint(points));
}
void CorrectionsTest::startTooHigh() {
    QString calGroup = "ports 1-2 1-8 ghz 100 mhz steps.cal";
    _vna->channel().setCalGroup(calGroup);
    QVERIFY(!_vna->channel().calGroup().isEmpty());

    Calibration calibration;
    calibration.source().setChannel(1);

    QVector<uint> ports;
    ports << 1 << 2;

    const int    points   = 0;
    const double start_Hz = 9.0E9;
    calibration.range().setStart(start_Hz);
    const double stop_Hz  = DBL_MAX;
    calibration.range().setStop(stop_Hz);
    Corrections corrections(calibration, _vna.data());
    QVERIFY(!corrections.isSwitchMatrix());
    QCOMPARE(corrections.ports(),                  ports);
    QCOMPARE(corrections.frequencies_Hz().size(),  points);
    QCOMPARE(corrections.directivity(1,1).size(),  uint(points));
}
void CorrectionsTest::stopTooLow() {
    QString calGroup = "ports 1-2 1-8 ghz 100 mhz steps.cal";
    _vna->channel().setCalGroup(calGroup);
    QVERIFY(!_vna->channel().calGroup().isEmpty());

    Calibration calibration;
    calibration.source().setChannel(1);

    QVector<uint> ports;
    ports << 1 << 2;

    const int    points   = 0;
    const double start_Hz = 1;
    calibration.range().setStart(start_Hz);
    const double stop_Hz  = 2;
    calibration.range().setStop(stop_Hz);
    Corrections corrections(calibration, _vna.data());
    QVERIFY(!corrections.isSwitchMatrix());
    QCOMPARE(corrections.ports(),                  ports);
    QCOMPARE(corrections.frequencies_Hz().size(),  points);
    QCOMPARE(corrections.directivity(1,1).size(),  uint(points));
}
void CorrectionsTest::displayText() {
    Calibration calibration;
    calibration.source().setCalGroup("linear.cal");

    Corrections corrections(calibration, _vna.data());
    QCOMPARE(corrections.displayText(), QString("1.234 GHz - 5.678 GHz, Ports 1-3"));
}
