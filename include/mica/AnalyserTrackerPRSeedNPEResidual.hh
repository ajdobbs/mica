#ifndef ANALYSERTRACKERPRSEEDNPERESIDUAL_HH
#define ANALYSERTRACKERPRSEEDNPERESIDUAL_HH

#include <vector>

#include "TVirtualPad.h"
#include "TH2.h"

#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"
#include "mica/AnalyserBase.hh"


namespace mica {

class AnalyserTrackerPRSeedNPEResidual : public AnalyserBase {
  public:
    AnalyserTrackerPRSeedNPEResidual();
    ~AnalyserTrackerPRSeedNPEResidual() {}

    virtual bool analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent);

    virtual void draw(TVirtualPad* aPad);

  private:
    std::vector<TH2D*> mHResidualsTkU;
    std::vector<TH2D*> mHResidualsTkD;
};
} // ~namespace mica


#endif
