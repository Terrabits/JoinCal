#ifndef CALIBRATIONSOURCETEST_H
#define CALIBRATIONSOURCETEST_H


// RsaToolbox
#include <TestClass.h>

// Qt
#include <QObject>


class CalibrationSourceTest : public RsaToolbox::TestClass
{
    Q_OBJECT
public:
    explicit CalibrationSourceTest(QObject *parent = 0);
    ~CalibrationSourceTest();

private slots:
    void empty();
    void channel();
    void calGroup();
    void compareEqual();
    void compareNotEqual();
    void copy();
    void displayText();
};

#endif // CALIBRATIONSOURCETEST_H
