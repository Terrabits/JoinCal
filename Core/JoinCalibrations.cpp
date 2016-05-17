#include "JoinCalibrations.h"


// Project
//

// RsaToolbox
#include <VnaChannel.h>
using namespace RsaToolbox;

// Qt
#include <QSharedPointer>


// Assumes:
// - sections are valid
// - sections are sorted
//   low to high frequency
typedef QSharedPointer<Corrections> SharedCorrections;
JoinCalibrations::JoinCalibrations(const QVector<Corrections*> &corrections,
                                   const QVector<uint> &ports,
                                   RsaToolbox::Vna *vna,
                                   QString saveAs)
{
    QRowVector frequencies_Hz;
    for (int i = 0; i < corrections.size(); i++) {
        frequencies_Hz += corrections[i]->frequencies_Hz();
    }

    uint c = vna->createChannel();
    VnaChannel channel = vna->channel(c);
    if (channel.isCalGroup())
        channel.dissolveCalGroupLink();
    channel.setFrequencies(frequencies_Hz);

    VnaCorrections newCorrections = channel.corrections();
    channel.calibrate().start("JoinCal", VnaCalibrate::CalType::Tosm, ports);
    newCorrections.loadDefaultCorrections();

    foreach (int p1, ports) {
        foreach (int p2, ports) {
            ComplexRowVector directivity;
            ComplexRowVector sourceMatch;
            ComplexRowVector reflectionTracking;
            ComplexRowVector loadMatch;
            ComplexRowVector transmissionTracking;
            for (int i = 0; i < corrections.size(); i++) {
                append(directivity,              corrections[i]->directivity(p1, p2));
                append(sourceMatch,              corrections[i]->sourceMatch(p1, p2));
                append(reflectionTracking,       corrections[i]->reflectionTracking(p1, p2));
                if (p2 != p1) {
                    append(loadMatch,            corrections[i]->loadMatch(p1, p2));
                    append(transmissionTracking, corrections[i]->transmissionTracking(p1 ,p2));
                }
            }
            newCorrections.setDirectivity(directivity, p1, p2);
            newCorrections.setSourceMatch(sourceMatch, p1, p2);
            newCorrections.setReflectionTracking(reflectionTracking, p1 ,p2);
            if (p2 != p1) {
                newCorrections.setLoadMatch(loadMatch, p1, p2);
                newCorrections.setTransmissionTracking(transmissionTracking, p1, p2);
            }
        }
    }

    channel.saveCalibration(saveAs);
    channel.dissolveCalGroupLink();
    channel.setCalGroup(saveAs);
    vna->deleteChannel(c);
}

JoinCalibrations::~JoinCalibrations()
{

}

void JoinCalibrations::append(ComplexRowVector &vector, const ComplexRowVector &values) {
    vector.insert(vector.end(), values.begin(), values.end());
}
