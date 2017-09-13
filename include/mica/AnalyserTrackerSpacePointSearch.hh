/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#ifndef ANALYSERTRACKERSPACEPOINTSEARCH_HH
#define ANALYSERTRACKERSPACEPOINTSEARCH_HH

#include <vector>

#include "TVirtualPad.h"
#include "TH2.h"

#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"
#include "mica/AnalyserBase.hh"


namespace mica {

class AnalyserTrackerSpacePointSearch : public AnalyserBase {
  public:
    AnalyserTrackerSpacePointSearch();
    ~AnalyserTrackerSpacePointSearch() {}

    virtual bool analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent);

    virtual void draw(TVirtualPad* aPad);

  private:
    TH2D* mHSeeds;
    TH2D* mHAddOns;
};
} // ~namespace mica


#endif
