

// Project
#include "CalibrationSourceTest.h"
#include "CalibrationTest.h"
#include "CalSortTest.h"
#include "CorrectionsTest.h"
#include "JoinCalibrationsTest.h"
#include "GetCalDialogTest.h"

// RsaToolbox
#include <Definitions.h>
#include <TestClass.h>
#include <TestRunner.h>
using namespace RsaToolbox;

// Qt
#include <QApplication>
#include <QTest>
#include <QDebug>


int main()
{
    RsaToolbox::ConnectionType connectionType = RsaToolbox::ConnectionType::VisaTcpSocketConnection;
    QString address = "127.0.0.1::5025";

    TestRunner testRunner;
    testRunner.addTest(new CalibrationSourceTest);
    testRunner.addTest(new CalibrationTest);
    testRunner.addTest(new CorrectionsTest(connectionType, address));
    testRunner.addTest(new JoinCalibrationsTest(connectionType, address));
    testRunner.addTest(new GetCalDialogTest(connectionType, address));
    testRunner.addTest(new CalSortTest(connectionType, address));

    qDebug() << "Global result: " << (testRunner.runTests() ? "PASS" : "FAIL");
    return 0;
}
