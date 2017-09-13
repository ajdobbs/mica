/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#ifndef ANALYSERTRACKERPRSEEDRESIDUAL_HH
#define ANALYSERTRACKERPRSEEDRESIDUAL_HH

#include <vector>

#include "TVirtualPad.h"
#include "TH1.h"

#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"
#include "mica/AnalyserBase.hh"


namespace mica {

class AnalyserTrackerPRSeedResidual : public AnalyserBase {
  public:
    AnalyserTrackerPRSeedResidual();
    ~AnalyserTrackerPRSeedResidual() {}

    virtual bool analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent);

    virtual void draw(TVirtualPad* aPad);

    bool getLogScale() const { return mLogScale; }
    void setLogScale(bool aLogScale) { mLogScale = aLogScale; }

  private:
    bool mLogScale;
    std::vector<TH1D*> mHResidualsTkU;
    std::vector<TH1D*> mHResidualsTkD;
};
} // ~namespace mica


#endif
