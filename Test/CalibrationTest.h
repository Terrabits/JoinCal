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
    explicit CalibrationTest(QObject *parent = 0);
    CalibrationTest(RsaToolbox::ConnectionType type, const QString &address, QObject *parent);
    ~CalibrationTest();

private:
    QDir _calGroups;

private slots:
    void test1();
};

#endif // CALIBRATIONTEST_H
