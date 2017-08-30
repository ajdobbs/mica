#ifndef ANALYSERGROUP_HH
#define ANALYSERGROUP_HH

#include <vector>

#include "mica/AnalyserBase.hh"

namespace mica {

class AnalyserGroup {
  public:
    AnalyserGroup() {};
    virtual ~AnalyserGroup() {}

    void AddAnalyser(AnalyserBase* aAnalyser) { mAnalysers.push_back(aAnalyser); }
    std::vector<AnalyserBase*> GetAnalysers() const { return mAnalysers; }
    void SetAnalysers(std::vector<AnalyserBase*>& aAnalysers) { mAnalysers = aAnalysers; }

    bool Analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent);

    // AnalyserGroup* Clone();

    std::vector<TVirtualPad*> Draw();

    bool Merge(AnalyserGroup* aAnalyserGroup);

  private:
    std::vector<AnalyserBase*> mAnalysers;

};
} // ~namespace mice

#endif
