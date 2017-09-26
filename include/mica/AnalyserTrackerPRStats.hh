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

    /** @brief Analyse the given event
     *  @param aReconEvent The recon event
     *  @param aMCEvent The corresponding MC event
     *  @return Boolean indicating if the cuts passed and the analysis happened
     */
    virtual bool analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent);

    /** @brief After analysing all the events, draw the results
     *  @param aPad ROOT TPad to draw results on
     */
    virtual void draw(std::shared_ptr<TVirtualPad> aPad);

  private:
    TH1D* mHCircleChiSqTKU; ///< mHCircleChiSqTKU Histogram for TkU circle chisq per dof
    TH1D* mHCircleChiSqTKD; ///< mHCircleChiSqTKD Histogram for TkD circle chisq per dof
    TH1D* mHSZChiSqTKU; ///< mHSZChiSqTKU Histogram for TkU sz chisq per dof
    TH1D* mHSZChiSqTKD; ///< mHSZChiSqTKD Histogram for TkD sz chisq per dof
};
} // ~namespace mica

#endif
