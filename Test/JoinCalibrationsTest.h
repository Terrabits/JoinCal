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
    explicit JoinCalibrationsTest(QObject *parent = 0);
    explicit JoinCalibrationsTest(RsaToolbox::ConnectionType type, const QString &address, QObject *parent = 0);
    ~JoinCalibrationsTest();

private:
    QDir _calGroups;

private slots:
    virtual void initTestCase();

    void test1();
};

#endif // JOINCALIBRATIONSTEST_H
