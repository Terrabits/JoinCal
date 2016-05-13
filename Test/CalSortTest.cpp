#include "CalSortTest.h"

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

    _logFilenames << "1 - Presorted.txt"
                  << "2 - Exclusive.txt"
                  << "3 - Overlapping.txt"
                  << "4 - Same Start.txt"
                  << "5 - Same Stop.txt"
                  << "6 - Subset.txt";
}

CalSortTest::~CalSortTest()
{

}

void CalSortTest::presorted() {

}
void CalSortTest::exclusive() {

}
void CalSortTest::overlapping() {

}
void CalSortTest::sameStart() {

}
void CalSortTest::sameStop() {

}
void CalSortTest::subset() {

}

