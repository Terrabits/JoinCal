#include "CalibrationSourceTest.h"


// Project
#include "CalibrationSource.h"


CalibrationSourceTest::CalibrationSourceTest(QObject *parent) :
    TestClass(parent)
{

}

CalibrationSourceTest::~CalibrationSourceTest()
{

}

void CalibrationSourceTest::empty() {
    CalibrationSource source;
    QVERIFY(!source.isChannel());
    QVERIFY(!source.isCalGroup());
    QVERIFY(source.isEmpty());

    source.clear();
    QVERIFY(!source.isChannel());
    QVERIFY(!source.isCalGroup());
    QVERIFY(source.isEmpty());

    source.setCalGroup("Cal Group");
    source.clear();
    QVERIFY(!source.isChannel());
    QVERIFY(!source.isCalGroup());
    QVERIFY(source.isEmpty());

    source.setChannel(5);
    source.clear();
    QVERIFY(!source.isChannel());
    QVERIFY(!source.isCalGroup());
    QVERIFY(source.isEmpty());
}
void CalibrationSourceTest::channel() {
    CalibrationSource source(2);
    QVERIFY(source.isChannel());
    QCOMPARE(source.channel(), uint(2));
    QVERIFY(!source.isCalGroup());
    QVERIFY(!source.isEmpty());

    source.clear();
    source.setChannel(3);
    QVERIFY(source.isChannel());
    QCOMPARE(source.channel(), uint(3));
    QVERIFY(!source.isCalGroup());
    QVERIFY(!source.isEmpty());

    source.setCalGroup("Cal Group");
    source.setChannel(4);
    QVERIFY(source.isChannel());
    QCOMPARE(source.channel(), uint(4));
    QVERIFY(!source.isCalGroup());
    QVERIFY(!source.isEmpty());

    source.setChannel(5);
    QVERIFY(source.isChannel());
    QCOMPARE(source.channel(), uint(5));
    QVERIFY(!source.isCalGroup());
    QVERIFY(!source.isEmpty());
}
void CalibrationSourceTest::calGroup() {
    CalibrationSource source("Cal Group 1.cal");
    QVERIFY(!source.isChannel());
    QVERIFY(source.isCalGroup());
    QCOMPARE(source.calGroup(), QString("Cal Group 1"));
    QVERIFY(!source.isEmpty());

    source.clear();
    source.setCalGroup("Cal Group 2.CAL");
    QVERIFY(!source.isChannel());
    QVERIFY(source.isCalGroup());
    QCOMPARE(source.calGroup(), QString("Cal Group 2"));
    QVERIFY(!source.isEmpty());

    source.setChannel(4);
    source.setCalGroup("Cal Group 3");
    QVERIFY(!source.isChannel());
    QVERIFY(source.isCalGroup());
    QCOMPARE(source.calGroup(), QString("Cal Group 3"));
    QVERIFY(!source.isEmpty());

    source.setCalGroup("Cal Group 4");
    QVERIFY(!source.isChannel());
    QVERIFY(source.isCalGroup());
    QCOMPARE(source.calGroup(), QString("Cal Group 4"));
    QVERIFY(!source.isEmpty());
}
void CalibrationSourceTest::compareEqual() {
    CalibrationSource source1;
    CalibrationSource source2;

    source1.clear();
    source2.clear();
    QVERIFY(source1 == source2);

    source1.setChannel(4);
    QVERIFY(!(source1 == source2));

    source2.setChannel(5);
    QVERIFY(!(source1 == source2));

    source2.setChannel(4);
    QVERIFY(source1 == source2);

    source1.setCalGroup("My Cal Group");
    QVERIFY(!(source1 == source2));

    source2.setCalGroup("Cal Group 2");
    QVERIFY(!(source1 == source2));

    source2.setCalGroup("My Cal Group");
    QVERIFY(source1 == source2);

    source1.clear();
    QVERIFY(!(source1 == source2));
}
void CalibrationSourceTest::compareNotEqual() {
    CalibrationSource source1;
    CalibrationSource source2;

    source1.clear();
    source2.clear();
    QVERIFY(!(source1 != source2));

    source1.setChannel(4);
    QVERIFY(source1 != source2);

    source2.setChannel(5);
    QVERIFY(source1 != source2);

    source2.setChannel(4);
    QVERIFY(!(source1 != source2));

    source1.setCalGroup("My Cal Group");
    QVERIFY(source1 != source2);

    source2.setCalGroup("Cal Group 2");
    QVERIFY(source1 != source2);

    source2.setCalGroup("My Cal Group");
    QVERIFY(!(source1 != source2));

    source1.clear();
    QVERIFY(source1 != source2);
}
void CalibrationSourceTest::copy() {
    CalibrationSource empty;
    CalibrationSource calGroup("Cal Group");
    CalibrationSource channel(1);

    QCOMPARE(CalibrationSource(empty), empty);
    QCOMPARE(CalibrationSource(calGroup), calGroup);
    QCOMPARE(CalibrationSource(channel), channel);

    CalibrationSource source;
    QCOMPARE(source, empty);
    source = calGroup;
    QCOMPARE(source, calGroup);
    source = channel;
    QCOMPARE(source, channel);
    source = empty;
    QCOMPARE(source, empty);
}
void CalibrationSourceTest::displayText() {
    CalibrationSource source;
    QCOMPARE(source.displayText(), QString());

    source.setCalGroup("Cal Group name");
    QCOMPARE(source.displayText(), QString("Cal Group name"));

    source.setChannel(5);
    QCOMPARE(source.displayText(), QString("Channel 5"));
}
