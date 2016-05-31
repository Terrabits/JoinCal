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
    explicit GetCalDialogTest(RsaToolbox::ConnectionType type, const QString &address, QObject *parent = 0);
    ~GetCalDialogTest();

private:
    QDir _calGroups;
    QString _calGroup1;
    QString _calGroup1Filename;
    QString _calGroup2;
    QString _calGroup2Filename;

    void clearCalGroups();
    void copyCalGroups();
    void copyManyCalGroups();
    void makeTwoCalibratedChannels();

private slots:
    virtual void initTestCase();

    virtual void init();
    virtual void cleanup();

    void acceptCalGroup();
    void acceptChannel();
    void rejectDialog();
    void cannotAcceptNothing();
    void noChannels();
    void noCalGroups();
    void noCalibrationsAtAll();
    void manyCalGroups();
};


#endif // GETCALDIALOGTEST_H
