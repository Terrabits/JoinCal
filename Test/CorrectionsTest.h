#ifndef CORRECTIONSTEST_H
#define CORRECTIONSTEST_H


// RsaToolbox
#include <VnaTestClass.h>

// Qt
#include <QDir>
#include <QObject>


class CorrectionsTest : public RsaToolbox::VnaTestClass
{
    Q_OBJECT
public:
    CorrectionsTest(QObject *parent = 0);
    ~CorrectionsTest();

private:
    QDir _calGroups;

private slots:
    virtual void initTestCase();
    virtual void cleanupTestCase();

    void fromChannel();
    void fromCalGroup();
    void fullCorrections();
    void subsection();
    void stopExcluded();
    void stopIncluded();
    void startTooHigh();
    void stopTooLow();
    void displayText();
};

#endif // CORRECTIONSTEST_H
