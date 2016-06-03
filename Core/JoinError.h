#ifndef JOINERROR_H
#define JOINERROR_H


// Qt
#include <QString>

class JoinError
{
public:
    JoinError();
    ~JoinError();

    enum /*class*/ Code {
        Calibration1,
        Calibration2,
        FrequencyRange,
        Ports,
        Filename
    };

    Code code;
    uint index; // of calibration
    QString msg;
};

#endif // JOINERROR_H
