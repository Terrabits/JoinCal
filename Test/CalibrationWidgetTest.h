#ifndef CALIBRATIONWIDGETTEST_H
#define CALIBRATIONWIDGETTEST_H


// RsaToolbox
#include <VnaTestClass.h>

// Qt
#include <QObject>


class CalibrationWidgetTest : public RsaToolbox::VnaTestClass
{
    Q_OBJECT
public:
    explicit CalibrationWidgetTest(QObject *parent = 0);
    ~CalibrationWidgetTest();

private:
    QDir _calGroups;
    QString _calGroup1;
    QString _calGroup1Filename;
    QString _calGroup2;
    QString _calGroup2Filename;

    void clearCalGroups();
    void copyCalGroups();
    void clearChannels();
    void makeTwoCalibratedChannels();

private slots:
    void test();
};

#endif // CALIBRATIONWIDGETTEST_H
