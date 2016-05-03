#ifndef JOINCALIBRATIONSTEST_H
#define JOINCALIBRATIONSTEST_H


// RsaToolbox
#include <VnaTestClass.h>

// Qt
#include <QDir>
#include <QObject>


class JoinCalibrationsTest : public RsaToolbox::VnaTestClass
{
    Q_OBJECT
public:
    JoinCalibrationsTest();
    ~JoinCalibrationsTest();

private:
    QDir _calGroups;

private slots:
    void test1();
};

#endif // JOINCALIBRATIONSTEST_H
