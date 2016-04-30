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
    void fullCorrections();
    void subsection();
};

#endif // CORRECTIONSTEST_H
