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

/** @class  AnalyserTrackerPRStats
 *    Calculate the chisq per dof for patrec tracks and plot them
 *  @author A. Dobbs
 */
class AnalyserTrackerPRStats : public AnalyserBase {
  public:
    AnalyserTrackerPRStats();
    ~AnalyserTrackerPRStats() {}

  private:
    virtual bool analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent) override;
    virtual bool draw(std::shared_ptr<TVirtualPad> aPad) override;

    TH1D* mHCircleChiSqTKU; ///< mHCircleChiSqTKU Histogram for TkU circle chisq per dof
    TH1D* mHCircleChiSqTKD; ///< mHCircleChiSqTKD Histogram for TkD circle chisq per dof
    TH1D* mHSZChiSqTKU; ///< mHSZChiSqTKU Histogram for TkU sz chisq per dof
    TH1D* mHSZChiSqTKD; ///< mHSZChiSqTKD Histogram for TkD sz chisq per dof
};
} // ~namespace mica

#endif
