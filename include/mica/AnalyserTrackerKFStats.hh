/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#ifndef ANALYSERTRACKERKFSTATS_HH
#define ANALYSERTRACKERKFSTATS_HH

#include <vector>

#include "TVirtualPad.h"
#include "TH2.h"

#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"
#include "mica/AnalyserBase.hh"

namespace mica {

/** @class  AnalyserTrackerKFStats
 *    Calculate the chisq per dof and p-value for kalman fit tracks and plot them
 *  @author A. Dobbs
 */
class AnalyserTrackerKFStats : public AnalyserBase {
  public:
    AnalyserTrackerKFStats();
    ~AnalyserTrackerKFStats() {}

  private:
    virtual bool analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent);
    virtual bool draw(std::shared_ptr<TVirtualPad> aPad);

    TH1D* mHChiSqTKU; ///< mHChiSqTKU Histogram for TkU circle chisq per dof
    TH1D* mHChiSqTKD; ///< mHCircleChiSqTKD Histogram for TkD circle chisq per dof
    TH1D* mHPValueTKU; ///< mHSZChiSqTKU Histogram for TkU p-value
    TH1D* mHPValueTKD; ///< mHSZChiSqTKD Histogram for TkD p-value
};
} // ~namespace mica

#endif
