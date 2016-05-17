#include "CalSortTest.h"


// Project
#include "Calibration.h"
#include "CalSort.h"

// RsaToolbox
using namespace RsaToolbox;


CalSortTest::CalSortTest(QObject *parent) :
    VnaTestClass(parent)
{
    _applicationName = "Corrections (class) Test";
    _version         = "0.0";

    _logDir.setPath(SOURCE_DIR);
    _logDir.mkpath("CalSortTest");
    _logDir.cd("CalSortTest");
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

    _logFilenames << "01 - Presorted Exclusive.txt"
                  << "02 - Exclusive.txt"
                  << "03 - Presorted Overlapping.txt"
                  << "04 - Overlapping.txt"
                  << "05 - Presorted Same Start.txt"
                  << "06 - Same start.txt"
                  << "07 - Presorted SameStop.txt"
                  << "08 - Same Stop.txt"
                  << "09 - Presorted Subset.txt"
                  << "10 - Subset.txt";

    _exclusive_a   = "exclusive_a.cal";
    _exclusive_b   = "exclusive_b.cal";
    _overlapping_a = "overlapping_a.cal";
    _overlapping_b = "overlapping_b.cal";
    _same_start_a  = "same_start_a.cal";
    _same_start_b  = "same_start_b.cal";
    _same_stop_a   = "same_stop_a.cal";
    _same_stop_b   = "same_stop_b.cal";
    _subset_a      = "subset_a.cal";
    _subset_b      = "subset_b.cal";
}

CalSortTest::~CalSortTest() {

}

void CalSortTest::initTestCase() {
    _initTestCase();
    _vna.reset(new Vna(_connectionType, _address));
    _vna->fileSystem().uploadFile(_calGroups.filePath( _exclusive_a  ), _exclusive_a,   VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->fileSystem().uploadFile(_calGroups.filePath( _exclusive_b  ), _exclusive_b,   VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->fileSystem().uploadFile(_calGroups.filePath( _overlapping_a), _overlapping_a, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->fileSystem().uploadFile(_calGroups.filePath( _overlapping_b), _overlapping_b, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->fileSystem().uploadFile(_calGroups.filePath( _same_start_a ), _same_start_a,  VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->fileSystem().uploadFile(_calGroups.filePath( _same_start_b ), _same_start_b,  VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->fileSystem().uploadFile(_calGroups.filePath( _same_stop_a  ), _same_stop_a,   VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->fileSystem().uploadFile(_calGroups.filePath( _same_stop_b  ), _same_stop_b,   VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->fileSystem().uploadFile(_calGroups.filePath( _subset_a     ), _subset_a,      VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->fileSystem().uploadFile(_calGroups.filePath( _subset_b     ), _subset_b,      VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna.reset();
}
void CalSortTest::cleanupTestCase() {
    qDebug() << "cleanupTestCase";
    _vna.reset(new Vna(_connectionType, _address));
    _vna->fileSystem().changeDirectory(VnaFileSystem::CAL_GROUP_DIRECTORY);
    _vna->fileSystem().deleteFile( _exclusive_a   );
    _vna->fileSystem().deleteFile( _exclusive_b   );
    _vna->fileSystem().deleteFile( _overlapping_a );
    _vna->fileSystem().deleteFile( _overlapping_b );
    _vna->fileSystem().deleteFile( _same_start_a  );
    _vna->fileSystem().deleteFile( _same_start_b  );
    _vna->fileSystem().deleteFile( _same_stop_a   );
    _vna->fileSystem().deleteFile( _same_stop_b   );
    _vna->fileSystem().deleteFile( _subset_a      );
    _vna->fileSystem().deleteFile( _subset_b      );
    _vna->fileSystem().changeDirectory(VnaFileSystem::DEFAULT_DIRECTORY);
    _vna.reset();
    _cleanupTestCase();
}

void CalSortTest::presortedExclusive() {
    Calibration calA;
    calA.source().setCalGroup(_exclusive_a);
    Calibration calB;
    calB.source().setCalGroup(_exclusive_b);
    CalSort(calA, calB, _vna.data());
    QVERIFY(calA.source().isCalGroup());
    QCOMPARE(calA.source().calGroup(), QString("exclusive_a"));
    QVERIFY(calB.source().isCalGroup());
    QCOMPARE(calB.source().calGroup(), QString("exclusive_b"));
}
void CalSortTest::exclusive() {
    Calibration calA;
    calA.source().setCalGroup(_exclusive_a);
    Calibration calB;
    calB.source().setCalGroup(_exclusive_b);
    CalSort(calB, calA, _vna.data());
    QVERIFY(calA.source().isCalGroup());
    QCOMPARE(calA.source().calGroup(), QString("exclusive_b"));
    QVERIFY(calB.source().isCalGroup());
    QCOMPARE(calB.source().calGroup(), QString("exclusive_a"));
}
void CalSortTest::presortedOverlapping() {
    Calibration calA;
    calA.source().setCalGroup(_overlapping_a);
    Calibration calB;
    calB.source().setCalGroup(_overlapping_b);
    CalSort(calA, calB, _vna.data());
    QVERIFY(calA.source().isCalGroup());
    QCOMPARE(calA.source().calGroup(), QString("overlapping_a"));
    QVERIFY(calB.source().isCalGroup());
    QCOMPARE(calB.source().calGroup(), QString("overlapping_b"));
}
void CalSortTest::overlapping() {
    Calibration calA;
    calA.source().setCalGroup(_overlapping_a);
    Calibration calB;
    calB.source().setCalGroup(_overlapping_b);
    CalSort(calB, calA, _vna.data());
    QVERIFY(calA.source().isCalGroup());
    QCOMPARE(calA.source().calGroup(), QString("overlapping_b"));
    QVERIFY(calB.source().isCalGroup());
    QCOMPARE(calB.source().calGroup(), QString("overlapping_a"));
}
void CalSortTest::presortedSameStart() {
    Calibration calA;
    calA.source().setCalGroup(_same_start_a);
    Calibration calB;
    calB.source().setCalGroup(_same_start_b);
    CalSort(calA, calB, _vna.data());
    QVERIFY(calA.source().isCalGroup());
    QCOMPARE(calA.source().calGroup(), QString("same_start_a"));
    QVERIFY(calB.source().isCalGroup());
    QCOMPARE(calB.source().calGroup(), QString("same_start_b"));
}
void CalSortTest::sameStart() {
    Calibration calA;
    calA.source().setCalGroup(_same_start_a);
    Calibration calB;
    calB.source().setCalGroup(_same_start_b);
    CalSort(calB, calA, _vna.data());
    QVERIFY(calA.source().isCalGroup());
    QCOMPARE(calA.source().calGroup(), QString("same_start_b"));
    QVERIFY(calB.source().isCalGroup());
    QCOMPARE(calB.source().calGroup(), QString("same_start_a"));
}
void CalSortTest::presortedSameStop() {
    Calibration calA;
    calA.source().setCalGroup(_same_stop_a);
    Calibration calB;
    calB.source().setCalGroup(_same_stop_b);
    CalSort(calA, calB, _vna.data());
    QVERIFY(calA.source().isCalGroup());
    QCOMPARE(calA.source().calGroup(), QString("same_stop_a"));
    QVERIFY(calB.source().isCalGroup());
    QCOMPARE(calB.source().calGroup(), QString("same_stop_b"));
}
void CalSortTest::sameStop() {
    Calibration calA;
    calA.source().setCalGroup(_same_stop_a);
    Calibration calB;
    calB.source().setCalGroup(_same_stop_b);
    CalSort(calB, calA, _vna.data());
    QVERIFY(calA.source().isCalGroup());
    QCOMPARE(calA.source().calGroup(), QString("same_stop_b"));
    QVERIFY(calB.source().isCalGroup());
    QCOMPARE(calB.source().calGroup(), QString("same_stop_a"));
}
void CalSortTest::presortedSubset() {
    Calibration calA;
    calA.source().setCalGroup(_subset_a);
    Calibration calB;
    calB.source().setCalGroup(_subset_b);
    CalSort(calA, calB, _vna.data());
    QVERIFY(calA.source().isCalGroup());
    QCOMPARE(calA.source().calGroup(), QString("subset_a"));
    QVERIFY(calB.source().isCalGroup());
    QCOMPARE(calB.source().calGroup(), QString("subset_b"));
}
void CalSortTest::subset() {
    Calibration calA;
    calA.source().setCalGroup(_subset_a);
    Calibration calB;
    calB.source().setCalGroup(_subset_b);
    CalSort(calB, calA, _vna.data());
    QVERIFY(calA.source().isCalGroup());
    QCOMPARE(calA.source().calGroup(), QString("subset_b"));
    QVERIFY(calB.source().isCalGroup());
    QCOMPARE(calB.source().calGroup(), QString("subset_a"));
}

