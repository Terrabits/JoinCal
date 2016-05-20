#ifndef GETCALDIALOGTEST_H
#define GETCALDIALOGTEST_H


// RsaToolbox
#include <VnaTestClass.h>

// Qt
#include <QDir>
#include <QObject>

class GetCalDialogTest : public RsaToolbox::VnaTestClass
{
    Q_OBJECT
public:
    explicit GetCalDialogTest(QObject *parent = 0);
    ~GetCalDialogTest();

private:
    QDir _calGroups;
    QString _calGroup1;
    QString _calGroup1Filename;
    QString _calGroup2;
    QString _calGroup2Filename;

    void clearCalGroups();
    void copyCalGroups();
    void makeTwoCalibratedChannels();

private slots:
    void acceptCalGroup();
    void acceptChannel();
    void rejectDialog();
    void cannotAcceptNothing();
    void noChannels();
    void noCalGroups();
    void noCalibrationsAtAll();
};

#endif // GETCALDIALOGTEST_H
