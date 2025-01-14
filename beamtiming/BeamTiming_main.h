#ifndef BEAMTIMING_MAIN_H
#define BEAMTIMING_MAIN_H

#include <vector>
#include <string>
#include <TString.h>

class BeamTiming {
private:
    std::vector<double> timings;
    std::vector<double> counts;
    std::vector<double> arisings;
    std::vector<double> fallings;
    int nBeams;

public:
    BeamTiming();
    int GetNbeams() const;
    std::vector<double> GetTimings() const;
    std::vector<double> GetCounts() const;
    std::vector<double> GetArisings() const;
    std::vector<double> GetFallings() const;
    bool isBeamTiming(double time);
    void GetBeamTimings(TString dir, bool figures, TString image);
};

#endif

