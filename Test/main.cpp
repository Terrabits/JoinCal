

// Project
#include "CalibrationTest.h"
#include "CorrectionsTest.h"

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
    testRunner.addTest(new CalibrationTest);
    testRunner.addTest(new CorrectionsTest);

    qDebug() << "Global result: " << (testRunner.runTests() ? "PASS" : "FAIL");
    return 0;
}
