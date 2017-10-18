#ifndef ANALYSERGROUP_HH
#define ANALYSERGROUP_HH

#include <vector>
#include <memory>

#include "mica/AnalyserBase.hh"

namespace mica {

/** @class AnalyserGroup
 *         Store a group of MICA analysers in a vector, plus convenience functions.
 *  @author A. Dobbs
 */
class AnalyserGroup {
  public:
    AnalyserGroup() {};
    virtual ~AnalyserGroup() {}

    /** Return an analyser at a given position of the storage vector */
    AnalyserBase* operator [](int i) const { return mAnalysers[i]; }

    /** Add an analyser to the group */
    void AddAnalyser(AnalyserBase* aAnalyser) { mAnalysers.push_back(aAnalyser); }

    /** Call Analyse on each analyser */
    bool Analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent);

    // AnalyserGroup* Clone();

    /** Call Draw on each analyser */
    std::vector<std::shared_ptr<TVirtualPad>> Draw();

    /** Draw and save the plots to a pdf, ofname specifies the output pdf file name */
    void MakePlots(const std::string& ofname);

    /** Merge the data from another set of identical analysers into this AnalyserGroup */
    bool Merge(AnalyserGroup* aAnalyserGroup);

    size_t size() { return mAnalysers.size(); }

  private:
    std::vector<AnalyserBase*> mAnalysers;
};
} // ~namespace mice

#endif
