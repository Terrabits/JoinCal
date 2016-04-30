#ifndef FREQUENCYRANGE_H
#define FREQUENCYRANGE_H


class FrequencyRange
{
public:
    FrequencyRange();
    ~FrequencyRange();

    bool   isStopInclusive() const;
    void   setStopInclusive(bool isStopInclusive);

    double start_Hz() const;
    void   setStart(double frequency_Hz);

    double stop_Hz() const;
    void   setStop(double frequency_Hz);

    bool   isEmpty() const;
    void   clear();

private:
    bool  _isStopInclusive;
    double _start_Hz;
    double _stop_Hz;
};

#endif // FREQUENCYRANGE_H
