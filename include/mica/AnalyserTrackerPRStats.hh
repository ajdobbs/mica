/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#ifndef ANALYSERTRACKERPRSTATS_HH
#define ANALYSERTRACKERPRSTATS_HH

#include <vector>

#include "TVirtualPad.h"
#include "TH2.h"

#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"
#include "mica/AnalyserBase.hh"


namespace mica {

class AnalyserTrackerPRStats : public AnalyserBase {
  public:
    AnalyserTrackerPRStats();
    ~AnalyserTrackerPRStats() {}

    virtual bool analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent);

    virtual void draw(TVirtualPad* aPad);

  private:
    TH1D* mHCircleChiSqTKU;
    TH1D* mHCircleChiSqTKD;
    TH1D* mHSZChiSqTKU;
    TH1D* mHSZChiSqTKD;
};
} // ~namespace mica


#endif
