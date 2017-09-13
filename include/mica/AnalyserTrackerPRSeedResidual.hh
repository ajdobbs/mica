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

    /** @brief Return if plots should be a log scale on y axis */
    bool getLogScale() const { return mLogScale; }

    /** @brief Set if plots should be a log scale on y axis */
    void setLogScale(bool aLogScale) { mLogScale = aLogScale; }

  private:
    bool mLogScale; ///< Should plots be a log scale on y axis
    std::vector<TH1D*> mHResidualsTkU; ///< TkU residuals of seeds from fit
    std::vector<TH1D*> mHResidualsTkD; ///< TkD residuals of seeds from fit
};
} // ~namespace mica

#endif
