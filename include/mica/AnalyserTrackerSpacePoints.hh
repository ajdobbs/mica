#ifndef ANALYSERTRACKERSPACEPOINTS_HH
#define ANALYSERTRACKERSPACEPOINTS_HH

#include <vector>

#include "TVirtualPad.h"
#include "TH2.h"

#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"
#include "mica/AnalyserBase.hh"


namespace mica {

class AnalyserTrackerSpacePoints : public AnalyserBase {
  public:
    AnalyserTrackerSpacePoints();
    virtual ~AnalyserTrackerSpacePoints() {}

    virtual bool analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent);

    virtual void draw(TVirtualPad* aPad);

  private:
    TH1D* mHNpeTKU;
    TH1D* mHNpeTKD;
    TH1D* mHStationNumTKU;
    TH1D* mHStationNumTKD;
    TH2D* mHPositionTKU;
    TH2D* mHPositionTKD;
};
} // ~namespace mica


#endif
