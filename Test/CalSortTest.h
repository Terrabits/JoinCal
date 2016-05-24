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
    virtual void init();
    virtual void cleanup();

    void presortedExclusive();
    void exclusive();
    void presortedOverlapping();
    void overlapping();
    void presortedSameStart();
    void sameStart();
    void presortedSameStop();
    void sameStop();
    void presortedSubset();
    void subset();

private:
    QDir _calGroups;
    QString _exclusive_a;
    QString _exclusive_b;
    QString _overlapping_a;
    QString _overlapping_b;
    QString _same_start_a;
    QString _same_start_b;
    QString _same_stop_a;
    QString _same_stop_b;
    QString _subset_a;
    QString _subset_b;
};


#endif // CALSORTTEST_H
