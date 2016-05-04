#include "CalibrationSource.h"


CalibrationSource::CalibrationSource() :
    _channel(0)
{
    //
}
CalibrationSource::CalibrationSource(uint channel) :
    _channel(channel)
{
    //
}
CalibrationSource::CalibrationSource(QString calGroup) :
    _channel(0)
{
    setCalGroup(calGroup);
}

CalibrationSource::~CalibrationSource()
{
    //
}

bool CalibrationSource::isEmpty() const {
    return !_channel && _calGroup.isEmpty();
}

bool CalibrationSource::isChannel() const {
    return _channel;
}
uint CalibrationSource::channel() const {
    return _channel;
}
void CalibrationSource::setChannel(uint index) {
    clear();
    _channel = index;
}

bool CalibrationSource::isCalGroup() const {
    return !_calGroup.isEmpty();
}
QString CalibrationSource::calGroup() const {
    return _calGroup;
}
void CalibrationSource::setCalGroup(QString calGroup) {
    clear();

    if (calGroup.endsWith(".cal", Qt::CaseInsensitive))
        calGroup.chop(4);
    _calGroup = calGroup;
}

void CalibrationSource::clear() {
    _channel = 0;
    _calGroup.clear();
}

QString CalibrationSource::displayText() const {
    if (isCalGroup()) {
        return calGroup();
    }
    if (isChannel()) {
        return QString("Channel %1").arg(channel());
    }

    return QString();
}

bool operator!=(const CalibrationSource &left, const CalibrationSource &right) {
    if (left.isEmpty() != right.isEmpty())
        return true;
    if (left.isEmpty())
        return false;

    if (left.isChannel() != right.isChannel())
        return true;

    if (left.isChannel()) {
        if (left.channel() != right.channel())
            return true;
    }
    else if (left.isCalGroup()) {
        const QString leftGroup  =  left.calGroup().toLower();
        const QString rightGroup = right.calGroup().toLower();
        if (leftGroup != rightGroup)
            return true;
    }

    return false;
}
bool operator==(const CalibrationSource &left, const CalibrationSource &right) {
    return !(left != right);
}
