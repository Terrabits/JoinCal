

// Project
#include "CalibrationSourceTest.h"
#include "CalibrationTest.h"
#include "CalibrationWidgetTest.h"
#include "CorrectionsTest.h"
#include "JoinCalibrationsTest.h"
#include "GetCalDialogTest.h"

// RsaToolbox
#include <TestClass.h>
#include <TestRunner.h>
using namespace RsaToolbox;

// Qt
#include <QApplication>
#include <QTest>
#include <QDebug>


int main()
{
    TestRunner testRunner;
//    testRunner.addTest(new CalibrationSourceTest);
//    testRunner.addTest(new CalibrationTest);
//    testRunner.addTest(new CorrectionsTest);
//    testRunner.addTest(new JoinCalibrationsTest);
//    testRunner.addTest(new GetCalDialogTest);
    testRunner.addTest(new CalibrationWidgetTest);

    qDebug() << "Global result: " << (testRunner.runTests() ? "PASS" : "FAIL");
    return 0;
}
