#ifndef CALIBRATIONTEST_H
#define CALIBRATIONTEST_H


// RsaToolbox
#include <VnaTestClass.h>

// Qt
#include <QDir>
#include <QObject>


class CalibrationTest : public RsaToolbox::VnaTestClass
{
    Q_OBJECT
public:
    CalibrationTest(QObject *parent = 0);
    ~CalibrationTest();

private:
    QDir _calGroups;

private slots:
    void test1();
};

#endif // CALIBRATIONTEST_H
