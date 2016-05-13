#ifndef CALSORTTEST_H
#define CALSORTTEST_H


// RsaToolbox
#include <VnaTestClass.h>

// Qt
#include <QDir>
#include <QObject>


class CalSortTest : public RsaToolbox::VnaTestClass
{
    Q_OBJECT
public:
    explicit CalSortTest(QObject *parent = 0);
    ~CalSortTest();


private slots:
    void presorted();
    void exclusive();
    void overlapping();
    void sameStart();
    void sameStop();
    void subset();

private:
    QDir _calGroups;
};

#endif // CALSORTTEST_H
