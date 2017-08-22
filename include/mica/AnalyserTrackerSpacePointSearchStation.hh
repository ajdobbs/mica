#ifndef ANALYSERTRACKERSPACEPOINTSEARCHSTATION_HH
#define ANALYSERTRACKERSPACEPOINTSEARCHSTTAION_HH

#include <vector>

#include "TVirtualPad.h"
#include "TH2.h"

#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"
#include "mica/AnalyserBase.hh"


namespace mica {

class AnalyserTrackerSpacePointSearchStation : public AnalyserBase {
  public:
    AnalyserTrackerSpacePointSearchStation();
    ~AnalyserTrackerSpacePointSearchStation() {}

    virtual bool analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent);

    virtual void draw(TVirtualPad* aPad);

  private:
    std::vector<TH2D*> mHSeeds;
    std::vector<TH2D*> mHAddOns;
};
} // ~namespace mica


#endif
